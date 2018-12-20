#include <Tools/utilityString.h>

#include "NNSound.h"
 

std::string NNSound::getVoicePath()
{
	tagGlobalUserData * pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	if (pGlobalUserData->cbGender) {
		return utility::toString(VOICE_PATH, MAN_PATH);
	} else {
		return utility::toString(VOICE_PATH, WOMEN_PATH);
	}
}

void NNSound::playNN(TTLNN::NNCardType cardType,NNPlayer* pPlayer)
{
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;
	if (canPlayEffect) {
		std::string voicePath = utility::toString(NNSound::VOICE_PATH, "NN_", cardType - TTLNN::NNCardType_None, FILE_TAIL);
		SoundFun::Instance().playEffect(voicePath);
	}
}

void NNSound::playBet(int bets)
{
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;// cocos2d::UserDefault::getInstance()->getBoolForKey("NN_Voice");
	if (canPlayEffect) {
		NNSound::playEffect(NNSound::USER_CALL);
	}
}

void NNSound::playSnatch(int snatchRatio)
{
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;// cocos2d::UserDefault::getInstance()->getBoolForKey("NN_Voice");
	if (canPlayEffect) {
		NNSound::playEffect(NNSound::USER_CALL);
	}
}

void NNSound::playBackMusic(std::string music)
{
	bool canPlayMusic =cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume",1.f) > 0;// cocos2d::UserDefault::getInstance()->getBoolForKey("NN_Music");
	if (canPlayMusic) {
		music = utility::toString(VOICE_PATH, music);
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(music.c_str(),true);
	}
}

void NNSound::stopBackMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void NNSound::playEffect(std::string effectName)
{
	//bool canPlayEffect = cocos2d::UserDefault::getInstance()->getBoolForKey("NN_Voice");
	bool canPlayEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume",1.f) > 0;
	if (canPlayEffect) {
		std::string voicePath = utility::toString(VOICE_PATH, effectName);
		SoundFun::Instance().playEffect(voicePath);
	}
}

std::string NNSound::getDefineSound(int iGender,std::string kStr)
{
	std::string strValue = utility::toString("fix_msg_",kStr);
	if (iGender==UserSex::US_Male)
	{
		strValue = utility::toString("man/",strValue);
	}
	else if (iGender==UserSex::US_Femal)
	{
		strValue =utility::toString("woman/",strValue);
	}
	else
	{
		strValue =utility::toString("woman/",strValue);
	}
	return utility::toString("sound/",strValue,".mp3");
}

