#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
struct  NNGameRecordPlayer {
    DWORD dwUserID;
	WORD chairID;
    std::string kHead;
    std::string kNickName;
    void StreamValue(datastream& kData, bool bSend)
    {
        Stream_VALUE(dwUserID);
		Stream_VALUE(chairID);
        Stream_VALUE(kHead);
        Stream_VALUE(kNickName);
    }
};

struct  NNGameRecordOperateResult {
    enum Type {
        TYPE_NULL,
        TYPE_Start,
		TYPE_SnatchBanker,
		TYPE_UserSnatchBanker,
        TYPE_UserCall,
        TYPE_SendCardAll,
		TYPE_SendCardAdd,
        TYPE_UserSplitCard,
        TYPE_CompareCard,
    };

    NNGameRecordOperateResult()
    {
        actionType = TYPE_NULL;
        bankerChairID = INVALID_CHAIR;
		bankerRatio = 0;
        operatorChairID = INVALID_CHAIR;
        userBetsRatio = 0;
		ZeroMemory(&cardType, sizeof(cardType));
		cardCount = 0;
        ZeroMemory(playerCards, sizeof(playerCards));
		ZeroMemory(playerCardsAdd, sizeof(playerCardsAdd));
		ZeroMemory(playerScores, sizeof(playerScores));
    }

    BYTE	 actionType;
    WORD bankerChairID;						//ׯ��
	BYTE	 bankerRatio;							//��ׯ����
    WORD operatorChairID;						//�����û�
    BYTE	 userBetsRatio;				//��ע����
	NNCardType_Result	cardType;						//���ƽ��
	BYTE	 cardCount;
    BYTE	 playerCards[NN_GAME_PLAYER][MAX_HAND_CARD];
	BYTE	 playerCardsAdd[NN_GAME_PLAYER];
    int playerScores[NN_GAME_PLAYER];

    void StreamValue(datastream& kData, bool bSend)
    {
        Stream_VALUE(actionType);
        Stream_VALUE(bankerChairID);
		Stream_VALUE(bankerRatio);
        Stream_VALUE(operatorChairID);
        Stream_VALUE(userBetsRatio);
		Stream_VALUE(cardCount);
		Stream_VALUE_SIZE(&cardType, sizeof(cardType));
        Stream_VALUE_SIZE(playerCards, sizeof(playerCards));
		Stream_VALUE_SIZE(playerCardsAdd, sizeof(playerCardsAdd));
        Stream_VALUE_SIZE(playerScores, sizeof(playerScores));
    }
};

struct  NNGameRecord {
    DWORD dwKindID;
    DWORD dwVersion;
	DWORD gameTypeIndex;
	DWORD gameRuleIndex;
	DWORD hostUser;
    std::vector<NNGameRecordPlayer> kPlayers;
    std::vector<NNGameRecordOperateResult> kAction;
    void StreamValue(datastream& kData, bool bSend)
    {
        StructVecotrMember(NNGameRecordPlayer, kPlayers);
        StructVecotrMember(NNGameRecordOperateResult, kAction);
        Stream_VALUE(dwKindID);
        Stream_VALUE(dwVersion);
		Stream_VALUE(gameTypeIndex);
		Stream_VALUE(gameRuleIndex);
		Stream_VALUE(hostUser);
    }
};

//////////////////////////////////////////////////////////////////////////
//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
public:
    BYTE	 m_GameTypeIdex;//��Ϸ����
    DWORD m_GameRuleIdex;//��Ϸ����
	WORD _wCurTuiZhuRatio;//��ע����

protected:
    NNGameRecord m_GameRecord;
    CGameLogic m_GameLogic;//��Ϸ�߼�
    ITableFrame* m_pITableFrame;//��ܽӿ�
    const tagGameServiceOption* m_pGameServiceOption;//���ò���

protected:
    NNGameStatus m_GameStatus;// ��Ϸ״̬
    WORD m_BankerChairID;
    DWORD _dwCreateUserID;
	WORD m_LastBankerChairID;
    WORD _MasterChairID;//����
	DWORD _MasterUserID;//����

	IServerUserItem* _pMasterUser;
	IServerUserItem* _pCreateUser;

    BYTE	 m_MaxRatio;
    BYTE	 m_BankerRatio;
    int m_BankerScore;
    short m_PlayerBankerRatio[NN_GAME_PLAYER];

	NNPlayerBet m_PlayerAllBets[NN_GAME_PLAYER][MAX_STAKE_COUNT];
	NNPlayerBet m_PlayerBets[NN_GAME_PLAYER];

    BYTE	 m_GameCards[MAX_CARD_COUNT];//�˿�
    BYTE	 m_CardLeftCount;//�˿�ʣ������
    NNPlayerStatus m_PlayerStatus[NN_GAME_PLAYER];//���״̬

	bool m_UseCheatingCards;
	BYTE m_PlayerCardsCheating[NN_GAME_PLAYER][MAX_HAND_CARD];//��������
	BYTE m_PlayerCards[NN_GAME_PLAYER][MAX_HAND_CARD];//�������
    BYTE m_PlayerCardsAdd[NN_GAME_PLAYER];//��Ҳ���
    std::string	m_PlayerCardsName[NN_GAME_PLAYER][MAX_HAND_CARD];//�����������
    NNCardType_Result m_PlayerCardTypes[NN_GAME_PLAYER];
    int m_PlayerSingleResult[NN_GAME_PLAYER];
	NNPlayerCardResult m_PlayerSingleResultRecord[NN_GAME_PLAYER];//��һ����ҵ��Ƽ�¼
	CMD_S_CalculateTotal _calculate_total[NN_GAME_PLAYER];//�ƾ�ͳ��

private:

	DWORD _dwCurrentPlayRound;//��ǰ�ľ���
	DWORD _dwTotalPlayRound;//�ܵľ���

	DWORD m_dwCellScore;
	DWORD m_dwGoOutScoce;	//��������
	DWORD m_dwEnterMatchNum;//�������

public:
    //���캯��
    CTableFrameSink();
    //��������
    virtual ~CTableFrameSink();

    //�����ӿ�
public:
    //�ͷŶ���
    virtual VOID Release() { }
    //�ӿڲ�ѯ
    virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

    //����ӿ�
public:
    //��ʼ��
    virtual bool Initialization(IUnknownEx * pIUnknownEx);
    //��λ����
    virtual VOID RepositionSink();

    //��ѯ�ӿ�
public:
    //��ѯ�޶�
    virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)
    {
        return 0;
    };
    //���ٻ���
    virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
    {
        return 0;
    };
    //��ѯ�Ƿ�۷����
    virtual bool QueryBuckleServiceCharge(WORD wChairID)
    {
        return true;
    }

    //�����ӿ�
public:
    //���û���
    virtual void SetGameBaseScore(LONG lBaseScore) {};

    //��Ϸ�¼�
public:
    //��Ϸ��ʼ
    virtual bool OnEventGameStart();
    //��Ϸ����
    virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
    //���ͳ���
    virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//////////////////////////////////////////////////////////////////////////
    //ϴ��
    void Shuffle();
    //����
    void rationCardForUser(WORD cardCount);
    //����
    void rationCardForUser_Add();
    //��Ϸ��ʼ
    void startGame();
    //��ȡׯ��
    void getBanker();
    //��ׯʱ��ȡׯ��
    void confirmSnatchBanker();
	//����ܷ���ע
	int checkTuiZhuScore(WORD wChairID);
    //��ȡ��ע��Χ
    void getBets();
    //����
    void calculate();
	//�ܽ���
	void calculateTotal();
	//
	void callCheatingCards(WORD chairID, bool forWin = true);
	//////////////////////////////////////////////////////////////////////////
    //�¼��ӿ�
public:
    //��ʱ���¼�
    virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
    //�����¼�
    virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
    {
        return false;
    }
    //�����¼�
    virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
    {
        return false;
    }

    //����ӿ�
public:
    //��Ϸ��Ϣ����
    virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
    //�����Ϣ����
    virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

    //�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem) override;
	//�û�����
	virtual bool OnActionUserSitDown(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) override;
	//�û�����
	virtual bool OnActionUserStandUp(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) override;
	//�û�ͬ��
	virtual bool OnActionUserOnReady(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) override;
    //�û�����
    virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem);

    virtual void SetPrivateInfo(BYTE bGameTypeIdex, DWORD bGameRuleIdex, VOID* pData) override;

public:
	//////////////////////////////////////////////////////////////////////////
	//���ô�����
	virtual void SetCreateUserID(DWORD	dwUserID) override;
	virtual void SetCreateUser(IServerUserItem* pIServerUserItem) override;
	//��ȡ������
	virtual DWORD GetCreateUserID() override;
	virtual IServerUserItem* GetCreateUser() override;
	//���÷���
	virtual bool SetMasterUserID(DWORD	dwUserID) override;
	virtual bool SetMasterUser(IServerUserItem * pIServerUserItem) override;
	//��ȡ����
	virtual DWORD GetMasterUserID() override;
	virtual IServerUserItem* GetMasterUser() override;
	//�л�������
	virtual bool SwitchMaster(IServerUserItem * pIServerUserItem) override;
	//////////////////////////////////////////////////////////////////////////
    //��Ϸ�¼�
protected:
    bool hasRule(BYTE rule);
	bool isAdminUser(DWORD userID);
public:
	void startGameRecord();
	void addGameOperator(CMD_S_BankerInfo& oper);
	void addGameOperator(CMD_S_UserCall& oper);
	void addGameOperator(CMD_S_SnatchBanker& oper);
	void addGameOperator(CMD_S_UserSnatchBanker& oper);
	void addGameOperator(CMD_S_SendCardAll& oper);
	void addGameOperator(CMD_S_SendCardAdd& oper);
	void addGameOperator(CMD_S_UserShowCard& oper);
	void addGameOperator(CMD_S_Calculate& oper);
};

//////////////////////////////////////////////////////////////////////////

#endif
