#include "SRLFSetting.h"
#include "constant.h"

USING_NS_CC_EXT;

FV_SINGLETON_STORAGE(SRLFSetting);
SRLFSetting::SRLFSetting()
	:m_EffectSlider(NULL)
	,m_SoundSlider(NULL)
{
	init();
}

SRLFSetting::~SRLFSetting()
{
}

bool SRLFSetting::init()
{
	if(!cocos2d::Node::init()) {
		return false;
	}

	WidgetScenceXMLparse SRLFSettingPanel("GameSRLF/xml/SRLFSetting.xml", this);

	//WidgetManager::addButtonCB("Button_ChangeAccount", this, button_selector(SRLFSetting::Button_ChangeAccount));
	WidgetManager::addButtonCB("Button_Close", this, button_selector(SRLFSetting::Button_Close));
	WidgetFun::setVisible(this,"Button_ChangeAccount",false);
	float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", Constant::DEFAULT_SOUND);
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", Constant::DEFAULT_EFFECT);
	m_SoundSlider = ControlSlider::create("GamePlaza/HomeScene/progress_bg.png"
		, "GamePlaza/HomeScene/progress.png"
		, "GamePlaza/HomeScene/Button_Slider.png");
	m_SoundSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SRLFSetting::Slider_Sound), Control::EventType::VALUE_CHANGED);
	m_SoundSlider->setMinimumValue(0);
	m_SoundSlider->setMaximumValue(100.0f);
	m_SoundSlider->setValue(sound_volume * 100);
	addChild(m_SoundSlider);
	auto music_node = WidgetFun::getChildWidgetByName(this,"music_silder_Node");
	m_SoundSlider->setPosition(music_node->getPosition());

	m_EffectSlider = ControlSlider::create("GamePlaza/HomeScene/progress_bg.png"
		, "GamePlaza/HomeScene/progress.png"
		, "GamePlaza/HomeScene/Button_Slider.png");
	m_EffectSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SRLFSetting::Slider_Effect), Control::EventType::VALUE_CHANGED);
	m_EffectSlider->setMinimumValue(0);
	m_EffectSlider->setMaximumValue(100.0f);
	m_EffectSlider->setValue(effect_volume * 100);
	addChild(m_EffectSlider);
	auto effect_node = WidgetFun::getChildWidgetByName(this,"effect_silder_Node");
	m_EffectSlider->setPosition(effect_node->getPosition());
	return true;
}

void SRLFSetting::initButton()
{

}

void SRLFSetting::show()
{
	float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", Constant::DEFAULT_SOUND);
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", Constant::DEFAULT_EFFECT);
	m_SoundSlider->setValue(sound_volume * 100);
	m_EffectSlider->setValue(effect_volume * 100);
	setVisible(true);
}

void SRLFSetting::hide()
{
	setVisible(false);
}


void SRLFSetting::Button_ChangeAccount(cocos2d::Ref*, WidgetUserInfo*)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	exit(0);
#else
	cocos2d::Director::getInstance()->end();
#endif

}

void SRLFSetting::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void SRLFSetting::Slider_Sound(cocos2d::Ref* soundSlider, cocos2d::extension::Control::EventType eventType)
{
	ControlSlider* slider = static_cast<ControlSlider *> (soundSlider);
	if (slider) {
		float value = slider->getValue() / slider->getMaximumValue();
		cocos2d::UserDefault::getInstance()->setFloatForKey("sound_volume", value);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
		SoundFun::Instance().SetSoundBackground(value);
	}
}

void SRLFSetting::Slider_Effect(cocos2d::Ref* effectSlider, cocos2d::extension::Control::EventType)
{
	ControlSlider* slider = static_cast<ControlSlider *> (effectSlider);
	if (slider) {
		float value = slider->getValue() / slider->getMaximumValue();
		cocos2d::UserDefault::getInstance()->setFloatForKey("effect_volume", value);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(value);
		SoundFun::Instance().SetSoundEffect(value);
	}
}