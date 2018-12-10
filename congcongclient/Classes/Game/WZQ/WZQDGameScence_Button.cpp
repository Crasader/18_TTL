#include "WZQDGameScence.h"
#include "GAME.h"
#include "JniCross/JniFun.h"

#include "CMD_WZQ.h"
#include "WZQDPlayer.h"
#include "WZQDGameLogic.h"
#include "WZQDSoundFun.h"
#include "Game/Game/MissionWeiXin.h"

#include "UTILITY.h"

void WZQDGameScence::initButton()
{
	WidgetManager::addButtonCB("Button_Ready",this,button_selector(WZQDGameScence::WZQButton_Ready));
	WidgetManager::addButtonCB("Button_SHURu",this,button_selector(WZQDGameScence::WZQButton_SHURu));//ȷ������ѧ�Ѱ�ť
	WidgetManager::addButtonCB("Button_RENshu",this,button_selector(WZQDGameScence::WZQButton_RENshu));//���䰴ť
	WidgetManager::addButtonCB("Button_game_LIKAI",this,button_selector(WZQDGameScence::WZQButton_GameExit));
	WidgetManager::addButtonCB("Button_GameExit",this,button_selector(WZQDGameScence::WZQButton_GameExit));
	WidgetManager::addButtonCB("WZQ_ButtonWeiXin",this,button_selector(WZQDGameScence::WZQButtonWeiXin));
}
void WZQDGameScence::WZQButton_Ready(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	sendReady();
	defaultState();
}
void WZQDGameScence::WZQButton_RENshu(cocos2d::Ref*,WidgetUserInfo*)//����
{
	if (m_nGameState == WZQ_STATE_PLAYING)
	{
		if(m_WZQScore > 0 )
		{
			WidgetFun::setVisible(this,"Button_game_LIKAI",true);
			WidgetFun::setVisible(this,"Button_game_JIXU",true);
			SendSocketData(SUB_C_LOSESELF,NULL,NULL);
		}
		else
		{
			NoticeMsg::Instance().ShowTopMsg(utility::a_u8("ѧ��Ϊ0�֣��������䣡"));
		}
	}
	else
	{
		NoticeMsg::Instance().ShowTopMsg(utility::a_u8("��Ϸ��ʼ��������䣡"));
	}
}

void WZQDGameScence::WZQButtonWeiXin(cocos2d::Ref*,WidgetUserInfo*)
{
	//resultRule +=" "+utility::toString("��ע��", pNetInfo->dwBaseScore,"\n");
	std::string kCCWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		utility::a_u8("������"),(int)m_kPrivateRoomInfo.dwRoomNum,utility::a_u8(""));
	MissionWeiXin::Instance().shareTextWeiXin(kCCWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
	/*MissionWeiXin::Instance().shareUrlWeiXin(script::getStr("CCWeiXinSharUrl"),
		script::getStr("CCWeiXinSharTitle"),
		kCCWeiXinSharDes,
		MissionWeiXin::SHARE_SESSION);*/
}


void WZQDGameScence::WZQButton_GameExit(cocos2d::Ref*,WidgetUserInfo*)
{
	GameBase::Button_ExitGameBase(NULL,NULL);
}

void WZQDGameScence::XZDDButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*)
{
	XZDDButton_WeiXinImagic(NULL,NULL);
}
void WZQDGameScence::XZDDButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*)
{
	WZQButton_Ready(NULL,NULL);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}
void WZQDGameScence::XZDDButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}


void WZQDGameScence::WZQButton_SHURu(cocos2d::Ref*,WidgetUserInfo*)
{
	if (m_nGameState == WZQ_STATE_PLAYING)
	{
		if (m_pLocal->GetChairID() != m_iBankerUser)
		{
			NoticeMsg::Instance().ShowTopMsg(utility::a_u8("������������ѧ�֣�"));
		}else
		{
			GPHomeEditNumPanel::Instance().show("WZQ",1);
		}
	}
	else
	{
		NoticeMsg::Instance().ShowTopMsg(utility::a_u8("��ʼ��Ϸ��ſ�������ѧ�֣�"));
	}
	
}

void WZQDGameScence::XZDDButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
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
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(WZQDGameScence::endButtonTalk,this),5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}
void WZQDGameScence::endButtonTalk()
{
	//XZDDButton_TalkEnd(NULL,NULL);
}


