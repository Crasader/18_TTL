#ifndef _CARD_FRONT_TURN_H_
#define _CARD_FRONT_TURN_H_

#include "cocos2d.h"
#include "CardTurnOption.h"

USING_NS_CC;

namespace cardturn {

typedef void* cardTurnCompleteFunc(void* param);

class TouchMathine;

class CardFrontTurn
	:public Ref
{

public:

    CardFrontTurn(Node* scene, 
		TouchMathine* state,
		std::string imgPath,
		std::string imgPathEnd,
        Vec2 basePos,
        float scaleSize);

    virtual ~CardFrontTurn();

protected:

    CardFrontTurn();

public:
    
    //////////////////////////////////////////////////////////////////////////

    void init();
    void update(float tt);
    //�ƶ�һ���ľ���
    void doMove(float fmove);
	void setOnCompleteFun(cardTurnCompleteFunc* onCompleteFun);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	bool onTouchBegin(Touch* touch, Event* ev);
	void onTouchMove(Touch* touch, Event* ev);
	void onTouchEnd(Touch* touch, Event* ev);
	void onTouchCancel(Touch* touch, Event* ev);

    //////////////////////////////////////////////////////////////////////////

protected:

	//////////////////////////////////////////////////////////////////////////

    //�����ü�ͼ��
    void ceateClipCardShape(CardTurn_Direction direction);

	//////////////////////////////////////////////////////////////////////////

protected:

	//�������////////////////////////////////////////////////////////////////////////
    //Sence����
    Node* _pScene;
    //�õ���ʱ��
    float _useTime;
    //���ű���
    float _scaleSize;
    //��ʼ�����
    Vec2 _basePos;
    //���ڲü�����ɫ
    Color4F _clipColor;
	//ԲͲ�İ뾶��С
	float _fRadius;
	//////////////////////////////////////////////////////////////////////////

	//��Ƭ����////////////////////////////////////////////////////////////////////////
    //�������
    Vec2 _cardBasePos;
    //ͼƬ·��
    std::string _imagePath;
	//ͼƬ1·��
	std::string _imagePathEnd;
	//����ͼ�ξ���
	Sprite3D* _sp3DCardFront;
	//����
	Sprite3DMaterial* _mat;
    //��Ƭ��С
    Rect _CardRect;
    //������������
    Vec2 _RealPos;
    //ƫ�����꣬���ڵ����ĵ�ʱ�������׼��
    Vec2 _offsetPos;
	//////////////////////////////////////////////////////////////////////////

    //���Ʋü���////////////////////////////////////////////////////////////////////////
	Vec2 _clipBasePos;
    //���Ʋü���
    ClippingNode* _cpCard;
    //�ü����ƶ��ľ���
    float _cp_moved_offset;
    //���ڲü����Ƶ�ͼ��
    DrawNode* _pDrawNodeCard;
    //���ڲü�����ľ��ζ���
    Vec2 _cpCardRect[CLIP_VERTEXT_SIZE];
    //////////////////////////////////////////////////////////////////////////

	//�����¼�////////////////////////////////////////////////////////////////////////
	//
	TouchMathine* _touch;
	//////////////////////////////////////////////////////////////////////////

	cardTurnCompleteFunc* _onComplete;
};

}
#endif
