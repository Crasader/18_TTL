#ifndef CMD_TLJ_HEAD_FILE
#define CMD_TLJ_HEAD_FILE
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)
namespace TLJ {
	//�������
#define TLJ_GAME_PLAYER				5									//��Ϸ����
#define GAME_NAME					TEXT("��ս������")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

	//��Ϸ״̬
#define GS_TLJ_FREE					GAME_STATUS_FREE					//����״̬
#define GS_TLJ_SCORE				GAME_STATUS_PLAY					
#define GS_TLJ_PLAY					(GAME_STATUS_PLAY+1)				

enum TLJPlayerStatus {
	NNPlayerStatus_Invalid, // ��Ч
	NNPlayerStatus_Sitting, // ��Ч
	NNPlayerStatus_Ready,  // ׼��
	NNPlayerStatus_Playing, // ��Ϸ��
};
 
	//��������
#define TLJ_MAX_COUNT					3									//�����Ŀ
#define MAX_REPERTORY				52									//�����

	//����ԭ��
#define GER_NO_PLAYER				0x10								//û�����
#define GER_COMPARECARD				0x20								//���ƽ���
#define GER_OPENCARD				0x30								//���ƽ���

#define GAME_TYPE_ZZ				0
#define GAME_TYPE_CS				1

#define GAME_TYPE_ZZ_ZIFU_4			1		//4��֧��
#define GAME_TYPE_ZZ_YILAIDAODI		2		//һ񮵽��
#define GAME_TYPE_ZZ_DIFEN5			3		//�׷�Ϊ5
#define GAME_TYPE_ZZ_DIFEN10		4		//�׷�Ϊ10
#define GAME_TYPE_ZZ_DIFEN20		5		//�׷�Ϊ20
#define GAME_TYPE_ZZ_DIFEN50		6		//�׷�Ϊ50
#define GAME_TYPE_ZZ_DIFEN100		7		//�׷�Ϊ100
#define GAME_TYPE_ZZ_QIHU8			8		//8�����
#define GAME_TYPE_ZZ_QIHU16			9		//16�����

	//���״̬
#define PLAYER_STATE_NULL			0
#define PLAYER_STATE_AN				1
#define PLAYER_STATE_MING			2
#define PLAYER_STATE_QI				3
#define PLAYER_STATE_COMPARE_LOSE	4

	//Ͷע��Ϣ
#define SCORE_CELL_ONE				8
#define SCORE_CELL_TWO				16
#define SCORE_CELL_THREE			20
#define SCORE_CELL_FOUR				30
	//��Ϸ״̬
#define TLJ_STATUS_NULL				0
#define TLJ_STATUS_PLAYING			1
#define TLJ_STATUS_COMPARE		 	2

	//////////////////////////////////////////////////////////////////////////

	//�������
	struct CMD_WeaveItem {
		DWORD							cbWeaveKind;						//�������
		BYTE							cbCenterCard;						//�����˿�
		BYTE							cbPublicCard;						//������־
		WORD							wProvideUser;						//��Ӧ�û�
	};

	//////////////////////////////////////////////////////////////////////////
	//����������ṹ
	//����������ṹ
#define SUB_S_GAME_START_TLJ				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE_TLJ					101									//��ע���
#define SUB_S_GIVE_UP_TLJ					102									//������ע
#define SUB_S_SEND_CARD_TLJ					103									//������Ϣ
#define SUB_S_GAME_END_TLJ					104									//��Ϸ����
#define SUB_S_COMPARE_CARD_TLJ				105									//���Ƹ�ע
#define SUB_S_LOOK_CARD_TLJ					106									//���Ƹ�ע
#define SUB_S_PLAYER_EXIT_TLJ				107									//�û�ǿ��
#define SUB_S_OPEN_CARD_TLJ					108									//������Ϣ
#define SUB_S_WAIT_COMPARE_TLJ				109									//�ȴ�����
#define SUB_S_TRUSTEE_TLJ					110									//�û��й�

	//��Ϸ״̬
	struct CMD_S_StatusFree
	{
		LONGLONG							lCellScore;							//��������
	};

	//��Ϸ״̬
	struct CMD_S_StatusPlay
	{
		//��ע��Ϣ
		BYTE								cbScoreLevel;								//��Ԫ��ע
		WORD								wCurrentTimes;								//��ǰ����

		//״̬��Ϣ
		WORD								wBankerUser;								//ׯ���û�
		WORD				 				wCurrentUser;								//��ǰ���
		BYTE								cbPlayStatus[TLJ_GAME_PLAYER];					//��Ϸ״̬
		bool								bMingStatus[TLJ_GAME_PLAYER];					//����״̬
		//�˿���Ϣ
		BYTE								cbHandCardData[TLJ_MAX_COUNT];					//�˿�����
		//״̬��Ϣ
		bool								bCompareState;								//����״̬		

		WORD								wCurrentTurn;								//��ǰ����
		LONGLONG							lTotalScore;								//��ע��
		LONGLONG							lUserScore[TLJ_GAME_PLAYER];				//���Ͷע��
		WORD								wChips[2][5];								//����
		DWORD								dwBaseScore;									//�׷�
	};

	//��Ϸ��ʼ
	struct CMD_S_GameStart
	{
		//��ע��Ϣ
		BYTE								cbScoreLevel;						//��Ԫ��ע
		WORD								wCurrentTimes;						//��ǰ����

		//�û���Ϣ
		WORD								wBankerUser;						//ׯ���û�
		WORD				 				wCurrentUser;						//��ǰ���

		WORD				 				wGameTax;							//��Ϸ˰��
		WORD								wCurrentTurn;						//��ǰ����
		LONGLONG							lTotalScore;						//��ע��
	};


	//�������˿�
	struct CMD_S_AndroidCard
	{
		BYTE								cbHandCard[TLJ_GAME_PLAYER][TLJ_MAX_COUNT];	//�����˿�
		WORD								wCurrentUser ;						//��ǰ���
		WORD								m_wBankerUser;						//ׯ���û�
		LONGLONG							lCurrentTimes;						//��ǰ����
		LONGLONG							lMaxScore;							//�����ע
		LONGLONG							lCellScore;							//��Ԫ��ע
		LONGLONG							lUserMaxScore;						//��������
		BYTE								cbPlayStatus[TLJ_GAME_PLAYER];			//��Ϸ״̬

	};

	//�û��й�
	struct CMD_S_Trustee
	{
		bool							bTrustee;							//�Ƿ��й�
		WORD							wChairID;							//�й��û�
	};

	//�û���ע
	struct CMD_S_AddScore
	{
		WORD								wCurrentUser;						//��ǰ�û�
		WORD								wAddScoreUser;						//��ע�û�
		WORD								wCompareState;						//����״̬
		BYTE								cbScoreLevel;						//��ע��Ŀ
		WORD								wCurrentTimes;						//��ǰ����	
		WORD								wCurrentTurn;						//��ǰ����
		LONGLONG							lTotalScore;						//��ע��
	};

	//�û�����
	struct CMD_S_GiveUp
	{
		WORD								wGiveUpUser;						//�����û�
		WORD								wCurrentUser;						//��ǰ�û�
		WORD								wCurrentTurn;						//��ǰ����
	};

	//�������ݰ�
	struct CMD_S_CompareCard
	{
		WORD								wCurrentUser;						//��ǰ�û�
		WORD								wCompareUser[2];					//�����û�
		WORD								wLostUser;							//�����û�
		WORD								wCurrentTurn;						//��ǰ����
		LONGLONG							lTotalScore;						//��ע��
	};

	//�������ݰ�
	struct CMD_S_LookCard
	{
		WORD								wLookCardUser;						//�����û�
		BYTE								cbCardData[TLJ_MAX_COUNT];				//�û��˿�
	};

	//�����˿�
	struct CMD_S_SendCard
	{
		BYTE								cbCardData[TLJ_GAME_PLAYER][TLJ_MAX_COUNT];	//�û��˿�
	};

	//�������ݰ�
	struct CMD_S_OpenCard
	{
		WORD								wWinner;							//ʤ���û�
	};

	//��Ϸ����
	struct CMD_S_GameEnd
	{
		LONGLONG							lTotalScore;							//��Ϸ�÷�
		BYTE								cbCardData[TLJ_GAME_PLAYER][3];			//�û��˿�
		WORD								wCompareUser[TLJ_GAME_PLAYER][5];		//�����û�
		WORD								wEndState;								//����״̬
		WORD								wWinner;								//Ӯ��
	};

	//�û��˳�
	struct CMD_S_PlayerExit
	{
		WORD								wPlayerID;							//�˳��û�
	};

	//�ȴ�����
	struct CMD_S_WaitCompare
	{
		WORD								wCompareUser;						//�����û�
	};	
	//////////////////////////////////////////////////////////////////////////

	//�ͻ�������ṹ
#define SUB_C_ADD_SCORE					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������Ϣ
#define SUB_C_COMPARE_CARD				3									//������Ϣ
#define SUB_C_LOOK_CARD					4									//������Ϣ
#define SUB_C_OPEN_CARD					5									//������Ϣ
#define SUB_C_WAIT_COMPARE				6									//�ȴ�����
#define SUB_C_FINISH_FLASH				7									//��ɶ���

	//�û���ע
	struct CMD_C_AddScore
	{
		BYTE								cbScoreLevel;					//��ע�ȼ�
	};

	//�������ݰ�
	struct CMD_C_CompareCard
	{	
		WORD								wCompareUser;						//�����û�
	};
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
