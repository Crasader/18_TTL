#include "GPHomeUserInfoPanel.h"
#include "GameLib/Game/Game/UserInfo.h"
#include "GameLib/Game/Script/utility.h"
#include "GameLib/Tools/tools/Convert.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetFun.h"
#include "cocos2d.h"
#include "GameLib/GameNet/ImagicDownManager.h"
#include "GamePlaza/GPSceneManager.h"

FV_SINGLETON_STORAGE(GPHomeUserInfoPanel);
GPHomeUserInfoPanel::GPHomeUserInfoPanel()
{
	init();
}

GPHomeUserInfoPanel::~GPHomeUserInfoPanel()
{
}

bool GPHomeUserInfoPanel::init() {
	if (!cocos2d::Node::init()) {
		return false;
	}
	
	WidgetScenceXMLparse sharePanel("GamePlaza/Script/GPHomeUserInfoPanel.xml", this);
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeUserInfoPanel::Button_Close));
	WidgetManager::addButtonCB("Button_ok", this, button_selector(GPHomeUserInfoPanel::Button_Close));
	UserInfo::Instance().addUpPlayerInfoCB(this, QY_CALLFUNC_SELECTOR(GPHomeUserInfoPanel::onUserInfo));
	return true;
}


void GPHomeUserInfoPanel::show()
{
	onUserInfo();
	setVisible(true);
}

void GPHomeUserInfoPanel::hide()
{
	setVisible(false);
}

void GPHomeUserInfoPanel::onUserInfo()
{
	WidgetFun::setImagic(WidgetFun::getChildWidgetByName(this,"User_Avar1"),"GamePlaza/HomeScene/avatar_male.png",true);
	WidgetFun::setImagic(WidgetFun::getChildWidgetByName(this,"User_Avar"),"GamePlaza/HomeScene/Tou_OutSide.png",true);
	auto pos = WidgetFun::getChildWidgetByName(this,"User_Avar1")->getPosition();
	auto avar = GPSceneManager::getCircleAvatar(WidgetFun::getChildWidgetByName(this,"User_Avar1"), "GamePlaza/HomeScene/avatar_male.png", cocos2d::Size(146,146),  pos);
	ImagicDownManager::Instance().addDown(avar,UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());
	WidgetFun::setText(this,"ID_Txt",utility::toString(UserInfo::Instance().getUserID()));
	WidgetFun::setText(this,"Nick_Txt",utility::toString(UserInfo::Instance().getUserNicName()));
	if((int)UserInfo::Instance().getGender() == UserSex::US_Male)
	{
		WidgetFun::setChecked(this,"Button_Cheak_Female",false); 
		WidgetFun::setChecked(this,"Button_Cheak_Male",true);
	}else if((int)UserInfo::Instance().getGender() == UserSex::US_Femal)
	{
		WidgetFun::setChecked(this,"Button_Cheak_Female",true); 
		WidgetFun::setChecked(this,"Button_Cheak_Male",false);
	}
	else
	{
		WidgetFun::setChecked(this,"Button_Cheak_Male",false);
		WidgetFun::setChecked(this,"Button_Cheak_Female",false);
	}
}

cocos2d::Node* GPHomeUserInfoPanel::getCircleAvatar(cocos2d::Node* pRootNode, const std::string& filename, const cocos2d::Size& targetSize,  const cocos2d::Vec2& position)
{
	auto pClip = cocos2d::ClippingNode::create();
	auto sprite = cocos2d::Sprite::create(filename);
	auto viewSize = sprite->getContentSize();
	if (viewSize.height < targetSize.height || viewSize.width < targetSize.width) {
		auto scaleHeight = targetSize.height * 1.f / viewSize.height;
		auto scaleWidth = targetSize.width * 1.f / viewSize.width;
		auto targetScale = utility::Max(scaleWidth, scaleHeight);
		sprite->setScale(targetScale);
	}
	auto stencil = cocos2d::Sprite::create("GamePlaza/Avatar.png");
	stencil->setScale(1.1f,1.1f);
	pClip->setAlphaThreshold(.7f);
	pClip->setStencil(stencil);
	pClip->addChild(sprite);
	pClip->setInverted(false);
	pRootNode->addChild(pClip);
	pClip->setPosition(position);
	return sprite;
}


void GPHomeUserInfoPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}
