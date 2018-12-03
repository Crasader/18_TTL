#include "GamePlayer.h"
#include "Game/GameLib.h"
#include "Platform/PFKernel/CGPIndividualMission.h"

GamePlayer::GamePlayer(IClientUserItem* pUserItem)
	: m_pUserItem(pUserItem)
	, pHeadNode(nullptr)
{
	m_kRequestMissionHead = new CGPReqPlayerInfoMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value());
	m_kRequestMissionHead->setMissionSink(this);
}
GamePlayer::~GamePlayer()
{
	if (m_kRequestMissionHead) {
		delete m_kRequestMissionHead;
		m_kRequestMissionHead = nullptr;
	}
}
void GamePlayer::setUserItem(IClientUserItem* pItem)
{
	m_pUserItem = pItem;
	upPlayerInfo();
}
IClientUserItem* GamePlayer::getUserItem(bool bAssert)
{
	CCAssert(m_pUserItem || !bAssert,"");
	return m_pUserItem;
}
word GamePlayer::GetTableID()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetTableID();
}
word GamePlayer::GetChairID()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetChairID();
}
byte GamePlayer::GetUserStatus()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetUserStatus();
}
SCORE GamePlayer::GetUserScore()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetUserScore();
}
word GamePlayer::GetFaceID()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetFaceID();
}
byte GamePlayer::GetGender()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetGender();
}
dword GamePlayer::GetUserID()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetUserID();
}
dword GamePlayer::GetGameID()
{
	if (!m_pUserItem) {
		return 0;
	}
	return m_pUserItem->GetGameID();
}
std::string GamePlayer::GetNickName()
{
	if (!m_pUserItem) {
		return "";
	}
	return utility::a_u8(m_pUserItem->GetNickName());
}
std::string GamePlayer::GetHeadHttp()
{
	if (!m_pUserItem) {
		return "";
	}
	return m_pUserItem->GetUserInfo()->szHeadHttp;
}
tagUserInfo * GamePlayer::GetUserInfo()
{
	if (!m_pUserItem) {
		return NULL;
	}
	return m_pUserItem->GetUserInfo();
}

void GamePlayer::setHeadNode(cocos2d::Node* pHeadNode)
{
	this->pHeadNode = pHeadNode;
}

void GamePlayer::requesUserInfo()
{
	if (m_pUserItem) {
		m_kRequestMissionHead->query(m_pUserItem->GetUserID());
	}
}
void GamePlayer::requestHeadInfo()
{
	if (m_pUserItem && m_pUserItem->GetUserInfo()->szHeadHttp[0] == 0) {
		m_kRequestMissionHead->requreUserInfoSingle(DTP_GP_UI_HEAD_HTTP);
	}
}
void GamePlayer::on_GP_UserIndividual(void* data, int dataSize)
{
	if (!m_pUserItem) {
		utility::log("GamePlayer::on_GP_UserIndividual failed !m_pUserItem");
	}
	CMD_GP_UserIndividual* pModifyIndividual = (CMD_GP_UserIndividual*)data;

	void * pDataBuffer = NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pModifyIndividual + 1, dataSize - sizeof(CMD_GP_UserIndividual));

	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();

	//扩展信息
	while (true) {
		pDataBuffer = RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe == DTP_NULL) break;
		switch (DataDescribe.wDataDescribe) {
			case DTP_GP_UI_HEAD_HTTP: {	//头像url
					if (DataDescribe.wDataSize < LEN_USER_NOTE) {
						memcpy(m_pUserItem->GetUserInfo()->szHeadHttp, pDataBuffer, DataDescribe.wDataSize);
						m_pUserItem->GetUserInfo()->szHeadHttp[DataDescribe.wDataSize ] = 0;
						if (pHeadNode) {
							ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pHeadNode,
								"NNPlayer_Avatar"),
								m_pUserItem->GetUserInfo()->szHeadHttp,
								m_pUserItem->GetUserID());
						}
					}
					break;
				}
			case DTP_GP_UI_IP: {//联系地址
				if (DataDescribe.wDataSize < LEN_NICKNAME) {
					memcpy(m_pUserItem->GetUserInfo()->szLogonIP, pDataBuffer, DataDescribe.wDataSize);
					m_pUserItem->GetUserInfo()->szLogonIP[DataDescribe.wDataSize] = 0;
				}
				break;
			}
			case DTP_GP_UI_CHANNEL: {//联系地址
				if (DataDescribe.wDataSize < LEN_NICKNAME) {
					//如果是自己更新自己的信息
					if (pModifyIndividual->dwUserID == pGlobalUserInfo->getUserID()) {
						tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
						memcpy(pGlobalUserData->szUserChannel, pDataBuffer, DataDescribe.wDataSize);
						pGlobalUserData->szUserChannel[DataDescribe.wDataSize] = 0;
						if (pModifyIndividual->dwUserID == pGlobalUserInfo->getUserID()) {
						}
					}
				}
				break;
			}
		}
	}

	//如果是自己更新自己的信息
	if (pModifyIndividual->dwUserID == pGlobalUserInfo->getUserID()) {
		//金币信息
		pGlobalUserInfo->upPlayerInfo();
	}
}
void GamePlayer::onUserInfoSingle(void * data, int size)
{

}
