#pragma once

#include "common.h"
#include "Game/NN/CMD_NN.h"

using namespace TTLNN;

class NNPlayer
	:public GamePlayer
{
public:
	NNPlayer();
	~NNPlayer();

#pragma region 数据处理
public:
	void clearGameData();
	
	void setValid(bool valid);
	bool isValid();

	int getVisioChairID();

	void setPlayerBets(TTLNN::NNPlayerBet& bets);
	word getPlayerBets();

	void setSnatchBankerRatio(word ratio);
	word getSnatchBankerRatio();

	void setPlayerCards(byte* cards, word count);
	void addPlayerCard(byte card);
	NNPlayerCard_Entity getPlayerCards();

	void setPlayerCardType(NNCardType_Result result);
	NNCardType_Result getPlayerCardType();

	void setPlayerSingleCalculate(int score);
	NNPlayerSingleCalculate getPlayerSingleCalculate();

	void setPlayerStatus(TTLNN::NNPlayerStatus status);
	TTLNN::NNPlayerStatus getPlayerStatus();

	void setBanker(bool flag);
	//游戏局数
	void setPlayCount(word wCount);
	word getPlayCount();
#pragma endregion 数据处理

#pragma region 框架消息
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
#pragma endregion 框架消息
	void calculateInfo();
public:
	void removeLastCard();

private:
	bool m_Valid;
	word m_VisioChairID;
	TTLNN::NNPlayerBet m_PlayerBets;
	word m_SnatchBankerRatio;

	TTLNN::NNPlayerStatus m_Status;
	NNPlayerCard_Entity m_PlayerCards;
	NNCardType_Result m_CardType;
	NNPlayerSingleCalculate m_Calculate;

	word m_wPlayCount;		//游戏局数
};