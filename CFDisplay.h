#ifndef CFDISPLAY_H
#define CFDISPLAY_H

#include "includes.h"

class CFDisplay {
public:
	HWND handle;
	ID2D1HwndRenderTarget* renderTarget;

	static constexpr float WIDTH = 450, HEIGHT = 900;
	float width = WIDTH, height = HEIGHT;

	static constexpr float X = 100, Y = 100;
	float x = X, y = Y;

	inline float centerX() const { return width / 2; }
	inline float centerY() const { return height / 2; }
	inline D2D1_RECT_F clientRect() const { return D2D1::RectF(0, 0, width, height); }

	void createWindow(CFSystem& s, int nCmdShow);
};

#endif // !CFDISPLAY_H
