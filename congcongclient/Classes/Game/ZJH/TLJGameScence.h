#pragma once
#include "CMD_TLJ.h"

#include "GDEFINE.h"
#include "Platform/PFDefine/df/Module.h"
#include "Platform/PFDefine/msg/CMD_GameServer.h"
#include "TLJHeaders.h"
#include "TLJPlayer.h"

using namespace TLJ;
class TLJGameScence
	: public GameBase
	, public TimeNode
	, public FvSingleton<TLJGameScence>
{
public:
	const static int MAX_PLAYER = 5;
	const static int KIND_ID = 317;									//游戏 I D
	const static int VERSION_SERVER = PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT = PROCESS_VERSION(6,0,3);				//程序版本
	const static int RECORD_TAG = 65535;

	const static int WAIT_TIME = 60;
public:
	TLJGameScence();
	TLJGameScence(int iGameType, unsigned int iOption);
	~TLJGameScence();
public:
	virtual bool init();

#pragma region 游戏场景
public:
	void show();
	void hide();
	void EnterScence();
	void resetScene();
	void ResetGameClient();
#pragma endregion 

#pragma region 游戏界面
public:
	void initPanel();
	void hideAllPanels();
	void showCalculate();
#pragma endregion 

#pragma region 触摸事件
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 lPos,cocos2d::Vec2 pPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);
#pragma endregion 

#pragma region 场景按钮相关
public:
	void initButton();
	void Button_Exit(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	void Button_Setting(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	void BackHomeScene();
	void Button_TalkDefine(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
    void Button_Send_TalkStr2(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	void Button_Send_TalkStr(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	void HNMJButton_BiaoQing(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	void HideButton(bool bHide);
	void Button_Info(cocos2d::Ref*, WidgetUserInfo*);
	void Buttion_Background(cocos2d::Ref*, WidgetUserInfo*);
	//下注按钮响应
	void Button_Betting(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	//弃牌按钮响应
	void Button_QiPai(cocos2d::Ref*, WidgetUserInfo*);
	//跟注按钮响应
	void PlayerGenPai();
	//比牌按钮响应
	void Button_BiPai(cocos2d::Ref*, WidgetUserInfo*);
	//准备按钮响应
	void Button_Ready(cocos2d::Ref*, WidgetUserInfo*);
	//规则按钮响应
	void Button_Help(cocos2d::Ref*, WidgetUserInfo*);
	//刷新投注按钮 可点击状态
	void refreshBettingBtn(byte cbLevel);
	//刷新投注按钮  倍数显示
	void changeBettingBtnImagic(word wTimes);
	//看牌按钮响应
	void Button_KanPai(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);

	void Button_TalkBegin(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);

	void endButtonTalk();

	void Button_TalkEnd(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	//菜单按钮响应
	void Button_Menu(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*);

#pragma endregion 

#pragma  region 辅助函数
	//更换当前玩家
	void changeCurrentPlayer(word wChairId);
	//游戏状态
	void setGameState(word wState);
	word getGameState();
	//玩家数
	word getPlayerCount();
	//z准备下一局
	void readyToNextMatch();
	//准备状态
	void refreshReadyStatus();
	//金币校验
	bool checkUserScore(byte cbLevel);
	//准备倒计时
	void showReadyTimes(int times);
	//准备倒计时
	void hideReadyTimes();
	//扣税收显示(仅首局显示)
	void showGameTax(int iGameTax);
#pragma endregion

#pragma region 服务器消息
public:
	void initNet();
	void OnSubGameStart(const void * pBuffer, word wDataSize);
	void OnLookCard(const void * pBuffer, word wDataSize);
	void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScene(void* data, int dataSize);
	void OnPlayerScene(void* data, int dataSize);
	void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	//弃牌
	void OnSubGiveUp(const void * pBuffer, word wDataSize);
	//用户加注
	void onSubAddScore(const void * pBuffer, word wDataSize);
	//用户比牌
	void onSubCompare(const void * pBuffer, word wDataSize);
	//游戏结束
	void onSubGameEnd(const void * pBuffer, word wDataSize);
#pragma endregion 

#pragma region 客户端消息
	void playerLookCard();
	void playerGiveUp();
	void playerBetting(byte cbLevel);
	void playerCompare(word wChairID);
	 CMD_GF_Private_Room_Info getRoomInfo();
#pragma endregion

#pragma region 房间信息
public:
	void updateRoomInfo();
	std::string getRoomInfoText(bool forShare /*= false*/);
	virtual bool IsInGame();
#pragma endregion 

#pragma region 玩家信息
public:
	void updateUserInfo();
	bool isCurrentUser(TLJPlayer& player);
	bool isBankerUser(TLJPlayer& player);
	bool isLocalUser(TLJPlayer& player);

	GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	TLJPlayer* getPlayerByChairID(word chairID);
	TLJPlayer* getPlayerByVisioID(word wVisioID);
	TLJPlayer* getLocalPlayer();

	int getVisioChairID(TLJPlayer& player);
	int getRoomScore(TLJPlayer& player);
	dword getBaseScore();
	byte getScoreLevel();
	word getCurrentTimes();

	int getPlayCount();
	void setPlayCount(int nCount);

	bool isMyTurn();
#pragma endregion

#pragma region 游戏规则
	void getGameRuleStr(std::vector<std::string>& kRuleList,dword dwGameRuleIdex);
	void pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow);
#pragma endregion 

	void TLJ_Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*);
	void showWeiXin();
	void hideWeiXin();
	
public:
	CMD_GF_Private_Room_Info	m_RoomInfo;							// 房间信息
private:
	TLJPlayer*					m_pPlayers[TLJ_GAME_PLAYER];			//玩家列表
	word						m_RoomCreator;						// 创建者
	word						m_BankerUser;						// 庄家
	word						m_CurrentUser;						// 当前玩家

	bool						m_IsInGame;
	int                         m_nPlayCount;

	byte						m_cbScoreLevel;						//当前注等
	word						m_wCurrentTimes;					//当前倍数

	word						m_wGameState;						//游戏状态
	word						m_wCurrentTurn;						//游戏轮数
	longlong					m_lTotalScore;						//总注数
};
