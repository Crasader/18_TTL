#include "WZQDGameScence.h"
#include "GAME.h"
#include "CMD_WZQ.h"
#include "WZQDPlayer.h"
#include "WZQDGameLogic.h"
#include UTILITY_CONVERT

using namespace WZQ;
bool WZQDGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_WZQ_FREE)
	{
		OnFreeScence(data,dataSize);
		m_GameZT = GS_WZQ_FREE;
	}
	/*if(cbGameStatus == GS_MJ_HUANPAI)
	{
		OnHuanPaiScence(data,dataSize);
		m_GameZT = GS_MJ_HUANPAI;
	}
	if (cbGameStatus == GS_MJ_XUANQUE)
	{
		OnXuanQueScence(data,dataSize);
		m_GameZT = GS_MJ_XUANQUE;
	}*/
	if (cbGameStatus == GS_WZQ_PLAY)
	{
		OnPlayScence(data,dataSize);
		m_GameZT = GS_WZQ_PLAY;
	}
	return true;
}
void WZQDGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return;

	CMD_S_StatusFree* pNetInfo = (CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);

	m_nGameState = WZQ_STATE_NULL;
}

void WZQDGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	if (m_pLocal->GetUserStatus() >= US_READY )
	{
		//WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}

void WZQDGameScence::OnPlayScence(void* data, int wDataSize)//重新加入游戏场景消息
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return;

	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;
	defaultState();
	m_nGameState = WZQ_STATE_PLAYING;
	m_iBankerUser=pNetInfo->wBankerUser;
	m_WZQScore = pNetInfo->lScorePay;
	WidgetFun::setText(this,"Txt_ShuRuScore",utility::toString(utility::a_u8("学费："),m_WZQScore));

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WZQDPlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		pPlayer->upPlayerInfo();
	}
	
}
