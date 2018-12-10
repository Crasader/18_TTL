#include "TLJSoundFun.h"
#include "UTILITY.h"
#include "Game/Script/SoundFun.h"
#include "TLJGameLogic.h"
#include "Platform/PFDefine/df/types.h"

namespace TLJSoundFun
{
	static std::string LG_PATH_HN = "Game/TLJ/sound/";
	static std::string LG_FLAG_HN = "menji/";

	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	void playBackMusic(std::string kName)
	{
		kName = utility::toString(LG_PATH_HN, kName);
		SoundFun::Instance().playBackMusic(kName);
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName, bool bPreload)
	{
		kName = utility::toString(LG_PATH_HN, LG_FLAG_HN,kName, RAW_KIND);
		SoundFun::Instance().playEffect(kName, bPreload);
	}

	std::string getDefineSound(int iGender, std::string kStr)
	{
		std::string strValue = utility::toString("fix_msg_",kStr);

		if (iGender==UserSex::US_Male)
		{
			strValue = utility::toString("man/",strValue);
		}
		else if(iGender==UserSex::US_Femal)
		{
			strValue =utility::toString("woman/",strValue);
		}
		else
		{
			strValue =utility::toString("woman/",strValue);
		}
		return utility::toString("sound/",strValue,RAW_KIND);
	}

	void playCardByGender(int iGender, int nCard)
	{
		/*int nColor = TLJGameLogic::Instance().GetCardColor(nCard);
		int nValue = TLJGameLogic::Instance().GetCardValue(nCard);
		std::string strValue = utility::toString(nValue);
		std::string strColor;
		if (nColor == CARD_XIAO)
		{
			strColor = "mjt1_";
		}
		else if (nColor == CARD_DA)
		{
			strColor = "mjt2_";
		}
		std::string kName = utility::toString(strColor, strValue);
		if (iGender)
		{
			kName = utility::toString("man/", kName);
		}
		else
		{
			kName = utility::toString("woman/", kName);
		}
		playEffect(kName);*/
	}

	void playEffectByKind(int iGender, std::string kName)
	{
		if (iGender == UserSex::US_Male)
		{
			kName = utility::toString("man/", kName);
		}
		else if ( iGender == UserSex::US_Femal)
		{
			kName = utility::toString("woman/", kName);
		}
		else
		{
			kName =utility::toString("woman/",kName);
		}
		playEffect(kName);
	}

	void playEffectEX(std::string kName)
	{
		kName = utility::toString(LG_PATH_HN, kName);
		SoundFun::Instance().playEffect(kName);
	}

	std::string getPath()
	{
		return std::string();
	}
}

