#pragma once
#include "CMD_TLJ.h"
#include "TLJGameLogic.h"
#include "TLJPlayer.h"

using namespace TLJ;
class TLJPlayer_Card
	:public cocos2d::Node
	,public FvSingleton<TLJPlayer_Card>
{
public:
	TLJPlayer_Card();
	~TLJPlayer_Card();
public:
	bool init();
	void show();
	void hide();

public:
	void showCard(TLJPlayer& player, bool afterOpterate = false);
	void hideCard(word visioChairID);

	void setCardImagic(Node* pNode,byte cbCardData,bool isOwn = false);

	//�������
	void PlayerGiveUp(TLJPlayer& player);
	//������� ��ʾ�ڵ�
	Node* GetPlayerCardNode(word wVisioChairID);
	//���ѡ�еĽڵ�
	Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
};