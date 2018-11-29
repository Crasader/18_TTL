#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS_READY					2									//最少时间
#define TIME_LESS								4

//游戏时间
#define TIME_USER_CALL_BANKER		2									//叫庄定时器
#define TIME_USER_START_GAME		3									//开始定时器
#define TIME_USER_ADD_SCORE			3									//下注定时器
#define TIME_USER_OPEN_CARD			12									//摊牌定时器

//游戏时间
#define IDI_ANDROID_ITEM_SINK    100
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			//开始时间
#define IDI_CALL_BANKER			(IDI_ANDROID_ITEM_SINK+1)			//叫庄时间
#define IDI_USER_ADD_SCORE		(IDI_ANDROID_ITEM_SINK+2)			//下注时间
#define IDI_OPEN_CARD			(IDI_ANDROID_ITEM_SINK+3)			//开牌时间

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink() {
    m_lTurnMaxScore = 0;
    ZeroMemory(m_HandCardData, sizeof(m_HandCardData));

    //接口变量
    m_pIAndroidUserItem = NULL;

    return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink() {
}

//接口查询
void*   CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(IAndroidUserItemSink, Guid, dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink, Guid, dwQueryVer);
    return NULL;
}

//初始接口
bool  CAndroidUserItemSink::Initialization(IUnknownEx* pIUnknownEx) {
    //查询接口
    ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAndroidUserItem) != NULL);
    m_pIAndroidUserItem = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAndroidUserItem);

    //错误判断
    if (m_pIAndroidUserItem == NULL) {
        ASSERT(FALSE);
        return false;
    }
    return true;
}

//重置接口
bool  CAndroidUserItemSink::RepositionSink() {
	if (m_pIAndroidUserItem != nullptr) {
		m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
		m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
		m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
		m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
	}

	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData, sizeof(m_HandCardData));
	return true;
}

//时间消息
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID) {
    switch (nTimerID) {
        case IDI_START_GAME: {	//开始定时器
            //发送准备
            m_pIAndroidUserItem->SendUserReady(NULL, 0);
			m_pIAndroidUserItem->SendSocketData(SUB_C_HOST_ALLOW_START);
            return true;
        }

        case IDI_CALL_BANKER: {	//叫庄定时
            int snatchBankerRatio = m_MaxRatio == 0 ? 0 : (rand() % 2 ? 0 : m_MaxRatio);
            CMD_C_UserSnatchBanker snatchBanker;
            ZeroMemory(&snatchBanker, sizeof(snatchBanker));
            snatchBanker.ratio = snatchBankerRatio;
            m_pIAndroidUserItem->SendSocketData(SUB_C_USER_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
            return true;
        }

        case IDI_USER_ADD_SCORE: {	//加注定时
            int maxIndex = 0;
            for (int index = 0; index < MAX_STAKE_COUNT; ++index) {
                ++maxIndex;
                if (m_AllBets[index] == 0) {
                    break;
                }
            }

            if (maxIndex == 0) {
                return true;
            }

			//DONE:随机选择下注,这样好吗
#ifdef RandomBet
            NNPlayerBet userBet = m_AllBets[rand() % maxIndex];
#else
			NNPlayerBet userBet = m_AllBets[0];
#endif

            CMD_C_UserCall userCall;
            ZeroMemory(&userCall, sizeof(userCall));
            userCall.playerBets = userBet;
            m_pIAndroidUserItem->SendSocketData(SUB_C_USER_CALL, &userCall, sizeof(userCall));
            return true;
        }

        case IDI_OPEN_CARD: {		//开牌定时
			CMD_C_UserShowCard showCard;
			ZeroMemory(&showCard, sizeof(showCard));
			showCard.result = m_GameLogic.checkNNType(m_HandCardData, -1);
			m_pIAndroidUserItem->SendSocketData(SUB_C_USER_SHOW_CARD, &showCard, sizeof(showCard));
            return true;
        }
    }

    return false;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID* pData, WORD wDataSize) {
    switch (wSubCmdID) {
		case SUB_S_NOTIFY_GAME_END: {
			UINT nElapse = rand() % (3) + TIME_LESS_READY + 3;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);
			return true;
		}
        case SUB_S_SNATCH_BANKER: {
            return OnSnatchBanker(pData, wDataSize);
        }
        case SUB_S_BANKER_INFO: {
            return OnBankerInfo(pData, wDataSize);
        }
        case SUB_S_SEND_CARD_ADD: {
            return OnSendCardAdd(pData, wDataSize);
        }
		case SUB_S_GAME_START:
        case SUB_S_HOST_CONFIRM_START:
        case SUB_S_USER_SNATCH_BANKER:
        case SUB_S_USER_CALL:
        case SUB_S_USER_SHOW_CARD:
        case SUB_S_CALCULATE:
        case SUB_S_SPLIT_CARD:
            return true;
    }

    //错误断言
    ASSERT(FALSE);
    return true;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID* pData, WORD wDataSize) {
    return true;
}

//场景消息
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID* pData, WORD wDataSize) {
    switch (cbGameStatus) {
        case GS_NN_FREE: {	//空闲状态
            //效验数据
            if (wDataSize != sizeof(CMD_S_GameScene)) return false;
            UINT nElapse = rand() % (3) + TIME_LESS_READY;
            m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);
            return true;
        }
	
		case GS_NN_PLAY:
		case GS_NN_CALCULATE:
			return true;
    }

    ASSERT(FALSE);
    return false;
}

//用户进入
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//用户离开
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//用户积分
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//用户状态
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//用户段位
void  CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//用户叫庄
bool CAndroidUserItemSink::OnSnatchBanker(const void* pBuffer, WORD wDataSize) {
    if (wDataSize != sizeof(CMD_S_SnatchBanker)) return false;

    auto pInfo = static_cast<const CMD_S_SnatchBanker*>(pBuffer);

    ZeroMemory(m_HandCardData, sizeof(m_HandCardData));
    memcpy(m_HandCardData, pInfo->cards[m_pIAndroidUserItem->GetChairID()], sizeof(BYTE) * pInfo->cardCount);
    m_MaxRatio = pInfo->maxRatio;

    //叫庄时间
    UINT nElapse = rand() % (3) + TIME_LESS;
    m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER, nElapse);
    return true;
}

bool CAndroidUserItemSink::OnBankerInfo(const void* pBuffer, WORD wDataSize) {
    if (wDataSize != sizeof(CMD_S_BankerInfo)) return false;

    auto pInfo = static_cast<const CMD_S_BankerInfo*>(pBuffer);

    if (pInfo->bankerChairID == m_pIAndroidUserItem->GetChairID()) {
        return true;
    }

    ZeroMemory(m_AllBets, sizeof(m_AllBets));
    memcpy(m_AllBets, pInfo->bets, sizeof(pInfo->bets));

    //叫庄时间
    UINT nElapse = rand() % (3) + TIME_LESS;
    m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
    return true;
}

bool CAndroidUserItemSink::OnSendCardAdd(const void* pBuffer, WORD wDataSize) {
	if (wDataSize != sizeof(CMD_S_SendCardAdd)) return false;

	auto pInfo = static_cast<const CMD_S_SendCardAdd*>(pBuffer);

	m_HandCardData[MAX_HAND_CARD - 1] = pInfo->playerCards[m_pIAndroidUserItem->GetChairID()];

	//叫庄时间
	UINT nElapse = rand() % (4) + TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD, nElapse);
	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void* pBuffer, WORD wDataSize) {
    //效验数据
// 	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
// 	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
//
// 	//设置变量
// 	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
//
// 	//设置筹码
// 	if (pGameStart->lTurnMaxScore>0)
// 	{
// 		//下注时间
// 		UINT nElapse=rand()%(4)+TIME_LESS;
// 		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
// 	}

    return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void* pBuffer, WORD wDataSize) {
    //效验数据
// 	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
// 	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

    return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void* pBuffer, WORD wDataSize) {
    //效验数据
// 	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
// 	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
//
// 	//设置数据
// 	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
// 	CopyMemory(m_HandCardData,pSendCard->cbCardData[wMeChiarID],sizeof(m_HandCardData));
//
// 	//开牌时间
// 	UINT nElapse=rand()%(6)+TIME_LESS*2;
// 	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

    return true;
}

//用户摊牌
bool CAndroidUserItemSink::OnSubOpenCard(const void* pBuffer, WORD wDataSize) {
    //效验数据
// 	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
// 	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

    return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void* pBuffer, WORD wDataSize) {
    //效验数据
    return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void* pBuffer, WORD wDataSize) {
    //效验参数
// 	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
// 	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
//
// 	//删除定时器
// 	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
// 	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
// 	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
//
// 	//开始时间
// 	UINT nElapse=rand()%(3)+TIME_LESS;
// 	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
//
// 	//清理变量
// 	m_lTurnMaxScore = 0;
// 	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

    return true;
}

//银行操作
void CAndroidUserItemSink::BankerOperate() {

    //变量定义
    IServerUserItem* pUserItem = m_pIAndroidUserItem->GetMeUserItem();
    LONGLONG lRobotScore = pUserItem->GetUserScore();// + m_lBankerLostScore;
    LONGLONG lInsureScore = pUserItem->GetUserInsure();

    {
        //判断存取
        if (lRobotScore > m_lRobotScoreRange[1]) {
            //发送消息
            LONGLONG lStorageValue = LONGLONG(lRobotScore * m_nRobotBankStorageMul / 100);

            if (lStorageValue > lRobotScore)
                lStorageValue = lRobotScore;


            if (lStorageValue > 0)
                m_pIAndroidUserItem->PerformSaveScore(lStorageValue);
        } else if (lRobotScore < m_lRobotScoreRange[0]) {
            //发送消息
            LONGLONG lGetValue =  m_lRobotBankGetScore;

            if (lGetValue > lInsureScore)
                lGetValue = lInsureScore;

            if (lGetValue > 0)
                m_pIAndroidUserItem->PerformTakeScore(lGetValue);
        }
    }

}


//读取配置
void CAndroidUserItemSink::ReadConfigInformation(TCHAR szRoomName[]) {

    //获取目录
    TCHAR szPath[MAX_PATH] = TEXT("");
    GetCurrentDirectory(CountArray(szPath), szPath);

    _sntprintf(m_szRoomName, sizeof(m_szRoomName), TEXT("OxEx"));

    //读取配置
    TCHAR szConfigFileName[MAX_PATH] = TEXT("");
    _sntprintf(szConfigFileName, sizeof(szConfigFileName), TEXT("%s\\OxServer.ini"), szPath);


    TCHAR OutBuf[255] = TEXT("");

    //提款数额
    ZeroMemory(OutBuf, sizeof(OutBuf));
    GetPrivateProfileString(szRoomName, TEXT("RobotBankGet"), _T("20000000"), OutBuf, 255, szConfigFileName);
    _sntprintf(OutBuf, sizeof(OutBuf), _T("%I64d"), &m_lRobotBankGetScore);

    //存款倍数
    m_nRobotBankStorageMul = GetPrivateProfileInt(szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

    if (m_nRobotBankStorageMul < 0 || m_nRobotBankStorageMul > 100) m_nRobotBankStorageMul = 20;

    //分数限制
    ZeroMemory(OutBuf, sizeof(OutBuf));
    GetPrivateProfileString(szRoomName, TEXT("RobotScoreMin"), _T("10000000"), OutBuf, 255, szConfigFileName);
    _sntprintf(OutBuf, sizeof(OutBuf), _T("%I64d"), &m_lRobotScoreRange[0]);

    ZeroMemory(OutBuf, sizeof(OutBuf));
    GetPrivateProfileString(szRoomName, TEXT("RobotScoreMax"), _T("100000000"), OutBuf, 255, szConfigFileName);
    _sntprintf(OutBuf, sizeof(OutBuf), _T("%I64d"), &m_lRobotScoreRange[1]);

    if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];
}

//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
