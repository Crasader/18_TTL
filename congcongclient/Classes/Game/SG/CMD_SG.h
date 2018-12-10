#pragma once

#include <vector>
#include "DEFINE.h"
 
//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)
namespace SG
{
//�������
#define SG_GAME_PLAYER					6									//��Ϸ����

//��Ϸ״̬
#define GS_SG_FREE					GAME_STATUS_FREE					//����״̬
#define GS_SG_PLAY					(GAME_STATUS_PLAY+1)				//��Ϸ״̬
#define GS_SG_CALCULATE				(GAME_STATUS_PLAY+2)				//����״̬

#define TIME_FOR_SNATCH_BANKER 6//20
#define TIME_FOR_USER_CALL 	6//20
#define TIME_FOR_USER_SPLIT_CARD 6//20

//��Ϸ����
#define MAX_HAND_CARD 3 //���������
#define MAX_STAKE_COUNT 5 //��ע��Χö��

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
    SG_Result_None, //δ����
    SG_Result_Win,
    SG_Result_Lose
};

enum SGPlayerStatus {
    SGPlayerStatus_Invalid, // ��Ч
    SGPlayerStatus_Sitting, // ��Ч
    SGPlayerStatus_Ready,  // ׼��
    SGPlayerStatus_Playing, // ��Ϸ��
};

enum SGGameStatus {
    SGGameStatus_Free, // δ��ʼ
    SGGameStatus_HostConfirm, //����ȷ�Ͽ�ʼ��Ϸ
    SGGameStatus_Start, // ��Ϸ��ʼ
    SGGameStatus_SnatchBanker, // ��ׯ
    SGGameStatus_Call, //��ע
    SGGameStatus_SplitCard, //����
	SGGameStatus_Calculate, // ����
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
	SGCardType_None, //����
	SGCardType_HSG, //������
	SGCardType_XSG, //С����
	SGCardType_BZ, // ����
	SGCardType_DSG, //������
};

enum SGCardCompareResult{
	SGCardCompareResult_Win, //Ӯ
	SGCardCompareResult_Lose, //��
	SGCardCompareResult_Draw, //ƽ��
};

//struct SGCardType_Result {
//    SGCardType type;
//    byte centerCard;
//    bool isCardSelected[MAX_HAND_CARD];
//};

struct SGCardType_Result {
	SGCardType type; //����-SGCardType
	byte cardPoint; //����
	dword comparePoint; //���Ƶ���-��������
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
//����������ṹ
#define SUB_S_GAME_START 100 //��Ϸ��ʼ
#define SUB_S_HOST_CONFIRM_START 101 // ����ȷ�Ͽ�ʼ
#define SUB_S_SNATCH_BANKER 102  //��ׯ
#define SUB_S_USER_SNATCH_BANKER 103 //�û���ׯ
#define SUB_S_BANKER_INFO 104  //��ׯ
#define SUB_S_USER_CALL 105 //�����ע
#define SUB_S_SEND_CARD_ALL 106 //����
#define SUB_S_SEND_CARD_ADD 107 //����
#define SUB_S_SPLIT_CARD 108 //֪ͨ��Ҳ���
#define SUB_S_USER_SHOW_CARD 109 //�������
#define SUB_S_CALCULATE 110 //���ֽ���
#define SUB_S_NOTIFY_GAME_END 111 // ��Ϸ����
// #define SUB_S_SNATCH_BANKER 101	//��ׯ
// #define SUB_S_USER_SNATCH_BANKER 102 //�����ׯ
// #define SUB_S_BANKER_INFO 103 //����ׯ����Ϣ
//
// #define SUB_S_SEND_CARD_ALL 104 //��������
// #define SUB_S_SEND_CARD_FIRST 105 //��һ�η���
// #define SUB_S_SEND_CARD_SECOND 106 //�ڶ��η���
//
// #define SUB_S_USER_SHOW_CARD 107 //�������
// #define SUB_S_COMPARE_CARD_RESULT 108 //����
//
// #define SUB_S_CALCULATE 109 //���ֽ���
// #define SUB_S_GAME_END 110 //��Ϸ����

#define SUB_S_ANDROID_RULE			200                                 //���͹����������

struct CMD_S_AndroidRule {
    byte							cbGameTypeIdex;	//��Ϸ����
    dword							dwGameRuleIdex;	//��Ϸ����
};

struct CMD_S_GameStart {
	SGPlayerStatus status[SG_GAME_PLAYER];
};

struct CMD_S_BankerInfo {
	word bankerChairID;
	word bankerRatio; //��ׯ����
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
    byte maxRatio; // �����
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
//�ͻ�������ṹ
#define SUB_C_HOST_ALLOW_START 1 //������ʼ��Ϸ
#define SUB_C_USER_SNATCH_BANKER	 2 //��ׯ
#define SUB_C_USER_DROP_BANKER 3 //��ׯ
#define SUB_C_USER_CALL 4 //��ע
#define SUB_C_USER_SHOW_CARD 5//����

struct CMD_C_UserSnatchBanker {
    byte ratio; // �������
};

struct CMD_C_UserCall {
    word playerBets;
};
struct CMD_C_UserShowCard {
    SGCardType_Result result;
};
}
#pragma pack()
