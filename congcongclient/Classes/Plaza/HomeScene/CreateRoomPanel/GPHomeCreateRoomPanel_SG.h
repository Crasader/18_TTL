#pragma once

#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
#include "Game/Game/GameDefine.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

class GPHomeCreateRoomPanel_SG
	:public cocos2d::Node
	,public FvSingleton<GPHomeCreateRoomPanel_SG>
{

public:

public:
	GPHomeCreateRoomPanel_SG();
	~GPHomeCreateRoomPanel_SG();

public:
	bool init(); 
	void initLayout();
	void initButton();
	void initData();
	void resetGameData();

public:
	void updateRatioPanel();

public:
	void show();
	void hide();
public:
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

	void Button_AllowStrangerJoin(cocos2d::Ref*, WidgetUserInfo*);
	void Button_GameRules(cocos2d::Ref*, WidgetUserInfo*);

	void Button_ChangeGameScore(cocos2d::Ref*, WidgetUserInfo*);
	void updateScore(BYTE cbIndex,dword wScore);
private:
	int								m_GameRuleIndex;			//游戏规则
	bool							m_bAllowedStrangerJoin;		//是否允许陌生人加入
	bool							m_bAllowedStrangerQiangZhuang;		//是否允许陌生人加入
	dword                           dwBaseScore;				//底注
	dword                           dwEnterMatchNum;			//入场限制
	dword                           dwOutMatchNum;				//离场限制
};