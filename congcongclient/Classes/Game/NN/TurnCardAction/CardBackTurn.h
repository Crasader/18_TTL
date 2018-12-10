#ifndef _CARD_BACK_H_
#define _CARD_BACK_H_

#include <cocos2d.h>
#include "CardTurnOption.h"

using namespace cocos2d;

namespace cardturn {

class TouchMathine;

class CardBackTurn
	: public Ref
{

public:

    CardBackTurn(Node* scene, 
		TouchMathine* state,
		std::string backMaterialPath,
		Vec2 basePos, 
		float scaleSize);

    virtual ~CardBackTurn();

protected:

    CardBackTurn();

public:
    
    //////////////////////////////////////////////////////////////////////////

    void init();
	//
	void update(float tt);
    //�ƶ�һ���ľ���
    void doMove(float fmove);
	//

	//////////////////////////////////////////////////////////////////////////

	bool onTouchBegin(Touch* touch, Event* ev);
	void onTouchMove(Touch* touch, Event* ev);
	void onTouchEnd(Touch* touch, Event* ev);
	void onTouchCancel(Touch* touch, Event* ev);

    //////////////////////////////////////////////////////////////////////////

protected:

	//////////////////////////////////////////////////////////////////////////

    //�����ü�ͼ��
    void ceateClipShape(CardTurn_Direction cdt_direction);


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
    //���ڲü�����ɫ
    Color4F _clipColor;
	//////////////////////////////////////////////////////////////////////////

	//�Ʊ�////////////////////////////////////////////////////////////////////////
    //ͼƬ·��
    std::string _materialPath;
    //��Ƭ��С
    Rect _CardRect;
	//�Ƶı���
	Sprite3D* _cardBack;
	//����
	Sprite3DMaterial* _mat;
	//////////////////////////////////////////////////////////////////////////

	//�ü���////////////////////////////////////////////////////////////////////////
    //�ü���
    ClippingNode* _pClipper;
	//�ü�ͼ�εĻ�׼��
	Vec2 _clipBasePos;
    //���ڲü�����ľ��ζ���
    Vec2 _clipRect[CLIP_VERTEXT_SIZE];
    //���ڲü���ͼ��
    DrawNode* _pDrawNode;
    //�ü����ƶ��ľ���
    float _cp_moved_offset;
	//�뾶
	float _fRadius;
	//////////////////////////////////////////////////////////////////////////

	//�������////////////////////////////////////////////////////////////////////////
	//
	TouchMathine* _touch;
	//////////////////////////////////////////////////////////////////////////

};

}
#endif
