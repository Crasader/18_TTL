#pragma once

#include "CommonHeader/CommonHeader.h"
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"
#include "ClientHN_THJ/Game/SG/Player/SGPlayer.h"

namespace SGSound
{
	const static std::string VOICE_PATH = "SG/Sound/";
	const static std::string MAN_PATH = "man/";
	const static std::string WOMEN_PATH = "women/";
	const static std::string FILE_TAIL = ".mp3";

	const static std::string BUTTON_EFFECT = "SG_Button.mp3";
	const static std::string USER_CALL = "SG_Bets.mp3";
	const static std::string GAME_START = "SG_Start.mp3";
	const static std::string SG_WIN = "SG_Win.mp3";
	const static std::string SG_LOSE = "SG_Lose.mp3";

	std::string getVoicePath();
	void playSG(SG::SGCardType_Result result,SGPlayer* pPlayer);
	void playBet(int bets);
	void playSnatch(int snatchRatio);
	void playEffect(std::string effectName);

	void playBackMusic(std::string music);
	void stopBackMusic();
	std::string getDefineSound(int iGender,std::string kStr);
};