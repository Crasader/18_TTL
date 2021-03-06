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

	//玩家弃牌
	void PlayerGiveUp(TLJPlayer& player);
	//玩家手牌 显示节点
	Node* GetPlayerCardNode(word wVisioChairID);
	//获得选中的节点
	Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
};