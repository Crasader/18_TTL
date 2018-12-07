#pragma once
#include "cocos2d.h"
#include "../TLJHeaders.h"
#include "cocos-ext.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetManager.h"

USING_NS_CC_EXT;

class TLJOperate
	: public cocos2d::Node
	, public FvSingleton<TLJOperate>
{
public:
	const static int NORMAL_MODE = 1;//普通状态
	const static int WAIT_MODE = 2;//等待状态
	const static int ADD_SCORE_MODE = 3;//加注状态
	const static int AUTO_MODE = 4;//自动加注状态
	const static int INVALID_MODE = 5;//观战状态
private:
	int m_iMode;
public:
	TLJOperate();
	~TLJOperate();

	bool init();
	void initButton();

	void TLJ_Operate_BiPai(cocos2d::Ref* ref, WidgetUserInfo* pInfo);
	void TLJ_Operate_Gen(cocos2d::Ref* ref, WidgetUserInfo* pInfo);
	void TLJ_Operate_GenDaoDi(cocos2d::Ref* ref, WidgetUserInfo* pInfo);
	void TLJ_Operate_Cancle(cocos2d::Ref*, WidgetUserInfo*);
	void TLJ_Operate_JiaZhu(cocos2d::Ref*, WidgetUserInfo*);
	void TLJ_Operate_QiPai(cocos2d::Ref* ref, WidgetUserInfo* pInfo);

	void TLJ_Operate_Betting(cocos2d::Ref* ref, WidgetUserInfo* pInfo);
	//取消加注
	void TLJ_Operate_AddScore_Cancle(cocos2d::Ref* ref, WidgetUserInfo* pInfo);

	void changeOperateMode(int iMode);
	int getOperateMode();

	void refreshBettingBtn();
	void changeBettingBtnImagic();

	void show();
	void hide();
};
