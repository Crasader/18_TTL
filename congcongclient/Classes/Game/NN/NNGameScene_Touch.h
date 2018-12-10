#pragma once

#include "common.h"

class NNGameScene_Touch
	:public cocos2d::Layer
{
public:
	virtual bool init();
public:
	CREATE_FUNC(NNGameScene_Touch);
public:
	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
};