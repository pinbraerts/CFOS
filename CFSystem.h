#ifndef CFSYSTEM_H
#define CFSYSTEM_H

#include "CFWidget.h"

class CFSystem : public CFWidget, public WNDCLASSEX {
private:
	static constexpr TCHAR WndClassName[] = _T("CF");
	static constexpr TCHAR WndTitle[] = _T("CF");

	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);

	ID2D1Factory* directFactory;

public:
	HWND window;
	ID2D1HwndRenderTarget* renderTarget;
	IWICImagingFactory* imagingFactory;

	HRESULT createDeviceIndependentResources();
	HRESULT createDeviceResources() override;
	void discardDeviceResources() override;

	CFSystem(HINSTANCE hInst);

	void createWindow(int nCmdShow);
	int run();
	void resize(UINT width, UINT height);
	void draw() override;

	std::vector<std::reference_wrapper<CFWidget>> applications;

	~CFSystem();
};

#endif // !CFSYSTEM_H
