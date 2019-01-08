#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

#include <vector>

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)
#define KIND_ID						27

//�������

//DONE:�޸�������
#define NN_GAME_PLAYER 8									//��Ϸ����
//������
#define MAX_ROUND_INDEX 40
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

#define GAME_NAME					TEXT("ţ����")				//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_NN_FREE					GAME_STATUS_FREE					//����״̬
#define GS_NN_PLAY					(GAME_STATUS_PLAY+1)				//��Ϸ״̬
#define GS_NN_CALCULATE				(GAME_STATUS_PLAY+2)				//����״̬

#define TIME_FOR_SNATCH_BANKER (6+5)//(20+5)
#define TIME_FOR_USER_CALL (6+2)//(20+2)
#define TIME_FOR_USER_SPLIT_CARD (6+3)//(20+3)

//��Ϸ����
#define MAX_HAND_CARD 5 //���������
#define MAX_STAKE_COUNT 5 //��ע��Χö��

enum NNGameRound {
	NNGR_Six = 1,//���˳�
	NNGR_Eight = 2,//���˳�
};

//�Ƿ������ע
#define RandomBet

//��ע����
enum NNGameBetType {
	NNGBT_Normal,
	NNGBT_TuiZhu,
};

//�׷ֱ���
#define DiFen_BeiShu_0 1
#define DiFen_BeiShu_1 2
#define DiFen_BeiShu_2 4
#define DiFen_BeiShu_3 5

//��ע����

#define MAX_TuiZhu_INDEX	4
//ţ1��ţ3,����
#define TuiZhuBeiShu_0 3
//ţ4��ţ6,����
#define TuiZhuBeiShu_1 4
//ţ7��ţ9,�˱�
#define TuiZhuBeiShu_2 8
//ţ10����,ʮ��
#define TuiZhuBeiShu_3 10

//��ׯ����
#define SnatchRatio_0 1
#define SnatchRatio_1 2
#define SnatchRatio_2 3
#define SnatchRatio_3 4
#define SnatchRatio_ex 5

enum NNGameType {
	NNGameType_NNBanker = 0, //ţţ��ׯ
	NNGameType_HostBanker = 1, //�̶�ׯ��
	NNGameType_SnatchBanker = 2, //������ׯ
	NNGameType_SnatchBankerShowCard = 3, //������ׯ
	NNGameType_AllCompare = 4, //ͨ��ţţ
	NNGameType_NNRatio = 5, //ţ���⼸
	NNGameType_HLN = 6, //��«ţ
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
	NNGameRule_SpecialRule_SZN, //˳��ţ
	NNGameRule_SpecialRule_WHN, //�廨ţ
	NNGameRule_SpecialRule_THN, //ͬ��ţ
	NNGameRule_SpecialRule_HLN, //��«ţ
	NNGameRule_SpecialRule_ZDN, //ը��ţ
	NNGameRule_SpecialRule_WXN, //��Сţ
	NNGameRule_TZ,
	NNGameRule_TZRatio_0,
	NNGameRule_TZRatio_1,
	NNGameRule_TZRatio_2,
	NNGameRule_TZRatio_3,
	NNGameRule_ExpertSetting_NOCP,
	NNGameRule_ExpertSetting_NOJOIN,
	NNGameRule_SixRound,//���˳�
};

enum NN_GameResult {
    NN_Result_None, //δ����
    NN_Result_Win,
    NN_Result_Lose
};

enum NNPlayerStatus {
    NNPlayerStatus_Invalid, // ��Ч
    NNPlayerStatus_Sitting, // ��Ч
    NNPlayerStatus_Ready,  // ׼��
    NNPlayerStatus_Playing, // ��Ϸ��
};

enum NNGameStatus {
    NNGameStatus_Free, // δ��ʼ
	NNGameStatus_HostConfirm, //����ȷ�Ͽ�ʼ��Ϸ
	NNGameStatus_Start, // ��Ϸ��ʼ
    NNGameStatus_SnatchBanker, // ��ׯ
    NNGameStatus_Call, //��ע
    NNGameStatus_SplitCard, //����
	NNGameStatus_Calculate, // ����
};

enum NNSnatchBanker {
	NNSnatchBanker_Invalid =  -1,
	NNSnatchBanker_None,
	NNSnatchBanker_1,
	NNSnatchBanker_2,
	NNSnatchBanker_3,
	NNSnatchBanker_4,
};

//˳��ţ
#define CardTypeRatio_SZN 5
//�廨ţ
#define CardTypeRatio_WHN 5
//ͬ��ţ
#define CardTypeRatio_THN 5
//��«ţ
#define CardTypeRatio_HLN 6
//ը��ţ
#define CardTypeRatio_ZDN 6
//��Сţ
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
	NNCardType_SZN, //˳��ţ
	NNCardType_WHN, //�廨ţ
	NNCardType_THN, //ͬ��ţ
	NNCardType_HLN, //��«ţ
	NNCardType_ZDN,//ը��ţ
	NNCardType_WXN, //��Сţ
};

struct NNCardType_Result {
	NNCardType type;
	BYTE centerCard;
	bool isCardSelected[MAX_HAND_CARD];
};

struct NNPlayerCardResult {
	long wScore;
	WORD wCardType;
	bool bTuiZhu;
	bool bBanker;
};

struct NNPlayerBet {
	WORD wBet;//��ע����
	WORD wBetType;//��ע����
	bool operator == (NNPlayerBet& val) {
		return (wBet == val.wBet) && (wBetType == val.wBetType);
	}
	bool operator == (WORD bet) {
		return (wBet == bet);
	}
	NNPlayerBet& operator = (NNPlayerBet& val) {
		this->wBet = val.wBet;
		this->wBetType = val.wBetType;
		return *this;
	}
	NNPlayerBet& operator = (WORD bet) {
		this->wBet = bet;
		this->wBetType = 0;
		return *this;
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
#define SUB_S_USER_TRUSTEESHIP 112 //����й�
#define SUB_S_CALCULATE_TOTAL 113 //�ܽ���

#define SUB_S_ANDROID_RULE			200                                 //���͹����������

struct CMD_S_AndroidRule {
    BYTE							cbGameTypeIdex;	//��Ϸ����
    DWORD							dwGameRuleIdex;	//��Ϸ����
};

struct CMD_S_GameStart {
	NNPlayerStatus status[NN_GAME_PLAYER];
};

struct CMD_S_BankerInfo {
	WORD bankerChairID;//
	WORD bankerRatio;//��ׯ����
	BYTE cbSanch;//�Ƿ�����ׯ
	NNPlayerBet bets[NN_GAME_PLAYER][MAX_STAKE_COUNT];
};

struct CMD_S_UserCall {
	WORD chairID;
	NNPlayerBet playerBets;
};

struct CMD_S_UserShowCard {
	WORD chairID;
	NNCardType_Result result;
	BYTE playerCards[MAX_HAND_CARD];
};

struct CMD_S_Calculate {
	int playerScores[NN_GAME_PLAYER];
};

struct CMD_S_CalculateTotal {
	BYTE	 dwRoundCount;//��ľ���
	int banker_id[MAX_ROUND_INDEX];
	int playerScores[MAX_ROUND_INDEX];
};

struct CMD_S_SnatchBanker {
    BYTE maxRatio;// �����
	BYTE cardCount;
	BYTE cards[NN_GAME_PLAYER][MAX_HAND_CARD];
	NNPlayerStatus status[NN_GAME_PLAYER];
	BYTE bTuiZhu;//�ܷ���ע
};

struct CMD_S_UserSnatchBanker {
    WORD userChairID;
    BYTE ratio;
};

struct CMD_S_SendCardAll {
    NNPlayerStatus playerStatus[NN_GAME_PLAYER];
    BYTE playerCards[NN_GAME_PLAYER][MAX_HAND_CARD];
};

struct CMD_S_SendCardAdd {
    NNPlayerStatus playerStatus[NN_GAME_PLAYER];
    BYTE playerCards[NN_GAME_PLAYER];
};

struct CMD_S_GameScene {
	NNGameStatus gameStatus;
	NNPlayerStatus playerStatus[NN_GAME_PLAYER];
	WORD bankerChairID;
	BYTE bankerRatio;
	BYTE maxRatio;
	WORD playerSnatchRatio[NN_GAME_PLAYER];
	NNPlayerBet playerBetsAllow[NN_GAME_PLAYER][MAX_STAKE_COUNT];
	NNPlayerBet playerBet[NN_GAME_PLAYER];
	BYTE cardCount;
	BYTE playerCards[NN_GAME_PLAYER][MAX_HAND_CARD];
	NNCardType_Result playerCardTypes[NN_GAME_PLAYER];
	WORD operatorLeftTimes;
    DWORD dwBaseScore;        //��ע
};

//////////////////////////////////////////////////////////////////////////

//�ͻ�������ṹ
#define SUB_C_HOST_ALLOW_START 1 //������ʼ��Ϸ
#define SUB_C_USER_SNATCH_BANKER	 2 //��ׯ
#define SUB_C_USER_DROP_BANKER 3 //��ׯ
#define SUB_C_USER_CALL 4 //��ע
#define SUB_C_USER_SHOW_CARD 5//����

struct CMD_C_UserSnatchBanker {
    BYTE ratio; // �������
};

struct CMD_C_UserCall {
	NNPlayerBet playerBets;
};

struct CMD_C_UserShowCard {
	NNCardType_Result result;
};

#pragma pack()
#endif
