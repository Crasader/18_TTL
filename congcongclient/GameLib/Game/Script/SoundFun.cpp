#include "SoundFun.h"
#include "SimpleAudioEngine.h"
#include "TimeManager.h"

//using namespace script;

FV_SINGLETON_STORAGE(SoundFun);

SoundFun::SoundFun()
{
	s_soundPath = "";
	s_bMute = false;
	s_fSound1 = 0;
	s_fSound2 = 0;
	m_bPlayButton = true;
}

void SoundFun::SetSoundBackground(float fValue)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume( fValue*(!s_bMute));
	s_fSound1 = fValue;
}

void SoundFun::SetSoundEffect(float fValue)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume( fValue *(!s_bMute));
	s_fSound2 = fValue;
}

void SoundFun::playBackMusic(std::string kName)
{
	if (m_kBackSound == kName)
	{
		return;
	}
	m_kBackSound = kName;
	kName = s_soundPath + kName;
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(kName.c_str(),true);	
}

void SoundFun::stopBackMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();	
}

void SoundFun::playEffect(std::string kName,bool bPreload)
{
	if (s_bMute)
	{
		return;
	}
	kName = s_soundPath + kName;
	if (bPreload)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kName.c_str());
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(kName.c_str());
	}
}

void SoundFun::playEffectDirect(std::string kName,bool bPreload )
{

	if (s_bMute)
	{
		return;
	}
	if (bPreload)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(kName.c_str());
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(kName.c_str());
	}
}

void SoundFun::playCardByGender(int iGender,std::string kName,bool bPreload)
{
	if (iGender==1)
	{
		kName = "F_" + kName;
	}
	else
	{
		kName = "M_" + kName;
	}
	playEffect(kName,bPreload);
}

void SoundFun::PreloadEffectByGender(std::string kName)
{
	playCardByGender(0,kName,true);
	playCardByGender(1,kName,true);
}

void SoundFun::setMute(bool bMute)
{
	if (s_bMute == bMute)
	{
		return;
	}
	SetSoundBackground(s_fSound1);
	SetSoundEffect(s_fSound2);
	s_bMute = bMute;
	cocos2d::UserDefault::getInstance()->setBoolForKey("Mute",bMute);
}

void SoundFun::playCustomEffect(std::string kName)
{
	if (s_bMute)
	{
		return;
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(kName.c_str());
}

void SoundFun::PaseBackMusic()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SoundFun::OnResumeBackMusic,this));
#endif
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void SoundFun::OnResumeBackMusic()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SoundFun::OnResumeBackMusic,this));
#endif
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void SoundFun::ResumeBackMusic(float fTime)
{
	float fTempValue =  s_fSound2*10.0f;
	if(fTempValue > 1.0f)
	{
		fTempValue = 1.0f;
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(fTempValue*(!s_bMute));
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SoundFun::OnResumeBackMusic,this));
#endif
	if (fTime > 0.01f)
	{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SoundFun::OnResumeBackMusic,this),fTime);
#endif
	}
	else
	{
		OnResumeBackMusic();
	}
}

void SoundFun::PaseEffectMusic()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SoundFun::OnResumeEffectMusic,this));
#endif
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void SoundFun::OnResumeEffectMusic()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SoundFun::OnResumeEffectMusic,this));
#endif
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void SoundFun::ResumeEffectMusic(float fTime)
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SoundFun::OnResumeEffectMusic,this));
#endif
	if (fTime > 0.01f)
	{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SoundFun::OnResumeEffectMusic,this),fTime);
#endif
	}
	else
	{
		OnResumeEffectMusic();
	}
}

void SoundFun::setPlayButton(bool bNeed)
{
	m_bPlayButton = bNeed;
}

void SoundFun::playButtonEffect()
{
	if (m_bPlayButton )
	{
		playEffect("sound/Audio_Button_Click.mp3");
	}
}
