#include "CGPopularizeMission.h"

CGPopularizeMission::CGPopularizeMission(const char* url, int port)
	:CCallMission("CGPopularizeMission", url, port)
{
	m_Sink = nullptr;
	addNetCall(CC_CALLBACK_2(CGPopularizeMission::Net_GetPopularizeList, this), SUB_GP_POPULARIZE_LIST);
	addNetCall(CC_CALLBACK_2(CGPopularizeMission::Net_PopularizeBind, this), SUB_GP_POPULARIZE_BIND);
	addNetCall(CC_CALLBACK_2(CGPopularizeMission::Net_PopularizeHttp, this), SUB_GP_POPULARIZE_HTTP);
}

void CGPopularizeMission::setMissionSink(CGPopularizeSink* sink)
{
	m_Sink = sink;
}

void CGPopularizeMission::GetPopularizeList(BYTE bPopularizeType, DWORD gameKindID)
{
	CCLOG("CGPopularizeMission::GetPopularizeList\n");
	addLinkCallFun(CC_CALLBACK_0(CGPopularizeMission::CB_GetPopularizeList, this, bPopularizeType, gameKindID));
	start();
}

void CGPopularizeMission::CB_GetPopularizeList(BYTE bPopularizeType, DWORD gameKindID)
{
	CCLOG("CGPopularizeMission::CB_GetPopularizeList\n");
	CMD_GP_PopularizeList kNetInfo;
	kNetInfo.bPopularizeType = bPopularizeType;
	kNetInfo.dwGameKindID = gameKindID;
	send(MDM_GP_USER_SERVICE, SUB_GP_POPULARIZE_LIST, &kNetInfo, sizeof(kNetInfo));
}

void CGPopularizeMission::Net_GetPopularizeList(void* data, int dataSize)
{
	CCLOG("CGPopularizeMission::Net_GetPopularizeList\n");

	datastream kDataStream(data, dataSize);
	CMD_GP_PopularizeListRes kNetInfo;
	kNetInfo.StreamValue(kDataStream, false);

	if (m_Sink) {
		m_Sink->onGPPopularizeListRes(&kNetInfo);
	}
	stop();
}

void CGPopularizeMission::PopularizeBind(std::string popularizeCode)
{
	CCLOG("CGPopularizeMission::GetPopularizeList\n");
	addLinkCallFun(CC_CALLBACK_0(CGPopularizeMission::CB_PopularizeBind, this, popularizeCode));
	start();
}

void CGPopularizeMission::CB_PopularizeBind(std::string popularizeCode)
{
	CCLOG("CGPopularizeMission::CB_PopularizeBind\n");
	CMD_GP_PopularizeBind kNetInfo;
	memset(&kNetInfo, 0, sizeof(kNetInfo)); 
	memcpy(kNetInfo.szPopularizeCode, popularizeCode.c_str(), sizeof(kNetInfo.szPopularizeCode));
	send(MDM_GP_USER_SERVICE, SUB_GP_POPULARIZE_BIND, &kNetInfo, sizeof(kNetInfo));
}

void CGPopularizeMission::Net_PopularizeBind(void* data, int dataSize)
{
	CCLOG("CGPopularizeMission::Net_PopularizeBind\n");
	
	CCASSERT(dataSize == sizeof(CMD_GP_PopularizeBindRes), "PopularizeBindRes Error!");
	CMD_GP_PopularizeBindRes kNetInfo;
	memcpy(&kNetInfo, data, sizeof(kNetInfo));
	if (m_Sink) {
		m_Sink->onGPPopularizeBindRes(kNetInfo);
	}
	stop();
}

void CGPopularizeMission::PopularizeHttp(int Version)
{
	CCLOG("CGPopularizeMission::PopularizeHttp\n");
	addLinkCallFun(CC_CALLBACK_0(CGPopularizeMission::CB_PopularizeHttp, this, Version));
	start();
}

void CGPopularizeMission::CB_PopularizeHttp(int Version )
{
	CCLOG("CGPopularizeMission::CB_PopularizeHttp\n");
	CMD_GP_PopularizeHttp kNetInfo;
	memset(&kNetInfo, 0, sizeof(kNetInfo)); 
	kNetInfo.dwVersion = Version;
	send(MDM_GP_USER_SERVICE, SUB_GP_POPULARIZE_HTTP, &kNetInfo, sizeof(kNetInfo));
}

void CGPopularizeMission::Net_PopularizeHttp(void* data, int dataSize)
{
	CCLOG("CGPopularizeMission::Net_PopularizeHttp\n");

	CCASSERT(dataSize == sizeof(CMD_GP_PopularizeHttpRes), "PopularizeHttpRes Error!");
	CMD_GP_PopularizeHttpRes kNetInfo;
	memcpy(&kNetInfo, data, sizeof(kNetInfo));
	if (m_Sink) {
		m_Sink->onGPPopularizeHttpRes(kNetInfo);
	}
	stop();
}
