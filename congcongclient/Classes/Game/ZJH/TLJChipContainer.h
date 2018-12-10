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
	//��ӳ���
	void addChip(byte cbScoreLevel,word wTimes,word chipCount,dword dwBaseScore);
	//��ӳ���
	void addChip(TLJPlayer& player,byte cbScoreLevel,word wTimes);
	//�������
	void clearChips();
	//��������
	Node* getChipContainer();

	void hide();
	void show();
};

