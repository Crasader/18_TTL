#include "XZDDGameScence.h"
#include "Game/GameLib.h"
#include "GameLib/Game/FV/FvMask.h"

#include "CMD_XZDD.h"
#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"

void XZDDGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START, this, XZDDGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD, this, XZDDGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD, this, XZDDGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY, this, XZDDGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT, this, XZDDGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END, this, XZDDGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE, this, XZDDGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_CHI_HU, this, XZDDGameScence::OnSubUserChiHu);
	addNetCBDefine(SUB_S_GANG_SCORE, this, XZDDGameScence::OnSubGangScore);

	addNetCBDefine(SUB_S_HUANPAI, this, XZDDGameScence::OnSubHuanPai);
	addNetCBDefine(SUB_S_XUANQUE_NOTICE, this, XZDDGameScence::OnSubXuanQueNotice);
	addNetCBDefine(SUB_S_XUANQUE, this, XZDDGameScence::OnSubXuanQue);
	addNetCBDefine(SUB_S_HUANPAI_CHENGDU, this, XZDDGameScence::OnSubHuanPaiChengDu);

	addNetCBDefine(SUB_S_HUANPAI_NOTICE, this, XZDDGameScence::OnSubHuanPaiNotice);

	addNetCBDefine(SUB_S_LEFT_TIMER, this, XZDDGameScence::OnSubTimerNotice);

	addNetCBDefine(SUB_S_MASTER_HANDCARD, this, XZDDGameScence::OnMasterHandCard);
	addNetCBDefine(SUB_S_MASTER_LEFTCARD, this, XZDDGameScence::OnMasterLeftCard);
}
//游戏开始
void XZDDGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_GameStart));
	if (wDataSize != sizeof(XZDD::CMD_S_GameStart))
		return;

	//变量定义
	XZDD::CMD_S_GameStart * pGameStart = (XZDD::CMD_S_GameStart *)pBuffer;

	defaultState();
	showSaiZi(pGameStart->lSiceCount);
	m_iBankerUser = pGameStart->wBankerUser;
	m_iCurrentUser = pGameStart->wCurrentUser;
	m_iUserAction = pGameStart->cbUserAction;
	m_cbActionCardData =pGameStart->cbActionData;
	m_nLeftTime = pGameStart->nLeftTime;

	word wSice2 = word(pGameStart->lSiceCount);
	BYTE SiceFirst = (wSice2 >> 8);
	BYTE SiceSecond = (wSice2);

	int m  = 0;

	if(SiceFirst > SiceSecond)
		m = SiceSecond;
	else
		m = SiceFirst;

	WidgetFun::setVisible(pnode, true);
	QiSiWeiZ = (m_iBankerUser + m_pLocal->GetChairID()+ SiceFirst + SiceSecond) % 4 + 1;
	QiSiWeiZ = QiSiWeiZ * 1000 + 100 + m;
	paitag = QiSiWeiZ;

	Setpaidui();
	int k = 0;
	while (1)
	{
		if (k == 108 - 56) 
			break;
		Node* p_node = pnode->getChildByTag(paitag);
		WidgetFun::setVisible(p_node, false);
		paitag = HuoQuPaiTag(paitag);
		k++;
	}

	WidgetFun::setVisible(this, "TimeNode",true);

	WidgetFun::setText(this, "LastCardCout", (int)pGameStart->cbLeftCardCount);

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		m_pPlayer[i]->setHandCard(pGameStart->cbCardData, MAX_COUNT - 1);
		m_pPlayer[i]->startGame();
	}
	XZDDButtonAction_ShowCard(NULL, NULL);
	updataPlayerZhuangState();
}
//用户出牌
void XZDDGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_OutCard));
	if (wDataSize != sizeof(XZDD::CMD_S_OutCard))
		return;
	m_nGameState = XZDD_STATE_PLAYING;
	//消息处理
	XZDD::CMD_S_OutCard * pOutCard = (XZDD::CMD_S_OutCard *)pBuffer;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		m_pPlayer[i]->setActOutCard(-1);
	}

	XZDDPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
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
	m_pLocal->SetTingPaidepai();
	if (pPlayer == m_pLocal)
	{
		WidgetFun::setVisible(this,"SelfActionNode",false);
	}
	//////////////////////

	WidgetFun::setVisible(this,"SelfActionNode",false);
}
//发牌消息
void XZDDGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	Node* p_node = pnode->getChildByTag(paitag);
	if(NULL == p_node) return;
	WidgetFun::setVisible(p_node, false);
	paitag = HuoQuPaiTag(paitag);

	//效验数据
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_SendCard));
	if (wDataSize != sizeof(XZDD::CMD_S_SendCard))
		return;
	m_nGameState = XZDD_STATE_PLAYING;
	WidgetFun::setVisible(this, "SelfActionNode", false); //发牌了 先隐藏操作按钮
	//变量定义
	XZDD::CMD_S_SendCard * pSendCard = (XZDD::CMD_S_SendCard *)pBuffer;
	setCurrentPlayer(pSendCard->wCurrentUser, pSendCard->cbActionMask, pSendCard->cbCardData);
	XZDDPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	if (pSendCard->cbCardData != 0)
	{
		pPlayer->addNewInCard(pSendCard->cbCardData);
		pPlayer->showHandCard();
		int iLastCardCout = utility::parseInt(WidgetFun::getText(this, "LastCardCout")) - 1;
		WidgetFun::setText(this, "LastCardCout", iLastCardCout);
	}
	memset(m_nGangCards,0,sizeof(m_nGangCards));
	memcpy(m_nGangCards,pSendCard->cbGangCards,sizeof(m_nGangCards));
	m_nGangCardsCount = pSendCard->cbGangCardCount;
	if (pPlayer == m_pLocal&& !pSendCard->bHasHuPai)
	{
		m_pLocal->blackHandCardByQueColor();
	}
	else if (pPlayer == m_pLocal&& pSendCard->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
		/*if (pSendCard->cbActionMask == WIK_NULL)
		{
			TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOutCard, this), 3, pSendCard->cbCardData);
		}
		else
		{
			TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOperate, this), 3.f, pSendCard->cbActionMask);
		}*/
	}
	else
	{
		m_pLocal->blackAllHandCard(true);
	}

	//showClock(pSendCard->nLeftTime);
}
//操作提示
void XZDDGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_OperateNotify));
	if (wDataSize != sizeof(XZDD::CMD_S_OperateNotify))
		return;

	//变量定义
	XZDD::CMD_S_OperateNotify * pOperateNotify = (XZDD::CMD_S_OperateNotify *)pBuffer;
	m_wResumeUser = pOperateNotify->wResumeUser;
	m_cbActionCardData =pOperateNotify->cbActionCard;
	m_iUserAction = pOperateNotify->cbActionMask;
	memset(m_nGangCards,0,sizeof(m_nGangCards));
	if(pOperateNotify->cbActionMask&WIK_GANG)
	{
		m_nGangCards[0]	  = m_cbActionCardData;
		m_nGangCardsCount = 1;
	}
	else
	{
		m_nGangCardsCount = 0;
	}
	setCurrentPlayer(m_pLocal->GetChairID(), pOperateNotify->cbActionMask, pOperateNotify->cbActionCard);
	if (pOperateNotify->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
		//TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOperate, this), 3.f, pOperateNotify->cbActionMask);
	}
	//showClock(12);
}
//操作结果
void XZDDGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_OperateResult));
	if (wDataSize != sizeof(XZDD::CMD_S_OperateResult))
		return;

	//消息处理
	XZDD::CMD_S_OperateResult * pOperateResult = (XZDD::CMD_S_OperateResult *)pBuffer;

	XZDDPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	pPlayer->setOperateResoult(pOperateResult);
	if (pPlayer == m_pLocal)
	{
		m_pLocal->blackHandCardByQueColor();
	}
	XZDDPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer)
	{
		pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		pProvidePlayer->setActOutCard(-1);
	}

	setCurrentPlayer(pOperateResult->wOperateUser, WIK_NULL);
	WidgetFun::setVisible(this, "SelfActionNode", false);
}
//游戏结束
void XZDDGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	m_nGameState = XZDD_STATE_NULL;

	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	XZDD::AllEndInfo kEndInfo;
	kEndInfo.StreamValue(kStream, false);

	WidgetFun::setVisible(this, "FreeStateNode", true);
	WidgetFun::setVisible(this, "GameResoult2", true);//本地方做了修改0000000000000000000000000000000000000000000000000000000000000000000000000

	Node* pNode = WidgetFun::getChildWidget(this,"GameResoult2");

	Setpaidui_fasle();//新加 隐藏牌堆

	/*for (int i  = 0;i<MAX_PLAYER;i++)
	{
		Node* m_node  =  WidgetFun::getChildWidget(pNode,utility::toString("head_",i));
		GamePlayer* pPlayer =  getPlayerByChairID(i);		

		if (!pPlayer)
		{
			continue;
		}
		int iChairID = pPlayer->GetChairID();
		
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_node,"HeadImagic"),pPlayer->GetUserInfo()->szHeadHttp,true);
		WidgetFun::setVisible(m_node, "GoldImagic", false);
	}
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
			XZDDPlayer* pPlayer = getPlayerByChairID(i);
			if(pPlayer == NULL)continue;
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
	
	/*for (int i = 0; i < MAX_PLAYER; i++)
	{
		getPlayerByChairID(i)->defaultState();
		getPlayerByChairID(i)->EndGame();
	}*/
	pGameMan->OnGameEnd(this, 3);
}

void XZDDGameScence::showJieSuanInfo(const XZDD::AllEndInfo& kEndInfo)
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this, "GameResoult2");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setMingPai(true);
		m_pPlayer[i]->showHandCard();

		//WidgetFun::setButtonTxt(pRootNode, utility::toString("Button_ResoultPlayer", i), pPlayer->GetNickName());
		WidgetFun::setText(pRootNode, utility::toString("name", i), pPlayer->GetNickName());
		WidgetFun::setText(pRootNode, utility::toString("GoldTxt", i), kEndInfo.lGameScoreTotal[i]);
		setGameResoultPlayerInfo_shuju(kEndInfo, pPlayer, WidgetFun::getChildWidget(pRootNode, utility::toString("ResoultPlayer_", i)));
		cocos2d::Node* p_node = WidgetFun::getChildWidget(pRootNode, utility::toString("JiesuanCard", i));
		pPlayer->showjiesuanCard(p_node);//结算
	}
	
	WidgetManager::Instance().ButtonRelease(WidgetFun::getChildWidget(this,
		utility::toString("Button_ResoultPlayer", m_pLocal->GetChairID())));
	
	
}
//用户托管
void XZDDGameScence::OnSubTrustee(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_Trustee));
	if (wDataSize != sizeof(XZDD::CMD_S_Trustee))
		return;

	//消息处理
	XZDD::CMD_S_Trustee * pTrustee = (XZDD::CMD_S_Trustee *)pBuffer;
}
//
void XZDDGameScence::OnSubUserChiHu(const void *pBuffer, WORD wDataSize)//持牌胡牌逻辑
{
	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	XZDD::CMD_S_ChiHu kChiHu;
	kChiHu.StreamValue(kStream, false);


	XZDDPlayer* pPlayer = getPlayerByChairID(kChiHu.wChiHuUser);
	pPlayer->showEffect("Hu");
	pPlayer->setChiHuCard(kChiHu.cbChiHuCard);
	if (kChiHu.wChiHuUser != kChiHu.wProviderUser)
	{
		XZDDPlayer* pDestPlayer = getPlayerByChairID(kChiHu.wProviderUser);
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
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		XZDDPlayer* pPlayerTemp = getPlayerByChairID(i);
		pPlayerTemp->showAddGold(kChiHu.lGameScore[i]);
	}
}
//
void XZDDGameScence::OnSubGangScore(const void *pBuffer, WORD wDataSize)
{
	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	XZDD::CMD_S_GangScore kGangScore;
	kGangScore.StreamValue(kStream, false);


	for (int i = 0; i < MAX_PLAYER; i++)
	{
		int nScore = kGangScore.lGangScore[i];
		if (nScore != 0)
		{
			XZDDPlayer* pPlayer = getPlayerByChairID(i);
			pPlayer->showAddGold(nScore);
		}
	}
}

void XZDDGameScence::OnSubHuanPai(const void *pBuffer, WORD wDataSize)
{
	return;
	//效验消息
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_HuanPai));
	if (wDataSize != sizeof(XZDD::CMD_S_HuanPai))return;

	XZDD::CMD_S_HuanPai* pHuanPai = (XZDD::CMD_S_HuanPai*)pBuffer;
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->setHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this, "ExchangeCardNode", false);
}
void XZDDGameScence::OnSubHuanPaiChengDu(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_HuanPaiChengDu));
	if (wDataSize != sizeof(XZDD::CMD_S_HuanPaiChengDu))return;
	m_nGameState = XZDD_STATE_PLAYING;
	XZDD::CMD_S_HuanPaiChengDu* pHuanPai = (XZDD::CMD_S_HuanPaiChengDu*)pBuffer;

	showSaiZi_HuanPai(pHuanPai->wSice);
	BYTE SiceFirst = (pHuanPai->wSice >> 8);
	BYTE SiceSecond = (pHuanPai->wSice);
	int nModel = (SiceFirst + SiceSecond)%GAME_PLAYER;
	if (nModel == 0)
	{
		NoticeMsg::Instance().ShowNoticeMsg(utility::a_u8("顺时针换"));
	}
	else if(nModel == 2)
	{
		NoticeMsg::Instance().ShowNoticeMsg(utility::a_u8("逆时针换"));
	}
	else
	{
		NoticeMsg::Instance().ShowNoticeMsg(utility::a_u8("对家互换"));
	}
	if (pHuanPai->wChairId == getSelfChairID())
	{
		m_pLocal->saveRemoveHuanPai(pHuanPai->cbRemoveHuanCard);
		m_pLocal->saveHuanPai(pHuanPai->cbHuanCard);
		m_pLocal->setHuanPai_ChengDu();
	}
	WidgetFun::setVisible(this, "ExchangeCardNode", false);
}

void XZDDGameScence::OnSubXuanQueNotice(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_XuanQueNotice));
	if (wDataSize != sizeof(XZDD::CMD_S_XuanQueNotice))return;
	m_nGameState = XZDD_STATE_PLAYING;
	XZDD::CMD_S_XuanQueNotice* pInfo = (XZDD::CMD_S_XuanQueNotice*)pBuffer;
	m_nLeftTime = pInfo->nLeftTime;
	if (pInfo->wChairId == getSelfChairID())
	{
		m_pLocal->setQueColor(pInfo->nQueColor);
	}
	DoXuanQueNotice();
	//TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(XZDDGameScence::DoXuanQueNotice, this), 12.f);
}

void XZDDGameScence::OnSubXuanQue(const void *pBuffer, WORD wDataSize)//缺牌消息
{
	//效验消息
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_XuanQue));
	if (wDataSize != sizeof(XZDD::CMD_S_XuanQue))return;

	XZDD::CMD_S_XuanQue* pInfo = (XZDD::CMD_S_XuanQue*)pBuffer;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		cocos2d::log(" user chairid :%d", i);
		cocos2d::log(" user nQueColor:%d", (int)pInfo->nQueColor[i]);

		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		pPlayer->setQueColor(pInfo->nQueColor[i]);
		pPlayer->showQueIconAni();
		pPlayer->showXuanQue(false);
	}

}

void XZDDGameScence::OnSubHuanPaiNotice(const void *pBuffer, WORD wDataSize)
{
	WidgetFun::setVisible(this, "ExchangeCardNode", true);
	m_nGameState = XZDD_STATE_HUANPAI;
	showHuanPai();
}

void XZDDGameScence::OnSubTimerNotice(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(XZDD::CMD_S_LeftTimer));
	if (wDataSize != sizeof(XZDD::CMD_S_LeftTimer))return;

	XZDD::CMD_S_LeftTimer* pInfo = (XZDD::CMD_S_LeftTimer*)pBuffer;
	showClock(pInfo->nLeftTime);
}

void XZDDGameScence::showHuanPai()
{
	ASSERT(m_pLocal);

	WidgetFun::setVisible(this, "ExchangeCardNode", true);
	m_nGameState = XZDD_STATE_HUANPAI;
	m_pLocal->showHuanPai();
}

int XZDDGameScence::HuoQuPaiTag(int paitag)
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
void XZDDGameScence::Setpaidui()
{ 
	int paitag = 1100;
	for (int i = 0; i < 108; ++i)
	{
		Node* p_node = pnode->getChildByTag(paitag);
		WidgetFun::setVisible(p_node, true);
		paitag = HuoQuPaiTag(paitag);
	}
}
void XZDDGameScence::Setpaidui_fasle()
{
	int paitag = 1100;
	for (int i = 0; i < 108; ++i)
	{
		Node* p_node = pnode->getChildByTag(paitag);
		WidgetFun::setVisible(p_node, false);
		paitag = HuoQuPaiTag(paitag);
	}
}

bool XZDDGameScence::isXZDDGameStatusHSZ()
{
	bool isXZDD = m_nGameState == XZDD_STATE_HUANPAI;
	//cocos2d::log(utility::a_u8(utility::toString("换三张状态：",m_nGameState,"== 2")).c_str());
	if (isXZDD) return true;
	return false;
}