#include "GPGameManager.h"

#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNGameScene.h"
#include "ClientHN_THJ/Game/MENJI/TLJGameScence.h"
#include "ClientHN_THJ/Game/SRLF/SRLFGameScence.h"
#include "ClientHN_THJ/Game/WZQ/WZQDGameScence.h"
#include "ClientHN_THJ/Game/SG/GameScene/SGGameScence.h"
#include "ClientHN_THJ/Game/XZDD/XZDDGameScence.h"
#include "GameLib/Game/FV/FvSingleton.h"

FV_SINGLETON_STORAGE(GPGameManager)

GPGameManager::GPGameManager()
	: _bCreated_NN(false)
{
	setInstance(this);
	pGameMan->setPrivateServerSink(GPGameLink::pInstance());
}

GPGameManager::~GPGameManager()
{
}

void GPGameManager::init()
{
	GPSceneManager::Instance().addGameNode(NNGameScene::pInstance());

	return;
	//DONE:后面的都不用初始化了,省资源
	//////////////////////////////////////////////////////////////////////////

	GPSceneManager::Instance().addGameNode(SRLFGameScence::pInstance());
	GPSceneManager::Instance().addGameNode(TLJGameScence::pInstance());
	GPSceneManager::Instance().addGameNode(WZQDGameScence::pInstance());
	GPSceneManager::Instance().addGameNode(SGGameScence::pInstance());
	GPSceneManager::Instance().addGameNode(XZDDGameScence::pInstance());
}

void GPGameManager::hideAll()
{

	NNGameScene::Instance().hide();
	NNGameScene::Instance().stopAllActions();

	return;

	//DONE:后面的都不用初始化了,省资源
	//////////////////////////////////////////////////////////////////////////

	SRLFGameScence::Instance().setVisible(false);
    SRLFGameScence::Instance().stopAllActions();

	TLJGameScence::Instance().setVisible(false);
    TLJGameScence::Instance().stopAllActions();

	WZQDGameScence::Instance().setVisible(false);
    WZQDGameScence::Instance().stopAllActions();

	SGGameScence::Instance().hide();
    SGGameScence::Instance().stopAllActions();

	XZDDGameScence::Instance().setVisible(false);
	XZDDGameScence::Instance().stopAllActions();
}

IClientKernelSink* GPGameManager::CreateGame()
{
    GPSceneManager::Instance().HideAll();
	GameManagerBase::setJoinWithRoomNO(0);
	switch(_wCurGameID)
	{
	case NNGameScene::KIND_ID:
		{
			NNGameScene::Instance().enterScene();
			setInCenter(false);
			return NNGameScene::pInstance();
		}
	case SRLFGameScence::KIND_ID:
		{
			SRLFGameScence::Instance().EnterScence();
			setInCenter(false);
			return SRLFGameScence::pInstance();
		}
	case TLJGameScence::KIND_ID:
		{
			TLJGameScence::Instance().EnterScence();
			setInCenter(false);
			return TLJGameScence::pInstance();
		}
	case WZQDGameScence::KIND_ID:
		{
			WZQDGameScence::Instance().EnterScence();
			setInCenter(false);
			return WZQDGameScence::pInstance();
		}
	case SGGameScence::KIND_ID:
		{
			SGGameScence::Instance().enterScene();
			setInCenter(false);
			return SGGameScence::pInstance();
		}

	case XZDDGameScence::KIND_ID:
		{
			XZDDGameScence::Instance().EnterScence();
			setInCenter(false);
			return XZDDGameScence::pInstance();
		}
	}
    return NULL;
}

void GPGameManager::loadGameBaseData(word wKindID)
{
	if (NNGameScene::KIND_ID == wKindID) {
		DF::shared()->init(wKindID, NNGameScene::MAX_PLAYER, NNGameScene::VERSION_CLIENT, "Game");
	}

	if (SRLFGameScence::KIND_ID == wKindID) {
		DF::shared()->init(wKindID, SRLFGameScence::MAX_PLAYER, SRLFGameScence::VERSION_CLIENT, "Game");
	}
		
	if (TLJGameScence::KIND_ID == wKindID) {
		DF::shared()->init(wKindID, TLJGameScence::MAX_PLAYER, TLJGameScence::VERSION_CLIENT, "Game");
	}

	if (WZQDGameScence::KIND_ID == wKindID) {
		DF::shared()->init(wKindID, WZQDGameScence::MAX_PLAYER, WZQDGameScence::VERSION_CLIENT, "Game");
	}

	if (SGGameScence::KIND_ID == wKindID) {
		DF::shared()->init(wKindID, SGGameScence::MAX_PLAYER, SGGameScence::VERSION_CLIENT, "Game");
	}
}

void GPGameManager::StartHNMJRecord(datastream& kDataStream)
{
    GPSceneManager::Instance().HideAll();
}
