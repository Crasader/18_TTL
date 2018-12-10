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

#pragma region 初始化
public:
	bool init();
	void initLayout();
#pragma endregion 初始化

#pragma region 显示与隐藏
public:
	void show();
	void hide();

	void showPlayer(SGPlayer& player);
	void hidePlayer(word visioChairID);
#pragma endregion 显示与隐藏

#pragma region 发牌动作
	void sendPlayerCard();
	void sendPlayerCardAdd();
	void startOrbitAction(cocos2d::Sprite* sprite, int cardIndex);
#pragma endregion 发牌动作

#pragma region 摸牌判断
	int getTouchCard(cocos2d::Vec2 kTouchPos);
#pragma endregion 摸牌判断

private:
	std::vector<cocos2d::Sprite*> m_TempCards;
};