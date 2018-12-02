#include "GameBase.h"
#include "Game/GameLib.h"
#include "Kernel/kernel/server/IServerItem.h"

//旁观消息
bool GameBase::OnEventLookonMode(void* data, int dataSize)
{
	return true;
}
//场景消息
bool GameBase::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
void GameBase::addNetCB(int iIdex, cocos2d::Ref* pScence, std::function<void(void*, int)> pCallBack, std::string kCallName)
{
	NET_CBInfo kInfo;
	kInfo.iIdex = iIdex;
	kInfo.pCallBack = pCallBack;
	kInfo.pSrc1 = pScence;
	kInfo.kCallFunName = kCallName;
	m_kCBInfoList.push_back(kInfo);
}
//场景消息
bool GameBase::OnEventGameMessage(int sub, void* data, int dataSize)
{
	//std::vector<NET_CBInfo*> kCallFun;
	//NET_CBInfoList::iterator itCBInfo = m_kCBInfoList.begin();
	//while (itCBInfo != m_kCBInfoList.end()) {
	//	if (itCBInfo->iIdex == sub) {
	//		kCallFun.push_back(&(*itCBInfo));
	//	}
	//	itCBInfo++;
	//}
	auto itCBInfo = m_kCBInfoList.begin();
	while (itCBInfo != m_kCBInfoList.end()) {
		if (itCBInfo->iIdex == sub) {
			//kCallFun.push_back(&(*itCBInfo));
			utility::log(itCBInfo->kCallFunName.c_str());
			itCBInfo->pCallBack(data, dataSize);
		}
		itCBInfo++;
	}
	//std::vector<NET_CBInfo*>::iterator itCBInfoToCall = kCallFun.begin();
	//while (itCBInfoToCall != kCallFun.end()) {
	//	utility::log((*itCBInfoToCall)->kCallFunName.c_str());
	//	((*itCBInfoToCall)->pCallBack)(data, dataSize);
	//	itCBInfoToCall++;
	//}
	return true;
}

void GameBase::sendReady()
{
	IServerItem::get()->SendUserReady(0, 0);
}
//发送函数
bool GameBase::SendSocketData( word wSubCmdID)
{
	utility::log("GameBase::SendSocketData %d", (int)wSubCmdID);
	return IServerItem::get()->SendSocketData(MDM_GF_GAME,wSubCmdID);
}
//发送函数
bool GameBase::SendSocketData( word wSubCmdID, void * data, word dataSize)
{
	return IServerItem::get()->SendSocketData(MDM_GF_GAME,wSubCmdID,data,dataSize);
}
