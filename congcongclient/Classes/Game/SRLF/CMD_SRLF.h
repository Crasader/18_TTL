#ifndef CMD_SRLF_HEAD_FILE
#define CMD_SRLF_HEAD_FILE
#include "Game/Game/GameDefine.h"

#pragma pack(1)
namespace SRLF
{
//�������
#define		GAME_PLAYER					3									//��Ϸ����
#define		GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define		GS_MJ_FREE					GAME_STATUS_FREE					//����״̬
#define		GS_MJ_PLAY					(GAME_STATUS_PLAY+1)				//��Ϸ״̬

#define		GS_MJ_HUANPAI               (GAME_STATUS_PLAY+2)				//����״̬
#define		GS_MJ_XUANQUE               (GAME_STATUS_PLAY+3)				//ѡȱ״̬

#define		TIME_START_GAME				30									//��ʼ��ʱ��
#define		TIME_OPERATE_CARD			15									//������ʱ��

#define		GAME_TYPE_SICHUAN			0									//�Ĵ��齫
#define		GAME_TYPE_CHENGDU			1									//�ɶ��齫
#define		GAME_TYPE_YICHANG			2

#define		GAME_RULE_NO_WAIT			1
#define		GAME_RULE_XZ				2
#define		GAME_RULE_XL				3
#define		GAME_RULE_HUAN3				4
#define		GAME_RULE_DIN_QUE			5

#define		MAX_GAME_RULE				2		//������
//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_CHI_HU				108									//
#define SUB_S_GANG_SCORE			110									//

//2016.5.20
#define SUB_S_HUANPAI 120				//����
#define SUB_S_XUANQUE 121				//ѡȱ
#define SUB_S_XUANQUE_NOTICE 122		//֪ͨ��ʼѡȱ
#define SUB_S_HUANPAI_CHENGDU 123				//����
#define SUB_S_HUANPAI_ANDROID 124
#define SUB_S_HUANPAI_NOTICE  125				//����

#define SUB_S_PIAOSTATE		  126									//Ʈ
#define SUB_S_TING_FIRST	  127									//�Ƿ���
#define SUB_S_PIAO_NOTICE	  128									//Ʈ��ʾ ��ʾ����ʾ
#define SUB_S_BAOTING_NOTICE  129									//������ʾ ��ʾ����ʾ
#define SUB_S_LEFT_TIMER	  130									//ʣ��ʱ��

#define SUB_S_ANDROID_RULE			200                                 //���͹����������
#define SUB_S_MASTER_HANDCARD		220									//
#define SUB_S_MASTER_LEFTCARD		221									//ʣ���ƶ�

#define ZI_PAI_COUNT	7												//����ȫ��

#define MASK_CHI_HU_RIGHT			0x0fffffff							//���ȨλDWORD����			

//��������
#define MAX_WEAVE					4									//������
#define MAX_INDEX					34									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY_SRLF 72									//�����

//��������
struct CMD_S_TING_FIRST
{
	WORD							wTingCardUser;						//�����û�
};

struct CMD_S_BaoTingNotice
{
	bool							cbShowBaoTing;						//������ʾ
	WORD							iTimer;
	WORD							wChairID;
};

//��Ʈ����
struct CMD_S_PiaoState
{
	BYTE							cbPiaoState;						//�Ƿ�Ʈ
	WORD							wPiaoUser;							//Ʈ���
};

//��Ʈ��ʾ
struct CMD_S_PiaoNotice
{
	bool							cbShowPiao;						//�Ƿ�Ʈ
	WORD							iTimer;
};


//���Ƶ÷�
struct tagGangScore
{
	BYTE		cbGangCount;											//�ܸ���
	LONGLONG	lScore[MAX_WEAVE][GAME_PLAYER];							//ÿ���ܵ÷�
	BYTE		cbGangCount_Single;
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�

	//״̬����
	BYTE							cbActionCard;								//�����˿�
	BYTE							cbActionMask;								//��������
	BYTE							cbGangCards[MAX_WEAVE];						//�����˿�
	BYTE							cbGangCardsCount;							//�˿�����
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	WORD							wWinOrder[GAME_PLAYER];						//
	BYTE							cbWinCout;									//��ʤ��Ŀ

	//������Ϣ
	WORD							wOutCardUser;								//�����û�
	BYTE							cbOutCardData;								//�����˿�
	BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//�˿�����
	BYTE							cbCardCount;								//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	//������Ϣ
	WORD							wHeapHand;									//����ͷ��
	WORD							wHeapTail;									//����β��
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ


	BYTE							nQueColor[GAME_PLAYER];						//ȱɫ
	BYTE							nChiHuCard[GAME_PLAYER][60];
	WORD							nChiHuCardNum[GAME_PLAYER];
	dword							dwGameRuleIdex;								//��Ϸ����
	BYTE							cbPiaoState[GAME_PLAYER];					//���Ʈ״̬
	bool							bTingCard[GAME_PLAYER];						//�Ƿ����ɹ�

};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	QYLONG							lSiceCount;									//���ӵ���
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	BYTE							cbUserAction;								//�û�����
	BYTE							cbActionData;								//�����˿�
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//�˿��б�
	WORD							wHeapHand;									//������ͷ
	WORD							wHeapTail;									//������β
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
	BYTE							cbLeftCardCount;							//ʣ��������
	BYTE							nLeftTime;									//���Ƶ���ʱ
};

struct CMD_S_StateHuanPai
{
	WORD							wBankerUser;								//ׯ���û�
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//���Ƶ���ʱ
	bool                            bHuan;									    //�Ƿ��Ѿ�����
};

struct CMD_S_StateXuanQue
{
	WORD							wBankerUser;								//ׯ���û�
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//XuanQue����ʱ
	BYTE                            nQueColor;									
};
//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bTail;								//ĩβ����
	BYTE                            nLeftTime;
	bool							bHasHuPai;							//�Ƿ��Ѿ���
	bool							bFirstSended;						//�Ƿ�������
	BYTE						    cbGangCard;							//�����ƻ����θ�
	bool						    bBaoTing;							//�Ƿ��Ǳ���״̬
	BYTE							cbGangCards[MAX_WEAVE];
	BYTE							cbGangCardCount;
};


//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
	bool							bHasHuPai;
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbCardCount[GAME_PLAYER];			//
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//
	//������Ϣ
	WORD							wProvideUser[GAME_PLAYER];			//��Ӧ�û�
	dword							dwChiHuRight[GAME_PLAYER];			//��������

	//������Ϣ
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
	int								lGameTax[GAME_PLAYER];				//

	WORD							wWinOrder[GAME_PLAYER];				//��������

	tagGangScore					lGangScoreInfo[GAME_PLAYER];			//��ϸ�÷�
	LONGLONG						lGangScore[GAME_PLAYER];				//��ϸ�÷�
	LONGLONG						lChaJiaoScore[GAME_PLAYER];//��е÷�
	LONGLONG						lHuaZhuScore[GAME_PLAYER];//����÷�
	WORD							lChaJiaoFanShuMax[GAME_PLAYER];//��������

	BYTE							cbGenCount[GAME_PLAYER];			//
	WORD							wLostFanShu[GAME_PLAYER][GAME_PLAYER];
	WORD							wLeftUser;							//

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	WORD							nFanShu[GAME_PLAYER];
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

struct CMD_S_ChiHu
{
	WORD							wChiHuUser;							//
	WORD							wProviderUser;						//
	BYTE							cbChiHuCard;						//
	std::vector<SCORE>            lGameScore;
	DWORD						dwChiHuRight;			//��������
	BYTE							cbGenCount;							//

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChiHuUser);
		Stream_VALUE(wProviderUser);
		Stream_VALUE(cbChiHuCard);
		Stream_VECTOR(lGameScore);
		Stream_VALUE(dwChiHuRight);
		Stream_VALUE(cbGenCount);
	}	
	void resizeData()
	{
		lGameScore.resize(GAME_PLAYER);
	}
};

//
struct CMD_S_GangScore
{
	WORD							wChairId;							//
	WORD							wPravadeId;							//
	bool                         cbXiaYu;
	std::vector<SCORE>            lGangScore;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChairId);
		Stream_VALUE(wPravadeId);
		Stream_VALUE(cbXiaYu);
		Stream_VECTOR(lGangScore);
	}
	void resizeData()
	{
		lGangScore.resize(GAME_PLAYER);
	}	
};

struct CMD_S_GameEndCards
{
	WORD						wChairId;							//
	std::vector<BYTE>           cbCardsList;
	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(wChairId);
		Stream_VECTOR(cbCardsList);
	}
	void resizeData()
	{
		cbCardsList.resize(MAX_COUNT);
	}
};

struct AllEndInfo
{
	std::vector<BYTE>			    cbCard;
	std::vector<SCORE>              lGameScoreTotal;
	std::vector<SCORE>              lHuaZhuScore;
	std::vector<SCORE>              lChaJiaoScore;
	std::vector<BYTE>				cbGenCount;
	std::vector<BYTE>				cbFanShu;
	std::vector<CMD_S_ChiHu>		kChiHuInfoList;
	std::vector<CMD_S_GangScore>    kGangInfoList;
	std::vector<CMD_S_GameEndCards> kGameEndCardsList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(cbCard);
		Stream_VECTOR(lGameScoreTotal);
		Stream_VECTOR(lHuaZhuScore);
		Stream_VECTOR(lChaJiaoScore);
		Stream_VECTOR(cbGenCount);
		Stream_VECTOR(cbFanShu);
		StructVecotrMember(CMD_S_ChiHu,kChiHuInfoList);
		StructVecotrMember(CMD_S_GangScore,kGangInfoList);
		StructVecotrMember(CMD_S_GameEndCards,kGameEndCardsList);
	}
	void resizeData()
	{
		cbCard.resize(GAME_PLAYER*MAX_COUNT);
		lGameScoreTotal.resize(GAME_PLAYER);
		lHuaZhuScore.resize(GAME_PLAYER);
		lChaJiaoScore.resize(GAME_PLAYER);
		cbGenCount.resize(GAME_PLAYER);
		cbFanShu.resize(GAME_PLAYER);
	}
};

//2016.5.20
struct CMD_S_HuanPai
{
	WORD   wChairId;
	BYTE   cbHuanCard[3];
};
struct CMD_S_HuanPaiChengDu
{
	WORD   wChairId;
	BYTE   cbRemoveHuanCard[3];
	BYTE   cbHuanCard[3];
	WORD   wSice;
};
struct CMD_S_XuanQue
{
	BYTE nQueColor[4];
	BYTE nLeftTime;
};

struct CMD_S_XuanQueNotice
{
	WORD	wChairId;
	BYTE	nLeftTime;
	BYTE	nQueColor;
};

struct CMD_S_LeftTimer
{
	WORD wCurrentUser;
	WORD nLeftTime;
};


//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�

#define SUB_C_MASTER_LEFTCARD		6									//ʣ���ƶ�
#define SUB_C_MASTER_CHEAKCARD		7									//ѡ�����
#define SUB_C_MASTER_ZHANIAO		8									//����
//2016.5.20
#define SUB_C_HUANPAI 11				//����
#define SUB_C_XUANQUE 12				//ѡȱ
#define SUB_C_PIAO    13				//��Ʈ
#define SUB_C_TING_FIRST 14				//�û�����

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};
//2016.5.20
struct CMD_C_HuanPai
{
	BYTE    cbHuanCard[3];
	BYTE		nMasttKindColor;
};
struct CMD_C_XuanQue
{
	BYTE nQueChoice;
};

struct CMD_C_PiaoState
{
	bool							cbPiaoState;						//�Ƿ�Ʈ
};
//////////////////////////////////////////////////////////////////////////


struct  Card_Info
{
	int		nLen;
	int		nColor;
	std::vector<BYTE> kCardList;

	bool   operator <  (const   Card_Info&   rhs   )  const   //��������ʱ����д�ĺ���
	{  
		return   nLen   <   rhs.nLen;
	}
};



struct  SRLFGameRecordPlayer
{
	DWORD dwUserID;
	std::string kHead;
	std::string kNickName;
	std::vector<BYTE> cbCardData;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(kHead);
		Stream_VALUE(kNickName);
		Stream_VECTOR(cbCardData);
	}
};

struct  SRLFGameRecordOperateResult
{
	enum Type
	{
		TYPE_NULL,
		TYPE_OperateResult,
		TYPE_SendCard,
		TYPE_OutCard,
		TYPE_ChiHu,
	};
	SRLFGameRecordOperateResult()
	{
		cbActionType = 0;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0;
		cbOperateCard = 0;
	}
	BYTE							cbActionType;
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbActionType);
		Stream_VALUE(wOperateUser);
		Stream_VALUE(wProvideUser);
		Stream_VALUE(cbOperateCode);
		Stream_VALUE(cbOperateCard);
	}
};
struct  SRLFGameRecord
{
	std::vector<SRLFGameRecordPlayer> kPlayers;
	std::vector<SRLFGameRecordOperateResult> kAction;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(SRLFGameRecordPlayer,kPlayers);
		StructVecotrMember(SRLFGameRecordOperateResult,kAction);
	}

};


struct MasterHandCardInfo
{
	int nChairId;
	std::vector<BYTE>    kMasterHandCard;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(nChairId);
		Stream_VECTOR(kMasterHandCard);
	}
};

struct MasterHandCard
{
	std::vector<MasterHandCardInfo>    kMasterHandCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(MasterHandCardInfo,kMasterHandCardList);
	}
};
struct MaterCheckCard
{
	BYTE							cbCheakCard;						//��������
};

struct MaterNiaoCout
{
	BYTE							cbNiaoCout;							//��������
};

struct MasterLeftCard
{
	BYTE      kMasterLeftIndex[MAX_INDEX];
	BYTE      kMasterCheakCard;
};
}
#pragma pack()
#endif
