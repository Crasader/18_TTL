#include "common.h"

#include "SRLFRoomDetail.h"
#include "SCRIPT.h"

FV_SINGLETON_STORAGE(SRLFRoomDetail);
SRLFRoomDetail::SRLFRoomDetail()
{
    init();
}

SRLFRoomDetail::~SRLFRoomDetail()
{

}

bool SRLFRoomDetail::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }
    initLayout();
    initButton();
    return true;
}

void SRLFRoomDetail::initLayout()
{
    WidgetScenceXMLparse SRLFroomDetail("GameSRLF/xml/SRLFRoomDetail.xml", this);
}

void SRLFRoomDetail::initButton()
{
	WidgetManager::addButtonCB("NNOperator_ButtonClose", this, button_selector(SRLFRoomDetail::Button_Close));
}

void SRLFRoomDetail::show()
{
    setVisible(true);
}

void SRLFRoomDetail::hide()
{
    setVisible(false);
}

void SRLFRoomDetail::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

