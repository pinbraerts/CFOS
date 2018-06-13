#ifndef CFSYSUI_H
#define CFSYSUI_H

#include "CFWidget.h"

class CFSysUI: public CFWidgetHolder {
public:
	using CFWidgetHolder::CFWidgetHolder;

	void load() override;
	void unload() override;
	void draw() override;
};

#endif // !CFSYSUI_H
