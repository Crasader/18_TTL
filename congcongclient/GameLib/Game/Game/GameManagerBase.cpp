#include "cocos2d.h"
#include "GameManagerBase.h"

#include "NoticeMsg.h"
#include "NoticeMsgBox.h"
#include "UserInfo.h"
#include "Game/Script/utility.h"
#include "JniCross/JniFun.h"
#include "GameScriptNet.h"
#include "Game/Game/MissionWeiXin.h"

USING_NS_CC;
using namespace experimental;

GameManagerBase* GameManagerBase::g_GameMan = nullptr;

#define MaxReconnectTime  120.0f

//等待时间100毫秒
#define WAITING_FLUSH_PRIVATE_INFO	1000.0f
#define WAITING_FLUSH_SERVERLIST	1000.0f
#define WAITING_FLUSH_TABLE	1000.0f

dword GameManagerBase::_room_number = 0;

GameManagerBase::GameManagerBase()
	: mCServerItem(NULL)
	, m_fHeatTickTime(0)
	, mGameServerItem(NULL)
	, m_eInReconnect(ReconnectStatus_NULL)
	, m_kLoginMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
	, _wCurGameID(ScriptData<int>("GameKind").Value())
	, flush_private_info_time(WAITING_FLUSH_PRIVATE_INFO)
	, flush_server_list_time(WAITING_FLUSH_SERVERLIST)
	, flush_table_time(WAITING_FLUSH_TABLE)
	, _bInCenter(true)
	, _waitingTime(0.f)
{
	pSevList->SetServerListDataSink(this);
	mCServerItem = dynamic_cast<CServerItem *>(IServerItem::create());
	mCServerItem->SetServerItemSink(this);
	mCServerItem->SetStringMessageSink(this);
	//SocketTimer::Instance().addTimerSink(this);

	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(GameManagerBase::upDataTime,this),this,0.0f,false,"GameManagerBase");
	pInsGameScript->OnGetConfig();
}

GameManagerBase::~GameManagerBase()
{
	if (IServerItem::get())
	{
		mCServerItem->SetServerItemSink(0);
		mCServerItem->SetStringMessageSink(0);
		mCServerItem->SetStringMessageSink(0);
	}

	if (IServerItem::get())
	{
		IServerItem::get()->IntermitConnect(true);
	}

	IServerItem::destory();
	mCServerItem = 0;

	cocos2d::Director::getInstance()->getScheduler()->unschedule("GameManagerBase",this);
}

//void GameManagerBase::applicationWillEnterForeground()
//{
//	cocos2d::log("GameManagerBase::applicationWillEnterForeground");
//#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
//	if (IServerItem::get())
//	{
//		cocos2d::log("GameManagerBase::StartGameReconnect");
//		StartGameReconnect();
//	}
//#endif
//	pInsGameScript->OnGetConfig();
//}

//////////////////////////////////////////////////////////////////////////

bool GameManagerBase::IsPrivateGame()
{
	if (!mGameServerItem)
	{
		return false;
	}
	return mGameServerItem->IsPrivateRoom();
}

int GameManagerBase::getActServerID()
{
	if (mGameServerItem)
	{
		return mGameServerItem->m_GameServer.wServerID;
	}
	return -1;
}

int GameManagerBase::connectGameServerByKindID(word wKindID,word wServerType)
{
	CGameServerItem* pServer = pSevList->getGameServerByKind(wKindID,wServerType);
	if (pServer)
	{
		return connectGameServer(pServer);
	}
	else
	{
		setConectToDoStatus(CS_UPDATE_SERVERLIST);
	}
	return 0;
}

int GameManagerBase::connectGameServerByServerID(int iServerID)
{
	CGameServerItem* pServer = pSevList->SearchGameServer(iServerID);
	if (pServer)
	{
		return connectGameServer(pServer);
	}
	else
	{
		setConectToDoStatus(CS_UPDATE_SERVERLIST);
	}
	return 0;
}

int GameManagerBase::connectGameServer(CGameServerItem* pGameServerItem)
{
	utility::log("GameManagerBase::connectGameServer");

	//DONE:这里修改了断线重连,没有必要每次断开连接
	if (pGameServerItem != nullptr &&
		mGameServerItem != nullptr &&
		mGameServerItem->m_GameServer.wServerID == pGameServerItem->m_GameServer.wServerID) {
		utility::log("GameManagerBase::connectGameServer already conected");
		if (mCServerItem->IsService()) {
			return ServiceStatus_ServiceIng;
		}
	}

	if (mCServerItem->IsService()) {
		disconnectServer();
	}

	utility::log("GameManagerBase::connectGameServer connectServer");
	mGameServerItem = pGameServerItem;
	
	connectServer();
#ifdef ENABLE_SEND_HEART_TICK_TIMER
	RestTick();
#endif
	return 0;
}

void GameManagerBase::setConectToDoStatus(ConectState estate)
{
	_conToDoStatus = estate;
	flush_table_time = WAITING_FLUSH_TABLE;
}

void GameManagerBase::sendSubGRProvateInfo(float fTime)
{
	if(mCServerItem)
		mCServerItem->sendSubGRPrivateInfo();
}

void GameManagerBase::sendPrivateTableInfo(float fTime)
{
	if (mCServerItem)
		mCServerItem->sendPrivateTablesInfo();
}

//////////////////////////////////////////////////////////////////////////
// IServerListDataSink
//完成通知
void GameManagerBase::OnGameItemFinish()
{
	connectGameServerByKindID(_wCurGameID, GAME_GENRE_EDUCATE);
	if (IServerItem::get()) {
		IServerItem::get()->SetServerPrivateSink(_iPrivateServerSink);
	}
	//setConectToDoStatus(CS_REQUEST_PRIVATE_INFO);
	setConectToDoStatus(CS_REQUEST_PRIVATE_TABLE);
}

//完成通知
void GameManagerBase::OnGameKindFinish(uint16 wKindID)
{
}

//更新通知
void GameManagerBase::OnGameItemUpdateFinish()
{
}

//插入通知
void GameManagerBase::OnGameItemInsert(CGameListItem * pGameListItem)
{
}

//更新通知
void GameManagerBase::OnGameItemUpdate(CGameListItem * pGameListItem)
{
}

//删除通知
void GameManagerBase::OnGameItemDelete(CGameListItem * pGameListItem)
{
}

//////////////////////////////////////////////////////////////////////////
//登陆信息

//请求失败
void GameManagerBase::onGRRequestFailure(const std::string& szDescribeString)
{
	PLAZZ_PRINTF(("GameManager::onGRRequestFailure"));
}

//登陆成功
void GameManagerBase::OnGRLogonSuccess(void* data, int dataSize)
{
	PLAZZ_PRINTF(("GameManager::OnGRLogonSuccess"));

	//效验参数
	ASSERT(dataSize==sizeof(CMD_GR_LogonSuccess));
	if (dataSize!=sizeof(CMD_GR_LogonSuccess)) return;

	//消息处理
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)data;

	//玩家属性
	UserInfo::Instance().setUserRight(pLogonSuccess->dwUserRight);
	UserInfo::Instance().setMasterRight(pLogonSuccess->dwMasterRight);
#ifdef ENABLE_SEND_HEART_TICK_TIMER
	RestTick();
#endif
}

//登陆失败
void GameManagerBase::OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString)
{
	PLAZZ_PRINTF(("GameManager::OnGRLogonFailure"));
}

//登陆完成
void GameManagerBase::OnGRLogonFinish()
{
	PLAZZ_PRINTF(("GameManager::OnGRLogonFinish"));

	m_eInReconnect = ReconnectStatus_NULL;
	TimeManager::Instance().removeByFun(TIME_CALLBACK(GameManagerBase::closeClinet,this));

	//重入判断
	if (mCServerItem && (mCServerItem->GetMeUserItem()!=NULL)&&(mCServerItem->GetMeUserItem()->GetUserStatus() >= US_SIT))
	{
		//启动进程
		if (!StartGame())
		{
			mCServerItem->OnGFGameClose(GameExitCode_CreateFailed);
		}
		return;
	}
	else if (m_eInReconnect == ReconnectStatus_Connecting)
	{
		if (mCServerItem)
			mCServerItem->OnGFGameClose(GameExitCode_Normal);
		return;
	}
	CB_GameLogonFinsh();
}
void GameManagerBase::CB_GameLogonFinsh()
{
	if (mCServerItem)
	{
#ifdef ENABLE_QUIK_SIT_DOWN
		//DONE:这里也是莫名其妙吧,为什么要自动坐下
		if (!mCServerItem->PerformQuickSitDown())
		{
			NoticeMsg::Instance().ShowTopMsgByScript("RoomFull");
		}
#else
		//DONE:这里请求私人场数据吧
		//sendSubGRProvateInfo();
#endif
	}
}
//更新通知
void GameManagerBase::OnGRUpdateNotify(byte cbMustUpdate, const std::string& szDownHttp)
{
	NoticeMsg::Instance().ShowTopMsg("Update");
	PLAZZ_PRINTF(("GameManager::OnGRUpdateNotify"));
	NoticeMsgBox::Instance().setDownLoadURL(szDownHttp.c_str());
	NoticeMsgBox::Instance().show();
	//JniFun::showWebView(szDownHttp.c_str());
}

//////////////////////////////////////////////////////////////////////////
//配置信息

//列表配置
void GameManagerBase::OnGRConfigColumn()
{
	PLAZZ_PRINTF(("GameManager::OnGRConfigColumn"));
}
//房间配置
void GameManagerBase::OnGRConfigServer()
{
	PLAZZ_PRINTF(("GameManager::OnGRConfigServer"));
	//创建桌子
	if (!mCServerItem) return;
	word tChairCount = mCServerItem->GetServerAttribute().wChairCount;

}
//道具配置
void GameManagerBase::OnGRConfigProperty()
{
	PLAZZ_PRINTF(("OnGRConfigProperty\n"));
}
//玩家权限配置
void GameManagerBase::OnGRConfigUserRight()
{
	PLAZZ_PRINTF(("OnGRConfigUserRight\n"));
}

//配置完成
void GameManagerBase::OnGRConfigFinish()
{
	PLAZZ_PRINTF(("OnGRConfigFinish\n"));

}

//////////////////////////////////////////////////////////////////////////
//用户信息

//用户进入
void GameManagerBase::OnGRUserEnter(IClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF(("GameManager::OnGRUserEnter"));

	char szDescribe[256] = { 0 };
	sprintf(szDescribe, ("OnGRUserEnter:%s\n"), pIClientUserItem->GetNickName());
	//PLAZZ_PRINTF(szDescribe);
}
//用户更新
void GameManagerBase::OnGRUserUpdate(IClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF(("GameManager::OnGRUserUpdate"));

	if (pIClientUserItem == IServerItem::get()->GetMeUserItem())
	{
	}
}
//用户删除
void GameManagerBase::OnGRUserDelete(IClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF("GameManager::OnGRUserDelete");
}

//////////////////////////////////////////////////////////////////////////
//框架消息

//用户邀请
void GameManagerBase::OnGFUserInvite(const std::string& szMessage)
{
	PLAZZ_PRINTF("GameManager::OnGFUserInvite");
}
//用户邀请失败
void GameManagerBase::OnGFUserInviteFailure(const std::string& szMessage)
{
	PLAZZ_PRINTF("GameManager::OnGFUserInviteFailure");
}

//房间退出
void GameManagerBase::OnGFServerClose(const std::string& szMessage)
{
	PLAZZ_PRINTF("ServerScene::OnGFServerClose");

}

//启动游戏
bool GameManagerBase::StartGame()
{
	PLAZZ_PRINTF("GameManager::StartGame");

	this->CreateKernel();

	return true;
}

void GameManagerBase::setPrivateServerSink(IServerPrivateSink* PrivateServerSink)
{
	_iPrivateServerSink = PrivateServerSink;
}

//创建游戏内核
bool GameManagerBase::CreateKernel()
{
	PLAZZ_PRINTF("GameManager::CreateKernel");

	setCurGameKindID(mGameServerItem->m_GameServer.wKindID);
	IClientKernelSink* pKernelSink = CreateGame();
	if (!pKernelSink)
	{
		IServerItem::get()->IntermitConnect(true);
		return false;
	}
	pKernelSink->clearInfo();
	pKernelSink->OnGameServerItem(mGameServerItem);
	IServerItem::get()->SetClientKernelSink(pKernelSink);
	IServerItem::get()->OnGFGameReady();
	return false;
}

//进入事件
bool GameManagerBase::InsertUserEnter(const char* pszUserName)
{
	return true;
}
//离开事件
bool GameManagerBase::InsertUserLeave(const char* pszUserName)
{
	return true;
}
//断线事件
bool GameManagerBase::InsertUserOffLine(const char* pszUserName)
{
	return true;

}
//////////////////////////////////////////////////////////////////////////

//普通消息(窗口输出)
bool GameManagerBase::InsertNormalString(const char* pszString)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pszString));
	return true;
}
//系统消息(窗口输出)
bool GameManagerBase::InsertSystemString(const char* pszString)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pszString));
	return true;
}
//系统消息(窗口输出)
bool GameManagerBase::InsertSystemStringScript(const char* pszString)
{
	NoticeMsg::Instance().ShowTopMsgByScript(pszString);
	return true;
}
//提示消息(对话框方式??)0:确认 1:确认,取消
int GameManagerBase::InsertPromptString(const char* pszString, int iButtonType)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pszString));
	return true;
}
bool GameManagerBase::InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////

void GameManagerBase::connectServer()
{
	if (mCServerItem->GetServiceStatus() == ServiceStatus_ServiceIng
		|| mCServerItem->GetServiceStatus() == ServiceStatus_Entering)
	{
		return;
	}
#ifdef ENABLE_SEND_HEART_TICK_TIMER
	RestTick();
#endif
	///< 载入游戏基础数据
	loadGameBaseData(mGameServerItem->m_GameServer.wKindID);
	mCServerItem->ConnectServer(mGameServerItem);
}

void GameManagerBase::disconnectServer()
{
	if (mCServerItem) {
		if (IServerItem::get()) {
			IServerItem::get()->IntermitConnect(true);
		}
	}
}
void GameManagerBase::StartGameReconnect()
{
	if (IServerItem::get()->IsService()) {
		m_eInReconnect = ReconnectStatus_DisConnect;
		disconnectServer();
	}
}
void GameManagerBase::onEventTCPSocketError(int errorCode)
{
	UserInfo::Instance().checkInGameServer();
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(GameManagerBase::closeClinet,this), 3.0f);
}
void GameManagerBase::closeClinet()
{
	if (mCServerItem) {
		mCServerItem->OnGFGameClose(GameExitCode_Normal);
	}
}

//////////////////////////////////////////////////////////////////////////

void GameManagerBase::upDataTime(float fTime)
{
	switch (_conToDoStatus)
	{
	case CS_REQUEST_PRIVATE_INFO:
		{
			//DONE: 这里没有必要再发送私人场请求了, 服务器在游戏登录时已经发送了
			_waitingTime += fTime;
			if (_waitingTime > flush_private_info_time / 1000.0f) {
				if (IServerItem::get()) {
					sendSubGRProvateInfo(WAITING_FLUSH_PRIVATE_INFO);
					_conToDoStatus = CS_NULL;
					_waitingTime = 0.f;
				}
			}
			break;
		}
	case CS_UPDATE_SERVERLIST:
		{
			_waitingTime += fTime;
			if (_waitingTime > flush_server_list_time / 1000.0f) {
				m_kLoginMission.updateServerList();
				_conToDoStatus = CS_NULL;
				_waitingTime = 0.f;
			}
			break;
		}
	case CS_REQUEST_PRIVATE_TABLE:
		{
			_waitingTime += fTime;
			if (_waitingTime > flush_table_time / 1000.0f) {
				sendPrivateTableInfo(WAITING_FLUSH_TABLE);
				_conToDoStatus = CS_NULL;
				_waitingTime = 0.f;
			}
			break;
		}
	}
#ifdef ENABLE_BACK_GROUND_THREAD
	if (!IServerItem::get() || !mGameServerItem)
	{
		return;
	}
	if (IServerItem::get()->IsService() && m_eInReconnect == ReconnectStatus_NULL)
	{
		m_fHeatTickTime += fTime;
	}
	if (m_eInReconnect == ReconnectStatus_DisConnect)
	{
		if (IServerItem::get()->IsService())
		{
			disconnectServer();
		}
		else
		{
			m_eInReconnect = ReconnectStatus_Connecting;
			connectServer();
		}
	}
	if (m_fHeatTickTime > MaxReconnectTime)
	{
		m_fHeatTickTime = 0;
		if (m_eInReconnect == ReconnectStatus_NULL)
		{
			NoticeMsg::Instance().ShowNoticeMsgByScript("NetReconnectError");
			m_eInReconnect = ReconnectStatus_DisConnect;
			disconnectServer();
		}
		else
		{
			m_eInReconnect = ReconnectStatus_NULL;

			if (mCServerItem)
			{
				mCServerItem->OnGFGameClose(GameExitCode_Normal);
			}
		}
	}
#endif
}

#ifdef ENABLE_SEND_HEART_TICK_TIMER

void GameManagerBase::sendHeartTick()
{
	TempCommandRec pCommand;
	pCommand.Command.wMainCmdID = MDM_KN_COMMAND;
	pCommand.Command.wSubCmdID = SUB_KN_DETECT_SOCKET;
	pCommand.pDataBuffer = new byte[4];
	pCommand.wDataSize = 8;
	if (CServerItem::get()) {
		CServerItem::get()->SendSocketData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pCommand.pDataBuffer, pCommand.wDataSize);
	}
}
void GameManagerBase::RestTick()
{
	m_eInReconnect = ReconnectStatus_NULL;
	m_fHeatTickTime = 0;
}
void GameManagerBase::HeartTick()
{
	if (!IServerItem::get()) {
		return;
	}
	RestTick();
}

#endif // ENABLE_SEND_HEART_TICK_TIMER

void GameManagerBase::onWeiXinResult(int result)
{
	switch (MissionWeiXin::Instance().getWXActionState())
	{
		case WX_AS_Share:
		{
			switch (result)
			{
				case 0:
					//public static final int ERR_OK = 0;//微信分享成功
					NoticeMsg::Instance().ShowTopMsg(utility::getScriptString("CCWeiXinShare_OK"));
					break;
				case -2:
					//public static final int ERR_USER_CANCEL = -2;
					NoticeMsg::Instance().ShowTopMsg(utility::getScriptString("CCWeiXinShare_Cancel"));
					break;
					//public static final int ERR_COMM = -1;
					//public static final int ERR_SENT_FAILED = -3;
					//public static final int ERR_AUTH_DENIED = -4;
					//public static final int ERR_UNSUPPORT = -5;
					//public static final int ERR_BAN = -6;
				default:
					break;
			}
			break;
		}
	}
}

bool GameManagerBase::BackKey()
{
	if (mCServerItem)
	{
		return mCServerItem->BackKey();
	}
	return false;
}

SCORE GameManagerBase::getGameDiFen()
{
	if (!mGameServerItem)
	{
		return 0;
	}
	return mGameServerItem->m_GameServer.lCellScore;
}

//////////////////////////////////////////////////////////////////////////

WORD GameManagerBase::getGameChairNum()
{
	if (!CServerItem::get()) {
		return 0;
	}
	return CServerItem::get()->GetCurServerChairsCount();
}
void sendLogonPackage()
{
	if (IServerItem::get()) {
		IServerItem::get()->SendLogonPacket();
	}
}
bool GameManagerBase::RequreRefreshTableInfo()
{
	if (!CServerItem::get()) {
		return false;
	}
	CMD_GF_Private_Request_Refresh_Table_info RefleshTableInfo;
	memset(&RefleshTableInfo, 0, sizeof(RefleshTableInfo));
	return CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_REQUEST_REFRESH_TABLE, &RefleshTableInfo, sizeof(RefleshTableInfo));
}
