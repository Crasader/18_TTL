#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Game/NN/CMD_NN.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//逻辑掩码
#define MASK_COLOR 0xF0 //花色掩码
#define MASK_VALUE 0x0F //数值掩码

#define CARD_INDEX_MIN 0x01 //最小牌值
#define CARD_INDEX_MAX 0x0D //最大牌值

#define MAX_CARD_COUNT 52 //最大牌张数

//游戏逻辑类
class NNGameLogic
{
public:
	const static byte GAME_CARDS[MAX_CARD_COUNT];
	const static std::string GAME_CARDS_NAME[MAX_CARD_COUNT];

public:
	static void initCard(byte cbCardData[], byte cbMaxCount);

	//混乱扑克
	static void RandCardData(byte cbCardData[], byte cbMaxCount);

	static TTLNN::NNCardType_Result checkNNType(byte cbCardData[], dword GameRules);

	static bool isCardValid(byte cardData);

	static bool hasRule(dword gameRule, byte rule);

	static int getCardIndex(byte cardData);
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
