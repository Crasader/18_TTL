#pragma once

#include <vector>
#include "DEFINE.h"
 
//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)
namespace SG
{
//组件属性
#define SG_GAME_PLAYER					6									//游戏人数

//游戏状态
#define GS_SG_FREE					GAME_STATUS_FREE					//空闲状态
#define GS_SG_PLAY					(GAME_STATUS_PLAY+1)				//游戏状态
#define GS_SG_CALCULATE				(GAME_STATUS_PLAY+2)				//结算状态

#define TIME_FOR_SNATCH_BANKER 6//20
#define TIME_FOR_USER_CALL 	6//20
#define TIME_FOR_USER_SPLIT_CARD 6//20

//游戏配置
#define MAX_HAND_CARD 3 //最大手牌数
#define MAX_STAKE_COUNT 5 //下注范围枚举

enum SGGameType {
	SGGameType_SGBanker = 0,
	SGGameType_HostBanker = 1,
	SGGameType_SnatchBanker = 2,
	SGGameType_SnatchBankerShowCard = 3,
	SGGameType_AllCompare = 4,
	SGGameType_SGRatio = 5,
};

enum SGGameRule {
    SGGameRule_Score_0 = 1,
    SGGameRule_Score_1,
    SGGameRule_Score_2,
    SGGameRule_Pay_0,
    SGGameRule_Pay_1,
    SGGameRule_Ratio_0,
    SGGameRule_Ratio_1,
    SGGameRule_BankerScore_0,
    SGGameRule_BankerScore_1,
    SGGameRule_BankerScore_2,
    SGGameRule_BankerScore_3,
    SGGameRule_SnatchRatio_0,
    SGGameRule_SnatchRatio_1,
    SGGameRule_SnatchRatio_2,
    SGGameRule_SnatchRatio_3,
    SGGameRule_SpecialRule_1,
    SGGameRule_SpecialRule_2,
    SGGameRule_SpecialRule_3,
    SGGameRule_ExpertSetting_1,
    SGGameRule_ExpertSetting_2,
};

enum SG_GameResult {
    SG_Result_None, //未参与
    SG_Result_Win,
    SG_Result_Lose
};

enum SGPlayerStatus {
    SGPlayerStatus_Invalid, // 无效
    SGPlayerStatus_Sitting, // 有效
    SGPlayerStatus_Ready,  // 准备
    SGPlayerStatus_Playing, // 游戏中
};

enum SGGameStatus {
    SGGameStatus_Free, // 未开始
    SGGameStatus_HostConfirm, //房主确认开始游戏
    SGGameStatus_Start, // 游戏开始
    SGGameStatus_SnatchBanker, // 抢庄
    SGGameStatus_Call, //下注
    SGGameStatus_SplitCard, //拆牌
	SGGameStatus_Calculate, // 结算
};

enum SGSnatchBanker {
    SGSnatchBanker_Invalid = -1,
    SGSnatchBanker_None,
    SGSnatchBanker_1,
    SGSnatchBanker_2,
    SGSnatchBanker_3,
    SGSnatchBanker_4,
};

enum SGCardType {
	SGCardType_Invalid,
	SGCardType_None, //单牌
	SGCardType_HSG, //混三公
	SGCardType_XSG, //小三公
	SGCardType_BZ, // 豹子
	SGCardType_DSG, //大三公
};

enum SGCardCompareResult{
	SGCardCompareResult_Win, //赢
	SGCardCompareResult_Lose, //输
	SGCardCompareResult_Draw, //平局
};

//struct SGCardType_Result {
//    SGCardType type;
//    byte centerCard;
//    bool isCardSelected[MAX_HAND_CARD];
//};

struct SGCardType_Result {
	SGCardType type; //牌型-SGCardType
	byte cardPoint; //点数
	dword comparePoint; //比牌点数-服务器用
};

struct SGPlayerCard_Entity {
	bool isValid;
	word cardCount;
	byte cards[MAX_HAND_CARD];

	SGPlayerCard_Entity() {
		isValid = false;
		cardCount = 0;
		zeromemory(cards, sizeof(cards));
	}

	SGPlayerCard_Entity(byte* cards, word count) {
		isValid = true;
		cardCount = count;
		zeromemory(this->cards, sizeof(this->cards));
		memcpy(this->cards, cards, sizeof(byte)* count);
	}

	void addCard(byte card) {
		cards[cardCount++] = card;
	}
};

struct SGPlayerSingleCalculate {
	bool isValid;
	int score;

	SGPlayerSingleCalculate() {
		isValid = false;
		score = 0;
	}

	SGPlayerSingleCalculate(int score) {
		this->isValid = true;
		this->score = score;
	}
};
//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_START 100 //游戏开始
#define SUB_S_HOST_CONFIRM_START 101 // 房主确认开始
#define SUB_S_SNATCH_BANKER 102  //抢庄
#define SUB_S_USER_SNATCH_BANKER 103 //用户抢庄
#define SUB_S_BANKER_INFO 104  //定庄
#define SUB_S_USER_CALL 105 //玩家下注
#define SUB_S_SEND_CARD_ALL 106 //发牌
#define SUB_S_SEND_CARD_ADD 107 //补牌
#define SUB_S_SPLIT_CARD 108 //通知玩家拆牌
#define SUB_S_USER_SHOW_CARD 109 //玩家亮牌
#define SUB_S_CALCULATE 110 //单局结算
#define SUB_S_NOTIFY_GAME_END 111 // 游戏结束
// #define SUB_S_SNATCH_BANKER 101	//抢庄
// #define SUB_S_USER_SNATCH_BANKER 102 //玩家抢庄
// #define SUB_S_BANKER_INFO 103 //发送庄家信息
//
// #define SUB_S_SEND_CARD_ALL 104 //发牌命令
// #define SUB_S_SEND_CARD_FIRST 105 //第一次发牌
// #define SUB_S_SEND_CARD_SECOND 106 //第二次发牌
//
// #define SUB_S_USER_SHOW_CARD 107 //玩家亮牌
// #define SUB_S_COMPARE_CARD_RESULT 108 //比牌
//
// #define SUB_S_CALCULATE 109 //单局结算
// #define SUB_S_GAME_END 110 //游戏结束

#define SUB_S_ANDROID_RULE			200                                 //发送规则给机器人

struct CMD_S_AndroidRule {
    byte							cbGameTypeIdex;	//游戏类型
    dword							dwGameRuleIdex;	//游戏规则
};

struct CMD_S_GameStart {
	SGPlayerStatus status[SG_GAME_PLAYER];
};

struct CMD_S_BankerInfo {
	word bankerChairID;
	word bankerRatio; //抢庄倍率
	word bets[MAX_STAKE_COUNT];
};

struct CMD_S_UserCall {
	word chairID;
	word playerBets;
};

struct CMD_S_UserShowCard {
	word chairID;
	SGCardType_Result result;
	byte playerCards[MAX_HAND_CARD];
};

struct CMD_S_Calculate {
	int playerScores[SG_GAME_PLAYER];
};

struct CMD_S_SnatchBanker {
    byte maxRatio; // 最大倍率
	byte cardCount;
	byte cards[SG_GAME_PLAYER][MAX_HAND_CARD];
	SGPlayerStatus status[SG_GAME_PLAYER];
};

struct CMD_S_UserSnatchBanker {
    word userChairID;
    byte ratio;
};

struct CMD_S_SendCardAll {
    SGPlayerStatus playerStatus[SG_GAME_PLAYER];
    byte playerCards[SG_GAME_PLAYER][MAX_HAND_CARD];
};

struct CMD_S_SendCardAdd {
    SGPlayerStatus playerStatus[SG_GAME_PLAYER];
    byte playerCards[SG_GAME_PLAYER];
};

struct CMD_S_GameScene {
	SGGameStatus gameStatus;
	SGPlayerStatus playerStatus[SG_GAME_PLAYER];
	word bankerChairID;
	byte bankerRatio;
	byte maxRatio;
	int playerSnatchRatio[SG_GAME_PLAYER];
	word playerBetsAllow[MAX_STAKE_COUNT];
	word playerBet[SG_GAME_PLAYER];
	byte cardCount;
	byte playerCards[SG_GAME_PLAYER][MAX_HAND_CARD];
	SGCardType_Result playerCardTypes[SG_GAME_PLAYER];
	word operatorLeftTimes;
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_HOST_ALLOW_START 1 //房主开始游戏
#define SUB_C_USER_SNATCH_BANKER	 2 //抢庄
#define SUB_C_USER_DROP_BANKER 3 //弃庄
#define SUB_C_USER_CALL 4 //下注
#define SUB_C_USER_SHOW_CARD 5//亮牌

struct CMD_C_UserSnatchBanker {
    byte ratio; // 最大赔率
};

struct CMD_C_UserCall {
    word playerBets;
};
struct CMD_C_UserShowCard {
    SGCardType_Result result;
};
}
#pragma pack()
