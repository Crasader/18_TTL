#pragma once
#include "cocos2d.h"
#include "Platform/PlatformHeader.h"
#include "Kernel/network/CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
struct  ICGPReqPlayerInfoSink
{
	virtual void on_GP_UserIndividual(void* data, int size) = 0;
	virtual void onUserInfoSingle(void* data, int size) = 0;
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPReqPlayerInfoMission
{
public:

	CGPReqPlayerInfoMission(const char* url, int port);
	virtual ~CGPReqPlayerInfoMission();

public:
	// 设置回调接口
	void setMissionSink(ICGPReqPlayerInfoSink* pReqPlayerInfoSink);

	void query(int iAccountID);
	void CB_queryInfo(int iAccountID);
	void Net_queryInfo(void* data, int dataSize);
	//////////////////////////////////////////////////////////////////////////
	//查询头像url
	void requreUserInfoSingle(byte infoType);
	void onUserInfoSingle(void* data, int size);
	//////////////////////////////////////////////////////////////////////////
private:
	// 回调
	ICGPReqPlayerInfoSink* mIGPReqPlayerInfoSink;
	CCallMission* _CallMission;
}; 
