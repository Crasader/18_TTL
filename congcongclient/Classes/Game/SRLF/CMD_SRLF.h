#ifndef CMD_SRLF_HEAD_FILE
#define CMD_SRLF_HEAD_FILE
 
#pragma pack(1)

#include "DEFINE.h"

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
	byte							cbWeaveKind;						//�������
	byte							cbCenterCard;						//�����˿�
	byte							cbPublicCard;						//������־
	word							wProvideUser;						//��Ӧ�û�
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

#define MASK_CHI_HU_RIGHT			0x0fffffff							//���Ȩλdword����			

//��������
#define MAX_WEAVE					4									//������
#define MAX_INDEX					34									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY_SRLF 72									//�����

//��������
struct CMD_S_TING_FIRST
{
	word							wTingCardUser;						//�����û�
};

struct CMD_S_BaoTingNotice
{
	bool							cbShowBaoTing;						//������ʾ
	word							iTimer;
	word							wChairID;
};

//��Ʈ����
struct CMD_S_PiaoState
{
	byte							cbPiaoState;						//�Ƿ�Ʈ
	word							wPiaoUser;							//Ʈ���
};

//��Ʈ��ʾ
struct CMD_S_PiaoNotice
{
	bool							cbShowPiao;						//�Ƿ�Ʈ
	word							iTimer;
};


//���Ƶ÷�
struct tagGangScore
{
	byte		cbGangCount;											//�ܸ���
	longlong	lScore[MAX_WEAVE][GAME_PLAYER];							//ÿ���ܵ÷�
	byte		cbGangCount_Single;
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	longlong						lCellScore;							//�������
	word							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	longlong						lCellScore;									//��Ԫ����
	word							wBankerUser;								//ׯ���û�
	word							wCurrentUser;								//��ǰ�û�

	//״̬����
	byte							cbActionCard;								//�����˿�
	byte							cbActionMask;								//��������
	byte							cbGangCards[MAX_WEAVE];						//�����˿�
	byte							cbGangCardsCount;							//�˿�����
	byte							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	word							wWinOrder[GAME_PLAYER];						//
	byte							cbWinCout;									//��ʤ��Ŀ

	//������Ϣ
	word							wOutCardUser;								//�����û�
	byte							cbOutCardData;								//�����˿�
	byte							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	byte							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//�˿�����
	byte							cbCardCount;								//�˿���Ŀ
	byte							cbCardData[MAX_COUNT];						//�˿��б�
	byte							cbSendCardData;								//�����˿�

	//����˿�
	byte							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	//������Ϣ
	word							wHeapHand;									//����ͷ��
	word							wHeapTail;									//����β��
	byte							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ


	byte							nQueColor[GAME_PLAYER];						//ȱɫ
	byte							nChiHuCard[GAME_PLAYER][60];
	word							nChiHuCardNum[GAME_PLAYER];
	dword							dwGameRuleIdex;								//��Ϸ����
	byte							cbPiaoState[GAME_PLAYER];					//���Ʈ״̬
	bool							bTingCard[GAME_PLAYER];						//�Ƿ����ɹ�

};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	word							lSiceCount;									//���ӵ���
	word							wBankerUser;								//ׯ���û�
	word							wCurrentUser;								//��ǰ�û�
	byte							cbUserAction;								//�û�����
	byte							cbActionData;								//�����˿�
	byte							cbCardData[MAX_COUNT*GAME_PLAYER];			//�˿��б�
	word							wHeapHand;									//������ͷ
	word							wHeapTail;									//������β
	byte							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
	byte							cbLeftCardCount;							//ʣ��������
	byte							nLeftTime;									//���Ƶ���ʱ
};

struct CMD_S_StateHuanPai
{
	word							wBankerUser;								//ׯ���û�
	byte							cbCardData[MAX_COUNT];						//�˿��б�
	byte							cbLeftCardCount;							//
	byte							nLeftTime;									//���Ƶ���ʱ
	bool                            bHuan;									    //�Ƿ��Ѿ�����
};

struct CMD_S_StateXuanQue
{
	word							wBankerUser;								//ׯ���û�
	byte							cbCardData[MAX_COUNT];						//�˿��б�
	byte							cbLeftCardCount;							//
	byte							nLeftTime;									//XuanQue����ʱ
	byte                            nQueColor;									
};
//��������
struct CMD_S_OutCard
{
	word							wOutCardUser;						//�����û�
	byte							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	byte							cbCardData;							//�˿�����
	byte							cbActionMask;						//��������
	word							wCurrentUser;						//��ǰ�û�
	bool							bTail;								//ĩβ����
	byte                            nLeftTime;
	bool							bHasHuPai;							//�Ƿ��Ѿ���
	bool							bFirstSended;						//�Ƿ�������
	byte						    cbGangCard;							//�����ƻ����θ�
	bool						    bBaoTing;							//�Ƿ��Ǳ���״̬
	byte							cbGangCards[MAX_WEAVE];
	byte							cbGangCardCount;
};


//������ʾ
struct CMD_S_OperateNotify
{
	word							wResumeUser;						//��ԭ�û�
	byte							cbActionMask;						//��������
	byte							cbActionCard;						//�����˿�
	bool							bHasHuPai;
};

//��������
struct CMD_S_OperateResult
{
	word							wOperateUser;						//�����û�
	word							wProvideUser;						//��Ӧ�û�
	byte							cbOperateCode;						//��������
	byte							cbOperateCard;						//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	byte							cbCardCount[GAME_PLAYER];			//
	byte							cbCardData[GAME_PLAYER][MAX_COUNT];	//
	//������Ϣ
	word							wProvideUser[GAME_PLAYER];			//��Ӧ�û�
	dword							dwChiHuRight[GAME_PLAYER];			//��������

	//������Ϣ
	longlong						lGameScore[GAME_PLAYER];			//��Ϸ����
	int								lGameTax[GAME_PLAYER];				//

	word							wWinOrder[GAME_PLAYER];				//��������

	tagGangScore					lGangScoreInfo[GAME_PLAYER];			//��ϸ�÷�
	longlong						lGangScore[GAME_PLAYER];				//��ϸ�÷�
	longlong						lChaJiaoScore[GAME_PLAYER];//��е÷�
	longlong						lHuaZhuScore[GAME_PLAYER];//����÷�
	word							lChaJiaoFanShuMax[GAME_PLAYER];//��������

	byte							cbGenCount[GAME_PLAYER];			//
	word							wLostFanShu[GAME_PLAYER][GAME_PLAYER];
	word							wLeftUser;							//

	//����˿�
	byte							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	word							nFanShu[GAME_PLAYER];
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	word							wChairID;							//�й��û�
};

struct CMD_S_ChiHu
{
	word							wChiHuUser;							//
	word							wProviderUser;						//
	byte							cbChiHuCard;						//
	std::vector<SCORE>            lGameScore;
	dword						dwChiHuRight;			//��������
	byte							cbGenCount;							//

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
	word							wChairId;							//
	word							wPravadeId;							//
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
	word						wChairId;							//
	std::vector<byte>           cbCardsList;
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
	std::vector<byte>			    cbCard;
	std::vector<SCORE>              lGameScoreTotal;
	std::vector<SCORE>              lHuaZhuScore;
	std::vector<SCORE>              lChaJiaoScore;
	std::vector<byte>				cbGenCount;
	std::vector<byte>				cbFanShu;
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
	word   wChairId;
	byte   cbHuanCard[3];
};
struct CMD_S_HuanPaiChengDu
{
	word   wChairId;
	byte   cbRemoveHuanCard[3];
	byte   cbHuanCard[3];
	word   wSice;
};
struct CMD_S_XuanQue
{
	byte nQueColor[4];
	byte nLeftTime;
};

struct CMD_S_XuanQueNotice
{
	word	wChairId;
	byte	nLeftTime;
	byte	nQueColor;
};

struct CMD_S_LeftTimer
{
	word wCurrentUser;
	word nLeftTime;
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
	byte							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	byte							cbOperateCode;						//��������
	byte							cbOperateCard;						//�����˿�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};
//2016.5.20
struct CMD_C_HuanPai
{
	byte    cbHuanCard[3];
	byte		nMasttKindColor;
};
struct CMD_C_XuanQue
{
	byte nQueChoice;
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
	std::vector<byte> kCardList;

	bool   operator <  (const   Card_Info&   rhs   )  const   //��������ʱ����д�ĺ���
	{  
		return   nLen   <   rhs.nLen;
	}
};



struct  SRLFGameRecordPlayer
{
	dword dwUserID;
	std::string kHead;
	std::string kNickName;
	std::vector<byte> cbCardData;
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
	byte							cbActionType;
	word							wOperateUser;						//�����û�
	word							wProvideUser;						//��Ӧ�û�
	byte							cbOperateCode;						//��������
	byte							cbOperateCard;						//�����˿�
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
	std::vector<byte>    kMasterHandCard;

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
	byte							cbCheakCard;						//��������
};

struct MaterNiaoCout
{
	byte							cbNiaoCout;							//��������
};

struct MasterLeftCard
{
	byte      kMasterLeftIndex[MAX_INDEX];
	byte      kMasterCheakCard;
};
}
#pragma pack()
#endif
