#pragma once

#include "cocos2d.h"
#include "../GameLib.h"

#include "Kernel/kernel/server/IServerItemSink.h"
#include "Platform/PFDefine/data/ServerListData.h"
#include "kernel/kernel/server/CServerItem.h"
#include "Kernel/kernel/game/IClientKernelSink.h"
#include "Platform/PFKernel/CGPLoginMission.h"

enum ConectState
{
	CS_NULL = 0,
	CS_REQUEST_PRIVATE_INFO = 1,
	CS_UPDATE_SERVERLIST = 2,
	CS_REQUEST_PRIVATE_TABLE = 3,
};

#define pGameMan GameManagerBase::pGMInstance()
#define pSceneMan ScenceManagerBase::pScenceManager()

class GameManagerBase
	:public IServerItemSink
	,public IServerListDataSink
	,public IStringMessageSink
	,public cocos2d::Ref
	,public TimeNode
{
protected:
	GameManagerBase();
private:
	static GameManagerBase* g_GameMan;
public:
	~GameManagerBase();
	//////////////////////////////////////////////////////////////////////////
	virtual void setInstance(GameManagerBase* instance) {
		assert(instance != nullptr);
		g_GameMan = instance;
	}
	static GameManagerBase* pGMInstance() {
		assert(g_GameMan);
		return g_GameMan;
	};
public:
	//virtual void applicationDidEnterBackground();
	//virtual void applicationWillEnterForeground();
	virtual void OnGameEnd(cocos2d::Node* pNode, float fWaiteTime) {}

public:
	//是否是私人场
	bool IsPrivateGame();
	//获取当前serverid
	int getActServerID();
	//连接到游戏类型
	int connectGameServerByKindID(word wKindID,word wServerType = INVALID_WORD);
	//连接到游戏id
	int connectGameServerByServerID(int iServerID);
	//指定连接到服务器
	int connectGameServer(CGameServerItem* pGameServerItem);
	//设置当前连接之后的要做的状态
	void setConectToDoStatus(ConectState estate);
	//请求私人场信息
	void sendSubGRProvateInfo(float fTime);
	//请求桌子信息
	void sendPrivateTableInfo(float fTime);

private:
//私人场数据////////////////////////////////////////////////////////////////////////
	//获取游戏服务器信息

	float flush_private_info_time;
	float flush_server_list_time;
	float flush_table_time;

	//登陆信息
public:
	//坐下失败
	virtual void onGRRequestFailure(const std::string& sDescribeString);
	//登陆成功
	virtual void OnGRLogonSuccess(void* data, int dataSize);
	//登陆失败
	virtual void OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString);
	//登陆完成
	virtual void OnGRLogonFinish();
	//更新通知
	virtual void OnGRUpdateNotify(byte cbMustUpdate, const std::string& sDescribeString);
	//游戏登录完成
	virtual void CB_GameLogonFinsh();

	//配置信息
public:
	//列表配置
	virtual void OnGRConfigColumn();
	//房间配置
	virtual void OnGRConfigServer();
	//道具配置
	virtual void OnGRConfigProperty();
	//玩家权限配置
	virtual void OnGRConfigUserRight();
	//配置完成
	virtual void OnGRConfigFinish();

	//用户信息
public:
	//用户进入
	virtual void OnGRUserEnter(IClientUserItem* pIClientUserItem);
	//用户更新
	virtual void OnGRUserUpdate(IClientUserItem* pIClientUserItem);
	//用户删除
	virtual void OnGRUserDelete(IClientUserItem* pIClientUserItem);

	//框架消息
public:
	//用户邀请
	virtual void OnGFUserInvite(const std::string& szMessage);
	//用户邀请失败
	virtual void OnGFUserInviteFailure(const std::string& szMessage);
	//房间退出
	virtual void OnGFServerClose(const std::string& szMessage);
	//创建游戏内核
	virtual bool CreateKernel();
	//启动游戏
	virtual bool StartGame();
	//设置私人场接口
	virtual void setPrivateServerSink(IServerPrivateSink* PrivateServerSink);
public:
	//完成通知
	virtual void OnGameItemFinish();
	//完成通知
	virtual void OnGameKindFinish(uint16 wKindID);
	//更新通知
	virtual void OnGameItemUpdateFinish();

	//更新通知
public:
	//插入通知
	virtual void OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual void OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual void OnGameItemDelete(CGameListItem * pGameListItem);

public:

	virtual IClientKernelSink* CreateGame() = 0;
	virtual void loadGameBaseData(word wKindID) = 0;

	void setCurGameKindID(word wKindID) {
		_wCurGameID = wKindID;
	}
	word getCurGameKindID() {
		return _wCurGameID;
	}
	void setInCenter(bool inCenter) {
		_bInCenter = inCenter;
	}
	bool getInCenter() {
		return _bInCenter;
	}

public:
	///< 链接服务器
	void connectServer();
	///< 断开连接
	void disconnectServer();	//事件消息

//IStringMessageSink////////////////////////////////////////////////////////////////////////
public:
	//进入事件
	virtual bool InsertUserEnter(const char* pszUserName);
	//离开事件
	virtual bool InsertUserLeave(const char* pszUserName);
	//断线事件
	virtual bool InsertUserOffLine(const char* pszUserName);

	//普通消息(窗口输出)
	virtual bool InsertNormalString(const char* pszString);
	//系统消息(窗口输出)
	virtual bool InsertSystemString(const char* pszString);
	//系统消息(窗口输出)
	virtual bool InsertSystemStringScript(const char* pszString);
	//提示消息(对话框方式??)0:确认 1:确认,取消
	virtual int InsertPromptString(const char* pszString, int iButtonType);

	virtual bool InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum);

//////////////////////////////////////////////////////////////////////////
	
	void closeClinet();
	void StartGameReconnect();
	virtual void onEventTCPSocketError(int errorCode);

//////////////////////////////////////////////////////////////////////////

	void upDataTime(float fTime);

//////////////////////////////////////////////////////////////////////////

//心跳包
#ifdef ENABLE_SEND_HEART_TICK_TIMER
public:
	virtual void sendHeartTick();
	virtual void HeartTick();
	void RestTick();
#endif
public:
	static void setJoinWithRoomNO(dword dwNnmber) {
		CCLOG("setJoinWithRoomNO dwNnmber = %d", dwNnmber);
		_room_number = dwNnmber;
	}
	static dword getJoinWithRoomNO() {
		return _room_number;
	}
	static void setConectedServer(bool bConected) {
		_is_conected_server = bConected;
	}
	static bool getConecetedServer() {
		return _is_conected_server;
	}
public:
	SCORE getGameDiFen();
	WORD  getGameChairNum();
	//请求更新桌子数据
	bool BackKey();
	//请求刷新桌子
	virtual bool RequreRefreshTableInfo();
	//微信分享返回结果
	void onWeiXinResult(int result);

//////////////////////////////////////////////////////////////////////////

protected:
	enReconnectStatus m_eInReconnect;
	CGameServerItem* mGameServerItem;
	CServerItem* mCServerItem;
	CGPLoginMission m_kLoginMission;
	IServerPrivateSink* _iPrivateServerSink;
	float m_fHeatTickTime;
	//用于设置连接服务器干什么事情的状态
	ConectState _conToDoStatus;
	//等待时间
	float _waitingTime;
	//////////////////////////////////////////////////////////////////////////
	//是否在大厅
	bool _bInCenter;
	//当前的游戏ID
	word _wCurGameID;

	//////////////////////////////////////////////////////////////////////////
	//玩家通过分享加入房间的房号
	static dword _room_number;
	static bool _is_conected_server;
	//////////////////////////////////////////////////////////////////////////
};

