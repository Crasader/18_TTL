#pragma once

#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

class GPHomeExitPanel
	: public cocos2d::Node
	, public FvSingleton<GPHomeExitPanel>
{
public:
	GPHomeExitPanel();
	~GPHomeExitPanel();
public:
	bool init();
	void hide();
	void show();
	void Button_Cancel(cocos2d::Ref*, WidgetUserInfo *);
	void Button_Comfirm(cocos2d::Ref*, WidgetUserInfo *);
};