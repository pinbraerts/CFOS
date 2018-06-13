#include "CFSysUI.h"
#include "CFSystem.h"

void CFSysUI::load() {
	if (sys.display.renderTarget != nullptr)
		return;

	HRESULT hr = sys.directFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			sys.display.handle,
			D2D1::SizeU((unsigned)sys.display.width, (unsigned)sys.display.height)
		),
		&sys.display.renderTarget
	);
	CHECK;

	CFWidgetHolder::load();
}

void CFSysUI::unload() {
	release(sys.display.renderTarget);
	CFWidgetHolder::unload();
}

void CFSysUI::draw() {
	load();

	target.BeginDraw();
	target.SetTransform(D2D1::Matrix3x2F::Identity());
	target.Clear(D2D1::ColorF(D2D1::ColorF::White));

	CFWidgetHolder::draw();

	HRESULT hr;
	hr = target.EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
		unload();
	else CHECK;
}
