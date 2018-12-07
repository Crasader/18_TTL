#pragma once
#include "CommonHeader/CommonHeader.h"
#include "Game/Game/GameManagerBase.h"
#include "../GPSceneManager.h"
//#include "ClientHN_THJ/Game/HNMJ/HNMJGameScence.h"
#include "ClientHN_THJ/Game/SRLF/SRLFGameScence.h"

class GPGameManager
	: public GameManagerBase
	, public FvSingleton<GPGameManager>
{
public:
	GPGameManager();
	~GPGameManager();

public:
	void init();
	void hideAll();

public:

	IClientKernelSink* CreateGame();
	void loadGameBaseData(word wKindID);
	void StartHNMJRecord(datastream& kDataStream);
	bool _bCreated_NN;
};