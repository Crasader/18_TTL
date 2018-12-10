#include "GPHomeHelpPanel.h"
FV_SINGLETON_STORAGE(GPHomeHelpPanel);
GPHomeHelpPanel::GPHomeHelpPanel()
	: m_CurrentIndex(0)
{
	init();
}

GPHomeHelpPanel::~GPHomeHelpPanel()
{

}

bool GPHomeHelpPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initButton();
	return true;
}

void GPHomeHelpPanel::initLayout()
{
	WidgetScenceXMLparse helpPanel("GamePlaza/Script/GPHomeHelpPanel.xml", this);
}

void GPHomeHelpPanel::initButton()
{
	WidgetManager::addButtonCB("Help_ButtonClose", this, button_selector(GPHomeHelpPanel::Button_Close));
	for (int index = 0; index < 5; ++index){
		WidgetManager::addButtonCB(utility::toString("Help_Button_", index), this, button_selector(GPHomeHelpPanel::Button_Select));
	}
}

void GPHomeHelpPanel::updateStatus()
{
	auto pNode = WidgetFun::getChildWidget(this, "Help_Content");
	pNode->removeAllChildren();
	WidgetManager::Instance().createWidget(utility::toString("HelpContentSkin_", m_CurrentIndex), pNode);
	
	widget::ListViewEx* pList = dynamic_cast<widget::ListViewEx*> (pNode);
	if (pList) {
		pList->jumpToTop();
	}
}

void GPHomeHelpPanel::show()
{
	m_CurrentIndex = 0;
	updateStatus();
	setVisible(true);
}

void GPHomeHelpPanel::hide()
{
	setVisible(false);
}

void GPHomeHelpPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeHelpPanel::Button_Select(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	m_CurrentIndex = utility::parseInt(pInfo->kUserInfo.at("Index"));
	for (int index = 0; index < 5; ++index) {
		if (m_CurrentIndex == index) {
			WidgetFun::setChecked(this, utility::toString("Help_Button_", index), true);
		} else {
			WidgetFun::setChecked(this, utility::toString("Help_Button_", index), false);
		}
	}
	updateStatus();
}

