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
    //移动一定的距离
    void doMove(float fmove);

    //////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	bool onTouchBegin(Touch* touch, Event* ev);
	void onTouchMove(Touch* touch, Event* ev);
	void onTouchEnd(Touch* touch, Event* ev);
	void onTouchCancel(Touch* touch, Event* ev);

	//////////////////////////////////////////////////////////////////////////

protected:

    //创建裁剪图形
    void ceateClipShape(CardTurn_Direction derection);

protected:

	//场景相关////////////////////////////////////////////////////////////////////////
    //Sence对象
    Node* _pScene;
    //基准坐标点
    Vec2 _basePos;
    //缩放比例
    float _scaleSize;
    //经过时间
    float _useTime;
	//////////////////////////////////////////////////////////////////////////

	//裁剪器////////////////////////////////////////////////////////////////////////
    //用于裁剪的颜色
    Color4F _clipColor;
    //用于裁剪背面的矩形顶点
    Vec2 _clipRect[CLIP_VERTEXT_SIZE];
    //用于裁剪的图形
    DrawNode* _pDrawNodeCylinder;
    //裁剪器
    ClippingNode* _pClipper;
	//裁剪器的坐标
	Vec2 _clipPos;
    //裁剪器移动的距离
    float _cp_moved_offset;
	//裁剪图形原始坐标
	Vec2 _DrawNodeCylinderPos;
	//////////////////////////////////////////////////////////////////////////

	//圆筒模型////////////////////////////////////////////////////////////////////////
	//圆筒模型
	Sprite3D* _cylinderCard;
	//圆筒到2D的大小
	Rect _CardRect;
	//图片路径
	std::string _materialPath;
	//材质
	Sprite3DMaterial* _mat;
	//贴图的便宜量,用于shader计算，值0到1
	float _cylinder_texture_offset;
	//阴影的深度
	float _shadow_alpha;
	//shader参数
	cocos2d::GLProgramState * _state;
	//圆筒的半径大小
	float _fRadius;
	//原始半径大小
	float _fRadius_origal;
	//////////////////////////////////////////////////////////////////////////
	
	//触摸相关////////////////////////////////////////////////////////////////////////
	//
	TouchMathine* _touch;
	//////////////////////////////////////////////////////////////////////////

};

}
#endif
