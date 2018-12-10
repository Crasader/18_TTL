#include "CardBackTurn.h"
#include "CardTurnTouch.h"

using namespace cardturn;

CardBackTurn::CardBackTurn()
{
    
}

CardBackTurn::~CardBackTurn()
{
    if (_cardBack)
		_cardBack->release();

    if (_pDrawNode)
        _pDrawNode->release();
}

CardBackTurn::CardBackTurn(Node* scene, TouchMathine* state, std::string imgPath, Vec2 basePos, float scaleSize)
    : _pScene(nullptr)
	, _touch(state)
    , _materialPath(imgPath)
    , _basePos(basePos)
    , _clipColor(1, 0, 0, 1)
    , _scaleSize(scaleSize)
    , _cp_moved_offset(0.0f)
    , _useTime(0.0f)
{
    memset(_clipRect, 0, sizeof(_clipRect));
    _pScene = scene;
}

//////////////////////////////////////////////////////////////////////////

void CardBackTurn::init()
{
	switch (_touch->getDirection()) {
		case CTD_Boom_To_Up:
			{

				_pClipper = ClippingNode::create();

				cocos2d::log("CardTurnAction::BK create ,anc x = %f, anc y = %f\n",
					_pClipper->getAnchorPoint().x,
					_pClipper->getAnchorPoint().y);
				_pClipper->setAnchorPoint(Vec2(0.5,0.5));
				_pClipper->setPosition(_basePos);

				_cardBack = Sprite3D::create("TTLNN/CardTurn/card.c3b");
				_mat = Sprite3DMaterial::createWithFilename(_materialPath);
				_cardBack->setMaterial(_mat);
				_cardBack->setScale(_scaleSize);

				_CardRect = _cardBack->getBoundingBox();
				_fRadius = _CardRect.size.width / (2 * PI);

				//向后移动圆柱体的直径+牌背和牌正面的宽度
				float diameter = _CardRect.size.width / PI + 2;
				_cardBack->setPosition3D(Vec3(0.0f, 2.0f, -diameter));
				_cardBack->setForce2DQueue(true);

				ceateClipShape(_touch->getDirection());
				_pDrawNode = DrawNode::create();
				_pDrawNode->drawPolygon(_clipRect, CLIP_VERTEXT_SIZE, _clipColor, 0, _clipColor);
				_clipBasePos = _cardBack->getPosition();

				_pClipper->setStencil(_pDrawNode);
				_pClipper->addChild(_cardBack);
				_pClipper->setScaleY(CARD_PROPOTION);

				//Rect trigerRect = Rect(_basePos.x-10 , _basePos.y-10, _CardRect.size.width+20, 20);
				//Rect trigerRect = Rect(_basePos.x-_CardRect.size.width-20 , _basePos.y-_CardRect.size.height, _CardRect.size.width+20, 20);
				auto sSize =  Director::getInstance()->getWinSize(); 
				Rect trigerRect = Rect(0,0,sSize.width,sSize.height);
				_touch->setTouchBeginRect(trigerRect);
				//Rect trigerRect1 = Rect(0 ,0,  sSize.width, sSize.height);
				//CCLayer *layer = CCLayerColor::create(ccc4(0xff, 0x00, 0x00, 0x80), sSize.width, sSize.height);  
				//layer->ignoreAnchorPointForPosition(false);  
				//layer->setPosition( trigerRect1.getMidX(), trigerRect1.getMidY());  
				//_pScene->addChild(layer, 1, layer->getTag());  
				
				CCASSERT(_pScene, "Initalize card back sence failed.");
				_pScene->addChild(_pClipper);

				break;
			}
		case CTD_BoomRight_To_TopLeft:
			{
				break;
			}
	}

}

void cardturn::CardBackTurn::doMove(float fmove)
{
	//cocos2d::log("CardBackTurn fmove = %f\n", fmove);
	switch (_touch->getDirection()) {
		case CTD_BoomRight_To_TopLeft:
			{
				break;
			}
		case CTD_Boom_To_Up:
			{
				if (fmove <= _CardRect.size.height) {
					_pDrawNode->setPosition(Vec2(_clipBasePos.x, _clipBasePos.y + fmove));
					_pClipper->setPosition(Vec2(_basePos.x, _basePos.y + _cp_moved_offset * CARD_PROPOTION));
				} 
				break;
			}
	}
}

void CardBackTurn::update(float tt)
{
     //_useTime += 0.1f;
     //doMove(_useTime);
	float cur_distance = 0;
	switch (_touch->getState())
	{
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
			} else {
				_touch->setState(CTAS_Null);
			}
			break;
		}
	case cardturn::CTAS_ForceComplete:
		cur_distance = _touch->getMovedTotalDistance();
		_touch->setMoveDistance(cur_distance + _touch->getForceCompleteStepValue());
		doMove(_touch->getMovedTotalDistance());
		_cp_moved_offset -= _touch->getMovedStepDistance() * (_CardRect.size.width + _fRadius) / (_CardRect.size.width / 2);
		if (_touch->getMovedStepDistance() == 0.0f) {
			_touch->setState(CTAS_Complete);
		}
		break;
	case cardturn::CTAS_Complete:
		break;
	case cardturn::CTAS_End:
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

void CardBackTurn::ceateClipShape(CardTurn_Direction cdt_direction)
{
	switch (cdt_direction) {
		case CTD_Boom_To_Up:
			_CardRect = _cardBack->getBoundingBox();
			_clipRect[0] = Vec2(_CardRect.getMinX(), _CardRect.getMinY());
			_clipRect[1] = Vec2(_CardRect.getMaxX(), _CardRect.getMinY());
			_clipRect[2] = Vec2(_CardRect.getMaxX(), _CardRect.getMaxY());
			_clipRect[3] = Vec2(_CardRect.getMinX(), _CardRect.getMaxY());
			break;
		case CTD_BoomRight_To_TopLeft:
			//裁剪图形为平行四边形
			_CardRect = _cardBack->getBoundingBox();
			_clipRect[0] = Vec2(_CardRect.getMinX(), _CardRect.getMaxY());
			_clipRect[1] = Vec2(_CardRect.getMinX() + _CardRect.size.height + _CardRect.size.width, _CardRect.getMaxY());
			_clipRect[2] = Vec2(_CardRect.getMinX() + _CardRect.size.width, _CardRect.getMinY());
			_clipRect[3] = Vec2(_CardRect.getMinX(), _CardRect.getMinY());
			break;
	}

}

//////////////////////////////////////////////////////////////////////////

bool CardBackTurn::onTouchBegin(Touch* touch, Event* ev)
{
	if (_touch->getState() == CTAS_Null) {
		_touch->setState(CTAS_Touched);
	}
    return true;
}

void CardBackTurn::onTouchMove(Touch* touch, Event* ev)
{
	switch (_touch->getState()) {
		case CTAS_Touched:
		{
			Vec2 touchPos = touch->getLocation();
			if (_touch->triggerTouch(touchPos)) {
				_touch->setTouchBegin(touchPos);
				_touch->setState(CTAS_Moved);
			}
			break;
		}
		case CTAS_Moved:
		{
			if (_touch->setMoveDistance(touch->getLocation())) {
				float distance = _touch->getMovedTotalDistance();
				doMove(distance < 0 ? 0 : distance);
			}
			break;
		}
	}
}

void CardBackTurn::onTouchEnd(Touch* touch, Event* ev)
{

}

void CardBackTurn::onTouchCancel(Touch* touch, Event* ev)
{

}
