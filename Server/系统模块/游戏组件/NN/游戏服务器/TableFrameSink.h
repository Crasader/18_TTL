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
    WORD bankerChairID;						//庄家
	BYTE	 bankerRatio;							//抢庄倍率
    WORD operatorChairID;						//操作用户
    BYTE	 userBetsRatio;				//下注倍率
	NNCardType_Result	cardType;						//分牌结果
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
//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
public:
    BYTE	 m_GameTypeIdex;//游戏类型
    DWORD m_GameRuleIdex;//游戏规则
	WORD _wCurTuiZhuRatio;//推注倍数

protected:
    NNGameRecord m_GameRecord;
    CGameLogic m_GameLogic;//游戏逻辑
    ITableFrame* m_pITableFrame;//框架接口
    const tagGameServiceOption* m_pGameServiceOption;//配置参数

protected:
    NNGameStatus m_GameStatus;// 游戏状态
    WORD m_BankerChairID;
    DWORD _dwCreateUserID;
	WORD m_LastBankerChairID;
    WORD _MasterChairID;//房主
	DWORD _MasterUserID;//房主

	IServerUserItem* _pMasterUser;
	IServerUserItem* _pCreateUser;

    BYTE	 m_MaxRatio;
    BYTE	 m_BankerRatio;
    int m_BankerScore;
    short m_PlayerBankerRatio[NN_GAME_PLAYER];

	NNPlayerBet m_PlayerAllBets[NN_GAME_PLAYER][MAX_STAKE_COUNT];
	NNPlayerBet m_PlayerBets[NN_GAME_PLAYER];

    BYTE	 m_GameCards[MAX_CARD_COUNT];//扑克
    BYTE	 m_CardLeftCount;//扑克剩余张数
    NNPlayerStatus m_PlayerStatus[NN_GAME_PLAYER];//玩家状态

	bool m_UseCheatingCards;
	BYTE m_PlayerCardsCheating[NN_GAME_PLAYER][MAX_HAND_CARD];//作弊手牌
	BYTE m_PlayerCards[NN_GAME_PLAYER][MAX_HAND_CARD];//玩家手牌
    BYTE m_PlayerCardsAdd[NN_GAME_PLAYER];//玩家补牌
    std::string	m_PlayerCardsName[NN_GAME_PLAYER][MAX_HAND_CARD];//玩家手牌名称
    NNCardType_Result m_PlayerCardTypes[NN_GAME_PLAYER];
    int m_PlayerSingleResult[NN_GAME_PLAYER];
	NNPlayerCardResult m_PlayerSingleResultRecord[NN_GAME_PLAYER];//上一把玩家的牌记录
	CMD_S_CalculateTotal _calculate_total[NN_GAME_PLAYER];//牌局统计

private:

	DWORD _dwCurrentPlayRound;//当前的局数
	DWORD _dwTotalPlayRound;//总的局数

	DWORD m_dwCellScore;
	DWORD m_dwGoOutScoce;	//滚蛋分数
	DWORD m_dwEnterMatchNum;//开搞分数

public:
    //构造函数
    CTableFrameSink();
    //析构函数
    virtual ~CTableFrameSink();

    //基础接口
public:
    //释放对象
    virtual VOID Release() { }
    //接口查询
    virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

    //管理接口
public:
    //初始化
    virtual bool Initialization(IUnknownEx * pIUnknownEx);
    //复位桌子
    virtual VOID RepositionSink();

    //查询接口
public:
    //查询限额
    virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)
    {
        return 0;
    };
    //最少积分
    virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
    {
        return 0;
    };
    //查询是否扣服务费
    virtual bool QueryBuckleServiceCharge(WORD wChairID)
    {
        return true;
    }

    //比赛接口
public:
    //设置基数
    virtual void SetGameBaseScore(LONG lBaseScore) {};

    //游戏事件
public:
    //游戏开始
    virtual bool OnEventGameStart();
    //游戏结束
    virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
    //发送场景
    virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//////////////////////////////////////////////////////////////////////////
    //洗牌
    void Shuffle();
    //配牌
    void rationCardForUser(WORD cardCount);
    //补牌
    void rationCardForUser_Add();
    //游戏开始
    void startGame();
    //获取庄家
    void getBanker();
    //抢庄时获取庄家
    void confirmSnatchBanker();
	//检查能否推注
	int checkTuiZhuScore(WORD wChairID);
    //获取下注范围
    void getBets();
    //结算
    void calculate();
	//总结算
	void calculateTotal();
	//
	void callCheatingCards(WORD chairID, bool forWin = true);
	//////////////////////////////////////////////////////////////////////////
    //事件接口
public:
    //定时器事件
    virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
    //数据事件
    virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
    {
        return false;
    }
    //积分事件
    virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
    {
        return false;
    }

    //网络接口
public:
    //游戏消息处理
    virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
    //框架消息处理
    virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

    //用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem) override;
	//用户坐下
	virtual bool OnActionUserSitDown(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) override;
	//用户起来
	virtual bool OnActionUserStandUp(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) override;
	//用户同意
	virtual bool OnActionUserOnReady(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) override;
    //用户重入
    virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem);

    virtual void SetPrivateInfo(BYTE bGameTypeIdex, DWORD bGameRuleIdex, VOID* pData) override;

public:
	//////////////////////////////////////////////////////////////////////////
	//设置创建者
	virtual void SetCreateUserID(DWORD	dwUserID) override;
	virtual void SetCreateUser(IServerUserItem* pIServerUserItem) override;
	//获取创建者
	virtual DWORD GetCreateUserID() override;
	virtual IServerUserItem* GetCreateUser() override;
	//设置房主
	virtual bool SetMasterUserID(DWORD	dwUserID) override;
	virtual bool SetMasterUser(IServerUserItem * pIServerUserItem) override;
	//获取房主
	virtual DWORD GetMasterUserID() override;
	virtual IServerUserItem* GetMasterUser() override;
	//切换创建者
	virtual bool SwitchMaster(IServerUserItem * pIServerUserItem) override;
	//////////////////////////////////////////////////////////////////////////
    //游戏事件
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
