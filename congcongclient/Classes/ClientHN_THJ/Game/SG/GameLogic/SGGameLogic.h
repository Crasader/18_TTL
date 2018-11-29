#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//逻辑掩码
#define MASK_COLOR 0xF0 //花色掩码
#define MASK_VALUE 0x0F //数值掩码

#define CARD_INDEX_MIN 0x01 //最小牌值
#define CARD_INDEX_MAX 0x0D //最大牌值

#define MAX_CARD_COUNT 52 //最大牌张数

//游戏逻辑类
class SGGameLogic
{
public:
	const static BYTE GAME_CARDS[MAX_CARD_COUNT];
	const static std::string GAME_CARDS_NAME[MAX_CARD_COUNT];

public:
	static void initCard(BYTE cbCardData[], BYTE cbMaxCount);

	//混乱扑克
	static void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);

	static SG::SGCardType_Result checkSGType(BYTE cbCardData[],DWORD bGameRuleIdex);

	static bool isCardValid(BYTE cardData);

	static bool hasRule(DWORD gameRule, BYTE rule);

	static int getCardIndex(BYTE cardData);

	static int getSGRatio(SG::SGCardType_Result result);
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
