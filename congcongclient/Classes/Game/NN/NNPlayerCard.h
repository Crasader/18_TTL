#pragma once

#include "common.h"
#include "NNPlayer.h"

using namespace TTLNN;

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
	void hidePlayer(word visioChairID);
	void calculatePlayer(NNPlayer& player);

#pragma endregion 显示与隐藏

#pragma region 发牌动作
	void onSendPlayerCard();
	void onSendPlayerCardAdd();
	void fanCard(int index);
	cocos2d::Sprite* startOrbitAction(cocos2d::Sprite* sprite, int cardIndex, std::vector<cocos2d::Sprite*>* spList, cocos2d::Vec2* offset = nullptr, float fobTime = 0.25);
	void removeLastCard(NNPlayer& player);
	void setFanPai(bool flag);
	bool getFanPai();
#pragma endregion 发牌动作

#pragma region 摸牌判断
	int getTouchCard(cocos2d::Vec2 kTouchPos);
#pragma endregion 摸牌判断

protected:
	void clearTurningCard();
private:
	//没得意义的临时卡组, 用来消除翻拍动作临时精灵, 正则表达式中使用了
	std::vector<cocos2d::Sprite*> _turning_cards;
	//这一把是否已经翻盘
	bool _b_fan;
};