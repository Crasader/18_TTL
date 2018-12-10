#pragma once

#include "common.h"

enum NN_DismissRoom_Type{
	NN_DismissRoom_HostBeforeStart,
	NN_DismissRoom_InGameSelf,
	NN_DismissRoom_InGameOthers,
};

class NNDismissRoom
	: public cocos2d::Node
	, public FvSingleton<NNDismissRoom>
{
public:
	NNDismissRoom();
	~NNDismissRoom();

public:
	bool init();
	void initLayout();
	void initButton();
	void initData();

public:
	void show(NN_DismissRoom_Type type = NN_DismissRoom_HostBeforeStart, CMD_GF_Private_Dismiss_Info* pInfo = nullptr);
	void hide();

public:
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Refuse(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

private:
	TTFConfig m_LabelConfig;
	NN_DismissRoom_Type m_DismissRoomType;
};