#include "Kernel/kernel/server/CServerItem.h"
#include "GameLib/Game/Game/GameManagerBase.h"
#include "Tools/utilityLog.h"
#include "Platform/PFDefine/data/GlobalUserInfo.h"

//////////////////////////////////////////////////////////////////////////
//用户接口

//自己位置
word CServerItem::GetMeChairID()
{
	if (m_pMeUserItem == 0) return INVALID_CHAIR;
	return m_pMeUserItem->GetChairID();
}

//自己位置
IClientUserItem * CServerItem::GetMeUserItem()
{
	return m_pMeUserItem;
}

//游戏用户
IClientUserItem * CServerItem::GetTableUserItem(word wChairID)
{
	return mUserManager->EnumUserItem(wChairID);
}

//查找用户
IClientUserItem * CServerItem::SearchUserByUserID(dword dwUserID)
{
	return mUserManager->SearchUserByUserID(dwUserID);
}

//查找用户
IClientUserItem * CServerItem::SearchUserByGameID(dword dwGameID)
{
	return mUserManager->SearchUserByGameID(dwGameID);
}

//查找用户
IClientUserItem * CServerItem::SearchUserByNickName(const char* szNickName)
{
	return mUserManager->SearchUserByNickName(szNickName);
}
///< 获取用户数
dword CServerItem::GetActiveUserCount()
{
	return mUserManager->GetActiveUserCount();
}

//获取对应桌子是否锁的状态
bool CServerItem::GetTableLockState(int tableId)
{
	return m_TableFrame.GetLockerFlag(tableId);
}

//获取对应桌子的游戏状态
bool CServerItem::GetTableGameState(int tableId)
{
	return m_TableFrame.GetPlayFlag(tableId);
}

//////////////////////////////////////////////////////////////////////////
// IUserManagerSink
//////////////////////////////////////////////////////////////////////////
void CServerItem::OnUserItemAcitve(IClientUserItem* pIClientUserItem)
{
	//判断自己
	if (m_pMeUserItem==0)
	{
		m_pMeUserItem=pIClientUserItem;
	}

	//设置桌子
	byte cbUserStatus=pIClientUserItem->GetUserStatus();
	if ((cbUserStatus>=US_SIT)&&(cbUserStatus!=US_LOOKON))
	{
		word wTableID=pIClientUserItem->GetTableID();
		word wChairID=pIClientUserItem->GetChairID();
		m_TableFrame.SetClientUserItem(wTableID,wChairID,pIClientUserItem);
	}

	//提示信息
	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();
	if ((pParameterGlobal->m_bNotifyUserInOut==true)&&(mServiceStatus==ServiceStatus_ServiceIng))
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertUserEnter(pIClientUserItem->GetNickName());
	}

 	if (mIServerItemSink)
 		mIServerItemSink->OnGRUserEnter(pIClientUserItem);

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventUserEnter(pIClientUserItem, 
		pIClientUserItem->GetUserStatus() == US_LOOKON);
}

void CServerItem::OnUserItemDelete(IClientUserItem* pIClientUserItem)
{
	//变量定义
	word wLastTableID=pIClientUserItem->GetTableID();
	word wLastChairID=pIClientUserItem->GetChairID();
	dword dwLeaveUserID=pIClientUserItem->GetUserID();

	//变量定义
	ASSERT(CParameterGlobal::shared());
	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserDelete(pIClientUserItem);

	//提示信息
	if ((pParameterGlobal->m_bNotifyUserInOut==true)&&(mServiceStatus==ServiceStatus_ServiceIng))
	{
		if (mIStringMessageSink)
			mIStringMessageSink->InsertUserLeave(pIClientUserItem->GetNickName());
	}

	if (mIClientKernelSink)
		mIClientKernelSink->OnEventUserLeave(pIClientUserItem,pIClientUserItem->GetUserStatus() == US_LOOKON);

	if (m_pMeUserItem == pIClientUserItem)
	{
		m_pMeUserItem = NULL;
	}
}

void CServerItem::OnUserFaceUpdate(IClientUserItem * pIClientUserItem)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//更新桌子
	if ((pUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->cbUserStatus!=US_LOOKON))
	{
		m_TableFrame.UpdateTableView(pUserInfo->wTableID);
	}
}

void CServerItem::OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserScore& LastScore)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pMeUserItem->GetUserInfo();

	//房间界面通知
	if (pIClientUserItem==m_pMeUserItem)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
		tagUserInsureInfo * pUserInsureData=pGlobalUserInfo->GetUserInsureInfo();

		//设置变量
		pUserInsureData->lUserScore+=pIClientUserItem->GetUserScore()-LastScore.lScore;	
		pUserInsureData->lUserInsure+=pIClientUserItem->GetUserInsure()-LastScore.lInsure;
		pGlobalUserData->lUserScore = pUserInsureData->lUserScore;
		pGlobalUserData->lUserInsure = pUserInsureData->lUserInsure;
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//游戏通知
	if ((pMeUserInfo->wTableID!=INVALID_TABLE)&&(pUserInfo->wTableID==pMeUserInfo->wTableID))
	{
		if (mIClientKernelSink)
			mIClientKernelSink->OnEventUserScore(pIClientUserItem,pIClientUserItem->GetUserStatus() == US_LOOKON);
	}
}

void CServerItem::OnUserItemUpdate(IClientUserItem* pIClientUserItem, const tagUserStatus& LastStatus)
{
	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagUserInfo * pMeUserInfo=m_pMeUserItem->GetUserInfo();
	//用户自己的状态
	byte pMeUserStatus = m_pMeUserItem->GetUserStatus();
	//更新用户信息
	word wNowTableID=pIClientUserItem->GetTableID(),wLastTableID=LastStatus.wTableID;
	word wNowChairID=pIClientUserItem->GetChairID(),wLastChairID=LastStatus.wChairID;
	byte cbNowStatus=pIClientUserItem->GetUserStatus(),cbLastStatus=LastStatus.cbUserStatus;

	// 更新界面上的分数
	if (mIServerItemSink)
	{
		utility::filelog("CServerItem::OnUserItemUpdate OnGRUserUpdate");
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);
	}
	
	//桌子离开
	if ((wLastTableID!=INVALID_TABLE)&&((wLastTableID!=wNowTableID)||(wLastChairID!=wNowChairID)))
	{
		utility::filelog("CServerItem::OnUserItemUpdate out table");
		IClientUserItem * pITableUserItem=m_TableFrame.GetClientUserItem(wLastTableID,wLastChairID);
		if (pITableUserItem==pIClientUserItem)
			m_TableFrame.SetClientUserItem(wLastTableID,wLastChairID,0);
	}

	//桌子加入
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		utility::filelog("CServerItem::OnUserItemUpdate enter table");
		//厌恶判断（黑名单）
		if(pUserInfo->dwUserID != pMeUserInfo->dwUserID && cbNowStatus == US_SIT && pMeUserInfo->wTableID == wNowTableID)
		{
			//变量定义
			ASSERT(CParameterGlobal::shared()!=0);
			CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();
		}
		m_TableFrame.SetClientUserItem(wNowTableID,wNowChairID,pIClientUserItem);
	}
	
	//桌子状态
	if ((m_TableFrame.GetChairCount() < MAX_CHAIR)&&(wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		utility::filelog("CServerItem::OnUserItemUpdate UpdateTableView");
		m_TableFrame.UpdateTableView(wNowTableID);
	}

	//DONE:玩家自己从大厅返回游戏,先要创建桌子,重新展示一下游戏房间
	if (pUserInfo->dwUserID == pMeUserInfo->dwUserID &&
		pMeUserStatus == cbLastStatus &&
		cbLastStatus == US_SIT &&
		pGameMan->getInCenter())
	{
		utility::filelog("CServerItem::OnUserItemUpdate CreateGame");
		pGameMan->CreateGame();
		IServerItem::get()->OnGFGameReady();
	}

	if (wNowTableID != wLastTableID || wNowChairID != wLastChairID)
	{
		if (wLastTableID != INVALID_TABLE)
		{
			//游戏通知
			if ((pMeUserInfo->wTableID != INVALID_TABLE) && (pUserInfo->wTableID == pMeUserInfo->wTableID))
			{
				utility::filelog("CServerItem::OnUserItemUpdate wLastTableID != INVALID_TABLE");
				if (mIClientKernelSink)
					mIClientKernelSink->OnEventUserStatus(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);
			}
		}
		//离开桌子
		if (wNowTableID == INVALID_TABLE)
		{
			if (m_pMeUserItem == pIClientUserItem)
			{
				utility::filelog("CServerItem::OnUserItemUpdate wNowTableID == INVALID_TABLE");
				//设置变量
				m_wReqTableID = INVALID_TABLE;
				m_wReqChairID = INVALID_CHAIR;
				OnGFRoomClose(0);
			}
		}
		//进入新的桌子
		else //wNowTableID != INVALID_TABLE
		{
			//自己加入新的桌子
			if (m_pMeUserItem == pIClientUserItem)
			{
				utility::filelog("CServerItem::OnUserItemUpdate m_pMeUserItem == pIClientUserItem");
				//设置变量
				m_wReqTableID = INVALID_TABLE;
				m_wReqChairID = INVALID_CHAIR;
				//启动进程
				if (!mIServerItemSink || !mIServerItemSink->StartGame())
				{
					OnGFRoomClose(GameExitCode_CreateFailed);
					return;
				}
			}
			//其他玩家加入新的桌子
			else
			{
				//游戏通知
				if (pMeUserInfo->wTableID == wNowTableID)
				{
					utility::filelog("CServerItem::OnUserItemUpdate pMeUserInfo->wTableID == wNowTableID");
					if (mIClientKernelSink)
						mIClientKernelSink->OnEventUserEnter(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);
				}
			}
			return;
		}
	}

	//状态改变
	if ((wNowTableID!=INVALID_TABLE)&&(wNowTableID==wLastTableID)&&(pMeUserInfo->wTableID==wNowTableID))
	{
		utility::filelog("CServerItem::OnUserItemUpdate OnEventUserStatus status changed");
		//游戏通知
		tagUserStatus UserStatus;
		UserStatus.wTableID=wNowTableID;
		UserStatus.wChairID=wNowChairID;
		UserStatus.cbUserStatus=cbNowStatus;

		if (mIClientKernelSink)
			mIClientKernelSink->OnEventUserStatus(pIClientUserItem, pIClientUserItem->GetUserStatus() == US_LOOKON);

		return;
	}
	
	return;
}

//用户更新
void CServerItem::OnUserItemUpdate(IClientUserItem * pIClientUserItem, const tagUserAttrib & UserAttrib)
{
	//变量定义
	word wMeTableID=m_pMeUserItem->GetTableID();
	word wUserTableID=pIClientUserItem->GetTableID();

	if (mIServerItemSink)
		mIServerItemSink->OnGRUserUpdate(pIClientUserItem);

	//通知游戏
	if ((wMeTableID!=INVALID_TABLE)&&(wMeTableID==wUserTableID))
	{
		//变量定义
		tagUserAttrib UserAttrib;
		UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

		//发送通知
		if (mIClientKernelSink)
			mIClientKernelSink->OnEventUserScore(pIClientUserItem,
			pIClientUserItem->GetUserStatus() == US_LOOKON);
	}
	return;
}
