#include "GPLoginScene.h"
#include "common.h"
#include "constant.h"
#include "UserProtocol.h"

#include USERINFO
 
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
    if(!cocos2d::CCNode::init()) {
        return false;
    };

	//bool bPlayLoginAni = ScriptData<bool>("PlayLoginAnimation").Value();

    WidgetScenceXMLparse xmlScenceLogin("GamePlaza/Script/GPLoginScene.xml", this);

    WidgetManager::addButtonCB("Button_WeiXinLogon", this, button_selector(GPLoginScene::Button_WeiXinLogon));
    WidgetManager::addButtonCB("Button_UserXieYiCheak", this, button_selector(GPLoginScene::Button_UserXieYiCheak));
    WidgetManager::addButtonCB("Button_UserXieYi", this, button_selector(GPLoginScene::Button_UserXieYi));
	
    WidgetFun::setEnable(this, "Button_WeiXinLogon", bXieYiChecked);
	WidgetFun::setChecked(this, "Button_UserXieYiCheak", bXieYiChecked);

	this->addChild(UserProtocol::pInstance());
	UserProtocol::pInstance()->hide();

    return true;
}

void GPLoginScene::setEnableButtons(bool flag)
{
	auto panel = WidgetFun::getChildWidget(this, "LogonScencePlane");
	dynamic_cast<cocos2d::ui::Button*>(WidgetFun::getChildWidget(panel, "Button_WeiXinLogon"))->setTouchEnabled(flag);
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
	if (kAccounts != "" && kPassword != "") 
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		std::string result = GetWxLoginWin32();
		kAccounts = "WeiXinoef1Y1jKLussTDkIAaMpdXHdH";// utility::split(result, ":")[0];
		kPassword = "WeixinPassword";//utility::split(result,":")[1];
#endif
        m_kPssword = kPassword;
        CMD_GP_LogonAccounts loginAccount;
        loginAccount.dwPlazaVersion = DF::shared()->GetPlazaVersion();//ScriptData<int>("PlazaVersion").Value();//2000;
        loginAccount.cbValidateFlags = LOW_VER_VALIDATE_FLAGS;
        strcpy(loginAccount.szAccounts, kAccounts.c_str());
        strcpy(loginAccount.szPassword, kPassword.c_str());
        m_kLoginMission.loginAccount(loginAccount);
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    else
    {
		onWxLoginSuccess(m_kWeiXinUserInfo);
    }
#endif
}

void GPLoginScene::RegisterAccount()
{
	#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
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
    std::string kAccounts = utility::split(result,":")[0];
	std::string kPssword  = utility::split(result,":")[1];
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
	// Î¢ÐÅÐÔ±ð
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

	//account = "WeiXinoznOM0oURRnxOpbFnZdxsyxRU";
	//pass = "WeiXinPassword";

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
