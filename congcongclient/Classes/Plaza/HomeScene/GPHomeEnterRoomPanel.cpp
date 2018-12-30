#include "GPHomeEnterRoomPanel.h"
#include "Plaza/GameManager/GPGameLink.h"

FV_SINGLETON_STORAGE(GPHomeEnterRoomPanel)
GPHomeEnterRoomPanel::GPHomeEnterRoomPanel()
	:msRoomNum("")
{
	init();
}

GPHomeEnterRoomPanel::~GPHomeEnterRoomPanel()
{
}

bool GPHomeEnterRoomPanel::init() {
	if (!cocos2d::Node::init()){
		return false;
	}
	
	WidgetScenceXMLparse enterRoomPanel("GamePlaza/Script/GPHomeEnterRoomPanel.xml", this);
	initButton();
	return true;
}

void GPHomeEnterRoomPanel::initButton()
{
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeEnterRoomPanel::Button_Close));
	WidgetManager::addButtonCB("Button_Cancel", this, button_selector(GPHomeEnterRoomPanel::Button_Cancel));
	WidgetManager::addButtonCB("Button_Delete", this, button_selector(GPHomeEnterRoomPanel::Button_Delete));
	WidgetManager::addButtonCB("Button_Zero", this, button_selector(GPHomeEnterRoomPanel::Button_Zero));
	WidgetManager::addButtonCB("Button_One", this, button_selector(GPHomeEnterRoomPanel::Button_One));
	WidgetManager::addButtonCB("Button_Two", this, button_selector(GPHomeEnterRoomPanel::Button_Two));
	WidgetManager::addButtonCB("Button_Three", this, button_selector(GPHomeEnterRoomPanel::Button_Three));
	WidgetManager::addButtonCB("Button_Four", this, button_selector(GPHomeEnterRoomPanel::Button_Four));
	WidgetManager::addButtonCB("Button_Five", this, button_selector(GPHomeEnterRoomPanel::Button_Five));
	WidgetManager::addButtonCB("Button_Six", this, button_selector(GPHomeEnterRoomPanel::Button_Six));
	WidgetManager::addButtonCB("Button_Seven", this, button_selector(GPHomeEnterRoomPanel::Button_Seven));
	WidgetManager::addButtonCB("Button_Eight", this, button_selector(GPHomeEnterRoomPanel::Button_Eight));
	WidgetManager::addButtonCB("Button_Nine", this, button_selector(GPHomeEnterRoomPanel::Button_Nine));
}

void GPHomeEnterRoomPanel::show()
{
	setVisible(true);
	msRoomNum = "";
	updateRoomNum();
}

void GPHomeEnterRoomPanel::hide()
{
	setVisible(false);
}

void GPHomeEnterRoomPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeEnterRoomPanel::Button_Zero(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("0");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_One(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("1");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Two(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("2");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Three(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("3");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Four(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("4");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Five(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("5");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Six(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("6");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Seven(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("7");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Eight(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("8");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Nine(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 6) {
		return;
	}
	msRoomNum.append("9");
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Cancel(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 0) {
		return;
	}
	msRoomNum = "";
	updateRoomNum();
}

void GPHomeEnterRoomPanel::Button_Delete(cocos2d::Ref*, WidgetUserInfo*)
{
	if (msRoomNum.length() == 0) {
		return;
	}
	msRoomNum = msRoomNum.substr(0, msRoomNum.length() - 1);
	updateRoomNum();
}

void GPHomeEnterRoomPanel::updateRoomNum()
{
	for (int index = 0; index < msRoomNum.length(); ++index)
	{
		std::string widgetName = utility::toString("Home_RoomNum", index + 1);
		WidgetFun::setImagic(this, widgetName , utility::toString("GamePlaza/HomeScene/EnterRoomPanel/", msRoomNum.c_str()[index], ".png"));
		WidgetFun::setVisible(this, widgetName, true);
	}

	for (int index = msRoomNum.length(); index < 6; ++index)
	{
		std::string widgetName = utility::toString("Home_RoomNum", index + 1);
		WidgetFun::setVisible(this, widgetName, false);
	}

	if (msRoomNum.length() == 6)
	{
		GPGameLink::Instance().JoinRoom(msRoomNum);
	}
}
