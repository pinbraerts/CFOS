#ifndef CFSYSTEM_H
#define CFSYSTEM_H

#include "CFApplication.h"

class CFSystem : public CFApplication, public WNDCLASSEX {
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

	HRESULT loadBitmap(LPCWSTR uri, ID2D1Bitmap*& bmp, UINT dWidth = 0, UINT dHeight = 0);
	inline HRESULT loadBitmap(std::wstring uri, ID2D1Bitmap*& bmp, UINT dWidth = 0, UINT dHeight = 0) {
		return loadBitmap(uri.c_str(), bmp, dWidth = 0, dHeight = 0);
	}

	std::vector<std::reference_wrapper<CFApplication>> applications;

	~CFSystem();
};

#endif // !CFSYSTEM_H
