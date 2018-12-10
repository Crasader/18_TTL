#include "SelectBox.h"

using namespace ui;

SelectBox* SelectBox::create( std::string unSelect, std::string Select )
{
	SelectBox *pRet = new SelectBox(unSelect,Select);
	if (pRet&&pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

SelectBox::SelectBox( std::string unSelect, std::string Select )
	:m_sUnSelectFileName(unSelect)
	,m_sSelectFileName(Select)
	,m_bSelect(false)
{
}

bool SelectBox::init()
{
	if(!Button::init(m_sUnSelectFileName))
		return false;
	//loadTextureNormal(m_sSelectFileName);
	//loadTextureNormal(m_sUnSelectFileName);
	return true;
}

bool SelectBox::onTouchBegan( Touch *touch, Event *unusedEvent )
{
	return Button::onTouchBegan(touch,unusedEvent);
}

void SelectBox::onTouchMoved( Touch *touch, Event *unusedEvent )
{

}

void SelectBox::onTouchEnded( Touch *touch, Event *unusedEvent )
{
	changeStatus();
	if (m_CallBack)
	{
		m_CallBack(this,getSelectStatus());
	}
}

void SelectBox::onTouchCancelled( Touch *touch, Event *unusedEvent )
{

}

void SelectBox::changeStatus()
{
	m_bSelect =!m_bSelect;
	if (m_bSelect)
	{
		loadTextureNormal(m_sSelectFileName);
	}
	else
	{
		loadTextureNormal(m_sUnSelectFileName);
	}
}

void SelectBox::setSelectStatus( bool isSelect )
{
	if (isSelect!=m_bSelect)
	{
		changeStatus();
	}
}

void SelectBox::setUnSelectFileName(std::string unSelect)
{
	m_sUnSelectFileName = unSelect;
	if (m_bSelect)
	{
		loadTextureNormal(m_sSelectFileName);
	}
}

void SelectBox::setSelectFileName(std::string selected)
{
	m_sSelectFileName = selected;
	if (!m_bSelect)
	{
		loadTextureNormal(m_sUnSelectFileName);
	}
}

