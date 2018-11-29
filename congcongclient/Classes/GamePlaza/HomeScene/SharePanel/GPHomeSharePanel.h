#pragma once

#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

class GPHomeSharePanel
	: public cocos2d::Node
	, public FvSingleton<GPHomeSharePanel>
{
public:
	GPHomeSharePanel();
	~GPHomeSharePanel();
public:
	bool init();
	void initButton();

	void show();
	void hide();

	void Button_ShareToFriend(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ShareToFriendCircle(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
};