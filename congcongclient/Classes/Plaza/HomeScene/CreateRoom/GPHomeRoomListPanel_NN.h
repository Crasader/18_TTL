#pragma once

#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
 
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

class GPHomeRoomListPanel_NN
	: public cocos2d::Node
	, public FvSingleton<GPHomeRoomListPanel_NN>
{
public:
	GPHomeRoomListPanel_NN();
	~GPHomeRoomListPanel_NN();
public:
	bool init();
	void initListView();
	void initButton();
public:
	void show();
	void hide();
public:

	//////////////////////////////////////////////////////////////////////////
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Refresh(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Join(cocos2d::Ref*, WidgetUserInfo*);
	void Button_CreateRoom(cocos2d::Ref*, WidgetUserInfo*);
	void Button_JionRomByIDLeft(cocos2d::Ref*, WidgetUserInfo*);
	void Button_JionRomByIDRight(cocos2d::Ref*, WidgetUserInfo*);
	//////////////////////////////////////////////////////////////////////////

	void onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
	void RefreshListView(void * data, size_t iDataSize);

	void setGameRuleIdex(int iIdex);

private:
	int m_CurrentModeIndex;                //֧����ʽ
	int m_CurrentRuleIndex;                //����
	time_t m_lastTime;
};
