#pragma once

#include "common.h"

class GPHomeMorePanel
	: public cocos2d::Node
	, public FvSingleton<GPHomeMorePanel>
{
public:
	GPHomeMorePanel();
	~GPHomeMorePanel();
public:
	virtual bool init();
public:
	virtual void initLayout();
	virtual void initButton();
public:
	void show();
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*); 
	void Button_Service(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Setting(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Quit(cocos2d::Ref*, WidgetUserInfo*);
};