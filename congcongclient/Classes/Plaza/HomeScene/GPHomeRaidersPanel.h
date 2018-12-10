#pragma once

#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

class GPHomeRaidersPanel
	: public cocos2d::Node
	, public FvSingleton<GPHomeRaidersPanel>
{
public:
	GPHomeRaidersPanel();
	~GPHomeRaidersPanel();
public:
	bool init();
	void initButton();
	void show();
	void hide();
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
};
