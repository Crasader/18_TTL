#pragma once

#include "common.h"
#include "Game/NN/TurnCardAction/TurnCardNode.h"

class NNTurnCard
	:public cocos2d::Node
	//,public TimeNode
	,public FvSingleton<NNTurnCard>
{
public:
	NNTurnCard();
	~NNTurnCard();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
	void initButton();
	void removeTurnCard();
	void createTurnCard(int cardIndex);
#pragma endregion ��ʼ��
	void show();
	void hide();
	void delayHide();
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
public:
	TurnCardNode* m_pTurnCard;
};