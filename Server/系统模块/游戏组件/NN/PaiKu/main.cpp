#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "handcard.h"
#include "zhadan.hpp"

//炸弹牌组
std::vector<handcard> vct_zhadan;
//游戏逻辑
CGameLogic m_GameLogic;

int main(int argv, char* argvs)
{
	//////////////////////////////////////////////////////////////////////////

	printf("-------------开始生成炸弹牌组-----------------\n");

	write_zhadanniu();

	read_zhadanniu();

	//////////////////////////////////////////////////////////////////////////

	system("pause");
	return 0;
}
