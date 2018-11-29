#include "TurnCardNode.h"

USING_NS_CC;

using namespace cardturn;
CardTurnAction* cardTurnACT;

bool TurnCardNode::init()
{
	if (!cocos2d::Node::init())
	{
		 return false;
	}
	//_eventDispatcher->setEnabled(true);
	//scheduleUpdate();
	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//cardTurnACT = new CardTurnAction();

	//cardTurnACT->init(this,
	//	std::string("TTLNN/CardTurn/poker_0.material"),
	//	std::string("TTLNN/CardTurn/poker_1.material"),
	//	std::string("TTLNN/CardTurn/poker_1_cylinder.material"),
	//	Vec2(visibleSize.width/2+200, visibleSize.height/2+128),
	//	8,
	//	glTurnCardFinish);
	//cardTurnACT->setTouchEvent(_eventDispatcher, this);
    return true;
}

void TurnCardNode::setTurnCardData(std::string backMaterialPath,
									std::string frontMaterialPath,
									std::string frontMaterialPathEnd,
									std::string cylinderMaterialPath,
									Vec2 basePos, 
									float scaleSize,
									cardTurnCompleteFunc* completeFuc)
{
	_eventDispatcher->setEnabled(true);
	scheduleUpdate();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	cardTurnACT = new CardTurnAction();

	cocos2d::log("CardTurnAction::layer create ,anc x = %f, anc y = %f\n",
		this->getAnchorPoint().x,
		this->getAnchorPoint().y);
	this->setAnchorPoint(Vec2(0.5,0.5));

	cardTurnACT->init(this,
		backMaterialPath,
		frontMaterialPath,
		frontMaterialPathEnd,
		cylinderMaterialPath,
		basePos,
		scaleSize,
		completeFuc);
	cardTurnACT->setTouchEvent(_eventDispatcher, this);
}

void TurnCardNode::setTurnCardPrc( 
	std::string backMaterialPath,
	std::string frontMaterialPath,
	std::string cylinderMaterialPath)
{
	
}

void TurnCardNode::update(float fTime)
{
	cardTurnACT->update(fTime);
}


