#pragma once

#include "common.h"
#include "Game/SG/CMD_SG.h"

using SG::SGCardType_Result;
using SG::SGPlayerCard_Entity;
using SG::SGPlayerSingleCalculate;

class SGPlayer
	:public GamePlayer
{
public:
	SGPlayer();
	~SGPlayer();

#pragma region 数据处理
public:
	void clearGameData();
	
	void setValid(bool valid);
	bool isValid();

	int getVisioChairID();

	void setPlayerBets(word bets);
	word getPlayerBets();

	void setSnatchBankerRatio(word ratio);
	word getSnatchBankerRatio();

	void setPlayerCards(byte* cards, word count);
	void addPlayerCard(byte card);
	SGPlayerCard_Entity getPlayerCards();

	void setPlayerCardType(SGCardType_Result result);
	SGCardType_Result getPlayerCardType();

	void setPlayerSingleCalculate(int score);
	SGPlayerSingleCalculate getPlayerSingleCalculate();

	void setPlayerStatus(SG::SGPlayerStatus status);
	SG::SGPlayerStatus getPlayerStatus();

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

private:
	bool m_Valid;
	word m_VisioChairID;
	word m_PlayerBets;
	word m_SnatchBankerRatio;

	word m_wPlayCount;		//游戏局数

	SG::SGPlayerStatus m_Status;
	SGPlayerCard_Entity m_PlayerCards;
	SGCardType_Result m_CardType;
	SGPlayerSingleCalculate m_Calculate;
};