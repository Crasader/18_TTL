#pragma once
#include "common.h"
#include "Game/Game/UserInfo.h"

class GPHomeServicePanel
	:public cocos2d::Node
	, public FvSingleton<GPHomeServicePanel>
{
public:
	GPHomeServicePanel();
	~GPHomeServicePanel();
public:
	bool init();
public:
	void initLayout();
	void initButton();
public:
	void show();
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
};