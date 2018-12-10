#pragma once

#include "common.h"

class NNCalculate
	: public cocos2d::Node
	, public FvSingleton<NNCalculate>
{
public:
	NNCalculate();
	~NNCalculate();

#pragma region 初始化
	bool init();
	void initLayout();
	void initButton();
#pragma endregion 初始化

#pragma region 显示与隐藏
public:
	void show(CMD_GF_Private_End_Info* pInfo);
	void hide();
	void update(CMD_GF_Private_End_Info* pInfo);
#pragma endregion 显示与隐藏

#pragma region 按钮事件
	void Button_BackPlaza(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Share(cocos2d::Ref*, WidgetUserInfo*);
#pragma endregion 按钮事件
};
