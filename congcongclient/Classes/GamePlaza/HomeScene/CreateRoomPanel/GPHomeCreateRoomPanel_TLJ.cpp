#include "GPHomeCreateRoomPanel_TLJ.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/MENJI/TLJGameScence.h"
#include "../EditNumPanel/GPHomeEditNumPanel.h"

USING_NS_CC;
FV_SINGLETON_STORAGE(GPHomeCreateRoomPanel_TLJ);

GPHomeCreateRoomPanel_TLJ::GPHomeCreateRoomPanel_TLJ()
{
	init();
}

GPHomeCreateRoomPanel_TLJ::~GPHomeCreateRoomPanel_TLJ()
{

}

bool GPHomeCreateRoomPanel_TLJ::init()
{
	if(!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initData();
	initButton();

	return true;
}

void GPHomeCreateRoomPanel_TLJ::initLayout()
{
	WidgetScenceXMLparse createRoom_TLJ("GamePlaza/Script/GPHomeCreateRoomPanel_TLJ.xml", this);
}

void GPHomeCreateRoomPanel_TLJ::initButton()
{
	WidgetManager::addButtonCB("TLJ_CreateRoom_ButtonConfirm", this, button_selector(GPHomeCreateRoomPanel_TLJ::Button_Confirm));
	WidgetManager::addButtonCB("TLJ_CreateRoom_ButtonClose", this, button_selector(GPHomeCreateRoomPanel_TLJ::Button_Close));
	WidgetManager::addButtonCB("TLJ_CreateRoom_GameRule_Allow", this, button_selector(GPHomeCreateRoomPanel_TLJ::Button_AllowStrangerJoin));
	WidgetManager::addButtonCB("TLJ_CreateRoom_GameScore_Btn", this, button_selector(GPHomeCreateRoomPanel_TLJ::Button_ChangeGameScore));
	WidgetManager::addButtonCB("TLJ_CreateRoom_EnterNum_Btn", this, button_selector(GPHomeCreateRoomPanel_TLJ::Button_ChangeGameScore));
	//WidgetManager::addButtonCB("TLJ_CreateRoom_OutNum_Btn", this, button_selector(GPHomeCreateRoomPanel_TLJ::Button_ChangeGameScore));
	for (int i = GAME_RULE_COMMON;i<=GAME_RULE_BRAISED;i++)
	{
		std::string kName = utility::toString("TLJ_CreateRoom_GameRule_",i);
		WidgetManager::addButtonCB(kName, this, button_selector(GPHomeCreateRoomPanel_TLJ::Button_GameRules));
		if(WidgetFun::isChecked(this,kName)){
			FvMask::Add(m_GameRuleIndex, _MASK_(i));
		}
	}
}

void GPHomeCreateRoomPanel_TLJ::initData()
{
	resetGameData();
}

void GPHomeCreateRoomPanel_TLJ::resetGameData()
{
	m_GameRuleIndex = 0;
	m_bAllowedStrangerJoin = true;
	dwBaseScore = 50;
	dwEnterMatchNum = dwBaseScore*40;
	dwOutMatchNum = dwBaseScore*30;
}

void GPHomeCreateRoomPanel_TLJ::show()
{
	m_bAllowedStrangerJoin = true;
	WidgetFun::setChecked(this,"TLJ_CreateRoom_GameRule_Allow",m_bAllowedStrangerJoin);
	dwBaseScore = 50;
	dwEnterMatchNum = dwBaseScore*40;
	dwOutMatchNum = dwBaseScore*30;
	WidgetFun::setText(this,"TLJ_CreateRoom_GameScore_Txt",dwBaseScore);
	WidgetFun::setText(this,"TLJ_CreateRoom_EnterNum_Txt",dwBaseScore*40);
	WidgetFun::setText(this,"TLJ_CreateRoom_OutNum_Txt",dwBaseScore*30);
	setVisible(true);
}

void GPHomeCreateRoomPanel_TLJ::hide()
{
	setVisible(false);
}
void GPHomeCreateRoomPanel_TLJ::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
	CMD_GR_Create_Private createRoom;
	zeromemory(&createRoom, sizeof(createRoom));
	createRoom.cbGameType = Type_Private;
	createRoom.bPlayCoutIdex = 0;
	createRoom.bGameTypeIdex = 4;
	createRoom.bGameRuleIdex = m_GameRuleIndex;
	createRoom.bAllowedStrangerJoin = m_bAllowedStrangerJoin;
	createRoom.dwEnterMatchNum = utility::parseInt(WidgetFun::getText(this,"TLJ_CreateRoom_EnterNum_Txt"));
	createRoom.dwBaseScore = utility::parseInt(WidgetFun::getText(this,"TLJ_CreateRoom_GameScore_Txt"));
	createRoom.dwOutMatchNum = utility::parseInt(WidgetFun::getText(this,"TLJ_CreateRoom_OutNum_Txt"));
	createRoom.wCharNum =-1;
	GPGameLink::Instance().CreateRoom(TLJGameScence::KIND_ID, createRoom);
}

void GPHomeCreateRoomPanel_TLJ::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeCreateRoomPanel_TLJ::Button_GameRules(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	DWORD gameRule = utility::parseInt(pInfo->kUserInfo.at("Index"));

	
	if(GAME_RULE_BRAISED==gameRule){
		if(WidgetFun::isChecked(this,utility::toString("TLJ_CreateRoom_GameRule_",GAME_RULE_BRAISED))){
			FvMask::Add(m_GameRuleIndex, _MASK_(GAME_RULE_BRAISED));
		}else{
			FvMask::Del(m_GameRuleIndex, _MASK_(GAME_RULE_BRAISED));
		}
	}else
	{
		for (DWORD i = GAME_RULE_COMMON;i<GAME_RULE_BRAISED;i++)
		{
			if(i==gameRule){
				FvMask::Add(m_GameRuleIndex, _MASK_(i));
			}else{
				FvMask::Del(m_GameRuleIndex, _MASK_(i));
				WidgetFun::setChecked(WidgetFun::getChildWidget(this,utility::toString("TLJ_CreateRoom_GameRule_",i)),false);
			}
		}
	}
}

void GPHomeCreateRoomPanel_TLJ::Button_AllowStrangerJoin(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	m_bAllowedStrangerJoin = !m_bAllowedStrangerJoin;
}

void GPHomeCreateRoomPanel_TLJ::Button_ChangeGameScore(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	int cbIndex = utility::parseInt(pInfo->kUserInfo.at("ScoreType"));
	GPHomeEditNumPanel::Instance().show(utility::toString("TLJ"),cbIndex);
}

void GPHomeCreateRoomPanel_TLJ::updateScore(BYTE cbIndex,dword wScore)
{
	switch (cbIndex)
	{
	case 1:
		{
			WidgetFun::setText(this,"TLJ_CreateRoom_GameScore_Txt",wScore);
			WidgetFun::setText(this,"TLJ_CreateRoom_EnterNum_Txt",wScore*40);
			WidgetFun::setText(this,"TLJ_CreateRoom_OutNum_Txt",wScore*30);
		}
		break;
	case 2:
		WidgetFun::setText(this,"TLJ_CreateRoom_EnterNum_Txt",wScore);
		break;
	case 3:
		WidgetFun::setText(this,"TLJ_CreateRoom_OutNum_Txt",wScore);
		break;
	default:
		break;
	}
}
