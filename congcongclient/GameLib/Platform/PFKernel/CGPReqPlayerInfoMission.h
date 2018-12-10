#pragma once

#include <cocos2d.h>

#include "Kernel/network/CCallMission.h"
#include "Platform/PFDefine/df/types.h"

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

	void QUERY_GP_USER_INDIVIDUAL(int iAccountID);
	void CB_GP_USER_INDIVIDUAL(int iAccountID);
	void ON_GP_USER_INDIVIDUAL(void* data, int dataSize);
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
