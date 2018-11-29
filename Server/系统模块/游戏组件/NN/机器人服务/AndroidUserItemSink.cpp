#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS_READY					2									//����ʱ��
#define TIME_LESS								4

//��Ϸʱ��
#define TIME_USER_CALL_BANKER		2									//��ׯ��ʱ��
#define TIME_USER_START_GAME		3									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			3									//��ע��ʱ��
#define TIME_USER_OPEN_CARD			12									//̯�ƶ�ʱ��

//��Ϸʱ��
#define IDI_ANDROID_ITEM_SINK    100
#define IDI_START_GAME			(IDI_ANDROID_ITEM_SINK+0)			//��ʼʱ��
#define IDI_CALL_BANKER			(IDI_ANDROID_ITEM_SINK+1)			//��ׯʱ��
#define IDI_USER_ADD_SCORE		(IDI_ANDROID_ITEM_SINK+2)			//��עʱ��
#define IDI_OPEN_CARD			(IDI_ANDROID_ITEM_SINK+3)			//����ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink() {
    m_lTurnMaxScore = 0;
    ZeroMemory(m_HandCardData, sizeof(m_HandCardData));

    //�ӿڱ���
    m_pIAndroidUserItem = NULL;

    return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink() {
}

//�ӿڲ�ѯ
void*   CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(IAndroidUserItemSink, Guid, dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink, Guid, dwQueryVer);
    return NULL;
}

//��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx* pIUnknownEx) {
    //��ѯ�ӿ�
    ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAndroidUserItem) != NULL);
    m_pIAndroidUserItem = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAndroidUserItem);

    //�����ж�
    if (m_pIAndroidUserItem == NULL) {
        ASSERT(FALSE);
        return false;
    }
    return true;
}

//���ýӿ�
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

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID) {
    switch (nTimerID) {
        case IDI_START_GAME: {	//��ʼ��ʱ��
            //����׼��
            m_pIAndroidUserItem->SendUserReady(NULL, 0);
			m_pIAndroidUserItem->SendSocketData(SUB_C_HOST_ALLOW_START);
            return true;
        }

        case IDI_CALL_BANKER: {	//��ׯ��ʱ
            int snatchBankerRatio = m_MaxRatio == 0 ? 0 : (rand() % 2 ? 0 : m_MaxRatio);
            CMD_C_UserSnatchBanker snatchBanker;
            ZeroMemory(&snatchBanker, sizeof(snatchBanker));
            snatchBanker.ratio = snatchBankerRatio;
            m_pIAndroidUserItem->SendSocketData(SUB_C_USER_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
            return true;
        }

        case IDI_USER_ADD_SCORE: {	//��ע��ʱ
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

			//DONE:���ѡ����ע,��������
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

        case IDI_OPEN_CARD: {		//���ƶ�ʱ
			CMD_C_UserShowCard showCard;
			ZeroMemory(&showCard, sizeof(showCard));
			showCard.result = m_GameLogic.checkNNType(m_HandCardData, -1);
			m_pIAndroidUserItem->SendSocketData(SUB_C_USER_SHOW_CARD, &showCard, sizeof(showCard));
            return true;
        }
    }

    return false;
}

//��Ϸ��Ϣ
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

    //�������
    ASSERT(FALSE);
    return true;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID* pData, WORD wDataSize) {
    return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID* pData, WORD wDataSize) {
    switch (cbGameStatus) {
        case GS_NN_FREE: {	//����״̬
            //Ч������
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

//�û�����
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//�û��뿪
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//�û�״̬
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//�û���λ
void  CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem* pIAndroidUserItem, bool bLookonUser) {
    return;
}

//�û���ׯ
bool CAndroidUserItemSink::OnSnatchBanker(const void* pBuffer, WORD wDataSize) {
    if (wDataSize != sizeof(CMD_S_SnatchBanker)) return false;

    auto pInfo = static_cast<const CMD_S_SnatchBanker*>(pBuffer);

    ZeroMemory(m_HandCardData, sizeof(m_HandCardData));
    memcpy(m_HandCardData, pInfo->cards[m_pIAndroidUserItem->GetChairID()], sizeof(BYTE) * pInfo->cardCount);
    m_MaxRatio = pInfo->maxRatio;

    //��ׯʱ��
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

    //��ׯʱ��
    UINT nElapse = rand() % (3) + TIME_LESS;
    m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE, nElapse);
    return true;
}

bool CAndroidUserItemSink::OnSendCardAdd(const void* pBuffer, WORD wDataSize) {
	if (wDataSize != sizeof(CMD_S_SendCardAdd)) return false;

	auto pInfo = static_cast<const CMD_S_SendCardAdd*>(pBuffer);

	m_HandCardData[MAX_HAND_CARD - 1] = pInfo->playerCards[m_pIAndroidUserItem->GetChairID()];

	//��ׯʱ��
	UINT nElapse = rand() % (4) + TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD, nElapse);
	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void* pBuffer, WORD wDataSize) {
    //Ч������
// 	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
// 	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
//
// 	//���ñ���
// 	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
//
// 	//���ó���
// 	if (pGameStart->lTurnMaxScore>0)
// 	{
// 		//��עʱ��
// 		UINT nElapse=rand()%(4)+TIME_LESS;
// 		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
// 	}

    return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void* pBuffer, WORD wDataSize) {
    //Ч������
// 	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
// 	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

    return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void* pBuffer, WORD wDataSize) {
    //Ч������
// 	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
// 	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
//
// 	//��������
// 	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
// 	CopyMemory(m_HandCardData,pSendCard->cbCardData[wMeChiarID],sizeof(m_HandCardData));
//
// 	//����ʱ��
// 	UINT nElapse=rand()%(6)+TIME_LESS*2;
// 	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

    return true;
}

//�û�̯��
bool CAndroidUserItemSink::OnSubOpenCard(const void* pBuffer, WORD wDataSize) {
    //Ч������
// 	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
// 	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

    return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void* pBuffer, WORD wDataSize) {
    //Ч������
    return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void* pBuffer, WORD wDataSize) {
    //Ч�����
// 	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
// 	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
//
// 	//ɾ����ʱ��
// 	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
// 	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
// 	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
//
// 	//��ʼʱ��
// 	UINT nElapse=rand()%(3)+TIME_LESS;
// 	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
//
// 	//�������
// 	m_lTurnMaxScore = 0;
// 	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

    return true;
}

//���в���
void CAndroidUserItemSink::BankerOperate() {

    //��������
    IServerUserItem* pUserItem = m_pIAndroidUserItem->GetMeUserItem();
    LONGLONG lRobotScore = pUserItem->GetUserScore();// + m_lBankerLostScore;
    LONGLONG lInsureScore = pUserItem->GetUserInsure();

    {
        //�жϴ�ȡ
        if (lRobotScore > m_lRobotScoreRange[1]) {
            //������Ϣ
            LONGLONG lStorageValue = LONGLONG(lRobotScore * m_nRobotBankStorageMul / 100);

            if (lStorageValue > lRobotScore)
                lStorageValue = lRobotScore;


            if (lStorageValue > 0)
                m_pIAndroidUserItem->PerformSaveScore(lStorageValue);
        } else if (lRobotScore < m_lRobotScoreRange[0]) {
            //������Ϣ
            LONGLONG lGetValue =  m_lRobotBankGetScore;

            if (lGetValue > lInsureScore)
                lGetValue = lInsureScore;

            if (lGetValue > 0)
                m_pIAndroidUserItem->PerformTakeScore(lGetValue);
        }
    }

}


//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation(TCHAR szRoomName[]) {

    //��ȡĿ¼
    TCHAR szPath[MAX_PATH] = TEXT("");
    GetCurrentDirectory(CountArray(szPath), szPath);

    _sntprintf(m_szRoomName, sizeof(m_szRoomName), TEXT("OxEx"));

    //��ȡ����
    TCHAR szConfigFileName[MAX_PATH] = TEXT("");
    _sntprintf(szConfigFileName, sizeof(szConfigFileName), TEXT("%s\\OxServer.ini"), szPath);


    TCHAR OutBuf[255] = TEXT("");

    //�������
    ZeroMemory(OutBuf, sizeof(OutBuf));
    GetPrivateProfileString(szRoomName, TEXT("RobotBankGet"), _T("20000000"), OutBuf, 255, szConfigFileName);
    _sntprintf(OutBuf, sizeof(OutBuf), _T("%I64d"), &m_lRobotBankGetScore);

    //����
    m_nRobotBankStorageMul = GetPrivateProfileInt(szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

    if (m_nRobotBankStorageMul < 0 || m_nRobotBankStorageMul > 100) m_nRobotBankStorageMul = 20;

    //��������
    ZeroMemory(OutBuf, sizeof(OutBuf));
    GetPrivateProfileString(szRoomName, TEXT("RobotScoreMin"), _T("10000000"), OutBuf, 255, szConfigFileName);
    _sntprintf(OutBuf, sizeof(OutBuf), _T("%I64d"), &m_lRobotScoreRange[0]);

    ZeroMemory(OutBuf, sizeof(OutBuf));
    GetPrivateProfileString(szRoomName, TEXT("RobotScoreMax"), _T("100000000"), OutBuf, 255, szConfigFileName);
    _sntprintf(OutBuf, sizeof(OutBuf), _T("%I64d"), &m_lRobotScoreRange[1]);

    if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];
}

//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
