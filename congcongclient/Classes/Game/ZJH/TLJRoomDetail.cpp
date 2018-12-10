#include "TLJRoomDetail.h"
#include "WIDGET.h"

FV_SINGLETON_STORAGE(TLJRoomDetail);
TLJRoomDetail::TLJRoomDetail()
{
    init();
}

TLJRoomDetail::~TLJRoomDetail()
{

}

bool TLJRoomDetail::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();

    return true;
}

void TLJRoomDetail::initLayout()
{
    WidgetScenceXMLparse TLJroomDetail("Game/TLJ/Script/TLJRoomDetail.xml", this);
}

void TLJRoomDetail::initButton()
{
	WidgetManager::addButtonCB("NNOperator_ButtonClose", this, button_selector(TLJRoomDetail::Button_Close));
}

void TLJRoomDetail::show()
{
    setVisible(true);
}

void TLJRoomDetail::hide()
{
    setVisible(false);
}

void TLJRoomDetail::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

