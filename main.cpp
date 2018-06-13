#include "CFOS.h"

int CALLBACK _tWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInt,
	LPTSTR lpCmdLine,
	int nCmdShow
) {
	int res;
	CoInitialize(nullptr);
	try {
		CFSystem sys(hInst);

		Launcher l{ sys };
		sys.applications.emplace_back(l);

		sys.createWindow(nCmdShow);
		res = sys.run();
	}
	catch (const std::runtime_error& e) {
		MessageBoxA(nullptr, e.what(), "CFAST", 0);
		res = 1;
	}
	CoUninitialize();
	return res;
}
