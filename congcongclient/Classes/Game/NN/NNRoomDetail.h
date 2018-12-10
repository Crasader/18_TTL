#pragma once

#include "common.h"

class NNRoomDetail
	:public cocos2d::Node
	, public FvSingleton<NNRoomDetail>
{
public:
	NNRoomDetail();
	~NNRoomDetail();

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