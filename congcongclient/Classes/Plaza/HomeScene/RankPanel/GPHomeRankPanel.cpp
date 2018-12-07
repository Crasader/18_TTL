#include "GPHomeRankPanel.h"
#include "Game/Script/ScriptData.h"
#include "GameLib/GameNet/ImagicDownManager.h"


FV_SINGLETON_STORAGE(GPHomeRankPanel);
GPHomeRankPanel::GPHomeRankPanel()
	: m_CurrentIndex(0)
	, m_pRootNode(NULL)
	, m_PopularizeMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
{
	init();
	m_PopularizeMission.setMissionSink(this);
}

GPHomeRankPanel::~GPHomeRankPanel()
{

}

bool GPHomeRankPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}
	WidgetScenceXMLparse RankPanel("GamePlaza/Script/GPHomeNewRankPanel.xml", this);
	initButton();
	return true;
}

void GPHomeRankPanel::initLayout()
{
}

void GPHomeRankPanel::initButton()
{
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeRankPanel::ButtonClose));
}

void GPHomeRankPanel::initStatus()
{
}

void GPHomeRankPanel::show()
{
	setVisible(true);
	m_PopularizeMission.GetPopularizeList(0, 0);
}

void GPHomeRankPanel::hide()
{
	setVisible(false);
}

void GPHomeRankPanel::onGPPopularizeListRes(CMD_GP_PopularizeListRes* pNetInfo)
{
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this, "RankListView");
	pList->removeAllChildren();
	dword dwKindID = pNetInfo->dwGameKindID;
	auto vcPopularizeList = pNetInfo->vcPopularizeList;
	for ( int i=0; i<vcPopularizeList.size();i++) {
		auto itemInfo = vcPopularizeList.at(i);
		cocos2d::Node* pItem = WidgetManager::Instance().createWidget("GameNewRankListItem", WidgetFun::getListViewWidget(this, "RankListView"));
		WidgetFun::setVisible(pItem,"Img_No_1",i==0);
		WidgetFun::setVisible(pItem,"Img_No_2",i==1);
		WidgetFun::setVisible(pItem,"Img_No_3",i==2);
		WidgetFun::setVisible(pItem,"Img_No_0",i>2);
		WidgetFun::setVisible(pItem,"Img_No_0",i>2);
		if(i>2)
		{
			WidgetFun::setText(pItem,"Txt_No",utility::toString(i+1));
		}else
		{
			WidgetFun::setText(pItem,"Txt_No","");
		}
		WidgetFun::setText(pItem,"Txt_name",utility::a_u8(itemInfo.szName));
		WidgetFun::setText(pItem,"Txt_id",utility::toString("ID:",itemInfo.dwUserID));
		WidgetFun::setText(pItem,"Txt_gold",utility::toString(itemInfo.lScore));
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidgetByName(pItem,"sprite_icon"),itemInfo.szAvatar,itemInfo.dwUserID);
	}
	pList->forceDoLayout();
}

void GPHomeRankPanel::Button_Selected(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	m_CurrentIndex = utility::parseInt(pInfo->kUserInfo.at("Index"));
	initStatus();
}

void GPHomeRankPanel::ButtonClose(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	hide();
}

void GPHomeRankPanel::Button_Close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (cocos2d::ui::Widget::TouchEventType::ENDED != type ) return;
	hide();
}