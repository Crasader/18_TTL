#pragma once

#include "CMD_NN.h"
#include "GAME.h"

#include "NNPlayer.h"
#include "NNTurnCard.h"
#include "Platform/PFDefine/df/Module.h"
#include CMD_GAME

struct RoomShareInfo {
	dword dwRoomNum;
	std::string strTitle;
	std::string strDes;
	std::string strUrl;
};

class NNGameScene
	: public GameBase
	, public TimeNode
	, public FvSingleton<NNGameScene>
{
public:
	const static int KIND_ID = 27;
	//DONE: 修改了人数
	const static int MAX_PLAYER = NN_GAME_PLAYER;

	const static int VERSION_SERVER = PROCESS_VERSION(6, 0, 3);		//程序版本
	const static int VERSION_CLIENT = PROCESS_VERSION(6, 0, 3);		//程序版本

	const static int MAX_BETS_COUNT = 5;
public:
	NNGameScene();
	~NNGameScene();

#pragma region 初始化
public:
	bool init();
	void initLayout();
	void initButton();
	void initPublicPanel();
	void initPanel();
	void initData();
	void initNet();
	void initTalkSence();
#pragma endregion 初始化

#pragma region 显示与隐藏
	void show();
	void hide();

	void showReady();
	void hideReady();
	void onReady();
	void sendReady();

	void showDropBanker();
	void hideDropBanker();

	//扣税收显示(仅首局显示)
	void showGameTax(int iGameTax);
#pragma endregion 显示与隐藏
	
#pragma region 进入场景
public:
	void enterScene();
	void clearPanel();
	void hideMenuPanel();
	void resetData();
#pragma endregion 进入场景

#pragma region 玩家相关
public:
	GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	void updateUserInfo();
	int getGamePlayerCount();
	NNPlayer* getSelf();
	NNPlayer* getMaster();
	NNPlayer* getCreater();
	NNPlayer* getPlayerByChairID(word chairID);
	int getVisioChairID(NNPlayer& player);
	bool isBankerUser(NNPlayer& player);
	//bool isHostUser(NNPlayer& player);
	NNPlayer** getPlayers();
#pragma endregion 玩家相关

#pragma region 游戏状态
	void playSoundSnachBanker(float fPass);
	bool IsInGame();
	word getGameStatus();
	TTLNN::NNPlayerBet* getAllBets();
	word getSnatchRatio();
	word getBankerRatio();
#pragma endregion 游戏状态

#pragma region 按钮消息
	void setButtonsEnable(bool flag);

	void Button_Menu(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Help(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Exit(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Dismiss(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Setting(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TalkBegin(cocos2d::Ref*, WidgetUserInfo*);
	void TalkEnd();
	void TalkInterval();
	void Button_TalkEnd(cocos2d::Ref*, WidgetUserInfo*);
	void Button_DropBanker(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ButtonMessage(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TalkType_List(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TalkType_BiaoQing(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void Button_Send_TalkStr( cocos2d::Ref*,WidgetUserInfo* );
	void HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
	void Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Share(cocos2d::Ref*, WidgetUserInfo*);
#pragma endregion 按钮消息

#pragma region 房间相关
	RoomShareInfo* getShareInfo(dword dwRoomID);
	void addRoomshareInfo(CMD_GF_Private_Room_Info* pRoomInfo);
	void clearRoomShareInfo();

	void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
#pragma endregion 房间相关

#pragma region 分牌触摸消息处理
public:
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	bool isSplitCard();
	void setSelectCards(std::vector<int> selectCards);
	std::vector<int> getSelectCards();
#pragma endregion 触摸消息处理

#pragma region 游戏消息收发
	void sendGameStart();
	void sendSnatchBanker(byte snatchRatio);
	void sendBet(TTLNN::NNPlayerBet& bet);
	void sendShowCard();

	void OnEventUserStatus(GamePlayer * pPlayer) override;
	void onGameStart(const void * pBuffer, word wDataSize);
	void onHostConfirmStart(const void * pBuffer, word wDataSize);
	void onSnatchBanker(const void * pBuffer, word wDataSize);
	void onUserSnatchBanker(const void * pBuffer, word wDataSize);
	void onBankerInfo(const void * pBuffer, word wDataSize);
	void onUserCall(const void* pBuffer, word wDataSize);
	void onSplitCard(const void* pBuffer, word wDataSize);
	void onSendCardAll(const void* pBuffer, word wDataSize);
	void onSendCardAdd(const void* pBuffer, word wDataSize);
	void onUserShowCard(const void* pBuffer, word wDataSize);
	void onCalculate(const void* pBuffer, word wDataSize);
	void onGameEnd(const void* pBuffer, word wDataSize);
	void onCalculateToTal(const void* pBuffer, word wDataSize);
	bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
#pragma endregion 游戏消息收发

public:

	CMD_GF_Private_Room_Info	m_RoomInfo;// 房间结算信息

private:

	word m_MaxRatio;
	word m_GameStatus;
	TTLNN::NNPlayerBet m_AllBets[MAX_STAKE_COUNT];

	word m_BankerUser;
	word m_BankerRatio;
	NNPlayer* m_Players[MAX_PLAYER];
	std::vector<int> m_SelectCards;
	static dword _dwSpeak_time_begin;
	static dword _dwSpeak_time_end;
	static dword _dwSpeak_time_interval;

	std::map<dword, RoomShareInfo> _mpShareInfo;
};
