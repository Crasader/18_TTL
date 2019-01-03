#pragma once

#include "common.h"
#include "NNPlayer.h"

class NNPlayerCard
	: public cocos2d::Node
	, public FvSingleton<NNPlayerCard>
{
public:
	const static int CARD_DWON_POSY = 80;
	const static int CARD_UP_POSY = 110;
public:
	NNPlayerCard();
	~NNPlayerCard();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
#pragma endregion ��ʼ��

#pragma region ��ʾ������
public:
	void show();
	void hide();

	void showPlayer(NNPlayer& player);
	void hidePlayer(word visioChairID);
	void calculatePlayer(NNPlayer& player);
#pragma endregion ��ʾ������

#pragma region ���ƶ���
	void onSendPlayerCard();
	void onSendPlayerCardAdd();
	void fanCard(int index);
	cocos2d::Sprite* startOrbitAction(cocos2d::Sprite* sprite, int cardIndex, std::vector<cocos2d::Sprite*>* spList, cocos2d::Vec2* offset = nullptr, float fobTime = 0.25);
	void removeLastCard(NNPlayer& player);
	void setFanPai(bool flag);
#pragma endregion ���ƶ���

#pragma region �����ж�
	int getTouchCard(cocos2d::Vec2 kTouchPos);
#pragma endregion �����ж�
	
private:
	//û���������ʱ����, �����������Ķ�����ʱ����
	std::vector<cocos2d::Sprite*> m_TempCards;
	//��һ���Ƿ��Ѿ�����
	bool _b_fan;
};