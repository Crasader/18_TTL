#include "GPHomeCreateRoomPanel_SG.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/SG/GameScene/SGGameScence.h"
#include "../EditNumPanel/GPHomeEditNumPanel.h"
#include "../EditNumPanel/GPHomeEditNumPanel.h"
#include "GamePlaza/GameManager/GPGameManager.h"
#include "CommonHeader/LocalConstant.h"

USING_NS_CC;
FV_SINGLETON_STORAGE(GPHomeCreateRoomPanel_SG);

GPHomeCreateRoomPanel_SG::GPHomeCreateRoomPanel_SG()
{
	init();
}

GPHomeCreateRoomPanel_SG::~GPHomeCreateRoomPanel_SG()
{

}

bool GPHomeCreateRoomPanel_SG::init()
{
	if(!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initData();
	initButton();

	return true;
}

void GPHomeCreateRoomPanel_SG::initLayout()
{
	WidgetScenceXMLparse createRoom_SG("GamePlaza/Script/GPHomeCreateRoomPanel_SG.xml", this);
}

void GPHomeCreateRoomPanel_SG::initButton()
{
	WidgetManager::addButtonCB("SG_CreateRoom_ButtonConfirm", this, button_selector(GPHomeCreateRoomPanel_SG::Button_Confirm));
	WidgetManager::addButtonCB("SG_CreateRoom_ButtonClose", this, button_selector(GPHomeCreateRoomPanel_SG::Button_Close));
	WidgetManager::addButtonCB("SG_CreateRoom_GameRule", this, button_selector(GPHomeCreateRoomPanel_SG::Button_AllowStrangerJoin));
	WidgetManager::addButtonCB("SG_CreateRoom_GameScore_Btn", this, button_selector(GPHomeCreateRoomPanel_SG::Button_ChangeGameScore));
	WidgetManager::addButtonCB("SG_CreateRoom_EnterNum_Btn", this, button_selector(GPHomeCreateRoomPanel_SG::Button_ChangeGameScore));
}

void GPHomeCreateRoomPanel_SG::initData()
{
	resetGameData();
}

void GPHomeCreateRoomPanel_SG::resetGameData()
{
	m_GameRuleIndex = 0;
	m_bAllowedStrangerJoin = true;
	dwBaseScore = 50;
	dwEnterMatchNum = dwBaseScore*40;
	dwOutMatchNum = dwBaseScore*30;
	updateScore(1,dwBaseScore);
}

void GPHomeCreateRoomPanel_SG::show()
{
	m_bAllowedStrangerJoin = true;
	WidgetFun::setChecked(this,"SG_CreateRoom_GameRule",m_bAllowedStrangerJoin);
	//FvMask::Add(m_GameRuleIndex, _MASK_(GAME_RULE_ALLOW_QIANG_ZHUANG));
	//WidgetFun::setChecked(this,"SG_CreateRoom_GameRule_AllowQZ",m_bAllowedStrangerQiangZhuang);
	//WidgetFun::setVisible(this,"SG_CreateRoom_GameRule_AllowQZ",false);
	dwBaseScore = 50;
	dwEnterMatchNum = dwBaseScore*40;
	dwOutMatchNum = dwBaseScore*30;
	updateScore(1,dwBaseScore);
	setVisible(true);
}

void GPHomeCreateRoomPanel_SG::hide()
{
	setVisible(false);
}

void GPHomeCreateRoomPanel_SG::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
	FvMask::Add(m_GameRuleIndex, _MASK_(GAME_RULE_SCORE_0));
	FvMask::Add(m_GameRuleIndex, _MASK_(GAME_RULE_SNATCH_RATIO_3));
	FvMask::Add(m_GameRuleIndex, _MASK_(GAME_RULE_SPECIAL_RULE_0));
	FvMask::Add(m_GameRuleIndex, _MASK_(GAME_RULE_SPECIAL_RULE_1));
	CMD_GR_Create_Private createRoom;
	zeromemory(&createRoom, sizeof(createRoom));
	createRoom.cbGameType = Type_Private;
	createRoom.bPlayCoutIdex = 0;
	createRoom.bGameTypeIdex = 3;
	//if(m_bAllowedStrangerQiangZhuang)
	createRoom.bGameRuleIdex = m_GameRuleIndex;
	createRoom.bAllowedStrangerJoin = m_bAllowedStrangerJoin;
	createRoom.dwEnterMatchNum = utility::parseInt(WidgetFun::getText(this,"SG_CreateRoom_EnterNum_Txt"));
	createRoom.dwBaseScore = utility::parseInt(WidgetFun::getText(this,"SG_CreateRoom_GameScore_Txt"));
	createRoom.dwOutMatchNum = utility::parseInt(WidgetFun::getText(this,"SG_CreateRoom_OutNum_Txt"));
	createRoom.wCharNum =-1;
	GPGameLink::Instance().CreateRoom(SGGameScence::KIND_ID, createRoom);
}

void GPHomeCreateRoomPanel_SG::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeCreateRoomPanel_SG::Button_GameRules(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	DWORD gameRule = utility::parseInt(pInfo->kUserInfo.at("Index"));
	if(pInfo->kName=="SG_CreateRoom_GameRule_AllowQZ"){
		/*if(WidgetFun::isChecked(this,"SG_CreateRoom_GameRule_AllowQZ")){
			FvMask::Add(m_GameRuleIndex, _MASK_(GAME_RULE_ALLOW_QIANG_ZHUANG));
		}else{
			FvMask::Del(m_GameRuleIndex, _MASK_(GAME_RULE_ALLOW_QIANG_ZHUANG));
		}*/
	}
}

void GPHomeCreateRoomPanel_SG::Button_AllowStrangerJoin(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	m_bAllowedStrangerJoin = !m_bAllowedStrangerJoin;
}

void GPHomeCreateRoomPanel_SG::Button_ChangeGameScore(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	int cbIndex = utility::parseInt(pInfo->kUserInfo.at("ScoreType"));
	GPHomeEditNumPanel::Instance().show(utility::toString("SG"),cbIndex);
}

void GPHomeCreateRoomPanel_SG::updateScore(BYTE cbIndex,dword wScore)
{
	switch (cbIndex)
	{
	case 1:
		{
			WidgetFun::setText(this,"SG_CreateRoom_GameScore_Txt",wScore);
			WidgetFun::setText(this,"SG_CreateRoom_EnterNum_Txt",wScore*40);
			WidgetFun::setText(this,"SG_CreateRoom_OutNum_Txt",wScore*30);
			break;
		}
	case 2:
		WidgetFun::setText(this,"SG_CreateRoom_EnterNum_Txt",wScore);
		break;
	case 3:
		WidgetFun::setText(this,"SG_CreateRoom_OutNum_Txt",wScore);
		break;
	default:
		break;
	}
}
