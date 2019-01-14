#ifndef _ZHADAN_HPP_
#define _ZHADAN_HPP_

#include <fstream>

#include "../��Ϸ������/FvMask.h"
#include "../��Ϸ������/GameLogic.h"

#include "handcard.h"

//��ֵ
#define CardValue CGameLogic::GAME_CARDS
//ը������
#define zd_data "zhandan.data"
//ը������
extern std::vector<handcard> vct_zhadan;
//��Ϸ�߼�
extern CGameLogic m_GameLogic;

inline void write_zhadanniu()
{
	vct_zhadan.clear();

	DWORD rule = 0;

	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_SZN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_WHN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_THN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_HLN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_ZDN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_WXN));

	std::fstream fs;
	fs.open(zd_data, std::ios::out | std::ios::binary);

	//1.������е�ը��
	int card_len = ARRAYSIZE(CardValue);
	for (int idx = 0; idx < card_len / 4; idx++) {
		//��ȷ��ǰ4����
		BYTE  zhadan[5] = { CardValue[idx], CardValue[idx] + 0x10,  CardValue[idx] + 0x20, CardValue[idx] + 0x30 };
		//��������5����
		for (int idx_4 = 0; idx_4 < card_len; idx_4++) {

			//ȥ����ֵ��ͬ����
			if ((CardValue[idx] & 0xf) == (CardValue[idx_4] & 0xf)) {
				continue;
			}
			zhadan[4] = CardValue[idx_4];
			auto ret = m_GameLogic.checkNNType(zhadan, rule);
			if (ret.type != NNCardType::NNCardType_ZDN) {
				printf("ret.type = %d\n", ret.type);
				continue;
			}
			//��������
			handcard val = zhadan;
			vct_zhadan.push_back(val);
			fs.write(reinterpret_cast<const char*>(val.card), 5);

		}
	}

	fs.close();
	printf("������ը������: %d.\n", vct_zhadan.size());
}

inline void read_zhadanniu()
{
	std::fstream fs;
	fs.open(zd_data, std::ios::in | std::ios::binary);
	vct_zhadan.clear();

	int count = 0;
	char cards[5];
	while (fs.read(cards, 5)) {
		handcard hc = reinterpret_cast<const BYTE*>(cards);
#ifdef _DEBUG
		hc.print();
		printf("\n");
#endif
		vct_zhadan.push_back(hc);
		count++;
	}

	fs.close();
	printf("��ȡ��ը������: %d.\n", count);
}

#endif
