#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "../PaiKu/zhadan.hpp"

extern std::vector<handcard> vct_zhadan;
extern BYTE** ZHADAN_DATA;
extern int ZHA_DAN_LEN;

void CTableFrameSink::InitPaiKu()
{
	ReadZhaDanArray();
}

void CTableFrameSink::ReadZhaDanArray()
{
	if (ZHADAN_DATA == nullptr) {
		read_zhadanniu();
		if (vct_zhadan.size() != 0) {
			ZHA_DAN_LEN = vct_zhadan.size();
			ZHADAN_DATA = new BYTE*[ZHA_DAN_LEN];
			for (size_t idx = 0; idx < vct_zhadan.size(); idx++) {
				ZHADAN_DATA[idx] = new BYTE[5];
				memcpy(ZHADAN_DATA[idx], vct_zhadan[idx].card, sizeof(vct_zhadan[idx]));
			}
		}
	}
}
