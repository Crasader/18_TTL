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

#pragma region 初始化
public:
	bool init();
	void initLayout();
#pragma endregion 初始化

#pragma region 显示与隐藏对应玩家信息
	void showPlayer(NNPlayer& player);
	void hidePlayer(int playerVisioChairID);
	void playerTalk(NNPlayer& player, CMD_GR_C_TableTalk* pInfo);
#pragma endregion 显示与隐藏对应玩家信息
};