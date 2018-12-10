#include "TLJDismissRoom.h"
#include "Plaza/GameManager/GPGameLink.h"

FV_SINGLETON_STORAGE(TLJDismissRoom);
TLJDismissRoom::TLJDismissRoom()
{
	init();
}

TLJDismissRoom::~TLJDismissRoom()
{

}

bool TLJDismissRoom::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	return true;
}

void TLJDismissRoom::initLayout(bool isPlaying)
{
	if (isPlaying) 
	{
		WidgetScenceXMLparse dismissRoom("Game/TLJ/Script/DismissRoomPanelPlaying.xml", this);
	} 
	else 
	{
		WidgetScenceXMLparse dismissRoom("Game/TLJ/Script/DismissRoomPanel.xml", this);
	}
}

void TLJDismissRoom::initButton()
{
	WidgetManager::addButtonCB("DismissRoom_ButtonClose", this, button_selector(TLJDismissRoom::Button_Close));
	WidgetManager::addButtonCB("DismissRoom_ButtonConfirm", this, button_selector(TLJDismissRoom::Button_Confirm));
}

void TLJDismissRoom::show(bool isPlaying)
{
	removeAllChildren();
	initLayout(isPlaying);
	initButton();
	setVisible(true);
}

void TLJDismissRoom::hide()
{
	setVisible(false);
}

void TLJDismissRoom::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void TLJDismissRoom::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
	GPGameLink::Instance().DismissRoom();
	hide();
}

