#include "GPHomeMorePanel.h"
#include "GamePlaza/HomeScene/ServicePanel/GPHomeServicePanel.h"
#include "GamePlaza/HomeScene/SettingPanel/GPHomeSettingPanel.h"

FV_SINGLETON_STORAGE(GPHomeMorePanel);
GPHomeMorePanel::GPHomeMorePanel()
{
	init();
}

GPHomeMorePanel::~GPHomeMorePanel()
{

}

bool GPHomeMorePanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initButton();
	return true;
}

void GPHomeMorePanel::initLayout()
{
	WidgetScenceXMLparse morePanel("GamePlaza/Script/GPHomeMorePanel.xml", this);
}

void GPHomeMorePanel::initButton()
{
	WidgetManager::addButtonCB("", this, button_selector(GPHomeMorePanel::Button_Close));
	WidgetManager::addButtonCB("More_ButtonService", this, button_selector(GPHomeMorePanel::Button_Service));
	WidgetManager::addButtonCB("More_ButtonSetting", this, button_selector(GPHomeMorePanel::Button_Setting));
	WidgetManager::addButtonCB("More_ButtonQuit", this, button_selector(GPHomeMorePanel::Button_Quit));
}

void GPHomeMorePanel::show()
{
	setVisible(true);
}

void GPHomeMorePanel::hide()
{
	setVisible(false);
}

void GPHomeMorePanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeMorePanel::Button_Service(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeServicePanel::Instance().show();
	hide();
}

void GPHomeMorePanel::Button_Setting(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(true);
	hide();
}

void GPHomeMorePanel::Button_Quit(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	exit(0);
#else
	cocos2d::Director::getInstance()->end();
#endif
}