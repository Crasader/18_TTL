#include "CardTurnAction.h"
#include "CardBackTurn.h"
#include "CardFrontTurn.h"
#include "CardMidTurn.h"
#include "CardTurnTouch.h"
#include "GameLib/Game/Script/WindowInfo.h"

USING_NS_CC;

using namespace cardturn;

CardTurnAction::CardTurnAction()
    : cardBKTurn (nullptr)
    , cardFTTurn (nullptr)
	, cardMDTurn (nullptr)
{

}

CardTurnAction::~CardTurnAction()
{
    if(cardBKTurn)
        delete cardBKTurn;

    if (cardFTTurn)
        delete cardFTTurn;

	if (cardMDTurn)
		delete cardMDTurn;

	if (_touch)
		delete _touch;

	if (_evtDispatcher)
		_evtDispatcher->removeAllEventListeners();
}

void CardTurnAction::init(cocos2d::Node* pSenceNode, 
	std::string backMaterialPath,
	std::string frontMaterialPath,
	std::string frontMaterialPathEnd,
	std::string cylinderMaterialPath,
    Vec2 basePos, 
    float scaleSize,
	cardTurnCompleteFunc* completeFuc)
{
	_touch = new TouchMathine();
	_touch->setDirection(CTD_Boom_To_Up);
	_touch->setState(CTAS_Null);
	_touch->setCompleteAction(CTCAT_ForceEndOverHalf);

	_onComplete = completeFuc;

	Size visibleSize = WindowInfo::Instance().getUseSize(); //Director::getInstance()->getVisibleSize();
	//_camera = Camera::createPerspective(90, visibleSize.width / visibleSize.height, 0.1, 100);
	//_camera->setCameraFlag(CameraFlag::USER1);
	//Vec3 pos = _camera->getPosition3D();
	//pos.y = 0;
	//_camera->setPosition3D(pos);
	//pSenceNode->addChild(_camera);
	pSenceNode->setCameraMask(2);
	
	cocos2d::log("CardTurnAction::before BK create\n");
    cardBKTurn = new CardBackTurn(pSenceNode, _touch, backMaterialPath, basePos, scaleSize);
	cocos2d::log("CardTurnAction::before FT create\n");
    cardFTTurn = new CardFrontTurn(pSenceNode, _touch, frontMaterialPath,frontMaterialPathEnd, basePos, scaleSize);
	cocos2d::log("CardTurnAction::before MD create\n");
	cardMDTurn = new CardMidTurn(pSenceNode, _touch, cylinderMaterialPath, basePos, scaleSize);

	cocos2d::log("CardTurnAction::before BK init\n");
	cardBKTurn->init();
	cocos2d::log("CardTurnAction::before MD init\n");
	cardMDTurn->init();
	cocos2d::log("CardTurnAction::before FK init\n");
	cardFTTurn->init();

	cocos2d::log("setOnCompleteFun::before FK init\n");
	cardFTTurn->setOnCompleteFun(_onComplete);
	
	pSenceNode->addChild(this);

}

void CardTurnAction::update(float fTime)
{

	cardBKTurn->update(fTime);
	cardMDTurn->update(fTime);
	cardFTTurn->update(fTime);

}

//////////////////////////////////////////////////////////////////////////

void CardTurnAction::setTouchEvent(EventDispatcher* evtDispatcher, cocos2d::Node* pNode)
{
	CCASSERT(evtDispatcher, "card turn EventDispatcher is null.");

	_evtDispatcher = evtDispatcher;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CardTurnAction::onTouchBegin, this);
	listener->onTouchMoved = CC_CALLBACK_2(CardTurnAction::onTouchMove, this);
	listener->onTouchEnded = CC_CALLBACK_2(CardTurnAction::onTouchEnd, this);
	listener->onTouchCancelled = CC_CALLBACK_2(CardTurnAction::onTouchCancel, this);

	_evtDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool CardTurnAction::onTouchBegin(Touch* touch, Event* ev)
{
	cardBKTurn->onTouchBegin(touch, ev);
	cardMDTurn->onTouchBegin(touch, ev);
	cardFTTurn->onTouchBegin(touch, ev);
	return true;
}

void CardTurnAction::onTouchMove(Touch* touch, Event* ev)
{
	cardBKTurn->onTouchMove(touch, ev);
	cardMDTurn->onTouchMove(touch, ev);
	cardFTTurn->onTouchMove(touch, ev);
}

void CardTurnAction::onTouchEnd(Touch* touch, Event* ev)
{
	cardBKTurn->onTouchEnd(touch, ev);
	cardMDTurn->onTouchEnd(touch, ev);
	cardFTTurn->onTouchEnd(touch, ev);
}

void CardTurnAction::onTouchCancel(Touch* touch, Event* ev)
{
	cardBKTurn->onTouchCancel(touch, ev);
	cardMDTurn->onTouchCancel(touch, ev);
	cardFTTurn->onTouchCancel(touch, ev);
}
