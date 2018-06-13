#ifndef CFSYSTEM_H
#define CFSYSTEM_H

#include "CFDisplay.h"
#include "CFWidget.h"
#include "CFSysUI.h"

class CFSystem : public WNDCLASSEX {
public:
	static constexpr TCHAR WndClassName[] = _T("CF");
	static constexpr TCHAR WndTitle[] = _T("CF");

	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);

	void createDeviceIndependentResources();
	void createWindow(bool nCmdShow);

	CFSystem(HINSTANCE hInst);

	int run();
	void mouseMove(UINT x, UINT y);

	ID2D1Factory* directFactory;
	IWICImagingFactory* imagingFactory;

	CFSysUI ui;
	CFDisplay display;

	Event<float, float> mouseMoveEvent;

	~CFSystem();
};

#endif // !CFSYSTEM_H
