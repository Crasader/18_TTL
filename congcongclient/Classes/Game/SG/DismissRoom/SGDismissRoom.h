#pragma once

#include "CommonHeader/CommonHeader.h"

enum SG_DismissRoom_Type{
	SG_DismissRoom_HostBeforeStart,
	SG_DismissRoom_InGameSelf,
	SG_DismissRoom_InGameOthers,
};

class SGDismissRoom
	: public cocos2d::Node
	, public FvSingleton<SGDismissRoom>
{
public:
	SGDismissRoom();
	~SGDismissRoom();

public:
	bool init();
	void initLayout();
	void initButton();
	void initData();

public:
	void show(SG_DismissRoom_Type type = SG_DismissRoom_HostBeforeStart, CMD_GF_Private_Dismiss_Info* pInfo = nullptr);
	void hide();

public:
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Refuse(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

private:
	TTFConfig m_LabelConfig;
	SG_DismissRoom_Type m_DismissRoomType;
};