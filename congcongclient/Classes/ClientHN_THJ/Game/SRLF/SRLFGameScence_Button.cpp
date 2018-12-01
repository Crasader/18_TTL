#include "SRLFGameScence.h"
#include "Game/GameLib.h"
#include "JniCross/JniFun.h"

#include "CMD_SRLF.h"
#include "SRLFPlayer.h"
#include "SRLFGameLogic.h"
#include "SRLFSoundFun.h"
#include "SRLFSetting.h"
#include "SRLFRoomDetail.h"
#include "GamePlaza/HomeScene/MallPanel/GPHomeMallPanel.h"
#include "GamePlaza/HomeScene/SettingPanel/GPHomeSettingPanel.h"
#include "GameLib/Game/Game/MissionWeiXin.h"

void SRLFGameScence::initButton()
{
	//WidgetManager::addButtonCB("SRLFButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));
	WidgetManager::addButtonCB("SRLFButton_Ready",this,button_selector(SRLFGameScence::SRLFButton_Ready));
	WidgetManager::addButtonCB("SRLFButton_GuoAction",this,button_selector(SRLFGameScence::SRLFButton_GuoAction));
	WidgetManager::addButtonCB("SRLFButton_GangAction",this,button_selector(SRLFGameScence::SRLFButton_GangAction));
	WidgetManager::addButtonCB("SRLFButton_PengAction",this,button_selector(SRLFGameScence::SRLFButton_PengAction));
	WidgetManager::addButtonCB("SRLFButton_HuAction",this,button_selector(SRLFGameScence::SRLFButton_HuAction));

	WidgetManager::addButtonCB("SRLFButtonAction_ShowCard",this,button_selector(SRLFGameScence::SRLFButtonAction_ShowCard));
	WidgetManager::addButtonCB("SRLFButtonAction_ShowNext",this,button_selector(SRLFGameScence::SRLFButtonAction_ShowNext));
	WidgetManager::addButtonCB("SRLFButtonAction_HuanPaiChengDu",this,button_selector(SRLFGameScence::SRLFButtonAction_HuanPaiChengDu));

	WidgetManager::addButtonCB("SRLFButton_ReChoice",this,button_selector(SRLFGameScence::SRLFButton_ReChoice));
	WidgetManager::addButtonCB("SRLFButton_OkChoice",this,button_selector(SRLFGameScence::SRLFButton_OkChoice));

	WidgetManager::addButtonCB("SRLFButton_GameExit",this,button_selector(SRLFGameScence::SRLFButton_GameExit));
	WidgetManager::addButtonCB("SRLFButton_Set",this,button_selector(SRLFGameScence::SRLFButton_Set));

	//WidgetManager::addButtonCB("SRLFButton_QueWan",this,button_selector(SRLFGameScence::SRLFButton_QueWan));
	//WidgetManager::addButtonCB("SRLFButton_QueTong",this,button_selector(SRLFGameScence::SRLFButton_QueTong));
	//WidgetManager::addButtonCB("SRLFButton_QueTiao",this,button_selector(SRLFGameScence::SRLFButton_QueTiao));

	WidgetManager::addButtonCB("SRLFButton_OnceAgain",this,button_selector(SRLFGameScence::SRLFButton_OnceAgain));
	WidgetManager::addButtonCB("SRLFButton_RoomHead",this,button_selector(SRLFGameScence::SRLFButton_RoomHead));

	WidgetManager::addButtonCBBegin("SRLFButton_Talk",this,button_selector(SRLFGameScence::SRLFButton_TalkBegin));
	WidgetManager::addButtonCB("SRLFButton_Talk",this,button_selector(SRLFGameScence::SRLFButton_TalkEnd));

	WidgetManager::addButtonCB("SRLFButton_TalkDefine",this,button_selector(SRLFGameScence::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(SRLFGameScence::Button_Send_TalkStr));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(SRLFGameScence::HNMJButton_BiaoQing));
	
	//WidgetManager::addButtonCB("SRLFGameScene_ButtonHelp",this,button_selector(SRLFGameScence::Button_Help));
	WidgetFun::setVisible(this,"SRLFGameScene_ButtonHelp",false);

	WidgetManager::addButtonCB("SRLFGameScene_ButtonMenu", this, button_selector(SRLFGameScence::Button_Menu));
	WidgetManager::addButtonCB("SRLFGameScene_ButtonLeave", this, button_selector(SRLFGameScence::Button_Exit));
	WidgetManager::addButtonCB("SRLFGameScene_ButtonSetting", this, button_selector(SRLFGameScence::Button_Setting));
	WidgetManager::addButtonCB("SRLFGameScene_ButtonChongZhi", this, button_selector(SRLFGameScence::Button_ChongZhi));
	//WidgetFun::setVisible(this,"SRLFGameScene_ButtonChongZhi",false);
	WidgetManager::addButtonCB("SRLFGameScene_ButtonFriend", this, button_selector(SRLFGameScence::SRLFButton_friendPrivateInfo));
	//WidgetManager::addButtonCB("NHMJ_MASTER_LEFTCARD",this,button_selector(SRLFGameScence::NHMJ_MASTER_LEFTCARD));
	WidgetManager::addButtonCB("SRLFGameScene_Button_JiaPiao", this, button_selector(SRLFGameScence::Button_JiaPiao));
	WidgetManager::addButtonCB("SRLFGameScene_Button_NoJiaPiao", this, button_selector(SRLFGameScence::Button_JiaPiao));

	WidgetManager::addButtonCB("SRLFGameScene_Button_BaoTing", this, button_selector(SRLFGameScence::SRLFButton_TingAction));
	WidgetManager::addButtonCB("SRLFGameScene_Button_NoBaoTing", this, button_selector(SRLFGameScence::SRLFButton_TingActionNo));
	WidgetManager::addButtonCB("SRLFGameScene_Button_NoBaoTing", this, button_selector(SRLFGameScence::SRLFButton_TingActionNo));
}
void SRLFGameScence::SRLFButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	for (size_t i=0;i<DF::shared()->GetGamePlayer();i++)
	{
		getPlayerByChairID(i)->defaultState();
	}
	defaultState();
}

void SRLFGameScence::SRLFButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"SelfActionNode",false);
	//状态判断
	if ((m_wResumeUser==m_pLocal->GetChairID()))
	{
		setCurrentPlayer(m_wResumeUser,WIK_NULL);
	}
	SRLF::CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_NULL;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
	//remmoveAutoAction();
}
void SRLFGameScence::SRLFButton_GangAction(cocos2d::Ref*,WidgetUserInfo*)
{
	SRLF::CMD_C_OperateCard OperateCard;
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
void SRLFGameScence::SRLFButton_PengAction(cocos2d::Ref*,WidgetUserInfo*)
{
	SRLF::CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_PENG;
	//OperateCard.cbOperateCard=utility::parseInt(WidgetFun::getWidgetUserInfo(this,"NotifyCard"));
	OperateCard.cbOperateCard=m_cbActionCard;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
	WidgetFun::setVisible(this,"SelfActionNode",false);
	//remmoveAutoAction();
}
void SRLFGameScence::SRLFButton_HuAction(cocos2d::Ref*,WidgetUserInfo*)
{
	SRLF::CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=WIK_CHI_HU;
	OperateCard.cbOperateCard=0;
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
	WidgetFun::setVisible(this,"SelfActionNode",false);
	//remmoveAutoAction();
}

void SRLFGameScence::SRLFButton_TingAction(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"BaoTingNode",false);
	SendSocketData(SUB_C_TING_FIRST);
	//OnAutoOutCard(m_pLocal->getNewCard());
	//remmoveAutoAction();
}

void SRLFGameScence::SRLFButton_TingActionNo(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"BaoTingNode",false);
	//SendSocketData(SUB_C_TING_FIRST);
	//OnAutoOutCard(m_pLocal->getNewCard());
	//remmoveAutoAction();
}

void SRLFGameScence::SRLFButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*)
{
	showSaiZi(utility::parseUInt(WidgetFun::getWidgetUserInfo(this,"SaiZiNode","NextValue")));
}
void SRLFGameScence::SRLFButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*)
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		getPlayerByChairID(i)->showHandCard();
	}
	//setCurrentPlayer(m_iCurrentUser,WIK_NULL);
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
}

void SRLFGameScence::SRLFButtonAction_HuanPaiChengDu(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setHuanPai_ChengDu();
//	DoXuanQueNotice(m_nLeftTime);
}

void SRLFGameScence::SRLFButton_ReChoice(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->blackAllHandCard(false);
	m_pLocal->seatDownCard();
}
void SRLFGameScence::SRLFButton_OkChoice(cocos2d::Ref*,WidgetUserInfo*)
{
	SRLF::CMD_C_HuanPai kInfo;
	zeromemory(kInfo.cbHuanCard,sizeof(kInfo.cbHuanCard));
	if (!m_pLocal->getHuanCards(kInfo.cbHuanCard))
	{
		return;
	}

	utility::log(utility::toString("SRLFGameScence::SRLFButton_OkChoice",
		(int)kInfo.cbHuanCard[0],
		" ",(int)kInfo.cbHuanCard[1]," "
		,(int)kInfo.cbHuanCard[2]).c_str());
	kInfo.nMasttKindColor = m_pLocal->getMastKindColor();

	SendSocketData(SUB_C_HUANPAI,&kInfo,sizeof(kInfo));

	WidgetFun::setVisible(this,"ExchangeCardNode",false);

	m_bIsHuanPai = true;
}
void SRLFGameScence::SRLFButton_GameExit(cocos2d::Ref*,WidgetUserInfo*)
{
	if (IServerItem::get())
	{
		IServerItem::get()->PerformStandUpAction();
	}
	else
	{
		exitRoom();
	}
	NoticeMsg::Instance().ShowTopMsg("解散房间");
}
void SRLFGameScence::SRLFButton_Set(cocos2d::Ref*,WidgetUserInfo*)
{
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
	//SRLFSetting::Instance().show();
}

void SRLFGameScence::SRLFButton_QueWan(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setQueColor(CARD_COLOR_WAN);
	m_pLocal->showXuanQue(false);
	m_pLocal->showQueIconAni_Reset();

	SRLF::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();
	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));

}
void SRLFGameScence::SRLFButton_QueTong(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setQueColor(CARD_COLOR_TONG);
	m_pLocal->showXuanQue(false);
	m_pLocal->showQueIconAni_Reset();

	SRLF::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();
	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));
}
void SRLFGameScence::SRLFButton_QueTiao(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->setQueColor(CARD_COLOR_TIAO);
	m_pLocal->showXuanQue(false);
	m_pLocal->showQueIconAni_Reset();

	SRLF::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();
	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));
}

void SRLFGameScence::SRLFButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*)
{
	//分享调用函数
	SRLFButton_WeiXinImagic(NULL,NULL);
}
void SRLFGameScence::SRLFButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*)
{
	SRLFButton_Ready(NULL,NULL);
	changeFreeStateNodeState(m_pLocal->GetUserStatus() < US_READY);
}
void SRLFGameScence::SRLFButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	changeFreeStateNodeState(m_pLocal->GetUserStatus() < US_READY);
}

void SRLFGameScence::SRLFButton_OnceAgain(cocos2d::Ref*,WidgetUserInfo*)
{
	SRLFButton_Ready(NULL,NULL);
}

void SRLFGameScence::SRLFButton_RoomHead( cocos2d::Ref* pButton,WidgetUserInfo* )
{
	WidgetFun::setVisible(this,"GameRoomUserInfo",true);

	Node* pNode = ((Node*)pButton)->getParent();
	if (!pNode) return;
	
	int nIndex = utility::parseInt(WidgetFun::getWidgetUserInfo(pNode,"PlayerIndex"));
	showRoomUserInfo(nIndex);
}
void SRLFGameScence::SRLFButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
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
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SRLFGameScence::endButtonTalk,this),5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}
void SRLFGameScence::endButtonTalk()
{
	SRLFButton_TalkEnd(NULL,NULL);
}
void SRLFGameScence::SRLFButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
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
void SRLFGameScence::remmoveAutoAction()
{
	TimeManager::Instance().removeByFunInt(TIME_CALLBACK_Int(SRLFGameScence::OnAutoOperate,this));
}
void SRLFGameScence::OnAutoOperate(int cbAction)
{
	if (cbAction&WIK_CHI_HU || cbAction&WIK_ZI_MO)
	{
		SRLFButton_HuAction(NULL,NULL);
	}
	else if (cbAction&WIK_GANG)
	{
		SRLFButton_GangAction(NULL,NULL);
	}
	else
	{
		SRLFButton_GuoAction(NULL,NULL);
	}
}

void SRLFGameScence::OnAutoOutCard(int cbCardData)
{ 
	ASSERT(m_nGameState == SRLF_STATE_PLAYING);
	if ( m_nGameState != SRLF_STATE_PLAYING ) return;
	ASSERT(cbCardData != 0 );
	if ( cbCardData == 0 ) return;
	SRLF::CMD_C_OutCard OutCard;
	OutCard.cbCardData = cbCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
}

void SRLFGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = SRLFSoundFun::getDefineSound(UserInfo::Instance().getGender(),utility::toString(iUserIdex));
	sendTalkDefine(m_pLocal->GetChairID(),utility::toString(kTxt,":",kTaskStr));
	WidgetFun::setVisible(this,"GameTalkPlane",false);
}
void SRLFGameScence::Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
	sendTalkString(m_pLocal->GetChairID(),kTxt);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}
void SRLFGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(m_pLocal->GetChairID(),kFile);
	WidgetFun::setVisible(this,"GameTalkPlane",false);
}


void SRLFGameScence::Button_Menu(cocos2d::Ref*, WidgetUserInfo*)
{
    if(WidgetFun::isWidgetVisble(this, "SRLFGameScene_MenuNode")) {
        WidgetFun::setVisible(this, "SRLFGameScene_MenuNode", false);
    } else {
        WidgetFun::setVisible(this, "SRLFGameScene_MenuNode", true);
    }
}

void SRLFGameScence::Button_Help(cocos2d::Ref*, WidgetUserInfo*)
{
	SRLFRoomDetail::Instance().show();
}

void SRLFGameScence::Button_Exit(cocos2d::Ref*, WidgetUserInfo*)
{
	Button_ExitGameBase(NULL, NULL);
}

void SRLFGameScence::Button_Setting(cocos2d::Ref*, WidgetUserInfo*)
{
	//SRLFSetting::Instance().show();
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
}

void SRLFGameScence::Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*)
{
	if (GPHomeMallPanel::pInstance() == nullptr)
		new GPHomeMallPanel;
	if (GPHomeMallPanel::pInstance() != nullptr)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());
	GPHomeMallPanel::Instance().show();
}

void SRLFGameScence::Button_JiaPiao(cocos2d::Ref* pRef, WidgetUserInfo* pUserInfo)
{
	WidgetFun::setVisible(this,"JiaPiaoNode",false);
	if ( m_nGameState != SRLF_STATE_NULL ) return;
	CMD_C_PiaoState pTempNetInfo;
	memset(&pTempNetInfo,0,sizeof(CMD_C_PiaoState));
	if (pUserInfo->kName == "SRLFGameScene_Button_JiaPiao")
	{
		pTempNetInfo.cbPiaoState = true;
	}else if(pUserInfo->kName == "SRLFGameScene_Button_NoJiaPiao")
	{
		pTempNetInfo.cbPiaoState = false;
	}
	SendSocketData(SUB_C_PIAO,&pTempNetInfo,sizeof(pTempNetInfo));
}

void SRLFGameScence::showReadyTimes(int times)
{
	WidgetFun::getChildWidget(this, "SRLFOperator_AlarmNode")->stopAllActions();
	
	WidgetFun::setVisible(this, "SRLFOperator_AlarmNode", true);
	int first = times / 10;
	int second = times % 10;
	auto pAlarmNode = WidgetFun::getChildWidget(this, "SRLFOperator_AlarmNode");
	WidgetFun::setImagic(WidgetFun::getChildWidget(this, "SRLFAlarm_Timer_0"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), first, ".png"), false);
	WidgetFun::setImagic(WidgetFun::getChildWidget(this, "SRLFAlarm_Timer_1"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), second, ".png"), false);

	if(times > 0) {
		DelayTime* delay = DelayTime::create(1.f);
		CallFunc* func = CallFunc::create([ = ] {
			showReadyTimes(times - 1);
		});
		Sequence* seq = Sequence::create(delay, func, NULL);
		WidgetFun::getChildWidget(this, "SRLFOperator_AlarmNode")->runAction(seq);
	}
}

void SRLFGameScence::hideReadyTimes()
{
	WidgetFun::getChildWidget(this, "SRLFOperator_AlarmNode")->stopAllActions();
	WidgetFun::setVisible(this, "SRLFOperator_AlarmNode", false);
}

void SRLFGameScence::changeFreeStateNodeState(bool bVisible)
{
	WidgetFun::setVisible(this,"FreeStateNode",bVisible);
	if(bVisible)
	{
		if(!WidgetFun::isWidgetVisble(this,"SRLFOperator_AlarmNode"))
		{
			showReadyTimes(10);
		}
	}
	else
	{
		hideReadyTimes();
	}
}

void SRLFGameScence::SRLFButton_friendPrivateInfo(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string resultRule ="";
	int renShu = 0;
	int cardShu = 0;
	if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN))) //人数
		renShu = 3;
	if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN))) //人数
		renShu = 2;
	if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_14CARDS))) //14张牌
		cardShu = 13;
	if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_8CARDS))) //8张牌
		cardShu = 7;
	std::string ruleName="";
	if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_XZDD))) //
		ruleName =" 血战";
	else if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_TDH))) //
		ruleName =" 推倒";
	resultRule += utility::toString(renShu,"人",cardShu,"张",ruleName);
	if (FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIADI))) 
		resultRule +="自摸底";
	else if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIAFAN))) 
		resultRule +="自摸番";
	std::string gangHuaName ="";
	if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_DP))) //
		resultRule +="点杠炮";
	else if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_ZM))) //
		resultRule +="点杠花";
	if(FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_JP))) //
		resultRule +="加飘";
	if (FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_GFXY))) 
		resultRule +="下雨";
	if (FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_LAO)))
		resultRule +="海底";
	resultRule += utility::toString("底注：",m_kPrivateRoomInfo.dwBaseScore);
	std::string kCCWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		utility::a_u8("三人两房"),(int)m_kPrivateRoomInfo.dwRoomNum,utility::a_u8(resultRule));
	MissionWeiXin::Instance().shareTextWeiXin(kCCWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
}
