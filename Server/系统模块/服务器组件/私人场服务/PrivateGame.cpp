#include "StdAfx.h"
#include "PrivateGame.h"
#include "FangKaHttpUnits.h"
#include "..\游戏服务器\DataBasePacket.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//时钟定义
#define IDI_PRIVATE_ROOM_TIMER_EVENT         (IDI_PRIVATE_MODULE_START+1)

#if defined(ALLOWED_KICK_UNREADY_USER)
//创建定时器的Timer ID
#define IDI_CREATE_TIMER_THREAD_EVENT        (IDI_PRIVATE_MODULE_START+2)

#define CHECK_READY_STATUS_TIME_FRAME     4 * 1000      //检查准备状态最小时间间隔

#define CHECK_READY_TIME                                30 * 1000                            //检查准备状态时间间隔
#endif

#define DISMISS_WAITE_TIME		                        60					//请求解散时间

#define AGAIN_WAITE_TIME		                        30					//重新开始等待时间

#define OFFLINE_WAITE_TIME		                        60*60*24			//全部掉线等待时间

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//构造函数
PriaveteGame::PriaveteGame()
#if defined(ALLOWED_KICK_UNREADY_USER)
    : m_minTimeFrame(CHECK_READY_STATUS_TIME_FRAME)
    , m_checkReadyTime(CHECK_READY_TIME)
#endif
{
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	//内核接口
	m_pTableInfo=NULL;
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//服务接口
	m_pIGameServiceFrame=NULL;
	m_pIServerUserManager=NULL;
	m_pAndroidUserManager=NULL;
}

PriaveteGame::~PriaveteGame(void)
{
	//释放资源
	SafeDeleteArray(m_pTableInfo);

	//关闭定时器
	m_pITimerEngine->KillTimer(IDI_PRIVATE_ROOM_TIMER_EVENT);

#if defined(ALLOWED_KICK_UNREADY_USER)
    while(!m_timerIDs.empty())
    {
        m_pITimerEngine->KillTimer(m_timerIDs.front());
        m_timerIDs.pop();
    }
#endif
}

//接口查询
VOID* PriaveteGame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{	
	QUERYINTERFACE(IGamePrivateItem,Guid,dwQueryVer);
	QUERYINTERFACE(IPrivateEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);	
	QUERYINTERFACE_IUNKNOWNEX(IGamePrivateItem,Guid,dwQueryVer);
	return NULL;
}

//绑定桌子
bool PriaveteGame::BindTableFrame(ITableFrame * pTableFrame,WORD wTableID)
{
	if(pTableFrame==NULL || wTableID>m_pGameServiceOption->wTableCount)
	{
		ASSERT(false);
		return false;
	}

	//创建钩子
	CTableFramePrivate * pTableFrameHook=new CTableFramePrivate();
	pTableFrameHook->InitTableFramePrivate(QUERY_OBJECT_PTR_INTERFACE(pTableFrame,IUnknownEx));
	pTableFrameHook->SetPrivateEventSink(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	//设置接口
	pTableFrame->SetTableFramePrivate(QUERY_OBJECT_PTR_INTERFACE(pTableFrameHook,IUnknownEx));
	m_pTableInfo[wTableID].pITableFrame=pTableFrame;
	pTableFrame->SetTableInfoExtra(&m_pTableInfo[wTableID]);
	m_pTableInfo[wTableID].restValue();

	return true;
}

//初始化接口
bool PriaveteGame::InitPrivateInterface(tagPrivateManagerParameter & MatchManagerParameter)
{
	m_pGameServiceOption=MatchManagerParameter.pGameServiceOption;
	m_pGameServiceAttrib=MatchManagerParameter.pGameServiceAttrib;

	//内核组件
	m_pITimerEngine=MatchManagerParameter.pITimerEngine;
	m_pIDataBaseEngine=MatchManagerParameter.pICorrespondManager;
	m_pITCPNetworkEngineEvent=MatchManagerParameter.pTCPNetworkEngine;

	//服务组件		
	m_pIGameServiceFrame=MatchManagerParameter.pIMainServiceFrame;		
	m_pIServerUserManager=MatchManagerParameter.pIServerUserManager;
	m_pAndroidUserManager=MatchManagerParameter.pIAndroidUserManager;
	m_pIServerUserItemSink=MatchManagerParameter.pIServerUserItemSink;

	//创建桌子
	if (m_pTableInfo==NULL)
	{
		m_pTableInfo = new PrivateTableInfo[m_pGameServiceOption->wTableCount];
	}

	return true;
}
void PriaveteGame::OnStartService()
{
	//变量定义
	DBR_GR_Private_Info kPrivateInfo;
	ZeroMemory(&kPrivateInfo,sizeof(kPrivateInfo));
	kPrivateInfo.wKindID=m_pGameServiceOption->wKindID;
	m_pIDataBaseEngine->PostDataBaseRequest(0L,DBR_GR_PRIVATE_INFO,0L,&kPrivateInfo,sizeof(kPrivateInfo));

	m_pITimerEngine->SetTimer(IDI_PRIVATE_ROOM_TIMER_EVENT,5000L,TIMES_INFINITY,0);

#if defined(ALLOWED_KICK_UNREADY_USER)
    //创建建立定时器的线程
    int iCheckNum = m_checkReadyTime/m_minTimeFrame;
    m_pITimerEngine->SetTimer(IDI_CREATE_TIMER_THREAD_EVENT,m_minTimeFrame,iCheckNum,0);

    //将检测玩家是否准备的定时器ID装进队列
    for(int i = 0; i <iCheckNum; i++ )
    {
        m_timerIDs.push(IDI_CREATE_TIMER_THREAD_EVENT + i + 1);
    }
#endif
}

//时间事件
bool PriaveteGame::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{	
	switch(dwTimerID)
	{
	case IDI_PRIVATE_ROOM_TIMER_EVENT:				//解散等待时间 10s
		{
			for(int i = 0;i<m_pGameServiceOption->wTableCount;i++)
			{
				PrivateTableInfo* pTableInfo = &m_pTableInfo[i];
				if (pTableInfo->bInEnd)
				{
					pTableInfo->fAgainPastTime += 5.0f;
					if (pTableInfo->fAgainPastTime >= AGAIN_WAITE_TIME)
					{
						DismissRoom(pTableInfo);
						ClearRoom(pTableInfo);
					}
				}
				if (pTableInfo->kDismissChairID.size())
				{
					pTableInfo->fDismissPastTime += 5.0f;
					if (pTableInfo->fDismissPastTime >= DISMISS_WAITE_TIME)
					{
						if (pTableInfo->kNotAgreeChairID.size() <= 1)
						{
							DismissRoom(pTableInfo);
							ClearRoom(pTableInfo);
						}
					}
				}
				if (pTableInfo->IsAllOffline())
				{
					pTableInfo->fOfflinePastTime += 5.0f;
					if (pTableInfo->fOfflinePastTime >= OFFLINE_WAITE_TIME)
					{
						DismissRoom(pTableInfo);
						ClearRoom(pTableInfo);
					}
				}
			}
			return true;
		}
#if defined(ALLOWED_KICK_UNREADY_USER)
    case IDI_CREATE_TIMER_THREAD_EVENT:
        {
            DWORD curID = m_timerIDs.front(); m_timerIDs.pop();
            m_pITimerEngine->SetTimer(curID,m_checkReadyTime, TIMES_INFINITY, 0);
            m_timerIDs.push(curID);
            break;
        }
    default:
        {
            if(m_timerIDs.front() == dwTimerID){
                //遍历所有的桌子，看是否有桌子到了检查准备的时间
                for (WORD tc = 0; tc < m_pGameServiceOption->wTableCount; tc++){
                    PrivateTableInfo& pTable = m_pTableInfo[tc];

                    ITableFrame* pTableFrame = pTable.pITableFrame;
                    if(!pTableFrame) continue;

                    if (pTableFrame->GetGameStatus() < GAME_STATUS_PLAY || pTableFrame->GetGameStatus() >= GAME_STATUS_WAIT ){
                        //检测是否所有玩家已经准备，否则踢出玩家
                        bool bKickUser = false;
                        for(int idx = 0; idx < pTable.getChairCout(); idx++){
                            //如果当前座位的玩家计时器已经达到
                            if(m_timerIDs.front() != pTable.m_TimerEventIDs[idx])   continue;

                            IServerUserItem* pSevItem = pTableFrame->GetTableUserItem(idx);
                            if(!pSevItem)   continue;
                            //该玩家还是处于未准备状态
                            if((pSevItem->GetUserStatus() == US_SIT || pSevItem->GetUserStatus() == US_FREE || pSevItem->GetUserStatus() == US_OFFLINE)
#ifndef KICK_ROOM_OWNER //如果不允许踢房主
								&& pTableFrame->GetCreateUserID() != pSevItem->GetUserID()
#endif
								){
                                //如果此人是房主，任命下一届房主
                                pTableFrame->SwitchRoomCreater(pSevItem);
                                pTableFrame->PerformStandUpActionReally(pSevItem, US_NULL);//设置
                                m_pIGameServiceFrame->SendRoomMessage(pSevItem,TEXT("对不起，亲，你因为长时间没准备，被系统请出房间。"), SMT_EJECT);
                                bKickUser = true;
                            }
                        }
                        if(bKickUser){
                            for(int ci = 0; ci < pTable.getChairCout(); ci++){
                                IServerUserItem* pSevItemInfo = pTableFrame->GetTableUserItem(ci);
                                if(!pSevItemInfo)   continue;
                                sendPrivateRoomInfo(pSevItemInfo, &pTable);
                            }
                        }
                    }
                }
                DWORD curID = m_timerIDs.front(); 
                m_timerIDs.pop();
                m_timerIDs.push(curID);
            }
            break;
        }
#endif
	}
	return true;
}

//发送数据
bool PriaveteGame::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(pIServerUserItem!=NULL)
		return m_pIGameServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	//DONE:这个操作非常浪费
	//else
	//	return m_pIGameServiceFrame->SendData(BG_ALL_CLIENT, wMainCmdID,wSubCmdID, pData, wDataSize);

	return true;
}

bool PriaveteGame::SendTableData(ITableFrame*	pITableFrame, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return pITableFrame->SendTableData(INVALID_CHAIR,wSubCmdID,pData,wDataSize,wMainCmdID);
}

void PriaveteGame::CreatePrivateCost(PrivateTableInfo* pTableInfo)
{
	if (pTableInfo->bRoomType == Type_Private)
	{
		//DONE: 这里扣除了房卡或金币
		DBR_GR_Create_Private_Cost kNetInfo;
		kNetInfo.dwUserID = pTableInfo->pITableFrame->GetCreateUserID();
		//需要扣除金币
		if (m_kPrivateInfo.lCostGold != 0)
		{
			kNetInfo.dwCostType = 1;
			kNetInfo.dwCost = static_cast<DWORD>(m_kPrivateInfo.lCostGold);
		}
		//需要扣除房卡
		else
		{
			kNetInfo.dwCostType = 0;
			kNetInfo.dwCost = static_cast<DWORD>(m_kPrivateInfo.lPlayCost[pTableInfo->bPlayCoutIdex]);
		}
        m_pIDataBaseEngine->PostDataBaseRequest(0L,DBR_GR_CREATE_PRIVAT_COST,0L,&kNetInfo,sizeof(kNetInfo));
	}
	if (pTableInfo->bRoomType == Type_Public)
	{
		for (int i = 0;i<pTableInfo->pITableFrame->GetSitUserCount();i++)
		{
			if (!pTableInfo->pITableFrame->GetTableUserItem(i))
			{
				continue;
			}
			DBR_GR_Create_Private_Cost kNetInfo;
			kNetInfo.dwUserID = pTableInfo->pITableFrame->GetTableUserItem(i)->GetUserID();
            kNetInfo.dwCost = pTableInfo->dwPlayCost;
            kNetInfo.dwCostType = pTableInfo->bRoomType;
            m_pIDataBaseEngine->PostDataBaseRequest(0L,DBR_GR_CREATE_PRIVAT_COST,0L,&kNetInfo,sizeof(kNetInfo));
		}
	}
}

PrivateTableInfo* PriaveteGame::getTableInfoByRoomID(DWORD dwRoomID)
{
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if (m_pTableInfo[i].dwRoomNum == dwRoomID)
		{
			return &m_pTableInfo[i];
		}
	}
	return NULL;
}

PrivateTableInfo* PriaveteGame::getTableInfoByCreaterID(DWORD dwUserID)
{
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if (m_pTableInfo[i].pITableFrame->GetCreateUserID() == dwUserID)
		{
			return &m_pTableInfo[i];
		}
	}
	return NULL;
}

PrivateTableInfo* PriaveteGame::getTableInfoByUserID(DWORD dwUserID)
{
    for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
    {
        if(m_pTableInfo[i].pITableFrame->SearchUserItem(dwUserID))
        {
            return &m_pTableInfo[i];
        }
    }
    return NULL;
}

PrivateTableInfo* PriaveteGame::getTableInfoByTableID(DWORD dwRoomID)
{
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if (m_pTableInfo[i].pITableFrame && m_pTableInfo[i].pITableFrame->GetTableID() == dwRoomID)
		{
			return &m_pTableInfo[i];
		}
	}
	return NULL;
}

PrivateTableInfo* PriaveteGame::getTableInfoByTableFrame(ITableFrame* pTableFrame)
{
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if (m_pTableInfo[i].pITableFrame == pTableFrame)
		{
			return &m_pTableInfo[i];
		}
	}
	return NULL;
}

//数据库事件
bool PriaveteGame::OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GR_PRIVATE_INFO:		//私人场信息
		{
			//参数效验
			if(wDataSize>sizeof(DBO_GR_Private_Info)) return false;

			//提取数据
			DBO_GR_Private_Info * pPrivate = (DBO_GR_Private_Info*)pData;
			m_kPrivateInfo.wKindID = pPrivate->wKindID;
			m_kPrivateInfo.lCostGold = pPrivate->lCostGold;

			for (int i = 0; i < CountArray((m_kPrivateInfo.bPlayCout)); i++)
			{
#if defined(ALLOWED_PLAY_COUNT_UNLIMITED)
				m_kPrivateInfo.bPlayCout[i] = USHRT_MAX;
#else
				m_kPrivateInfo.bPlayCout[i] = pPrivate->bPlayCout[i];
#endif
			}

#if defined(ALLOWED_CREATE_ROOM_NO_COST)
            for (int i = 0; i < CountArray( (m_kPrivateInfo.lPlayCost) ); i++)
            {
                m_kPrivateInfo.lPlayCost[i] = 0;
            }
#else
            memcpy(&m_kPrivateInfo.lPlayCost,pPrivate->lPlayCost,sizeof(m_kPrivateInfo.lPlayCost));
#endif
			break;
		}
	case DBO_GR_CREATE_PRIVATE:		//私人场信息
		{
			OnEventCreatePrivate(wRequestID,pIServerUserItem,pData,wDataSize,"");
			break;
		}
    case DBR_GR_PRIVAT_REFRESH_TABLES_END:
        {
            OnEventRefreshTable(wRequestID,pIServerUserItem,pData,wDataSize,"");
            break;
        }
	}
	return true;
}

bool PriaveteGame::joinPrivateRoom(IServerUserItem * pIServerUserItem, ITableFrame * pICurrTableFrame, PrivateTableInfo* pTableInfo)
{
    ASSERT(pIServerUserItem);

	WORD wGaveInChairID = INVALID_CHAIR;
	for (WORD i=0;i<pICurrTableFrame->GetChairCount();i++)
	{
		if (pICurrTableFrame->GetTableUserItem(i)==pIServerUserItem)
		{
			wGaveInChairID = i;
			break;
		}
	}

	if (wGaveInChairID!=INVALID_CHAIR)
	{
		pIServerUserItem->SetUserStatus(US_SIT, pICurrTableFrame->GetTableID(),wGaveInChairID);
		return true;
	}
	WORD wChairID = INVALID_CHAIR;
	//椅子搜索
	for (WORD i=0;i<pICurrTableFrame->GetChairCount();i++)
	{
		if (pICurrTableFrame->GetTableUserItem(i)==NULL)
		{
			wChairID = i;
			break;
		}
	}

	//分配用户
	if (wChairID!=INVALID_CHAIR)
	{
		//用户起立
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			return pIServerUserItem->GetTableID() == pICurrTableFrame->GetTableID();
		}

		//用户坐下
		if(pICurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
		{
			return false;
		}
		//pIServerUserItem->SetUserStatus(US_READY,pICurrTableFrame->GetTableID(),wChairID);
#if defined(ALLOWED_KICK_UNREADY_USER)
        if(pTableInfo)
        {
            pTableInfo->m_TimerEventIDs[wChairID] = m_timerIDs.back();
        }
#endif
		return true;
	}	
	return false;
}

bool PriaveteGame::OnEventRefreshTable(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize, std::string kChannel)
{
	ASSERT(wDataSize == sizeof(DBR_GP_Private_Refresh_Table_Ret));
	if (wDataSize != sizeof(DBR_GP_Private_Refresh_Table_Ret)) return false;

	DWORD dwTableID = ((DBR_GP_Private_Refresh_Table_Ret*)pData)->dwTableID;
	DWORD dwUserID = ((DBR_GP_Private_Refresh_Table_Ret*)pData)->dwUserID;

	PrivateTableInfo* pTable = getTableInfoByUserID(dwUserID);
	ASSERT(pTable); if (!pTable) goto STEP_FREASH_SCORE_BY_SOCKET_ID;

	ITableFrame* pTableFrame = pTable->pITableFrame;
	ASSERT(pTableFrame); if (!pTableFrame) goto STEP_FREASH_SCORE_BY_SOCKET_ID;

	IServerUserItem * pIUserItem = pTable->pITableFrame->SearchUserItem(dwUserID);;
	ASSERT(pIUserItem); if (!pIUserItem) goto STEP_FREASH_SCORE_BY_SOCKET_ID;

	pIUserItem->SetUserScore(((DBR_GP_Private_Refresh_Table_Ret*)pData)->dwUserScore);
	pIUserItem->SetUserInsureScore(((DBR_GP_Private_Refresh_Table_Ret*)pData)->dwInsureScore);

	CMD_GR_UserScore UserScore;
	UserScore.dwUserID = pIUserItem->GetUserID();
	tagUserInfo * pUserInfo = pIUserItem->GetUserInfo();

	//构造数据
	UserScore.dwUserID = pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount = pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount = pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount = pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount = pUserInfo->dwFleeCount;
	UserScore.UserScore.dwUserMedal = pUserInfo->dwUserMedal;
	UserScore.UserScore.dwExperience = pUserInfo->dwExperience;
	UserScore.UserScore.lLoveLiness = pUserInfo->lLoveLiness;

	//构造积分
	UserScore.UserScore.lGrade = pUserInfo->lGrade;
	UserScore.UserScore.lInsure = pUserInfo->lInsure;

	//构造积分
	UserScore.UserScore.lScore = pUserInfo->lScore;
	if (pIServerUserItem)
	{
		UserScore.UserScore.lScore += pIServerUserItem->GetTrusteeScore();
		UserScore.UserScore.lScore += pIServerUserItem->GetFrozenedScore();
	}

	pTableFrame->SendTableData(INVALID_CHAIR, SUB_GR_USER_SCORE, &UserScore, sizeof(UserScore), MDM_GR_USER);

	return true;

STEP_FREASH_SCORE_BY_SOCKET_ID:
	{
		ASSERT(pIServerUserItem); if (!pIServerUserItem) return false;

		pIServerUserItem->SetUserScore(((DBR_GP_Private_Refresh_Table_Ret*)pData)->dwUserScore);
		pIServerUserItem->SetUserInsureScore(((DBR_GP_Private_Refresh_Table_Ret*)pData)->dwInsureScore);

		UserScore.dwUserID = pIServerUserItem->GetUserID();
		tagUserInfo * pUserInfo2 = pIServerUserItem->GetUserInfo();

		//构造数据
		UserScore.dwUserID = pUserInfo2->dwUserID;
		UserScore.UserScore.dwWinCount = pUserInfo2->dwWinCount;
		UserScore.UserScore.dwLostCount = pUserInfo2->dwLostCount;
		UserScore.UserScore.dwDrawCount = pUserInfo2->dwDrawCount;
		UserScore.UserScore.dwFleeCount = pUserInfo2->dwFleeCount;
		UserScore.UserScore.dwUserMedal = pUserInfo2->dwUserMedal;
		UserScore.UserScore.dwExperience = pUserInfo2->dwExperience;
		UserScore.UserScore.lLoveLiness = pUserInfo2->lLoveLiness;

		//构造积分
		UserScore.UserScore.lGrade = pUserInfo2->lGrade;
		UserScore.UserScore.lInsure = pUserInfo2->lInsure;

		//构造积分
		UserScore.UserScore.lScore = pUserInfo2->lScore;
		if (pIServerUserItem)
		{
			UserScore.UserScore.lScore += pIServerUserItem->GetTrusteeScore();
			UserScore.UserScore.lScore += pIServerUserItem->GetFrozenedScore();
		}
		SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_USER_SCORE, &UserScore, sizeof(UserScore));
	}

	return true;
}

//创建房间
bool PriaveteGame::OnEventCreatePrivate(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel)
{
	//参数效验
	if(pIServerUserItem==NULL) return true;
	if(wDataSize>sizeof(DBO_GR_CreatePrivateResoult)) return false;

	//提取数据
	DBO_GR_CreatePrivateResoult* pPrivate = (DBO_GR_CreatePrivateResoult*)pData;	
	DWORD dwAgaginTable = pPrivate->dwAgaginTable;

	//报名失败
	if(pPrivate->bSucess==false)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pPrivate->szDescribeString,SMT_EJECT);
		return true;
	}

#if defined(ALLOWED_PLAY_COUNT_UNLIMITED)
    //do nothing...
#else
    if (pPrivate->bPlayCoutIdex < 0 || pPrivate->bPlayCoutIdex >= 4)
    {
        return false;
    }
#endif

    if(pIServerUserItem->GetUserScore() < pPrivate->dwEnterMatchNum)
    {
        m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("对不起，亲，你的金币不够进入房间！") , SMT_EJECT);
        return false;
    }

	//寻找位置
	ITableFrame * pICurrTableFrame=NULL;
	PrivateTableInfo* pCurrTableInfo=NULL;
	if (dwAgaginTable != INVALID_DWORD)
	{
		pCurrTableInfo = getTableInfoByTableID(dwAgaginTable);
		if (!pCurrTableInfo)
		{
			return false;
		}
		pICurrTableFrame = pCurrTableInfo->pITableFrame;
		if (pCurrTableInfo->bInEnd == false)
		{
			joinPrivateRoom(pIServerUserItem,pICurrTableFrame,pCurrTableInfo);
			return true;
		}
		else
		{
			pCurrTableInfo->restAgainValue();
			sendPrivateRoomInfo(NULL,pCurrTableInfo);
		}
	}
	else
	{
		WORD wTableID = 0;
		for (wTableID=0;wTableID<m_pGameServiceOption->wTableCount;wTableID++)
		{
			//获取对象
			ASSERT(m_pTableInfo[wTableID].pITableFrame!=NULL);
			ITableFrame * pITableFrame=m_pTableInfo[wTableID].pITableFrame;
			if(m_pTableInfo[wTableID].bInEnd)
			{
				continue;
			}
			//状态判断
			if (pITableFrame->GetNullChairCount()==pITableFrame->GetChairCount())
			{
				pICurrTableFrame = pITableFrame;
				pCurrTableInfo = &m_pTableInfo[wTableID];
                short wCharNum = static_cast<short>(pPrivate->wCharNum);
                if (wCharNum > 0)
                    pCurrTableInfo->restValue(pPrivate->wCharNum);
                else
                    pCurrTableInfo->restValue();
				break;
			}
		}
		if (getTableInfoByCreaterID(pIServerUserItem->GetUserID()))
		{
			return true;
		}
	}
	//桌子判断
	if(pICurrTableFrame==NULL)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建房间失败, 服务器房间已满。"),SMT_EJECT);
		return true;
	}


	if(!joinPrivateRoom(pIServerUserItem,pICurrTableFrame, pCurrTableInfo))
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("进入房间失败。"),SMT_EJECT);
		return true;
	}
	int iRandNum = 1000+rand()%8900;
	while (getTableInfoByRoomID(iRandNum))
	{
		iRandNum = 1000+rand()%8900;
	}
	if (m_pGameServiceOption->wServerID >= 90)
	{
		//ASSERT(false);
		//return true;
	}
	iRandNum = (m_pGameServiceOption->wServerID+10)*10000+iRandNum;
	pCurrTableInfo->dwPlayCout = (DWORD)m_kPrivateInfo.bPlayCout[pPrivate->bPlayCoutIdex];
	if (pPrivate->cbRoomType == Type_Private)
	{
		pCurrTableInfo->dwPlayCost = (DWORD)m_kPrivateInfo.lPlayCost[pPrivate->bPlayCoutIdex];
	}
	else
	{
		pCurrTableInfo->dwPlayCost = (DWORD)m_kPrivateInfo.lCostGold;
	}
	pCurrTableInfo->setRoomNum(iRandNum);
    pCurrTableInfo->pITableFrame->SetCreateUserID(pIServerUserItem->GetUserID());
	pCurrTableInfo->kHttpChannel = kChannel;
	pCurrTableInfo->bRoomType = pPrivate->cbRoomType;
    pCurrTableInfo->dwGameKindID = pPrivate->dwGameKindID;
	pCurrTableInfo->dwGameRuleIdex = pPrivate->bGameRuleIdex;
	pCurrTableInfo->bGameTypeIdex = pPrivate->bGameTypeIdex;
	pCurrTableInfo->bPlayCoutIdex = pPrivate->bPlayCoutIdex;

#if defined(PLATFORM_CONGCONG)
	pCurrTableInfo->bPassionationMode = pPrivate->bPassionationMode;	//激情模式开关
	pCurrTableInfo->bBloodFightMode = pPrivate->bBloodFightMode;	//血战到底开关
#endif

	pCurrTableInfo->bAllowedStrangerJoin = pPrivate->bAllowedStrangerJoin;	//是否允许陌生人加入
	pCurrTableInfo->dwBaseScore = pPrivate->dwBaseScore;                 //底注
	pCurrTableInfo->dwEnterMatchNum = pPrivate->dwEnterMatchNum;       //入场限制
	pCurrTableInfo->dwOutMatchNum = pPrivate->dwOutMatchNum;         //离场限制

	GetLocalTime(&pCurrTableInfo->kTotalRecord.kPlayTime);

	struct PrivateInfo {
		DWORD dwBaseScore;
		DWORD dwEnterScore;
		DWORD dwOutScore;
		DWORD PlayCount;
	};

	PrivateInfo pInfo;
	pInfo.dwBaseScore = pPrivate->dwBaseScore;
	pInfo.dwEnterScore = pPrivate->dwEnterMatchNum;
	pInfo.dwOutScore = pPrivate->dwOutMatchNum;
	pInfo.PlayCount = m_kPrivateInfo.bPlayCout[pPrivate->bPlayCoutIdex];

	pICurrTableFrame->SetPrivateInfo(pCurrTableInfo->bGameTypeIdex,
		pCurrTableInfo->dwGameRuleIdex,
		&pInfo);

	pICurrTableFrame->SetCreateUserID(pCurrTableInfo->pITableFrame->GetCreateUserID());

	CMD_GF_Create_Private_Sucess kSucessInfo;
	kSucessInfo.lCurSocre = pPrivate->bSucess;
	kSucessInfo.dwRoomNum = pCurrTableInfo->dwRoomNum;
	SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_CREATE_PRIVATE_SUCESS,&kSucessInfo,sizeof(kSucessInfo));

	sendPrivateRoomInfo(NULL,pCurrTableInfo);
	return true;
}

//创建私人场
bool PriaveteGame::OnTCPNetworkSubCreatePrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Create_Private));
	if(wDataSize!=sizeof(CMD_GR_Create_Private)) return false;

	CMD_GR_Create_Private* pCMDInfo = (CMD_GR_Create_Private*)pData;
	pCMDInfo->stHttpChannel[CountArray(pCMDInfo->stHttpChannel)-1]=0;

	if (pIServerUserItem->GetTableID() != INVALID_TABLE)
	{
		PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
		if (!pTableInfo)
		{
			return true;
		}
		joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame,pTableInfo);
		return true;
	}

	if(pCMDInfo->cbGameType == Type_Private)
	{
		DBR_GR_Create_Private kDBRInfo;
		kDBRInfo.dwUserID = pIServerUserItem->GetUserID();
		kDBRInfo.wKindID = m_pGameServiceAttrib->wKindID;
		kDBRInfo.cbRoomType = Type_Private;
        kDBRInfo.dwCostType = Type_Private;
        kDBRInfo.dwCost = (DWORD)m_kPrivateInfo.lPlayCost[pCMDInfo->bPlayCoutIdex];
		kDBRInfo.dwAgaginTable = INVALID_DWORD;
		kDBRInfo.bPlayCoutIdex = pCMDInfo->bPlayCoutIdex;
		kDBRInfo.bGameRuleIdex = pCMDInfo->bGameRuleIdex;
		kDBRInfo.bGameTypeIdex = pCMDInfo->bGameTypeIdex;
        kDBRInfo.wCharNum = pCMDInfo->wCharNum;
#if defined(PLATFORM_CONGCONG)
		kDBRInfo.bBloodFightMode = pCMDInfo->bBloodFightMode;
		kDBRInfo.bPassionationMode = pCMDInfo->bPassionationMode;
#endif
        kDBRInfo.bAllowedStrangerJoin = pCMDInfo->bAllowedStrangerJoin;
        kDBRInfo.dwBaseScore  = pCMDInfo->dwBaseScore;                 //底注
        kDBRInfo.dwEnterMatchNum  = pCMDInfo->dwEnterMatchNum;       //入场限制
        kDBRInfo.dwOutMatchNum  = pCMDInfo->dwOutMatchNum;         //离场限制
		DBR_CreatePrivate(&kDBRInfo,dwSocketID,pIServerUserItem,pCMDInfo->stHttpChannel);
	}
	else
	{
		ITableFrame * pICurrTableFrame=NULL;
		PrivateTableInfo* pCurrTableInfo=NULL;
		WORD wTableID = 0;
		for (wTableID=0;wTableID<m_pGameServiceOption->wTableCount;wTableID++)
		{
			//获取对象
			PrivateTableInfo& pTableInfo = m_pTableInfo[wTableID];
			ASSERT(pTableInfo.pITableFrame!=NULL);
			ITableFrame * pITableFrame=pTableInfo.pITableFrame;
			if(pTableInfo.bInEnd)
			{
				continue;
			}
			if(pTableInfo.bRoomType != Type_Public)
			{
				continue;
			}
			if (m_pTableInfo[wTableID].dwGameRuleIdex != pCMDInfo->bGameRuleIdex 
				|| m_pTableInfo[wTableID].bGameTypeIdex != pCMDInfo->bGameTypeIdex )
			{
				continue;
			}
			//状态判断
			if (pITableFrame->GetNullChairCount() >= 0)
			{
				pICurrTableFrame = pITableFrame;
				pCurrTableInfo = &m_pTableInfo[wTableID];
				break;
			}
		}
		if (pICurrTableFrame == NULL)
		{
			DBR_GR_Create_Private kDBRInfo;
			kDBRInfo.dwUserID = pIServerUserItem->GetUserID();
			kDBRInfo.wKindID = m_pGameServiceAttrib->wKindID;
			kDBRInfo.cbRoomType = Type_Public;
            kDBRInfo.dwCostType = Type_Private;
            kDBRInfo.dwCost = (DWORD)m_kPrivateInfo.lPlayCost[pCMDInfo->bPlayCoutIdex];
			kDBRInfo.dwAgaginTable = INVALID_DWORD;
			kDBRInfo.bPlayCoutIdex = pCMDInfo->bPlayCoutIdex;
			kDBRInfo.bGameRuleIdex = pCMDInfo->bGameRuleIdex;
			kDBRInfo.bGameTypeIdex = pCMDInfo->bGameTypeIdex;
            kDBRInfo.wCharNum = pCMDInfo->wCharNum;
#if defined(PLATFORM_CONGCONG)
            kDBRInfo.bBloodFightMode = pCMDInfo->bBloodFightMode;
            kDBRInfo.bPassionationMode = pCMDInfo->bPassionationMode;
#endif
            kDBRInfo.bAllowedStrangerJoin = pCMDInfo->bAllowedStrangerJoin;
            kDBRInfo.dwBaseScore = pCMDInfo->dwBaseScore;
            kDBRInfo.dwEnterMatchNum = pCMDInfo->dwEnterMatchNum;
            kDBRInfo.dwOutMatchNum = pCMDInfo->dwOutMatchNum;

			DBR_CreatePrivate(&kDBRInfo,dwSocketID,pIServerUserItem,"");
		}
		else
		{
			joinPrivateRoom(pIServerUserItem,pICurrTableFrame,pCurrTableInfo);
		}
	}
	return true;
}
//重新加入私人场
bool PriaveteGame::OnTCPNetworkSubAgainEnter(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Again_Private));
	if(wDataSize!=sizeof(CMD_GR_Again_Private)) return false;

	CMD_GR_Again_Private* pCMDInfo = (CMD_GR_Again_Private*)pData;
	pCMDInfo->stHttpChannel[CountArray(pCMDInfo->stHttpChannel)-1] = 0;

	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return true;
	}
	if (pTableInfo->pITableFrame->GetCreateUserID() != pIServerUserItem->GetUserID())
	{
		joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame, pTableInfo);
		return true;
	}
	if (!pTableInfo->bInEnd && pTableInfo->dwRoomNum != 0)
	{
		joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame, pTableInfo);
		return true;
	}
	DBR_GR_Create_Private kDBRInfo;
	ZeroMemory(&kDBRInfo,sizeof(kDBRInfo));
	kDBRInfo.cbRoomType = pTableInfo->bRoomType;
	kDBRInfo.dwUserID = pIServerUserItem->GetUserID();
	kDBRInfo.wKindID = m_pGameServiceAttrib->wKindID;
	kDBRInfo.dwCost = (DWORD)m_kPrivateInfo.lPlayCost[pTableInfo->bPlayCoutIdex];
	kDBRInfo.dwAgaginTable = pIServerUserItem->GetTableID();
	kDBRInfo.bGameTypeIdex = pTableInfo->bGameTypeIdex;
	kDBRInfo.bGameRuleIdex = pTableInfo->dwGameRuleIdex;

#if defined(PLATFORM_CONGCONG)
    kDBRInfo.bPassionationMode  = pTableInfo->bPassionationMode;	//激情模式开关
    kDBRInfo.bBloodFightMode  = pTableInfo->bBloodFightMode;	//血战到底开关
#endif
    kDBRInfo.bAllowedStrangerJoin  = pTableInfo->bAllowedStrangerJoin;	//是否允许陌生人加入
    kDBRInfo.dwBaseScore  = pTableInfo->dwBaseScore;                 //底注
    kDBRInfo.dwEnterMatchNum  = pTableInfo->dwEnterMatchNum;       //入场限制
    kDBRInfo.dwOutMatchNum  = pTableInfo->dwOutMatchNum;         //离场限制

	DBR_CreatePrivate(&kDBRInfo,dwSocketID,pIServerUserItem,pCMDInfo->stHttpChannel);
	return true;
}
//加入私人场
bool PriaveteGame::OnTCPNetworkSubJoinPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
    ASSERT(pIServerUserItem); 
    if(!pIServerUserItem) return false;
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Join_Private));
	if(wDataSize!=sizeof(CMD_GR_Join_Private)) return false;

	CMD_GR_Join_Private* pCMDInfo = (CMD_GR_Join_Private*)pData;
	PrivateTableInfo* pTableInfo = getTableInfoByRoomID(pCMDInfo->dwRoomNum);
	if (!pTableInfo)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，没有找到该房间，可能房主已经退出。"),SMT_EJECT|SMT_CHAT);
		return true;
	}
	if (pTableInfo->pITableFrame->GetNullChairCount() <= 0)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏人数已满，无法加入。"),SMT_EJECT|SMT_CHAT);
		return true;
	}
	if ((pTableInfo->pITableFrame->GetStartMode() != START_MODE_MASTER_CONTROL && pTableInfo->pITableFrame->GetStartMode() != START_MODE_FULL_READY) && (pTableInfo->bStart || pTableInfo->bInEnd))
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏已经开始，无法加入。"),SMT_EJECT|SMT_CHAT);
		return true;
	}
    if(pIServerUserItem->GetUserScore() <pTableInfo->dwEnterMatchNum)
    {
        m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，亲，你的金币不够进入该房间。"),SMT_EJECT|SMT_CHAT);
        return true;
    }

	if (!joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame, pTableInfo))
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("加入房间失败。"),SMT_EJECT);
	}
	return true;
}
bool PriaveteGame::OnTCPNetworkSubDismissPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Dismiss_Private));
	if(wDataSize!=sizeof(CMD_GR_Dismiss_Private)) return false;

	CMD_GR_Dismiss_Private* pCMDInfo = (CMD_GR_Dismiss_Private*)pData;

	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	ASSERT(pTableInfo); if (!pTableInfo) return false;

	if (!pTableInfo->bStart && pTableInfo->pITableFrame->GetCreateUserID() != pIServerUserItem->GetUserID())
	{
		return true;
	}
	if (pTableInfo->bInEnd)
	{
		return true;
	}
	if (pTableInfo->kDismissChairID.size() == 0 && !pCMDInfo->bDismiss)
	{
		return true;
	}
	if(pCMDInfo->bDismiss)
	{
		for (int i = 0;i<(int)pTableInfo->kDismissChairID.size();i++)
		{
			if (pTableInfo->kDismissChairID[i] == pIServerUserItem->GetChairID())
			{
				return true;
			}
		}
		pTableInfo->kDismissChairID.push_back(pIServerUserItem->GetChairID());
	}
	else
	{
		for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size();i++)
		{
			if (pTableInfo->kNotAgreeChairID[i] == pIServerUserItem->GetChairID())
			{
				return true;
			}
		}
		pTableInfo->kNotAgreeChairID.push_back(pIServerUserItem->GetChairID());
	}
	CMD_GF_Private_Dismiss_Info kNetInfo;
	kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
	kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
	for (int i = 0;i<(int)pTableInfo->kDismissChairID.size();i++)
	{
		kNetInfo.dwDissChairID[i] = pTableInfo->kDismissChairID[i];
	}
	for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size();i++)
	{
		kNetInfo.dwNotAgreeChairID[i] = pTableInfo->kNotAgreeChairID[i];
	}
	kNetInfo.dwValue2 = DISMISS_WAITE_TIME - (DWORD)pTableInfo->fDismissPastTime;
	SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));

	bool bClearDismissInfo = false;
	if (pTableInfo->kNotAgreeChairID.size() >= 1)
	{
		bClearDismissInfo = true;
	}

	if (!pTableInfo->bStart || (int)kNetInfo.dwDissUserCout >= (int)pTableInfo->pITableFrame->GetSitUserCount())
	{
		bClearDismissInfo = true;

		pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);

		DismissRoom(pTableInfo);

		ClearRoom(pTableInfo);

	}
	if (bClearDismissInfo)
	{
		pTableInfo->kNotAgreeChairID.clear();
		pTableInfo->kDismissChairID.clear();
		pTableInfo->fDismissPastTime = 0;
		kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
		kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
		SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
	}
	return true;
}
//比赛事件
bool PriaveteGame::OnEventSocketPrivate(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_PRIVATE_INFO:	//私人场信息
		{
			return SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_PRIVATE_INFO,&m_kPrivateInfo,sizeof(m_kPrivateInfo));
		}
	case SUB_GR_CREATE_PRIVATE:	//创建私人场
		{
			return OnTCPNetworkSubCreatePrivate(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_RIVATE_AGAIN:	//重新加入私人场
		{
			return OnTCPNetworkSubAgainEnter(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_JOIN_PRIVATE:	//加入私人场
		{
			return OnTCPNetworkSubJoinPrivate(pData,wDataSize,pIServerUserItem,dwSocketID);	
		}
	case SUB_GR_PRIVATE_DISMISS:	//解散
		{
			return OnTCPNetworkSubDismissPrivate(pData,wDataSize,pIServerUserItem,dwSocketID);	
		}
	case SUB_GR_INQUIRE_TABLES:
		{
			return OnTCPNetworkSubInqureTables(pData,wDataSize,pIServerUserItem,dwSocketID);	
		}
    case SUB_GR_PAYMENT_REFRESH_TABLE:
        {
            return OnPaymentSubRequestRefreshTable(pData, wDataSize, pIServerUserItem, dwSocketID);
        }
    case SUB_GR_PAYMENT_WEIXIN_ORDER_RET:
        {
            return OnPaymentSubRequestRequestWeiXinOrderRet(pData, wDataSize, pIServerUserItem, dwSocketID);
        }
	}
	return true;
}

//重置桌位
void PriaveteGame::DismissRoom(PrivateTableInfo* pTableInfo)
{
	ASSERT(pTableInfo!=NULL);
	if (pTableInfo==NULL) return;

	ITableFrame* pTableFrame=pTableInfo->pITableFrame;
	ASSERT(pTableFrame!=NULL);
	if (pTableFrame==NULL) return;

	if (pTableInfo->bStart)
	{
		CMD_GF_Private_End_Info kNetInfo;
		kNetInfo.lPlayerWinLose.resize(pTableFrame->GetChairCount());
		kNetInfo.lPlayerAction.resize(pTableFrame->GetChairCount()*MAX_PRIVATE_ACTION);
		for (int i = 0;i<pTableFrame->GetChairCount();i++)
		{
			kNetInfo.lPlayerWinLose[i] = pTableInfo->lPlayerWinLose[i];
			for (int m = 0;m<MAX_PRIVATE_ACTION;m++)
			{
				kNetInfo.lPlayerAction[i*MAX_PRIVATE_ACTION+m] = pTableInfo->lPlayerAction[i][m];
			}
		}
		datastream kDataStream;
		kNetInfo.StreamValue(kDataStream,true);
		SendTableData(pTableFrame,MDM_GR_PRIVATE,SUB_GF_PRIVATE_END,&kDataStream[0],kDataStream.size());
	}

	pTableInfo->bInEnd = true;

	//强制解散游戏
	if (pTableFrame->IsGameStarted()==true)
	{
		bool bSuccess=pTableFrame->DismissGame();
		if (bSuccess==false)
		{
			CTraceService::TraceString(TEXT("PriaveteGame 解散游戏"),TraceLevel_Exception);
			return;
		}
	}

	if (pTableInfo->bStart)
	{

		datastream kDataStream;
		pTableInfo->kTotalRecord.StreamValue(kDataStream,true);

		m_pIDataBaseEngine->PostDataBaseRequest(INVALID_DWORD,DBR_GR_PRIVATE_GAME_RECORD,0,&kDataStream[0],kDataStream.size());	

		pTableInfo->bStart = false;
		sendPrivateRoomInfo(NULL,pTableInfo);
	}
	else
	{
		ClearRoom(pTableInfo);
	}
	return;
}
void PriaveteGame::ClearRoom(PrivateTableInfo* pTableInfo)
{
	ASSERT(pTableInfo!=NULL);
	if (pTableInfo==NULL) return;

	ITableFrame* pTableFrame=pTableInfo->pITableFrame;
	ASSERT(pTableFrame!=NULL);
	if (pTableFrame==NULL) return;

	//强制解散游戏
	if (pTableFrame->IsGameStarted()==true)
	{
		bool bSuccess=pTableFrame->DismissGame();
		if (bSuccess==false)
		{
			CTraceService::TraceString(TEXT("PriaveteGame 解散游戏"),TraceLevel_Exception);
			return;
		}
	}
	//该桌用户全部离开
	for (int i=0;i<pTableFrame->GetChairCount();i++)
	{
		IServerUserItem* pUserItem=pTableFrame->GetTableUserItem(i);
		if(pUserItem&&pUserItem->GetTableID()!=INVALID_TABLE)
		{
			pTableFrame->PerformStandUpActionReally(pUserItem);
		}
	}
	pTableInfo->restValue();
}
void PriaveteGame::DBR_CreatePrivate(DBR_GR_Create_Private* kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem,std::string kHttpChannel)
{
	if (kHttpChannel != "")
	{
		 int iCout = FangKaHttpUnits::UseCard(pIServerUserItem->GetUserID(),0,kHttpChannel);
		 DBO_GR_CreatePrivateResoult kNetInfo;
		 ZeroMemory(&kNetInfo,sizeof(kNetInfo));
		 kNetInfo.dwAgaginTable = kInfo->dwAgaginTable;
		 kNetInfo.bGameRuleIdex = kInfo->bGameRuleIdex;
		 kNetInfo.bGameTypeIdex = kInfo->bGameTypeIdex;
		 kNetInfo.bPlayCoutIdex = kInfo->bPlayCoutIdex;
		 kNetInfo.cbRoomType = Type_Private;
         kNetInfo.wCharNum = kInfo->wCharNum;
		 kNetInfo.dwBaseScore = kInfo->dwBaseScore;
		 kNetInfo.dwEnterMatchNum = kInfo->dwEnterMatchNum;
		 kNetInfo.dwOutMatchNum = kInfo->dwOutMatchNum;

		 if (iCout < (int)kInfo->dwCost)
		 {
			 kNetInfo.bSucess = false;
			 lstrcpyn(kNetInfo.szDescribeString,TEXT("房卡不够"),CountArray(kNetInfo.szDescribeString));
		 }
		 else
		 {
			 kNetInfo.bSucess = true;
			 kNetInfo.lCurSocre = iCout;
		 }
		 OnEventCreatePrivate(DBO_GR_CREATE_PRIVATE,pIServerUserItem,&kNetInfo,sizeof(kNetInfo),kHttpChannel);
	}
	else
	{
		m_pIDataBaseEngine->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_CREATE_PRIVAT,dwSocketID,kInfo,sizeof(DBR_GR_Create_Private));
	}
}
//用户积分
bool PriaveteGame::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	m_pIServerUserItemSink->OnEventUserItemScore(pIServerUserItem,cbReason);
	return true;
}

//用户状态
bool PriaveteGame::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemStatus(pIServerUserItem,wOldTableID,wOldChairID);
	}

	return true;
}

//用户权限
bool PriaveteGame::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemRight(pIServerUserItem,dwAddRight,dwRemoveRight,bGameRight);
	}

	return true;
}

//用户登录
bool PriaveteGame::OnEventUserLogon(IServerUserItem * pIServerUserItem)
{
	//DONE:登陆成功就给用户发送私人场的信息
	SendData(pIServerUserItem, MDM_GR_PRIVATE, SUB_GR_PRIVATE_INFO, &m_kPrivateInfo, sizeof(m_kPrivateInfo));
	return true;
}

//用户登出
bool PriaveteGame::OnEventUserLogout(IServerUserItem * pIServerUserItem)
{
	return true;
}

//进入事件
bool PriaveteGame::OnEventEnterPrivate(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile)
{
	//手机用户
	if(bIsMobile == true)
	{
		//处理消息
		CMD_GR_LogonMobile * pLogonMobile=(CMD_GR_LogonMobile *)pData;
		pLogonMobile->szPassword[CountArray(pLogonMobile->szPassword)-1]=0;
		pLogonMobile->szMachineID[CountArray(pLogonMobile->szMachineID)-1]=0;

		//变量定义
		DBR_GR_LogonMobile LogonMobile;
		ZeroMemory(&LogonMobile,sizeof(LogonMobile));

		//构造数据
		LogonMobile.dwUserID=pLogonMobile->dwUserID;
		LogonMobile.dwClientAddr=dwUserIP;		
		LogonMobile.cbDeviceType=pLogonMobile->cbDeviceType;
		LogonMobile.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
		LogonMobile.wPageTableCount=pLogonMobile->wPageTableCount;		
		lstrcpyn(LogonMobile.szPassword,pLogonMobile->szPassword,CountArray(LogonMobile.szPassword));
		lstrcpyn(LogonMobile.szMachineID,pLogonMobile->szMachineID,CountArray(LogonMobile.szMachineID));

		//投递请求
		m_pIDataBaseEngine->PostDataBaseRequest(LogonMobile.dwUserID,DBR_GR_LOGON_MOBILE,dwSocketID,&LogonMobile,sizeof(LogonMobile));		
	}
	else
	{
		//处理消息
		CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
		pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
		pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

		//变量定义
		DBR_GR_LogonUserID LogonUserID;
		ZeroMemory(&LogonUserID,sizeof(LogonUserID));

		//构造数据
		LogonUserID.dwClientAddr=dwUserIP;
		LogonUserID.dwUserID=pLogonUserID->dwUserID;
		lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
		lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

		//投递请求
		m_pIDataBaseEngine->PostDataBaseRequest(LogonUserID.dwUserID,DBR_GR_LOGON_USERID,dwSocketID,&LogonUserID,sizeof(LogonUserID));
	}
	return true;
}
bool PriaveteGame::AddPrivateAction(ITableFrame* pTbableFrame,DWORD dwChairID, BYTE	bActionIdex)
{
	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pTbableFrame);
	ASSERT(pTableInfo);
	if (!pTableInfo)
	{
		return true;
	}
	if (dwChairID >= 100 || bActionIdex >= MAX_PRIVATE_ACTION)
	{
		return true;
	}
	pTableInfo->lPlayerAction[dwChairID][bActionIdex] ++;
	return true;
}

//用户
bool PriaveteGame::OnEventUserJoinPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID)
{
	//参数校验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem == NULL) return false;

	//DONE: 这个消息移动到了玩家登陆成功时触发
	//SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_PRIVATE_INFO,&m_kPrivateInfo,sizeof(m_kPrivateInfo));

	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	//判断状态
	if(pIServerUserItem->GetTableID()!=INVALID_TABLE) {
		sendPrivateRoomInfo(pIServerUserItem,getTableInfoByTableID(pIServerUserItem->GetTableID()));
	}
	return true;
}

//用户退赛
bool PriaveteGame::OnEventUserQuitPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank, DWORD dwContextID)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	return true;
}

//游戏开始
bool PriaveteGame::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pITableFrame);
	ASSERT(pTableInfo);
	if (!pTableInfo)
	{
		return true;
	}
	pTableInfo->dwStartPlayCout ++;
	pTableInfo->bStart = true;
	pTableInfo->newRandChild();

#if defined(ALLOWED_REBATE)
    //发送抽水
    if(pITableFrame)
    {
        for(int idx = 0; idx < pTableInfo->getChairCout(); idx++)
        {
            DBR_GP_Private_Rebate Rabate;

            IServerUserItem* pSerUserItem = pITableFrame->GetTableUserItem(idx);
            if(!pSerUserItem) continue;

            Rabate.dwUserID = pSerUserItem->GetUserID();
            Rabate.BottomNote = pTableInfo->dwBaseScore;
            switch(m_pGameServiceAttrib->wKindID)
            {
            case 27: //牛牛
                         Rabate.dwServerID = 1;
                         break;
            case 302://
                        Rabate.dwServerID = 2;
                        break;
            case 317:
                        Rabate.dwServerID = 3;
                        break;
            case 999:
                        Rabate.dwServerID = 4;
                        break;
            }

            m_pIDataBaseEngine->PostDataBaseRequest(0L, DBR_GR_PRIVAT_REBATE, 0L, &Rabate, sizeof(Rabate));
        }
    }
#endif

	sendPrivateRoomInfo(NULL,pTableInfo);
	return true;
}

//游戏结束
bool PriaveteGame::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return false;

	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pITableFrame);
	if (!pTableInfo)
	{
		return true;
	}
	//ZeroMemory(pTableInfo->cbLastOfflineReadyState,sizeof(pTableInfo->cbLastOfflineReadyState));
	if (pTableInfo->bRoomType == Type_Private)
	{
		//在第一句游戏结束的时候扣除房费
		if (pTableInfo->dwFinishPlayCout == 0
			&& !pTableInfo->bInEnd)
		{
			if (pTableInfo->kHttpChannel != "")
			{
				FangKaHttpUnits::UseCard(pTableInfo->pITableFrame->GetCreateUserID(),(int)m_kPrivateInfo.lPlayCost[pTableInfo->bPlayCoutIdex],pTableInfo->kHttpChannel);
			}
			else
			{
				CreatePrivateCost(pTableInfo);
			}
		}
		pTableInfo->dwFinishPlayCout ++;

#if defined(ALLOWED_KICK_UNREADY_USER)

        for(int idx = 0; idx < pTableInfo->getChairCout(); idx++)
        {
            //重置当前玩家的准备计时器
            pTableInfo->m_TimerEventIDs[idx] = m_timerIDs.back();

            IServerUserItem* pSerUserItem = pITableFrame->GetTableUserItem(idx);
            if(!pSerUserItem) continue;

            //如果用户的钱不够进入下一局游戏，踢出用户
            if (pSerUserItem->GetUserScore() < pTableInfo->dwOutMatchNum)
            {
                if (pSerUserItem->GetUserStatus() == US_SIT || pSerUserItem->GetUserStatus() == US_READY)
                {
					//这里要交换房主，免得钱不够了
                    if (m_kPrivateInfo.wKindID != 317)//317为焖鸡   焖鸡暂不由框架踢人  由子游戏踢人
                    {
						pITableFrame->SwitchRoomCreater(pSerUserItem);
                        pITableFrame->SendRoomMessage(pSerUserItem, TEXT("对不起，亲，你因为金币不够被请出房间。"), SMT_EJECT);
                        pITableFrame->PerformStandUpActionReally(pSerUserItem);
                    }
                }
            }

        }
#endif

        sendPrivateRoomInfo(NULL, pTableInfo);
		if (pTableInfo->dwFinishPlayCout >= pTableInfo->dwPlayCout || cbReason == GER_MASTER_END)
		{
			//pITableFrame->SendGameMessage(TEXT("游戏已经结束!"), SMT_EJECT);
			DismissRoom(pTableInfo);
			ClearRoom(pTableInfo);
		}
	}
	else if (pTableInfo->bRoomType == Type_Public)
	{
		CreatePrivateCost(pTableInfo);
		DismissRoom(pTableInfo);
		ClearRoom(pTableInfo);
	}
	return true;
}
bool PriaveteGame::WriteTableScore(ITableFrame* pITableFrame,tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& kData)
{
	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pITableFrame);
	if (!pTableInfo)
	{
		return true;
	}
	pTableInfo->writeSocre( ScoreInfoArray,wScoreCount,kData);

	return true;
}

#if defined(ROOM_ONLY_COST_GOLD)

void PriaveteGame::checkUserScore(IServerUserItem * pIServerUserItem,PrivateTableInfo* pTableInfo)
{
    
}

#endif

void PriaveteGame::sendPrivateRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo* pTableInfo)
{
	ASSERT(pTableInfo); if (!pTableInfo) return;

	CMD_GF_Private_Room_Info kNetInfo;
	kNetInfo.bRoomType = pTableInfo->bRoomType;
	kNetInfo.bStartGame = pTableInfo->bStart;
	kNetInfo.bCurPeopleNum = pTableInfo->pITableFrame->GetSitUserCount();
	kNetInfo.bMaxPeopleNum = pTableInfo->getChairCout();
	kNetInfo.dwRoomNum = pTableInfo->dwRoomNum;
	kNetInfo.dwPlayCout = pTableInfo->dwStartPlayCout;
	kNetInfo.dwCreateUserID = pTableInfo->pITableFrame->GetCreateUserID();
	kNetInfo.dwGameRuleIdex = pTableInfo->dwGameRuleIdex;
	kNetInfo.bGameTypeIdex = pTableInfo->bGameTypeIdex;
	kNetInfo.bPlayCoutIdex = pTableInfo->bPlayCoutIdex;
	kNetInfo.dwPlayTotal = pTableInfo->dwPlayCout;
	kNetInfo.kWinLoseScore.resize(pTableInfo->pITableFrame->GetChairCount());
	kNetInfo.dwBaseScore = pTableInfo->dwBaseScore;
	kNetInfo.dwEnterMatchNum = pTableInfo->dwEnterMatchNum;
	kNetInfo.dwOutMatchNum = pTableInfo->dwOutMatchNum;
	
#if defined(ROOM_ONLY_COST_GOLD)

     ITableFrame* pTableFrame = pTableInfo->pITableFrame;

    ASSERT(pTableFrame); if( !pTableFrame ) return;

	for (WORD i = 0; i< pTableFrame->GetChairCount();i++)
	{
        IServerUserItem* pSerUserItem = pTableFrame->GetTableUserItem(i);
        if(!pSerUserItem) continue;

        //写入当前积分
		kNetInfo.kWinLoseScore[i] = static_cast<int>(pSerUserItem->GetUserScore());
	}

#else
    for (WORD i = 0;i<pTableInfo->pITableFrame->GetChairCount();i++)
	{
	    kNetInfo.kWinLoseScore[i] =  (int)pTableInfo->lPlayerWinLose[i];
	}
#endif

	datastream kDataStream;
	kNetInfo.StreamValue(kDataStream,true);
	
	if (pIServerUserItem)
	{
		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GF_PRIVATE_ROOM_INFO,&kDataStream[0],kDataStream.size());
	}
	else
	{
		SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GF_PRIVATE_ROOM_INFO,&kDataStream[0],kDataStream.size());
	}
}

//断线
bool PriaveteGame::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem); if (!pIServerUserItem) return false;

#if defined(ALLOWED_QUIT_ROOM_INGAME_WITHOUT_AGREE)
    PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
    ASSERT(pTableInfo); if(!pTableInfo) return false;

    ITableFrame* pTBFrame =  pTableInfo->pITableFrame;
    ASSERT(pTBFrame); if(!pTBFrame) return false;

    //如果不在游戏中
    //TODO: 这个判断可能要改，个别游戏改了GAME_STATUS_WAIT, 导致判断 (GetGameStatus() ！= GAME_STATUS_PLAY) 失效了
    if (pTBFrame->GetGameStatus() < GAME_STATUS_PLAY || pTBFrame->GetGameStatus() >= GAME_STATUS_WAIT )
    {
        pTBFrame->SwitchRoomCreater(pIServerUserItem);
        return true;
    }
    sendPrivateRoomInfo(NULL, pTableInfo);
#else
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	ASSERT(pIServerUserItem); if (!pTableInfo) return false;
	
	//游戏没有开始
	if (!pTableInfo->bStart) {
		//房主掉了,解散房间
		if (pTableInfo->pITableFrame->GetCreateUserID() == pIServerUserItem->GetUserID()) {
			DismissRoom(pTableInfo);
			ClearRoom(pTableInfo);
		} else {//其他人掉了,踢掉他
			pTableInfo->pITableFrame->PerformStandUpActionReally(pIServerUserItem);
		}
		return true;
	}
	//游戏没有结束
	if (!pTableInfo->bInEnd) {
		//什么也不干
	}
	//游戏已经结束
	if (pTableInfo->bInEnd) {
		//房主掉了
		if (pTableInfo->pITableFrame->GetCreateUserID() == pIServerUserItem->GetUserID()) {
			pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"), SMT_EJECT);
			DismissRoom(pTableInfo);
			ClearRoom(pTableInfo);
		} else {//其他人掉了
			pTableInfo->pITableFrame->PerformStandUpActionReally(pIServerUserItem);
		}
	}
#endif

	return true;
}
//用户坐下
bool PriaveteGame::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{ 
	return true; 
}

//用户起立
bool PriaveteGame::OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//移除分组
	return true;
}

 //用户同意
bool PriaveteGame::OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem)
	{
		return true;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return true;
	}
	if (pTableInfo->bInEnd)
	{
		return false;
	}
	if (wChairID >= MAX_CHAIR)
	{
		return false;
	}
	//pTableInfo->cbLastOfflineReadyState[wChairID] = 1;
	return true; 
}

bool PriaveteGame::OnEventReqStandUP(IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem); if (!pIServerUserItem) return false;

	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	ASSERT(pTableInfo); if (!pTableInfo) return false;

#if !defined(ALLOWED_QUIT_ROOM_INGAME_WITHOUT_AGREE)
	//传统模式, 要打完局数才能退出游戏
	if (pTableInfo->bStart && !pTableInfo->bInEnd)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("游戏已经开始，无法退出。"), SMT_EJECT);
		return true;
	}
	if (pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("游戏正在进行，无法退出。"), SMT_EJECT);
		return true;
	}
#endif

    ITableFrame* pTBFrame = pTableInfo->pITableFrame;
    ASSERT(pTBFrame); if (!pTBFrame) return false;

#if defined(ALLOWED_CREATE_EMPTY_ROOM)
    //否则，离开座位，保留空房间
    //TODO:  解决离开座位清空房主数据
    IServerUserItem* pServerItem = pTableInfo->pITableFrame->GetTableUserItem(0);
    pTableInfo->pITableFrame->SetCreateUserID(-1);
#elif defined(ALLOWED_QUIT_ROOM_INGAME_WITHOUT_AGREE)

    if (1== pTBFrame->GetSitUserCount())
    {
        ClearRoom(pTableInfo);
        return true;
    }

    if(pIServerUserItem->GetUserStatus() == US_PLAYING)
    {
        m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("游戏正在进行，无法退出。"), SMT_EJECT);
        return true;
    }

    //如果此人是房主
    if (pTBFrame->GetCreateUserID() == pIServerUserItem->GetUserID())
    {
        pTBFrame->SwitchRoomCreater(pIServerUserItem);
    }

    pTBFrame->PerformStandUpActionReally(pIServerUserItem);
    sendPrivateRoomInfo(NULL, pTableInfo);

    return true;
#else

	//游戏已经结束
    if (pTableInfo && pTableInfo->bInEnd)
    {
        pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);
        ClearRoom(pTableInfo);
        return true;
    }
	//游戏尚未开始
    if (pTableInfo && !pTableInfo->bStart &&!pTableInfo->bInEnd)
    {
        m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("您已返回大厅, 房间将会继续保留."), SMT_CLOSE_ROOM);
    }
    pTBFrame->PerformStandUpActionReally(pIServerUserItem, true);

#endif
	return true;
}

bool PriaveteGame::OnEventClientReady(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem)
	{
		return true;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return true;
	}
	sendPrivateRoomInfo(pIServerUserItem,pTableInfo);

	if (pTableInfo->pITableFrame->GetGameStatus() == GAME_STATUS_FREE)
	{
		//if (wChairID < MAX_CHAIR && pTableInfo->cbLastOfflineReadyState[wChairID])
		//{
		//	pIServerUserItem->SetUserStatus(US_READY,
		//		pTableInfo->pITableFrame->GetTableID(),wChairID);
		//}
		//else
		{
			pIServerUserItem->SetUserStatus(US_SIT,
				pTableInfo->pITableFrame->GetTableID(),wChairID);
		}
	}

	if (pTableInfo->kDismissChairID.size())
	{
		CMD_GF_Private_Dismiss_Info kNetInfo;
		kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
		kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
		for (int i = 0;i<(int)pTableInfo->kDismissChairID.size() && i < MAX_CHAIR -1;i++)
		{
			kNetInfo.dwDissChairID[i] = pTableInfo->kDismissChairID[i];
		}
		for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size() && i < MAX_CHAIR -1;i++)
		{
			kNetInfo.dwNotAgreeChairID[i] = pTableInfo->kNotAgreeChairID[i];
		}
		kNetInfo.dwValue2 = DISMISS_WAITE_TIME - (DWORD)pTableInfo->fDismissPastTime;
		SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
	}

	return true;
}

bool PriaveteGame::OnPaymentSubRequestRefreshTable(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	ASSERT(wDataSize == sizeof(SUB_PM_PAY_SUC));

	if (wDataSize != sizeof(SUB_PM_PAY_SUC)) return false;

    SUB_PM_PAY_SUC* Paydata = static_cast<SUB_PM_PAY_SUC*>(pData);

    DBR_GP_Private_Refresh_Table dbr_refresh;
    dbr_refresh.dwUserID = Paydata->dwUserID;
    dbr_refresh.dwTableID = Paydata->wTableID;
	dbr_refresh.dwUserSocketID = Paydata->dwUserSocketID;

	m_pIDataBaseEngine->PostDataBaseRequest(0L, DBR_GR_PRIVAT_REFRESH_TABLES, 0L, &dbr_refresh, sizeof(dbr_refresh));
	return true;
}

bool PriaveteGame::OnPaymentSubRequestRequestWeiXinOrderRet(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	ASSERT(wDataSize == sizeof(SUB_PM_GEN_WEIXIN_OD_RET));
	if (wDataSize != sizeof(SUB_PM_GEN_WEIXIN_OD_RET)) return false;

	SUB_PM_GEN_WEIXIN_OD_RET* odRet = static_cast<SUB_PM_GEN_WEIXIN_OD_RET*>(pData);

	CMD_GF_Pay_GenWXOrder_ret orderRet;
	memset(&orderRet, 0, sizeof(orderRet));

	memcpy(orderRet.nonce_str, odRet->nonce_str, sizeof(odRet->nonce_str));
	memcpy(orderRet.prepare_id, odRet->prepare_id, sizeof(odRet->prepare_id));
	memcpy(orderRet.order, odRet->order, sizeof(odRet->order));
	orderRet.wPoductID = odRet->wPoductID;
	orderRet.nonce_str_len = odRet->nonce_str_len;
	orderRet.prepare_id_len = odRet->prepare_id_len;
	orderRet.order_len = odRet->order_len;

	DWORD dwTableID = odRet->dwTableID;
	DWORD dwUserID = odRet->dwUserID;

	PrivateTableInfo* pTable = getTableInfoByUserID(dwUserID);
	if (!pTable)  goto InSquare;

	ITableFrame* pTableFrame = pTable->pITableFrame;
	if (!pTableFrame) goto InSquare;

	IServerUserItem * pIUserItem = pTable->pITableFrame->SearchUserItem(dwUserID);
	ASSERT(pIUserItem); if (!pIUserItem) return false;
	SendData(pIUserItem, MDM_GF_PAY, SUB_GF_PAY_GEN_WEIXIN_ORDER_RET, &orderRet, sizeof(orderRet));

	return true;

InSquare://玩家在大厅
	{
		ASSERT(pIServerUserItem); if (!pIServerUserItem) return false;
		SendData(pIServerUserItem, MDM_GF_PAY, SUB_GF_PAY_GEN_WEIXIN_ORDER_RET, &orderRet, sizeof(orderRet));
	}

	return true;
}

bool PriaveteGame::OnTCPNetworkSubInqureTables(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	ASSERT(pIServerUserItem);
	ASSERT(wDataSize == sizeof(CMD_GR_INQURE_TABLES_INFO));

	if(wDataSize!=sizeof(CMD_GR_INQURE_TABLES_INFO)) return false;

	datastream ds_tables;

	//压入头
	CMD_GR_INQURE_TABLES_INFO_DATA_HEAD dt_head;

	dt_head.dwTablesTotal = 0;
    dt_head.dwGameKindID = m_pGameServiceAttrib->wKindID;

	ds_tables.push(dt_head);

	CMD_GR_INQURE_TABLES_INFO_DATA_BODY dt_body;

	for (WORD wTableID=0;wTableID<m_pGameServiceOption->wTableCount;wTableID++) {
		PrivateTableInfo& pTableInfo = m_pTableInfo[wTableID];
        //不是同一个游戏
        if(pTableInfo.dwGameKindID != dt_head.dwGameKindID) {
            continue;
        }
		//不是非私人场
		if( pTableInfo.bRoomType != Type_Private) {
			continue;
		}

		//如果房主不在线则继续
        IServerUserItem* pCreateUserItem = nullptr;
        DWORD dwUserID = pTableInfo.pITableFrame->GetCreateUserID();

        if(dwUserID > 0)
            pCreateUserItem = pTableInfo.pITableFrame->SearchUserItem(dwUserID);

        if( pCreateUserItem == nullptr || !pCreateUserItem->IsClientReady() )
            continue;

		//如果不是自己创建的房间
		if (pCreateUserItem->GetUserID() != pIServerUserItem->GetUserID()) {
			continue;
		}

//		if (
//#if defined(PLATFORM_CONGCONG)
//			//不是不能加入的模式
//			!pTableInfo.bAllowedStrangerJoin ||
//#endif
//			//不是空桌子
//			pTableInfo.pITableFrame->GetSitUserCount() == 0) {
//			continue;
//		}

		dt_body.bSitUserSum = static_cast<BYTE>(pTableInfo.pITableFrame->GetSitUserCount());
		dt_body.bChairSum = static_cast<BYTE>(pTableInfo.getChairCout());
		dt_body.dwCreateUserID = pTableInfo.pITableFrame->GetCreateUserID();             //房主
		dt_body.dwTableNum = pTableInfo.dwRoomNum;                 //房号
		dt_body.dwGameRuleIdex = pTableInfo.dwGameRuleIdex;           //游戏规则

#if defined(PLATFORM_CONGCONG)
        dt_body.bBloodFightMode = pTableInfo.bBloodFightMode;
        dt_body.bPassionationMode = pTableInfo.bPassionationMode;
#endif

        dt_body.bAllowedStrangerJoin = pTableInfo.bAllowedStrangerJoin;
		dt_body.dwBaseScore = pTableInfo.dwBaseScore;						//底注
		dt_body.dwEnterMatchNum = pTableInfo.dwEnterMatchNum;	//入场限制
		dt_body.dwOutMatchNum = pTableInfo.dwOutMatchNum;        //离场限制

		ds_tables.push(dt_body);

		dt_head.dwTablesTotal ++;
	}

	//从新写入显示的桌子数量
	memcpy(ds_tables.data(), &dt_head, sizeof(dt_head));

	bool bRet = SendData(pIServerUserItem, MDM_GR_PRIVATE, SUB_GR_INQUIRE_TABLES_RET, &ds_tables[0], ds_tables.size());

    return true;
}
