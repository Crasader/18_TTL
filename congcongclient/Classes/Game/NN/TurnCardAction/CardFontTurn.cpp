#include "CardFrontTurn.h"
#include "CardTurnOption.h"
#include "CardTurnTouch.h"

using namespace cardturn;
using namespace cocos2d;

CardFrontTurn::CardFrontTurn()
{
    
}

CardFrontTurn::~CardFrontTurn()
{
    if (_sp3DCardFront)
		_sp3DCardFront->release();
    if (_pDrawNodeCard)
        _pDrawNodeCard->release();
}

CardFrontTurn::CardFrontTurn(Node* scene,
	TouchMathine* state,
	std::string imgPath,
	std::string imgPathEnd,
    Vec2 basePos,
    float scaleSize)
    : _pScene(nullptr)
	, _touch(state)
    , _imagePath(imgPath)
	, _imagePathEnd(imgPathEnd)
    , _basePos(basePos)
	, _clipColor(1, 0, 1, 0.2)
    , _offsetPos(0, 0)
    , _RealPos(0, 0)
    , _scaleSize(scaleSize)
    , _cp_moved_offset(0.0f)
    , _useTime(0.0f)
	, _onComplete(nullptr)
{
    memset(_cpCardRect, 0, sizeof(_cpCardRect));
    _pScene = scene;
}

//////////////////////////////////////////////////////////////////////////

#define hand_code_adjust_cf_1 35.f

void CardFrontTurn::init()
{
	try
	{
		cocos2d::log("CardTurnAction::CF Init Start!");
		CCASSERT(_pScene, "Initalize card back sence failed.");
		//Rect card;
		switch (_touch->getDirection())
		{
			case CTD_Boom_To_Up:
				_cpCard = ClippingNode::create();
				cocos2d::log("CardTurnAction::FT create ,anc x = %f, anc y = %f\n",
					_cpCard->getAnchorPoint().x,
					_cpCard->getAnchorPoint().y);
				_cpCard->setAnchorPoint(Vec2(0.5,0.5));

				_sp3DCardFront = Sprite3D::create("TTLNN/CardTurn/card.c3b");
				_mat = Sprite3DMaterial::createWithFilename(_imagePath);
				_sp3DCardFront->setMaterial(_mat);
				_CardRect = _sp3DCardFront->getBoundingBox();
				_sp3DCardFront->setScale(_scaleSize);
				_sp3DCardFront->setRotation(90);
				//旋转之后要调整位置,并且与锚点无关,坑
				_sp3DCardFront->setPosition3D(cocos2d::Vec3(-_CardRect.size.width * _scaleSize / 2, _CardRect.size.height * _scaleSize / 2, -4));
				_sp3DCardFront->setForce2DQueue(true);

				ceateClipCardShape(_touch->getDirection());
				_pDrawNodeCard = DrawNode::create();
				_pDrawNodeCard->drawPolygon(_cpCardRect, CLIP_VERTEXT_SIZE, _clipColor, 0, _clipColor);
				_fRadius = _CardRect.size.width / (2 * PI);

				_clipBasePos = _pDrawNodeCard->getPosition();
				_clipBasePos.y += _CardRect.size.height + _fRadius;
				_pDrawNodeCard->setPosition(Vec2(_clipBasePos.x, _clipBasePos.y));

				_cpCard->setScaleY(CARD_PROPOTION);
				_cardBasePos = _basePos;
				_cardBasePos.y -= _CardRect.size.height * CARD_PROPOTION;
				_cardBasePos.y +=  _fRadius * ( 2 * CARD_PROPOTION - 1);
				_cardBasePos.y += hand_code_adjust_cf_1;
				_cpCard->setPosition(_cardBasePos.x, _cardBasePos.y);

				_cpCard->setStencil(_pDrawNodeCard);
				_cpCard->addChild(_sp3DCardFront);

				CCASSERT(_pScene, "Initalize card front sence failed.");
				_pScene->addChild(_cpCard);

				//_cpCard->setInverted(true);
				//_pScene->addChild(_pDrawNodeCard);
				//_cpCard->setVisible(false);
				break;
			case CTD_BoomRight_To_TopLeft:
				break;
			} 
		} catch (...) {

		}
}

void CardFrontTurn::setOnCompleteFun(cardTurnCompleteFunc* onCompleteFun)
{
	_onComplete = onCompleteFun;
}

#define  hand_code_adjust_1 1.2f
#define hand_code_adjust_2 8.f

void CardFrontTurn::doMove(float fmove)
{
	//cocos2d::log("CardFrontTurn fmove = %f\n", fmove);
	switch (_touch->getDirection()) {
		case CTD_BoomRight_To_TopLeft:
			break;
		case CTD_Boom_To_Up:
		{
			if (fmove <= _CardRect.size.height + _fRadius) {
				if (fmove <_fRadius) {//等待圆筒卷牌
					_pDrawNodeCard->setPosition(Vec2(_clipBasePos.x, _clipBasePos.y));
					_cpCard->setPosition(_cardBasePos.x, _cardBasePos.y);
				} else if(fmove > _fRadius) {
					if (fmove > _fRadius + 120) {
						CCLOG("FrontTurn 3333");
						_cpCardRect[0].y = _CardRect.getMinY() + hand_code_adjust_2;
						_cpCardRect[1].y = _CardRect.getMinY() + hand_code_adjust_2;
						_cpCardRect[2].y = _CardRect.getMinY() + fmove - _fRadius;
						_cpCardRect[3].y = _CardRect.getMinY() + fmove - _fRadius;
					} else if (fmove > _fRadius + 20) {
						CCLOG("FrontTurn 2222");
						_cpCardRect[0].y = _CardRect.getMinY() + hand_code_adjust_1;
						_cpCardRect[1].y = _CardRect.getMinY() + hand_code_adjust_1;
						_cpCardRect[2].y = _CardRect.getMinY() + fmove - _fRadius;
						_cpCardRect[3].y = _CardRect.getMinY() + fmove - _fRadius;
					} else {
						CCLOG("FrontTurn 1111");
						_cpCardRect[2].y = _CardRect.getMinY() + hand_code_adjust_1;
						_cpCardRect[3].y = _CardRect.getMinY() + hand_code_adjust_1;
					}
					_pDrawNodeCard->clear();
					_pDrawNodeCard->drawPolygon(_cpCardRect, CLIP_VERTEXT_SIZE, _clipColor, 0, _clipColor);
					_pDrawNodeCard->setPosition(Vec2(_clipBasePos.x, _clipBasePos.y - fmove));
					_cpCard->setPosition(_cardBasePos.x, _cardBasePos.y + (_cp_moved_offset + 2 * fmove) * CARD_PROPOTION);
				}
			} 
			break;
		}
	}
}

void CardFrontTurn::update(float tt)
{
	//_useTime += 0.1f;
	//doMove(_useTime);

	switch (_touch->getState()) {
	case cardturn::CTAS_Null:
		break;
	case cardturn::CTAS_Touched:
		break;
	case cardturn::CTAS_Moved:
		break;
	case cardturn::CTAS_Reset: 
		{
			if (_touch->getMovedTotalDistance() > 0) {
				if (_touch->getMovedTotalDistance() > _touch->getRollBackStepValue())
					doMove(_touch->getMovedTotalDistance() - _touch->getRollBackStepValue());
				else
					doMove(0);
				_touch->goBack();
			} else {
				_touch->setState(CTAS_Null);
			}
			break;
		}
	case cardturn::CTAS_ForceComplete:
		doMove(_touch->getMovedTotalDistance());
		_cp_moved_offset -= _touch->getMovedStepDistance() * (_CardRect.size.width + _fRadius) / (_CardRect.size.width / 2);
		break;
	case cardturn::CTAS_Complete: 
		{
			if (_onComplete) {
				_mat = Sprite3DMaterial::createWithFilename(_imagePathEnd);
				_sp3DCardFront->setMaterial(_mat);
				_onComplete(nullptr);
				_onComplete=nullptr;
				
			}
			break;
		}
	case cardturn::CTAS_End: 
		{
			if (_onComplete) {
				_mat = Sprite3DMaterial::createWithFilename(_imagePathEnd);
				_sp3DCardFront->setMaterial(_mat);
				_onComplete(nullptr);
				_onComplete=nullptr;
			}
			break;
		}
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

void CardFrontTurn::ceateClipCardShape(CardTurn_Direction direction)
{
	switch (direction) {
	case CTD_Boom_To_Up:
		_CardRect = _sp3DCardFront->getBoundingBox();
		_cpCardRect[0] = Vec2(_CardRect.getMinX() - 1, _CardRect.getMinY() + hand_code_adjust_1);
		_cpCardRect[1] = Vec2(_CardRect.getMaxX() + 1, _CardRect.getMinY() + hand_code_adjust_1);
		_cpCardRect[2] = Vec2(_CardRect.getMaxX() - 1, _CardRect.getMinY() + hand_code_adjust_1);
		_cpCardRect[3] = Vec2(_CardRect.getMinX() + 1, _CardRect.getMinY() + hand_code_adjust_1);
		break;
	case CTD_BoomRight_To_TopLeft:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

bool CardFrontTurn::onTouchBegin(Touch* touch, Event* ev)
{
    return true;
}

void CardFrontTurn::onTouchMove(Touch* touch, Event* ev)
{
	switch (_touch->getState()) {
		case CTAS_Touched:
			{
				break;
			}
		case CTAS_Moved:
			{
				float distance = _touch->getMovedTotalDistance();
				doMove(distance < 0 ? 0 : distance);
				break;
			}
	}
}

void CardFrontTurn::onTouchEnd(Touch* touch, Event* ev)
{
	switch (_touch->getState()) {
		case	CTAS_Touched:
		case CTAS_Moved:
			_touch->setState(CTAS_Reset);
			break;
	}
}

void CardFrontTurn::onTouchCancel(Touch* touch, Event* ev)
{
	switch (_touch->getState()) {
		case	CTAS_Touched:
		case CTAS_Moved:
			_touch->setState(CTAS_Reset);
			break;
	}
}
