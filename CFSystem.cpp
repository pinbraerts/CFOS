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
		sys->ui->draw();
		ValidateRect(hWnd, nullptr);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		sys->mouseMove(LOWORD(lParam), HIWORD(lParam));
		InvalidateRect(hWnd, nullptr, false);
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

CFSystem::CFSystem(HINSTANCE hInst) : WNDCLASSEX{
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
		((ID2D1HwndRenderTarget*)renderTarget)->Resize(D2D1::SizeU(width, height));
}

void CFSystem::mouseMove(UINT x, UINT y) {
	mouseMoveEvent((float)x, (float)y);
}

CFSystem::~CFSystem() {
	ui->unload();
	release(directFactory);
	release(imagingFactory);
}
