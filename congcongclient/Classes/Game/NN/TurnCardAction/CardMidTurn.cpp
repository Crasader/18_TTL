#include "CardMidTurn.h"
#include "CardTurnTouch.h"

 using namespace cardturn;

CardMidTurn::CardMidTurn()
{
    
}

CardMidTurn::~CardMidTurn()
{
    if (_pDrawNodeCylinder)
        _pDrawNodeCylinder->release();

	if (_pClipper)
		_pClipper->release();
}

CardMidTurn::CardMidTurn(Node* scene,
	TouchMathine* state,
	std::string material,
	Vec2 basePos,
	float scaleSize)
    : _pScene(nullptr)
	, _touch(state)
    , _materialPath(material)
    , _basePos(basePos)
    , _clipColor(1, 0, 0, 1)
    , _scaleSize(scaleSize)
    , _cp_moved_offset(0)
    , _useTime(0)
{
    memset(_clipRect, 0, sizeof(_clipRect));
    _pScene = scene;
}

//////////////////////////////////////////////////////////////////////////

void CardMidTurn::init()
{
	_cylinder_texture_offset = 0;

	_cylinderCard = Sprite3D::create("TTLNN/CardTurn/cylinder.c3b");
	_mat = Sprite3DMaterial::createWithFilename(_materialPath);
	_state = _mat->getTechniqueByIndex(0)->getPassByIndex(0)->getGLProgramState();
	_cylinderCard->setMaterial(_mat);
	_CardRect = _cylinderCard->getBoundingBox();

	_fRadius_origal = _CardRect.size.height / 2;
	_state->setUniformFloat("radius", _fRadius_origal);

	_cylinderCard->setScale(_scaleSize);

	_CardRect = _cylinderCard->getBoundingBox();
	_fRadius = _CardRect.size.height / 2;
	_cylinderCard->setPosition3D(Vec3(-_CardRect.size.width/2, _fRadius, -_fRadius));
	_cylinderCard->setForce2DQueue(true);

	_pClipper = ClippingNode::create();

	cocos2d::log("CardTurnAction::MD create ,anc x = %f, anc y = %f\n",
		_pClipper->getAnchorPoint().x,
		_pClipper->getAnchorPoint().y);
	_pClipper->setAnchorPoint(Vec2(0.5,0.5));

	_CardRect = _cylinderCard->getBoundingBox();
	ceateClipShape(_touch->getDirection());
	_pDrawNodeCylinder = DrawNode::create();
	_pDrawNodeCylinder->drawPolygon(_clipRect, CLIP_VERTEXT_SIZE, _clipColor, 0, _clipColor);
	_DrawNodeCylinderPos = _pDrawNodeCylinder->getPosition();
	_DrawNodeCylinderPos.y -= _fRadius;
	_pDrawNodeCylinder->setPosition(_DrawNodeCylinderPos.x, _DrawNodeCylinderPos.y);

	_pClipper->setStencil(_pDrawNodeCylinder);
	_pClipper->addChild(_cylinderCard);

	_clipPos = _basePos;
	_pClipper->setPosition(_clipPos);

	_pScene->addChild(_pClipper);

	_touch->setMaxMoveDistance(_CardRect.size.width + _fRadius);
}

void cardturn::CardMidTurn::doMove(float fmove)
{
	//cocos2d::log("CardMidTurn fmove = %f\n", fmove);
	if (fmove <= _CardRect.size.width + _fRadius) {
		_cylinder_texture_offset = fmove / _CardRect.size.width + 0.33;
		_cylinder_texture_offset = _cylinder_texture_offset - static_cast<int>(_cylinder_texture_offset);
		_state->setUniformFloat("offset", _cylinder_texture_offset);
		ceateClipShape(_touch->getDirection());
		 if (fmove < _fRadius) {
			 _clipRect[2].y = _CardRect.getMidY() - 2;
			 _clipRect[3].y = _CardRect.getMidY() - 2;
			 _pDrawNodeCylinder->clear();
			 _pDrawNodeCylinder->drawPolygon(_clipRect, CLIP_VERTEXT_SIZE, _clipColor, 0, _clipColor);
			 _pDrawNodeCylinder->setPosition(_DrawNodeCylinderPos.x, _DrawNodeCylinderPos.y + fmove - 2);
		} else if (fmove > _CardRect.size.width) {//结束圆柱体卷牌
			_clipRect[0].y = -(_CardRect.size.width - fmove);
			_clipRect[1].y = -(_CardRect.size.width - fmove);
			_pDrawNodeCylinder->clear();
			_pDrawNodeCylinder->drawPolygon(_clipRect, CLIP_VERTEXT_SIZE, _clipColor, 0, _clipColor);
		} else if (fmove > _fRadius) {//圆柱体和牌面同时卷牌
			_pDrawNodeCylinder->clear();
			_pDrawNodeCylinder->drawPolygon(_clipRect, CLIP_VERTEXT_SIZE, _clipColor, 0, _clipColor);
			_pDrawNodeCylinder->setPosition(_DrawNodeCylinderPos.x, _DrawNodeCylinderPos.y + _fRadius);
			if (_touch->getCompleteAction() == CTCAT_ForceEndOverHalf && fmove > _fRadius + _CardRect.size.width / 2) { //如果是强制结束状态
				_touch->setState(CTAS_ForceComplete);
			}
		}
		_pClipper->setPosition(_clipPos.x, _clipPos.y + ( _cp_moved_offset + fmove) * CARD_PROPOTION);
	}
}

void CardMidTurn::update(float tt)
{
	//_useTime += 0.1f;
	//doMove(_useTime);

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
		}
		break;
	case cardturn::CTAS_ForceComplete:
		doMove(_touch->getMovedTotalDistance());
		_cp_moved_offset -= _touch->getMovedStepDistance() * (_CardRect.size.width + _fRadius) / (_CardRect.size.width/2);
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

void CardMidTurn::ceateClipShape(CardTurn_Direction direction)
{
	switch (direction) {
		case CTD_Boom_To_Up:
		{
			_clipRect[0] = Vec2(_CardRect.getMinX() - _CardRect.size.width / 4, _CardRect.getMinY());
			_clipRect[1] = Vec2(_CardRect.getMaxX() + _CardRect.size.width / 4, _CardRect.getMinY());
			_clipRect[2] = Vec2(_CardRect.getMaxX() + _CardRect.size.width / 4, _CardRect.getMidY());
			_clipRect[3] = Vec2(_CardRect.getMinX() - _CardRect.size.width / 4, _CardRect.getMidY());
			break;
		}
	}
}

bool CardMidTurn::onTouchBegin(Touch* touch, Event* ev)
{
    return true;
}

void CardMidTurn::onTouchMove(Touch* touch, Event* ev)
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

void CardMidTurn::onTouchEnd(Touch* touch, Event* ev)
{
	switch (_touch->getState()) {
		case	CTAS_Touched:
		case CTAS_Moved:
			_touch->setState(CTAS_Reset);
			break;
	}
}

void CardMidTurn::onTouchCancel(Touch* touch, Event* ev)
{
	switch (_touch->getState()) {
		case	CTAS_Touched:
		case CTAS_Moved:
			_touch->setState(CTAS_Reset);
			break;
	}
}
