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
	//DONE:�����ȳ�ʼ����Ϸ, ��Ϊ��Ϸ�����������������
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

Node* GPSceneManager::getCircleAvatar(cocos2d::Node* pRootNode, const std::string& filename, const Size& targetSize,  const Vec2& position)
{
	auto pClip = ClippingNode::create();
	auto sprite = Sprite::create(filename);
	auto viewSize = sprite->getContentSize();
	if (viewSize.height < targetSize.height || viewSize.width < targetSize.width) {
		auto scaleHeight = targetSize.height * 1.f / viewSize.height;
		auto scaleWidth = targetSize.width * 1.f / viewSize.width;
		auto targetScale = utility::Max(scaleWidth, scaleHeight);
		sprite->setScale(targetScale);
	}else{
		auto scaleHeight = viewSize.height * 1.f / targetSize.height;
		auto scaleWidth  = viewSize.width * 1.f / targetSize.width;
		auto targetScale = utility::Min(scaleWidth, scaleHeight);
		sprite->setScale(targetScale);
	}
	auto stencil = Sprite::create(filename);
	//stencil->setScale(1.1f,1.1f);
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
