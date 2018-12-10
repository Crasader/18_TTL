#include "TLJInfoPanel.h"

USING_NS_CC_EXT;

FV_SINGLETON_STORAGE(TLJInfoPanel);
TLJInfoPanel::TLJInfoPanel()
{
    init();
}

TLJInfoPanel::~TLJInfoPanel()
{
}

bool TLJInfoPanel::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    WidgetScenceXMLparse infoPanel("Game/TLJ/Script/TLJInfoPanel.xml", this);

	return true;
}


void TLJInfoPanel::show()
{
	setVisible(true);
}

void TLJInfoPanel::hide()
{
    setVisible(false);
}
