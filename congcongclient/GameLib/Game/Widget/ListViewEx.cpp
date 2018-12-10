#include "ListViewEx.h"

using namespace widget;

ListViewEx* ListViewEx::create()
{
	ListViewEx* widget = new (std::nothrow) ListViewEx();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}
