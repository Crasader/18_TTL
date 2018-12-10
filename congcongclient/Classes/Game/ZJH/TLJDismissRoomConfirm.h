#pragma once

#include "common.h"
#include CMD_GAME

class TLJDismissRoomConfirm
	: public cocos2d::Node
	, public FvSingleton<TLJDismissRoomConfirm>
{
public:
	TLJDismissRoomConfirm();
	~TLJDismissRoomConfirm();
public:
	bool init();
	void initLayout();
	void initButton();
	void updateInfo();
public:
	void show(CMD_GF_Private_Dismiss_Info& dismissInfo);
	void hide();
public:
	void Button_Refuse(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
private:
	CMD_GF_Private_Dismiss_Info m_DismissInfo;
};