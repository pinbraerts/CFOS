#ifndef CFSYSTEM_H
#define CFSYSTEM_H

#include "CFContext.h"
#include "CFWidget.h"
#include "CFSysUI.h"

class CFSystem : public WNDCLASSEX, public CFContext {
private:
	static constexpr TCHAR WndClassName[] = _T("CF");
	static constexpr TCHAR WndTitle[] = _T("CF");

	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);

public:
	HRESULT createDeviceIndependentResources();

	CFSystem(HINSTANCE hInst);

	void createWindow(int nCmdShow);
	int run();
	void resize(UINT width, UINT height);
	void mouseMove(UINT x, UINT y);

	ID2D1Factory* directFactory;
	CFSysUI* ui;

	Event<float, float> mouseMoveEvent;

	~CFSystem();
};

#endif // !CFSYSTEM_H
