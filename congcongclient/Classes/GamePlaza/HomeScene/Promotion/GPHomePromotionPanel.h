#pragma once

#include "../GPHomeHeader.h"

class GPHomePromotionPanel
	:public cocos2d::Node
	, public FvSingleton<GPHomePromotionPanel>
{
public:
	GPHomePromotionPanel();
	~GPHomePromotionPanel();
public:
	bool init();
	void initLayout();
public:
	void show();
	void hide();
public:
	void initButton();
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Bind(cocos2d::Ref*, WidgetUserInfo*);
};