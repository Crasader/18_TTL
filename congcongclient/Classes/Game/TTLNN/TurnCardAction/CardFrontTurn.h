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
    //移动一定的距离
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

    //创建裁剪图形
    void ceateClipCardShape(CardTurn_Direction direction);

	//////////////////////////////////////////////////////////////////////////

protected:

	//场景相关////////////////////////////////////////////////////////////////////////
    //Sence对象
    Node* _pScene;
    //用掉的时间
    float _useTime;
    //缩放比例
    float _scaleSize;
    //初始坐标点
    Vec2 _basePos;
    //用于裁剪的颜色
    Color4F _clipColor;
	//圆筒的半径大小
	float _fRadius;
	//////////////////////////////////////////////////////////////////////////

	//卡片精灵////////////////////////////////////////////////////////////////////////
    //正面基点
    Vec2 _cardBasePos;
    //图片路径
    std::string _imagePath;
	//图片1路径
	std::string _imagePathEnd;
	//正面图形精灵
	Sprite3D* _sp3DCardFront;
	//材质
	Sprite3DMaterial* _mat;
    //卡片大小
    Rect _CardRect;
    //调整后的坐标点
    Vec2 _RealPos;
    //偏移坐标，用于当翻拍的时候调整基准点
    Vec2 _offsetPos;
	//////////////////////////////////////////////////////////////////////////

    //卡牌裁剪器////////////////////////////////////////////////////////////////////////
	Vec2 _clipBasePos;
    //卡牌裁剪器
    ClippingNode* _cpCard;
    //裁剪器移动的距离
    float _cp_moved_offset;
    //用于裁剪卡牌的图形
    DrawNode* _pDrawNodeCard;
    //用于裁剪背面的矩形顶点
    Vec2 _cpCardRect[CLIP_VERTEXT_SIZE];
    //////////////////////////////////////////////////////////////////////////

	//触摸事件////////////////////////////////////////////////////////////////////////
	//
	TouchMathine* _touch;
	//////////////////////////////////////////////////////////////////////////

	cardTurnCompleteFunc* _onComplete;
};

}
#endif
