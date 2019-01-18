#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "../PaiKu/allniu.hpp"

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

extern bool paiku_readover;

void CTableFrameSink::InitPaiKu()
{
	if (paiku_readover) {
		return;
	}

	read_allniu();

	ASSERT(vct_niu1.size()!=0);
	ASSERT(vct_niu2.size() != 0);
	ASSERT(vct_niu3.size() != 0);
	ASSERT(vct_niu4.size() != 0);
	ASSERT(vct_niu5.size() != 0);
	ASSERT(vct_niu6.size() != 0);
	ASSERT(vct_niu7.size() != 0);
	ASSERT(vct_niu8.size() != 0);
	ASSERT(vct_niu9.size() != 0);
	ASSERT(vct_niu10.size() != 0);

	ASSERT(vct_shunzi.size() != 0);
	ASSERT(vct_wuhua.size() != 0);
	ASSERT(vct_tonghua.size() != 0);
	ASSERT(vct_hulu.size() != 0);
	ASSERT(vct_zhadan.size() != 0);
	ASSERT(vct_wuxiao.size() != 0);

	paiku_readover = true;
}
