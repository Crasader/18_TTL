#pragma once

#include "CommonHeader/CommonHeader.h"
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"

using SG::SGCardType_Result;
using SG::SGPlayerCard_Entity;
using SG::SGPlayerSingleCalculate;

class SGPlayer
	:public GamePlayer
{
public:
	SGPlayer();
	~SGPlayer();

#pragma region ���ݴ���
public:
	void clearGameData();
	
	void setValid(bool valid);
	bool isValid();

	int getVisioChairID();

	void setPlayerBets(WORD bets);
	WORD getPlayerBets();

	void setSnatchBankerRatio(WORD ratio);
	WORD getSnatchBankerRatio();

	void setPlayerCards(BYTE* cards, WORD count);
	void addPlayerCard(BYTE card);
	SGPlayerCard_Entity getPlayerCards();

	void setPlayerCardType(SGCardType_Result result);
	SGCardType_Result getPlayerCardType();

	void setPlayerSingleCalculate(int score);
	SGPlayerSingleCalculate getPlayerSingleCalculate();

	void setPlayerStatus(SG::SGPlayerStatus status);
	SG::SGPlayerStatus getPlayerStatus();

	//��Ϸ����
	void setPlayCount(WORD wCount);
	WORD getPlayCount();
#pragma endregion ���ݴ���

#pragma region �����Ϣ
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
#pragma endregion �����Ϣ

private:
	bool m_Valid;
	WORD m_VisioChairID;
	WORD m_PlayerBets;
	WORD m_SnatchBankerRatio;

	WORD m_wPlayCount;		//��Ϸ����

	SG::SGPlayerStatus m_Status;
	SGPlayerCard_Entity m_PlayerCards;
	SGCardType_Result m_CardType;
	SGPlayerSingleCalculate m_Calculate;
};