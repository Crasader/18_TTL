#include "SRLFGameScence.h"
#include "Game/GameLib.h"

#include "CMD_SRLF.h"
#include "SRLFPlayer.h"
#include "SRLFGameLogic.h"

bool SRLFGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_MJ_FREE)
	{
		OnFreeScence(data,dataSize);
	}
	if(cbGameStatus == GS_MJ_HUANPAI)
	{
		OnHuanPaiScence(data,dataSize);
	}
	if (cbGameStatus == GS_MJ_XUANQUE)
	{
		OnXuanQueScence(data,dataSize);
	}
	if (cbGameStatus == GS_MJ_PLAY)
	{
		OnPlayScence(data,dataSize);
	}
	return true;
}
void SRLFGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(SRLF::CMD_S_StatusFree));
	if (wDataSize!=sizeof(SRLF::CMD_S_StatusFree)) return;

	SRLF::CMD_S_StatusFree* pNetInfo = (SRLF::CMD_S_StatusFree*)data;
	changeFreeStateNodeState(m_pLocal->GetUserStatus() < US_READY);
	
	m_nGameState = SRLF_STATE_NULL;

}
void SRLFGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	if (m_pLocal->GetUserStatus() >= US_READY )
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	changeFreeStateNodeState(m_pLocal->GetUserStatus() < US_READY);
}
void SRLFGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(SRLF::CMD_S_StatusPlay));
	if (wDataSize!=sizeof(SRLF::CMD_S_StatusPlay)) return;
	SRLF::CMD_S_StatusPlay* pNetInfo = (SRLF::CMD_S_StatusPlay*)data;
	defaultState();
	for (int i=0;i<MAX_PLAYER;i++)
	{
		auto pPlayer= getPlayerByChairID(i);
		if(pPlayer)	pPlayer->defaultState();
	}
	memset(m_nGangCards,0,sizeof(m_nGangCards));
	memcpy(m_nGangCards,pNetInfo->cbGangCards,sizeof(m_nGangCards));
	m_nGangCardsCount = pNetInfo->cbGangCardsCount;
	m_dwGameRuleIdex = pNetInfo->dwGameRuleIdex;
	m_iBankerUser=pNetInfo->wBankerUser;
	if(FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN))) //人数
		m_pPlayerCount = 3;
	if(FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN))) //人数
		m_pPlayerCount = 2;
	int index = 0;
	for (int i = 0;i< MAX_PLAYER;i++)
	{
		SRLFPlayer* pPlayer = getPlayerByChairID(i);
		if ( NULL == pPlayer ) continue;
		if( UserInfo::Instance().getUserID() != pPlayer->GetUserID() )
		{	
			pPlayer->setWeaveItem(pNetInfo->WeaveItemArray[index],pNetInfo->cbWeaveCount[index]);
			int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
			if(pNetInfo->wCurrentUser == pPlayer->GetChairID())
				pPlayer->setHandCard(pNetInfo->cbCardData,max_count - pNetInfo->cbWeaveCount[index]*3);
			else
				pPlayer->setHandCard(pNetInfo->cbCardData,max_count - pNetInfo->cbWeaveCount[index]*3-1);
			pPlayer->setQueColor(pNetInfo->nQueColor[index]);
			pPlayer->showQueIcon_NoAni();
			for (BYTE m = 0;m<pNetInfo->cbDiscardCount[index];m++)
			{
				pPlayer->addHandOutCard(pNetInfo->cbDiscardCard[index][m]);
			}
		}else{
			pPlayer->setWeaveItem(pNetInfo->WeaveItemArray[index],pNetInfo->cbWeaveCount[index]);
			pPlayer->setHandCard(pNetInfo->cbCardData,pNetInfo->cbCardCount);
			pPlayer->setQueColor(pNetInfo->nQueColor[index]);
			pPlayer->showQueIcon_NoAni();
			for (BYTE m = 0;m<pNetInfo->cbDiscardCount[index];m++)
			{
				pPlayer->addHandOutCard(pNetInfo->cbDiscardCard[index][m]);
			}
		}
		pPlayer->setPiao(pNetInfo->cbPiaoState[index]==2);
		pPlayer->setBaoTing(pNetInfo->bTingCard[index]);
		pPlayer->upBaoTing();
		pPlayer->updataPiao();
		index++;
	}
	defaultPlayerActionState();
	if (pNetInfo->wOutCardUser != INVALID_WORD && pNetInfo->cbOutCardData)
	{
		SRLFPlayer* pPlayer = getPlayerByChairID(pNetInfo->wOutCardUser);
		if(pPlayer)	pPlayer->showMJPoint();
	}
	setCurrentPlayer(pNetInfo->wCurrentUser,pNetInfo->cbActionMask,pNetInfo->cbActionCard);
	auto pPlayer = getPlayerByChairID(m_iCurrentUser);
	if(pPlayer)
	{
		if (pPlayer != m_pLocal )
		{
			pPlayer->addNewInCard(0);
			m_pLocal->blackAllHandCard(true);
		}
		else
		{
			m_pLocal->blackHandCardByQueColor();
		}
	}
	for (int i = 0;i<pNetInfo->cbWinCout;i++)
	{
		auto pPlayer = getPlayerByChairID(pNetInfo->wWinOrder[i]);
		if ( NULL == pPlayer )	continue;
		pPlayer->showHuPai(false);
		pPlayer->showStatusImagic("Hu");
	}
	m_nGameState = SRLF_STATE_PLAYING;
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		auto pPlayer = getPlayerByChairID(i);
		if ( NULL == pPlayer )	continue;
		pPlayer->upPlayerInfo();
		pPlayer->showHandCard();
		pPlayer->startGameNoTime();
	}
	m_cbActionCard=pNetInfo->cbActionCard;
	WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString((int)pNetInfo->cbActionCard));
}

void SRLFGameScence::OnHuanPaiScence(void* data, int dataSize)
{
	return;
	//效验数据
	ASSERT(dataSize==sizeof(SRLF::CMD_S_StateHuanPai));
	if (dataSize!=sizeof(SRLF::CMD_S_StateHuanPai)) return;

	SRLF::CMD_S_StateHuanPai* pNetInfo = (SRLF::CMD_S_StateHuanPai*)data;

	defaultState();
	m_iBankerUser=pNetInfo->wBankerUser;
	m_nLeftTime = pNetInfo->nLeftTime;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		SRLFPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	}
	m_pLocal->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		getPlayerByChairID(i)->showHandCard();
		getPlayerByChairID(i)->startGameNoTime();
	}
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	SRLFButtonAction_ShowCard(NULL,NULL);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);

	m_nGameState = SRLF_STATE_HUANPAI;
}
void SRLFGameScence::OnXuanQueScence(void* data, int dataSize)
{
	return;
	//效验数据
	ASSERT(dataSize==sizeof(SRLF::CMD_S_StateXuanQue));
	if (dataSize!=sizeof(SRLF::CMD_S_StateXuanQue)) return;

	SRLF::CMD_S_StateXuanQue* pNetInfo = (SRLF::CMD_S_StateXuanQue*)data;
	defaultState();
	m_iBankerUser=pNetInfo->wBankerUser;
	m_nLeftTime = pNetInfo->nLeftTime;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		SRLFPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	}
	m_pLocal->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		getPlayerByChairID(i)->showHandCard();
		getPlayerByChairID(i)->startGameNoTime();
	}
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	DoXuanQueNotice();
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);

}
void SRLFGameScence::OnGameServerItem(CGameServerItem* pGameServerItem)
{
	if (!WidgetFun::getChildWidget(this,"GameDiFen"))
	{
		return;
	}
	WidgetFun::setVisible(this,"GameDiFen",!IsPrivateGame());
	if (!IsPrivateGame()&& pGameServerItem )
	{
		WidgetFun::setPlaceText(this,"GameDiFen",utility::toString(pGameServerItem->m_GameServer.lCellScore));
	}
}

