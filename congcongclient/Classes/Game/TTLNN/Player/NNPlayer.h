#pragma once

#include "CommonHeader/CommonHeader.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"

using TTLNN::NNCardType_Result;
using TTLNN::NNPlayerCard_Entity;
using TTLNN::NNPlayerSingleCalculate;

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
	WORD getPlayerBets();

	void setSnatchBankerRatio(WORD ratio);
	WORD getSnatchBankerRatio();

	void setPlayerCards(BYTE* cards, WORD count);
	void addPlayerCard(BYTE card);
	NNPlayerCard_Entity getPlayerCards();

	void setPlayerCardType(NNCardType_Result result);
	NNCardType_Result getPlayerCardType();

	void setPlayerSingleCalculate(int score);
	NNPlayerSingleCalculate getPlayerSingleCalculate();

	void setPlayerStatus(TTLNN::NNPlayerStatus status);
	TTLNN::NNPlayerStatus getPlayerStatus();

	//游戏局数
	void setPlayCount(WORD wCount);
	WORD getPlayCount();
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
	WORD m_VisioChairID;
	TTLNN::NNPlayerBet m_PlayerBets;
	WORD m_SnatchBankerRatio;

	TTLNN::NNPlayerStatus m_Status;
	NNPlayerCard_Entity m_PlayerCards;
	NNCardType_Result m_CardType;
	NNPlayerSingleCalculate m_Calculate;

	WORD m_wPlayCount;		//游戏局数
};