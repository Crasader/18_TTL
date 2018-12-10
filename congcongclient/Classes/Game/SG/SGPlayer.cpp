#include "SGPlayer.h"
#include "SGPlayerPanel.h"
#include "SGPlayerCard.h"
#include "Game/SG/SGGameScence.h"
#include "Game/SG/CMD_SG.h"
#include "Game/SG/SGRoomInfo.h"
#include "Game/SG/SGOperator.h"

#include USERINFO

SGPlayer::SGPlayer()
	:GamePlayer(NULL)
{
	m_Valid = false;
	m_VisioChairID = INVALID_CHAIR;
	m_PlayerBets = 0;
	m_Status = SG::SGPlayerStatus_Invalid;
	m_SnatchBankerRatio = SG::SGSnatchBanker_Invalid;
	m_wPlayCount = 0;
}

SGPlayer::~SGPlayer()
{

}

#pragma region 数据处理
void SGPlayer::clearGameData()
{
	m_PlayerBets = 0;
	m_SnatchBankerRatio = SG::SGSnatchBanker_Invalid;
	m_Status = SG::SGPlayerStatus_Invalid;
	zeromemory(&m_PlayerCards, sizeof(m_PlayerCards));
	zeromemory(&m_CardType, sizeof(m_CardType));
	zeromemory(&m_Calculate, sizeof(m_Calculate));
}

void SGPlayer::setValid(bool valid)
{
	m_Valid = valid;
}

bool SGPlayer::isValid()
{
	return m_Valid;
}

int SGPlayer::getVisioChairID()
{
	return m_VisioChairID;
}

void SGPlayer::setPlayerBets(word bets)
{
	m_PlayerBets = bets;
}

word SGPlayer::getPlayerBets()
{
	return m_PlayerBets*SGRoomInfo::Instance().getRoomInfo().dwBaseScore * SGGameScence::Instance().getBankerRatio() ;
}

void SGPlayer::setSnatchBankerRatio(word ratio)
{
	m_SnatchBankerRatio = ratio;
}

word SGPlayer::getSnatchBankerRatio()
{
	return m_SnatchBankerRatio;
}

void SGPlayer::setPlayerCards(byte cards[MAX_HAND_CARD], word count)
{
	m_PlayerCards = SGPlayerCard_Entity(cards, count);
}

void SGPlayer::addPlayerCard(byte card)
{
	if (m_PlayerCards.isValid) {
		m_PlayerCards.addCard(card);
	}
}

SGPlayerCard_Entity SGPlayer::getPlayerCards()
{
	return m_PlayerCards;
}

void SGPlayer::setPlayerCardType(SG::SGCardType_Result result)
{
	m_CardType = result;
}

SG::SGCardType_Result SGPlayer::getPlayerCardType()
{
	return m_CardType;
}

void SGPlayer::setPlayerSingleCalculate(int score)
{
	m_Calculate = SGPlayerSingleCalculate(score);
}

SGPlayerSingleCalculate SGPlayer::getPlayerSingleCalculate()
{
	return m_Calculate;
}

void SGPlayer::setPlayerStatus(SG::SGPlayerStatus status)
{
	if (UserInfo::Instance().getUserID() == GetUserID())
	{
		if(status < US_READY)
		{
			SGOperator::Instance().showReady();
		}
		else
		{
			SGOperator::Instance().hideReady();
		}
	}
	m_Status = status;
}

SG::SGPlayerStatus SGPlayer::getPlayerStatus()
{
	return m_Status;
}

//游戏局数
void SGPlayer::setPlayCount(word wCount)
{
	m_wPlayCount = wCount;
}
word SGPlayer::getPlayCount()
{
	return m_wPlayCount;
}
#pragma endregion 数据处理


#pragma region 框架消息
void SGPlayer::PlayerEnter()
{
	m_wPlayCount = 0;
	m_Valid = true; 
	m_VisioChairID = SGGameScence::Instance().getVisioChairID(*this);
	m_Status = SG::SGPlayerStatus_Sitting;
	upPlayerInfo();
}

void SGPlayer::PlayerLeave()
{
	m_Status = SG::SGPlayerStatus_Invalid;
	SGPlayerPanel::Instance().hidePlayer(m_VisioChairID);
	SGPlayerCard::Instance().hidePlayer(m_VisioChairID);
	m_Valid = false;
	m_VisioChairID = INVALID_CHAIR;
}

void SGPlayer::upPlayerInfo()
{
	if (m_Valid) {
		SGPlayerPanel::Instance().showPlayer(*this);
		SGPlayerCard::Instance().showPlayer(*this);
	}
}

void SGPlayer::upPlayerState()
{
	if (m_Valid) {
		SGPlayerPanel::Instance().showPlayer(*this);
		SGPlayerCard::Instance().showPlayer(*this);
	}
}

void SGPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)
{
	SGPlayerPanel::Instance().playerTalk(*this, pNetInfo);
}
#pragma endregion 框架消息


