#ifndef CFWIDGET_H
#define CFWIDGET_H

#include "includes.h"

struct CFWidget {
	CFSystem& sys;

	CFWidget(CFSystem& s) : sys(s) { }

	virtual void load() = 0;
	virtual void unload() = 0;
	virtual void draw() = 0;
};

struct CFWidgetHolder: CFWidget {
	using CFWidget::CFWidget;

	void load() override {
		for (auto& child : children)
			child.get().load();
	}
	void unload() override {
		for (auto& child : children)
			child.get().unload();
	}
	void draw() override {
		for (auto& child : children)
			child.get().draw();
	}

	std::vector<std::reference_wrapper<CFWidget>> children;
};

#endif // !CFWIDGET_H
