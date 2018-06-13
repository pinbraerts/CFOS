#include "CFSysUI.h"

void CFSysUI::load() {
	if (sys.renderTarget != nullptr)
		return;

	RECT rc;
	GetClientRect(sys.window, &rc);
	HRESULT hr = sys.directFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(sys.window, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		(ID2D1HwndRenderTarget**)&sys.renderTarget
	);
	CHECK;

	CFWidgetHolder::load();
}

void CFSysUI::unload() {
	release(sys.renderTarget);
	CFWidgetHolder::unload();
}

void CFSysUI::draw() {
	HRESULT hr;
	load();

	sys.renderTarget->BeginDraw();
	sys.renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	sys.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	CFWidgetHolder::draw();

	hr = sys.renderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
		unload();
	else CHECK;
}
