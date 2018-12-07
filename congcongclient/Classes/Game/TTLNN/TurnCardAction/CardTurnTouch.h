#ifndef _CARD_TURN_TOUCH_H
#define _CARD_TURN_TOUCH_H

#include "cocos2d.h"
#include "CardTurnOption.h"

USING_NS_CC;

namespace cardturn {

class TouchMathine 
{

public:

	TouchMathine();

	virtual ~TouchMathine();

public:

	void setState(CardTurn_Action_State state);
	CardTurn_Action_State getState();

	void setDirection(CardTurn_Direction curDirection);
	CardTurn_Direction getDirection();

	void setCompleteAction(CardTurn_Complete_Action_Type action);
	CardTurn_Complete_Action_Type getCompleteAction();

	//////////////////////////////////////////////////////////////////////////
	//设置触摸动作开始的触发区域
	void setTouchBeginRect(Rect beginRect);
	//设置触摸开始坐标点
	void setTouchBegin(Vec2 beginPos);
	//是否触发翻拍动作
	bool triggerTouch(Vec2 pos);
	//设置最大的移动距离
	void setMaxMoveDistance(float distance);
	//设置已经移动的距离
	float setMoveDistance(Vec2 pos);
	//设置已经移动的距离,返回实际移动的距离
	float setMoveDistance(float distance);
	//获取到初始点的移动距离
	float getMovedTotalDistance();
	//获取一个update的移动距离
	float getMovedStepDistance();
	//获取每一步move的距离，要保证每个move里面的每个if要走到，否则要出现裂缝
	float getRollBackStepValue();
	//获取每一步move的距离，要保证每个move里面的每个if要走到，否则要出现裂缝
	float getForceCompleteStepValue();
	//回滚一个step的距离
	bool goBack();

protected:

	//完成搓牌的方式
	CardTurn_Complete_Action_Type _completeType;
	//翻牌的方向
	CardTurn_Direction _direction;
	//当前的搓牌状态
	CardTurn_Action_State _state;
	//触摸起始位置
	Vec2 _touch_begin_pos;
	//可以触发搓牌动作开始的区域
	Rect _defaultTouchBeginRect;
	//回滚时每一步的距离
	float _rollBackStep;
	//	强制移动时每一步的距离
	float _forceStep;
	//已经移动的距离
	float _moveTotalDistance;
	//一次update移动的距离
	float _moveStepDistance;
	//最大移动距离
	float _maxMoveDistance;
};

}

#endif