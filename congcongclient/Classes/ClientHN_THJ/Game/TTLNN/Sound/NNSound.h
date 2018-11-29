#pragma once

#include "CommonHeader/CommonHeader.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"
#include "ClientHN_THJ/Game/TTLNN/Player/NNPlayer.h"
#include "GameLib/Platform/PFDefine/df/types.h"

namespace NNSound
{
	const static std::string VOICE_PATH = "TTLNN/Sound/";
	const static std::string MAN_PATH = "man/";
	const static std::string WOMEN_PATH = "women/";
	const static std::string FILE_TAIL = ".mp3";

	const static std::string BUTTON_EFFECT = "NN_Button.mp3";
	const static std::string USER_CALL = "NN_Bets.mp3";
	const static std::string GAME_START = "NN_Start.mp3";
	const static std::string NN_WIN = "NN_Win.mp3";
	const static std::string NN_LOSE = "NN_Lose.mp3";

	std::string getVoicePath();

	void playNN(TTLNN::NNCardType cardType,NNPlayer* pPlayer);
	void playBet(int bets);
	void playSnatch(int snatchRatio);
	void playEffect(std::string effectName);

	void playBackMusic(std::string music);
	void stopBackMusic();
	std::string getDefineSound(int iGender,std::string kStr);
};