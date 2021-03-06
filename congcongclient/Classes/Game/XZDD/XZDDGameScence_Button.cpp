#include "XZDDGameScence.h"
#include "GAME.h"
#include "JniCross/JniFun.h"

#include "CMD_XZDD.h"
#include "XZDDPlayer.h"
#include "XZDDGameLogic.h"
#include "XZDDSoundFun.h"
#include "Plaza/HomeScene/GPHomeMallPanel.h"				//新加
#include "Plaza/HomeScene/GPHomeSettingPanel.h"		//新加

#include UTILITY_LOG

void XZDDGameScence::initButton()
{
	WidgetManager::addButtonCB("XZDDButton_Ready",this,button_selector(XZDDGameScence::XZDDButton_Ready));
	WidgetManager::addButtonCB("XZDDButton_GuoAction",this,button_selector(XZDDGameScence::XZDDButton_GuoAction));
	WidgetManager::addButtonCB("XZDDButton_GangAction",this,button_selector(XZDDGameScence::XZDDButton_GangAction));
	WidgetManager::addButtonCB("XZDDButton_PengAction",this,button_selector(XZDDGameScence::XZDDButton_PengAction));
	WidgetManager::addButtonCB("XZDDButton_HuAction",this,button_selector(XZDDGameScence::XZDDButton_HuAction));

	//WidgetManager::addButtonCB("XZDDButtonAction_ShowCard",this,button_selector(XZDDGameScence::XZDDButtonAction_ShowCard));
	//WidgetManager::addButtonCB("XZDDButtonAction_ShowNext",this,button_selector(XZDDGameScence::XZDDButtonAction_ShowNext));
	//WidgetManager::addButtonCB("XZDDButtonAction_HuanPaiChengDu",this,button_selector(XZDDGameScence::XZDDButtonAction_HuanPaiChengDu));

	WidgetManager::addButtonCB("XZDDButton_ReChoice",this,button_selector(XZDDGameScence::XZDDButton_ReChoice));
	WidgetManager::addButtonCB("XZDDButton_OkChoice",this,button_selector(XZDDGameScence::XZDDButton_OkChoice));

	WidgetManager::addButtonCB("XZDDButton_GameExit",this,button_selector(XZDDGameScence::XZDDButton_GameExit));
	WidgetManager::addButtonCB("XZDDButton_Set",this,button_selector(XZDDGameScence::XZDDButton_Set));

	WidgetManager::addButtonCB("XZDDButton_QueWan",this,button_selector(XZDDGameScence::XZDDButton_QueWan));
	WidgetManager::addButtonCB("XZDDButton_QueTong",this,button_selector(XZDDGameScence::XZDDButton_QueTong));
	WidgetManager::addButtonCB("XZDDButton_QueTiao",this,button_selector(XZDDGameScence::XZDDButton_QueTiao));

	WidgetManager::addButtonCB("XZDDButton_OnceAgain",this,button_selector(XZDDGameScence::XZDDButton_OnceAgain));
	WidgetManager::addButtonCB("XZDDButton_RoomHead",this,button_selector(XZDDGameScence::XZDDButton_RoomHead));

	WidgetManager::addButtonCBBegin("XZDDButton_Talk",this,button_selector(XZDDGameScence::XZDDButton_TalkBegin));
	WidgetManager::addButtonCB("XZDDButton_Talk",this,button_selector(XZDDGameScence::XZDDButton_TalkEnd));

	WidgetManager::addButtonCB("Button_TalkDefine",this,button_selector(XZDDGameScence::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(XZDDGameScence::Button_Send_TalkStr));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(XZDDGameScence::HNMJButton_BiaoQing));
	
	WidgetManager::addButtonCB("Button_Master",this,button_selector(XZDDGameScence::HNMJButton_Master));
	WidgetManager::addButtonCB("Button_MasterClose",this,button_selector(XZDDGameScence::Button_MasterClose));
	WidgetManager::addButtonCB("NHMJ_MASTER_LEFTCARD",this,button_selector(XZDDGameScence::NHMJ_MASTER_LEFTCARD));

	WidgetManager::addButtonCB("XZDDGameScene_ButtonMenu", this, button_selector(XZDDGameScence::Button_Menu));//新加
	WidgetManager::addButtonCB("XZDDGameScene_ButtonLeave", this, button_selector(XZDDGameScence::Button_Exit));//新加
	WidgetManager::addButtonCB("XZDDGameScene_ButtonSetting", this, button_selector(XZDDGameScence::Button_Setting));//新加
	WidgetManager::addButtonCB("XZDDGameScene_ButtonRoom", this, button_selector(XZDDGameScence::Button_XDDRoom));//新加
	WidgetManager::addButtonCB("XZDDGameScene_ButtonFriend", this, button_selector(XZDDGameScence::XZDDButton_WeiXinFriend));//新加
}

void XZDDGameScence::Button_Menu(cocos2d::Ref*, WidgetUserInfo*)//新加 设置
{
 	if (WidgetFun::isWidgetVisble(this, "XZDDGameScene_MenuNode")) {
 		WidgetFun::setVisible(this, "XZDDGameScene_MenuNode", false);
 	}
 	else {
 		WidgetFun::setVisible(this, "XZDDGameScene_MenuNode", true);
 	}
}

void XZDDGameScence::Button_Setting(cocos2d::Ref*, WidgetUserInfo*)//新加
{
	//SRLFSetting::Instance().show();
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
}

void XZDDGameScence::Button_XDDRoom(cocos2d::Ref*, WidgetUserInfo*)//新加 
{
	if (WidgetFun::isWidgetVisble(this, "XZDDGameScene_XDDRoomX")) {
		WidgetFun::setVisible(this, "XZDDGameScene_XDDRoomX", false);
	}
	else {
		WidgetFun::setVisible(this, "XZDDGameScene_XDDRoomX", true);
	}
}

void XZDDGameScence::Button_Exit(cocos2d::Ref*, WidgetUserInfo*)//新加 
{
	Button_ExitGameBase(NULL, NULL);
}

void XZDDGameScence::XZDDButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
	Setpaidui();
}

void XZDDGameScence::XZDDButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"SelfActionNode",false);
	//状态判断
	if ((m_wResumeUser==m_pLocal->GetChairID()))
	{
		setCurrentPlayer(m_wResumeUser,WIK_NULL);
	}
	XZDD::CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_NULL;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
	//remmoveAutoAction();
}

void XZDDGameScence::XZDDButtonAction_HuanPaiChengDu(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setHuanPai_ChengDu();
}

void XZDDGameScence::XZDDButton_GangAction(cocos2d::Ref*,WidgetUserInfo*)
{
	m_bselectGangState = false;
	XZDD::CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_GANG;
	if (m_nGangCardsCount == 1)
	{
		OperateCard.cbOperateCard = m_nGangCards[0];
		m_bselectGangState = false;
		SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
		WidgetFun::setVisible(this,"SelfActionNode",false);
	}
	else if (m_nGangCardsCount > 1)
	{
		m_bselectGangState = true;
		m_pLocal->showGangCards(m_nGangCards,m_nGangCardsCount);
	}
}

void XZDDGameScence::XZDDButton_PengAction(cocos2d::Ref*,WidgetUserInfo*)
{
	XZDD::CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_PENG;
	OperateCard.cbOperateCard=m_cbActionCardData;//utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard"));
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
	//remmoveAutoAction();
}

void XZDDGameScence::XZDDButton_HuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	XZDD::CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_CHI_HU;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	WidgetFun::setVisible(this,"SelfActionNode",false);
	//remmoveAutoAction();
}

void XZDDGameScence::XZDDButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*)
{
	showSaiZi(utility::parseUInt(WidgetFun::getWidgetUserInfo(this,"SaiZiNode","NextValue")));
}

void XZDDGameScence::XZDDButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showHandCard();
	}
	setCurrentPlayer(m_iCurrentUser,WIK_NULL);
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}

void XZDDGameScence::XZDDButton_ReChoice(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->blackAllHandCard(false);
	m_pLocal->seatDownCard();
}

void XZDDGameScence::XZDDButton_OkChoice(cocos2d::Ref*,WidgetUserInfo*)
{
	XZDD::CMD_C_HuanPai kInfo;
	zeromemory(kInfo.cbHuanCard,sizeof(kInfo.cbHuanCard));
	if (!m_pLocal->getHuanCards(kInfo.cbHuanCard))
	{
		return;
	}

	utility::filelog(utility::toString("XZDDGameScence::XZDDButton_OkChoice",
		(int)kInfo.cbHuanCard[0],
		" ",(int)kInfo.cbHuanCard[1]," "
		,(int)kInfo.cbHuanCard[2]).c_str());
	kInfo.nMasttKindColor = m_pLocal->getMastKindColor();

	SendSocketData(SUB_C_HUANPAI,&kInfo,sizeof(kInfo));

	WidgetFun::setVisible(this,"ExchangeCardNode",false);

	m_bIsHuanPai = true;
}

void XZDDGameScence::XZDDButton_GameExit(cocos2d::Ref*,WidgetUserInfo*)
{

}

void XZDDGameScence::XZDDButton_Set(cocos2d::Ref*,WidgetUserInfo*)
{
	//WidgetFun::setVisible(this,"GameSetPlane",true);
	GPHomeSettingPanel::Instance().show();
}

void XZDDGameScence::XZDDButton_QueWan(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setQueColor(CARD_COLOR_WAN);
	m_pLocal->showXuanQue(false);
	m_pLocal->showQueIconAni_Reset();

	XZDD::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();
	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));

}

void XZDDGameScence::XZDDButton_QueTong(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setQueColor(CARD_COLOR_TONG);
	m_pLocal->showXuanQue(false);
	m_pLocal->showQueIconAni_Reset();

	XZDD::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();
	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));
}

void XZDDGameScence::XZDDButton_QueTiao(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setQueColor(CARD_COLOR_TIAO);
	m_pLocal->showXuanQue(false);
	m_pLocal->showQueIconAni_Reset();

	XZDD::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();
	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));
}

void XZDDGameScence::XZDDButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*)
{
	XZDDButton_WeiXinImagic(NULL,NULL);
}

void XZDDGameScence::XZDDButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*)
{
	XZDDButton_Ready(NULL,NULL);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}

void XZDDGameScence::XZDDButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}

void XZDDGameScence::XZDDButton_OnceAgain(cocos2d::Ref*,WidgetUserInfo*)
{
	XZDDButton_Ready(NULL,NULL);
}

void XZDDGameScence::XZDDButton_RoomHead( cocos2d::Ref* pButton,WidgetUserInfo* )
{
	WidgetFun::setVisible(this,"GameRoomUserInfo",true);

	Node* pNode = ((Node*)pButton)->getParent();
	if (!pNode) return;
	
	int nIndex = utility::parseInt(WidgetFun::getWidgetUserInfo(pNode,"PlayerIndex"));
	showRoomUserInfo(nIndex);
}

void XZDDGameScence::XZDDButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
{
	if (WidgetFun::isWidgetVisble(this,"TalkImagic"))
	{
		JniFun::stopSoundRecord();
		WidgetFun::setVisible(this,"TalkImagic",false);
		return;
	}
	SoundFun::Instance().PaseBackMusic();
	WidgetFun::setVisible(this,"TalkImagic",true);
	JniFun::startSoundRecord();
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(XZDDGameScence::endButtonTalk,this),5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}

void XZDDGameScence::endButtonTalk()
{
	XZDDButton_TalkEnd(NULL,NULL);
}

void XZDDGameScence::XZDDButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
{
	SoundFun::Instance().OnResumeBackMusic();
	if (!WidgetFun::isWidgetVisble(this,"TalkImagic"))
	{
		return;
	}
	int iTimeID = utility::parseInt(WidgetFun::getWidgetUserInfo(this,"TalkImagic","TimeID"));
	TimeManager::Instance().removeByID(iTimeID);
	WidgetFun::setVisible(this,"TalkImagic",false);
	std::string kFileName = JniFun::stopSoundRecord();
	sendTalkFile(m_pLocal->GetChairID(),kFileName);
}

void XZDDGameScence::remmoveAutoAction()
{
	TimeManager::Instance().removeByFunInt(TIME_CALLBACK_Int(XZDDGameScence::OnAutoOperate,this));
}

void XZDDGameScence::OnAutoOperate(int cbAction)
{
	if (cbAction&WIK_CHI_HU || cbAction&WIK_ZI_MO)
	{
		XZDDButton_HuAction(NULL,NULL);
	}
	else if (cbAction&WIK_GANG)
	{
		XZDDButton_GangAction(NULL,NULL);
	}
	else
	{
		XZDDButton_GuoAction(NULL,NULL);
	}
}

void XZDDGameScence::OnAutoOutCard(int cbCardData)
{
	ASSERT(m_nGameState == XZDD_STATE_PLAYING);
	if ( m_nGameState != XZDD_STATE_PLAYING ) return;
	WidgetFun::setVisible(this,"SelfActionNode",false);
	XZDD::CMD_C_OutCard OutCard;
	OutCard.cbCardData = cbCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
}

void XZDDGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = XZDDSoundFun::getDefineSound(m_pLocal->GetGender(),utility::toString(iUserIdex));
	sendTalkDefine(m_pLocal->GetChairID(),utility::toString(kTxt,":",kTaskStr));

	WidgetFun::setVisible(this,"GameTalkPlane",false);
}

void XZDDGameScence::Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
	sendTalkString(m_pLocal->GetChairID(),kTxt);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}

void XZDDGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(m_pLocal->GetChairID(),kFile);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
}