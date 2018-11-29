#pragma once

#include "CommonHeader/CommonHeader.h"
#include "SGPlayer.h"

class SGPlayerPanel
	: public cocos2d::Node
	, public FvSingleton<SGPlayerPanel>
{
public:
	SGPlayerPanel();
	~SGPlayerPanel();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
#pragma endregion ��ʼ��

#pragma region ��ʾ�����ض�Ӧ�����Ϣ
	void showPlayer(SGPlayer& player);
	void hidePlayer(int playerVisioChairID);
	void playerTalk(SGPlayer& player, CMD_GR_C_TableTalk* pInfo);
	void playEffectSGPlayer_Snatch(SGPlayer& player,bool isVisible = false);
#pragma endregion ��ʾ�����ض�Ӧ�����Ϣ
};