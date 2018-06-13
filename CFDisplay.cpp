#include "CFDisplay.h"
#include "CFSystem.h"

void CFDisplay::createWindow(CFSystem& s, int nCmdShow) {
	handle = CreateWindow(
		CFSystem::WndClassName,
		CFSystem::WndTitle,
		WS_POPUP,
		x, y,
		width, height,
		nullptr, nullptr,
		s.hInstance, &s
	);
	if (handle == nullptr)
		throw std::runtime_error("Call to CreateWindow failed!");
	ShowWindow(handle, nCmdShow);
	UpdateWindow(handle);
}
