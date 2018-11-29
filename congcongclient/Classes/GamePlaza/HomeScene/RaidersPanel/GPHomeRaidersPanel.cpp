#include "GPHomeRaidersPanel.h"

FV_SINGLETON_STORAGE(GPHomeRaidersPanel)
GPHomeRaidersPanel::GPHomeRaidersPanel()
{
	init();
}

GPHomeRaidersPanel::~GPHomeRaidersPanel()
{
}

bool GPHomeRaidersPanel::init() 
{
	if (!cocos2d::Node::init()) {
		return false;
	}
	
	WidgetScenceXMLparse raidersPanel("GamePlaza/Script/GPHomeRaidersPanel.xml", this);
	initButton();
	return true;
}

void GPHomeRaidersPanel::initButton()
{
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeRaidersPanel::Button_Close));
}

void GPHomeRaidersPanel::show()
{
	setVisible(true);
}

void GPHomeRaidersPanel::hide()
{
	setVisible(false);
}

void GPHomeRaidersPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*) {
	hide();
}