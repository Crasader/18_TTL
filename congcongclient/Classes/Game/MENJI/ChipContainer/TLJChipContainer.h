#pragma once

#include "CommonHeader/CommonHeader.h"
#include "../Player/TLJPlayer.h"

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
	void addChip(BYTE cbScoreLevel,WORD wTimes,WORD chipCount,DWORD dwBaseScore);
	//Ìí¼Ó³ïÂë
	void addChip(TLJPlayer& player,BYTE cbScoreLevel,WORD wTimes);
	//ÇåÀí³ïÂë
	void clearChips();
	//³ïÂëÈİÆ÷
	Node* getChipContainer();

	void hide();
	void show();
};

