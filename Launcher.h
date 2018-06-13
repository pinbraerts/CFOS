#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "CFSystem.h"
#include "Image.h"

class Launcher: public CFWidget {
private:
	ID2D1SolidColorBrush* b = nullptr;
	ID2D1Bitmap* bmp;

public:
	using CFWidget::CFWidget;

	HRESULT createDeviceResources() override {
		HRESULT hr;
		hr = ImageBuilder(L"Background.jpg", sys.imagingFactory).convert().to(sys.renderTarget, bmp);
		if (FAILED(hr)) return hr;
		hr = sys.renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SeaGreen), &b);
		return hr;
	}

	void discardDeviceResources() override { release(b); }

	void draw() override {
		auto r = D2D1::RectF(0, 0, 450, 900);
		sys.renderTarget->DrawBitmap(bmp, r, 1);
		sys.renderTarget->DrawRectangle(D2D1::RectF(10, 10, 50, 50), b);
	}
};

#endif // !LAUNCHER_H
