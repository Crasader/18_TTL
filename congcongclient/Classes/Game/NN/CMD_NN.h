#pragma once

#include <vector>

#include "GDEFINE.h"

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)
namespace TTLNN
{
//组件属性

//DONE:修改了人数
#define NN_GAME_PLAYER					8									//游戏人数
//最大局数
#define MAX_ROUND_INDEX 40

#define ENABLE_DISMISS_ROOM

//游戏状态
#define GS_NN_FREE					GAME_STATUS_FREE					//空闲状态
#define GS_NN_PLAY					(GAME_STATUS_PLAY+1)				//游戏状态
#define GS_NN_CALCULATE				(GAME_STATUS_PLAY+2)				//结算状态

#define TIME_FOR_SNATCH_BANKER 6//20
#define TIME_FOR_USER_CALL 6//20
#define TIME_FOR_USER_SPLIT_CARD 6//20

//游戏配置
#define MAX_HAND_CARD 5 //最大手牌数
#define MAX_STAKE_COUNT 5 //下注范围枚举

enum NNGameRound {
	NNGR_Six = 1,//六人场
	NNGR_Eight = 2,//八人场
};

//下注类型
enum NNGameBetType {
	NNGBT_Normal,
	NNGBT_TuiZhu,
};

//底分倍数
#define DiFen_BeiShu_0 1
#define DiFen_BeiShu_1 2
#define DiFen_BeiShu_2 4
#define DiFen_BeiShu_3 5

//推注倍数

#define MAX_TuiZhu_INDEX	4
//牛1到牛3,三倍
#define TuiZhuBeiShu_0 3
//牛4到牛6,三倍
#define TuiZhuBeiShu_1 4
//牛7到牛9,八倍
#define TuiZhuBeiShu_2 8
//牛10以上,十倍
#define TuiZhuBeiShu_3 10

//抢庄倍数
#define SnatchRatio_0 1
#define SnatchRatio_1 2
#define SnatchRatio_2 3
#define SnatchRatio_3 4
#define SnatchRatio_ex 5

enum NNGameType {
	NNGameType_NNBanker = 0, //牛牛上庄
	NNGameType_HostBanker = 1, //固定庄家
	NNGameType_SnatchBanker = 2, //自由抢庄
	NNGameType_SnatchBankerShowCard = 3, //明牌抢庄
	NNGameType_AllCompare = 4, //通比牛牛
	NNGameType_NNRatio = 5, //牛几赔几
	NNGameType_HLN = 6, //葫芦牛
};

enum NNGameRule {
	NNGameRule_Score_0 = 1,
	NNGameRule_Score_1,
	NNGameRule_Score_2,
	NNGameRule_Pay_0,
	NNGameRule_Pay_1,
	NNGameRule_Ratio_0,
	NNGameRule_Ratio_1,
	NNGameRule_BankerScore_0,
	NNGameRule_BankerScore_1,
	NNGameRule_BankerScore_2,
	NNGameRule_BankerScore_3,
	NNGameRule_SnatchRatio_0,
	NNGameRule_SnatchRatio_1,
	NNGameRule_SnatchRatio_2,
	NNGameRule_SnatchRatio_3,
	NNGameRule_SpecialRule_SZN, //顺子牛
	NNGameRule_SpecialRule_WHN, //五花牛
	NNGameRule_SpecialRule_THN, //同花牛
	NNGameRule_SpecialRule_HLN, //葫芦牛
	NNGameRule_SpecialRule_ZDN, //炸弹牛
	NNGameRule_SpecialRule_WXN, //五小牛
	NNGameRule_TZ,
	NNGameRule_TZRatio_0,
	NNGameRule_TZRatio_1,
	NNGameRule_TZRatio_2,
	NNGameRule_TZRatio_3,
	NNGameRule_ExpertSetting_NOCP,
	NNGameRule_ExpertSetting_NOJOIN,
	NNGameRule_SixRound,//六人场
};

enum NN_GameResult {
    NN_Result_None, //未参与
    NN_Result_Win,
    NN_Result_Lose
};

enum NNPlayerStatus {
    NNPlayerStatus_Invalid, // 无效
    NNPlayerStatus_Sitting, // 有效
    NNPlayerStatus_Ready,  // 准备
    NNPlayerStatus_Playing, // 游戏中
};

enum NNGameStatus {
    NNGameStatus_Free, // 未开始
	NNGameStatus_HostConfirm, //房主确认开始游戏
	NNGameStatus_Start, // 游戏开始
    NNGameStatus_SnatchBanker, // 抢庄
    NNGameStatus_Call, //下注
    NNGameStatus_SplitCard, //拆牌
	NNGameStatus_Calculate, // 结算
};

enum NNSnatchBanker {
	NNSnatchBanker_Invalid =  -1,
	NNSnatchBanker_None,
	NNSnatchBanker_1,
	NNSnatchBanker_2,
	NNSnatchBanker_3,
	NNSnatchBanker_4,
};

//顺子牛
#define CardTypeRatio_SZN 5
//五花牛
#define CardTypeRatio_WHN 5
//同花牛
#define CardTypeRatio_THN 5
//葫芦牛
#define CardTypeRatio_HLN 6
//炸弹牛
#define CardTypeRatio_ZDN 6
//五小牛
#define CardTypeRatio_WXN 8

enum NNCardTypeRatio_1 {
	CTR1_N1 = 1,
	CTR1_N2 = 1,
	CTR1_N3 = 1,
	CTR1_N4 = 1,
	CTR1_N5 = 1,
	CTR1_N6 = 1,
	CTR1_N7 = 2,
	CTR1_N8 = 2,
	CTR1_N9 = 3,
	CTR1_NN = 4,
};

enum NNCardTypeRatio_2 {
	CTR2_N1 = 1,
	CTR2_N2 = 1,
	CTR2_N3 = 1,
	CTR2_N4 = 1,
	CTR2_N5 = 1,
	CTR2_N6 = 1,
	CTR2_N7 = 1,
	CTR2_N8 = 2,
	CTR2_N9 = 2,
	CTR2_NN = 3,
};

enum NNCardType {
	NNCardType_Invalid,
	NNCardType_None,
	NNCardType_N1,
	NNCardType_N2,
	NNCardType_N3,
	NNCardType_N4,
	NNCardType_N5,
	NNCardType_N6,
	NNCardType_N7,
	NNCardType_N8,
	NNCardType_N9,
	NNCardType_NN,
	NNCardType_SZN, //顺子牛
	NNCardType_WHN, //五花牛
	NNCardType_THN, //同花牛
	NNCardType_HLN, //葫芦牛
	NNCardType_ZDN,//炸弹牛
	NNCardType_WXN, //五小牛
};

struct NNPlayerCard_Entity {
	bool isValid;
	word cardCount;
	byte cards[MAX_HAND_CARD];

	NNPlayerCard_Entity() {
		isValid = false;
		cardCount = 0;
		zeromemory(cards, sizeof(cards));
	}

	NNPlayerCard_Entity(byte* cards, word count) {
		isValid = true;
		cardCount = count;
		zeromemory(this->cards, sizeof(this->cards));
		memcpy(this->cards, cards, sizeof(byte)* count);
	}

	void addCard(byte card) {
		cards[cardCount++] = card;
	}
};

struct NNPlayerSingleCalculate {
	bool isValid;
	int score;

	NNPlayerSingleCalculate() {
		isValid = false;
		score = 0;
	}

	NNPlayerSingleCalculate(int score) {
		this->isValid = true;
		this->score = score;
	}
};

struct NNCardType_Result {
	NNCardType type;
	byte centerCard;
	bool isCardSelected[MAX_HAND_CARD];
};

struct NNPlayerCardResult {
	long wScore;
	word wCardType;
	bool bTuiZhu;
	bool bBanker;
};

struct NNPlayerBet {
	word wBet;//下注倍数
	word wBetType;//下注类型
	bool operator == (NNPlayerBet& val) {
		return (wBet == val.wBet) && (wBetType == val.wBetType);
	}
	bool operator == (word bet) {
		return (wBet == bet);
	}
	NNPlayerBet& operator = (NNPlayerBet& val) {
		this->wBet = val.wBet;
		this->wBetType = val.wBetType;
		return *this;
	}
	NNPlayerBet& operator = (word bet) {
		this->wBet = bet;
		this->wBetType = 0;
		return *this;
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
#define SUB_S_USER_TRUSTEESHIP 112 //玩家托管
#define SUB_S_CALCULATE_TOTAL 113 //总结算

#define SUB_S_ANDROID_RULE			200//发送规则给机器人

struct CMD_S_AndroidRule {
    byte cbGameTypeIdex;	//游戏类型
    dword dwGameRuleIdex;	//游戏规则
};

struct CMD_S_GameStart {
	NNPlayerStatus status[NN_GAME_PLAYER];
};

struct CMD_S_BankerInfo {
	word bankerChairID;
	word bankerRatio; //抢庄倍率
	NNPlayerBet bets[NN_GAME_PLAYER][MAX_STAKE_COUNT];
};

struct CMD_S_UserCall {
	word chairID;
	NNPlayerBet playerBets;
};

struct CMD_S_UserShowCard {
	word chairID;
	NNCardType_Result result;
	byte playerCards[MAX_HAND_CARD];
};

struct CMD_S_Calculate {
	int playerScores[NN_GAME_PLAYER];
};

struct CMD_S_CalculateTotal {
	byte	 dwRoundCount;//玩的局数
	int banker_id[MAX_ROUND_INDEX];
	int playerScores[MAX_ROUND_INDEX];
};
struct CMD_S_SnatchBanker {
    byte maxRatio; // 最大倍率
	byte cardCount;
	byte cards[NN_GAME_PLAYER][MAX_HAND_CARD];
	NNPlayerStatus status[NN_GAME_PLAYER];
	byte bTuiZhu;//能否推注
};

struct CMD_S_UserSnatchBanker {
    word userChairID;
    byte ratio;
};

struct CMD_S_SendCardAll {
    NNPlayerStatus playerStatus[NN_GAME_PLAYER];
    byte playerCards[NN_GAME_PLAYER][MAX_HAND_CARD];
};

struct CMD_S_SendCardAdd {
    NNPlayerStatus playerStatus[NN_GAME_PLAYER];
    byte playerCards[NN_GAME_PLAYER];
};

struct CMD_S_GameScene {
	NNGameStatus gameStatus;
	NNPlayerStatus playerStatus[NN_GAME_PLAYER];
	word bankerChairID;
	byte bankerRatio;
	byte maxRatio;
	word playerSnatchRatio[NN_GAME_PLAYER];
	NNPlayerBet playerBetsAllow[NN_GAME_PLAYER][MAX_STAKE_COUNT];
	NNPlayerBet playerBet[NN_GAME_PLAYER];
	byte cardCount;
	byte playerCards[NN_GAME_PLAYER][MAX_HAND_CARD];
	NNCardType_Result playerCardTypes[NN_GAME_PLAYER];
	word operatorLeftTimes;
	dword dwBaseScore;        //底注
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
	NNPlayerBet playerBets;
};

struct CMD_C_UserShowCard {
	NNCardType_Result result;
};

}
#pragma pack()
