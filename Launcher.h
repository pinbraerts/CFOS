#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "CFSystem.h"
#include "Image.h"

class Launcher: public CFWidget {
private:
	ID2D1SolidColorBrush* b = nullptr;
	ID2D1Bitmap* bmp;

public:
	float page = 0;

	Launcher(CFSystem& s) : CFWidget(s) {
		sys.mouseMoveEvent += [this](float x, float y) {
			onMouse(x - sys.display.centerX(), y - sys.display.centerY());
		};
	}

	void onMouse(float x, float y) {
		page = x;
	}

	void load() override {
		bmp = ImageBuilder(L"Background.jpg", sys.imagingFactory)
			.convert()
			.scale(0, sys.display.height, WICBitmapInterpolationModeHighQualityCubic)
			.to(target);
		HRESULT hr = target.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SeaGreen), &b);
		CHECK;
	}

	void unload() override {
		release(b);
		release(bmp);
	}

	void draw() override {
		auto s = bmp->GetSize();
		auto rect = D2D1::RectF(
			(s.width - sys.display.width) / 2 + page, 0,
			(s.width + sys.display.width) / 2 + page, s.height
		);
		target.DrawBitmap(
			bmp,
			sys.display.clientRect(),
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			&rect
		);
		target.DrawRectangle(D2D1::RectF(10, 10, 50, 50), b);
	}
};

#endif // !LAUNCHER_H
