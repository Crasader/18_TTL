#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//逻辑掩码
#define MASK_COLOR 0xF0 //花色掩码
#define MASK_VALUE 0x0F //数值掩码

#define CARD_INDEX_MIN 0x01 //最小牌值
#define CARD_INDEX_MAX 0x0D //最大牌值

#define MAX_CARD_COUNT 52 //最大牌张数

//游戏逻辑类
class CGameLogic
{
public:
    const static BYTE GAME_CARDS[MAX_CARD_COUNT];
	const static std::string GAME_CARDS_NAME[MAX_CARD_COUNT];

public:
    //构造函数
    CGameLogic();
    //析构函数
    virtual ~CGameLogic();

public:
	void initCard(BYTE cbCardData[], BYTE cbMaxCount);

    //混乱扑克
    void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);

	NNCardType_Result checkNNType(BYTE cbCardData[], DWORD GameRules);


	BOOL comparePlayerCards(NNCardType_Result result_1, NNCardType_Result result_2);

	//WORD comparePlayerCards_All(BYTE playerCards[NN_GAME_PLAYER][MAX_HAND_CARD], NNPlayerStatus playerStatus[NN_GAME_PLAYER], DWORD gameRules);

	int getNNRatio(NNCardType_Result result, DWORD gameRules);

	bool isCardValid(BYTE cardData);

	bool hasRule(DWORD gameRule, BYTE rule);
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
