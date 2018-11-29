#pragma once
#include "../TLJHeaders.h"
#include "../CMD_TLJ.h"

using namespace TLJ;
struct HandCard
{
	BYTE cbCardData[TLJ_MAX_COUNT];
	BYTE cbCardCount;
	bool bHasLook;
};

struct TLJPlayer_Record {
	WORD chairID;
	DWORD userID;
	std::string nickName;
	std::string headHttp;
};

class TLJPlayer
	:public GamePlayer
{
private:
	bool						m_InRecord;
	bool						m_Valid;
	int							m_GameScore;
public:
	int							m_index;
	cocos2d::Node*				m_pSeatNode;
	WORD						m_VisioChairID;
private:
	HandCard					m_HandCard;			//手牌
	WORD						m_wState;			//游戏状态

	bool						m_bAuto;			//自动跟注状态
	WORD						m_wPlayCount;		//游戏局数
public:
	TLJPlayer();
	~TLJPlayer();
public:
	void init();
	void reset();

#pragma region 玩家信息
public:
	WORD GetChairID();
	DWORD GetUserID();
	std::string GetNickName();
	std::string GetHeadHttp();

	virtual void upPlayerInfo();
	void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
	void upPlayerInfoAfterOperate();
	WORD getVisioChairID();

	bool isValid();
	void setValid(bool valid); 

	void setVariable();

	bool isSeatInLeft();

	int getRoomScore();

	void setGameScore(int gameScore);
	int getGameScore();
	//游戏状态
	WORD getState();
	void setState(WORD wState);
	//自动投注状态
	void setAutoState(bool bState);
	bool getAutoState();
	//游戏局数
	void setPlayCount(WORD wCount);
	WORD getPlayCount();
#pragma endregion 

#pragma region 玩家手牌
public:
	void setHandCard(BYTE cbCardData[TLJ_MAX_COUNT], bool haslook);
	HandCard getHandCard();
#pragma endregion 

#pragma region 玩家操作
public:
	//进入房间
	virtual void PlayerEnter();
	//离开房间
	virtual void PlayerLeave();
	//金币变动
	void showAddGold(int iGold);
	//计时器
	void showTimes(int times);
	//下注、跟注
	void PlayerBetting(BYTE cbScoreLevel,WORD wTimes);
	//玩家弃牌
	void PlayerGiveUp(WORD wState);
	//玩家比牌
	void PlayerCompare();
#pragma endregion 
};

