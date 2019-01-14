#ifndef _ZHADAN_HPP_
#define _ZHADAN_HPP_

#include <fstream>

#include "../游戏服务器/FvMask.h"
#include "../游戏服务器/GameLogic.h"

#include "handcard.h"

//牌值
#define CardValue CGameLogic::GAME_CARDS
//炸弹数据
#define zd_data "zhandan.data"
//炸弹牌组
extern std::vector<handcard> vct_zhadan;
//游戏逻辑
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

	//1.算出所有的炸弹
	int card_len = ARRAYSIZE(CardValue);
	for (int idx = 0; idx < card_len / 4; idx++) {
		//先确定前4张牌
		BYTE  zhadan[5] = { CardValue[idx], CardValue[idx] + 0x10,  CardValue[idx] + 0x20, CardValue[idx] + 0x30 };
		//遍历出第5张牌
		for (int idx_4 = 0; idx_4 < card_len; idx_4++) {

			//去掉牌值相同的牌
			if ((CardValue[idx] & 0xf) == (CardValue[idx_4] & 0xf)) {
				continue;
			}
			zhadan[4] = CardValue[idx_4];
			auto ret = m_GameLogic.checkNNType(zhadan, rule);
			if (ret.type != NNCardType::NNCardType_ZDN) {
				printf("ret.type = %d\n", ret.type);
				continue;
			}
			//保存牌型
			handcard val = zhadan;
			vct_zhadan.push_back(val);
			fs.write(reinterpret_cast<const char*>(val.card), 5);

		}
	}

	fs.close();
	printf("保存了炸弹牌组: %d.\n", vct_zhadan.size());
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
	printf("读取了炸弹牌组: %d.\n", count);
}

#endif
