#include "CardTurnTouch.h"

using namespace cardturn;

TouchMathine::TouchMathine()
	: _rollBackStep(ROLL_BACK_STEP_VALUE)
	, _forceStep(FORCE_STEP_VALUE)
{

}

TouchMathine::~TouchMathine()
{

}

void TouchMathine::setState(CardTurn_Action_State state)
{
	_state = state;
}

CardTurn_Action_State cardturn::TouchMathine::getState()
{
	return _state;
}

void TouchMathine::setDirection(CardTurn_Direction direction)
{
	_direction = direction;
}

CardTurn_Direction cardturn::TouchMathine::getDirection()
{
	return _direction;
}

void cardturn::TouchMathine::setCompleteAction(CardTurn_Complete_Action_Type action)
{
	_completeType = action;
}

cardturn::CardTurn_Complete_Action_Type cardturn::TouchMathine::getCompleteAction()
{
	return _completeType;
}

void cardturn::TouchMathine::setTouchBegin(Vec2 beginPos)
{
	_touch_begin_pos = beginPos;
}

void cardturn::TouchMathine::setMaxMoveDistance(float distance)
{
	_maxMoveDistance = distance;
}

float cardturn::TouchMathine::setMoveDistance(Vec2 pos)
{
	switch (_direction) {
		case CTD_Boom_To_Up:
		{
			float distance =  pos.y - _touch_begin_pos.y;
			return setMoveDistance(distance);
		}
	}
	return 0.0f;
}

float cardturn::TouchMathine::setMoveDistance(float distance)
{
	if (distance > _maxMoveDistance) {
		_moveStepDistance = _maxMoveDistance - _moveTotalDistance;
		_moveTotalDistance = _maxMoveDistance;
	} else {
		_moveStepDistance = distance - _moveTotalDistance;
		_moveTotalDistance = distance;
	}
	return _moveStepDistance;
}

float cardturn::TouchMathine::getMovedStepDistance()
{
	return _moveStepDistance;
}

float cardturn::TouchMathine::getRollBackStepValue()
{
	return _rollBackStep;
}

float cardturn::TouchMathine::getForceCompleteStepValue()
{
	return _forceStep;
}

bool cardturn::TouchMathine::goBack()
{
	if (_moveTotalDistance > 0) {
		_moveTotalDistance -= _rollBackStep;
		if(_moveTotalDistance < 0)
			_moveTotalDistance = 0;
		return true;
	}
	return false;
}

void TouchMathine::setTouchBeginRect(Rect beginPoint)
{
	_defaultTouchBeginRect = beginPoint;
}

bool TouchMathine::triggerTouch(Vec2 point)
{
	return !(_defaultTouchBeginRect.getMinX() > point.x ||
		_defaultTouchBeginRect.getMaxX() < point.x ||
		_defaultTouchBeginRect.getMinY() > point.y ||
		_defaultTouchBeginRect.getMaxY() < point.y);
}

float TouchMathine::getMovedTotalDistance()
{
	return _moveTotalDistance;
}
