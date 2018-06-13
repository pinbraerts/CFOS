#include "CFSystem.h"

LRESULT CALLBACK CFSystem::WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam) {
	CFSystem* sys = (CFSystem*)GetWindowLongPtr(hWnd, GWL_USERDATA);
	if (sys == nullptr) {
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		if (cs != nullptr) {
			sys = (CFSystem*)cs->lpCreateParams;
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)sys);
		}
		else return DefWindowProc(hWnd, message, wParam, lParam);
	}

	switch (message) {
	case WM_SIZE:
		sys->resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd, nullptr, false);
		break;
	case WM_PAINT:
		sys->draw();
		ValidateRect(hWnd, nullptr);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		// sys->mouseMove();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

HRESULT CFSystem::createDeviceIndependentResources() {
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &directFactory);
	if (FAILED(hr)) return hr;
	return CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&imagingFactory)
	);
}

HRESULT CFSystem::createDeviceResources() {
	if (renderTarget != nullptr)
		return S_OK;

	RECT rc;
	GetClientRect(window, &rc);
	HRESULT hr = directFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&renderTarget
	);
	if (FAILED(hr)) return hr;

	for (auto i : applications) {
		hr = i.get().createDeviceResources();
		if (FAILED(hr)) return hr;
	}

	return hr;
}

void CFSystem::discardDeviceResources() {
	release(renderTarget);
	for (auto i : applications)
		i.get().discardDeviceResources();
}

CFSystem::CFSystem(HINSTANCE hInst) : CFApplication(*this), WNDCLASSEX{
	sizeof(WNDCLASSEX),
	CS_HREDRAW | CS_VREDRAW,
	WndProc,
	0,
	sizeof(this),
	hInst,
	nullptr,
	LoadCursor(NULL, IDC_ARROW),
	nullptr,
	0,
	WndClassName,
	nullptr,
} {
	if (FAILED(createDeviceIndependentResources()))
		throw std::runtime_error("Call to CreateDeviceIndependentResources failed!");

	if (!RegisterClassEx(this))
		throw std::runtime_error("Call to RegisterCLassEx failed!");
}

void CFSystem::createWindow(int nCmdShow) {
	window = CreateWindow(
		WndClassName,
		WndTitle,
		WS_POPUP,
		500, 10,
		450, 900,
		nullptr,
		nullptr,
		hInstance,
		this
	);
	if (window == nullptr)
		throw std::runtime_error("Call to CreateWindow failed!");
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);
}

int CFSystem::run() {
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void CFSystem::resize(UINT width, UINT height) {
	if (renderTarget != nullptr)
		renderTarget->Resize(D2D1::SizeU(width, height));
}

void CFSystem::draw() {
	HRESULT hr = createDeviceResources();

	renderTarget->BeginDraw();
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	for (auto i : applications)
		i.get().draw();

	hr = renderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
		discardDeviceResources();
	else if (FAILED(hr))
		throw std::runtime_error("Failed render!");
}

HRESULT CFSystem::loadBitmap(LPCWSTR uri, ID2D1Bitmap *& bmp, UINT dWidth, UINT dHeight) {
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* src = nullptr;
	IWICFormatConverter* converter = nullptr;
	IWICBitmapScaler* scaler = nullptr;
	IWICBitmapSource* res = nullptr;

	HRESULT hr = imagingFactory->CreateDecoderFromFilename(
		uri, nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder
	);
	if (FAILED(hr)) goto cleanup;

	hr = decoder->GetFrame(0, &src);
	if (FAILED(hr)) goto cleanup;
	res = src;

	WICPixelFormatGUID pf;
	res->GetPixelFormat(&pf);
	if (pf != GUID_WICPixelFormat32bppPBGRA) {
		hr = imagingFactory->CreateFormatConverter(&converter);
		if (FAILED(hr)) goto cleanup;
		hr = converter->Initialize(
			res,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
		if (FAILED(hr)) goto cleanup;
		res = converter;
	}

	UINT w, h;
	res->GetSize(&w, &h);
	if ((dWidth != w || dHeight != h) && (dWidth != 0 && dHeight != 0)) {
		if (dWidth == 0)
			dWidth = w * dHeight / h;
		else if (dHeight == 0)
			dHeight = h * dWidth / w;
		hr = imagingFactory->CreateBitmapScaler(&scaler);
		if (FAILED(hr)) goto cleanup;
		hr = scaler->Initialize(res, dWidth, dHeight, WICBitmapInterpolationModeLinear);
		if (FAILED(hr)) return hr;
		res = scaler;
	}

	renderTarget->CreateBitmapFromWicBitmap(res, &bmp);

cleanup:
	release(decoder);
	release(src);
	release(converter);
	release(scaler);

	return hr;
}

CFSystem::~CFSystem() {
	discardDeviceResources();
	release(directFactory);
	release(imagingFactory);
}
