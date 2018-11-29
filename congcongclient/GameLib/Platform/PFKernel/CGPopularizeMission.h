#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CCallMission.h"

class CGPopularizeSink
{
public:
	virtual ~CGPopularizeSink(){}
	virtual void onGPPopularizeListRes(CMD_GP_PopularizeListRes* pNetInfo){}
	virtual void onGPPopularizeBindRes(const CMD_GP_PopularizeBindRes& pNetInfo){}
	virtual void onGPPopularizeHttpRes(const CMD_GP_PopularizeHttpRes& pNetInfo){}
};

class CGPopularizeMission
	: public CCallMission
{
public:
	CGPopularizeMission(const char* url, int port);

public:
	void setMissionSink(CGPopularizeSink* sink);

public:
	void GetPopularizeList(BYTE bPopularizeType, DWORD gameKindID);
	void CB_GetPopularizeList(BYTE bPopularizeType, DWORD gameKindID);
	void Net_GetPopularizeList(void* data, int dataSize);

	void PopularizeBind(std::string popularizeCode);
	void CB_PopularizeBind(std::string popularizeCode);
	void Net_PopularizeBind(void* data, int dataSize);

	void PopularizeHttp(int Version);
	void CB_PopularizeHttp(int Version);
	void Net_PopularizeHttp(void* data, int dataSize);
private:
	CGPopularizeSink* m_Sink;
};