#pragma once

#include "common.h"
#include "GUI/CCControlExtension/CCControl.h"
#include "TLJHeaders.h"

USING_NS_CC_EXT;

class TLJInfoPanel
	: public cocos2d::Node
	, public FvSingleton<TLJInfoPanel>
{
public:
	TLJInfoPanel();
	~TLJInfoPanel();

	bool init();
	
	void show();
	void hide();

private:
	//ControlSlider * m_SoundSlider;
	//ControlSlider * m_EffectSlider;
};

