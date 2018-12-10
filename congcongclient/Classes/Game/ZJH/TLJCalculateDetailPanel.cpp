#include "TLJCalculateDetailPanel.h"
#include "TLJGameScence.h"
#include "TLJGameLogic.h"
#include "TLJPlayer_Panel.h"

FV_SINGLETON_STORAGE(TLJCalculateDetailPanel);
TLJCalculateDetailPanel::TLJCalculateDetailPanel()
{
	init();
}

TLJCalculateDetailPanel::~TLJCalculateDetailPanel()
{

}

bool TLJCalculateDetailPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	WidgetScenceXMLparse calculateDetailPanel("Game/TLJ/Script/TLJCalculateDetailPanel.xml", this);


	return true;
}

void TLJCalculateDetailPanel::show()
{
	setVisible(true);
}

void TLJCalculateDetailPanel::hide()
{
	setVisible(false);
}