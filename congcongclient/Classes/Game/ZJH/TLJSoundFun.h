#pragma once
#include <cocos2d.h>

namespace TLJSoundFun
{
	const static int EFFECT_NUM = 3;

	const static int AN_GANG_NUM_W = 2;

	const static std::string RAW_KIND = ".mp3";

	const static std::string EFFECT_BIPAI = "bipai";
	const static std::string EFFECT_GENZHU = "genzhu";
	const static std::string EFFECT_KANPAI = "kanpai";
	const static std::string EFFECT_MENBI = "menbi";
	const static std::string EFFECT_MENZHU = "menzhu";
	const static std::string EFFECT_QIPAI = "qipai";
	const static std::string EFFECT_XIAOZHU = "xiaozhu";
	const static std::string EFFECT_ZHANGJIA = "zhangjia";

	void playBackMusic(std::string kName);
	void stopBackMusic();
	void playEffect(std::string kName, bool bPreload = false);
	std::string getDefineSound(int iGender, std::string kStr);
	void playCardByGender(int iGender, int nCard);
	void playEffectByKind(int iGender, std::string kName);
	void playEffectEX(std::string kName);
	std::string getPath();
}