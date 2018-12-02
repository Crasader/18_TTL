#include "GPSceneManager.h"
#include "LoginScene/GPLoginScene.h"
#include "HomeScene/GPHomeScene.h"
#include "ClientHN_THJ/Game/TTLNN/Calculate/NNCalculate.h"

FV_SINGLETON_STORAGE(GPSceneManager);

GPSceneManager::GPSceneManager()
	: m_bInBackground(false)
	, m_iScenceTimeID(0)
{
	setInstance(this);
}

GPSceneManager::~GPSceneManager()
{
}

void GPSceneManager::setParent(CCNode* pNode)
{
	_pRootNode = cocos2d::Node::create();
	pNode->addChild(_pRootNode);
}

cocos2d::Node* GPSceneManager::getRootNode()
{
	return _pRootNode;
}

void GPSceneManager::GameBackScence()
{
	InHomeScene();
}

void GPSceneManager::HideAll()
{
	GPGameManager::Instance().hideAll();
	GPLoginScene::Instance().setVisible(false);
	//GPHomeScene::Instance().removeAllPanels();
	GPHomeScene::Instance().setVisible(false);
    GPHomeScene::Instance().stopAllActions();
}

void GPSceneManager::init()
{
	//DONE:必须先初始化游戏, 因为游戏会抢夺两个大厅面板
	GPGameManager::Instance().init();

	addGameNode(GPLoginScene::pInstance());
	addGameNode(GPHomeScene::pInstance());

	_pRootNode->addChild(PopScence::pInstance());
	_pRootNode->addChild(EffectScence::pInstance());
	_pRootNode->addChild(NoticeMsg::pInstance());
	_pRootNode->addChild(NoticeMsgBox::pInstance());
	NoticeMsgBox::Instance().hide();

	InLogonScene();
}

void GPSceneManager::addGameNode(cocos2d::Node* pNode)
{
	_pRootNode->addChild(pNode, -1);
}

void GPSceneManager::InLogonScene()
{
	HideAll();
	GPLoginScene::Instance().setVisible(true);
	GPLoginScene::Instance().EnterScene();
}

void GPSceneManager::InHomeScene()
{
	HideAll();
	GPHomeScene::Instance().setVisible(true);
	GPHomeScene::Instance().onEnterScene();
}

void GPSceneManager::InHomeExScene(int gameKindID)
{
	HideAll();
	//GPHomeExScene::Instance().show(gameKindID);
}

Node* GPSceneManager::createCircleAvatar(cocos2d::Node* pRootNode, const std::string& filename, const std::string& stencilPath, const Vec2& position)
{
	auto pClip = ClippingNode::create();
	auto sprite = Sprite::create(filename);
	auto viewSize = sprite->getContentSize();
	auto stencil = Sprite::create(stencilPath);
	auto stSize = stencil->getContentSize();

	if (viewSize.height < stSize.height || viewSize.width < stSize.width) {
		auto scaleHeight = stSize.height * 0.76f / viewSize.height;
		auto scaleWidth = stSize.width * 0.76f / viewSize.width;
		auto targetScale = utility::Max(scaleWidth, scaleHeight);
		sprite->setScale(targetScale);
	}else{
		auto scaleHeight = viewSize.height * 0.76f / stSize.height;
		auto scaleWidth  = viewSize.width * 0.76f / stSize.width;
		auto targetScale = utility::Min(scaleWidth, scaleHeight);
		sprite->setScale(targetScale);
	}
	stencil->setScale(0.83f,0.83f);
	pClip->setAlphaThreshold(.7f);
	pClip->setStencil(stencil);
	pClip->addChild(sprite);
	pClip->setInverted(false);
	pRootNode->addChild(pClip);
	pClip->setPosition(position);
	return sprite;
}

void GPSceneManager::InNNCalculate(CMD_GF_Private_End_Info* pInfo)
{
	HideAll();
}

void GPSceneManager::InSGCalculate(CMD_GF_Private_End_Info* pInfo)
{
	HideAll();
}
