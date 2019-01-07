#include "GPHomeEditNumPanel.h"
#include "Game/Game/NoticeMsg.h"
#include "Game/WZQ/WZQDGameScence.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_NN.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_SRLF.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_TLJ.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_SG.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_XZDD.h"
#include "Tools/utilityConvert.h"

FV_SINGLETON_STORAGE(GPHomeEditNumPanel)
	GPHomeEditNumPanel::GPHomeEditNumPanel()
	:m_kScore("")
	,m_cbBaseScore(50)
{
	init();
}

GPHomeEditNumPanel::~GPHomeEditNumPanel()
{
}

bool GPHomeEditNumPanel::init() {
	if (!cocos2d::Node::init()){
		return false;
	}

	WidgetScenceXMLparse enterRoomPanel("GamePlaza/Script/GPHomeEditNumPanel.xml", this);
	initButton();
	return true;
}

void GPHomeEditNumPanel::initButton()
{
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeEditNumPanel::Button_Close));
	WidgetManager::addButtonCB("Button_Confirm", this, button_selector(GPHomeEditNumPanel::Button_Confirm));
	WidgetManager::addButtonCB("Button_Delete", this, button_selector(GPHomeEditNumPanel::Button_Delete));
	WidgetManager::addButtonCB("Button_EditNumPanel_Zero", this, button_selector(GPHomeEditNumPanel::Button_Zero));
	WidgetManager::addButtonCB("Button_EditNumPanel_One", this, button_selector(GPHomeEditNumPanel::Button_One));
	WidgetManager::addButtonCB("Button_EditNumPanel_Two", this, button_selector(GPHomeEditNumPanel::Button_Two));
	WidgetManager::addButtonCB("Button_EditNumPanel_Three", this, button_selector(GPHomeEditNumPanel::Button_Three));
	WidgetManager::addButtonCB("Button_EditNumPanel_Four", this, button_selector(GPHomeEditNumPanel::Button_Four));
	WidgetManager::addButtonCB("Button_EditNumPanel_Five", this, button_selector(GPHomeEditNumPanel::Button_Five));
	WidgetManager::addButtonCB("Button_EditNumPanel_Six", this, button_selector(GPHomeEditNumPanel::Button_Six));
	WidgetManager::addButtonCB("Button_EditNumPanel_Seven", this, button_selector(GPHomeEditNumPanel::Button_Seven));
	WidgetManager::addButtonCB("Button_EditNumPanel_Eight", this, button_selector(GPHomeEditNumPanel::Button_Eight));
	WidgetManager::addButtonCB("Button_EditNumPanel_Nine", this, button_selector(GPHomeEditNumPanel::Button_Nine));
}

void GPHomeEditNumPanel::show(std::string kGameType,int cbIndex)
{
	setVisible(true);
	m_kScore = "";
	m_GameType = kGameType;
	if(kGameType == "WZQ")
		WidgetFun::setImagic(this,"Edit_Title","GamePlaza/HomeScene/title_2.png");
	else
		WidgetFun::setImagic(this,"Edit_Title","GamePlaza/HomeScene/title_3.png");
	m_cbIndex = cbIndex;
	updateRoomNum();
}

void GPHomeEditNumPanel::hide()
{
	setVisible(false);
}

void GPHomeEditNumPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeEditNumPanel::Button_Zero(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("0");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_One(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("1");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Two(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("2");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Three(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("3");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Four(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("4");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Five(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("5");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Six(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("6");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Seven(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("7");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Eight(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("8");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Nine(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 8) {
		return;
	}
	m_kScore.append("9");
	updateRoomNum();
}

void GPHomeEditNumPanel::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 0) {
		return;
	}
	dword iScore = utility::parseInt(m_kScore);
	if (m_GameType != "WZQ" && m_GameType != "XZDD" ) {
		if ( m_cbIndex == 1 )
		{
			if (iScore<1){
				iScore = 1;
				//NoticeMsg::Instance().ShowTopMsg(utility::a_u8("底注最小为50！"));
			}
			if (iScore>5000){
				iScore = 5000;
				//NoticeMsg::Instance().ShowTopMsg(utility::a_u8("底注最大为5000！"));
			}
			m_cbBaseScore = iScore;
		}else if(m_cbIndex == 2)
		{
			if (iScore< m_cbBaseScore*40 ){
				iScore = m_cbBaseScore*40;
				NoticeMsg::Instance().ShowTopMsg(utility::a_u8(utility::toString("进场设定最小为", m_cbBaseScore*40,"!")));
			}
		}else if(m_cbIndex == 3)
		{
			if (iScore<m_cbBaseScore*30){
				iScore = m_cbBaseScore*30;
				NoticeMsg::Instance().ShowTopMsg(utility::a_u8("离场设定最小为1500！"));
			}
			
		}
	}

	if ( m_GameType == "XZDD" )
	{
		if ( m_cbIndex == 1 )
		{
			if (iScore < 1){
				iScore = 1;
				//NoticeMsg::Instance().ShowTopMsg(utility::a_u8("底注最小为50！"));
			}
			if (iScore>5000){
				iScore = 5000;
				//NoticeMsg::Instance().ShowTopMsg(utility::a_u8("底注最大为5000！"));
			}
			m_cbBaseScore = iScore;
		}else if(m_cbIndex == 2)
		{
			if (iScore< m_cbBaseScore*80 ){
				iScore = m_cbBaseScore*80;
				NoticeMsg::Instance().ShowTopMsg(utility::a_u8(utility::toString("进场设定最小为", m_cbBaseScore*80,"!")));
			}
		}else if(m_cbIndex == 3)
		{
			if (iScore<m_cbBaseScore*70){
				iScore = m_cbBaseScore*70;
				NoticeMsg::Instance().ShowTopMsg(utility::a_u8("离场设定最小为3500！"));
			}
		}
	}

	if(m_GameType == "TTLNN"){
		//GPHomeCreateRoomPanel_NN::Instance().updateScore(m_cbIndex,iScore);
	}
	//else if(m_GameType == "SRLF"){
	//	GPHomeCreateRoomPanel_SRLF::Instance().updateScore(m_cbIndex,iScore);
	//}else if(m_GameType == "TLJ"){
	//	GPHomeCreateRoomPanel_TLJ::Instance().updateScore(m_cbIndex,iScore);
	//}else if(m_GameType == "WZQ"){
	//	WZQDGameScence::Instance().updateScore(m_cbIndex,iScore);
	//}else if(m_GameType == "SG"){
	//	GPHomeCreateRoomPanel_SG::Instance().updateScore(m_cbIndex,iScore);
	//}else if(m_GameType == "XZDD"){
	//	GPHomeCreateRoomPanel_XZDD::Instance().updateScore(m_cbIndex,iScore);
	//}
	hide();
}

void GPHomeEditNumPanel::Button_Delete(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_kScore.length() == 0) {
		return;
	}
	m_kScore = m_kScore.substr(0, m_kScore.length() - 1);
	updateRoomNum();
}

void GPHomeEditNumPanel::updateRoomNum()
{
	for (size_t index = 0; index < m_kScore.length(); ++index)
	{
		std::string widgetName = "Home_RoomNum";
		std::stringstream temp;
		temp << (index + 1);
		widgetName.append(temp.str());
		WidgetFun::setText(this, widgetName , m_kScore.c_str()[index]);
	}
	for (size_t index = m_kScore.length(); index < 8; ++index)
	{
		std::string widgetName = "Home_RoomNum";
		std::stringstream temp;
		temp << (index + 1);
		widgetName.append(temp.str());
		WidgetFun::setText(this, widgetName, "");
	}
}
