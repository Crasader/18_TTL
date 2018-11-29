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
	//��ӳ���
	void addChip(BYTE cbScoreLevel,WORD wTimes,WORD chipCount,DWORD dwBaseScore);
	//��ӳ���
	void addChip(TLJPlayer& player,BYTE cbScoreLevel,WORD wTimes);
	//�������
	void clearChips();
	//��������
	Node* getChipContainer();

	void hide();
	void show();
};

