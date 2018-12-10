
#pragma once

#include <cocos2d.h>
#include "ui/CocosGUI.h"

namespace widget
{

class ListViewEx
	:public cocos2d::ui::ListView
{
public:
	static ListViewEx* create();

};
}

