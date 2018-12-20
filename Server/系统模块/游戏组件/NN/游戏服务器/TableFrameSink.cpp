#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "DlgCustomRule.h"

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <fcntl.h>

//////////////////////////////////////////////////////////////////////////
//��ׯ
#define IDI_TIMER_SNATCH_BANKER 1
//��ע
#define IDI_TIMER_USER_CALL 2
//����
#define IDI_TIMER_COMPARE_CARD 3	
//��Ҳ���
#define IDI_TIMER_USER_SPLIT_CARD 4
//������״̬
#define IDI_TIMER_CHECK_USER_STATUS 5
// ����չʾ
#define IDI_TIMER_CALCULATE_SHOW 6
// ��Ϸ�������������׼��
#define IDI_TIMER_ALL_USER_READY 7

#define TIME_SNATCH_BANKER TIME_FOR_SNATCH_BANKER * 1000
#define TIME_USER_CALL	TIME_FOR_USER_CALL * 1000
#define TIME_COMPARE_CARD TIME_FOR_USER_SPLIT_CARD * 1000
#define TIME_USER_SPLIT_CARD TIME_FOR_USER_SPLIT_CARD * 1000
#define TIME_CHECK_USER_STATUS 1 * 1000
#define TIME_CALCULATE_SHOW 5 * 1000
#define TIMER_ALL_USER_READY 12 * 1000

static int ANDROID_WIN_RATIO = 50;
static int ADMINUSER_WIN_RATIO = 50;
static std::vector<unsigned int> g_AdminUserVec;
static WORD g_TuiZhuRatio[MAX_TuiZhu_INDEX];

//���캯��
CTableFrameSink::CTableFrameSink() 
{
    m_pITableFrame = NULL;
    m_GameRuleIdex = 0;
    m_GameTypeIdex = 0;
    m_BankerScore = 0;
    m_dwCellScore = 1;
    m_GameStatus = NNGameStatus_Free;
	_wCurTuiZhuRatio = 0;
    ZeroMemory(m_GameCards, MAX_CARD_COUNT);
	ZeroMemory(m_PlayerSingleResultRecord, sizeof(m_PlayerSingleResultRecord));
	ZeroMemory(_calculate_total, sizeof(_calculate_total));
	_dwCurrentPlayRound = 0;

	_pMasterUser = nullptr;
	_MasterChairID = 0;
	_MasterUserID = 0;
	_pCreateUser = nullptr;
    _dwCreateUserID = 0;

	g_TuiZhuRatio[0] = TuiZhuBeiShu_0;
	g_TuiZhuRatio[1] = TuiZhuBeiShu_1;
	g_TuiZhuRatio[2] = TuiZhuBeiShu_2;
	g_TuiZhuRatio[3] = TuiZhuBeiShu_3;

    //��Ϸ����
    RepositionSink();

    //wcb
#if defined TEST_CONSOLE
    AllocConsole();										//�򿪿���̨��������ʾ������Ϣ
    SetConsoleTitleA("Debug Win Output");					//���ñ���
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);		//��ȡ����̨������
    INT hCrt = _open_osfhandle((INT)hCon, _O_TEXT);		//ת��ΪC�ļ�������
    FILE* hf = _fdopen(hCrt, "w");						//ת��ΪC�ļ���
    setvbuf(hf, NULL, _IONBF, 0);						//�޻���
    *stdout = *hf;										//�ض����׼���
#endif
}

//��������
CTableFrameSink::~CTableFrameSink(void) {
}

//�ӿڲ�ѯ
void* CTableFrameSink::QueryInterface(const IID& Guid, DWORD dwQueryVer) {
    QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);
    QUERYINTERFACE(ITableUserAction, Guid, dwQueryVer);
    QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
    return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx* pIUnknownEx) {
    //��ѯ�ӿ�
    ASSERT(pIUnknownEx != NULL);
    m_pITableFrame = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITableFrame);

    if (m_pITableFrame == NULL)
        return false;

    //��ȡ����
    m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
    ASSERT(m_pGameServiceOption != NULL);

    //��ʼģʽ
    m_pITableFrame->SetStartMode(START_MODE_MASTER_CONTROL);

    return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink() {
	killAllTimerEvent();
    m_UseCheatingCards = false;
    ZeroMemory(m_PlayerCardsCheating, sizeof(m_PlayerCardsCheating));
    m_BankerRatio = 1;
    m_CardLeftCount = MAX_CARD_COUNT;
    ZeroMemory(m_PlayerAllBets, sizeof(m_PlayerAllBets));
    ZeroMemory(m_PlayerBets, sizeof(m_PlayerBets));
    ZeroMemory(m_PlayerCardTypes, sizeof(m_PlayerCardTypes));
    ZeroMemory(m_PlayerSingleResult, sizeof(m_PlayerSingleResult));
    ZeroMemory(m_PlayerCardsAdd, sizeof(m_PlayerCardsAdd));

    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        m_PlayerBankerRatio[index] = -1;
    }

	_wCurTuiZhuRatio = 0;

    return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart() {
    RepositionSink();

    for (int playerIndex = 0; playerIndex < NN_GAME_PLAYER; ++playerIndex) {
        if (m_PlayerStatus[playerIndex] == NNPlayerStatus_Ready) {
            m_PlayerStatus[playerIndex] = NNPlayerStatus_Playing;
        }
    }

//���Ƿ���ģʽ�۷����
#ifndef ENABLE_ROOM_CARD_TYPE
    tagScoreInfo ScoreInfoArray[NN_GAME_PLAYER];
    ZeroMemory(ScoreInfoArray, sizeof(ScoreInfoArray));
    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        if (NULL == m_pITableFrame->GetTableUserItem(index)) {
            continue;
        }
        ScoreInfoArray[index].lRevenue = 0;
        ScoreInfoArray[index].lScore = -(((SCORE) m_dwCellScore) * 30 / 100);
        ScoreInfoArray[index].cbType = SCORE_TYPE_SERVICE;
        ScoreInfoArray[index].lGrade = -1;
        m_pITableFrame->WriteUserScore(index, ScoreInfoArray[index]);
    }
#endif

    CMD_S_GameStart gameStart;
    ZeroMemory(&gameStart, sizeof(gameStart));
    memcpy(gameStart.status, m_PlayerStatus, sizeof(gameStart.status));
    m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START, &gameStart, sizeof(gameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_START, &gameStart, sizeof(gameStart));

    startGame();
    m_pITableFrame->SetGameStatus(GS_NN_PLAY);
    return true;
}

void CTableFrameSink::Shuffle() {
    m_GameLogic.initCard(m_GameCards, MAX_CARD_COUNT);
    m_GameLogic.RandCardData(m_GameCards, MAX_CARD_COUNT);
    m_CardLeftCount = MAX_CARD_COUNT;
}

void CTableFrameSink::rationCardForUser(WORD cardCount) {
    std::vector<BYTE> cardList;

#if defined TEST_CODE
	//DONE:��������
	//0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, //����
	m_GameCards[0] = 0x22;
	m_GameCards[2] = 0x04;
	m_GameCards[4] = 0x03;
	m_GameCards[6] = 0x06;

	m_GameCards[1] = 0x22;
	m_GameCards[3] = 0x04;
	m_GameCards[5] = 0x03;
	m_GameCards[7] = 0x06;
#endif

    for (int cardIndex = 0; cardIndex < m_CardLeftCount; ++cardIndex) {
        cardList.push_back(m_GameCards[cardIndex]);
    }

    float androidWinRatio = (ANDROID_WIN_RATIO - 50) * 2;
    float androidLoseRatio = -1 * androidWinRatio;

	float adminUserWinRatio = (ADMINUSER_WIN_RATIO - 50) * 2;
	float adminUserLoseRatio = -1 * adminUserWinRatio;

    ZeroMemory(m_PlayerCardsCheating, sizeof(m_PlayerCardsCheating));
    int cardCountTemp = 0;
    for (int cardIndex = 0; cardIndex < MAX_HAND_CARD; ++cardIndex) {
        for (int playerIndex = 0; playerIndex < NN_GAME_PLAYER; ++playerIndex) {
            if (m_PlayerStatus[playerIndex] == NNPlayerStatus_Playing) {
                m_PlayerCardsCheating[playerIndex][cardIndex] = cardList.at(cardCountTemp++);
            }
        }
    }

    if (androidWinRatio > 0) {
        for (int index = 0; index < NN_GAME_PLAYER; ++index) {
            if (m_pITableFrame->GetTableUserItem(index) != nullptr && m_pITableFrame->GetTableUserItem(index)->IsAndroidUser()) {
                int randVar = rand() % 100;
                if (randVar < androidWinRatio) {
                    m_UseCheatingCards = true;
                    callCheatingCards(index);
                }
            }
        }
    } else if (androidWinRatio < 0) {
        for (int index = 0; index < NN_GAME_PLAYER; ++index) {
            if (m_pITableFrame->GetTableUserItem(index) != nullptr && m_pITableFrame->GetTableUserItem(index)->IsAndroidUser()) {
                int randVar = rand() % 100;
                if (randVar < androidLoseRatio) {
                    m_UseCheatingCards = true;
                    callCheatingCards(index, false);
                }
            }
        }
    }

	if (adminUserWinRatio > 0) {
		for (int index = 0; index < NN_GAME_PLAYER; ++index) {
			if (m_pITableFrame->GetTableUserItem(index) != nullptr && isAdminUser(m_pITableFrame->GetTableUserItem(index)->GetUserID())) {
				int randVar = rand() % 100;
				if (randVar < adminUserWinRatio) {
					m_UseCheatingCards = true;
					callCheatingCards(index);
				}
			}
		}
	} else if (adminUserWinRatio > 0) {
		for (int index = 0; index < NN_GAME_PLAYER; ++index) {
			if (m_pITableFrame->GetTableUserItem(index) != nullptr && isAdminUser(m_pITableFrame->GetTableUserItem(index)->GetUserID())) {
				int randVar = rand() % 100;
				if (randVar < adminUserLoseRatio) {
					m_UseCheatingCards = true;
					callCheatingCards(index, false);
				}
			}
		}
	}

    if (!m_UseCheatingCards) {
        int rationCount = 0;
        for (int cardIndex = 0; cardIndex < cardCount; ++cardIndex) {
            for (int playerIndex = 0; playerIndex < NN_GAME_PLAYER; ++playerIndex) {
                if (m_PlayerStatus[playerIndex] == NNPlayerStatus_Playing) {
                    m_PlayerCards[playerIndex][cardIndex] = cardList.at(0);
                    BYTE cardValue = m_PlayerCards[playerIndex][cardIndex];
                    m_PlayerCardsName[playerIndex][cardIndex] = std::string(CGameLogic::GAME_CARDS_NAME[((cardValue & MASK_COLOR) >> 4) * (CARD_INDEX_MAX - CARD_INDEX_MIN + 1) + (cardValue & MASK_VALUE) - CARD_INDEX_MIN].c_str());
                    cardList.erase(cardList.begin());
                    rationCount++;
                }
            }
        }

        m_CardLeftCount -= rationCount;

        ZeroMemory(m_GameCards, sizeof(m_GameCards));

        for (int index = 0; index < m_CardLeftCount; ++index) {
            m_GameCards[index] = cardList.at(index);
        }
    }

    switch (m_GameTypeIdex) {
        case NNGameType_NNBanker:
        case NNGameType_HostBanker:
        case NNGameType_AllCompare: {
			{
				CMD_S_SendCardAll sendCardAll;
				ZeroMemory(&sendCardAll, sizeof(sendCardAll));
				ASSERT(sizeof(sendCardAll.playerStatus) == sizeof(m_PlayerStatus));
				memcpy(sendCardAll.playerStatus, m_PlayerStatus, sizeof(m_PlayerStatus));
				m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SEND_CARD_ALL, &sendCardAll, sizeof(sendCardAll)); 
			}
            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                CMD_S_SendCardAll sendCardAll;
                ZeroMemory(&sendCardAll, sizeof(sendCardAll));
                ASSERT(sizeof(sendCardAll.playerStatus) == sizeof(m_PlayerStatus));
                memcpy(sendCardAll.playerStatus, m_PlayerStatus, sizeof(m_PlayerStatus));
                if (m_UseCheatingCards) {
                    ASSERT(sizeof(sendCardAll.playerCards[index]) == sizeof(m_PlayerCardsCheating[index]));
                    memcpy(sendCardAll.playerCards[index], m_PlayerCardsCheating[index], sizeof(m_PlayerCardsCheating[index]));
                } else {
                    ASSERT(sizeof(sendCardAll.playerCards[index]) == sizeof(m_PlayerCards[index]));
                    memcpy(sendCardAll.playerCards[index], m_PlayerCards[index], sizeof(m_PlayerCards[index]));
                }
                m_pITableFrame->SendTableData(index, SUB_S_SEND_CARD_ALL, &sendCardAll, sizeof(sendCardAll));
                m_pITableFrame->KillGameTimer(IDI_TIMER_USER_SPLIT_CARD);
                m_pITableFrame->SetGameTimer(IDI_TIMER_USER_SPLIT_CARD, TIME_USER_SPLIT_CARD, 1, NULL);
            }

            CMD_S_SendCardAll sendCardAll;
            ZeroMemory(&sendCardAll, sizeof(sendCardAll));
            memcpy(sendCardAll.playerCards, m_PlayerCards, sizeof(m_PlayerCards));
            addGameOperator(sendCardAll);

            m_GameStatus = NNGameStatus_SplitCard;
        }
        break;

        case NNGameType_SnatchBanker:
        case NNGameType_SnatchBankerShowCard:
        case NNGameType_NNRatio:
        case NNGameType_HLN: {
			BYTE cbPlayerCount = 0;
            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (NULL == m_pITableFrame->GetTableUserItem(index)) {
                    continue;
                }
                cbPlayerCount++;
            }
            ASSERT(cbPlayerCount >= 2);
			
            for (WORD index = 0; index < NN_GAME_PLAYER; ++index) {
                if (NULL == m_pITableFrame->GetTableUserItem(index)) {
                    continue;
                }
                CMD_S_SnatchBanker snatchBanker;
                ZeroMemory(&snatchBanker, sizeof(snatchBanker));
                ASSERT(sizeof(snatchBanker.status) == sizeof(m_PlayerStatus));
                memcpy(snatchBanker.status, m_PlayerStatus, sizeof(m_PlayerStatus));
                snatchBanker.maxRatio = m_MaxRatio;
                snatchBanker.cardCount = cardCount;
                if (m_UseCheatingCards) {
                    ASSERT(sizeof(snatchBanker.cards[index]) == sizeof(m_PlayerCardsCheating[index]));
                    memcpy(snatchBanker.cards[index], m_PlayerCardsCheating[index], sizeof(m_PlayerCardsCheating[index]));
                } else {
                    ASSERT(sizeof(snatchBanker.cards[index]) == sizeof(m_PlayerCards[index]));
                    memcpy(snatchBanker.cards[index], m_PlayerCards[index], sizeof(m_PlayerCards[index]));
                }
				snatchBanker.bTuiZhu = checkTuiZhuScore(index);
                m_pITableFrame->SendTableData(index, SUB_S_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
            }
            CMD_S_SnatchBanker snatchBanker;
            ZeroMemory(&snatchBanker, sizeof(snatchBanker));
            snatchBanker.maxRatio = m_MaxRatio;
            snatchBanker.cardCount = cardCount;
            memcpy(snatchBanker.cards, m_PlayerCards, sizeof(m_PlayerCards));
            addGameOperator(snatchBanker);

            m_pITableFrame->SetGameTimer(IDI_TIMER_SNATCH_BANKER, TIME_SNATCH_BANKER, 1, NULL);
            m_GameStatus = NNGameStatus_SnatchBanker;
            break;
        }
        default:
            break;
    }
}

bool CTableFrameSink::isAdminUser(DWORD userID) {
	for (size_t  index = 0; index < g_AdminUserVec.size(); index++) {
		if (g_AdminUserVec.at(index) == userID) {
			return true;
		}
	}
	return false;
}

void CTableFrameSink::rationCardForUser_Add() {
#if defined TEST_CODE
	//DONE:��������
	m_GameCards[0] = 0x16;
	m_GameCards[1] = 0x17;

	m_GameCards[2] = 0x15;
	m_GameCards[3] = 0x27;
#endif // DEBUG

    std::vector<BYTE> cardList;
    for (int cardIndex = 0; cardIndex < m_CardLeftCount; ++cardIndex) {
        cardList.push_back(m_GameCards[cardIndex]);
    }

    ZeroMemory(m_PlayerCardsAdd, sizeof(m_PlayerCardsAdd));

    if (!m_UseCheatingCards) {
        int rationCount = 0;
        for (int playerIndex = 0; playerIndex < NN_GAME_PLAYER; ++playerIndex) {
            if (m_PlayerStatus[playerIndex] == NNPlayerStatus_Playing) {
                m_PlayerCards[playerIndex][MAX_HAND_CARD - 1] = cardList.at(0);
                m_PlayerCardsAdd[playerIndex] = cardList.at(0);
                cardList.erase(cardList.begin());
                rationCount++;
            }
        }
        m_CardLeftCount -= rationCount;

        ZeroMemory(m_GameCards, sizeof(m_GameCards));
        for (int index = 0; index < m_CardLeftCount; ++index) {
            m_GameCards[index] = cardList.at(index);
        }
    } 
	else 
	{
        for (int playerIndex = 0; playerIndex < NN_GAME_PLAYER; ++playerIndex) {
            if (m_PlayerStatus[playerIndex] == NNPlayerStatus_Playing) {
                m_PlayerCardsAdd[playerIndex] = m_PlayerCardsCheating[playerIndex][MAX_HAND_CARD - 1];
            }
        }
    }


    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        if (NULL == m_pITableFrame->GetTableUserItem(index)) {
            continue;
        }

        CMD_S_SendCardAdd cardAdd;
        ZeroMemory(&cardAdd, sizeof(cardAdd));
        ASSERT(sizeof(cardAdd.playerStatus) == sizeof(m_PlayerStatus));
        memcpy(cardAdd.playerStatus, m_PlayerStatus, sizeof(m_PlayerStatus));
        cardAdd.playerCards[index] = m_PlayerCardsAdd[index];
        m_pITableFrame->SendTableData(index, SUB_S_SEND_CARD_ADD, &cardAdd, sizeof(cardAdd));
        addGameOperator(cardAdd);
    }

    m_GameStatus = NNGameStatus_SplitCard;
    m_pITableFrame->KillGameTimer(IDI_TIMER_USER_SPLIT_CARD);
    m_pITableFrame->SetGameTimer(IDI_TIMER_USER_SPLIT_CARD, TIME_USER_SPLIT_CARD, 1, NULL);
}

void CTableFrameSink::startGame() {
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetCurrentDirectory(CountArray(szPath), szPath);
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), TEXT("%s\\NNServer.ini"), szPath);
	ANDROID_WIN_RATIO = GetPrivateProfileInt(_T("NN"), _T("AndroidWinRatio"), 50, szConfigFileName);

	g_AdminUserVec.clear();
	ADMINUSER_WIN_RATIO = GetPrivateProfileInt(_T("NN"), _T("AdminUserWinRatio"), 50, szConfigFileName);
	char adminConfig[1024] = { 0 };
	GetPrivateProfileString(_T("NN"), _T("AdminUserList"), "", adminConfig, 1024, szConfigFileName);
	char *delim = ",";
	char *p;
	p = strtok(adminConfig, delim);

	if (p != nullptr) {
		g_AdminUserVec.push_back(std::atoi(p));
	}
	while (p = strtok(NULL, delim)) {
		g_AdminUserVec.push_back(std::atoi(p));
	}

    startGameRecord();
    Shuffle();

    switch (m_GameTypeIdex) {
        case	NNGameType_NNBanker:
        case NNGameType_HostBanker: {
            CMD_S_BankerInfo bankerInfo;
            ZeroMemory(&bankerInfo, sizeof(bankerInfo));

            if (m_GameTypeIdex == NNGameType_HostBanker) {
                m_BankerChairID = _MasterChairID;
            } else {
                if (m_BankerChairID == INVALID_CHAIR) {
                    getBanker();
                }
            }

            getBets();
            bankerInfo.bankerChairID = m_BankerChairID;
            bankerInfo.bankerRatio = 0;
            memcpy(bankerInfo.bets, m_PlayerAllBets, sizeof(m_PlayerAllBets));
            m_GameStatus = NNGameStatus_Call;
            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BANKER_INFO, &bankerInfo, sizeof(bankerInfo));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_BANKER_INFO, &bankerInfo, sizeof(bankerInfo));
            m_pITableFrame->SetGameTimer(IDI_TIMER_USER_CALL, TIME_USER_CALL, 1, NULL);
            addGameOperator(bankerInfo);
        }
        break;

        case NNGameType_SnatchBanker: {
            rationCardForUser(MAX_HAND_CARD);
        }
        break;

        case NNGameType_SnatchBankerShowCard:
        case NNGameType_NNRatio:
        case NNGameType_HLN: {
            rationCardForUser(MAX_HAND_CARD - 1);
        }
        break;

        case NNGameType_AllCompare: {
            CMD_S_BankerInfo bankerInfo;
            ZeroMemory(&bankerInfo, sizeof(bankerInfo));
            m_BankerChairID = INVALID_CHAIR;
            getBets();
            bankerInfo.bankerChairID = m_BankerChairID;
            bankerInfo.bankerRatio = 0;
            memcpy(bankerInfo.bets, m_PlayerAllBets, sizeof(m_PlayerAllBets));

            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                m_PlayerBets[index] = m_PlayerAllBets[0][0];
            }

            m_GameStatus = NNGameStatus_Call;
            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BANKER_INFO, &bankerInfo, sizeof(bankerInfo));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_BANKER_INFO, &bankerInfo, sizeof(bankerInfo));
            addGameOperator(bankerInfo);

            rationCardForUser(MAX_HAND_CARD);
        }
        break;

        default:
            break;
    }
}

void CTableFrameSink::getBanker() {
    int playerCount = 0;

    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        if (m_PlayerStatus[index] == NNPlayerStatus_Playing) {
            playerCount++;
        }
    }

    if (playerCount == 0) {
        m_BankerChairID = INVALID_CHAIR;
        return;
    }

    int bankerIndex = rand() % playerCount;

    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        if (m_PlayerStatus[index] == NNPlayerStatus_Playing) {
            if (0 == bankerIndex) {
                m_BankerChairID = index;
                return;
            }

            --bankerIndex;
        }
    }

    m_BankerChairID = INVALID_CHAIR;
}

void CTableFrameSink::confirmSnatchBanker() {
    int maxRatio = 0;
    int maxRatioCount = 0;
    int maxRatioChair = INVALID_CHAIR;

    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        if (m_PlayerBankerRatio[index] > maxRatio) {
            maxRatio = m_PlayerBankerRatio[index];
            maxRatioChair = index;
            maxRatioCount = 1;
        } else if (m_PlayerBankerRatio[index] == maxRatio) {
            maxRatioCount++;
        }
    }

	if (maxRatioCount == 0) {
		return;
	} else if (maxRatioCount == 1) {
        m_BankerChairID = maxRatioChair;
        m_BankerRatio = maxRatio;
    } else {
        int bankerIndex = rand() % maxRatioCount;

        for (int index = 0; index < NN_GAME_PLAYER; ++index) {
            if (m_PlayerBankerRatio[index] == maxRatio) {
                if (bankerIndex == 0) {
                    m_BankerChairID = index;
                    m_BankerRatio = max(maxRatio, 1);
                    break;
                }

                --bankerIndex;
            }
        }
    }

    getBets();

	m_GameStatus = NNGameStatus_Call;

    CMD_S_BankerInfo bankerInfo;
    BYTE cbPlyerCount = 0;
    for (WORD index = 0; index < NN_GAME_PLAYER; ++index) {
        if (NULL == m_pITableFrame->GetTableUserItem(index)) {
            continue;
        }
        cbPlyerCount++;
    }
    ASSERT(cbPlyerCount >= 2);
    ZeroMemory(&bankerInfo, sizeof(bankerInfo));
    bankerInfo.bankerChairID = m_BankerChairID;
    bankerInfo.bankerRatio = m_BankerRatio;
    memcpy(bankerInfo.bets, m_PlayerAllBets, sizeof(m_PlayerAllBets));
    m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BANKER_INFO, &bankerInfo, sizeof(bankerInfo));
	//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_BANKER_INFO, &bankerInfo, sizeof(bankerInfo));
    m_pITableFrame->SetGameTimer(IDI_TIMER_USER_CALL, TIME_USER_CALL, 1, NULL);
    addGameOperator(bankerInfo);
}

int CTableFrameSink::checkTuiZhuScore(WORD wChairID)
{
	if (wChairID > NN_GAME_PLAYER) {
		return 0;
	}
	if (!hasRule(NNGameRule_TZ)) {
		return 0;
	}
	if (m_PlayerSingleResultRecord[wChairID].wScore > 0 &&
		!m_PlayerSingleResultRecord[wChairID].bTuiZhu &&
		!m_PlayerSingleResultRecord[wChairID].bBanker) {
		switch (m_PlayerSingleResultRecord[wChairID].wCardType) {
		case 	NNCardType_None:
		case NNCardType_N1:
		case 	NNCardType_N2:
		case 	NNCardType_N3:
			return TuiZhuBeiShu_0 * m_dwCellScore;
		case NNCardType_N4:
		case 	NNCardType_N5:
		case 	NNCardType_N6:
			return TuiZhuBeiShu_1 * m_dwCellScore;
		case NNCardType_N7:
		case 	NNCardType_N8:
		case 	NNCardType_N9:
			return TuiZhuBeiShu_2 * m_dwCellScore;
		case	NNCardType_NN:
		case	NNCardType_SZN: //˳��ţ
		case	NNCardType_WHN://�廨ţ
		case	NNCardType_THN: //ͬ��ţ
		case	NNCardType_HLN: //��«ţ
		case	NNCardType_ZDN://ը��ţ
		case	NNCardType_WXN: //��Сţ
			return TuiZhuBeiShu_3 * m_dwCellScore;
		}
	}
	return 0;
}

void CTableFrameSink::getBets()
{
	ZeroMemory(m_PlayerAllBets, sizeof(m_PlayerAllBets));
	for (int index = 0; index < NN_GAME_PLAYER; ++index) {
		if (m_PlayerStatus[index] != NNPlayerStatus_Playing || index == m_BankerChairID) {
			continue;
		}

		m_PlayerAllBets[index][0].wBet = m_dwCellScore;
		if (m_GameTypeIdex != NNGameType_AllCompare) {
			m_PlayerAllBets[index][1].wBet = m_dwCellScore * 2;
			if (hasRule(NNGameRule_TZ)) {
				//��ע�����ж�
				if (m_PlayerSingleResultRecord[index].wScore > 0 &&
					!m_PlayerSingleResultRecord[index].bTuiZhu &&
					!m_PlayerSingleResultRecord[index].bBanker) {
					switch (m_PlayerSingleResultRecord[index].wCardType) {
					case 	NNCardType_None:
					case NNCardType_N1:
					case 	NNCardType_N2:
					case 	NNCardType_N3:
						m_PlayerAllBets[index][2].wBet = TuiZhuBeiShu_0 * m_dwCellScore;
						m_PlayerAllBets[index][2].wBetType = NNGBT_TuiZhu;
						break;
					case NNCardType_N4:
					case 	NNCardType_N5:
					case 	NNCardType_N6:
						m_PlayerAllBets[index][2].wBet = TuiZhuBeiShu_1 * m_dwCellScore;
						m_PlayerAllBets[index][2].wBetType = NNGBT_TuiZhu;
						break;
					case NNCardType_N7:
					case 	NNCardType_N8:
					case 	NNCardType_N9:
						m_PlayerAllBets[index][2].wBet = TuiZhuBeiShu_2 * m_dwCellScore;
						m_PlayerAllBets[index][2].wBetType = NNGBT_TuiZhu;
						break;
					case	NNCardType_NN:
					case	NNCardType_SZN://˳��ţ
					case	NNCardType_WHN://�廨ţ
					case	NNCardType_THN://ͬ��ţ
					case	NNCardType_HLN://��«ţ
					case	NNCardType_ZDN://ը��ţ
					case	NNCardType_WXN: //��Сţ
						m_PlayerAllBets[index][2].wBet = TuiZhuBeiShu_3 * m_dwCellScore;
						m_PlayerAllBets[index][2].wBetType = NNGBT_TuiZhu;
						break;
					}
				}
			}
		}
	}
}

void CTableFrameSink::calculate() {
    CMD_S_Calculate cmd_calculate;
    ZeroMemory(&cmd_calculate, sizeof(cmd_calculate));
    ZeroMemory(m_PlayerSingleResult, sizeof(m_PlayerSingleResult));

    if (m_BankerChairID != INVALID_CHAIR) { //is not Banker
        NNCardType_Result bankerResult = m_PlayerCardTypes[m_BankerChairID];
        int sum = 0;
        for (int index = 0; index < NN_GAME_PLAYER; ++index) {
			_calculate_total[index].dwRoundCount = _dwCurrentPlayRound;
			_calculate_total[index].banker_id[_dwCurrentPlayRound] = m_BankerChairID;
            if (index == m_BankerChairID || m_PlayerStatus[index] != NNPlayerStatus_Playing) {
                continue;
            }
            NNCardType_Result playerResult = m_PlayerCardTypes[index];
            if (m_GameLogic.comparePlayerCards(bankerResult, playerResult)) {
                m_PlayerSingleResult[index] = -1 * static_cast<int>(m_PlayerBets[index].wBet * m_GameLogic.getNNRatio(bankerResult, m_GameRuleIdex) * m_BankerRatio);
				_calculate_total[index].playerScores[_dwCurrentPlayRound] = m_PlayerSingleResult[index];
            } else {
                m_PlayerSingleResult[index] = m_PlayerBets[index].wBet * m_GameLogic.getNNRatio(playerResult, m_GameRuleIdex) * m_BankerRatio;
				_calculate_total[index].playerScores[_dwCurrentPlayRound] = m_PlayerSingleResult[index];
            }
            cmd_calculate.playerScores[index] = m_PlayerSingleResult[index];
            sum += m_PlayerSingleResult[index];
        }
		_calculate_total[m_BankerChairID].playerScores[_dwCurrentPlayRound] = -1 * sum;
        m_PlayerSingleResult[m_BankerChairID] = -1 * sum;
        cmd_calculate.playerScores[m_BankerChairID] = m_PlayerSingleResult[m_BankerChairID];
		//DONE:���������ע��ʶ
		//ZeroMemory(m_PlayerSingleResultRecord, sizeof(m_PlayerSingleResultRecord));
		for (size_t idx = 0; idx < NN_GAME_PLAYER; idx++) {
			if (idx == m_BankerChairID) {
				m_PlayerSingleResultRecord[idx].bBanker = true;
			} else {
				m_PlayerSingleResultRecord[idx].bBanker = false;
			}
			m_PlayerSingleResultRecord[idx].wScore = m_PlayerSingleResult[idx];
			m_PlayerSingleResultRecord[idx].wCardType = m_PlayerCardTypes[idx].type;
		}
        m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CALCULATE, &cmd_calculate, sizeof(cmd_calculate));
		//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CALCULATE, &calculate, sizeof(calculate));
        addGameOperator(cmd_calculate);
    } else { //is not Banker
        int winChair = INVALID_CHAIR;
        NNCardType_Result maxResult;

        for (int index = 0; index < NN_GAME_PLAYER; ++index) {
            if (m_PlayerStatus[index] != NNPlayerStatus_Playing) {
                continue;
            }

            if (winChair == INVALID_CHAIR) {
                winChair = index;
                maxResult = m_PlayerCardTypes[index];
                continue;
            }

            NNCardType_Result playerResult = m_PlayerCardTypes[index];

            if (m_GameLogic.comparePlayerCards(maxResult, playerResult)) {
                continue;
            }

            winChair = index;
            maxResult = playerResult;
        }

        int sum = 0;

        for (int index = 0; index < NN_GAME_PLAYER; index++) {
            if (index == winChair || m_PlayerStatus[index] != NNPlayerStatus_Playing) {
                continue;
            }

            m_PlayerSingleResult[index] = -((SCORE) m_dwCellScore) * m_PlayerBets[index].wBet * m_GameLogic.getNNRatio(maxResult, m_GameRuleIdex);
            cmd_calculate.playerScores[index] = m_PlayerSingleResult[index];

            sum += m_PlayerSingleResult[index];
        }

        m_PlayerSingleResult[winChair] = -1 * sum;
        cmd_calculate.playerScores[winChair] = m_PlayerSingleResult[winChair];
        m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CALCULATE, &cmd_calculate, sizeof(cmd_calculate));
		//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CALCULATE, &calculate, sizeof(calculate));
        addGameOperator(cmd_calculate);
    }

    m_GameStatus = NNGameStatus_Calculate;
	m_LastBankerChairID = m_BankerChairID;
    m_pITableFrame->KillGameTimer(IDI_TIMER_CALCULATE_SHOW);
    m_pITableFrame->SetGameTimer(IDI_TIMER_CALCULATE_SHOW, TIME_CALCULATE_SHOW, 1, NULL);
}

void CTableFrameSink::calculateTotal()
{
	//DONE:ţţʵ��ʹ�õĽ���ʱ˽�˳�������Ϣ
	return;
	for (size_t idx = 0; idx < NN_GAME_PLAYER; idx++) {
		m_pITableFrame->SendTableData(idx, SUB_S_CALCULATE_TOTAL, &_calculate_total[idx], sizeof(_calculate_total[idx]));
	}
}

void CTableFrameSink::callCheatingCards(WORD chairID, bool forWin /*= true*/) {
    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        if (m_PlayerStatus[index] == NNPlayerStatus_Playing && index != chairID) {
            NNCardType_Result resultPlayer = m_GameLogic.checkNNType(m_PlayerCardsCheating[index], m_GameRuleIdex);
            NNCardType_Result resultAndroid = m_GameLogic.checkNNType(m_PlayerCardsCheating[chairID], m_GameRuleIdex);

            if (forWin) {
                if (m_GameLogic.comparePlayerCards(resultPlayer, resultAndroid)) {
                    std::swap(m_PlayerCardsCheating[index], m_PlayerCardsCheating[chairID]);
                }
            } else {
                if (!m_GameLogic.comparePlayerCards(resultPlayer, resultAndroid)) {
                    std::swap(m_PlayerCardsCheating[index], m_PlayerCardsCheating[chairID]);
                }
            }
        }
    }
}

void CTableFrameSink::killAllTimerEvent()
{
	if (m_pITableFrame) {
		m_pITableFrame->KillGameTimer(IDI_TIMER_SNATCH_BANKER);
		m_pITableFrame->KillGameTimer(IDI_TIMER_USER_CALL);
		m_pITableFrame->KillGameTimer(IDI_TIMER_COMPARE_CARD);
		m_pITableFrame->KillGameTimer(IDI_TIMER_USER_SPLIT_CARD);
		m_pITableFrame->KillGameTimer(IDI_TIMER_CALCULATE_SHOW);
		m_pITableFrame->KillGameTimer(IDI_TIMER_CHECK_USER_STATUS);
		m_pITableFrame->KillGameTimer(IDI_TIMER_ALL_USER_READY);
	}
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem* pIServerUserItem, BYTE cbReason) {
    switch (cbReason) {
        case GER_NORMAL: {	//�������
            tagScoreInfo ScoreInfoArray[NN_GAME_PLAYER];
            ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));

            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (NULL == m_pITableFrame->GetTableUserItem(index)) {
                    continue;
                }

                ScoreInfoArray[index].lScore = m_PlayerSingleResult[index];
                BYTE ScoreKind;

                if (ScoreInfoArray[index].lScore > 0L) {
                    ScoreKind = SCORE_TYPE_WIN;
                } else if (ScoreInfoArray[index].lScore < 0L) {
                    ScoreKind = SCORE_TYPE_LOSE;
                } else {
                    ScoreKind = SCORE_TYPE_DRAW;
                }

                ScoreInfoArray[index].cbType = ScoreKind;
                ScoreInfoArray[index].lRevenue = 0;
            }

			datastream dataStream;
			m_GameRecord.StreamValue(dataStream, true);
			m_pITableFrame->WriteTableScore(ScoreInfoArray, m_pITableFrame->GetChairCount(), dataStream);

#if defined TEST_CONSOLE
			WORD count = m_pITableFrame->GetSitUserCount();
			for (WORD i= 0; i < count; i++) {
				switch (ScoreInfoArray[i].cbType)
				{
				case SCORE_TYPE_WIN:
					printf("player %d win %d\n", i, ScoreInfoArray[i].lScore);
					break;
				case SCORE_TYPE_LOSE:
					printf("player %d lose %d\n", i, ScoreInfoArray[i].lScore);
					break;
				case 	SCORE_TYPE_DRAW:
					printf("player %d draw %d\n", i, ScoreInfoArray[i].lScore);
					break;
				default:
					break;
				}
			}
			printf("\n");
#endif

			bool needDismissRoom = false;

			if (m_GameTypeIdex == NNGameType_HostBanker) {
				m_BankerScore += ScoreInfoArray[m_BankerChairID].lScore;
				int bankerSocre = 0;

				if (hasRule(NNGameRule_BankerScore_1)) {
					bankerSocre = -100;
				} else if (hasRule(NNGameRule_BankerScore_2)) {
					bankerSocre = -150;
				} else if (hasRule(NNGameRule_BankerScore_3)) {
					bankerSocre = -200;
				}

				if (bankerSocre < 0 && m_BankerScore < bankerSocre) {
					needDismissRoom = true;
				}
			}

			_dwCurrentPlayRound++;
			if (_dwCurrentPlayRound >= _dwTotalPlayRound) {
				calculateTotal();
			}

			if (needDismissRoom) {
				m_pITableFrame->ConcludeGame(GER_MASTER_END);
			} else {
				m_pITableFrame->ConcludeGame(GS_NN_FREE);
			}

			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_NOTIFY_GAME_END);

			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_NOTIFY_GAME_END);
            m_GameStatus = NNGameStatus_HostConfirm;

            for (int  index = 0; index < NN_GAME_PLAYER; index++) {
                if (m_PlayerStatus[index] == NNPlayerStatus_Playing) {
                    m_PlayerStatus[index] = NNPlayerStatus_Sitting;
                }
            }

#if !defined TEST_CODE
			//DONE: ����ģʽ�²��Զ���ʼ��һ��
			m_pITableFrame->SetGameTimer(IDI_TIMER_CHECK_USER_STATUS, TIME_CHECK_USER_STATUS, 1, NULL);
			m_pITableFrame->SetGameTimer(IDI_TIMER_ALL_USER_READY, TIMER_ALL_USER_READY, 1, NULL);
#endif
            return true;
        }

        case GER_DISMISS: {	//��Ϸ��ɢ
			calculateTotal();
            m_pITableFrame->ConcludeGame(GS_NN_FREE);
			killAllTimerEvent();
            return true;
        }

        case GER_NETWORK_ERROR:		//�������
        case GER_USER_LEAVE: {	//�û�ǿ��
            m_pITableFrame->ConcludeGame(GS_NN_FREE);
            return true;
        }

        case GER_MASTER_END: {
            m_pITableFrame->ConcludeGame(GER_MASTER_END);
            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_NOTIFY_GAME_END);
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_NOTIFY_GAME_END);
            return true;
        }
    }

    //�������
    ASSERT(FALSE);
    return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret) 
{
    switch (m_GameStatus) 
	{
        case NNGameStatus_Free:
        case NNGameStatus_HostConfirm:
        case NNGameStatus_Start: 
		{
            CMD_S_GameScene gameScene;
            ZeroMemory(&gameScene, sizeof(gameScene));
            gameScene.gameStatus = m_GameStatus;
            memcpy(gameScene.playerStatus, m_PlayerStatus, sizeof(m_PlayerStatus));
            gameScene.bankerChairID = INVALID_CHAIR;
            gameScene.dwBaseScore = m_dwCellScore;
            m_pITableFrame->SendGameScene(pIServerUserItem, &gameScene, sizeof(gameScene));
            return true;
        }
        break;

        default: 
		{
            CMD_S_GameScene gameScene;
            ZeroMemory(&gameScene, sizeof(gameScene));
            gameScene.gameStatus = m_GameStatus;
            gameScene.bankerChairID = m_BankerChairID;
            gameScene.maxRatio = m_MaxRatio;
            /////////////////////////////////////////////////////////////////

			if (m_GameTypeIdex == NNGameType_SnatchBanker ||
				m_GameTypeIdex == NNGameType_SnatchBankerShowCard ||
				m_GameTypeIdex == NNGameType_NNRatio ||
				m_GameTypeIdex == NNGameType_HLN) {
				gameScene.bankerRatio = m_BankerRatio;
			}

			gameScene.cardCount = MAX_HAND_CARD;

			if (m_GameStatus < NNGameStatus_SplitCard) {
				if (m_GameTypeIdex == NNGameType_SnatchBankerShowCard ||
					m_GameTypeIdex == NNGameType_NNRatio ||
					m_GameTypeIdex == NNGameType_HLN) {
					gameScene.cardCount = MAX_HAND_CARD - 1;
				} else if (m_GameTypeIdex == NNGameType_SnatchBanker) {
					gameScene.cardCount = MAX_HAND_CARD;
				} else {
					gameScene.cardCount = 0;
				}
			}

			ASSERT(sizeof(gameScene.playerStatus) == sizeof(m_PlayerStatus));
			memcpy(gameScene.playerStatus, m_PlayerStatus, sizeof(m_PlayerStatus));
			ASSERT(sizeof(gameScene.playerSnatchRatio) == sizeof(m_PlayerBankerRatio));
			memcpy(gameScene.playerSnatchRatio, m_PlayerBankerRatio, sizeof(m_PlayerBankerRatio));
			ASSERT(sizeof(gameScene.playerBetsAllow) == sizeof(m_PlayerAllBets));
			memcpy(gameScene.playerBetsAllow, m_PlayerAllBets, sizeof(m_PlayerAllBets));
            ASSERT(sizeof(gameScene.playerBet) == sizeof(m_PlayerBets));
            memcpy(gameScene.playerBet, m_PlayerBets, sizeof(m_PlayerBets));
            ASSERT(sizeof(gameScene.playerCardTypes) == sizeof(m_PlayerCardTypes));
            memcpy(gameScene.playerCardTypes, m_PlayerCardTypes, sizeof(m_PlayerCardTypes));

            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (m_PlayerCardTypes[index].type != NNCardType_Invalid) {
                    if (m_UseCheatingCards) {
                        memcpy(gameScene.playerCards[index], m_PlayerCardsCheating[index], sizeof(BYTE) * gameScene.cardCount);
                    } else {
                        memcpy(gameScene.playerCards[index], m_PlayerCards[index], sizeof(m_PlayerCards[index]));
                    }
                }
            }

			if (pIServerUserItem->GetChairID() < NN_GAME_PLAYER) {
				if (m_UseCheatingCards) {
					memcpy(gameScene.playerCards[pIServerUserItem->GetChairID()], m_PlayerCardsCheating[pIServerUserItem->GetChairID()], sizeof(BYTE) * gameScene.cardCount);
				} else {
					memcpy(gameScene.playerCards[pIServerUserItem->GetChairID()], m_PlayerCards[pIServerUserItem->GetChairID()], sizeof(m_PlayerCards[pIServerUserItem->GetChairID()]));
				}
				gameScene.operatorLeftTimes = TIME_SNATCH_BANKER;
				m_pITableFrame->SendGameScene(pIServerUserItem, &gameScene, sizeof(gameScene));
			}
        }
        break;
    }
	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
    switch (wTimerID) {
        case IDI_TIMER_CHECK_USER_STATUS: {
            int realPlayerCount = 0;
            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (m_pITableFrame->GetTableUserItem(index) != NULL && !m_pITableFrame->GetTableUserItem(index)->IsAndroidUser()) {
                    if (m_PlayerStatus[index] == NNPlayerStatus_Ready) {
                        ++realPlayerCount;
                    }
                }
            }

            if (realPlayerCount > 1) {
                for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                    if (m_pITableFrame->GetTableUserItem(index) != NULL && m_pITableFrame->GetTableUserItem(index)->IsAndroidUser()) {
                        bool leave = (rand() % 2 == 0);
                        if (leave) {
                            m_pITableFrame->PerformStandUpActionReally(m_pITableFrame->GetTableUserItem(index));
                        }
                    }
                }
            }

            // ������ȷ����Ϸ�ɿ�ʼ
            if (m_GameStatus == NNGameStatus_HostConfirm) {
                BOOL canStartGame = true;
                int playerCount = 0;

				for (int index = 0; index < NN_GAME_PLAYER; ++index) {
					if (m_PlayerStatus[index] == NNPlayerStatus_Sitting) {
						canStartGame = FALSE;
						break;
					}

					if (m_PlayerStatus[index] == NNPlayerStatus_Ready) {
						++playerCount;
					}
				}

				if (canStartGame && playerCount >= 2) {
					m_GameStatus = NNGameStatus_Start;
					m_pITableFrame->StartGame();
				}
			}
			return true;
		}

        case IDI_TIMER_SNATCH_BANKER: {
            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (m_PlayerStatus[index] != NNPlayerStatus_Playing) {
                    continue;
                }

				if (m_PlayerBankerRatio[index] < 0) {
					m_PlayerBankerRatio[index] = 0;
					CMD_S_UserSnatchBanker snatchBanker;
					ZeroMemory(&snatchBanker, sizeof(snatchBanker));
					snatchBanker.userChairID = index;
					snatchBanker.ratio = 0;
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_USER_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
					addGameOperator(snatchBanker);
				}
			}

			confirmSnatchBanker();
			return true;
		}

        case IDI_TIMER_USER_CALL: {
            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (index != m_BankerChairID && m_PlayerStatus[index] == NNPlayerStatus_Playing && m_PlayerBets[index].wBet == 0) {
                    m_PlayerBets[index] = m_PlayerAllBets[index][0];

					if (m_PlayerBets[index].wBetType == NNGBT_TuiZhu) {
						m_PlayerSingleResultRecord[index].bTuiZhu = true;
					} else {
						m_PlayerSingleResultRecord[index].bTuiZhu = false;
					}

                    CMD_S_UserCall userCall;
                    ZeroMemory(&userCall, sizeof(userCall));
                    userCall.chairID = index;
                    userCall.playerBets = m_PlayerAllBets[index][0];
                    m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_USER_CALL, &userCall, sizeof(userCall));
					//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_USER_CALL, &userCall, sizeof(userCall));
                    addGameOperator(userCall);
					//m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_USER_TRUSTEESHIP, &trusteeship, sizeof(trusteeship));
                }
            }

			switch (m_GameTypeIdex) {
				case NNGameType_HostBanker:
				case NNGameType_NNBanker: {
					rationCardForUser(MAX_HAND_CARD);
					return true;
				}

				case NNGameType_SnatchBanker: {
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SPLIT_CARD);
					//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SPLIT_CARD);
					m_GameStatus = NNGameStatus_SplitCard;
					m_pITableFrame->KillGameTimer(IDI_TIMER_USER_SPLIT_CARD);
					m_pITableFrame->SetGameTimer(IDI_TIMER_USER_SPLIT_CARD, TIME_USER_SPLIT_CARD, 1, NULL);
					return true;
				}

				case NNGameType_SnatchBankerShowCard:
                case NNGameType_NNRatio:
                case NNGameType_HLN: {
					rationCardForUser_Add();
					return true;
				}

				default:
					break;
			}

			return true;
		}

		//case IDI_TIMER_USER_CALL_TRUSTEESHIP: {
		//	return true;
		//}
		
		case IDI_TIMER_USER_SPLIT_CARD: {

			if (m_GameStatus != NNGameStatus_SplitCard) {
#ifdef _DEBUG
				printf("��ʱ�����ڿ���״̬���ܿ���");
#endif
				return true;
			}

			for (int chairID = 0; chairID < NN_GAME_PLAYER; ++chairID) {
				if (m_PlayerStatus[chairID] == NNPlayerStatus_Playing && m_PlayerCardTypes[chairID].type == NNCardType_Invalid) {
					NNCardType_Result result = m_GameLogic.checkNNType(m_PlayerCards[chairID], m_GameRuleIdex);
					CMD_S_UserShowCard userShowCard;
					ZeroMemory(&userShowCard, sizeof(userShowCard));
					userShowCard.chairID = chairID;
					userShowCard.result = result;
					memcpy(userShowCard.playerCards, m_PlayerCards[chairID], sizeof(userShowCard.playerCards));
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_USER_SHOW_CARD, &userShowCard, sizeof(userShowCard));
					//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_USER_SHOW_CARD, &userShowCard, sizeof(userShowCard));

					m_PlayerCardTypes[chairID] = result;
					addGameOperator(userShowCard);
				}
			}

			calculate();
			return true;
		}

		case IDI_TIMER_CALCULATE_SHOW: {
			if (m_GameTypeIdex == NNGameType_NNBanker) {
				int nnCount = 0;
				int nnChair = INVALID_CHAIR;

				for (int index = 0; index < NN_GAME_PLAYER; ++index) {
					if (m_PlayerCardTypes[index].type >= NNCardType_NN) {
						nnCount++;
						nnChair = index;
					}
				}

				if (1 == nnCount) {
					m_BankerChairID = nnChair;
				}
			}

			OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);
			return true;
		}
		case IDI_TIMER_ALL_USER_READY: {
			for (int index = 0; index < NN_GAME_PLAYER; ++index) {
				if (m_PlayerStatus[index] == NNPlayerStatus_Sitting) {
					auto pUser = m_pITableFrame->GetTableUserItem(index);
					if (pUser != nullptr) {
						pUser->SetUserStatus(US_READY, m_pITableFrame->GetTableID(), index);
					}
					m_PlayerStatus[index] = NNPlayerStatus_Ready;
					m_pITableFrame->SetGameTimer(IDI_TIMER_CHECK_USER_STATUS, TIME_CHECK_USER_STATUS, 1, NULL);
				}
			}
		}
	}
	return false;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID* pDataBuffer, WORD wDataSize, IServerUserItem* pIServerUserItem) {
    switch (wSubCmdID) {
        case SUB_C_HOST_ALLOW_START: {
            if (pIServerUserItem->GetChairID() == _MasterChairID && m_GameStatus == NNGameStatus_Free) {
                m_GameStatus = NNGameStatus_HostConfirm;
                m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_HOST_CONFIRM_START);
				//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_HOST_CONFIRM_START);

                BOOL canStartGame = true;
                int playerCount = 0;

                for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                    if (m_PlayerStatus[index] == NNPlayerStatus_Sitting) {
                        canStartGame = FALSE;
                        break;
                    }

                    if (m_PlayerStatus[index] == NNPlayerStatus_Ready) {
                        ++playerCount;
                    }
                }

                if (canStartGame && playerCount >= 2) {
                    m_GameStatus = NNGameStatus_Start;
                    m_pITableFrame->StartGame();
                }

                return true;
            } else {
                return false;
            }
        }
        break;

        case SUB_C_USER_CALL: {
            if (wDataSize != sizeof(CMD_C_UserCall)) {
                return false;
            }

            if (pIServerUserItem->GetChairID() >= NN_GAME_PLAYER) {
                return false;
            }

            if (pIServerUserItem->GetChairID() == m_BankerChairID) {
                //m_pITableFrame->SendGameMessage(TEXT("�����ׯ�Ҳ�����ע"),SMT_EJECT|SMT_CHAT);
                return false;
            }

            if (m_PlayerStatus[pIServerUserItem->GetChairID()] != NNPlayerStatus_Playing) {
                //m_pITableFrame->SendGameMessage(TEXT("���û�вμӵ�ǰ��Ϸ"),SMT_EJECT|SMT_CHAT);
                return false;
            }

            if (m_GameStatus != NNGameStatus_Call) {
                return true;//�����м���ע״̬ ������ע
            }

            if (m_PlayerBets[pIServerUserItem->GetChairID()].wBet != 0) {
                return true;//�м��Ѿ���ע �����ظ���ע
            }
            CMD_C_UserCall* pInfo = (CMD_C_UserCall*)pDataBuffer;
			WORD chairID = pIServerUserItem->GetChairID();

			//��֤��ע
			bool isBetValided = false;
			for (int index = 0; index < MAX_STAKE_COUNT; ++index) {
				if (m_PlayerAllBets[chairID][index] == pInfo->playerBets && pInfo->playerBets.wBet != 0) {
					isBetValided = true;
					break;
				}
			}

			if (!isBetValided) {
				return false;
			}
            //m_pITableFrame->SendGameMessage(TEXT("�����ׯ�Ҳ�����ע"),SMT_EJECT|SMT_CHAT|SMT_PROMPT);
//         if(m_PlayerAllBets[0] != pInfo->playerBets && m_PlayerAllBets[1] != pInfo->playerBets) {
//             return false;
//         }

            m_PlayerBets[pIServerUserItem->GetChairID()] = pInfo->playerBets;

			//����Ƿ���ע
			if (pInfo->playerBets.wBetType == NNGBT_TuiZhu) {
				m_PlayerSingleResultRecord[pIServerUserItem->GetChairID()].bTuiZhu = true;
			} else {
				m_PlayerSingleResultRecord[pIServerUserItem->GetChairID()].bTuiZhu = false;
			}

            CMD_S_UserCall userCall;
            ZeroMemory(&userCall, sizeof(userCall));
            userCall.chairID = pIServerUserItem->GetChairID();
            userCall.playerBets = pInfo->playerBets;
            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_USER_CALL, &userCall, sizeof(userCall));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_USER_CALL, &userCall, sizeof(userCall));
            addGameOperator(userCall);

            BOOL canSendCard = TRUE;

            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (index != m_BankerChairID && m_PlayerStatus[index] == NNPlayerStatus_Playing && m_PlayerBets[index].wBet == 0) {
                    canSendCard = FALSE;
                }
            }

            if (!canSendCard) {
                return true;
            }

            switch (m_GameTypeIdex) {
                case NNGameType_HostBanker:
                case NNGameType_NNBanker: {
                    rationCardForUser(MAX_HAND_CARD);
                    m_pITableFrame->KillGameTimer(IDI_TIMER_USER_CALL);
                    return true;
                }

                case NNGameType_SnatchBankerShowCard:
                case NNGameType_NNRatio:
                case NNGameType_HLN: {
                    rationCardForUser_Add();
                    m_pITableFrame->KillGameTimer(IDI_TIMER_USER_CALL);
                    return true;
                }

                case NNGameType_SnatchBanker: {
                    m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SPLIT_CARD);
					//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SPLIT_CARD);
                    m_GameStatus = NNGameStatus_SplitCard;
                    m_pITableFrame->KillGameTimer(IDI_TIMER_USER_SPLIT_CARD);
                    m_pITableFrame->SetGameTimer(IDI_TIMER_USER_SPLIT_CARD, TIME_USER_SPLIT_CARD, 1, NULL);
                    return true;
                }

                default:
                    break;
            }

            return true;
        }
        break;

        case SUB_C_USER_SHOW_CARD: {
            if (wDataSize != sizeof(CMD_C_UserShowCard)) {
                return false;
            }

            WORD chairID = pIServerUserItem->GetChairID();

            if (chairID >= NN_GAME_PLAYER) {
                return false;
            }

            if (m_PlayerCardTypes[chairID].type != NNCardType_Invalid) {
                return true;
            }
            if (m_PlayerStatus[chairID] != NNPlayerStatus_Playing) {
#ifdef _DEBUG
                printf("���Ǳ�����Ϸ���");
#endif
                return true;
            }

            NNCardType_Result result;
            if (m_UseCheatingCards) {
                result = m_GameLogic.checkNNType(m_PlayerCardsCheating[chairID], m_GameRuleIdex);
            } else {
                result = m_GameLogic.checkNNType(m_PlayerCards[chairID], m_GameRuleIdex);
            }

            if (result.type < NNCardType_WHN && result.type != NNCardType_None) {
                CMD_C_UserShowCard* pShowCard = (CMD_C_UserShowCard*)pDataBuffer;
                int selectCount = 0;
                int sum = 0;

                for (int index = 0; index < MAX_HAND_CARD; index++) {
                    if (pShowCard->result.isCardSelected[index]) {
                        selectCount++;
                        int cardValue;
                        if (m_UseCheatingCards) {
                            cardValue = m_PlayerCardsCheating[chairID][index] & MASK_VALUE;
                        } else {
                            cardValue = m_PlayerCards[chairID][index] & MASK_VALUE;
                        }
                        cardValue = cardValue > 0xa ? 0xa : cardValue;
                        sum += cardValue;
                    }
                }

                if (selectCount == 3 && sum % 10 == 0) {
                    memcpy(result.isCardSelected, pShowCard->result.isCardSelected, sizeof(result.isCardSelected));
                }
            }

            m_PlayerCardTypes[chairID] = result;

            CMD_S_UserShowCard userShowCard;
            ZeroMemory(&userShowCard, sizeof(userShowCard));
            userShowCard.chairID = chairID;
            userShowCard.result = result;
            if (m_UseCheatingCards) {
                memcpy(userShowCard.playerCards, m_PlayerCardsCheating[chairID], sizeof(userShowCard.playerCards));
            } else {
                memcpy(userShowCard.playerCards, m_PlayerCards[chairID], sizeof(userShowCard.playerCards));
            }
            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_USER_SHOW_CARD, &userShowCard, sizeof(userShowCard));
            addGameOperator(userShowCard);
            bool canCalculate = true;

            for (int index = 0; index < NN_GAME_PLAYER; index++) {
                if (m_PlayerStatus[index] == NNPlayerStatus_Playing && m_PlayerCardTypes[index].type == NNCardType_Invalid) {
                    canCalculate = false;
                    break;
                }
            }

            if (canCalculate) {
                m_pITableFrame->KillGameTimer(IDI_TIMER_USER_SPLIT_CARD);
                calculate();
            }

            return true;
        }
        break;

		case SUB_C_USER_SNATCH_BANKER: {
			if (wDataSize != sizeof(CMD_C_UserSnatchBanker)) {
				return false;
			}

            if (pIServerUserItem->GetChairID() >= NN_GAME_PLAYER) {
#ifdef _DEBUG
                printf("������Ӻų������ֵ\n");
#endif
                return false;
            }

            if (m_PlayerStatus[pIServerUserItem->GetChairID()] != NNPlayerStatus_Playing) {
#ifdef _DEBUG
                printf("��Ҳ�����Ϸ״̬\n");
#endif
                return false;
            }

            if (m_PlayerBankerRatio[pIServerUserItem->GetChairID()] != -1) {
#ifdef _DEBUG
                printf("����Ѿ���ׯ��ע1\n");
#endif
                return true;
            }

            if (m_PlayerBankerRatio[pIServerUserItem->GetChairID()] >= 0) {
#ifdef _DEBUG
                printf("����Ѿ���ׯ��ע2\n");
#endif
                return true;//����Ѿ���ע�����ظ���ע
            }

            if (m_GameStatus != NNGameStatus_SnatchBanker) {
#ifdef _DEBUG
                printf("��Ϸ������ׯ״̬\n");
#endif
                return true;//������ע״̬������ע
            }

            CMD_C_UserSnatchBanker* pInfo = (CMD_C_UserSnatchBanker*)pDataBuffer;

            if (pInfo->ratio > m_MaxRatio) {
#ifdef _DEBUG
                printf("��ׯ���������������ע����\n");
#endif
                return false;
            }

            m_PlayerBankerRatio[pIServerUserItem->GetChairID()] = pInfo->ratio;

            CMD_S_UserSnatchBanker snatchBanker;
            ZeroMemory(&snatchBanker, sizeof(snatchBanker));
            snatchBanker.userChairID = pIServerUserItem->GetChairID();
            snatchBanker.ratio = pInfo->ratio;
            m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_USER_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
			//m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_USER_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
            addGameOperator(snatchBanker);
            bool canConfirmBanker = true;

            for (int index = 0; index < NN_GAME_PLAYER; ++index) {
                if (m_PlayerStatus[index] != NNPlayerStatus_Playing) {
                    continue;
                }

                if (m_PlayerBankerRatio[index] < 0) {
                    canConfirmBanker = false;
                    break;
                }
            }

            if (canConfirmBanker) {
                confirmSnatchBanker();
                m_pITableFrame->KillGameTimer(IDI_TIMER_SNATCH_BANKER);
            }

            return true;
        }
        break;

        case SUB_C_USER_DROP_BANKER: {
            if (pIServerUserItem->GetChairID() != _MasterChairID) {
                return false;
            }

            OnEventGameConclude(INVALID_CHAIR, NULL, GER_MASTER_END);
            return true;
        }
        break;
    }

    return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID* pDataBuffer, WORD wDataSize, IServerUserItem* pIServerUserItem) {
    return false;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem* pIServerUserItem) {
    WORD chairID = pIServerUserItem->GetChairID();
    if (chairID > NN_GAME_PLAYER) {
        return true;
    }
    if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY) {

    } else {
        if (pIServerUserItem->GetChairID() < NN_GAME_PLAYER) {
            m_PlayerStatus[pIServerUserItem->GetChairID()] = NNPlayerStatus_Sitting;
        }
    }

    return true;
}

//�û�����
bool CTableFrameSink::OnActionUserConnect(ITableFrame *pITableFrame, WORD wChairID, IServerUserItem* pIServerUserItem) {
    WORD chairID = pIServerUserItem->GetChairID();
    if (chairID > NN_GAME_PLAYER) {
        return true;
    }
    if (m_pITableFrame->GetGameStatus() >= GAME_STATUS_PLAY) {

    } else {
        if (pIServerUserItem->GetUserStatus() == US_READY) {
            m_PlayerStatus[pIServerUserItem->GetChairID()] = NNPlayerStatus_Ready;
            m_pITableFrame->KillGameTimer(IDI_TIMER_CHECK_USER_STATUS);
            m_pITableFrame->SetGameTimer(IDI_TIMER_CHECK_USER_STATUS, TIME_CHECK_USER_STATUS, 1, NULL);
        }
    }

    return true;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem* pIServerUserItem, bool bLookonUser) {
    if (wChairID >= 0 && wChairID < NN_GAME_PLAYER && !bLookonUser) {
        m_PlayerStatus[wChairID] = NNPlayerStatus_Sitting;
    }

    return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem* pIServerUserItem, bool bLookonUser) {
    //ׯ������
    if (wChairID >= 0 && wChairID < NN_GAME_PLAYER && bLookonUser == false) {
        m_PlayerStatus[wChairID] = NNPlayerStatus_Invalid;
        m_pITableFrame->KillGameTimer(IDI_TIMER_CHECK_USER_STATUS);
        m_pITableFrame->SetGameTimer(IDI_TIMER_CHECK_USER_STATUS, TIME_CHECK_USER_STATUS, 1, NULL);
    }

    return true;
}

bool CTableFrameSink::OnActionUserOnReady(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	if (wChairID < NN_GAME_PLAYER && m_GameStatus < NNGameStatus_Start) {
		m_PlayerStatus[wChairID] = NNPlayerStatus_Ready;
		m_pITableFrame->KillGameTimer(IDI_TIMER_CHECK_USER_STATUS);
		m_pITableFrame->SetGameTimer(IDI_TIMER_CHECK_USER_STATUS, TIME_CHECK_USER_STATUS, 1, NULL);
	}

	return true;
}

void CTableFrameSink::SetPrivateInfo(BYTE bGameTypeIdex, DWORD bGameRuleIdex, VOID* pData) {
    m_GameTypeIdex = bGameTypeIdex;
    m_GameRuleIdex = bGameRuleIdex;

	struct PrivateInfo {
		DWORD dwBaseScore;
		DWORD dwEnterScore;
		DWORD dwOutScore;
		DWORD PlayCount;
	};

	auto pPri = static_cast<PrivateInfo*>(pData);
	_dwTotalPlayRound = pPri->PlayCount;

	ZeroMemory(m_PlayerSingleResultRecord, sizeof(m_PlayerSingleResultRecord));
    for (int index = 0; index < NN_GAME_PLAYER; ++index) {
        m_PlayerStatus[index] = NNPlayerStatus_Invalid;
    }

    m_GameStatus = NNGameStatus_Free;
    m_BankerChairID = INVALID_CHAIR;
	m_LastBankerChairID = INVALID_CHAIR;
    m_MaxRatio = SnatchRatio_0;

    if (hasRule(NNGameRule_SnatchRatio_1)) {
		m_MaxRatio = SnatchRatio_1;
    } else if (hasRule(NNGameRule_SnatchRatio_2)) {
        m_MaxRatio = SnatchRatio_2;
    } else if (hasRule(NNGameRule_SnatchRatio_3)) {
        m_MaxRatio = SnatchRatio_3;
	} 

	if (hasRule(NNGameRule_TZRatio_0)) {
		_wCurTuiZhuRatio = TuiZhuBeiShu_0;
	}else	if (hasRule(NNGameRule_TZRatio_1)) {
		_wCurTuiZhuRatio = TuiZhuBeiShu_1;
	} else if (hasRule(NNGameRule_TZRatio_2)) {
		_wCurTuiZhuRatio = TuiZhuBeiShu_2;
	} else if (hasRule(NNGameRule_TZRatio_3)) {
		_wCurTuiZhuRatio = TuiZhuBeiShu_3;
	}

    if (m_GameTypeIdex == NNGameType_HostBanker) {
        m_BankerScore = 0;
    }
    m_dwCellScore = pPri->dwBaseScore;
	if (m_dwCellScore < 1) {
		m_dwCellScore = 1;
	}
}

bool CTableFrameSink::hasRule(BYTE rule) {
	return FvMask::HasAny(m_GameRuleIdex, _MASK_(rule));
}

//////////////////////////////////////////////////////////////////////////

void CTableFrameSink::SetCreateUserID(DWORD dwUserID) {
    _dwCreateUserID = dwUserID;
	//DONE:����ҪĬ�Ϸ���
	//_MasterUserID = dwUserID;
}

void CTableFrameSink::SetCreateUser(IServerUserItem * pIServerUserItem)
{
	_pCreateUser = pIServerUserItem;
	if (_pCreateUser) {
		_dwCreateUserID = pIServerUserItem->GetUserID();
		//DONE:����ҪĬ�Ϸ���
		//_MasterUserID = pIServerUserItem->GetUserID();
		//_MasterChairID = pIServerUserItem->GetChairID();
	} else {
		_dwCreateUserID = 0;
	}
}

IServerUserItem* CTableFrameSink::GetCreateUser()
{
	return _pCreateUser;
}

DWORD CTableFrameSink::GetCreateUserID()
{
	return _dwCreateUserID;
}

bool CTableFrameSink::SetMasterUserID(DWORD dwUserID)
{
	_MasterUserID = dwUserID;
	return true;
}

bool CTableFrameSink::SetMasterUser(IServerUserItem * pIServerUserItem)
{
	_pMasterUser = pIServerUserItem;
	if (_pMasterUser) {
		_MasterUserID = _pMasterUser->GetUserID();
		_MasterChairID = _pMasterUser->GetChairID();
	} else {
		_MasterUserID = 0;
		_MasterChairID = 0;
	}
	return true;
}

DWORD CTableFrameSink::GetMasterUserID()
{
	return _MasterUserID;
}

IServerUserItem * CTableFrameSink::GetMasterUser()
{
	return _pMasterUser;
}

bool CTableFrameSink::SwitchMaster(IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem) return false;

	//������Ƿ���
	if (_MasterUserID != pIServerUserItem->GetUserID())
		return false;
	for (int idx = 0; idx < m_pITableFrame->GetChairCount(); idx++)
	{
		IServerUserItem* pServerItemNext = m_pITableFrame->GetTableUserItem(idx);
		if (pServerItemNext == nullptr) continue;
		//������������, ���Ҳ��Ƿ���, ������Ϊ����
		if (pServerItemNext->GetUserID() != 0 && pServerItemNext->GetUserID() != _MasterUserID)
		{
			SetMasterUser(pServerItemNext);
			m_pITableFrame->SendRoomMessage(pServerItemNext, TEXT("���Ѿ���Ϊ������"), SMT_EJECT);
			break;
		}
	}

	return true;
}
