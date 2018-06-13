#ifndef CFAPPLICATION_H
#define CFAPPLICATION_H

#include "includes.h"

struct CFApplication {
	CFSystem& sys;

	CFApplication(CFSystem& s) : sys(s) { }

	virtual HRESULT createDeviceResources() = 0;
	virtual void discardDeviceResources() = 0;
	virtual void draw() = 0;
};

#endif // !CFAPPLICATION_H
