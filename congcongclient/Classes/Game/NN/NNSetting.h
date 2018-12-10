#pragma once
#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetManager.h"
#include "Plaza/HomeScene/GPHomeExitPanel.h"
#include "Plaza/GameManager/GPSceneManager.h"
#include "cocos-ext.h"
#include "GUI/CCControlExtension/CCControl.h"
#include "SimpleAudioEngine.h"

USING_NS_CC_EXT;
class NNSetting
	: public cocos2d::Node
	, public FvSingleton<NNSetting>
{
public:
	NNSetting();
	~NNSetting();

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

