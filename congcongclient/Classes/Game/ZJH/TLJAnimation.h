#pragma once
#include "CMD_TLJ.h"
#include "TLJHeaders.h"
#include "TLJPlayer.h"

class TLJAnimation
	:public cocos2d::Node
	,public FvSingleton<TLJAnimation>
{
public:
	TLJAnimation();
	~TLJAnimation();
public:
	bool init();
	//播放下注动画
	void runBettingAnimation(Node& pChip);
	//播放弃牌动画
	void runGiveUpAnimation(Node& pCardNode);
	//弃牌动画 回调
	void giveUpAniCallBack(Node& pCard);
	//比牌动画	比牌玩家进场
	void runCompareAnimation(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//比牌动画	比牌特效
	void runCompareAnimation_2(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//比牌动画  输赢效果
	void runCompareAnimation_3(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//比牌动画  回调函数
	void CompareAnimationCallBack();
	//比牌玩家头像
	void showComparePlayerAvatar(TLJPlayer& player,std::string kValue);
	//开局动画
	void runGameStartAnimation(TLJPlayer& player);
	//结束动画
	void runGameEndAnimation(TLJPlayer& player);

	void GameEndAniCallBack();

	void hide();
	void show();
};

