#pragma once
#include <cocos2d.h>
#include "Kernel/network/CCallMission.h"
#include "Platform/PFDefine/msg/CMD_LogonServer.h"

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
struct tagPrivateRandTotalRecordList;
struct tagGameRecordListEx;
struct tagPrivateRandTotalRecord;
struct tagPrivateRandRecordChild;

class CGGameRecordSink
{
public:
	virtual ~CGGameRecordSink(){}
	virtual void onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo){}
	virtual void onGPBackGameRecordListEx(tagGameRecordListEx* pNetInfo){}
	virtual void onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo){}
	virtual void onGPBackGameChildRecord(tagPrivateRandRecordChild* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////
// 游戏记录
//////////////////////////////////////////////////////////////////////////
class CGPGameRecordMission
	: public CCallMission
{
public:
	CGPGameRecordMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGGameRecordSink* pIGPKefuMissionSink);

	void GetGameRecordList(CMD_GP_GetGameRecord_List listData);
	void CB_GetGameRecordList(CMD_GP_GetGameRecord_List listData);
	void Net_GetGameRecordList(void* data, int dataSize);

	// 金币输赢记录
	void GetGameRecordListEx(CMD_GP_GetGameRecord_List listData);
	void CB_GetGameRecordListEx(CMD_GP_GetGameRecord_List listData);
	void Net_GetGameRecordListEx(void* data, int dataSize);

	void GetPrivateRandTotalRecord(int iRecordID);
	void CB_PrivateRandTotalRecord(int iRecordID);
	void Net_PrivateRandTotalRecord(void* data, int dataSize);

	void GetPrivateRandChildRecord(int iChildRecordID);
	void CB_PrivateRandChildRecord(int iChildRecordID);
	void Net_PrivateRandChildRecord(void* data, int dataSize);

private:
	// 回调
	CGGameRecordSink* mIGPGameRecordSink;
}; 
