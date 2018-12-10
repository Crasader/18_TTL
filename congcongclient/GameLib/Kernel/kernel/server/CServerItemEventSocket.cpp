#include "Kernel/kernel/server/CServerItem.h"
#include "Platform/PFDefine/msg/CMD_GameServer.h"
#include "Tools/utilityConvert.h"
//////////////////////////////////////////////////////////////////////////
// ISocketEngineSink
//////////////////////////////////////////////////////////////////////////
void CServerItem::onEventTCPSocketLink()
{
	CCLOG("CServerItem::onEventTCPSocketLink\n");

	SetGameStatus(ServiceStatus_ServiceIng);
	SendLogonPacket();
}

void CServerItem::onEventTCPSocketShut()
{
	CCLOG("CServerItem::onEventTCPSocketShut\n");

	if (mIServerItemSink)
		mIServerItemSink->OnGFServerClose(("CServerItem::onEventTCPSocketShut()"));
	
	restSeverState();
}

void CServerItem::onEventTCPSocketError(int errorCode)
{
	CCLOG("CServerItem::onEventTCPSocketError err:%d\n");

	if (mIServerItemSink)
		mIServerItemSink->onEventTCPSocketError(errorCode);

	restSeverState();
}

bool CServerItem::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	switch (main)
	{ 
		//登录消息
	case MDM_GR_LOGON:	return OnSocketMainLogon(sub,data,dataSize);
		//配置信息
	case MDM_GR_CONFIG:	return OnSocketMainConfig(sub,data,dataSize);
		//用户信息
	case MDM_GR_USER:	return OnSocketMainUser(sub,data,dataSize);
		//状态信息
	case MDM_GR_STATUS:	return OnSocketMainStatus(sub,data,dataSize);
		//系统消息
	case MDM_CM_SYSTEM:	return OnSocketMainSystem(sub,data,dataSize);
		//游戏消息
	case MDM_GF_GAME:		
		//框架消息
	case MDM_GF_FRAME:	return OnSocketMainGameFrame(main,sub,data,dataSize);
		//比赛消息
	case MDM_GR_MATCH:	return OnSocketMainMatch(sub,data,dataSize);
		//私人场消息
	case MDM_GR_PRIVATE:return OnSocketMainPrivate(sub,data,dataSize);
        //系统消息
    case MDM_C2G_SYSTEM_MSG: return OnSocketSystemMessage(sub,data,dataSize);
		//抽奖测试信息
	//case MDM_GR_CHOUJIANG:return OnSocketMainChouJiang(sub,data,dataSize);
	}
	CCASSERT(false,"");
	return true;
}

//////////////////////////////////////////////////////////////////////////
