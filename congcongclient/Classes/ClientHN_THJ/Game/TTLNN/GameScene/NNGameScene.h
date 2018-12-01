#pragma once

#include "CommonHeader/CommonHeader.h"
#include "ClientHN_THJ/Game/TTLNN/Player/NNPlayer.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNTurnCard.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"

class NNGameScene
	: public GameBase
	, public TimeNode
	, public FvSingleton<NNGameScene>
{
public:
	const static int KIND_ID = 27;
	//DONE: �޸�������
	const static int MAX_PLAYER = 8;

	const static int VERSION_SERVER = PROCESS_VERSION(6, 0, 3);		//����汾
	const static int VERSION_CLIENT = PROCESS_VERSION(6, 0, 3);		//����汾

	const static int MAX_BETS_COUNT = 5;
public:
	NNGameScene();
	~NNGameScene();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
	void initButton();
	void initPublicPanel();
	void initPanel();
	void initData();
	void initNet();
	void initTalkSence();
#pragma endregion ��ʼ��

#pragma region ��ʾ������
	void show();
	void hide();

	void showReady();
	void hideReady();
	void onReady();
	void sendReady();

	void showDropBanker();
	void hideDropBanker();

	//��˰����ʾ(���׾���ʾ)
	void showGameTax(int iGameTax);
#pragma endregion ��ʾ������
	
#pragma region ���볡��
public:
	void enterScene();
	void clearPanel();
	void hideMenuPanel();
	void resetData();
#pragma endregion ���볡��

#pragma region ������
public:
	GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	void updateUserInfo();
	int getGamePlayerCount();
	NNPlayer* getLocalPlayer();
	NNPlayer* getPlayerByChairID(WORD chairID);
	int getVisioChairID(NNPlayer& player);
	bool isBankerUser(NNPlayer& player);
	//bool isHostUser(NNPlayer& player);
#pragma endregion ������

#pragma region ��Ϸ״̬
	bool IsInGame();
	WORD getGameStatus();
	TTLNN::NNPlayerBet* getAllBets();
	WORD getSnatchRatio();
	WORD getBankerRatio();
#pragma endregion ��Ϸ״̬

#pragma region ��ť��Ϣ
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
#pragma endregion ��ť��Ϣ

#pragma region �������
	void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
#pragma endregion �������

#pragma region ���ƴ�����Ϣ����
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);

	bool isSplitCard();
	void setSelectCards(std::vector<int> selectCards);
	std::vector<int> getSelectCards();
#pragma endregion ������Ϣ����

#pragma region ��Ϸ��Ϣ�շ�
	void sendGameStart();
	void sendSnatchBanker(BYTE snatchRatio);
	void sendBet(TTLNN::NNPlayerBet& bet);
	void sendShowCard();
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
#pragma endregion ��Ϸ��Ϣ�շ�

public:

	CMD_GF_Private_Room_Info	m_RoomInfo;							// ���������Ϣ

private:

	WORD m_MaxRatio;
	WORD m_GameStatus;
	TTLNN::NNPlayerBet m_AllBets[MAX_STAKE_COUNT];

	WORD m_BankerUser;
	WORD m_BankerRatio;
	NNPlayer* m_Players[MAX_PLAYER];
	std::vector<int> m_SelectCards;
	static dword _dwSpeak_time_begin;
	static dword _dwSpeak_time_end;
	static dword _dwSpeak_time_interval;
};