#pragma once

#include "common.h"

class TLJDismissRoom
	: public cocos2d::Node
	, public FvSingleton<TLJDismissRoom>
{
public:
	TLJDismissRoom();
	~TLJDismissRoom();
public:
	bool init();
	void initLayout(bool isPlaying = true);
	void initButton();
public:
	void show(bool isPlaying = true);
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
};