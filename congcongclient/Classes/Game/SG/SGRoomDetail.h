#pragma once

#include "common.h"

class SGRoomDetail
	:public cocos2d::Node
	, public FvSingleton<SGRoomDetail>
{
public:
	SGRoomDetail();
	~SGRoomDetail();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
	void initButton();
#pragma endregion ��ʼ��

	void show();
	void hide();

	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
};