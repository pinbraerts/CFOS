#ifndef CFCONTEXT_H
#define CFCONTEXT_H

#include "includes.h"

struct CFContext {
	HWND window;
	ID2D1RenderTarget* renderTarget = nullptr;
	IWICImagingFactory* imagingFactory = nullptr;
};

#endif // !CFCONTEXT_H
