#include "EffectScence.h"

#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

FV_SINGLETON_STORAGE(EffectScence);

EffectScence::EffectScence()
{
	init();
	WidgetScenceXMLparse xml("Script/GoldEffect.xml",this);
}
EffectScence::~EffectScence()
{

}
bool EffectScence::init()
{
	if (!Node::init())
	{
		return false;
	};
	return true;
}

void EffectScence::ShowGoldEffect()
{
	WidgetManager::Instance().createWidget("GoldEffect",this);
	//SoundFun::playEffect("Animation_CoinAudio.mp3");
}
