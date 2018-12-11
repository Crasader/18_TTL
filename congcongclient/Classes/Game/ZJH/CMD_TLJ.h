#ifndef CMD_TLJ_HEAD_FILE
#define CMD_TLJ_HEAD_FILE

#include "GDEFINE.h"

#include <vector>

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)
namespace TLJ {
	//组件属性
#define TLJ_GAME_PLAYER				5									//游戏人数
#define TLJ_MAX_COUNT 14
#define GAME_NAME					TEXT("决战拖拉机")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型

	//游戏状态
#define GS_TLJ_FREE					GAME_STATUS_FREE					//空闲状态
#define GS_TLJ_SCORE				GAME_STATUS_PLAY					
#define GS_TLJ_PLAY					(GAME_STATUS_PLAY+1)				

enum TLJPlayerStatus {
	NNPlayerStatus_Invalid, // 无效
	NNPlayerStatus_Sitting, // 有效
	NNPlayerStatus_Ready,  // 准备
	NNPlayerStatus_Playing, // 游戏中
};
 
	//常量定义
#define TLJ_MAX_COUNT					3									//最大数目
#define MAX_REPERTORY				52									//最大库存

	//结束原因
#define GER_NO_PLAYER				0x10								//没有玩家
#define GER_COMPARECARD				0x20								//比牌结束
#define GER_OPENCARD				0x30								//开牌结束

#define GAME_TYPE_ZZ				0
#define GAME_TYPE_CS				1

#define GAME_TYPE_ZZ_ZIFU_4			1		//4人支付
#define GAME_TYPE_ZZ_YILAIDAODI		2		//一癞到底
#define GAME_TYPE_ZZ_DIFEN5			3		//底分为5
#define GAME_TYPE_ZZ_DIFEN10		4		//底分为10
#define GAME_TYPE_ZZ_DIFEN20		5		//底分为20
#define GAME_TYPE_ZZ_DIFEN50		6		//底分为50
#define GAME_TYPE_ZZ_DIFEN100		7		//底分为100
#define GAME_TYPE_ZZ_QIHU8			8		//8颗起胡
#define GAME_TYPE_ZZ_QIHU16			9		//16颗起胡

	//玩家状态
#define PLAYER_STATE_NULL			0
#define PLAYER_STATE_AN				1
#define PLAYER_STATE_MING			2
#define PLAYER_STATE_QI				3
#define PLAYER_STATE_COMPARE_LOSE	4

	//投注信息
#define SCORE_CELL_ONE				8
#define SCORE_CELL_TWO				16
#define SCORE_CELL_THREE			20
#define SCORE_CELL_FOUR				30
	//游戏状态
#define TLJ_STATUS_NULL				0
#define TLJ_STATUS_PLAYING			1
#define TLJ_STATUS_COMPARE		 	2

	//////////////////////////////////////////////////////////////////////////

	//组合子项
	struct CMD_WeaveItem {
		dword							cbWeaveKind;						//组合类型
		byte							cbCenterCard;						//中心扑克
		byte							cbPublicCard;						//公开标志
		word							wProvideUser;						//供应用户
	};

	//////////////////////////////////////////////////////////////////////////
	//服务器命令结构
	//服务器命令结构
#define SUB_S_GAME_START_TLJ				100									//游戏开始
#define SUB_S_ADD_SCORE_TLJ					101									//加注结果
#define SUB_S_GIVE_UP_TLJ					102									//放弃跟注
#define SUB_S_SEND_CARD_TLJ					103									//发牌消息
#define SUB_S_GAME_END_TLJ					104									//游戏结束
#define SUB_S_COMPARE_CARD_TLJ				105									//比牌跟注
#define SUB_S_LOOK_CARD_TLJ					106									//看牌跟注
#define SUB_S_PLAYER_EXIT_TLJ				107									//用户强退
#define SUB_S_OPEN_CARD_TLJ					108									//开牌消息
#define SUB_S_WAIT_COMPARE_TLJ				109									//等待比牌
#define SUB_S_TRUSTEE_TLJ					110									//用户托管

	//游戏状态
	struct CMD_S_StatusFree
	{
		longlong							lCellScore;							//基础积分
	};

	//游戏状态
	struct CMD_S_StatusPlay
	{
		//加注信息
		byte								cbScoreLevel;								//单元下注
		word								wCurrentTimes;								//当前倍数

		//状态信息
		word								wBankerUser;								//庄家用户
		word				 				wCurrentUser;								//当前玩家
		byte								cbPlayStatus[TLJ_GAME_PLAYER];					//游戏状态
		bool								bMingStatus[TLJ_GAME_PLAYER];					//看牌状态
		//扑克信息
		byte								cbHandCardData[TLJ_MAX_COUNT];					//扑克数据
		//状态信息
		bool								bCompareState;								//比牌状态		

		word								wCurrentTurn;								//当前轮数
		longlong							lTotalScore;								//总注数
		longlong							lUserScore[TLJ_GAME_PLAYER];				//玩家投注数
		word								wChips[2][5];								//筹码
		dword								dwBaseScore;									//底分
	};

	//游戏开始
	struct CMD_S_GameStart
	{
		//下注信息
		byte								cbScoreLevel;						//单元下注
		word								wCurrentTimes;						//当前倍数

		//用户信息
		word								wBankerUser;						//庄家用户
		word				 				wCurrentUser;						//当前玩家

		word				 				wGameTax;							//游戏税收
		word								wCurrentTurn;						//当前轮数
		longlong							lTotalScore;						//总注数
	};


	//机器人扑克
	struct CMD_S_AndroidCard
	{
		byte								cbHandCard[TLJ_GAME_PLAYER][TLJ_MAX_COUNT];	//手上扑克
		word								wCurrentUser ;						//当前玩家
		word								m_wBankerUser;						//庄家用户
		longlong							lCurrentTimes;						//当前倍数
		longlong							lMaxScore;							//最大下注
		longlong							lCellScore;							//单元下注
		longlong							lUserMaxScore;						//分数上限
		byte								cbPlayStatus[TLJ_GAME_PLAYER];			//游戏状态

	};

	//用户托管
	struct CMD_S_Trustee
	{
		bool							bTrustee;							//是否托管
		word							wChairID;							//托管用户
	};

	//用户下注
	struct CMD_S_AddScore
	{
		word								wCurrentUser;						//当前用户
		word								wAddScoreUser;						//加注用户
		word								wCompareState;						//比牌状态
		byte								cbScoreLevel;						//加注数目
		word								wCurrentTimes;						//当前倍数	
		word								wCurrentTurn;						//当前轮数
		longlong							lTotalScore;						//总注数
	};

	//用户放弃
	struct CMD_S_GiveUp
	{
		word								wGiveUpUser;						//放弃用户
		word								wCurrentUser;						//当前用户
		word								wCurrentTurn;						//当前轮数
	};

	//比牌数据包
	struct CMD_S_CompareCard
	{
		word								wCurrentUser;						//当前用户
		word								wCompareUser[2];					//比牌用户
		word								wLostUser;							//输牌用户
		word								wCurrentTurn;						//当前轮数
		longlong							lTotalScore;						//总注数
	};

	//看牌数据包
	struct CMD_S_LookCard
	{
		word								wLookCardUser;						//看牌用户
		byte								cbCardData[TLJ_MAX_COUNT];				//用户扑克
	};

	//发送扑克
	struct CMD_S_SendCard
	{
		byte								cbCardData[TLJ_GAME_PLAYER][TLJ_MAX_COUNT];	//用户扑克
	};

	//开牌数据包
	struct CMD_S_OpenCard
	{
		word								wWinner;							//胜利用户
	};

	//游戏结束
	struct CMD_S_GameEnd
	{
		longlong							lTotalScore;							//游戏得分
		byte								cbCardData[TLJ_GAME_PLAYER][3];			//用户扑克
		word								wCompareUser[TLJ_GAME_PLAYER][5];		//比牌用户
		word								wEndState;								//结束状态
		word								wWinner;								//赢家
	};

	//用户退出
	struct CMD_S_PlayerExit
	{
		word								wPlayerID;							//退出用户
	};

	//等待比牌
	struct CMD_S_WaitCompare
	{
		word								wCompareUser;						//比牌用户
	};	
	//////////////////////////////////////////////////////////////////////////

	//客户端命令结构
#define SUB_C_ADD_SCORE					1									//用户加注
#define SUB_C_GIVE_UP					2									//放弃消息
#define SUB_C_COMPARE_CARD				3									//比牌消息
#define SUB_C_LOOK_CARD					4									//看牌消息
#define SUB_C_OPEN_CARD					5									//开牌消息
#define SUB_C_WAIT_COMPARE				6									//等待比牌
#define SUB_C_FINISH_FLASH				7									//完成动画

	//用户加注
	struct CMD_C_AddScore
	{
		byte								cbScoreLevel;					//加注等级
	};

	//比牌数据包
	struct CMD_C_CompareCard
	{	
		word								wCompareUser;						//比牌用户
	};
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
