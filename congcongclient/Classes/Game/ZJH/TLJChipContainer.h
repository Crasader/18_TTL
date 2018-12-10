#pragma once

#include "common.h"
#include "TLJPlayer.h"

class TLJChipContainer
	:public cocos2d::Node
	,public FvSingleton<TLJChipContainer>
{
public:
	TLJChipContainer();
	~TLJChipContainer();
public:
	bool init();
	//Ìí¼Ó³ïÂë
	void addChip(byte cbScoreLevel,word wTimes,word chipCount,dword dwBaseScore);
	//Ìí¼Ó³ïÂë
	void addChip(TLJPlayer& player,byte cbScoreLevel,word wTimes);
	//ÇåÀí³ïÂë
	void clearChips();
	//³ïÂëÈİÆ÷
	Node* getChipContainer();

	void hide();
	void show();
};

