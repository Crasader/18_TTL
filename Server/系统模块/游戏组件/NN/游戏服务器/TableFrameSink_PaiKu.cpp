#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "../PaiKu/allniu.hpp"

//ţ1����
extern std::vector<handcard> vct_niu1;
//ţ2����
extern std::vector<handcard> vct_niu2;
//ţ3����
extern std::vector<handcard> vct_niu3;
//ţ4����
extern std::vector<handcard> vct_niu4;
//ţ5����
extern std::vector<handcard> vct_niu5;
//ţ6����
extern std::vector<handcard> vct_niu6;
//ţ7����
extern std::vector<handcard> vct_niu7;
//ţ8����
extern std::vector<handcard> vct_niu8;
//ţ9����
extern std::vector<handcard> vct_niu9;
//ţţ����
extern std::vector<handcard> vct_niu10;

//˳��ţ����
extern std::vector<handcard> vct_shunzi;
//�廨ţ����
extern std::vector<handcard> vct_wuhua;
//ͬ��ţ����
extern std::vector<handcard> vct_tonghua;
//��«ţ����
extern std::vector<handcard> vct_hulu;
//ը������
extern std::vector<handcard> vct_zhadan;
//��Сţ����
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
