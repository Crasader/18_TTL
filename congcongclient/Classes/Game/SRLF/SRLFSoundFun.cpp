#include "SRLFSoundFun.h"
#include "SimpleAudioEngine.h"
#include "Game/Script/SoundFun.h"
#include "SRLFGameLogic.h"
#include "Platform/PFDefine/df/types.h"
#include "UTILITY.h"

namespace SRLFSoundFun
{
	static const std::string LG_PATH_SC = "GameSRLF/sound/";
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
		//kName = utility::toString(LG_PATH_SC,kName);
		SoundFun::Instance().playBackMusic(utility::a_u8(kName));
		cocos2d::log("SRLFSoundFun playBackMusic :%s",utility::a_u8(kName).c_str());
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::toString(LG_PATH_SC,LG_FLAG_SC,kName,RAW_KIND);
		SoundFun::Instance().playEffect(kName,bPreload);
		cocos2d::log("SRLFSoundFun:%s",kName.c_str());
	}

	void playCardByGender(int iGender,int nCard)
	{
		int nColor = SRLFLOGIC::CGameLogic::Instance().GetCardColor(nCard);
		int nValue = SRLFLOGIC::CGameLogic::Instance().GetCardValue(nCard);
		
		if (nColor == CARD_COLOR_WAN)
		{
			nColor = CARD_COLOR_TONG;
		}
		else if (nColor == CARD_COLOR_TONG)
		{
			nColor = CARD_COLOR_WAN;
		}

		std::string kName;
		if (iGender == UserSex::US_Femal)
		{
			int nNum = 0;
			std::string strTemp = utility::toString(nColor,nValue);
			if ( strTemp=="11" ||strTemp=="13"||strTemp=="19"||strTemp=="22"||strTemp=="28"||strTemp=="34"||strTemp=="36"||strTemp=="39")
			{
				nNum = rand()%2;
			}
			else if (strTemp=="14"||strTemp=="23"||strTemp=="24"||strTemp=="25"||strTemp=="29")
			{
				nNum = rand()%3;
			}
			else if (strTemp=="21"||strTemp=="31"||strTemp=="32")
			{
				nNum = rand()%4;
			}
			else if (strTemp=="38")
			{
				nNum = rand()%5;
			}
			kName = utility::toString("woman",nColor,nValue,"_",nNum);
		}
		else if(iGender == UserSex::US_Male)
		{
			int nNum = 0;
			std::string strTemp = utility::toString(nColor,nValue);
			if ( strTemp=="11" ||strTemp=="12"||strTemp=="13"||strTemp=="19"||strTemp=="28"||strTemp=="34"||strTemp=="36"||strTemp=="39")
			{
				nNum = rand()%2;
			}
			else if (strTemp=="14"||strTemp=="22"||strTemp=="23"||strTemp=="24"||strTemp=="25"||strTemp=="29")
			{
				nNum = rand()%3;
			}
			else if (strTemp=="21"||strTemp=="31")
			{
				nNum = rand()%4;
			}
			else if (strTemp=="32"||strTemp=="38")
			{
				nNum = rand()%5;
			}

			kName = utility::toString("man",nColor,nValue,"_",nNum);
		}
		else
		{
			int nNum = 0;
			std::string strTemp = utility::toString(nColor,nValue);
			if ( strTemp=="11" ||strTemp=="13"||strTemp=="19"||strTemp=="22"||strTemp=="28"||strTemp=="34"||strTemp=="36"||strTemp=="39")
			{
				nNum = rand()%2;
			}
			else if (strTemp=="14"||strTemp=="23"||strTemp=="24"||strTemp=="25"||strTemp=="29")
			{
				nNum = rand()%3;
			}
			else if (strTemp=="21"||strTemp=="31"||strTemp=="32")
			{
				nNum = rand()%4;
			}
			else if (strTemp=="38")
			{
				nNum = rand()%5;
			}
			kName = utility::toString("woman",nColor,nValue,"_",nNum);
		}
		playEffect(kName);
	}

	void playEffectByGender(int iGender,std::string kName)
	{
		if(iGender == UserSex::US_Femal)
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
		else if(iGender == UserSex::US_Male)
		{

			int nNum = EFFECT_NUM;
			if (kName == EFFECT_AN_GANG)
			{
				nNum = EFFECT_NUM-1;
			}
			if (kName == EFFECT_PENG)
			{
				nNum = EFFECT_NUM+1;
			}
			kName =utility::toString("man_",kName,rand()%nNum);
		}
		else
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

		playEffect(kName);
	}

	void playEffectEX(std::string kName)
	{
		SoundFun::Instance().Instance().playEffect(kName);
	}

	std::string getDefineSound(int iGender,std::string kStr)
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
} 
