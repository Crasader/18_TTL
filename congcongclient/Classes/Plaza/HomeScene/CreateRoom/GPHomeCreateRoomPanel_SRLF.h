#pragma once

#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
 
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "DEFINE.h"

class GPHomeCreateRoomPanel_SRLF
	: public cocos2d::Node
	, public FvSingleton<GPHomeCreateRoomPanel_SRLF>
{
public:
	const static dword GAME_RULE_YXMSRJOIN = 1; //允许陌生人加入
	const static dword GAME_RULE_14CARDS = 2; //十四张
	const static dword GAME_RULE_8CARDS = 3; //八张
	const static dword GAME_RULE_XZDD = 4; //血战到底
	const static dword GAME_RULE_TDH = 5; //推到胡
	const static dword GAME_RULE_DGH_DP = 6; // 点杠花（点炮）
	const static dword GAME_RULE_DGH_ZM = 7; // 点杠花（自摸）
	const static dword GAME_RULE_GFXY = 8;//刮风下雨
	const static dword GAME_RULE_JP = 9;//加飘
	const static dword GAME_RULE_3_REN = 10;//3人
	const static dword GAME_RULE_2_REN = 11;//2人
	const static dword GAME_RULE_2_FAN = 12;//2番
	const static dword GAME_RULE_3_FAN = 13;//3番
	const static dword GAME_RULE_4_FAN = 14;//4番
	const static dword GAME_RULE_HAIDI_LAO = 15;//海底捞
	const static dword GAME_RULE_HAIDI_PAO = 16;//海底炮
	const static dword GAME_RULE_ZIMO_JIADI = 17;//自摸加底
	const static dword GAME_RULE_ZIMO_JIAFAN = 18;//自摸加番
public:
	GPHomeCreateRoomPanel_SRLF();
	~GPHomeCreateRoomPanel_SRLF();
public:
	bool init();
	void initListView();
	void initButton();
	//void initModeStatus();
	//void initRuleStatus();
	//void initGameScore();
	//void initGameCount(); 
	void initData();
public:
	void show();
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	
	//void Button_PlayerRule(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);

	void Button_GameRuleJoinMoShenRen(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//允许陌生人加入;
	void Button_GameRuleCardsNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//牌张数		
	void Button_GameRuleHuPaiWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//胡牌
	void Button_GameRuleDianGangHua(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//点杠花
	void Button_GameRuleFanShuWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//番数目
	void Button_GameRuleZiMoJiaScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//自摸家底 自摸加翻
	void Button_GameRuleHaiDiScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//海底捞 和海底炮
	void Button_GameRulePlayerNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//人数
	void Button_GameRuleFanMaxNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//番数最大值
	void Button_GameRuleDiZhuNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//底注
	void Button_GameRuleLiChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//离场设定
	void Button_GameRuleJinChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//进场设定

	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Mode(cocos2d::Ref*, WidgetUserInfo*);
	void updateScore(byte cbIndex,dword wScore);



	void Button_PrivatePlayCout(cocos2d::Ref*, WidgetUserInfo*);
	void ButtonGameRule(cocos2d::Ref*, WidgetUserInfo*);
	void Button_PrivatePlayFanbei(cocos2d::Ref*, WidgetUserInfo*);

	void onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);

public:
	void changeGameTypeBtnTexture(std::string target, std::string value);

private:
	int m_CurrentModeIndex;                //支付方式
	int m_CurrentRuleIndex;                //规则
	int m_CurrentScore;                    //底分
	int m_CurrentGameCount;                //局数

	dword  m_GameRuleJoinMoShenRen;		  //允许陌生人加入	
	dword  m_GameRuleCardsNum;			  //牌张数		
	dword  m_GameRuleHuPaiWanFa;		  //胡牌
	dword  m_GameRuleDianGangHua;		  //点杠花
	dword  m_GameRuleFanShuWanFa;		  //番数目
	dword  m_GameRulePlayerNum;			  //人数
	dword  m_GameRuleFanMaxNum;			  //番数最大值
	dword  m_GameRuleDiZhuNum;			  //底注
	dword  m_GameRuleLiChangNum;		  //离场设定
	dword  m_GameRuleJinChangNum;		  //进场设定
};
