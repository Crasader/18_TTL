#include "SRLFGameScence.h"
#include "GAME.h"
#include "SRLFPlayer.h"
#include USERINFO

SRLFPlayer* SRLFGameScence::getPlayerByChairID(int iChairID)
{
	if(iChairID <0 && iChairID >= MAX_PLAYER) return NULL;
	if (!m_pLocal->getUserItem(false))
	{
		return m_pLocal;
	}
	int maxPlayer= DF::shared()->GetGamePlayer();
	int iIdex = (m_pLocal->GetChairID()-iChairID+maxPlayer)%maxPlayer;
	return m_pPlayer[iIdex];
}

GamePlayer* SRLFGameScence::CreatePlayer( IClientUserItem * pIClientUserItem )
{
	if (pIClientUserItem->GetUserID() == UserInfo::Instance().getUserID())
	{
		m_pLocal->setUserItem(pIClientUserItem);
		m_pLocal->setValid(true);
		return m_pLocal;
	}
	else
	{
		GamePlayer* pPlayer = getPlayerByChairID(pIClientUserItem->GetChairID());
		pPlayer->setUserItem(pIClientUserItem);
		dynamic_cast<SRLFPlayer*>(pPlayer)->setValid(true);
		return pPlayer;
	}
	return NULL;
}
void SRLFGameScence::DeletePlayer(GamePlayer* pPlayer)
{
	auto pTempPlayer= getPlayerByChairID(pPlayer->GetChairID());
	if (pTempPlayer) pTempPlayer->setValid(false);
}

void SRLFGameScence::upSelfPlayerInfo()
{
	updataPlayerSeat();
}
void SRLFGameScence::upOtherPlayerInfo()
{
}









