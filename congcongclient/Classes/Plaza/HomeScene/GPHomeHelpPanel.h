#pragma once
#include "common.h"

class GPHomeHelpPanel
	:public cocos2d::Node
	, public FvSingleton<GPHomeHelpPanel>
{
public:
	GPHomeHelpPanel();
	~GPHomeHelpPanel();
public:
	bool init();
public:
	void initLayout();
	void initButton();
	void updateStatus();
public:
	void show();
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Select(cocos2d::Ref*, WidgetUserInfo*);
public:
	int m_CurrentIndex;
};