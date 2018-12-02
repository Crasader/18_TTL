#include "Kernel/kernel/server/CServerItem.h"
//
bool CServerItem::OnSocketMainPrivate(int sub, void* data, int dataSize)
{
	switch (sub)
	{
	//费用查询
	case SUB_GR_PRIVATE_INFO:			return OnSocketSubPrivateInfo(data,dataSize);
	case SUB_GR_CREATE_PRIVATE_SUCESS:	return OnSocketSubPrivateCreateSuceess(data,dataSize);
	case SUB_GF_PRIVATE_ROOM_INFO:		return OnSocketSubPrivateRoomInfo(data,dataSize);
	case SUB_GF_PRIVATE_END:			return OnSocketSubPrivateEnd(data,dataSize);
	case SUB_GR_PRIVATE_DISMISS:		return OnSocketSubPrivateDismissInfo(data,dataSize);
	case SUB_GR_INQUIRE_TABLES_RET:	 return OnSocketSubPrivateInQureTableRet(data,dataSize);
	}
	return true;
}
//
bool CServerItem::OnSocketSubPrivateInfo(void* data, int dataSize)
{
	CCLOG("CServerItem::OnSocketSubPrivateInfo\n");

	CMD_GR_Private_Info private_game_Info;

	memcpy(&private_game_Info, data, sizeof(CMD_GR_Private_Info_Server));

	if (mIServerPrivateSink)
	{
		mIServerPrivateSink->OnSocketSubPrivateInfo(&private_game_Info);
	}

	return true;
}
//
bool CServerItem::OnSocketSubPrivateCreateSuceess(void* data, int dataSize)
{
	CCLOG("CServerItem::OnSocketSubPrivateCreateSuceess\n");

	ASSERT(dataSize==sizeof(CMD_GR_Create_Private_Sucess));
	if(dataSize!=sizeof(CMD_GR_Create_Private_Sucess)) return false;

	CMD_GR_Create_Private_Sucess *pNetInfo = (CMD_GR_Create_Private_Sucess*)data;

	if (mIServerPrivateSink)
	{
		mIServerPrivateSink->OnSocketSubPrivateCreateSuceess(pNetInfo);
	}

	return true;
}
bool CServerItem::OnSocketSubPrivateRoomInfo(void* data, int dataSize)
{
	CCLOG("CServerItem::OnSocketSubPrivateRoomInfo\n");

	datastream kDataStream(data,dataSize);
	CMD_GF_Private_Room_Info kNetInfo;
	kNetInfo.StreamValue(kDataStream,false);

	if (mIServerPrivateSink)
	{
		mIServerPrivateSink->OnSocketSubPrivateRoomInfo(&kNetInfo);
	}
	if(mIClientKernelSink)
	{
		mIClientKernelSink->OnSocketSubPrivateRoomInfo(&kNetInfo);
	}
	return true;
}
bool CServerItem::OnSocketSubPrivateEnd(void* data, int dataSize)
{
	CCLOG("CServerItem::OnSocketSubPrivateEnd\n");

	datastream kDataStream(data,dataSize);
	CMD_GF_Private_End_Info kNetInfo;
	kNetInfo.StreamValue(kDataStream,false);

	if (mIServerPrivateSink)
	{
		mIServerPrivateSink->OnSocketSubPrivateEnd(&kNetInfo);
	}
	if(mIClientKernelSink)
	{
		mIClientKernelSink->OnSocketSubPrivateEnd(&kNetInfo);
	}
	return true;
}
bool CServerItem::OnSocketSubPrivateDismissInfo(void* data, int dataSize)
{
	CCLOG("CServerItem::OnSocketSubPrivateDismissInfo\n");

	ASSERT(dataSize==sizeof(CMD_GF_Private_Dismiss_Info));
	if(dataSize!=sizeof(CMD_GF_Private_Dismiss_Info)) return false;

	CMD_GF_Private_Dismiss_Info *pNetInfo = (CMD_GF_Private_Dismiss_Info*)data;

	if (mIServerPrivateSink)
	{
		mIServerPrivateSink->OnSocketSubPrivateDismissInfo(pNetInfo);
	}
	if(mIClientKernelSink)
	{
		mIClientKernelSink->OnSocketSubPrivateDismissInfo(pNetInfo);
	}
	return true;
}
bool CServerItem::OnSocketSubPrivateInQureTableRet(void* data, int dataSize)
{
    if (mIServerPrivateSink)
    {
        mIServerPrivateSink->OnSocketSubPrivateInqureTableRet(data, dataSize);
        return true;
    }
    return false;
}
