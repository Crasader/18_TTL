#pragma once
#ifndef __BTN_TURN_H__
#define __BTN_TURN_H__
#include "cocos2d.h"
#include "GameLib/Platform/PFDefine/data/ServerListData.h"
#include "GameLib/Game/FV/FvSingleton.h"
USING_NS_CC;
enum BtnPos  
{  
    Pos_Bottom = 1,  
    Pos_Left,  
    Pos_Top,  
    Pos_Right,  
};  

class BtnTurnLayer
	: public cocos2d::Layer 
	//, public IServerListDataSink
	, public FvSingleton<BtnTurnLayer>
{
public:
	BtnTurnLayer();
	~BtnTurnLayer();

public:
	bool init();
	void initData();
	void showUIButton();
	void show();
	void hide();
public:
	//CREATE_FUNC(BtnTurn)
protected:  
    virtual bool onTouchBegan(Touch* touch, Event* pEvent);  
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);  
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);


 //点击按钮之后的动画  
    Sprite* runTouchedAmt(Sprite * btn);  
    //滑动界面的动画  
    void runSlidedAmt(bool isLeft,float ratio,float judgePosX);  
private:  
    Sprite * m_btn1;  
    Sprite * m_btn2;  
    Sprite * m_btn3;  
    Sprite * m_btn4;  
  
    Vec2 m_posBottom;  
    Vec2 m_posLeft;  
    Vec2 m_posTop;  
    Vec2 m_posRight;  
  
    Point m_firstPos;  
      
    Size m_winSize;  
    bool m_valid;//先点击有效区域  
    bool m_invalid;//先点击无效区域
	std::map<int,cocos2d::Texture2D*> m_mapTextrue;
	int m_curPage;
};
#endif