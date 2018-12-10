#pragma once

#include "common.h"
#include "NNPlayer.h"

class NNPlayerPanel
	: public cocos2d::Node
	, public FvSingleton<NNPlayerPanel>
{
public:
	NNPlayerPanel();
	~NNPlayerPanel();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
#pragma endregion ��ʼ��

#pragma region ��ʾ�����ض�Ӧ�����Ϣ
	void showPlayer(NNPlayer& player);
	void hidePlayer(int playerVisioChairID);
	void playerTalk(NNPlayer& player, CMD_GR_C_TableTalk* pInfo);
#pragma endregion ��ʾ�����ض�Ӧ�����Ϣ
};