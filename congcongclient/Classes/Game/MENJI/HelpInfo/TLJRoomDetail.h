#pragma once

#include "Game/GameLib.h"
#include "Game/Script/ScriptXMLparse.h"
#include "Game/FV/FvMask.h"

class TLJRoomDetail
	:public cocos2d::Node
	, public FvSingleton<TLJRoomDetail>
{
public:
	TLJRoomDetail();
	~TLJRoomDetail();

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