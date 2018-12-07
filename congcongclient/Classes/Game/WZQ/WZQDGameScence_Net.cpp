#include "WZQDGameScence.h"
#include "Game/GameLib.h"

#include "CMD_WZQ.h"
#include "WZQDPlayer.h"
#include "WZQDGameLogic.h"
using namespace WZQ;
void WZQDGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START, this, WZQDGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD, this, WZQDGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_SEND_CARD, this, WZQDGameScence::OnSubSendCard);
	addNetCBDefine(SUB_S_OPERATE_NOTIFY, this, WZQDGameScence::OnSubOperateNotify);
	addNetCBDefine(SUB_S_OPERATE_RESULT, this, WZQDGameScence::OnSubOperateResult);
	addNetCBDefine(SUB_S_GAME_END, this, WZQDGameScence::OnSubGameEnd);
	addNetCBDefine(SUB_S_TRUSTEE, this, WZQDGameScence::OnSubTrustee);
	addNetCBDefine(SUB_S_SET_XUEFEI, this, WZQDGameScence::OnSubSetXueFei);
	//addNetCBDefine(SUB_S_CHI_HU, this, WZQDGameScence::OnSubUserChiHu);
	//addNetCBDefine(SUB_S_GANG_SCORE, this, WZQDGameScence::OnSubGangScore);
	/*addNetCBDefine(SUB_S_HUANPAI, this, WZQDGameScence::OnSubHuanPai);
	addNetCBDefine(SUB_S_XUANQUE_NOTICE, this, WZQDGameScence::OnSubXuanQueNotice);
	addNetCBDefine(SUB_S_XUANQUE, this, WZQDGameScence::OnSubXuanQue);
	addNetCBDefine(SUB_S_HUANPAI_CHENGDU, this, WZQDGameScence::OnSubHuanPaiChengDu);
	addNetCBDefine(SUB_S_HUANPAI_NOTICE, this, WZQDGameScence::OnSubHuanPaiNotice);
	addNetCBDefine(SUB_S_MASTER_HANDCARD, this, WZQDGameScence::OnMasterHandCard);
	addNetCBDefine(SUB_S_MASTER_LEFTCARD, this, WZQDGameScence::OnMasterLeftCard);*/
}
//游戏开始
void WZQDGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_GameStart));
	if (wDataSize != sizeof(CMD_S_GameStart))
		return;


	//变量定义
	CMD_S_GameStart * pGameStart = (CMD_S_GameStart *)pBuffer;
	defaultState();
	m_iBankerUser = pGameStart->wBankerUser;
	m_iCurrentUser = pGameStart->wCurrentUser;
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		m_pPlayer[i]->startGame();
	}
	m_nGameState = WZQ_STATE_PLAYING;
}
//用户出牌
void WZQDGameScence::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(CMD_S_OutCard));
	if (wDataSize != sizeof(CMD_S_OutCard))
		return;

	//消息处理
	CMD_S_OutCard * pOutCard = (CMD_S_OutCard *)pBuffer;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		//m_pPlayer[i]->setActOutCard(-1);
	}

	WZQDPlayer* pPlayer = getPlayerByChairID(pOutCard->wOutCardUser);
	//pPlayer->sendOutCard(pOutCard->cbOutCardData);
	//pPlayer->blackAllHandCard(true);

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this, "TimeNode");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		WidgetFun::setVisible(pRootNode, utility::toString("TimePoint", i), false);
	}
	WidgetFun::getChildWidget(pRootNode, "ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode, "ActPlayerLastTime", "0");
}
//发牌消息
void WZQDGameScence::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_SendCard));
	if (wDataSize != sizeof(CMD_S_SendCard))
		return;

	//WidgetFun::setVisible(this, "SelfActionNode", false); //发牌了 先隐藏操作按钮
	////变量定义
	//CMD_S_SendCard * pSendCard = (CMD_S_SendCard *)pBuffer;
	//setCurrentPlayer(pSendCard->wCurrentUser, pSendCard->cbActionMask, pSendCard->cbCardData);
	//XZDDPlayer* pPlayer = getPlayerByChairID(pSendCard->wCurrentUser);
	//if (pSendCard->cbCardData != 0)
	//{
	//	pPlayer->addNewInCard(pSendCard->cbCardData);
	//	pPlayer->showHandCard();
	//	int iLastCardCout = utility::parseInt(WidgetFun::getText(this, "LastCardCout")) - 1;
	//	WidgetFun::setText(this, "LastCardCout", iLastCardCout);
	//}
	//if (pPlayer == m_pLocal&& !pSendCard->bHasHuPai)
	//{
	//	m_pLocal->blackHandCardByQueColor();
	//}
	//else if (pPlayer == m_pLocal&& pSendCard->bHasHuPai)
	//{
	//	m_pLocal->blackAllHandCard(true);
	//	if (pSendCard->cbActionMask == WIK_NULL)
	//	{
	//		TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(WZQDGameScence::OnAutoOutCard, this), 3, pSendCard->cbCardData);
	//	}
	//	else
	//	{
	//		TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(WZQDGameScence::OnAutoOperate, this), 3.f, pSendCard->cbActionMask);
	//	}
	//}
	//else
	//{
	//	m_pLocal->blackAllHandCard(true);
	//}
	//m_nGameState = XZDD_STATE_PLAYING;
}
//操作提示
void WZQDGameScence::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_OperateNotify));
	if (wDataSize != sizeof(CMD_S_OperateNotify))
		return;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify = (CMD_S_OperateNotify *)pBuffer;
	m_wResumeUser = pOperateNotify->wResumeUser;
	getPlayerByChairID(m_iBankerUser)->setZhuang();
	//setCurrentPlayer(m_pLocal->GetChairID(), pOperateNotify->cbActionMask, pOperateNotify->cbActionCard);
	/*if (pOperateNotify->bHasHuPai)
	{
		m_pLocal->blackAllHandCard(true);
		TimeManager::Instance().addCerterTimeInt(TIME_CALLBACK_Int(WZQDGameScence::OnAutoOperate, this), 3.f, pOperateNotify->cbActionMask);
	}*/
}
//操作结果
void WZQDGameScence::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(CMD_S_OperateResult));
	if (wDataSize != sizeof(CMD_S_OperateResult))
		return;

	//消息处理
	CMD_S_OperateResult * pOperateResult = (CMD_S_OperateResult *)pBuffer;

	WZQDPlayer* pPlayer = getPlayerByChairID(pOperateResult->wOperateUser);
	//pPlayer->setOperateResoult(pOperateResult);
	if (pPlayer == m_pLocal)
	{
	//	m_pLocal->blackHandCardByQueColor();
	}
	WZQDPlayer* pProvidePlayer = getPlayerByChairID(pOperateResult->wProvideUser);
	if (pProvidePlayer)
	{
		//pProvidePlayer->removeHandOutCard(pOperateResult->cbOperateCard);
		//pProvidePlayer->setActOutCard(-1);
	}

	//setCurrentPlayer(pOperateResult->wOperateUser, WIK_NULL);
	WidgetFun::setVisible(this, "SelfActionNode", false);
}
//游戏结束
void WZQDGameScence::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	m_nGameState = WZQ_STATE_NULL;
	ASSERT(wDataSize == sizeof(WZQ::CMD_S_GameEnd));
	WZQ::CMD_S_GameEnd * pOperateResult = (WZQ::CMD_S_GameEnd *)pBuffer;
	WidgetFun::setVisible(this, "FreeStateNode", true);
	WidgetFun::setVisible(this, "GameResoult2", true);
	Node* pNode = WidgetFun::getChildWidget(this,"GameResoult2");
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		if ( pOperateResult->dwChairId != m_pLocal->GetChairID() ){
			WidgetFun::setImagic(pNode,"WinTitle",WidgetFun::getWidgetUserInfo(pNode,"WinTitle","ShenLiTexture"));
			WidgetFun::setText(pNode,utility::toString("WINTxt",i),utility::a_u8("胜利"));
		}else{
			WidgetFun::setImagic(pNode,"WinTitle",WidgetFun::getWidgetUserInfo(pNode,"WinTitle","ShiBaiTexture"));
			WidgetFun::setText(pNode,utility::toString("WINTxt",i),utility::a_u8("失败"));
		}
		WidgetFun::setText(pNode,utility::toString("GoldTxt",i),utility::toString(pOperateResult->lGameScore[i]));
		WidgetFun::setText(pNode,utility::toString("name",i),utility::a_u8(m_pPlayer[i]->GetNickName()));
	}
	pGameMan->OnGameEnd(this, 3);
}

//用户托管
void WZQDGameScence::OnSubTrustee(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize == sizeof(CMD_S_Trustee));
	if (wDataSize != sizeof(CMD_S_Trustee))
		return;

	//消息处理
	CMD_S_Trustee * pTrustee = (CMD_S_Trustee *)pBuffer;
}

//用户托管
void WZQDGameScence::OnSubSetXueFei(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize == sizeof(CMD_S_Set_Success));
	if (wDataSize != sizeof(CMD_S_Set_Success))	return;	//消息处理
	CMD_S_Set_Success * pSet_Success = (CMD_S_Set_Success *)pBuffer;
	WidgetFun::setText(this,"Txt_ShuRuScore",utility::toString(utility::a_u8("学费："),pSet_Success->lXueFeiScore));
	m_WZQScore = wDataSize;
}

//
void WZQDGameScence::OnSubUserChiHu(const void *pBuffer, WORD wDataSize)//持牌胡牌逻辑
{
	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	CMD_S_ChiHu kChiHu;
	kChiHu.StreamValue(kStream, false);


	WZQDPlayer* pPlayer = getPlayerByChairID(kChiHu.wChiHuUser);
	pPlayer->showEffect("Hu");
	//pPlayer->setChiHuCard(kChiHu.cbChiHuCard);
	if (kChiHu.wChiHuUser != kChiHu.wProviderUser)
	{
		WZQDPlayer* pDestPlayer = getPlayerByChairID(kChiHu.wProviderUser);
		pPlayer->showStatusImagic("Hu");

	}
	else
	{
		pPlayer->showStatusImagic("ZiMo");
//		pPlayer->removeHandCard(kChiHu.cbChiHuCard);
	}
//	pPlayer->showHuPai(pPlayer != m_pLocal);
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		WZQDPlayer* pPlayerTemp = getPlayerByChairID(i);
		pPlayerTemp->showAddGold(kChiHu.lGameScore[i]);
	}
}
//
void WZQDGameScence::OnSubGangScore(const void *pBuffer, WORD wDataSize)
{
	datastream kStream(const_cast<void *>(pBuffer), wDataSize);
	CMD_S_GangScore kGangScore;
	kGangScore.StreamValue(kStream, false);


	for (int i = 0; i < MAX_PLAYER; i++)
	{
		int nScore = kGangScore.lGangScore[i];
		if (nScore != 0)
		{
			WZQDPlayer* pPlayer = getPlayerByChairID(i);
			pPlayer->showAddGold(nScore);
		}
	}
}

void WZQDGameScence::OnSubHuanPai(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(CMD_S_HuanPai));
	if (wDataSize != sizeof(CMD_S_HuanPai))return;

	CMD_S_HuanPai* pHuanPai = (CMD_S_HuanPai*)pBuffer;
	if (pHuanPai->wChairId == getSelfChairID())
	{
//		m_pLocal->setHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this, "ExchangeCardNode", false);
}
void WZQDGameScence::OnSubHuanPaiChengDu(const void *pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize == sizeof(CMD_S_HuanPaiChengDu));
	if (wDataSize != sizeof(CMD_S_HuanPaiChengDu))return;

	CMD_S_HuanPaiChengDu* pHuanPai = (CMD_S_HuanPaiChengDu*)pBuffer;

	//showSaiZi_HuanPai(pHuanPai->wSice);

	//XZDDButtonAction_HuanPaiChengDu(NULL,NULL);
//	m_pLocal->setHuanPai(pHuanPai->cbHuanCard);


	if (pHuanPai->wChairId == getSelfChairID())
	{
//		m_pLocal->saveRemoveHuanPai(pHuanPai->cbRemoveHuanCard);
//		m_pLocal->saveHuanPai(pHuanPai->cbHuanCard);
	}
	WidgetFun::setVisible(this, "ExchangeCardNode", false);
}

//void WZQDGameScence::OnSubXuanQueNotice(const void *pBuffer, WORD wDataSize)
//{
//	//效验消息
//	ASSERT(wDataSize == sizeof(CMD_S_XuanQueNotice));
//	if (wDataSize != sizeof(CMD_S_XuanQueNotice))return;
//
//	CMD_S_XuanQueNotice* pInfo = (CMD_S_XuanQueNotice*)pBuffer;
//	m_nLeftTime = pInfo->nLeftTime;
//
//	if (pInfo->wChairId == getSelfChairID())
//	{
//		m_pLocal->setQueColor(pInfo->nQueColor);
//	}
//	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(WZQDGameScence::DoXuanQueNotice, this), 2.f);
//}

void WZQDGameScence::OnSubXuanQue(const void *pBuffer, WORD wDataSize)//缺牌消息
{
	//效验消息
	ASSERT(wDataSize == sizeof(CMD_S_XuanQue));
	if (wDataSize != sizeof(CMD_S_XuanQue))return;

	CMD_S_XuanQue* pInfo = (CMD_S_XuanQue*)pBuffer;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		cocos2d::log(" user chairid :%d", i);
		cocos2d::log(" user nQueColor:%d", (int)pInfo->nQueColor[i]);

		WZQDPlayer* pPlayer = getPlayerByChairID(i);
	}

}

//void WZQDGameScence::OnSubHuanPaiNotice(const void *pBuffer, WORD wDataSize)
//{
//	showHuanPai();
//	//	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(WZQDGameScence::showHuanPai,this),4.2f);
//}
//
//void WZQDGameScence::showHuanPai()
//{
//	ASSERT(m_pLocal);
//
//	WidgetFun::setVisible(this, "ExchangeCardNode", true);
//	m_pLocal->showHuanPai(GAME_TYPE_CHENGDU);
//	m_nGameState = XZDD_STATE_HUANPAI;
//}
//
