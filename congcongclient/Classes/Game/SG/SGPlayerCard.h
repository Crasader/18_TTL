#pragma once

#include "common.h"
#include "SGPlayer.h"

class SGPlayerCard
	: public cocos2d::Node
	, public FvSingleton<SGPlayerCard>
{
public:
	const static int CARD_DWON_POSY = 80;
	const static int CARD_UP_POSY = 110;
public:
	SGPlayerCard();
	~SGPlayerCard();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
#pragma endregion ��ʼ��

#pragma region ��ʾ������
public:
	void show();
	void hide();

	void showPlayer(SGPlayer& player);
	void hidePlayer(word visioChairID);
#pragma endregion ��ʾ������

#pragma region ���ƶ���
	void sendPlayerCard();
	void sendPlayerCardAdd();
	void startOrbitAction(cocos2d::Sprite* sprite, int cardIndex);
#pragma endregion ���ƶ���

#pragma region �����ж�
	int getTouchCard(cocos2d::Vec2 kTouchPos);
#pragma endregion �����ж�

private:
	std::vector<cocos2d::Sprite*> m_TempCards;
};