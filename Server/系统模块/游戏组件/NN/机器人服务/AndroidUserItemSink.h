#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "../游戏服务器/stdafx.h"
#include "../游戏服务器/GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//加注信息
protected:
	LONGLONG						m_lTurnMaxScore;						//最大下注

	//用户扑克
protected:
	BYTE							m_HandCardData[MAX_HAND_CARD];				//用户数据							
	BYTE							m_MaxRatio;													//最大抢庄倍率
	NNPlayerBet m_AllBets[MAX_STAKE_COUNT];
	//控件变量
public:
	CGameLogic						m_GameLogic;							//游戏逻辑
	IAndroidUserItem *				m_pIAndroidUserItem;					//用户接口

	//银行操作
	LONGLONG						m_lRobotBankGetScore;					//提款数额
	int								m_nRobotBankStorageMul;					//存款倍数
	LONGLONG						m_lBankerLostScore;						//庄家输分 (当机器人为庄)
	LONGLONG						m_lRobotScoreRange[2];					//最大范围
	
	TCHAR							m_szRoomName[32];						//配置房间
	//函数定义
public:
	//构造函数
	CAndroidUserItemSink();
	//析构函数
	virtual ~CAndroidUserItemSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { delete this; }
	//接口查询
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//控制接口
public:
	//初始接口
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//重置接口
	virtual bool  RepositionSink();

	//游戏事件
public:
	//时间消息
	virtual bool  OnEventTimer(UINT nTimerID);
	//游戏消息
	virtual bool  OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏消息
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//场景消息
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//用户事件
public:
	//用户进入
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户离开
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户积分
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户状态
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//用户段位
	virtual void  OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//消息处理
protected:
	//用户叫庄
	bool OnSnatchBanker(const void * pBuffer, WORD wDataSize);
	//庄家信息
	bool OnBankerInfo(const void * pBuffer, WORD wDataSize);
	//补牌
	bool OnSendCardAdd(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//用户加注
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//发牌消息
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//用户摊牌
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//用户强退
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

private:
	//银行操作
	void BankerOperate();
	//读取配置
	void ReadConfigInformation(TCHAR szRoomName[]);
};

//////////////////////////////////////////////////////////////////////////

#endif
