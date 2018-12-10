#ifndef _CARD_MID_H_
#define _CARD_MID_H_

#include <cocos2d.h>
#include "CardTurnOption.h"

using namespace cocos2d;

namespace cardturn {

class TouchMathine;

class CardMidTurn
	:public Ref
{

public:

    CardMidTurn(Node* scene,
		TouchMathine* state,
		std::string materialPath,
		Vec2 basePos, 
		float scaleSize);

    virtual ~CardMidTurn();

protected:

    CardMidTurn();

public:
    
    //////////////////////////////////////////////////////////////////////////

    void init();
	//
    void update(float tt);
    //�ƶ�һ���ľ���
    void doMove(float fmove);

    //////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	bool onTouchBegin(Touch* touch, Event* ev);
	void onTouchMove(Touch* touch, Event* ev);
	void onTouchEnd(Touch* touch, Event* ev);
	void onTouchCancel(Touch* touch, Event* ev);

	//////////////////////////////////////////////////////////////////////////

protected:

    //�����ü�ͼ��
    void ceateClipShape(CardTurn_Direction derection);

protected:

	//�������////////////////////////////////////////////////////////////////////////
    //Sence����
    Node* _pScene;
    //��׼�����
    Vec2 _basePos;
    //���ű���
    float _scaleSize;
    //����ʱ��
    float _useTime;
	//////////////////////////////////////////////////////////////////////////

	//�ü���////////////////////////////////////////////////////////////////////////
    //���ڲü�����ɫ
    Color4F _clipColor;
    //���ڲü�����ľ��ζ���
    Vec2 _clipRect[CLIP_VERTEXT_SIZE];
    //���ڲü���ͼ��
    DrawNode* _pDrawNodeCylinder;
    //�ü���
    ClippingNode* _pClipper;
	//�ü���������
	Vec2 _clipPos;
    //�ü����ƶ��ľ���
    float _cp_moved_offset;
	//�ü�ͼ��ԭʼ����
	Vec2 _DrawNodeCylinderPos;
	//////////////////////////////////////////////////////////////////////////

	//ԲͲģ��////////////////////////////////////////////////////////////////////////
	//ԲͲģ��
	Sprite3D* _cylinderCard;
	//ԲͲ��2D�Ĵ�С
	Rect _CardRect;
	//ͼƬ·��
	std::string _materialPath;
	//����
	Sprite3DMaterial* _mat;
	//��ͼ�ı�����,����shader���㣬ֵ0��1
	float _cylinder_texture_offset;
	//��Ӱ�����
	float _shadow_alpha;
	//shader����
	cocos2d::GLProgramState * _state;
	//ԲͲ�İ뾶��С
	float _fRadius;
	//ԭʼ�뾶��С
	float _fRadius_origal;
	//////////////////////////////////////////////////////////////////////////
	
	//�������////////////////////////////////////////////////////////////////////////
	//
	TouchMathine* _touch;
	//////////////////////////////////////////////////////////////////////////

};

}
#endif
