#include "WZQDSoundFun.h"
#include "SimpleAudioEngine.h"
#include "Game/Script/utility.h"
#include "GameLib/Tools/tools/Convert.h"
#include "Game/Script/SoundFun.h"
#include "WZQDGameLogic.h"
#include "GameLib/Platform/PFDefine/df/types.h"
namespace WZQSoundFun
{
	static const std::string LG_PATH_SC = "GameWZQ/sound/";
	static const std::string LG_FLAG_SC = "sc_";
	static const std::string LG_FLAG_PT = "putong/";

	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	
	void setPuTong(bool bPuTong)
	{
		if (bPuTong)
		{
//			LG_FLAG_HN = "putong/";
		}
		else
		{
//			LG_FLAG_HN = "changsha/";
		}
	}

	void playBackMusic(std::string kName)
	{
		kName = utility::toString(LG_PATH_SC,kName);
		SoundFun::Instance().playBackMusic(utility::a_u8(kName));
		cocos2d::log("XZDDSoundFun playBackMusic :%s",utility::a_u8(kName).c_str());
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::toString(LG_PATH_SC,LG_FLAG_SC,kName,RAW_KIND);
		SoundFun::Instance().playEffect(kName,bPreload);
		cocos2d::log("XZDDSoundFun:%s",kName.c_str());
	}

	void playCardByGender(int iGender,int nCard)
	{
		return;
		std::string kName;
		if (iGender)
		{
			kName = utility::toString("man","_");
		}
		playEffect(kName);
	}

	void playEffectByGender(int iGender,std::string kName)
	{
		if (iGender == UserSex::US_Femal)
		{
			int nNum = EFFECT_NUM;
			if (kName == EFFECT_AN_GANG)
			{
				nNum = EFFECT_NUM-1;
			}
			else if (kName == EFFECT_PENG)
			{
				nNum = EFFECT_NUM+1;
			}
			kName =utility::toString("woman_",kName,rand()%nNum);
		}
		else if (iGender == UserSex::US_Male)
		{

			int nNum = EFFECT_NUM;
			if (kName == EFFECT_PENG)
			{
				nNum = EFFECT_NUM+1;
			}
			kName =utility::toString("man_",kName,rand()%nNum);
		}
		playEffect(kName);
	}

	void playEffectEX(std::string kName)
	{
		SoundFun::Instance().Instance().playEffect(kName);
	}

	std::string getDefineSound(int iGender,std::string kStr)
	{
		std::string strValue = utility::toString("fix_msg_",kStr);
		if (iGender)
		{
			strValue = utility::toString("man/",strValue);
		}
		else
		{
			strValue =utility::toString("man/",strValue);
		}
		return utility::toString(LG_PATH_SC,LG_FLAG_PT,strValue,RAW_KIND);
	}
} 
