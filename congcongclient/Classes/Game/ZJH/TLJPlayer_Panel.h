#pragma once

#include "TLJHeaders.h"
#include "TLJPlayer.h" 


class TLJPlayer_Panel
	: public cocos2d::Node
	, public FvSingleton<TLJPlayer_Panel>
{
public:
	TLJPlayer_Panel();
	~TLJPlayer_Panel();

public:
	bool init();

public:
	//显示玩家
	void showPlayer(TLJPlayer& player);
	//隐藏玩家
	void hidePlayer(word chairID);
	//显示玩家头像
	void showPlayerAvatar(TLJPlayer& player);
	//离线标识
	void showOffline(TLJPlayer& player,bool bShow = false);
	//准备标识
	void showReady(TLJPlayer& player,bool bShow = false);
	//庄家标识
	void showBanker(TLJPlayer& player,bool bShow = false);
	//看牌标识
	//void showKanPai(TLJPlayer& player,bool bShow = false);
	//弃牌标识
	//void showQiPai(TLJPlayer& player,bool bShow = false);
	//状态标识
	void showStateIdentification(TLJPlayer& player);
	//单局投注数
	void showCurrentMatchScore(TLJPlayer& player,bool bShow = false);
	//玩家坐标
	Vec2 getPlayerPosition(TLJPlayer& player);
	//玩家比牌
	void PlayerCompare(word wVisioChairID);
	//玩家比牌
	//void PlayerCompare(word wVisioChairID_0,word wVisoChairID_1,bool bResult);
	void PlayerCompare(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//玩家比牌
	void onPlayerTouched(word wVisioChairID);
	//比牌结束
	void PlayerCompareEnd();
	//显示
	void resetPanel();
	//获取点击玩家座位号
	int getTouchPlayerNode(cocos2d::Vec2 kPos);
	//玩家弃牌
	void PlayerGiveUp(TLJPlayer& player);
	//玩家头像遮罩 显示更改
	void changeMaskVisible(word wVisioChairID,bool bVisible);
	//积分变动提示
	void showAddGold(word wVisioChairID,int iGold);
	//计时器
	void showTimes(word wVisioChairID,int times);
	//隐藏计时器
	void hideTimes();
	//玩家显示节点
	Node* getSeatNode(word wVisioChairID);
private:
	word m_wInitiatorChairID;
	word m_wDestChairID;
	word m_wLoseID;
};
