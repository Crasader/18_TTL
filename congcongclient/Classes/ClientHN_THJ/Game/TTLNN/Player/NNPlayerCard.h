#pragma once

#include "CommonHeader/CommonHeader.h"
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

#pragma region 初始化
public:
	bool init();
	void initLayout();
#pragma endregion 初始化

#pragma region 显示与隐藏
public:
	void show();
	void hide();

	void showPlayer(NNPlayer& player);
	void hidePlayer(WORD visioChairID);
	void calculatePlayer(NNPlayer& player);
#pragma endregion 显示与隐藏

#pragma region 发牌动作
	void sendPlayerCard();
	void sendPlayerCardAdd();
	void startOrbitAction(cocos2d::Sprite* sprite, int cardIndex);
	void removeLastCard(NNPlayer& player);
#pragma endregion 发牌动作

#pragma region 摸牌判断
	int getTouchCard(cocos2d::Vec2 kTouchPos);
#pragma endregion 摸牌判断

private:
	std::vector<cocos2d::Sprite*> m_TempCards;
};