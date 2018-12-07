#include "GPHomeExitPanel.h"

FV_SINGLETON_STORAGE(GPHomeExitPanel);
GPHomeExitPanel::GPHomeExitPanel() 
{
	init();
}

GPHomeExitPanel::~GPHomeExitPanel()
{
}

bool GPHomeExitPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}
	
	WidgetScenceXMLparse exitPanel("GamePlaza/Script/GPHomeExitPanel.xml", this);
	WidgetManager::addButtonCB("Button_Comfirm", this, button_selector(GPHomeExitPanel::Button_Comfirm));
	WidgetManager::addButtonCB("Button_Cancel", this, button_selector(GPHomeExitPanel::Button_Cancel));
	return true;
}

void GPHomeExitPanel::hide()
{
	setVisible(false);
}

void GPHomeExitPanel::show()
{
	setVisible(true);
}

void GPHomeExitPanel::Button_Cancel(cocos2d::Ref* pRef, WidgetUserInfo * pUserInfo)
{
	hide();
}

void GPHomeExitPanel::Button_Comfirm(cocos2d::Ref*, WidgetUserInfo *)
{
	cocos2d::Director::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
