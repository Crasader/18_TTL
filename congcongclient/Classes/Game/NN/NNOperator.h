#pragma once

#include "common.h"
#include "Game/NN/CMD_NN.h"

class NNOperator
	:public cocos2d::Node
	, public FvSingleton<NNOperator>
{
public:
	NNOperator();
	~NNOperator();
	
#pragma region 初始化
public:
	bool init();
	void initLayout();
	void initButton();
#pragma endregion 初始化

#pragma region 显示与隐藏
	void show();
	void hide();

	void show(word status);

	void showMessage(std::string showText);
	void hideMessage();

	void showStartGame();
	void hideStartGame();

	void showReady();
	void hideReady();
	void showContinue();
	void hideContinue();

	void showWeiXin();
	void hideWeiXin();

	//推注提示
	void showNoteTuiZhu(byte bTuiZHu);
	void hideNoteTuiZhu();

	void showSnatchButton();
	void hideSnatchButton();

	void showCallButtons();
	void hideCallButtons();

	void showTimes(int times);
	void hideTimes();

	void showSplitButton();
	void hideSplitButton();

	void showSplitCalculate();
	void hideSplitCalculate();
	void updateSplitCalculate();

	void showCalculate();
	void hideCalculate();
#pragma endregion 显示与隐藏

#pragma region 按钮逻辑处理
	void Button_StartGame(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Ready(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*);
	void Button_SnatchBanker(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Call(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ShowCard(cocos2d::Ref*, WidgetUserInfo*);
	void Button_CuoCard(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Hint(cocos2d::Ref*, WidgetUserInfo*);
	TTLNN::NNPlayerBet  m_byteBet;
	int  m_byteSnatchRatio;
#pragma endregion 按钮逻辑处理
};