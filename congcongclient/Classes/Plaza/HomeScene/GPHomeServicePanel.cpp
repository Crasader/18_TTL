#include "GPHomeServicePanel.h"


FV_SINGLETON_STORAGE(GPHomeServicePanel);

GPHomeServicePanel::GPHomeServicePanel()
{
	init();
}

GPHomeServicePanel::~GPHomeServicePanel()
{

}

bool GPHomeServicePanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}
	initLayout();
	initButton();
	return true;
}

void GPHomeServicePanel::initLayout()
{
	WidgetScenceXMLparse servicePanel("GamePlaza/Script/GPHomeServicePanel.xml", this);
	WidgetFun::setText(this, "Service_Content",UserInfo::Instance().getUserData()->szGroupName);
}

void GPHomeServicePanel::initButton()
{
	WidgetManager::addButtonCB("Service_ButtonClose", this, button_selector(GPHomeServicePanel::Button_Close));
}

void GPHomeServicePanel::show()
{
	setVisible(true);
}

void GPHomeServicePanel::hide()
{
	setVisible(false);
}

void GPHomeServicePanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

