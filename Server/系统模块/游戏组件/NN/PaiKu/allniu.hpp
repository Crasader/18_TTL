#ifndef _WUXIAONIU_HPP_
#define _WUXIAONIU_HPP_

#include <fstream>

#include "../游戏服务器/FvMask.h"
#include "../游戏服务器/GameLogic.h"

#include "handcard.h"

//牌值
#define CardValue CGameLogic::GAME_CARDS

//牛1数据
#define niu1_data "niu1.data"
//牛2数据
#define niu2_data "niu2.data"
//牛3数据
#define niu3_data "niu3.data"
//牛4数据
#define niu4_data "niu4.data"
//牛5数据
#define niu5_data "niu5.data"
//牛6数据
#define niu6_data "niu6.data"
//牛7数据
#define niu7_data "niu7.data"
//牛8数据
#define niu8_data "niu8.data"
//牛9数据
#define niu9_data "niu9.data"
//牛牛数据
#define niu10_data "niu10.data"

//顺子数据
#define shunzi_data "shunzi.data"
//五花数据
#define wuhua_data "wuhua.data"
//同花数据
#define tonghua_data "tonghua.data"
//葫芦数据
#define hulu_data "hulu.data"
//炸弹数据
#define zhadan_data "zhandan.data"
//五小数据
#define wuxiao_data "wuxiao.data"

//游戏逻辑
extern CGameLogic m_GameLogic;

//牛1牌组
extern std::vector<handcard> vct_niu1;
//牛2牌组
extern std::vector<handcard> vct_niu2;
//牛3牌组
extern std::vector<handcard> vct_niu3;
//牛4牌组
extern std::vector<handcard> vct_niu4;
//牛5牌组
extern std::vector<handcard> vct_niu5;
//牛6牌组
extern std::vector<handcard> vct_niu6;
//牛7牌组
extern std::vector<handcard> vct_niu7;
//牛8牌组
extern std::vector<handcard> vct_niu8;
//牛9牌组
extern std::vector<handcard> vct_niu9;
//牛牛牌组
extern std::vector<handcard> vct_niu10;

//顺子牛牌组
extern std::vector<handcard> vct_shunzi;
//五花牛牌组
extern std::vector<handcard> vct_wuhua;
//同花牛牌组
extern std::vector<handcard> vct_tonghua;
//葫芦牛牌组
extern std::vector<handcard> vct_hulu;
//炸弹牌组
extern std::vector<handcard> vct_zhadan;
//五小牛牌组
extern std::vector<handcard> vct_wuxiao;

inline void write_allniu()
{
	vct_wuxiao.clear();

	DWORD rule = 0;

	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_SZN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_WHN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_THN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_HLN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_ZDN));
	FvMask::Add(rule, _MASK_(NNGameRule::NNGameRule_SpecialRule_WXN));

	std::fstream fs_niu1;
	fs_niu1.open(niu1_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu2;
	fs_niu2.open(niu2_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu3;
	fs_niu3.open(niu3_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu4;
	fs_niu4.open(niu4_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu5;
	fs_niu5.open(niu5_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu6;
	fs_niu6.open(niu6_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu7;
	fs_niu7.open(niu7_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu8;
	fs_niu8.open(niu8_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu9;
	fs_niu9.open(niu9_data, std::ios::out | std::ios::binary);
	std::fstream fs_niu10;
	fs_niu10.open(niu10_data, std::ios::out | std::ios::binary);

	std::fstream fs_shunzi;
	fs_shunzi.open(shunzi_data, std::ios::out | std::ios::binary);
	std::fstream fs_wuhua;
	fs_wuhua.open(wuhua_data, std::ios::out | std::ios::binary);
	std::fstream fs_tonghua;
	fs_tonghua.open(tonghua_data, std::ios::out | std::ios::binary);
	std::fstream fs_hulu;
	fs_hulu.open(hulu_data, std::ios::out | std::ios::binary);
	std::fstream fs_zhadan;
	fs_zhadan.open(zhadan_data, std::ios::out | std::ios::binary);
	std::fstream fs_wuxiao;
	fs_wuxiao.open(wuxiao_data, std::ios::out | std::ios::binary);

	int card_len = ARRAYSIZE(CardValue);
	handcard hc;
	for (int card1 = 0; card1 < card_len; card1++) {
		for (int card2 = card1 + 1; card2 < card_len; card2++) {
			for (int card3 = card2 + 1; card3 < card_len; card3++) {
				for (int card4 = card3 + 1; card4 < card_len; card4++) {
					for (int card5 = card4 + 1; card5 < card_len; card5++) {
						BYTE cards[] = {CardValue[card1], CardValue[card2], CardValue[card3], CardValue[card4], CardValue[card5]};
						NNCardType_Result ret = m_GameLogic.checkNNType(cards, rule);
						switch (ret.type)
						{
						case NNCardType_N1:
							fs_niu1.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu1.push_back(hc);
							break;
						case NNCardType_N2:
							fs_niu2.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu2.push_back(hc);
							break;
						case NNCardType_N3:
							fs_niu3.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu3.push_back(hc);
							break;
						case NNCardType_N4:
							fs_niu4.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu4.push_back(hc);
							break;
						case NNCardType_N5:
							fs_niu5.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu5.push_back(hc);
							break;
						case NNCardType_N6:
							fs_niu6.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu6.push_back(hc);
							break;
						case NNCardType_N7:
							fs_niu7.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu7.push_back(hc);
							break;
						case NNCardType_N8:
							fs_niu8.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu8.push_back(hc);
							break;
						case NNCardType_N9:
							fs_niu9.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu9.push_back(hc);
							break;
						case NNCardType_NN:
							fs_niu10.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_niu10.push_back(hc);
							break;
						case NNCardType_SZN://顺子牛
							fs_shunzi.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_shunzi.push_back(hc);
							break;
						case NNCardType_WHN://五花牛
							fs_wuhua.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_wuhua.push_back(hc);
							break;
						case NNCardType_THN://同花牛
							fs_tonghua.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_tonghua.push_back(hc);
							break;
						case NNCardType_HLN://葫芦牛
							fs_hulu.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_hulu.push_back(hc);
							break;
						case NNCardType_ZDN://炸弹牛
							fs_zhadan.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_zhadan.push_back(hc);
							break;
						case NNCardType_WXN://五小牛
							fs_wuxiao.write(reinterpret_cast<const char*>(cards), 5);
							hc = cards;
							vct_wuxiao.push_back(hc);
							break;
						default:
							break;
						}
					}
				}
			}
		}

		printf("==============================\n");

		printf("保存了牛1牌组: %d.\n", vct_niu1.size());
		printf("保存了牛2牌组: %d.\n", vct_niu2.size());
		printf("保存了牛3牌组: %d.\n", vct_niu3.size());
		printf("保存了牛4牌组: %d.\n", vct_niu4.size());
		printf("保存了牛5牌组: %d.\n", vct_niu5.size());
		printf("保存了牛6牌组: %d.\n", vct_niu6.size());
		printf("保存了牛7牌组: %d.\n", vct_niu7.size());
		printf("保存了牛8牌组: %d.\n", vct_niu8.size());
		printf("保存了牛9牌组: %d.\n", vct_niu9.size());
		printf("保存了牛10牌组: %d.\n", vct_niu10.size());

		printf("保存了顺子牌组: %d.\n", vct_shunzi.size());
		printf("保存了五花牌组: %d.\n", vct_wuhua.size());
		printf("保存了同花牌组: %d.\n", vct_tonghua.size());
		printf("保存了葫芦牌组: %d.\n", vct_hulu.size());
		printf("保存了炸弹牌组: %d.\n", vct_zhadan.size());
		printf("保存了五小牌组: %d.\n", vct_wuxiao.size());

		printf("==============================\n");

	}

	fs_niu1.close();
	fs_niu2.close();
	fs_niu3.close();
	fs_niu4.close();
	fs_niu5.close();
	fs_niu6.close();
	fs_niu7.close();
	fs_niu8.close();
	fs_niu9.close();
	fs_niu10.close();

	fs_shunzi.close();
	fs_wuhua.close();
	fs_tonghua.close();
	fs_hulu.close();
	fs_zhadan.close();
	fs_wuxiao.close();

}

inline void read_typeniu(std::vector<handcard>& vct, const char* name = nullptr)
{

	if (name == nullptr) {
		printf("Invalid filename to read.");
		return;
	}
	std::fstream fs;
	fs.open(name, std::ios::in | std::ios::binary);

	int count = 0;
	char cards[5];
	while (fs.read(cards, 5)) {
		handcard hc = reinterpret_cast<const BYTE*>(cards);
#ifdef _DEBUG
		//hc.print();
		//printf("\n");
#endif
		vct.push_back(hc);
		count++;
	}

	fs.close();
	printf("读取了%s牌组: %d.\n", name, count);

}

inline void read_allniu()
{

	//牛1牌组
	vct_niu1.clear();
	read_typeniu(vct_niu1, niu1_data);
	//牛2牌组
	vct_niu2.clear();
	read_typeniu(vct_niu2, niu2_data);
	//牛3牌组
	vct_niu3.clear();
	read_typeniu(vct_niu3, niu3_data);
	//牛4牌组
	vct_niu4.clear();
	read_typeniu(vct_niu4, niu4_data);
	//牛5牌组
	vct_niu5.clear();
	read_typeniu(vct_niu5, niu5_data);
	//牛6牌组
	vct_niu6.clear();
	read_typeniu(vct_niu6, niu6_data);
	//牛7牌组
	vct_niu7.clear();
	read_typeniu(vct_niu7, niu7_data);
	//牛8牌组
	vct_niu8.clear();
	read_typeniu(vct_niu8, niu8_data);
	//牛9牌组
	vct_niu9.clear();
	read_typeniu(vct_niu9, niu9_data);
	//牛牛牌组
	vct_niu10.clear();
	read_typeniu(vct_niu10, niu10_data);

	//顺子牛牌组
	vct_shunzi.clear();
	read_typeniu(vct_shunzi, shunzi_data);
	//五花牛牌组
	vct_wuhua.clear();
	read_typeniu(vct_wuhua, wuhua_data);
	//同花牛牌组
	vct_tonghua.clear();
	read_typeniu(vct_tonghua, tonghua_data);
	//葫芦牛牌组
	vct_hulu.clear();
	read_typeniu(vct_hulu, hulu_data);
	//炸弹牌组
	vct_zhadan.clear();
	read_typeniu(vct_zhadan, zhadan_data);
	//五小牛牌组
	vct_wuxiao.clear();
	read_typeniu(vct_wuxiao, wuxiao_data);

}

#endif
