#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "handcard.h"
#include "zhadan.hpp"
#include "allniu.hpp"

//游戏逻辑
CGameLogic m_GameLogic;

//牛1牌组
std::vector<handcard> vct_niu1;
//牛2牌组
std::vector<handcard> vct_niu2;
//牛3牌组
std::vector<handcard> vct_niu3;
//牛4牌组
std::vector<handcard> vct_niu4;
//牛5牌组
std::vector<handcard> vct_niu5;
//牛6牌组
std::vector<handcard> vct_niu6;
//牛7牌组
std::vector<handcard> vct_niu7;
//牛8牌组
std::vector<handcard> vct_niu8;
//牛9牌组
std::vector<handcard> vct_niu9;
//牛牛牌组
std::vector<handcard> vct_niu10;

//顺子牛牌组
std::vector<handcard> vct_shunzi;
//五花牛牌组
std::vector<handcard> vct_wuhua;
//同花牛牌组
std::vector<handcard> vct_tonghua;
//葫芦牛牌组
std::vector<handcard> vct_hulu;
//炸弹牌组
std::vector<handcard> vct_zhadan;
//五小牛牌组
std::vector<handcard> vct_wuxiao;

int main(int argv, char* argvs)
{
	//////////////////////////////////////////////////////////////////////////

	//printf("-------------开始生成炸弹牌组-----------------\n");

	//write_zhadanniu();

	//printf("-------------开始读取炸弹牌组-----------------\n");

	//read_zhadanniu();

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	printf("-------------开始生成所有牌组-----------------\n");

	//write_allniu();

	printf("-------------开始读取所有牌组-----------------\n");

	read_allniu();

	//////////////////////////////////////////////////////////////////////////

	system("pause");
	return 0;
}
