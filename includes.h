#ifndef INCLUDES_H
#define INCLUDES_H

#include <tchar.h>
#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1helper.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1")

#include <stdexcept>
#include <string>

#include "Event.h"

template<class T>
void release(T*& i) {
	if (i != nullptr) {
		i->Release();
		i = nullptr;
	}
}

class CFSystem;
struct CFApplication;

#endif // !INCLUDES_H
