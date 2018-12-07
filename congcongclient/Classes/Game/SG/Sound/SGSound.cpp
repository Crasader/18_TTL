#include "SGSound.h"
#include "SimpleAudioEngine.h"
#include "GameLib/Platform/PFDefine/df/types.h"

std::string SGSound::getVoicePath()
{
	tagGlobalUserData * pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	if (pGlobalUserData->cbGender) {
		return utility::toString(SGSound::VOICE_PATH, SGSound::MAN_PATH);
	} else {
		return utility::toString(SGSound::VOICE_PATH, SGSound::WOMEN_PATH);
	}
}

void SGSound::playSG(SG::SGCardType_Result result,SGPlayer* pPlayer)
{
	//bool canPlayEffect = cocos2d::UserDefault::getInstance()->getBoolForKey("SG_Voice");
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;
	if (canPlayEffect) {
		std::string sGVoice ="";
		if(result.type == SG::SGCardType_None)
			sGVoice = utility::toString("SG_",(int)result.cardPoint,SGSound::FILE_TAIL);
		else if(result.type == SG::SGCardType_HSG)
			sGVoice = utility::toString("SG_",10,SGSound::FILE_TAIL);
		else if(result.type > SG::SGCardType_HSG)
			sGVoice = utility::toString("SG_",11,SGSound::FILE_TAIL);
		std::string voicePath = utility::toString(SGSound::VOICE_PATH,pPlayer->GetGender() ? SGSound::MAN_PATH:SGSound::WOMEN_PATH,sGVoice);
		SoundFun::Instance().playEffect(voicePath);
	}
}

void SGSound::playBet(int bets)
{
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;// cocos2d::UserDefault::getInstance()->getBoolForKey("SG_Voice");
	if (canPlayEffect) {
		SGSound::playEffect(SGSound::USER_CALL);
	}
}

void SGSound::playSnatch(int snatchRatio)
{
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;// cocos2d::UserDefault::getInstance()->getBoolForKey("SG_Voice");
	if (canPlayEffect) {
		SGSound::playEffect(SGSound::USER_CALL);
	}
}

void SGSound::playBackMusic(std::string music)
{
	bool canPlayMusic =cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume",1.f) > 0;// cocos2d::UserDefault::getInstance()->getBoolForKey("SG_Music");
	if (canPlayMusic) {
		music = utility::toString(SGSound::VOICE_PATH, music);
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(music.c_str(),true);
	}
}

void SGSound::stopBackMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void SGSound::playEffect(std::string effectName)
{
	//bool canPlayEffect = cocos2d::UserDefault::getInstance()->getBoolForKey("SG_Voice");
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;
	if (canPlayEffect) {
		std::string voicePath = utility::toString(SGSound::VOICE_PATH, effectName);
		SoundFun::Instance().playEffect(voicePath);
	}
}

std::string SGSound::getDefineSound(int iGender,std::string kStr)
{
	std::string strValue = utility::toString("fix_msg_",kStr);
	if (iGender == UserSex::US_Male )
	{
		strValue = utility::toString("man/",strValue);
	}
	else if(iGender == UserSex::US_Femal)
	{
		strValue =utility::toString("woman/",strValue);
	}
	else
	{
		strValue =utility::toString("woman/",strValue);
	}
	return utility::toString("sound/",strValue,".mp3");
}

