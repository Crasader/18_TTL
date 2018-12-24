#include "CGPGameRecord.h"

#include "Platform/PFDefine/msg/CMD_LogonServer.h"

CGPGameRecordMission::CGPGameRecordMission(const char* url, int port)
: CCallMission("CGPGameRecordMission",url, port)
{
	mIGPGameRecordSink = 0;

	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_GetGameRecordList,this),SUB_GP_GAME_RECORD_LIST);
	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_GetGameRecordListEx, this), SUB_GP_GAME_RECORD_LIST_EX);
	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_PrivateRandTotalRecord,this),SUB_GP_GAME_RECORD_TOTAL);
	addNetCall(CC_CALLBACK_2(CGPGameRecordMission::Net_PrivateRandChildRecord,this),SUB_GP_GAME_RECORD_CHILD);
}

// 设置回调接口
void CGPGameRecordMission::setMissionSink(CGGameRecordSink* pIGPKefuMissionSink)
{
	mIGPGameRecordSink = pIGPKefuMissionSink;
}

void CGPGameRecordMission::GetGameRecordList(CMD_GP_GetGameRecord_List listData)
{
	CCLOG("CGPGameRecordMission::GetGameRecordList\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_GetGameRecordList,this, listData));

	start();
}

void CGPGameRecordMission::CB_GetGameRecordList(CMD_GP_GetGameRecord_List listData)
{
	CCLOG("CGPGameRecordMission::CB_GetGameRecordList\n");
	CMD_GP_GetGameRecord_List kNetInfo;
	kNetInfo = listData;

	send(MDM_GP_USER_SERVICE,SUB_GP_GAME_RECORD_LIST,&kNetInfo,sizeof(kNetInfo));
}

void CGPGameRecordMission::Net_GetGameRecordList(void* data, int dataSize)
{
	CCLOG("CGPGameRecordMission::Net_GetGameRecordList\n");

	datastream kDataStream(data,dataSize);
	tagPrivateRandTotalRecordList kNetInfo;
	kNetInfo.StreamValue(kDataStream,false);

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameRecordList(&kNetInfo);
	}

	stop();
}

void CGPGameRecordMission::GetGameRecordListEx(CMD_GP_GetGameRecord_List listData)
{
	CCLOG("CGPGameRecordMission::GetGameRecordListEx\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_GetGameRecordListEx, this, listData));

	start();
}

void CGPGameRecordMission::CB_GetGameRecordListEx(CMD_GP_GetGameRecord_List listData)
{
	CCLOG("CGPGameRecordMission::CB_GetGameRecordListEx\n");
	CMD_GP_GetGameRecord_List kNetInfo;
	kNetInfo = listData;

	send(MDM_GP_USER_SERVICE, SUB_GP_GAME_RECORD_LIST_EX, &kNetInfo, sizeof(kNetInfo));
}

void CGPGameRecordMission::Net_GetGameRecordListEx(void* data, int dataSize)
{
	CCLOG("CGPGameRecordMission::Net_GetGameRecordListEx\n");

	datastream kDataStream(data, dataSize);
	tagGameRecordListEx kNetInfo;
	kNetInfo.StreamValue(kDataStream, false);

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameRecordListEx(&kNetInfo);
	}

	stop();
}

void CGPGameRecordMission::GetPrivateRandTotalRecord(int iRecordID)
{
	CCLOG("CGPGameRecordMission::GetGameRecordVideo\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_PrivateRandTotalRecord,this,iRecordID));

	start();
}

void CGPGameRecordMission::CB_PrivateRandTotalRecord(int iRecordID)
{
	CMD_GP_GetGameTotalRecord kNetInfo;
	kNetInfo.dwUserID = 0;
	kNetInfo.dwRecordID = iRecordID;
	send(MDM_GP_USER_SERVICE, SUB_GP_GAME_RECORD_TOTAL,&kNetInfo,sizeof(CMD_GP_GetGameTotalRecord));
}

void CGPGameRecordMission::Net_PrivateRandTotalRecord(void* data, int dataSize)
{

	datastream kStream(data,dataSize);
	tagPrivateRandTotalRecord kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameTotalRecord(&kNetInfo);
	}

	stop();
}

void CGPGameRecordMission::GetPrivateRandChildRecord(int iChildRecordID)
{
	CCLOG("CGPGameRecordMission::GetPrivateRandChildRecord\n");

	addLinkCallFun(CC_CALLBACK_0(CGPGameRecordMission::CB_PrivateRandChildRecord,this,iChildRecordID));

	start();
}

void CGPGameRecordMission::CB_PrivateRandChildRecord(int iChildRecordID)
{
	CMD_GP_GetGameChildRecord kNetInfo;
	kNetInfo.dwUserID = 0;
	kNetInfo.dwRecordID = iChildRecordID;
	send(MDM_GP_USER_SERVICE, SUB_GP_GAME_RECORD_CHILD,&kNetInfo,sizeof(CMD_GP_GetGameChildRecord));
}

void CGPGameRecordMission::Net_PrivateRandChildRecord(void* data, int dataSize)
{
	datastream kStream(data,dataSize);
	tagPrivateRandRecordChild kNetInfo;
	kNetInfo.StreamValue(kStream,false);

	if (mIGPGameRecordSink)
	{
		mIGPGameRecordSink->onGPBackGameChildRecord(&kNetInfo);
	}

	stop();
}