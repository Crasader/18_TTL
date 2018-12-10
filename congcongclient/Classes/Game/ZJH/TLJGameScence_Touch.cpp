#include "TLJGameScence.h"
#include "GAME.h"
#include "TLJGameScence_Touch.h"
#include UTILITY_WIDGET


bool TLJTouchScence::init()
{
	if (!cocos2d::Layer::init()) {
		return false;
	}
	setTouchEnabled(true);
	setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
	return true;
}

bool TLJTouchScence::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	if (!utility::isAllVisible(this)) {
		return false;
	}
	if (pTouch->getID() != 0) {
		return false;
	}
	return TLJGameScence::Instance().ccTouchBegan(pTouch->getLocation());
}

void TLJTouchScence::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	if (!utility::isAllVisible(this)) {
		return;
	}
	if (pTouch->getID() != 0) {
		return;
	}
	return TLJGameScence::Instance().ccTouchMoved(pTouch->getLocation(),pTouch->getPreviousLocation());
}

void TLJTouchScence::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	if (!utility::isAllVisible(this)) {
		return;
	}
	if (pTouch->getID() != 0) {
		return;
	}
	return TLJGameScence::Instance().ccTouchEnded(pTouch->getLocation());
}
