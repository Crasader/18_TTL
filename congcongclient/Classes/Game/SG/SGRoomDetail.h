#pragma once

#include "common.h"

class SGRoomDetail
	:public cocos2d::Node
	, public FvSingleton<SGRoomDetail>
{
public:
	SGRoomDetail();
	~SGRoomDetail();

#pragma region 初始化
public:
	bool init();
	void initLayout();
	void initButton();
#pragma endregion 初始化

	void show();
	void hide();

	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
};