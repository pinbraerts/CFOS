#ifndef INCLUDES_H
#define INCLUDES_H

#include <tchar.h>
#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1helper.h>
#include <wincodec.h>
#include <comdef.h>
#pragma comment(lib, "d2d1")

#include <stdexcept>
#include <string>

#include "Event.h"

#define CHECK if(FAILED(hr)) throw std::runtime_error(_com_error(hr).ErrorMessage())

template<class T>
void release(T*& i) {
	if (i != nullptr) {
		i->Release();
		i = nullptr;
	}
}

struct CFContext;
class CFSystem;
class CFSysUI;

struct CFWidget;
struct CFWidgetHolder;

#endif // !INCLUDES_H
