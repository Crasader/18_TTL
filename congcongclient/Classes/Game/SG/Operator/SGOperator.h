#pragma once

#include "CommonHeader/CommonHeader.h"

class SGOperator
	:public cocos2d::Node
	, public FvSingleton<SGOperator>
{
public:
	SGOperator();
	~SGOperator();
	
#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
	void initButton();
#pragma endregion ��ʼ��

#pragma region ��ʾ������
	void show();
	void hide();

	void show(WORD status);

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
#pragma endregion ��ʾ������

#pragma region ��ť�߼�����
	void Button_StartGame(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Ready(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*);
	void Button_SnatchBanker(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Call(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ShowCard(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Hint(cocos2d::Ref*, WidgetUserInfo*);
#pragma endregion ��ť�߼�����
};