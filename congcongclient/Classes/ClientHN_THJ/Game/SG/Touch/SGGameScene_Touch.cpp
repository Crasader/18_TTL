#include "SGGameScene_Touch.h"
#include "ClientHN_THJ/Game/SG/GameScene/SGGameScence.h"

bool SGGameScene_Touch::init()
{
	if (!cocos2d::Layer::init()) {
		return false;
	}
	setTouchEnabled(true);
	setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
	return true;
}

bool SGGameScene_Touch::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	if (!utility::isAllVisible(this)) {
		return false;
	}
	if (pTouch->getID() != 0) {
		return false;
	}
	return SGGameScence::Instance().ccTouchBegan(pTouch->getLocation());
}

void SGGameScene_Touch::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}

void SGGameScene_Touch::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}
