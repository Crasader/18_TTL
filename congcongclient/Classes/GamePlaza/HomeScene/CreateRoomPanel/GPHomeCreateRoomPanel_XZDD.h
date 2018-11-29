#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
#include "Game/Game/GameDefine.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

class GPHomeCreateRoomPanel_XZDD
	:public cocos2d::Node
	, public FvSingleton<GPHomeCreateRoomPanel_XZDD>
{
public:
	//const static DWORD GAME_RULE_YXMSRJOIN = 1;			//允许陌生人加入
	const static DWORD GAME_RULE_NO_WAIT_ = 1;
	const static DWORD GAME_RULE_XZ_ = 2;
	const static DWORD GAME_RULE_XL_ = 3;
	const static DWORD GAME_RULE_HUAN3_ =4;	//换三张
	const static DWORD GAME_RULE_DIN_QUE_=5;	//定缺
	const static DWORD GAME_RULE_DI_FEN0_=6;	//2番封顶
	const static DWORD GAME_RULE_DI_FEN1_=7;	//3番封顶
	const static DWORD GAME_RULE_DI_FEN2_ =8;	//4番封顶
	const static DWORD GAME_RULE_G_S_P_=9;	//呼叫转移
	const static DWORD GAME_RULE_ZI_MO_JIAFAN_=10;	//自摸加番
	const static DWORD GAME_RULE_D_G_H_ZIMO_=11;//点杠花当自摸
	const static DWORD GAME_RULE_Jiang_dui_=12;//将对
	const static DWORD GAME_RULE_MEN_Q_=13;//门清
	const static DWORD GAME_RULE_T_D_HU_=14;//天地胡
	const static DWORD GAME_RULE_YI_JIU_=15;//幺九
	const static DWORD GAME_RULE_ZHONG_ZHANG_=16;//中张
	const static DWORD GAME_RULE_GUOSHOUHU_=17;//过手胡
	const static DWORD GAME_RULE_GUOSHOUHUDA_=18;//过手胡大

	const static DWORD GAME_RULE_ZI_MO_JIADI_ =19;//自摸家底
	const static DWORD GAME_RULE_HAI_DI_LAO_ =20;//海底捞 
	const static DWORD GAME_RULE_D_G_H_DIANPAO_=21;//点杠花当自摸
public:
	GPHomeCreateRoomPanel_XZDD();
	~GPHomeCreateRoomPanel_XZDD();

public:
	bool init();
	void initListView();
	void initLayout();
	void initButton();
	void show();
	void hide();

	void Button_Close(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//关闭
	void Button_CreateRoom_XZDD(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//关闭
	void Button_GameRuleDiZhuNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//底注
	void Button_GameRuleLiChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//离场设定
	void Button_GameRuleJinChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//进场设定
	void updateScore(BYTE cbIndex, dword wScore);
private:
	void initData();//私人显示
	void initGameRuleSheDing();
private:
	//私人
	void Button_GameRuleSheDing(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//游戏设定

	void Button_GameRuleFanShuWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//番数目
	void Button_GameRuleZiMoJiaScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//自摸家底 自摸加翻
private:
	int m_CurrentModeIndex;                //支付方式
	DWORD m_CurrentRuleIndex;                //规则
	int m_CurrentScore;                    //底分
	int m_CurrentGameCount;                //局数
	
	bool  m_GameRuleJoinMoShenRen;		  //允许陌生人加入	
	DWORD  m_GameRuleFanShuWanFa;		  //番数目//玩法选择
	DWORD  m_GameRuleFanMaxNum;			  //番数最大值//番数
	DWORD  m_GameRuleDiZhuNum;			  //底注
	DWORD  m_GameRuleLiChangNum;		  //离场设定
	DWORD  m_GameRuleJinChangNum;		  //进场设定
	DWORD  m_GameYouXiSheDing;			  //游戏设定
	DWORD  m_GameYouXiDianGangHua;		  //点杠花自摸/放炮
	byte   m_GameFangJian;				  //私人房为1/元宝房间为2
	DWORD  m_GameFangJianSeDing;		  //房间设定/新加
};

