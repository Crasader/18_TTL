#include "GPHomeSettingPanel.h"
#include "Game/Game/MissionWeiXin.h"
#include "common.h"
#include "constant.h"

USING_NS_CC_EXT;

FV_SINGLETON_STORAGE(GPHomeSettingPanel);
GPHomeSettingPanel::GPHomeSettingPanel()
	:m_SoundSlider(NULL)
	,m_EffectSlider(NULL)
{
    init();
}

GPHomeSettingPanel::~GPHomeSettingPanel()
{
}

bool GPHomeSettingPanel::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    WidgetScenceXMLparse settingPanel("GamePlaza/Script/GPHomeSettingPanel.xml", this);
	initButton();
	//initLoadBar();
	return true;
}

void GPHomeSettingPanel::initLoadBar()
{
	float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", Constant::DEFAULT_SOUND);
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", Constant::DEFAULT_EFFECT);
	m_SoundSlider = ControlSlider::create("GamePlaza/HomeScene/SettingPanel/progress_bg.png"
		, "GamePlaza/HomeScene/SettingPanel/progress.png"
		, "GamePlaza/HomeScene/SettingPanel/Button_Slider.png");
	m_SoundSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(GPHomeSettingPanel::Slider_Sound), Control::EventType::VALUE_CHANGED);
	m_SoundSlider->setMinimumValue(0);
	m_SoundSlider->setMaximumValue(100.0f);
	m_SoundSlider->setValue(sound_volume * 100);
	addChild(m_SoundSlider);
	auto music_node = WidgetFun::getChildWidgetByName(this,"music_silder_Node");
	m_SoundSlider->setPosition(music_node->getPosition());

	m_EffectSlider = ControlSlider::create("GamePlaza/HomeScene/SettingPanel/progress_bg.png"
		, "GamePlaza/HomeScene/SettingPanel/progress.png"
		, "GamePlaza/HomeScene/SettingPanel/Button_Slider.png");
	m_EffectSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(GPHomeSettingPanel::Slider_Effect), Control::EventType::VALUE_CHANGED);
	m_EffectSlider->setMinimumValue(0);
	m_EffectSlider->setMaximumValue(100.0f);
	m_EffectSlider->setValue(effect_volume * 100);
	addChild(m_EffectSlider);
	auto effect_node = WidgetFun::getChildWidgetByName(this,"effect_silder_Node");
	m_EffectSlider->setPosition(effect_node->getPosition());
}

void GPHomeSettingPanel::removeLoadBar()
{
	removeChild(m_SoundSlider);
	removeChild(m_EffectSlider);
	m_SoundSlider=NULL;
	m_EffectSlider=NULL;
}

void GPHomeSettingPanel::initButton()
{
	WidgetManager::addButtonCB("Button_ChangeAccount", this, button_selector(GPHomeSettingPanel::Button_ChangeAccount));
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeSettingPanel::Button_Close));
}

void GPHomeSettingPanel::hideOrShowQuitBtn(bool isShow)
{
	WidgetFun::setVisible(this,"Button_ChangeAccount",isShow);
}

void GPHomeSettingPanel::show()
{
	initLoadBar();
	/*float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", Constant::DEFAULT_SOUND);
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", Constant::DEFAULT_EFFECT);
	m_SoundSlider->setValue(sound_volume * 100);
	m_EffectSlider->setValue(effect_volume * 100);*/
    setVisible(true);
}

void GPHomeSettingPanel::hide()
{
	removeLoadBar();
    setVisible(false);
}


void GPHomeSettingPanel::Button_ChangeAccount(cocos2d::Ref*, WidgetUserInfo*)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	exit(0);
    
	MissionWeiXin::Instance().restWeiXin();
	cocos2d::UserDefault::getInstance()->setStringForKey("Accounts","");
	cocos2d::UserDefault::getInstance()->setStringForKey("Password","");
	GPSceneManager::Instance().InLogonScene();
	
#else
	cocos2d::Director::getInstance()->end();
#endif
	
}

void GPHomeSettingPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
    hide();
}

void GPHomeSettingPanel::Slider_Sound(cocos2d::Ref* soundSlider, cocos2d::extension::Control::EventType eventType)
{
	ControlSlider* slider = static_cast<ControlSlider *> (soundSlider);
	if (slider) {
		float value = slider->getValue() / slider->getMaximumValue();
		cocos2d::UserDefault::getInstance()->setFloatForKey("sound_volume", value);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value*100);
		SoundFun::Instance().SetSoundBackground(value);
	}
}

void GPHomeSettingPanel::Slider_Effect(cocos2d::Ref* effectSlider, cocos2d::extension::Control::EventType)
{
	ControlSlider* slider = static_cast<ControlSlider *> (effectSlider);
	if (slider) {
		float value = slider->getValue() / slider->getMaximumValue();
		cocos2d::UserDefault::getInstance()->setFloatForKey("effect_volume", value);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(value*100);
		SoundFun::Instance().SetSoundEffect(value);
		cocos2d::UserDefault::getInstance()->setBoolForKey("NN_Voice",slider->getValue()>0);
	}
}
