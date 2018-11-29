#include "NNGameScene_Touch.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNGameScene.h"

bool NNGameScene_Touch::init()
{
	if (!cocos2d::Layer::init()) {
		return false;
	}
	setTouchEnabled(true);
	setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
	return true;
}

bool NNGameScene_Touch::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	if (!utility::isAllVisible(this)) {
		return false;
	}
	if (pTouch->getID() != 0) {
		return false;
	}
	return NNGameScene::Instance().ccTouchBegan(pTouch->getLocation());
}

void NNGameScene_Touch::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}

void NNGameScene_Touch::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}
