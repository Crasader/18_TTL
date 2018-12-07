#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "Game/Game/GameDefine.h"

class GPHomeEditNumPanel
	: public cocos2d::Node
	, public FvSingleton<GPHomeEditNumPanel>
{
public:
	GPHomeEditNumPanel();
	~GPHomeEditNumPanel();
public:
	void show(std::string kGameType,int cbIndex);
	void hide();
public:
	bool init();
	void initButton();

	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
#pragma region 
	void Button_Zero(cocos2d::Ref*, WidgetUserInfo*);
	void Button_One(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Two(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Three(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Four(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Five(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Six(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Seven(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Eight(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Nine(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Delete(cocos2d::Ref*, WidgetUserInfo*);
#pragma endregion
	void updateRoomNum();
public:
	std::string m_kScore;
	std::string m_GameType;
	int m_cbIndex;
	dword m_cbBaseScore;
};