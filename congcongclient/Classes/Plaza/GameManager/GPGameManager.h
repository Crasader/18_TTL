#pragma once
#include "common.h"
#include "Game/Game/GameManagerBase.h"
#include "GPSceneManager.h"

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