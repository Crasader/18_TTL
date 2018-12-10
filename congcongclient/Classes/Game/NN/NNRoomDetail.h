#pragma once

#include "common.h"

class NNRoomDetail
	:public cocos2d::Node
	, public FvSingleton<NNRoomDetail>
{
public:
	NNRoomDetail();
	~NNRoomDetail();

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