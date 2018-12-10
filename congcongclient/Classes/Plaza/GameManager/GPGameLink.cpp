#include "GPGameLink.h"

#include "kernel/kernel/server/CServerItem.h"

#include "../HomeScene/GPHomeScene.h"
#include "../HomeScene/CreateRoom/GPHomeRoomListPanel_SRLF.h"
#include "../HomeScene/CreateRoom/GPHomeRoomListPanel_NN.h"
#include "../HomeScene/CreateRoom/GPHomeRoomListPanel_TLJ.h"
#include "../HomeScene/CreateRoom/GPHomeRoomListPanel_WZQ.h"
#include "../HomeScene/CreateRoom/GPHomeRoomListPanel_SG.h"
#include "../HomeScene/CreateRoom/GPHomeRoomListPanel_XZDD.h"

#include "Game/NN/NNGameScene.h"
#include "Game/ZJH/TLJGameScence.h"
#include "Game/SRLF/SRLFGameScence.h"
#include "Game/WZQ/WZQDGameScence.h"
#include "Game/SG/SGGameScence.h"
#include "Game/XZDD/XZDDGameScence.h"
 

//#define QUIT_ROOM_WITHOUT_AGREE

FV_SINGLETON_STORAGE(GPGameLink);

GPGameLink::GPGameLink()
	: m_kLoginMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
	, m_currentGameKindID(ScriptData<int>("GameKind").Value())
{
	zeromemory(&m_CreatePrivateNet, sizeof(m_CreatePrivateNet));
	zeromemory(&m_PrivateInfo, sizeof(m_PrivateInfo));
	m_JoinRoomNum.clear();
}

GPGameLink::~GPGameLink()
{

}

void GPGameLink::OnSocketSubPrivateInfo(CMD_GR_Private_Info* pNetInfo)
{
	memcpy(&m_PrivateInfo, pNetInfo, sizeof(m_PrivateInfo));
    switch(m_LinkAction)
    {
    case Type_Link_Create:
        {
            CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_CREATE_PRIVATE, &m_CreatePrivateNet, sizeof(m_CreatePrivateNet));
            zeromemory(&m_CreatePrivateNet, sizeof(m_CreatePrivateNet));
            break;
        }
    case Type_Link_Join:
        {
            CMD_GR_Join_Private kSendNet;
            kSendNet.dwRoomNum = utility::parseInt(m_JoinRoomNum);
            CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE, &kSendNet, sizeof(kSendNet));
            break;
        }
    case Type_Link_InqureTables:
        {
            m_gr_inqure_tables.bGetTableInfo = 1;
            CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_INQUIRE_TABLES, &m_gr_inqure_tables, sizeof(m_gr_inqure_tables));
            break;
        }
	default:
		{
			GameManagerBase::setConectedServer(true);
			dword dwRoomNO = GameManagerBase::getJoinWithRoomNO();
			CCLOG("OnSocketSubPrivateInfo dwRoomNo=%d", dwRoomNO);
			if (dwRoomNO != 0) {
				GPGameLink::Instance().JoinRoom(utility::toString(dwRoomNO));
			}
			break;
		}
    }
	m_LinkAction = Type_Link_NULL;
	UserInfo::Instance().setPrivateInGameServerID(pGameMan->getActServerID());
}

void GPGameLink::OnSocketSubPrivateCreateSuceess(CMD_GR_Create_Private_Sucess* pNetInfo)
{

}

void GPGameLink::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	
}

void GPGameLink::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	//UserInfo::Instance().setPrivateInGameServerID(-1);
	//DONE:显示结算面板
	GPHomeScene::Instance().showGameCalculate(pNetInfo);
}

void GPGameLink::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{

}

void GPGameLink::OnSocketSubPrivateInqureTableRet(void* data, size_t dataSize)
{
	CCAssert(dataSize >= sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_HEAD), "OnSocketSubPrivateInqureTableRet data size error");

	switch (m_currentGameKindID)
	{
	case NNGameScene::KIND_ID: //21:
		GPHomeScene::Instance().showGameRoomList(data, dataSize);
		//DONE:直接在大厅刷新桌子
		//GPHomeRoomListPanel_NN::Instance().RefreshListView(data, dataSize);
		break;
	//case SRLFGameScence::KIND_ID:// 302:
	//	GPHomeRoomListPanel_SRLF::Instance().RefreshListView(data, dataSize);
	//	break;
	//case TLJGameScence::KIND_ID://317:
	//	GPHomeRoomListPanel_TLJ::Instance().RefreshListView(data, dataSize);
	//	break;
	//case WZQDGameScence::KIND_ID://888:
	//	GPHomeRoomListPanel_WZQ::Instance().RefreshListView(data, dataSize);
	//	break;
	//case SGGameScence::KIND_ID://999:
	//	GPHomeRoomListPanel_SG::Instance().RefreshListView(data, dataSize);
	//	break;
	//case XZDDGameScence::KIND_ID:
	//	GPHomeRoomListPanel_XZDD::Instance().RefreshListView(data, dataSize);
	//	break;
	}
}

//////////////////////////////////////////////////////////////////////////

void GPGameLink::DismissRoom(bool isAgree)
{
#ifdef QUIT_ROOM_WITHOUT_AGREE
	CServerItem::get()->PerformStandUpAction();
#else
	CMD_GR_Dismiss_Private kNetInfo;
	kNetInfo.bDismiss = isAgree;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_PRIVATE_DISMISS, &kNetInfo, sizeof(kNetInfo));
#endif
}

//////////////////////////////////////////////////////////////////////////

void GPGameLink::JoinRoom(std::string roomNum)
{
	if (roomNum.length() == 6) {
		m_JoinRoomNum = roomNum;
		auto actJoinNum = utility::parseInt(roomNum);
		int iServerID = actJoinNum / 10000 - 10;
		CGameServerItem* pGameServer = pSevList->SearchGameServer(iServerID);

		if (pGameServer && pGameServer->IsPrivateRoom()) {
			auto ret = pGameMan->connectGameServerByServerID(iServerID);
			if (ret == ServiceStatus_ServiceIng) {
				CMD_GR_Join_Private kSendNet;
				kSendNet.dwRoomNum = utility::parseInt(m_JoinRoomNum);
				CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE, &kSendNet, sizeof(kSendNet));
			} else {
				m_LinkAction = Type_Link_Join;
			}
		} else {
			NoticeMsg::Instance().ShowTopMsgByScript("JoinRoomNumError");
		}
	}
}

void GPGameLink::CreateRoom(word gameKindID, CMD_GR_Create_Private& pNetInfo)
{
	auto ret = pGameMan->connectGameServerByKindID(gameKindID, GAME_GENRE_EDUCATE);
	if (ret == ServiceStatus_ServiceIng) {
		CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_CREATE_PRIVATE, &pNetInfo, sizeof(pNetInfo));
		zeromemory(&m_CreatePrivateNet, sizeof(m_CreatePrivateNet));
	} else {
		memcpy(&m_CreatePrivateNet, &pNetInfo, sizeof(pNetInfo));
		m_LinkAction = Type_Link_Create;
	}
}

//最简单粗暴的定时器
dword time_begin = 0;
void GPGameLink::ConnectAndInqureTables(word wKindID)
{
	if (time_begin == 0) {
		time_begin = time(nullptr);
	} else {
		dword time_end = time(nullptr);
		if (time_end - time_begin > 1) {
			time_begin = time_end;
		} else {
			return;
		}
	}
	auto ret = pGameMan->connectGameServerByKindID(wKindID, GAME_GENRE_EDUCATE);
	if (ret == ServiceStatus_ServiceIng) {
		m_gr_inqure_tables.bGetTableInfo = 1;
		CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_INQUIRE_TABLES, &m_gr_inqure_tables, sizeof(m_gr_inqure_tables));
	} else {
		m_LinkAction = Type_Link_InqureTables;
	}
	setCurrentRequestGameKindID(wKindID);
}
