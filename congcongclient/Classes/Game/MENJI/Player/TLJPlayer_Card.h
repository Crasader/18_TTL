#pragma once
#include "../CMD_TLJ.h"
#include "../Logic/TLJGameLogic.h"
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
	void hideCard(WORD visioChairID);

	void setCardImagic(Node* pNode,BYTE cbCardData,bool isOwn = false);

	//�������
	void PlayerGiveUp(TLJPlayer& player);
	//������� ��ʾ�ڵ�
	Node* GetPlayerCardNode(WORD wVisioChairID);
	//���ѡ�еĽڵ�
	Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
};