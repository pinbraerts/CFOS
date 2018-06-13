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
	float deltaPage = 10;

	Launcher(CFSystem& s) : CFWidget(s) {
		sys.mouseMoveEvent += [this](float x, float y) { onMouse(x, y); };
	}

	void onMouse(float x, float y) {
		RECT r;
		GetClientRect(sys.window, &r);
		x -= (r.left + r.right) / 2;
		page = x / deltaPage;
	}

	void load() override {
		bmp = ImageBuilder(L"Background.jpg", sys.imagingFactory)
			.convert()
			.scale(0, 900, WICBitmapInterpolationModeHighQualityCubic)
			.to(sys.renderTarget);
		HRESULT hr = sys.renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SeaGreen), &b);
		CHECK;
	}

	void unload() override {
		release(b);
		release(bmp);
	}

	void draw() override {
		RECT r;
		GetClientRect(sys.window, &r);
		float w = (float)(r.right - r.left), h = (float)(r.bottom - r.top);
		auto s = bmp->GetSize();
		sys.renderTarget->DrawBitmap(
			bmp,
			D2D1::RectF(r.left, r.top, r.right, r.bottom),
			1,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF((s.width - w) / 2 + page * deltaPage, 0, (s.width + w) / 2 + page * deltaPage, s.height));
		sys.renderTarget->DrawRectangle(D2D1::RectF(10, 10, 50, 50), b);
	}
};

#endif // !LAUNCHER_H
