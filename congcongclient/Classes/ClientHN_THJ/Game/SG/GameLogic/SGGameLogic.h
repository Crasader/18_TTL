#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"

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
	const static BYTE GAME_CARDS[MAX_CARD_COUNT];
	const static std::string GAME_CARDS_NAME[MAX_CARD_COUNT];

public:
	static void initCard(BYTE cbCardData[], BYTE cbMaxCount);

	//�����˿�
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
