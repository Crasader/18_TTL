#include "SRLFGameScence.h"
#include "Game/GameLib.h"

#include "CMD_SRLF.h"
#include "SRLFPlayer.h"
#include "SRLFGameLogic.h"

void SRLFGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START, this, SRLFGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD, this, SRLFGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD, this, SRLFGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY, this, SRLFGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT, this, SRLFGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END, this, SRLFGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE, this, SRLFGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU, this, SRLFGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE, this, SRLFGameScence::OnSubGangScore);

	addNetCBDefine(SUB_S_HUANPAI, this, SRLFGameScence::OnSubHuanPai);
	addNetCBDefine(SUB_S_XUANQUE_NOTICE, this, SRLFGameScence::OnSubXuanQueNotice);
	addNetCBDefine(SUB_S_XUANQUE, this, SRLFGameScence::OnSubXuanQue);
	addNetCBDefine(SUB_S_HUANPAI_CHENGDU, this, SRLFGameScence::OnSubHuanPaiChengDu);

	addNetCBDefine(SUB_S_HUANPAI_NOTICE, this, SRLFGameScence::OnSubHuanPaiNotice);

	addNetCBDefine(SUB_S_PIAOSTATE, this, SRLFGameScence::OnSubPiaoState);//飘
	addNetCBDefine(SUB_S_TING_FIRST, this, SRLFGameScence::OnSubTingFirst);//是否报听
	addNetCBDefine(SUB_S_PIAO_NOTICE, this, SRLFGameScence::OnSubPiaoNotice);//飘提示 显示不显示
	addNetCBDefine(SUB_S_BAOTING_NOTICE, this, SRLFGameScence::OnSubBaoTingNotice);//报叫提示 显示不显示
	addNetCBDefine(SUB_S_LEFT_TIMER, this, SRLFGameScence::OnLeftTimer);

	addNetCBDefine(SUB_S_MASTER_HANDCARD, this, SRLFGameScence::OnMasterHandCard);
	addNetCBDefine(SUB_S_MASTER_LEFTCARD, this, SRLFGameScence::OnMasterLeftCard);
}
//游戏开始
void SRLFGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_GameStart));
	if (wDataSize != sizeof(SRLF::CMD_S_GameStart))
		return;
	//变量定义
	SRLF::CMD_S_GameStart * pGameStart = (SRLF::CMD_S_GameStart *)pBuffer;
	defaultState();
	for (int i=0;i<MAX_PLAYER;i++)
	{
		auto pPlayer= getPlayerByChairID(i);
		if (pPlayer)
		{
			getPlayerByChairID(i)->defaultState();
		}
	}
	showSaiZi(pGameStart->lSiceCount);
	m_iBankerUser = pGameStart->wBankerUser;
	m_iCurrentUser = pGameStart->wCurrentUser;
	m_iUserAction = pGameStart->cbUserAction;
	m_nLeftTime = pGameStart->nLeftTime;
	m_cbActionCard=pGameStart->cbActionData;
	WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString((int)pGameStart->cbActionData));
	WidgetFun::setVisible(this, "TimeNode",true);
	WidgetFun::setText(this, "LastCardCout", (int)pGameStart->cbLeftCardCount);
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		SRLFPlayer* pPlyer = getPlayerByChairID(i);
		pPlyer->setHandCard(pGameStart->cbCardData,GetHandCardsMaxCount()-1);
		pPlyer->setZhuangVale(m_iBankerUser == pPlyer->GetChairID());
		pPlyer->upPlayerInfo();
		pPlyer->startGame();
	}
	SRLFButtonAction_ShowCard(NULL, NULL);
	m_nGameState = SRLF_STATE_READY;
	
	SRLFPlayer* pLocalPlayer = getPlayerByChairID(m_pSelfPlayer->GetChairID());
	pLocalPlayer->setPlayCount(pLocalPlayer->getPlayCount()+1);

#ifdef SHOW_SERVICE_CORE
	//扣税收显示(仅首局显示)
	if(pLocalPlayer->getPlayCount()==1){
		//三人两房服务费修改为0.2
		//int iGameTax = -0.3 * m_kPrivateRoomInfo.dwBaseScore;
		int iGameTax = -0.2 * m_kPrivateRoomInfo.dwBaseScore;
		showGameTax(iGameTax);
	}
#endif

	updataPlayerSeat();
}
//用户出牌
void SRLFGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_OutCard));
	if (wDataSize != sizeof(SRLF::CMD_S_OutCard))
		return;

	//消息处理
	SRLF::CMD_S_OutCard * pOutCard = (SRLF::CMD_S_OutCard *)pBuffer;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		getPlayerByChairID(i)->setActOutCard(-1);
	}

	SRLFPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	pPlayer->sendOutCard(pOutCard->cbOutCardData);
	pPlayer->blackAllHandCard(true);

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this, "TimeNode");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		WidgetFun::setVisible(pRootNode, utility::toString("TimePoint", i), false);
	}
	//WidgetFun::getChildWidget(pRootNode, "ActPlayerLastTime")->stopAllActions();
	//WidgetFun::setText(pRootNode, "ActPlayerLastTime", "0");
	showClock(0,true);
	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//发牌消息
void SRLFGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_SendCard));
	if (wDataSize != sizeof(SRLF::CMD_S_SendCard))
		return;
	WidgetFun::setVisible(this, "SelfActionNode", false); //发牌了 先隐藏操作按钮
	//变量定义
	SRLF::CMD_S_SendCard * pSendCard = (SRLF::CMD_S_SendCard *)pBuffer;
	SRLFPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->setFirstCardSended(pSendCard->bFirstSended);
		pPlayer->setTing(pSendCard->bBaoTing);
		pPlayer->setNewCard(pSendCard->cbCardData);
		pPlayer->addNewInCard(pSendCard->cbCardData);
	}
	memset(m_nGangCards,0,sizeof(m_nGangCards));
	memcpy(m_nGangCards,pSendCard->cbGangCards,sizeof(m_nGangCards));
	m_nGangCardsCount = pSendCard->cbGangCardCount;
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this, "LastCardCout")) - 1;
		WidgetFun::setText(this, "LastCardCout", iLastCardCout);
	}
	if (pPlayer->isBaoTing()|| pSendCard->bHasHuPai || (pPlayer != m_pLocal && pSendCard->cbActionMask ==WIK_NULL ))
	{
		m_pLocal->blackAllHandCard(true);
	}
	else
	{
		if (pPlayer == m_pLocal)
		{
			setCurrentPlayer(pSendCard->wCurrentUser, pSendCard->cbActionMask, pSendCard->cbCardData);
		}
		m_pLocal->blackAllHandCard(false);
	}
	m_nGameState = SRLF_STATE_PLAYING;
}
//操作提示
void SRLFGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_OperateNotify));
	if (wDataSize != sizeof(SRLF::CMD_S_OperateNotify))
		return;
	//变量定义
	SRLF::CMD_S_OperateNotify * pOperateNotify = (SRLF::CMD_S_OperateNotify *)pBuffer;
	m_wResumeUser     = pOperateNotify->wResumeUser;
	m_cbActionCard    = pOperateNotify->cbActionCard;
	memset(m_nGangCards,0,sizeof(m_nGangCards));
	if(pOperateNotify->cbActionMask&WIK_GANG)
	{
		m_nGangCards[0]	  = m_cbActionCard;
		m_nGangCardsCount = 1;
	}
	else
	{
		m_nGangCardsCount = 0;
	}
	setCurrentPlayer(m_pLocal->GetChairID(), pOperateNotify->cbActionMask, pOperateNotify->cbActionCard);
	//如果胡牌后禁止出牌
	if (pOperateNotify->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
	}
}
//操作结果
void SRLFGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_OperateResult));
	if (wDataSize != sizeof(SRLF::CMD_S_OperateResult))
		return;

	//消息处理
	SRLF::CMD_S_OperateResult * pOperateResult = (SRLF::CMD_S_OperateResult *)pBuffer;

	SRLFPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);
	if (pPlayer == m_pLocal)
	{
		m_pLocal->blackHandCardByQueColor();
	}
	SRLFPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer)
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}

	setCurrentPlayer(pOperateResult->wOperateUser, WIK_NULL);
	WidgetFun::setVisible(this, "SelfActionNode", false);
}
//游戏结束
void SRLFGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	m_nGameState = SRLF_STATE_NULL;
	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	SRLF::AllEndInfo kEndInfo;
	kEndInfo.StreamValue(kStream, false);

	changeFreeStateNodeState(true);
	WidgetFun::setVisible(this, "GameResoult2", true);//本地方做了修改0000000000000000000000000000000000000000000000000000000000000000000000000

	word wChairId = m_pLocal->GetChairID();
	SCORE lMyScore = kEndInfo.lGameScoreTotal[wChairId];

	/*WidgetFun::setVisible(this, "WinTitle1", lMyScore >= 0);
	WidgetFun::setVisible(this, "LoseTitle1", lMyScore < 0);

	WidgetFun::setVisible(this, "WinGoldAtlas", lMyScore >= 0);
	WidgetFun::setVisible(this, "LoseGoldAtlas", lMyScore < 0);
*/
	/*if (lMyScore >= 0)
	{
		WidgetFun::setAtlasTxt(this, "WinGoldAtlas", utility::toString("+", (int)lMyScore));
	}
	else
	{
		WidgetFun::setAtlasTxt(this, "LoseGoldAtlas", utility::toString((int)lMyScore));
	}*/

	showJieSuanInfo(kEndInfo);
	if (IsPrivateGame())
	{
	 	for (int i = 0;i<MAX_PLAYER;i++)
	 	{
	 		SRLFPlayer* pPlayer = getPlayerByChairID(i);
			if(pPlayer->getValid() == false)continue;
			auto vCardsList = kEndInfo.kGameEndCardsList[i].cbCardsList;
			BYTE cbCardData[MAX_COUNT];
			memset(cbCardData,0,sizeof(cbCardData));
			BYTE index=0;
			for (auto itor =vCardsList.begin();itor!=vCardsList.end();itor++ )
			{
				cbCardData[index++] = *itor;
			}
			pPlayer->setMingPai(true);
	 		pPlayer->setHandCard(cbCardData,vCardsList.size());
			pPlayer->showHandCard();
	 	}
	}
	defaultMaster();
	for(int i =0;i<MAX_PLAYER;i++)
	{
		getPlayerByChairID(i)->setBaoTing(false);
		getPlayerByChairID(i)->setPiao(false);
		getPlayerByChairID(i)->upBaoTing();
		getPlayerByChairID(i)->updataPiao();
		//getPlayerByChairID(i)->defaultState();
		//getPlayerByChairID(i)->EndGame();
	}
	pGameMan->OnGameEnd(this, 3);
}

void SRLFGameScence::showJieSuanInfo(const SRLF::AllEndInfo& kEndInfo)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this, "GameResoult2");
	for (int i = 0; i < 4 ; i++)
	{
		WidgetFun::setText(pRootNode, utility::toString("name", i), "");
		WidgetFun::setVisible(pRootNode, utility::toString("PiaoImg", i), false);
		WidgetFun::setText(pRootNode, utility::toString("ResoultPlayer_", i), "");
		WidgetFun::setText(pRootNode, utility::toString("GoldTxt", i), "");
		WidgetFun::setVisible(pRootNode, utility::toString("head_", i), false);
		auto pHead = WidgetFun::getChildWidget(pRootNode,utility::toString("head_", i));
		WidgetFun::setImagic(pHead,"HeadImagic","Head/Head0.png");
		cocos2d::Node* p_node = WidgetFun::getChildWidget(pRootNode, utility::toString("JiesuanCard", i));
		p_node->removeAllChildren();
	}
	std::vector<GamePlayer*> kPlayers;
	getGamePlayerToList(&kPlayers);
	int i = 0;
	for (auto itor =kPlayers.begin();itor!= kPlayers.end();itor++)
	{
		auto pPlayer = dynamic_cast<SRLFPlayer*>(*itor);
		WidgetFun::setVisible(pRootNode, utility::toString("PiaoImg", i),pPlayer->isPiao());
		WidgetFun::setVisible(pRootNode, utility::toString("head_", i), true);
		auto pHead = WidgetFun::getChildWidget(pRootNode,utility::toString("head_", i));
		auto pNode = WidgetFun::getChildWidget(pHead,"HeadImagic");
		if (pPlayer->GetHeadHttp() != "" && NULL != pNode)
		{
			ImagicDownManager::Instance().addDown(pNode,pPlayer->GetHeadHttp(),pPlayer->GetUserID());
		}
		pPlayer->setMingPai(true);
		BYTE tempIndex[MAX_INDEX];
		memset(tempIndex,0,sizeof(tempIndex));
		BYTE tempCard[MAX_COUNT];
		memset(tempCard,0,sizeof(tempCard));
		for(int j=0;j<MAX_COUNT;j++){
			tempCard[j] = kEndInfo.cbCard[i*MAX_COUNT+j];
		}
		SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(tempCard,MAX_COUNT,tempIndex);
		int iCount = (int)SRLFLOGIC::CGameLogic::Instance().GetCardCount(tempIndex);
		pPlayer->setHandCard(tempCard,iCount);
		//pPlayer->showHandCard();
		//WidgetFun::setButtonTxt(pRootNode, utility::toString("Button_ResoultPlayer", i), pPlayer->GetNickName());
		WidgetFun::setText(pRootNode, utility::toString("name", i), pPlayer->GetNickName());
		WidgetFun::setText(pRootNode, utility::toString("GoldTxt", i), kEndInfo.lGameScoreTotal[i]);
		setGameResoultPlayerInfo_shuju(kEndInfo, pPlayer, WidgetFun::getChildWidget(pRootNode, utility::toString("ResoultPlayer_", i)));
		cocos2d::Node* p_node = WidgetFun::getChildWidget(pRootNode, utility::toString("JiesuanCard", i));
		pPlayer->showjiesuanCard(p_node);
		i++;
	/*	WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,
			utility::toString("Button_ResoultPlayer", m_pLocal->GetChairID())));*/
	}
	
	/*WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,
		utility::toString("Button_ResoultPlayer", m_pLocal->GetChairID())));*/
}
//用户托管
void SRLFGameScence::OnSubTrustee(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_Trustee));
	if (wDataSize != sizeof(SRLF::CMD_S_Trustee))
		return;
	//消息处理
	SRLF::CMD_S_Trustee * pTrustee = (SRLF::CMD_S_Trustee *)pBuffer;
}
//
void SRLFGameScence::OnSubUserChiHu(const void *pBuffer, WORD wDataSize)//持牌胡牌逻辑
{
	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	SRLF::CMD_S_ChiHu kChiHu;
	kChiHu.StreamValue(kStream, false);


	SRLFPlayer* pPlayer = getPlayerByChairID(kChiHu.wChiHuUser);
	pPlayer->showEffect("Hu");
	pPlayer->setChiHuCard(kChiHu.cbChiHuCard);
	if (kChiHu.wChiHuUser != kChiHu.wProviderUser)
	{
		SRLFPlayer* pDestPlayer = getPlayerByChairID(kChiHu.wProviderUser);
		pPlayer->showStatusImagic("Hu");
		pDestPlayer->removeHandOutCard(kChiHu.cbChiHuCard);
		pDestPlayer->setActOutCard(-1);
		pDestPlayer->runAniDianPao();
	}
	else
	{
		pPlayer->showStatusImagic("ZiMo");
		pPlayer->runAniZiMo();
		pPlayer->removeHandCard(kChiHu.cbChiHuCard);
	}
	pPlayer->showHuPai(pPlayer != m_pLocal);
	//取消显示分数
	//for (int i = 0; i < MAX_PLAYER; i++)
	//{
	//	SRLFPlayer* pPlayerTemp = getPlayerByChairID(i);
	//	pPlayerTemp->showAddGold(kChiHu.lGameScore[i]);
	//}

	std::vector<GamePlayer*> kPlayers;
	getGamePlayerToList(&kPlayers);
	int i = 0;
	for (auto itor =kPlayers.begin();itor!= kPlayers.end();itor++)
	{
		if (kChiHu.lGameScore[i] != 0)
		{
			dynamic_cast<SRLFPlayer*>(*itor)->showAddGold(kChiHu.lGameScore[i]);
		}
		i++;
	}

}
//
void SRLFGameScence::OnSubGangScore(const void *pBuffer, WORD wDataSize)
{
	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	SRLF::CMD_S_GangScore kGangScore;
	kGangScore.StreamValue(kStream, false);

	/*for (int i = 0; i < MAX_PLAYER; i++)
	{
		int nScore = kGangScore.lGangScore[i];
		if (nScore != 0)
		{
			SRLFPlayer* pPlayer = getPlayerByChairID(i);
			pPlayer->showAddGold(nScore);
		}
	}*/
	std::vector<GamePlayer*> kPlayers;
	getGamePlayerToList(&kPlayers);
	int i = 0;
	for (auto itor =kPlayers.begin();itor!= kPlayers.end();itor++)
	{
		int nScore = kGangScore.lGangScore[i];
		if (nScore != 0)
		{
			dynamic_cast<SRLFPlayer*>(*itor)->showAddGold(nScore);
		}
		i++;
	}
}

void SRLFGameScence::OnSubHuanPai(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_HuanPai));
	if (wDataSize != sizeof(SRLF::CMD_S_HuanPai))return;

	SRLF::CMD_S_HuanPai* pHuanPai = (SRLF::CMD_S_HuanPai*)pBuffer;
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->setHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this, "ExchangeCardNode", false);
}
void SRLFGameScence::OnSubHuanPaiChengDu(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_HuanPaiChengDu));
	if (wDataSize != sizeof(SRLF::CMD_S_HuanPaiChengDu))return;

	SRLF::CMD_S_HuanPaiChengDu* pHuanPai = (SRLF::CMD_S_HuanPaiChengDu*)pBuffer;

	showSaiZi_HuanPai(pHuanPai->wSice);
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->saveRemoveHuanPai(pHuanPai->cbRemoveHuanCard);
		m_pLocal->saveHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this, "ExchangeCardNode", false);
}

void SRLFGameScence::OnSubXuanQueNotice(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_XuanQueNotice));
	if (wDataSize != sizeof(SRLF::CMD_S_XuanQueNotice))return;

	SRLF::CMD_S_XuanQueNotice* pInfo = (SRLF::CMD_S_XuanQueNotice*)pBuffer;
	m_nLeftTime = pInfo->nLeftTime;

	if (pInfo->wChairId == getSelfChairID())
	{
		m_pLocal->setQueColor(pInfo->nQueColor);
	}
	//TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SRLFGameScence::DoXuanQueNotice, this), 2.f);
}

void SRLFGameScence::OnSubXuanQue(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_XuanQue));
	if (wDataSize != sizeof(SRLF::CMD_S_XuanQue))return;

	SRLF::CMD_S_XuanQue* pInfo = (SRLF::CMD_S_XuanQue*)pBuffer;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		cocos2d::log(" user chairid :%d", i);
		cocos2d::log(" user nQueColor:%d", (int)pInfo->nQueColor[i]);

		SRLFPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setQueColor(pInfo->nQueColor[i]);
		pPlayer->showQueIconAni();
		pPlayer->showXuanQue(false);
	}

}

void SRLFGameScence::OnSubHuanPaiNotice(const void *pBuffer, WORD wDataSize)
{
	showHuanPai();
	//	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SRLFGameScence::showHuanPai,this),4.2f);
}

void SRLFGameScence::OnSubPiaoState(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_PiaoState));
	if (wDataSize != sizeof(SRLF::CMD_S_PiaoState))return;
	SRLF::CMD_S_PiaoState* pInfo = (SRLF::CMD_S_PiaoState*)pBuffer;
	getPlayerByChairID(pInfo->wPiaoUser)->setPiao(pInfo->cbPiaoState==2);
	getPlayerByChairID(pInfo->wPiaoUser)->updataPiao();
}

void SRLFGameScence::OnSubTingFirst(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_TING_FIRST));
	if (wDataSize != sizeof(SRLF::CMD_S_TING_FIRST))return;
	SRLF::CMD_S_TING_FIRST* pInfo = (SRLF::CMD_S_TING_FIRST*)pBuffer;
	getPlayerByChairID(pInfo->wTingCardUser)->setBaoTing(true);
	getPlayerByChairID(pInfo->wTingCardUser)->upBaoTing();
	//播放听特效
	//显示听牌变量 信息
	//TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SRLFGameScence::showHuanPai,this),4.2f);
}

void SRLFGameScence::OnSubPiaoNotice(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_PiaoNotice));
	if (wDataSize != sizeof(SRLF::CMD_S_PiaoNotice))return;
	SRLF::CMD_S_PiaoNotice* pInfo = (SRLF::CMD_S_PiaoNotice*)pBuffer;
	bool isAllowJiaPiao = FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_JP)) && (pInfo->cbShowPiao == true); 
	WidgetFun::setVisible(this,"JiaPiaoNode",isAllowJiaPiao);
	for (int i=0;i<MAX_PLAYER;i++)
	{
		auto pPlayer= getPlayerByChairID(i);
		if (pPlayer)
		{
			if (pPlayer->getValid())
			{
				pPlayer->show();
			}else
			{
				pPlayer->hide();
			}
		}
	}
	/*if(pInfo->cbShowPiao)
	{
		showClock(pInfo->iTimer,true);
	}*/
}

void SRLFGameScence::OnSubBaoTingNotice(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_BaoTingNotice));
	if (wDataSize != sizeof(SRLF::CMD_S_BaoTingNotice))return;
	SRLF::CMD_S_BaoTingNotice* pInfo = (SRLF::CMD_S_BaoTingNotice*)pBuffer;
	if( pInfo->wChairID == INVALID_CHAIR )
	{
		if(pInfo->cbShowBaoTing == false)
			WidgetFun::setVisible(this,"BaoTingNode",false);
	}else{
		if(pInfo->wChairID == getSelfChairID())
		{
			WidgetFun::setVisible(this,"BaoTingNode",pInfo->cbShowBaoTing);
		}
	}
	updataPlayerSeat();
}

void SRLFGameScence::OnLeftTimer(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(SRLF::CMD_S_LeftTimer));
	if (wDataSize != sizeof(SRLF::CMD_S_LeftTimer))return;
	SRLF::CMD_S_LeftTimer* pInfo = (SRLF::CMD_S_LeftTimer*)pBuffer;
	showClock(pInfo->nLeftTime,true);
}

void SRLFGameScence::showHuanPai()
{
	ASSERT(m_pLocal);
	WidgetFun::setVisible(this, "ExchangeCardNode", true);
	m_nGameState = SRLF_STATE_HUANPAI;
	m_pLocal->showHuanPai(GAME_TYPE_CHENGDU);
}


int SRLFGameScence::HuoQuPaiTag(int paitag)
{
	if (paitag / 1000 == 1)
	{
		if (paitag % 1000 == 13)
			return 2100;
		else if (paitag % 1000 / 100 == 0)
			return paitag + 1 + 100;
		else
			return paitag - 100;
	}
	else if (paitag / 1000 == 2)
	{
		if (paitag % 2000 == 12)
			return 3100;
		else if (paitag % 2000 / 100 == 0)
			return paitag + 1 + 100;
		else
			return paitag - 100;
	}
	else if (paitag / 1000 == 3)
	{
		if (paitag % 3000 == 13)
			return 4100;
		else if (paitag % 3000 / 100 == 0)
			return paitag + 1 + 100;
		else
			return paitag - 100;
	}
	else
	{
		if (paitag % 4000 == 12)
			return 1100;
		else if (paitag % 4000 / 100 == 0)
			return paitag + 1 + 100;
		else
			return paitag - 100;
	}
}

int SRLFGameScence::GetHandCardsMaxCount()
{
	if (m_dwGameRuleIdex == NULL)
		return MAX_COUNT;
	if(FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_14CARDS))) //14张牌
		return MAX_COUNT;
	if(FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_8CARDS))) //8张牌
		return MAX_COUNT-6;
	return MAX_COUNT;
}

int SRLFGameScence::GetWeaveMax()
{
	if (m_dwGameRuleIdex == NULL)
		return MAX_WEAVE;
	if(FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_14CARDS))) //14张牌
		return MAX_WEAVE;
	if(FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_8CARDS))) //8张牌
		return MAX_WEAVE-2;
	return MAX_WEAVE;
}