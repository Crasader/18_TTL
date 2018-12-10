#pragma once

#include "common.h"
#include "Kernel/kernel/server/IServerPrivateSink.h"
#include "Platform/PFKernel/CGPLoginMission.h"

#include CMD_GAME

class GPGameLink
	: public IServerPrivateSink
	, public FvSingleton<GPGameLink>
{
	FRIEND_SINGLETON(GPGameLink)
public:
	enum Type_LinkAction {
		Type_Link_NULL,
		Type_Link_Create,
		Type_Link_Join,
        Type_Link_InqureTables,
	};
public:
	virtual void OnSocketSubPrivateInfo(CMD_GR_Private_Info* pNetInfo);
	virtual void OnSocketSubPrivateCreateSuceess(CMD_GR_Create_Private_Sucess* pNetInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	virtual void OnSocketSubPrivateInqureTableRet(void* data, size_t dataSize) override;

public:
	//DONE: 三个逗比函数,如今也可以直接发送消息了,还是保留的以前的操作

	void CreateRoom(word gameKindID, CMD_GR_Create_Private& pNetInfo);
	void JoinRoom(std::string roomNum);
    void ConnectAndInqureTables(word iKindID = 0);

    void setCurrentRequestGameKindID(int iKindID){ 
		m_currentGameKindID = iKindID;
	}
    int getCurrentRequestGameKindID(){ 
		return m_currentGameKindID;
	}
	CMD_GR_Private_Info& privateGameInfo() { 
		return m_PrivateInfo;
	}

public:

	void DismissRoom(bool isAgree = true);

private:
	CMD_GR_Private_Info m_PrivateInfo;
	CMD_GF_Private_Room_Info m_PrivateRoomInfo;
	CMD_GR_Create_Private m_CreatePrivateNet;
	Type_LinkAction m_LinkAction;

    CMD_GR_INQURE_TABLES_INFO m_gr_inqure_tables;//请求游戏桌子数据
    int m_currentGameKindID;//当前请求的游戏ID

	std::string m_JoinRoomNum;
	CGPLoginMission m_kLoginMission;
};
