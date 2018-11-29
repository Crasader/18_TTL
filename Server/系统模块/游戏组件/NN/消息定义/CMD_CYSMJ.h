#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE
#include <vector>

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)

#define KIND_ID						307									//��Ϸ I D

//�������
#define GAME_PLAYER					6									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

#define GAME_NAME					TEXT("һ������")				//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_NN_FREE					GAME_STATUS_FREE					//����״̬
#define GS_NN_PLAY					(GAME_STATUS_PLAY+1)				//��Ϸ״̬
#define GS_NN_CALCULATE				(GAME_STATUS_PLAY+2)				//����״̬

//��������

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_START 100 //��Ϸ��ʼ
#define SUB_S_GAME_SNATCH_BANKER 101	//��ׯ
#define SUB_S_GAME_SEND_BANKER 102 //����ׯ����Ϣ
#define SUB_S_SEND_CARD_ALL 103 //��������
#define SUB_S_SEND_CARD_FIRST 104 //��һ�η���
#define SUB_S_SEND_CARD_SECOND 105 //�ڶ��η���
#define SUB_S_COMPARE_CARD 106 //����
#define SUB_S_GAME_CALCULATE 107 //���ֽ���
#define SUB_S_GAME_END 108 //��Ϸ����

#define SUB_S_ANDROID_RULE			200                                 //���͹����������

struct CMD_S_AndroidRule {
	BYTE							cbGameTypeIdex;	//��Ϸ����
	DWORD							dwGameRuleIdex;	//��Ϸ����
};

enum NN_GameRule {
	NN_RULE_WHN,
	NN_RULE_ZDN,
	NN_RULE_WXN,
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_USER_SNATCH_BANKER	 1 //��ׯ
#define SUB_C_BACK_CALL_START 2 //ׯ�ҿ�ʼ��Ϸ
#define SUB_C_USER_DROP_BANKER 3 //��ׯ
#define SUB_C_USER_CALL 4 //��ע


#pragma pack()
#endif
