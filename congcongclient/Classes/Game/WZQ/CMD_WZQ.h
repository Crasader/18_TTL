#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

 #include "GDEFINE.h"

#pragma pack(1)

namespace WZQ
{
//组件属性
#define GAME_PLAYER					2									//游戏人数

#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_WZQ_FREE					GAME_STATUS_FREE					//空闲状态
#define GS_WZQ_PLAY					(GAME_STATUS_PLAY)					//游戏状态

#define GS_MJ_HUANPAI                 (GAME_STATUS_PLAY+2)						//换牌状态
#define GS_MJ_XUANQUE                 (GAME_STATUS_PLAY+3)						//选缺状态

#define TIME_START_GAME				30									//开始定时器
#define TIME_OPERATE_CARD				15									//操作定时器

#define		GAME_TYPE_SICHUAN  0                      //四川麻将
#define		GAME_TYPE_CHENGDU  1					   //成都麻将
#define		GAME_TYPE_YICHANG  2

#define     Game_zhi_fu_AA     1
#define     Game_zhi_fu_ZF     2

#define		GAME_RULE_NO_WAIT       1
#define		GAME_RULE_XZ            2
#define		GAME_RULE_XL			3
#define		GAME_RULE_HUAN3		    4
#define		GAME_RULE_DIN_QUE		5
#define     GAME_RULE_DI_FEN0       6//封顶
#define     GAME_RULE_DI_FEN1       7
#define     GAME_RULE_DI_FEN2       8
#define     GAME_RULE_G_S_P         9//呼叫转移
#define     GAME_RULE_ZI_MO_JIAFAN     10//自摸加倍
#define     GAME_RULE_D_G_H_ZIMO    11//点杠花当自摸
#define     GAME_RULE_Jiang_dui     12//将对
#define     GAME_RULE_MEN_Q         13//门清
#define     GAME_RULE_T_D_HU        14//天地胡

#define MAX_GAME_RULE				2		//最多规则
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//组合子项
struct CMD_WeaveItem
{
	byte							cbWeaveKind;						//组合类型
	byte							cbCenterCard;						//中心扑克
	byte							cbPublicCard;						//公开标志
	word							wProvideUser;						//供应用户
};

////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//出牌命令
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_OPERATE_NOTIFY		106									//操作提示
#define SUB_S_OPERATE_RESULT		105									//操作命令
#define SUB_S_GAME_END				104									//游戏结束
#define SUB_S_TRUSTEE				107									//用户托管
#define SUB_S_CHI_HU				108									//
#define SUB_S_GANG_SCORE			110									//
#define SUB_S_SET_XUEFEI			111									//设置学费

//2016.5.20
#define SUB_S_HUANPAI 120				//换牌
#define SUB_S_XUANQUE 121				//选缺
#define SUB_S_XUANQUE_NOTICE 122		//通知开始选缺
#define SUB_S_HUANPAI_CHENGDU 123				//换牌

#define SUB_S_HUANPAI_NOTICE  125				//换牌

#define SUB_S_MASTER_HANDCARD		220									//
#define SUB_S_MASTER_LEFTCARD		221									//剩余牌堆


#define ZI_PAI_COUNT	7												//堆立全牌

#define MASK_CHI_HU_RIGHT			0x0fffffff							//最大权位dword个数			

//常量定义
#define MAX_WEAVE					4									//最大组合
#define MAX_INDEX					34									//最大索引
#define MAX_COUNT					14									//最大数目
#define MAX_REPERTORY				108									//最大库存

//杠牌得分
struct tagGangScore
{
	byte		cbGangCount;							//杠个数
	longlong	lScore[MAX_WEAVE][GAME_PLAYER];			//每个杠得分
	byte     cbGangCount_Single;
};

//游戏状态
struct CMD_S_StatusFree
{
	longlong						lCellScore;							//基础金币
	word							wBankerUser;						//庄家用户
};

//游戏状态
struct CMD_S_StatusPlay
{
	//加注信息
	longlong							lScorePay;									//学费		
	//状态信息
	word								wBankerUser;								//庄家用户		
};

//游戏开始
//游戏开始
struct CMD_S_GameStart
{
	//用户信息
	word								wBankerUser;						//庄家用户
	word				 				wCurrentUser;						//当前玩家
};

struct CMD_S_StateHuanPai
{
	word							wBankerUser;								//庄家用户
	byte							cbCardData[MAX_COUNT];						//扑克列表
	byte							cbLeftCardCount;							//
	byte							nLeftTime;									//换牌倒计时
	bool                            bHuan;									    //是否已经换了
};

struct CMD_S_StateXuanQue
{
	word							wBankerUser;								//庄家用户
	byte							cbCardData[MAX_COUNT];						//扑克列表
	byte							cbLeftCardCount;							//
	byte							nLeftTime;									//XuanQue倒计时
	byte                            nQueColor;									
};

//落子命令
struct CMD_S_OutCard
{
	word							wOutCardUser;						//出牌用户
	byte							cbOutCardData;						//落子位子
};

//发送扑克
struct CMD_S_SendCard
{
	byte							cbCardData;							//扑克数据
	byte							cbActionMask;						//动作掩码
	word							wCurrentUser;						//当前用户
	bool							bTail;								//末尾发牌

	byte                           nLeftTime;

	bool                        bHasHuPai;					//是否已经胡
};


//操作提示
struct CMD_S_OperateNotify
{
	word							wResumeUser;						//还原用户
	byte							cbActionMask;						//动作掩码
	byte							cbActionCard;						//动作扑克
	bool                            bHasHuPai;
};

//操作命令
struct CMD_S_OperateResult
{
	word							wOperateUser;						//操作用户
	word							wProvideUser;						//供应用户
	byte							cbOperateCode;						//操作代码
	byte							cbOperateCard;						//操作扑克
};

//游戏结束
struct CMD_S_GameEnd
{
	longlong							lGameScore[GAME_PLAYER];			//游戏得分
	word								dwChairId;							//认输椅子
};

//用户托管
struct CMD_S_Trustee
{
	bool							bTrustee;							//是否托管
	word							wChairID;							//托管用户
};

struct CMD_S_ChiHu
{
	word							wChiHuUser;							//
	word							wProviderUser;						//
	byte							cbChiHuCard;						//
	std::vector<SCORE>            lGameScore;
	dword						dwChiHuRight;			//胡牌类型
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

//设置学费成功
struct CMD_S_Set_Success
{
	longlong							lXueFeiScore;						//设置学费
};

struct AllEndInfo
{
	std::vector<SCORE>            lGameScoreTotal;
	std::vector<SCORE>            lHuaZhuScore;
	std::vector<SCORE>            lChaJiaoScore;
	std::vector<byte>				cbGenCount;
	std::vector<byte>				cbFanShu;
	std::vector<CMD_S_ChiHu>		kChiHuInfoList;
	std::vector<CMD_S_GangScore>   kGangInfoList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(lGameScoreTotal);
		Stream_VECTOR(lHuaZhuScore);
		Stream_VECTOR(lChaJiaoScore);
		Stream_VECTOR(cbGenCount);
		Stream_VECTOR(cbFanShu);
		StructVecotrMember(CMD_S_ChiHu,kChiHuInfoList);
		StructVecotrMember(CMD_S_GangScore,kGangInfoList);
	}
	void resizeData()
	{
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
	word   wChairId;
	byte nLeftTime;
	byte nQueColor;
};
////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			3									//操作扑克
#define SUB_C_TRUSTEE				4									//用户托管

#define SUB_C_MASTER_LEFTCARD		6									//剩余牌堆
#define SUB_C_MASTER_CHEAKCARD		7									//选择的牌
#define SUB_C_MASTER_ZHANIAO		8									//扎鸟
//2016.5.20
#define SUB_C_HUANPAI 11				//换牌
#define SUB_C_XUANQUE 12				//选缺

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
//出牌命令

#define SUB_C_SET_XUEFEI		8									//设置学费
#define SUB_C_LOSESELF			9									//认输

//设置学费
struct CMD_C_SetStudyPay
{
	longlong							lStudyPay;					//求高手指导金
};

struct CMD_C_OutCard
{
	byte							cbCardData;							//扑克数据
};

//操作命令
struct CMD_C_OperateCard
{
	byte							cbOperateCode;						//操作代码
	byte							cbOperateCard;						//操作扑克
};

//用户托管
struct CMD_C_Trustee
{
	bool							bTrustee;							//是否托管	
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
////////////////////////////////////////////////////////////////////////


struct  Card_Info
{
	int		nLen;
	int		nColor;
	std::vector<byte> kCardList;

	bool   operator <  (const   Card_Info&   rhs   )  const   //升序排序时必须写的函数
	{  
		return   nLen   <   rhs.nLen;
	}
};



struct  XZDDGameRecordPlayer
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

struct  XZDDGameRecordOperateResult
{
	enum Type
	{
		TYPE_NULL,
		TYPE_OperateResult,
		TYPE_SendCard,
		TYPE_OutCard,
		TYPE_ChiHu,
	};
	XZDDGameRecordOperateResult()
	{
		cbActionType = 0;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0;
		cbOperateCard = 0;
	}
	byte							cbActionType;
	word							wOperateUser;						//操作用户
	word							wProvideUser;						//供应用户
	byte							cbOperateCode;						//操作代码
	byte							cbOperateCard;						//操作扑克
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbActionType);
		Stream_VALUE(wOperateUser);
		Stream_VALUE(wProvideUser);
		Stream_VALUE(cbOperateCode);
		Stream_VALUE(cbOperateCard);
	}
};
struct  GameRecord
{
	std::vector<XZDDGameRecordPlayer> kPlayers;
	std::vector<XZDDGameRecordOperateResult> kAction;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(XZDDGameRecordPlayer,kPlayers);
		StructVecotrMember(XZDDGameRecordOperateResult,kAction);
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
	byte							cbCheakCard;						//操作代码
};

struct MaterNiaoCout
{
	byte							cbNiaoCout;							//操作代码
};

struct MasterLeftCard
{
	byte      kMasterLeftIndex[MAX_INDEX];
	byte      kMasterCheakCard;
};
};
#pragma pack()
#endif
