#include "GameBase.h"

#include "GamePlayer.h"
#include "Tools/utilityMath.h"
#include "Kernel/kernel/server/IServerItem.h"
#include "Platform/PFDefine/df/Define.h"
#include "Platform/PFDefine/df/DF.h"

GamePlayer* GameBase::getPoolPlayer(IClientUserItem * pIClientUserItem)
{
	GamePlayer* pPlayer = NULL;
	if (m_kPoolPlayer.size())
	{
		pPlayer = m_kPoolPlayer.back();
		m_kPoolPlayer.pop_back();
		pPlayer->setUserItem(pIClientUserItem);
	}
	return pPlayer;
}
GamePlayer* GameBase::CreatePlayer(IClientUserItem * pIClientUserItem)
{
	GamePlayer* pPlayer = getPoolPlayer(pIClientUserItem);
	if (pPlayer)
	{
		return pPlayer;
	}
	return new GamePlayer(pIClientUserItem);
}
void GameBase::DeletePlayer(GamePlayer* pPlayer)
{
	if (utility::haveInVector(m_kPoolPlayer,pPlayer))
	{
		pPlayer->clear();
		return;
	}
	m_kPoolPlayer.push_back(pPlayer);
}
void GameBase::LocalPlayerEnter()
{
	CCAssert(m_pSelfPlayer,"");
	if (!m_pSelfPlayer)
	{
		return;
	}
	if (m_pSelfPlayer->GetUserStatus() < US_READY)
	{
		sendReady();
	}
}
GamePlayer* GameBase::getGamePlayerByUserItem(IClientUserItem * pIClientUserItem)
{
	std::vector<GamePlayer*>::iterator itor = m_kPlayers.begin();
	for (;itor != m_kPlayers.end();itor++)
	{
		if ((*itor)->getUserItem() == pIClientUserItem)
		{
			return (*itor);
		}
	}
	return NULL;
}
GamePlayer* GameBase::getPlayerByChairID( word wChairID )
{
	std::vector<GamePlayer*>::iterator itor = m_kPlayers.begin();
	for (;itor != m_kPlayers.end();itor++)
	{
		if ((*itor)->GetChairID() == wChairID )
		{
			return (*itor);
		}
	}
	return NULL;
}
GamePlayer* GameBase::getPlayerByUserID( dword wUserID )
{
	std::vector<GamePlayer*>::iterator itor = m_kPlayers.begin();
	for (;itor != m_kPlayers.end();itor++)
	{
		if ((*itor)->GetUserID() == wUserID )
		{
			return (*itor);
		}
	}
	return NULL;
}

void GameBase::addGamePlayerToList(GamePlayer* pPlayer)
{
	CCAssert(pPlayer->getUserItem(),"");
	if (!utility::haveInVectorByValue(m_kPlayers,pPlayer))
	{
		m_kPlayers.push_back(pPlayer);
	}
	pPlayer->PlayerEnter();
	pPlayer->upPlayerInfo();
	pPlayer->upPlayerState();
	pPlayer->requesUserInfo();
	//TODO:干掉了查询
	//m_kReqPlayerInfo.query(pPlayer->GetUserID());
}
void GameBase::removeGamePlayerToList(GamePlayer* pPlayer)
{
	if (m_pSelfPlayer == pPlayer)
	{
		m_pSelfPlayer = NULL;
	}
	utility::removeVectorByValue(m_kPlayers,pPlayer);
	pPlayer->PlayerLeave();
	pPlayer->setUserItem(NULL);
}

void GameBase::getGamePlayerToList(std::vector<GamePlayer*>* kPlayers)
{
	for (auto itor =m_kPlayers.begin();itor != m_kPlayers.end();itor++)
	{
		kPlayers->push_back(*itor);
	}

	for (auto itor = kPlayers->begin();itor != kPlayers->end();itor++)
	{
		for (auto itor1 = itor+1;itor1 != kPlayers->end();itor1++)
		{
			word chairid1= (*itor)->GetChairID();
			word chairid2= (*itor1)->GetChairID();
			if ( chairid1 > chairid2 )
			{
				std::swap(*itor,*itor1);
			}
		}
	}
}

GamePlayer* GameBase::getSelfGamePlayer()
{
	return m_pSelfPlayer;
}
word GameBase::getSelfChairID()
{
	if (m_pSelfPlayer == NULL)
	{
		return 0;
	}
	return m_pSelfPlayer->GetChairID();
}
bool GameBase::IsPrivateGame()
{
	return IServerItem::get()->GetServerAttribute().wServerType == GAME_GENRE_EDUCATE;
}
//用户进入
void GameBase::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getGamePlayerByUserItem(pIClientUserItem);
	if (pPlayer) {
		//TODO:干掉了查询
		//m_kReqPlayerInfo.query(pIClientUserItem->GetUserID());
		pPlayer->requesUserInfo();
		pPlayer->upPlayerInfo();
		return;
	}
	IClientUserItem * pMeItem = IServerItem::get()->GetMeUserItem();
	if (!pMeItem) {
		return;
	}
	if (pMeItem->GetTableID() != pIClientUserItem->GetTableID()) {
		return;
	}
	if (pIClientUserItem->GetUserStatus() == US_LOOKON) {
		return;
	}
	//如果是自己进入桌子
	if (IServerItem::get()->GetMeUserItem() == pIClientUserItem) {
		CCAssert(m_pSelfPlayer == nullptr,"");
		m_pSelfPlayer = CreatePlayer(pIClientUserItem);
		addGamePlayerToList(m_pSelfPlayer);
#if defined(ENTER_GTAME_STATE_READY)
		LocalPlayerEnter();
#endif
		upSelfPlayerInfo();
		int iIdex = 0;
		while(true) {
			IClientUserItem* pTempUserItem = IServerItem::get()->GetTableUserItem(iIdex);
			iIdex++;
			if (!pTempUserItem) {
				break;
			}
			if (pTempUserItem->GetTableID() != pMeItem->GetTableID()) {
				continue;
			}
			if (pTempUserItem->GetUserStatus() == US_LOOKON) {
				continue;
			}
			if (pTempUserItem == pIClientUserItem) {
				continue;
			}
			GamePlayer* pTempPlayer = CreatePlayer(pTempUserItem);
			addGamePlayerToList(pTempPlayer);
		}
	} else {
		if (m_pSelfPlayer) {
			GamePlayer* pTempPlayer = CreatePlayer(pIClientUserItem);
			addGamePlayerToList(pTempPlayer);
		}
	}
#if defined(ENTER_GTAME_STATE_READY)
	LocalPlayerEnter();
#endif
	upSelfPlayerInfo();
}
//用户离开
void GameBase::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getGamePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		removeGamePlayerToList(pPlayer);
		DeletePlayer(pPlayer);
	}
}
//用户积分
void GameBase::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getGamePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerInfo();
	}
	if (pPlayer == m_pSelfPlayer)
	{
		upSelfPlayerInfo();
	}
}
//用户状态
void GameBase::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getGamePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerState();
		OnEventUserStatus(pPlayer);
	}
}
//用户属性
void GameBase::OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getGamePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerInfo();
	}
	if (pPlayer == m_pSelfPlayer)
	{
		upSelfPlayerInfo();
	}
}
//用户头像
void GameBase::OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	GamePlayer* pPlayer = getGamePlayerByUserItem(pIClientUserItem);
	if (pPlayer)
	{
		pPlayer->upPlayerInfo();
	}
}

//void GameBase::on_GP_UserIndividual(dword dwUserID, std::string strIP,std::string strHttp)
//{
//
//	GamePlayer * pPlayer = getPlayerByUserID(dwUserID);
//	if (!pPlayer)
//	{
//		return;
//	}
//	IClientUserItem * pIClientUserItem = pPlayer->getUserItem(false);
//	if (!pIClientUserItem)
//	{
//		return;
//	}
//
//	//获取用户
//	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
//	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();
//
//	strncpy(pUserInfo->szLogonIP,strIP.c_str(), countarray(pUserInfo->szLogonIP));
//	strncpy(pUserInfo->szHeadHttp,strHttp.c_str(), countarray(pUserInfo->szHeadHttp));
//
//	pPlayer->upPlayerInfo();
//}