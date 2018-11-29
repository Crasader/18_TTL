#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE
#include <vector>

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)

#define KIND_ID						307									//游戏 I D

//组件属性
#define GAME_PLAYER					6									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

#define GAME_NAME					TEXT("一赖到底")				//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_NN_FREE					GAME_STATUS_FREE					//空闲状态
#define GS_NN_PLAY					(GAME_STATUS_PLAY+1)				//游戏状态
#define GS_NN_CALCULATE				(GAME_STATUS_PLAY+2)				//结算状态

//常量定义

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_START 100 //游戏开始
#define SUB_S_GAME_SNATCH_BANKER 101	//抢庄
#define SUB_S_GAME_SEND_BANKER 102 //发送庄家信息
#define SUB_S_SEND_CARD_ALL 103 //发牌命令
#define SUB_S_SEND_CARD_FIRST 104 //第一次发牌
#define SUB_S_SEND_CARD_SECOND 105 //第二次发牌
#define SUB_S_COMPARE_CARD 106 //比牌
#define SUB_S_GAME_CALCULATE 107 //单局结算
#define SUB_S_GAME_END 108 //游戏结束

#define SUB_S_ANDROID_RULE			200                                 //发送规则给机器人

struct CMD_S_AndroidRule {
	BYTE							cbGameTypeIdex;	//游戏类型
	DWORD							dwGameRuleIdex;	//游戏规则
};

enum NN_GameRule {
	NN_RULE_WHN,
	NN_RULE_ZDN,
	NN_RULE_WXN,
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_USER_SNATCH_BANKER	 1 //抢庄
#define SUB_C_BACK_CALL_START 2 //庄家开始游戏
#define SUB_C_USER_DROP_BANKER 3 //弃庄
#define SUB_C_USER_CALL 4 //下注


#pragma pack()
#endif
