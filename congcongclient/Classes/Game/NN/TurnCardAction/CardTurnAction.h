#ifndef _CARD_TURN_ACTON_H_
#define _CARD_TURN_ACTON_H_

#include <cocos2d.h>
#include "CardTurnOption.h"

using namespace cocos2d;

namespace cocos2d {
    class Node;
    class EventDispatcher;
}

namespace cardturn {

class CardBackTurn;
class CardFrontTurn;
class CardMidTurn;
class TouchMathine;

class CardTurnAction : public cocos2d::Node
{
public:

    CardTurnAction();

    virtual ~CardTurnAction();

public:

    virtual void init(cocos2d::Node* pNode,
        std::string backMaterialPath,
        std::string frontMaterialPath,
		std::string frontMaterialPathEnd,
		std::string cylinderMaterialPath,
        Vec2 basePos,
        float scaleSize,
		cardTurnCompleteFunc* completeFuc = nullptr);

    virtual void setTouchEvent(cocos2d::EventDispatcher* evtDispatcher, cocos2d::Node* pNode);

    virtual void update(float fTime);

protected:

	bool onTouchBegin(Touch* touch, Event* ev);
	void onTouchMove(Touch* touch, Event* ev);
	void onTouchEnd(Touch* touch, Event* ev);
	void onTouchCancel(Touch* touch, Event* ev);

protected:

    CardBackTurn* cardBKTurn;
    CardFrontTurn* cardFTTurn;
	CardMidTurn* cardMDTurn;

	//////////////////////////////////////////////////////////////////////////
	//相机
	Camera* _camera;

	//////////////////////////////////////////////////////////////////////////
	TouchMathine* _touch;
	//消息队列
	EventDispatcher* _evtDispatcher;
	//////////////////////////////////////////////////////////////////////////

	cardTurnCompleteFunc* _onComplete;
};

}


#endif