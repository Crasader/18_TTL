#pragma once

#include "CommonHeader/CommonHeader.h"
#include "Game/FV/FvMask.h"

class GPHomeCreateRoomPanel_NN
	:public cocos2d::Node
	,public FvSingleton<GPHomeCreateRoomPanel_NN>
{
public:

	GPHomeCreateRoomPanel_NN();
	~GPHomeCreateRoomPanel_NN();

public:
	bool init(); 
	void initLayout();
	void initButton();
	void initData();
	void resetGameData();

public:
	//void updateRatioPanel();

public:
	void show();
	void hide();

public:

	//////////////////////////////////////////////////////////////////////////
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

	void Button_AllowedStranger(cocos2d::Ref*, WidgetUserInfo*);

	void Button_SixRound(cocos2d::Ref*, WidgetUserInfo*);
	void Button_EightRound(cocos2d::Ref*, WidgetUserInfo*);

	//选择局数按钮
	void Button_RoundBox1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_RoundBox2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_RoundBox3(cocos2d::Ref*, WidgetUserInfo*);

	//推注倍数
	void Button_TuiZhu1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TuiZhu2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TuiZhu3(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TuiZhu4(cocos2d::Ref*, WidgetUserInfo*);

	void Button_CanTuiZhu(cocos2d::Ref*, WidgetUserInfo*);

	//特殊牌型
	void Button_ShunZi(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TongHua(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WuHua(cocos2d::Ref*, WidgetUserInfo*);
	void Button_HuLu(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ZhaDan(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WuXiao(cocos2d::Ref*, WidgetUserInfo*);

	//翻倍规则
	void Button_MutipleRule1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_MutipleRule2(cocos2d::Ref*, WidgetUserInfo*);

	//底分倍数
	void Button_DiFen1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_DiFen2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_DiFen3(cocos2d::Ref*, WidgetUserInfo*);
	void Button_DiFen4(cocos2d::Ref*, WidgetUserInfo*);

	//抢庄倍数
	void Button_QiangZhuang1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_QiangZhuang2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_QiangZhuang3(cocos2d::Ref*, WidgetUserInfo*);
	void Button_QiangZhuang4(cocos2d::Ref*, WidgetUserInfo*);

	//房间类型
	//名牌抢庄
	void Button_RoomType_MPQZ(cocos2d::Ref*, WidgetUserInfo*);
	//////////////////////////////////////////////////////////////////////////

	void updateScore(BYTE cbIndex,dword wScore);

private:

	int								_nRoundIndex;	//局数选择序号
	dword							_dwEnterMatchNum;//入场限制
	dword							_dwOutMatchNum;//离场限制
	int								_nGameType;//游戏类型
	dword							_dwBaseScore;//底注
	int								_nQiangZhuangBeiShu;//抢庄倍数
	//游戏规则////////////////////////////////////////////////////////////////////////

	bool								_bCanTuiZhu;// 是否推注
	dword							_nGameRuleIndex;//游戏规则
	bool								_bSixRound;//六人场或者八人场
	int								_nTuiZhuIndex;//闲家推注倍数序号
	int								_nRatioRuleIndex;//牛牛的规则序号0为1,1为2
	bool								_bShunZi;//顺子牛
	bool								_bTongHua;//同花牛
	bool								_bWuHua;//五花牛
	bool								_bHuLu;//葫芦牛
	bool								_bZhaDan;//炸弹牛
	bool								_bWuXiao;//五小牛

	bool								_bAllowedStrangerJoin;//是否允许陌生人加入
};