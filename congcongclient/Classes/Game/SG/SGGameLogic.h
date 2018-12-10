#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
#include "Game/SG/CMD_SG.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�߼�����
#define MASK_COLOR 0xF0 //��ɫ����
#define MASK_VALUE 0x0F //��ֵ����

#define CARD_INDEX_MIN 0x01 //��С��ֵ
#define CARD_INDEX_MAX 0x0D //�����ֵ

#define MAX_CARD_COUNT 52 //���������

//��Ϸ�߼���
class SGGameLogic
{
public:
	const static byte GAME_CARDS[MAX_CARD_COUNT];
	const static std::string GAME_CARDS_NAME[MAX_CARD_COUNT];

public:
	static void initCard(byte cbCardData[], byte cbMaxCount);

	//�����˿�
	static void RandCardData(byte cbCardData[], byte cbMaxCount);

	static SG::SGCardType_Result checkSGType(byte cbCardData[],dword dwGameRuleIdex);

	static bool isCardValid(byte cardData);

	static bool hasRule(dword gameRule, byte rule);

	static int getCardIndex(byte cardData);

	static int getSGRatio(SG::SGCardType_Result result);
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
