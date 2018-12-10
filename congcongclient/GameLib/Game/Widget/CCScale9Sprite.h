#pragma once

#include <cocos2d.h>
#include "ui/UIScale9Sprite.h"

USING_NS_CC;

namespace widget
{

class Scale9SpriteEx
	:public ui::Scale9Sprite
{
public:
    static Scale9SpriteEx* create(const std::string& file);

};

}

