#include <cocos2d.h>
#include "GPHomeUserInfoPanel.h"
#include "Game/Game/UserInfo.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetFun.h"
#include "GameNet/ImagicDownManager.h"
#include "Plaza/GameManager/GPSceneManager.h"
#include "UTILITY.h"

FV_SINGLETON_STORAGE(GPHomeUserInfoPanel);
GPHomeUserInfoPanel::GPHomeUserInfoPanel()
	: _spUserHread(nullptr)
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
	if (_spUserHread == nullptr) {
		auto spHead = WidgetFun::getChildWidgetByName(this, "User_Avar1");
		auto pos = Vec2(spHead->getContentSize().width/2, spHead->getContentSize().height/2);
		WidgetFun::setImagic(WidgetFun::getChildWidgetByName(this, "User_Avar"), "GamePlaza/HomeScene/Tou_OutSide.png", true);
		std::string headPath = "GamePlaza/HomeScene/avatar_male.png";
		std::string stencilPath = "GamePlaza/HomeScene/UserInfoPanel/headbox.png";
		_spUserHread = createCircleAvatar(spHead,	headPath,	stencilPath, pos);
	}
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
	ImagicDownManager::Instance().addDown(_spUserHread,UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());
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

cocos2d::Node* GPHomeUserInfoPanel::createCircleAvatar(cocos2d::Node* pRootNode, const std::string& filename, const std::string& stencilPath, const cocos2d::Vec2& position)
{
	auto pClip = cocos2d::ClippingNode::create();
	auto sprite = cocos2d::Sprite::create(filename);
	auto viewSize = sprite->getContentSize();
	auto stencil = cocos2d::Sprite::create(stencilPath);
	auto ctSize = stencil->getContentSize();

	if (viewSize.height < ctSize.height || viewSize.width < ctSize.width) {
		auto scaleHeight = ctSize.height * 0.9f / viewSize.height;
		auto scaleWidth = ctSize.width * 0.9f / viewSize.width;
		auto targetScale = utility::Max(scaleWidth, scaleHeight);
		sprite->setScale(targetScale);
	}
	
	stencil->setScale(1.2f,1.2f);
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
