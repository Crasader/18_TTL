#include "GPLoadingScene.h"
#include "Game/Script/ScriptData.h"
#include "Game/Script/WindowInfo.h"
#include "Game/Script/ScriptXMLparse.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "../GPSceneManager.h"

USING_NS_CC;
using namespace experimental;

CCScene* GPLoadingScene::scene()
{
    CCScene *scene = CCScene::create();
    GPLoadingScene *layer = GPLoadingScene::create();
    scene->addChild(layer);
    return scene;
}

bool GPLoadingScene::init()
{
    Size visible = Director::getInstance()->getVisibleSize();
    Size win = Director::getInstance()->getWinSize();
	//TODO:这张logo图永远都在内存中,要把它干掉
    Sprite* logo = Sprite::create("GamePlaza/LoadingScene/logo.png");
    this->addChild(logo);
    logo->setScaleX((float)((float)visible.width) / 1280);
    logo->setScaleY((float)((float)visible.height) / 720);
    logo->setAnchorPoint(Vec2(0, 0));
    logo->setPosition(Vec2(0, 0));
    
	DelayTime* delay = DelayTime::create(1.5f);
    Hide* hide = Hide::create();
    CallFunc* func = CallFunc::create([ = ] {
        init_2();
    });
    Sequence* seq = Sequence::create(delay, hide, func, NULL);
    logo->runAction(seq);
    
	return true;
}

bool GPLoadingScene::init_2()
{
    if(!CCLayer::init()) {
        return false;
    }

	WidgetSkinXMLparse kSkinXml1("Script/HNWidgetSkin.xml");
	cocos2d::ScriptXMLparse kScriptXml1("Script/HNScriptValue.xml");
	cocos2d::ScriptXMLparse kScriptXml2("Script/HNScriptValueStr.xml");
	//cocos2d::ScriptXMLparse kScriptXml3("Script/HNScriptValueSame.xml");

    this->scheduleUpdate();
    this->setTouchEnabled(true);
    this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);

	GPSceneManager::Instance().setParent(this);
	WindowInfo::Instance().changWinSize(GPSceneManager::Instance().getRootNode());

	GPSceneManager::pInstance()->init();

    return true;
}

void GPLoadingScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
    if(touches.size() == 0) {
        return;
    }

}
void GPLoadingScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
    if(touches.size() == 0) {
        return;
    }
}
void GPLoadingScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
    if(touches.size() == 0) {
        return;
    }
}
void GPLoadingScene::onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event)
{
    if(touches.size() == 0) {
        return;
    }
}
void GPLoadingScene::update(float delta)
{
}
void GPLoadingScene::onExit()
{
    cocos2d::Layer::onExit();
}
void GPLoadingScene::exitRoom()
{
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
