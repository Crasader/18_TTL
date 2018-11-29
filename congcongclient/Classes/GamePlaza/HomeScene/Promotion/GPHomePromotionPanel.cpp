#include "GPHomePromotionPanel.h"

FV_SINGLETON_STORAGE(GPHomePromotionPanel);
GPHomePromotionPanel::GPHomePromotionPanel()
{
	init();
}

GPHomePromotionPanel::~GPHomePromotionPanel()
{

}

bool GPHomePromotionPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initButton();
	return true;
}

void GPHomePromotionPanel::initLayout()
{
	WidgetScenceXMLparse promotionPanel("GamePlaza/Script/GPHomePromotionPanel.xml", this);
}

void GPHomePromotionPanel::show()
{
	setVisible(true);
}

void GPHomePromotionPanel::hide()
{
	setVisible(false);
}

void GPHomePromotionPanel::initButton()
{
	WidgetManager::addButtonCB("Promotion_ButtonClose", this, button_selector(GPHomePromotionPanel::Button_Close));
	WidgetManager::addButtonCB("Promotion_ButtonBind", this, button_selector(GPHomePromotionPanel::Button_Bind));
}

void GPHomePromotionPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomePromotionPanel::Button_Bind(cocos2d::Ref*, WidgetUserInfo*)
{
	// to do
}

