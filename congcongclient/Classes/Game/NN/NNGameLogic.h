#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Game/NN/CMD_NN.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�߼�����
#define MASK_COLOR 0xF0 //��ɫ����
#define MASK_VALUE 0x0F //��ֵ����

#define CARD_INDEX_MIN 0x01 //��С��ֵ
#define CARD_INDEX_MAX 0x0D //�����ֵ

#define MAX_CARD_COUNT 52 //���������

//��Ϸ�߼���
class NNGameLogic
{
public:
	const static byte GAME_CARDS[MAX_CARD_COUNT];
	const static std::string GAME_CARDS_NAME[MAX_CARD_COUNT];

public:
	static void initCard(byte cbCardData[], byte cbMaxCount);

	//�����˿�
	static void RandCardData(byte cbCardData[], byte cbMaxCount);

	static TTLNN::NNCardType_Result checkNNType(byte cbCardData[], dword GameRules);

	static bool isCardValid(byte cardData);

	static bool hasRule(dword gameRule, byte rule);

	static int getCardIndex(byte cardData);
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
