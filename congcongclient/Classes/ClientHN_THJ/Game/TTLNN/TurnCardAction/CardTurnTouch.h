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
	//���ô���������ʼ�Ĵ�������
	void setTouchBeginRect(Rect beginRect);
	//���ô�����ʼ�����
	void setTouchBegin(Vec2 beginPos);
	//�Ƿ񴥷����Ķ���
	bool triggerTouch(Vec2 pos);
	//���������ƶ�����
	void setMaxMoveDistance(float distance);
	//�����Ѿ��ƶ��ľ���
	float setMoveDistance(Vec2 pos);
	//�����Ѿ��ƶ��ľ���,����ʵ���ƶ��ľ���
	float setMoveDistance(float distance);
	//��ȡ����ʼ����ƶ�����
	float getMovedTotalDistance();
	//��ȡһ��update���ƶ�����
	float getMovedStepDistance();
	//��ȡÿһ��move�ľ��룬Ҫ��֤ÿ��move�����ÿ��ifҪ�ߵ�������Ҫ�����ѷ�
	float getRollBackStepValue();
	//��ȡÿһ��move�ľ��룬Ҫ��֤ÿ��move�����ÿ��ifҪ�ߵ�������Ҫ�����ѷ�
	float getForceCompleteStepValue();
	//�ع�һ��step�ľ���
	bool goBack();

protected:

	//��ɴ��Ƶķ�ʽ
	CardTurn_Complete_Action_Type _completeType;
	//���Ƶķ���
	CardTurn_Direction _direction;
	//��ǰ�Ĵ���״̬
	CardTurn_Action_State _state;
	//������ʼλ��
	Vec2 _touch_begin_pos;
	//���Դ������ƶ�����ʼ������
	Rect _defaultTouchBeginRect;
	//�ع�ʱÿһ���ľ���
	float _rollBackStep;
	//	ǿ���ƶ�ʱÿһ���ľ���
	float _forceStep;
	//�Ѿ��ƶ��ľ���
	float _moveTotalDistance;
	//һ��update�ƶ��ľ���
	float _moveStepDistance;
	//����ƶ�����
	float _maxMoveDistance;
};

}

#endif