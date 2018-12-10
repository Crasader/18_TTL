#pragma once

#include <cocos2d.h>

USING_NS_CC;

namespace widget
{

class SliderMenu : public Menu
{
public:
	SliderMenu();
public:
	static SliderMenu* create(MenuItem* item);
	static SliderMenu* createWithArray(Vector<MenuItem*> pArrayOfItems);
public:
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);

public:
	void setTarget(Ref *rec, SEL_MenuHandler selector);
	void setontrolItem(MenuItem* pItem);
	void AddPosY(float fPosY);
	void SetButtonPosY(float fPosY);
	void setActValue(float fValue);
	void setMaxValue(float fValue);
	float getMaxValue();
	void setMaxPosY(float fValue);
	float getValue();

	void SetButtonPosX(float fPosX);
	void setMoveByX(bool bMoveByX);
private:
	float m_fToucPosY;

	float m_fMaxValue;
	float m_fMaxPosY;

	//∫·œÚª¨∂Ø
	bool m_bIsMoveByX;
	float m_fToucPosX;
	float m_fMaxPosX;

	float m_fActValue;
	MenuItem* m_pControlItem;

	Ref*       m_pListener;
	SEL_MenuHandler    m_pfnSelector;

};

}
