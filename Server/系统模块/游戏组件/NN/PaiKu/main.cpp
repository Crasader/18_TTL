#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "handcard.h"
#include "zhadan.hpp"
#include "allniu.hpp"

//��Ϸ�߼�
CGameLogic m_GameLogic;

//ţ1����
std::vector<handcard> vct_niu1;
//ţ2����
std::vector<handcard> vct_niu2;
//ţ3����
std::vector<handcard> vct_niu3;
//ţ4����
std::vector<handcard> vct_niu4;
//ţ5����
std::vector<handcard> vct_niu5;
//ţ6����
std::vector<handcard> vct_niu6;
//ţ7����
std::vector<handcard> vct_niu7;
//ţ8����
std::vector<handcard> vct_niu8;
//ţ9����
std::vector<handcard> vct_niu9;
//ţţ����
std::vector<handcard> vct_niu10;

//˳��ţ����
std::vector<handcard> vct_shunzi;
//�廨ţ����
std::vector<handcard> vct_wuhua;
//ͬ��ţ����
std::vector<handcard> vct_tonghua;
//��«ţ����
std::vector<handcard> vct_hulu;
//ը������
std::vector<handcard> vct_zhadan;
//��Сţ����
std::vector<handcard> vct_wuxiao;

int main(int argv, char* argvs)
{
	//////////////////////////////////////////////////////////////////////////

	//printf("-------------��ʼ����ը������-----------------\n");

	//write_zhadanniu();

	//printf("-------------��ʼ��ȡը������-----------------\n");

	//read_zhadanniu();

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	printf("-------------��ʼ������������-----------------\n");

	//write_allniu();

	printf("-------------��ʼ��ȡ��������-----------------\n");

	read_allniu();

	//////////////////////////////////////////////////////////////////////////

	system("pause");
	return 0;
}
