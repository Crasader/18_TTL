#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "handcard.h"
#include "zhadan.hpp"

//ը������
std::vector<handcard> vct_zhadan;
//��Ϸ�߼�
CGameLogic m_GameLogic;

int main(int argv, char* argvs)
{
	//////////////////////////////////////////////////////////////////////////

	printf("-------------��ʼ����ը������-----------------\n");

	write_zhadanniu();

	read_zhadanniu();

	//////////////////////////////////////////////////////////////////////////

	system("pause");
	return 0;
}
