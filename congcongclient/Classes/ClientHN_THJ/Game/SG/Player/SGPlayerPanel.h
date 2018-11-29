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

#pragma region 初始化
public:
	bool init();
	void initLayout();
#pragma endregion 初始化

#pragma region 显示与隐藏对应玩家信息
	void showPlayer(SGPlayer& player);
	void hidePlayer(int playerVisioChairID);
	void playerTalk(SGPlayer& player, CMD_GR_C_TableTalk* pInfo);
	void playEffectSGPlayer_Snatch(SGPlayer& player,bool isVisible = false);
#pragma endregion 显示与隐藏对应玩家信息
};