#include "XZDDGameScence.h"
#include "GAME.h"

#include "CMD_XZDD.h"
#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"

bool XZDDGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_MJ_FREE)
	{
		OnFreeScence(data,dataSize);
		m_GameZT = GS_MJ_FREE;
	}
	if(cbGameStatus == GS_MJ_HUANPAI)
	{
		OnHuanPaiScence(data,dataSize);
		m_GameZT = GS_MJ_HUANPAI;
	}
	if (cbGameStatus == GS_MJ_XUANQUE)
	{
		OnXuanQueScence(data,dataSize);
		m_GameZT = GS_MJ_XUANQUE;
	}
	if (cbGameStatus == GS_MJ_PLAY)
	{
		OnPlayScence(data,dataSize);
		m_GameZT = GS_MJ_PLAY;
	}
	return true;
}
void XZDDGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_StatusFree));
	if (wDataSize != sizeof(XZDD::CMD_S_StatusFree)) return;

	XZDD::CMD_S_StatusFree* pNetInfo = (XZDD::CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);

	m_nGameState = XZDD_STATE_NULL;
	updataPlayerZhuangState();

}
void XZDDGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	if (m_pLocal->GetUserStatus() >= US_READY )
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
	showClock(10,m_pLocal->GetUserStatus() < US_READY);
}
void XZDDGameScence::OnPlayScence(void* data, int wDataSize)//重新加入游戏场景消息
{
	//效验数据
	ASSERT(wDataSize == sizeof(XZDD::XZDD_CMD_S_StatusPlay));
	if (wDataSize != sizeof(XZDD::XZDD_CMD_S_StatusPlay)) return;

	XZDD::XZDD_CMD_S_StatusPlay* pNetInfo = (XZDD::XZDD_CMD_S_StatusPlay*)data;
	defaultState();
	m_nGameState = XZDD_STATE_PLAYING;
	for(int i = 0 ;i< GAME_PLAYER; ++i)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		for(int j = 0 ; j < 34; ++j)
		{
			if(pNetInfo->m_cbChiHuS_Z[i][j] != 0)
			{
				pPlayer->setChiHuCard(pNetInfo->m_cbChiHuS_Z[i][j]);
				if(pNetInfo->m_GameLeiXing != GAME_RULE_XL)
				{
					for(int m = 0 ; m < 14 ; ++m )
						if((int)pNetInfo->m_cbChiHuS_Z[i][j] == (int)pNetInfo->cbCardData[m])
						{
							//pNetInfo->cbCardData[m] = 0;
							pNetInfo->cbCardCount--;
							for(int k = m; k <13; ++k )
								pNetInfo->cbCardData[k] = pNetInfo->cbCardData[k+1];
							break;
						}
				}
			}
		}	 
	}

	//WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString(pNetInfo->cbActionCard));
	memset(m_nGangCards,0,sizeof(m_nGangCards));
	memcpy(m_nGangCards,pNetInfo->cbGangCards,sizeof(m_nGangCards));
	m_nGangCardsCount = pNetInfo->cbGangCardCount;
	m_cbOutCardData = pNetInfo->cbOutCardData;
	m_cbActionCardData =pNetInfo->cbActionCard;
	m_iBankerUser=pNetInfo->wBankerUser;
	m_iCurrentUser = pNetInfo->wCurrentUser;
	m_iUserAction = pNetInfo->cbActionMask;
	//重新确定排堆位置
	word wSice2 = word(pNetInfo->m_lSiceCount);
	byte SiceFirst = (wSice2 >> 8);
	byte SiceSecond = (wSice2);

	int m  = 0;

	if(SiceFirst > SiceSecond)
		m = SiceSecond;
	else
		m = SiceFirst;

	QiSiWeiZ = (m_iBankerUser + m_pLocal->GetChairID()+ SiceFirst + SiceSecond) % 4 + 1;
	QiSiWeiZ = QiSiWeiZ * 1000 + 100 + m;
	int n = QiSiWeiZ;

	for(int i = 0;i < 108 - pNetInfo->cbLeftCardCount;++i)
	{
		Node* p_node = pnode->getChildByTag(n);
		WidgetFun::setVisible(p_node, false);
		n = HuoQuPaiTag(n);
	}
	paitag = n;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		pPlayer->setWeaveItem(pNetInfo->WeaveItemArray[i],pNetInfo->cbWeaveCount[i]);
		if(pPlayer->GetChairID() == m_pLocal->GetChairID())
			m_pLocal->setHandCard(pNetInfo->cbCardData,pNetInfo->cbCardCount);
		else
			pPlayer->setHandCard(pNetInfo->cbCardData,pNetInfo->cbCardOther[i]);
		pPlayer->setQueColor(pNetInfo->nQueColor[i]);
		pPlayer->showQueIcon_NoAni();
		for (byte m = 0;m<pNetInfo->cbDiscardCount[i];m++)
		{
			pPlayer->addHandOutCard(pNetInfo->cbDiscardCard[i][m]);
		}
	}
	
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGameNoTime();
	}

	defaultPlayerActionState();
	if (pNetInfo->wOutCardUser != INVALID_WORD && pNetInfo->cbOutCardData)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(pNetInfo->wOutCardUser);
		pPlayer->showMJPoint();
	}

	if (pNetInfo->wCurrentUser != INVALID_WORD)
	{
		setCurrentPlayer(pNetInfo->wCurrentUser,pNetInfo->cbActionMask,pNetInfo->cbActionCard);

		XZDDPlayer* pPlayer = getPlayerByChairID(m_iCurrentUser);
		if (pPlayer != m_pLocal && pPlayer)
		{
			pPlayer->addNewInCard(0);
			m_pLocal->blackAllHandCard(true);
		}
		else if (pPlayer == m_pLocal&& pPlayer)
		{
			m_pLocal->blackHandCardByQueColor();
		}
	}
	
	else if (pNetInfo->cbActionMask != WIK_NULL)
	{
		setCurrentPlayer(m_pLocal->GetChairID(),pNetInfo->cbActionMask,pNetInfo->cbActionCard);
	}
	if(pNetInfo->cbWinCout > 4)
		pNetInfo->cbWinCout = 4;

	if(pNetInfo->m_bHuPaiWanJia[m_iCurrentUser])
	{
		m_pLocal->blackAllHandCard(true);
		if((pNetInfo->cbCardCount-2)%3==0)
		{
			SendSocketData(SUB_C_OUT_CARD,&pNetInfo->cbActionCard,sizeof(pNetInfo->cbActionCard));
		}
	}
	else
		m_pLocal->blackHandCardByQueColor();


	for (int i = 0;i<GAME_PLAYER;i++)
	{
		if(pNetInfo->m_bHuPaiWanJia[i])
		{
			XZDDPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			pPlayer->showHuPai(false);
			pPlayer->showStatusImagic("Hu");
			
		}	
	}
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);
	updataPlayerZhuangState();
}

void XZDDGameScence::OnHuanPaiScence(void* data, int dataSize)
{
	//效验数据
	printf("XZDDGameScence::OnHuanPaiScence(void* data, int dataSize)\n");
	ASSERT(dataSize==sizeof(XZDD::CMD_S_StateHuanPai));
	if (dataSize != sizeof(XZDD::CMD_S_StateHuanPai)) return;

	XZDD::CMD_S_StateHuanPai* pNetInfo = (XZDD::CMD_S_StateHuanPai*)data;

	defaultState();
	m_iBankerUser=pNetInfo->wBankerUser;
	m_nLeftTime = pNetInfo->nLeftTime;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	}
	m_pLocal->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGameNoTime();
	}
	//XZDDButtonAction_ShowCard(NULL,NULL);
	getPlayerByChairID(m_iBankerUser)->setZhuang(true);
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);
	showClock(m_nLeftTime);
	m_nGameState = XZDD_STATE_HUANPAI;
	showHuanPai();
	updataPlayerZhuangState();
}
void XZDDGameScence::OnXuanQueScence(void* data, int dataSize)//选缺场景
{
	//效验数据
	printf("XZDDGameScence::OnXuanQueScence(void* data, int dataSize)\n");
	ASSERT(dataSize == sizeof(XZDD::CMD_S_StateXuanQue));
	if (dataSize != sizeof(XZDD::CMD_S_StateXuanQue)) return;

	XZDD::CMD_S_StateXuanQue* pNetInfo = (XZDD::CMD_S_StateXuanQue*)data;

	defaultState();
	m_iBankerUser=pNetInfo->wBankerUser;
	m_nLeftTime = pNetInfo->nLeftTime;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	}
	m_pLocal->setHandCard(pNetInfo->cbCardData,MAX_COUNT-1);

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
		m_pPlayer[i]->startGameNoTime();
	}
	getPlayerByChairID(m_iBankerUser)->setZhuang(true);
	DoXuanQueNotice();
	WidgetFun::setText(this,"LastCardCout",(int)pNetInfo->cbLeftCardCount);
	showClock(m_nLeftTime);
	updataPlayerZhuangState();
}
void XZDDGameScence::OnGameServerItem(CGameServerItem* pGameServerItem)
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