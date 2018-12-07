#include "GPHomeSharePanel.h"
#include "GameLib/Game/Game/MissionWeiXin.h"
#include "GameLib/Game/Script/utility.h"
#include "GameLib/Tools/tools/Convert.h"

FV_SINGLETON_STORAGE(GPHomeSharePanel);
GPHomeSharePanel::GPHomeSharePanel()
{
	init();
}

GPHomeSharePanel::~GPHomeSharePanel()
{
}

bool GPHomeSharePanel::init() {
	if (!cocos2d::Node::init()) {
		return false;
	}
	
	WidgetScenceXMLparse sharePanel("GamePlaza/Script/GPHomeSharePanel.xml", this);
	initButton();
	return true;
}

void GPHomeSharePanel::initButton() {
	WidgetManager::addButtonCB("Button_ShareToFriend", this, button_selector(GPHomeSharePanel::Button_ShareToFriend));
	WidgetManager::addButtonCB("Button_ShareToFriendCircle", this, button_selector(GPHomeSharePanel::Button_ShareToFriendCircle));
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeSharePanel::Button_Close));
}

void GPHomeSharePanel::show()
{
	setVisible(true);
}

void GPHomeSharePanel::hide()
{
	setVisible(false);
}

void GPHomeSharePanel::Button_ShareToFriend(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	std::string strUrl = utility::a_u8("http://114.115.164.158:8080/evokeapp.html");
	std::string strTitle = ScriptData<std::string>("CCWeiXinShare_Game_Name").Value();
	std::string strIntroduce = ScriptData<std::string>("CCWeiXinShare_introduce").Value();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	MissionWeiXin::Instance().shareUrlWeiXin(strUrl,strTitle,strIntroduce, MissionWeiXin::SHARE_SESSION);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	MissionWeiXin::Instance().shareUrlWeiXin(strUrl,strTitle,strIntroduce, MissionWeiXin::SHARE_SESSION);
#endif
}

void GPHomeSharePanel::Button_ShareToFriendCircle(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	std::string strUrl = utility::a_u8("http://114.115.164.158:8080/evokeapp.html");
	std::string strTitle = ScriptData<std::string>("CCWeiXinShare_Game_Name").Value();
	std::string strIntroduce = ScriptData<std::string>("CCWeiXinShare_introduce").Value();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	MissionWeiXin::Instance().shareUrlWeiXin(strUrl, strTitle, strIntroduce, MissionWeiXin::SHARE_MOMENTS);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	MissionWeiXin::Instance().shareUrlWeiXin(strUrl, strTitle, strIntroduce, MissionWeiXin::SHARE_MOMENTS);
#endif
}

void GPHomeSharePanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}
