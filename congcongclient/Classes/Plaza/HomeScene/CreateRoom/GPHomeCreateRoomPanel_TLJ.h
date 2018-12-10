#pragma once
#include <cocos2d.h>
#include "common.h"


class GPHomeCreateRoomPanel_TLJ
	:public cocos2d::Node
	,public FvSingleton<GPHomeCreateRoomPanel_TLJ>
{

public:
	const static int GAME_RULE_COMMON = 1;	//普通模式
	const static int GAME_RULE_PASSION = 2; //激情模式
	const static int GAME_RULE_BRAISED = 3; //焖三圈
public:
	GPHomeCreateRoomPanel_TLJ();
	~GPHomeCreateRoomPanel_TLJ();

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
	void updateScore(byte cbIndex,dword wScore);
private:
	int								m_GameRuleIndex;			//游戏规则
	bool							m_bAllowedStrangerJoin;		//是否允许陌生人加入
	dword                           dwBaseScore;				//底注
	dword                           dwEnterMatchNum;			//入场限制
	dword                           dwOutMatchNum;				//离场限制
};