#pragma once

#include "common.h"
#include "GUI/CCControlExtension/CCControl.h"
#include "SimpleAudioEngine.h"
#include "Game/Script/SoundFun.h"

USING_NS_CC_EXT;
class TLJSettingPanel
	: public cocos2d::Node
	, public FvSingleton<TLJSettingPanel>
{
public:
	TLJSettingPanel();
	~TLJSettingPanel();

	bool init();
	void initButton();

	void show();
	void hide();

	void Button_ChangeAccount(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

	void Slider_Sound(cocos2d::Ref* ref, cocos2d::extension::Control::EventType eventType);
	void Slider_Effect(cocos2d::Ref* ref, cocos2d::extension::Control::EventType eventType);

private:
	ControlSlider* m_SoundSlider;
	ControlSlider* m_EffectSlider;
};

