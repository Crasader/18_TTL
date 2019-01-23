#include "GPLoginScene.h"
#include "common.h"
#include "constant.h"
#include "UserProtocol.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "IosHelper.h"
#endif
#include USERINFO
#include UTILITY_STRING
#include UTILITY_CONVERT

FV_SINGLETON_STORAGE(GPLoginScene);

GPLoginScene::GPLoginScene()
    : m_kLoginMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
    , m_kPopularizeMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
	, bXieYiChecked (true)
{
    MissionWeiXin::Instance().setMissionSink(this);
    m_kLoginMission.setMissionSink(this);
	m_kPopularizeMission.setMissionSink(this);
    init();
}

GPLoginScene::~GPLoginScene()
{
}

bool GPLoginScene::init()
{
	if (!cocos2d::CCNode::init()) {
		return false;
	};

	//bool bPlayLoginAni = ScriptData<bool>("PlayLoginAnimation").Value();

	WidgetScenceXMLparse xmlScenceLogin("GamePlaza/Script/GPLoginScene.xml", this);

#ifdef ENABLE_WEIXIN
	if (Constant::WEIXIN_INSTALL) {
		WidgetManager::addButtonCB("Button_WeiXinLogon", this, button_selector(GPLoginScene::Button_WeiXinLogon));
		WidgetFun::setEnable(this, "Button_WeiXinLogon", bXieYiChecked);
		WidgetFun::setChecked(this, "Button_UserXieYiCheak", bXieYiChecked);
		WidgetManager::addButtonCB("Button_UserXieYiCheak", this, button_selector(GPLoginScene::Button_UserXieYiCheak));
		WidgetManager::addButtonCB("Button_UserXieYi", this, button_selector(GPLoginScene::Button_UserXieYi));
	} else {
		WidgetFun::setVisible(this, "Button_WeiXinLogon", false);
		WidgetFun::setVisible(this, "Button_UserXieYiCheak", false);
		WidgetFun::setVisible(this, "Button_UserXieYi", false);
	}
#endif

	this->addChild(UserProtocol::pInstance());
	UserProtocol::pInstance()->hide();

    return true;
}

void GPLoginScene::setEnableButtons(bool flag)
{
	auto panel = WidgetFun::getChildWidget(this, "LogonScencePlane");
#ifdef ENABLE_WEIXIN
	if(Constant::WEIXIN_INSTALL)
		dynamic_cast<cocos2d::ui::Button*>(WidgetFun::getChildWidget(panel, "Button_WeiXinLogon"))->setTouchEnabled(flag);
#endif
	dynamic_cast<cocos2d::ui::Button*>(WidgetFun::getChildWidget(panel, "Button_UserXieYi"))->setTouchEnabled(flag);
	dynamic_cast<cocos2d::ui::CheckBox*>(WidgetFun::getChildWidget(panel, "Button_UserXieYiCheak"))->setTouchEnabled(flag);
}

void GPLoginScene::EnterScene()
{
	float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", Constant::DEFAULT_SOUND);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(sound_volume);
	SoundFun::Instance().SetSoundBackground(sound_volume);
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", Constant::DEFAULT_EFFECT);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effect_volume);
	SoundFun::Instance().SetSoundEffect(effect_volume);
	SoundFun::Instance().playBackMusic("bgplay.mp3");

	std::string kAccounts = cocos2d::UserDefault::getInstance()->getStringForKey("Accounts");
    std::string kPassword = cocos2d::UserDefault::getInstance()->getStringForKey("Password");
	if (kAccounts != "" || kPassword != "") {

        m_kPssword = kPassword;
        CMD_GP_LogonAccounts loginAccount;
        loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();//ScriptData<int>("PlazaVersion").Value();//2000;
        loginAccount.cbValidateFlags = LOW_VER_VALIDATE_FLAGS;
        strcpy(loginAccount.szAccounts, kAccounts.c_str());
        strcpy(loginAccount.szPassword, kPassword.c_str());
        m_kLoginMission.loginAccount(loginAccount);

	} else {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

		//ע�����
		RegisterAccount();
		return;

#endif

	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

	onWxLoginSuccess(m_kWeiXinUserInfo);

#endif

}

void GPLoginScene::RegisterAccount()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	//std::string kAccounts = "WeiXin" + m_kWeiXinUserInfo.openid;
	std::string kAccounts = "WeiXin" + m_kWeiXinUserInfo.unionid;
	m_kPssword = "WeiXinPassword";
	bool isGuest = false;
	//if (Constant::WEIXIN_INSTALL) {
	//	CCAssert(m_kWeiXinUserInfo.openid != "", "");
	//	if (m_kWeiXinUserInfo.openid == "") {
	//		return;
	//	}
	//} 
	//else
	{
		std::string time_str = utility::toString(time(nullptr));
		std::string rand_str = utility::toString(rand() % 900 + 100);
		kAccounts = time_str.substr(time_str.size() - 7, 7) + rand_str;
		m_kPssword = rand_str;
		isGuest = true;
	}

	CMD_GP_RegisterAccounts kRegister;
	zeromemory(&kRegister, sizeof(kRegister));
	kRegister.dwPlazaVersion = DF::shared()->GetPlazaVersion();//ScriptData<int>("PlazaVersion").Value();//2000;
	kRegister.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	kRegister.cbGender = 0;
	kRegister.wFaceID = 0;
	strncpy(kRegister.szAccounts, kAccounts.c_str(), kAccounts.size());
	strncpy(kRegister.szLogonPass, m_kPssword.c_str(), m_kPssword.size());
	if (isGuest) {
		std::string kNickName = utility::a_u8(utility::toString("玩家", kAccounts));
		strncpy(kRegister.szNickName, kNickName.c_str(), kNickName.size());
	} else {
		std::string kNickName = (m_kWeiXinUserInfo.nickname);
		strncpy(kRegister.szNickName, kNickName.c_str(), kNickName.size());
	}
	m_kLoginMission.registerServer(kRegister);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    //std::string kAccounts = "WeiXin" + m_kWeiXinUserInfo.openid;
    std::string kAccounts = "WeiXin" + m_kWeiXinUserInfo.unionid;
    m_kPssword = "WeiXinPassword";
    bool isGuest = false;
    if(IosHelper::isWinXinInstalled())
	{
        //CCAssert(m_kWeiXinUserInfo.unionid != "", "");
        if (m_kWeiXinUserInfo.unionid == "") {
            return;
        }
    }
    else
    {
        std::string time_str = utility::toString(time(nullptr));
        std::string rand_str = utility::toString(rand() % 900 + 100);
        kAccounts = time_str.substr(time_str.size() - 7, 7) + rand_str;
        m_kPssword = rand_str;
        isGuest = true;
    }
    
    CMD_GP_RegisterAccounts kRegister;
    zeromemory(&kRegister, sizeof(kRegister));
    kRegister.dwPlazaVersion = DF::shared()->GetPlazaVersion();//ScriptData<int>("PlazaVersion").Value();//2000;
    kRegister.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
    kRegister.cbGender = 0;
    kRegister.wFaceID = 0;
    strncpy(kRegister.szAccounts, kAccounts.c_str(), kAccounts.size());
    strncpy(kRegister.szLogonPass, m_kPssword.c_str(), m_kPssword.size());
    if(isGuest){
        std::string kNickName = utility::a_u8(utility::toString("�ο�", kAccounts));
        strncpy(kRegister.szNickName, kNickName.c_str(), kNickName.size());
    }else{
        std::string kNickName = (m_kWeiXinUserInfo.nickname);
        strncpy(kRegister.szNickName, kNickName.c_str(), kNickName.size());
    }
    m_kLoginMission.registerServer(kRegister);
    
#elif
    
	CCAssert(m_kWeiXinUserInfo.openid != "", "");
	if (m_kWeiXinUserInfo.openid == "") {
		return;
	}
	//std::string kAccounts = "WeiXin" + m_kWeiXinUserInfo.openid;
	std::string kAccounts = "WeiXin" + m_kWeiXinUserInfo.unionid;
	m_kPssword = "WeiXinPassword";
	CMD_GP_RegisterAccounts kRegister;
	zeromemory(&kRegister, sizeof(kRegister));
	kRegister.dwPlazaVersion = DF::shared()->GetPlazaVersion();//ScriptData<int>("PlazaVersion").Value();//2000;
	kRegister.cbValidateFlags = MB_VALIDATE_FLAGS | LOW_VER_VALIDATE_FLAGS;
	kRegister.cbGender = 0;
	kRegister.wFaceID = 0;
	strncpy(kRegister.szAccounts, kAccounts.c_str(), kAccounts.size());
	strncpy(kRegister.szLogonPass, m_kPssword.c_str(), m_kPssword.size());
	std::string kNickName = (m_kWeiXinUserInfo.nickname);
	strncpy(kRegister.szNickName, kNickName.c_str(), kNickName.size());
	m_kLoginMission.registerServer(kRegister);

#endif

}

void GPLoginScene::onWxLoginSuccess(WxUserInfo kWxUserInfo)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	std::string result = GetWxLoginWin32();
	auto tocken = utility::split(result, ":");
	if (tocken.size() != 2) {
		return;
	}
    std::string kAccounts = tocken[0];
	std::string kPssword  = tocken[1];
	CMD_GP_LogonAccounts loginAccount;
	loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();//ScriptData<int>("PlazaVersion").Value();//2000;
	loginAccount.cbValidateFlags = LOW_VER_VALIDATE_FLAGS;
	strcpy(loginAccount.szAccounts, kAccounts.c_str());
	strcpy(loginAccount.szPassword, kPssword.c_str());
	m_kLoginMission.loginAccount(loginAccount);
#else
	m_kWeiXinUserInfo = kWxUserInfo;
	std::string kAccounts = "WeiXin"+m_kWeiXinUserInfo.unionid;//m_kWeiXinUserInfo.openid;
	m_kPssword = "WeiXinPassword";
	CMD_GP_LogonAccounts loginAccount;
	loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();//ScriptData<int>("PlazaVersion").Value();//2000;
	loginAccount.cbValidateFlags = LOW_VER_VALIDATE_FLAGS;
	strcpy(loginAccount.szAccounts, kAccounts.c_str());
	strcpy(loginAccount.szPassword, m_kPssword.c_str());

	cocos2d::log("HNLogonScence::onWxLoginSuccess account = %s loginAccount.szPassword = %s",loginAccount.szAccounts,loginAccount.szPassword);
	m_kLoginMission.loginAccount(loginAccount);

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	// ΢���Ա�
	pGlobalUserData->cbGender = kWxUserInfo.sex;
	strncpy(pGlobalUserData->szHeadHttp,kWxUserInfo.headimgurl.c_str(), countarray(pGlobalUserData->szHeadHttp));

	CCLOG("onWxLoginSuccess headimgurl = %s", kWxUserInfo.headimgurl.c_str());
	UserDefault::sharedUserDefault()->setStringForKey("headimgurl",kWxUserInfo.headimgurl.c_str());
	//UserDefault::sharedUserDefault()->flush();
#endif
}

void GPLoginScene::onWxLoginFail(std::string kError)
{
}

std::string GPLoginScene::GetWxLoginWin32()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	int IndexMin = ScriptData<int>("IndexMin").Value();
	int IndexMax = ScriptData<int>("IndexMax").Value();
	int num = cocos2d::UserDefault::getInstance()->getIntegerForKey("Index", IndexMin);
	bool isLocked = ScriptData<bool>("AccountLocked").Value();
	std::string account("");
	if( isLocked == true)
	{
		num = ScriptData<int>("Account").Value();
	}else
	{
		if (num > IndexMax ) num = IndexMin;
	}
	if(num < 10)
		account = utility::toString("test00",num);
	else if(num < 100)
		account = utility::toString("test0",num);
	else
		account = utility::toString("test",num);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("Index", num + 1);
	cocos2d::UserDefault::getInstance()->flush();

	std::string pass = "111111";
	//account = cocos2d::UserDefault::getInstance()->getStringForKey("Accounts");
	//pass = cocos2d::UserDefault::getInstance()->getStringForKey("Password");

	//account = "WeiXinoznOM0oURRnxOpbFnZdxsyxRU";
	//pass = "WeiXinPassword";

	//account = "test011";

	std::string tocken = utility::toString(account, ":",  pass);
	return tocken;

#endif
}

void GPLoginScene::Button_WeiXinLogon(cocos2d::Ref*, WidgetUserInfo*)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    onWxLoginSuccess(m_kWeiXinUserInfo);
#else
    cocos2d::log("kuili:-----HNLogonScence::Button_WeiXinLogon call---------");
    MissionWeiXin::Instance().logonWeiXin();
#endif
}

void GPLoginScene::Button_UserXieYiCheak(cocos2d::Ref*, WidgetUserInfo*)
{
    bXieYiChecked = !bXieYiChecked;
    WidgetFun::setEnable(this, "Button_WeiXinLogon", bXieYiChecked);
}

void GPLoginScene::Button_UserXieYi(cocos2d::Ref*, WidgetUserInfo*)
{
	setEnableButtons(false);
	UserProtocol::Instance().show();
}
