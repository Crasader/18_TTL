#include "SRLFGameScence.h"
#include "GAME.h"
#include "SRLFPlayer.h"
#include "SRLFSoundFun.h"
#include "Game/Widget/SliderMenu.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "SRLFSetting.h"


void SRLFGameScence::initSetButton()
{
	if (WidgetFun::getChildWidget(this,"GameSetPlane") == NULL)
	{
		return;
	}
	//WidgetManager::addButtonCB("HNMJ_SetBarSider1",this,button_selector(SRLFGameScence::SetBarSider1));
	//WidgetManager::addButtonCB("HNMJ_SetBarSider2",this,button_selector(SRLFGameScence::SetBarSider2));
	//WidgetManager::addButtonCB("HNMJ_Button_HideSet",this,button_selector(SRLFGameScence::Button_HideSet));
	//WidgetManager::addButtonCB("HNMJ_Button_GameSet",this,button_selector(SRLFGameScence::Button_GameSet));
	//WidgetManager::addButtonCB("HNMJ_Button_Cheak_ChangSha",this,button_selector(SRLFGameScence::HNMJ_Button_Hua));
	//WidgetManager::addButtonCB("Button_DismissPrivate",this,button_selector(SRLFGameScence::HNMJButton_DismissRoom));
	
	/*{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar1",0.7f);
		SoundFun::Instance().SetSoundEffect(fTime);
	}
	{
		float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar2",0.7f);
		SoundFun::Instance().SetSoundBackground(fTime); 
	}*/
}



void SRLFGameScence::HNMJ_Button_Hua(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	return;
	std::string kIdex = WidgetFun::getUserInfoValue(pUserInfo,"Idex");
	if (kIdex == "0")
	{
		SRLFSoundFun::setPuTong(false);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_ChangSha",true);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_PuTong",false);
	}
	else
	{
		SRLFSoundFun::setPuTong(true);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_ChangSha",false);
		WidgetFun::setChecked(this,"HNMJ_Button_Cheak_PuTong",true);
	}
	cocos2d::UserDefault::getInstance()->setStringForKey("Hua",kIdex);
}
void SRLFGameScence::SetBarSider1(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	widget::SliderMenu* pSlider = WidgetFun::castNode<widget::SliderMenu>((Node*)pNode);
	SoundFun::Instance().SetSoundEffect(pSlider->getValue()/pSlider->getMaxValue());
	cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar1",pSlider->getValue()/100.0f);
}
void SRLFGameScence::SetBarSider2(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	widget::SliderMenu* pSlider = WidgetFun::castNode<widget::SliderMenu>((Node*)pNode);
	SoundFun::Instance().SetSoundBackground(pSlider->getValue()/pSlider->getMaxValue());
	cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar2",pSlider->getValue()/100.0f);
}
void SRLFGameScence::Button_HideSet(cocos2d::Ref* pNode,WidgetUserInfo*)
{
	cocos2d::Node* pRootNode = WidgetManager::Instance().getBaseNode((cocos2d::Node*)pNode);
	{
		widget::SliderMenu* pSlider = WidgetFun::castNode<widget::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider1"));
		cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar1",pSlider->getValue()/100.0f);
	}
	{
		widget::SliderMenu* pSlider = WidgetFun::castNode<widget::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider2"));
		cocos2d::UserDefault::getInstance()->setFloatForKey("SetBar2",pSlider->getValue()/100.0f);
	}
}
void SRLFGameScence::Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo)
{
	//cocos2d::Node* pRootNode = this;
	//{
	//	float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar1",0.7f);
	//	fTime =fTime*100;
	//	widget::SliderMenu* pSlider = WidgetFun::castNode<widget::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider1"));
	//	pSlider->setActValue(fTime);
	//	WidgetFun::setProgress(pRootNode,"SetBar1",fTime,100);
	//	SoundFun::Instance().SetSoundEffect(fTime/pSlider->getMaxValue());
	//}

	//{
	//	float fTime =cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar2",0.7f);
	//	fTime =fTime*100;
	//	widget::SliderMenu* pSlider = WidgetFun::castNode<widget::SliderMenu>(WidgetFun::getChildWidget(pRootNode,"SetBarSider2"));
	//	pSlider->setActValue(fTime);
	//	WidgetFun::setProgress(pRootNode,"SetBar2",fTime,100);
	//	SoundFun::Instance().SetSoundBackground(fTime/pSlider->getMaxValue());
	//}
	SRLFSetting::Instance().show();
}

void SRLFGameScence::HNMJButton_DismissRoom(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	 CServerItem::get()->PerformStandUpAction();
	//if (WidgetFun::getChildWidget(this, "GameSetPlane")) {
	//	WidgetFun::setVisible(this, "GameSetPlane", false);
	//}

	//if (m_nPlayCount == 0 && WidgetFun::getUserInfoValue(pUserInfo, "Pop") != "1") {
	//	/*PopScence::Instance().showAccessPlane(script::getStr("DismissPrivate"), this,
	//	button_selector(SRLFGameScence::HNMJButton_SureeDismissPrivate), NULL);*/
	//		GPGameLink::Instance().DismissRoom();
	//	return;
	//}
	//GPGameLink::Instance().DismissRoom(false);
	/*PopScence::Instance().showAccessPlane(script::getStr("SureDismissPrivate"), this,
		button_selector(SRLFGameScence::HNMJButton_SureeDismissPrivate), NULL);*/
}

void SRLFGameScence::HNMJButton_SureeDismissPrivate(cocos2d::Ref*, WidgetUserInfo*)
{
	GPGameLink::Instance().DismissRoom();
}

void SRLFGameScence::HNMJButton_SureeDismissPrivateNot(cocos2d::Ref*, WidgetUserInfo*)
{
	GPGameLink::Instance().DismissRoom(false);
}