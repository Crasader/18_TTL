#ifndef PRIVATE_HEAD_FILE
#define PRIVATE_HEAD_FILE

#pragma once

#include <queue>

//引入文件
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"
#include "PrivateTableInfo.h"

///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////


struct DBR_GR_Create_Private;

//定时赛
class PriaveteGame 
	:public IGamePrivateItem
	,public IPrivateEventSink
	,public IServerUserItemSink
{
	//比赛配置
protected:
	tagGameServiceOption *				m_pGameServiceOption;//服务配置
	tagGameServiceAttrib *				m_pGameServiceAttrib;//服务属性

	CMD_GR_Private_Info					m_kPrivateInfo;

#if defined(ALLOWED_KICK_UNREADY_USER)
    //时间控制
protected:
    std::queue<DWORD>                  m_timerIDs;//定时器的ID, 用于检测用户是否准备
    DWORD                                       m_checkReadyTime;//每个定时器的定时时间
    DWORD                                       m_minTimeFrame;//每个定时器的最小时间间隔
#endif

	//内核接口
protected:
	PrivateTableInfo* m_pTableInfo;//
	ITimerEngine* m_pITimerEngine;//时间引擎
	IDBCorrespondManager*	 m_pIDataBaseEngine;//数据引擎	
	ITCPNetworkEngineEvent* m_pITCPNetworkEngineEvent;	//网络引擎

	//服务接口
protected:
	IMainServiceFrame*	 m_pIGameServiceFrame;//功能接口
	IServerUserManager* m_pIServerUserManager;//用户管理
	IAndroidUserManager* m_pAndroidUserManager;//机器管理
	IServerUserItemSink* m_pIServerUserItemSink;//用户回调

	//函数定义
public:
	//构造函数
	PriaveteGame();
	//析构函数
	virtual ~PriaveteGame(void);

public:
	bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	bool SendTableData(ITableFrame*	pITableFrame, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

public:
	void CreatePrivateCost(PrivateTableInfo* pTableInfo);
	bool JoinPrivateRoom(IServerUserItem * pIServerUserItem,ITableFrame * pICurrTableFrame, PrivateTableInfo* pTableInfo = NULL);
	void SendRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo* pTableInfo);
	bool SendRoomList(IServerUserItem * pIServerUserItem, DWORD dwExcludeNumber = -1);

	PrivateTableInfo* getTableInfoByRoomID(DWORD dwRoomID);
	PrivateTableInfo* getTableInfoByCreaterID(DWORD dwUserID);
    PrivateTableInfo* getTableInfoByUserID(DWORD dwUserID);
	PrivateTableInfo* getTableInfoByTableID(DWORD dwRoomID);
	PrivateTableInfo* getTableInfoByTableFrame(ITableFrame* pTableFrame);
	void DismissRoom(PrivateTableInfo* pTableInfo);
	void ClearRoom(PrivateTableInfo* pTableInfo);

	//基础接口
public:
 	//释放对象
 	virtual VOID Release(){ delete this; }
 	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//控制接口
public:
	//启动通知
	virtual void OnStartService();

	//管理接口
public:
	//绑定桌子
	virtual bool BindTableFrame(ITableFrame * pTableFrame,WORD wTableID);
	//初始化接口
	virtual bool InitPrivateInterface(tagPrivateManagerParameter & MatchManagerParameter);	
	//系统事件
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//网络事件
public:
	//
	bool OnDBORefreshTable(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize, std::string kChannel);
	bool OnDBOCreatePrivate(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize, std::string kChannel);
	void SendDBOCreatePrivate(DBR_GR_Create_Private* kInfo, DWORD dwSocketID, IServerUserItem* pIServerUserItem, std::string kHttpChannel);

public:
	//支付服务更新玩家金币
	bool OnPaymentSubRequestRefreshTable(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//支付服务更新玩家金币
	bool OnPaymentSubRequestRequestWeiXinOrderRet(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);

public:
	//私人场消息
	bool OnTCPPrivateGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);	
	//创建私人场
	bool OnTCPNetworkSubCreatePrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//重新加入私人场
	bool OnTCPNetworkSubAgainEnter(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//加入私人场
	bool OnTCPNetworkSubJoinPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//解散
	bool OnTCPNetworkSubDismissPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//查询房间列表信息
	bool OnTCPNetworkSubInqureTables(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);

	//用户接口
public:
	//用户积分
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//用户状态
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR);
	//用户权限
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight=true);	
	//私人场用户事件
	virtual bool AddPrivateAction(ITableFrame* pTbableFrame,DWORD dwChairID, BYTE	bActionIdex);

	//事件接口
public:
	//用户登录
	virtual bool OnEventUserLogon(IServerUserItem * pIServerUserItem) override;
	//用户登出
	virtual bool OnEventUserLogout(IServerUserItem * pIServerUserItem) override;
	//进入事件
	virtual bool OnEventEnterPrivate(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile) override;
	//用户参赛
	virtual bool OnEventUserJoinPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID) override;
	//用户退赛
	virtual bool OnEventUserQuitPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD) override;

	 //功能函数
public:
	 //游戏开始
	 virtual bool OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount) override;
	 //游戏结束
	 virtual bool OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) override;
	 //用户进去游戏
	 virtual bool OnEventClientReady(ITableFrame *pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem) override;

	 //用户事件
public:
	 //用户断线
	 virtual bool OnActionUserOffLine(ITableFrame* pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem);
	 //用户坐下
	 virtual bool OnActionUserSitDown(ITableFrame* pITableFrame, WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //用户起来
	 virtual bool OnActionUserStandUp(ITableFrame* pITableFrame, WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //用户同意
	 virtual bool OnActionUserOnReady(ITableFrame* pITableFrame, WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	
public:
	 //用户起来
	virtual bool OnEventReqStandUP(IServerUserItem * pIServerUserItem);
	//写分
	virtual bool WriteTableScore(ITableFrame* pITableFrame,tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& kData);	
	//辅助函数
protected:
};

#endif
