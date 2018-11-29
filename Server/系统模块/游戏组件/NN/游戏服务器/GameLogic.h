#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�߼�����
#define MASK_COLOR 0xF0 //��ɫ����
#define MASK_VALUE 0x0F //��ֵ����

#define CARD_INDEX_MIN 0x01 //��С��ֵ
#define CARD_INDEX_MAX 0x0D //�����ֵ

#define MAX_CARD_COUNT 52 //���������

//��Ϸ�߼���
class CGameLogic
{
public:
    const static BYTE GAME_CARDS[MAX_CARD_COUNT];
	const static std::string GAME_CARDS_NAME[MAX_CARD_COUNT];

public:
    //���캯��
    CGameLogic();
    //��������
    virtual ~CGameLogic();

public:
	void initCard(BYTE cbCardData[], BYTE cbMaxCount);

    //�����˿�
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
