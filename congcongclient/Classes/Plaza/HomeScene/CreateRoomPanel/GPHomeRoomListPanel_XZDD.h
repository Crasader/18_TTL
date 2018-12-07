#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
#include "Game/Game/GameDefine.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"



class GPHomeRoomListPanel_XZDD
	: public cocos2d::Node
	, public FvSingleton<GPHomeRoomListPanel_XZDD>
{
public:
	GPHomeRoomListPanel_XZDD();
	~GPHomeRoomListPanel_XZDD();
public:
	bool init();
	void initButton();
	void show();
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Refresh(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Join(cocos2d::Ref*, WidgetUserInfo*);
	void Button_CreateRoom(cocos2d::Ref*, WidgetUserInfo*);
	void initListView();

	void onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
	void RefreshListView(void * data, size_t iDataSize);

	void Button_JionRomByIDLeft(cocos2d::Ref*, WidgetUserInfo*);
	void Button_JionRomByIDRight(cocos2d::Ref*, WidgetUserInfo*);
private:
	int m_CurrentRuleIndex;                //πÊ‘Ú
};