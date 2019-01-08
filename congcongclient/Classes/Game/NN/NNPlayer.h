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

#pragma region ���ݴ���
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
	//��Ϸ����
	void setPlayCount(word wCount);
	word getPlayCount();
#pragma endregion ���ݴ���

#pragma region �����Ϣ
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
#pragma endregion �����Ϣ
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

	word m_wPlayCount;		//��Ϸ����
};