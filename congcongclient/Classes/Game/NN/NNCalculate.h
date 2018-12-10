#pragma once

#include "common.h"

class NNCalculate
	: public cocos2d::Node
	, public FvSingleton<NNCalculate>
{
public:
	NNCalculate();
	~NNCalculate();

#pragma region ��ʼ��
	bool init();
	void initLayout();
	void initButton();
#pragma endregion ��ʼ��

#pragma region ��ʾ������
public:
	void show(CMD_GF_Private_End_Info* pInfo);
	void hide();
	void update(CMD_GF_Private_End_Info* pInfo);
#pragma endregion ��ʾ������

#pragma region ��ť�¼�
	void Button_BackPlaza(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Share(cocos2d::Ref*, WidgetUserInfo*);
#pragma endregion ��ť�¼�
};
