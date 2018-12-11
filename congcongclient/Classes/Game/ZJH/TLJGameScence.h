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
	const static int KIND_ID = 317;									//��Ϸ I D
	const static int VERSION_SERVER = PROCESS_VERSION(6,0,3);				//����汾
	const static int VERSION_CLIENT = PROCESS_VERSION(6,0,3);				//����汾
	const static int RECORD_TAG = 65535;

	const static int WAIT_TIME = 60;
public:
	TLJGameScence();
	TLJGameScence(int iGameType, unsigned int iOption);
	~TLJGameScence();
public:
	virtual bool init();

#pragma region ��Ϸ����
public:
	void show();
	void hide();
	void EnterScence();
	void resetScene();
	void ResetGameClient();
#pragma endregion 

#pragma region ��Ϸ����
public:
	void initPanel();
	void hideAllPanels();
	void showCalculate();
#pragma endregion 

#pragma region �����¼�
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 lPos,cocos2d::Vec2 pPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);
#pragma endregion 

#pragma region ������ť���
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
	//��ע��ť��Ӧ
	void Button_Betting(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	//���ư�ť��Ӧ
	void Button_QiPai(cocos2d::Ref*, WidgetUserInfo*);
	//��ע��ť��Ӧ
	void PlayerGenPai();
	//���ư�ť��Ӧ
	void Button_BiPai(cocos2d::Ref*, WidgetUserInfo*);
	//׼����ť��Ӧ
	void Button_Ready(cocos2d::Ref*, WidgetUserInfo*);
	//����ť��Ӧ
	void Button_Help(cocos2d::Ref*, WidgetUserInfo*);
	//ˢ��Ͷע��ť �ɵ��״̬
	void refreshBettingBtn(byte cbLevel);
	//ˢ��Ͷע��ť  ������ʾ
	void changeBettingBtnImagic(word wTimes);
	//���ư�ť��Ӧ
	void Button_KanPai(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);

	void Button_TalkBegin(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);

	void endButtonTalk();

	void Button_TalkEnd(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	//�˵���ť��Ӧ
	void Button_Menu(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*);

#pragma endregion 

#pragma  region ��������
	//������ǰ���
	void changeCurrentPlayer(word wChairId);
	//��Ϸ״̬
	void setGameState(word wState);
	word getGameState();
	//�����
	word getPlayerCount();
	//z׼����һ��
	void readyToNextMatch();
	//׼��״̬
	void refreshReadyStatus();
	//���У��
	bool checkUserScore(byte cbLevel);
	//׼������ʱ
	void showReadyTimes(int times);
	//׼������ʱ
	void hideReadyTimes();
	//��˰����ʾ(���׾���ʾ)
	void showGameTax(int iGameTax);
#pragma endregion

#pragma region ��������Ϣ
public:
	void initNet();
	void OnSubGameStart(const void * pBuffer, word wDataSize);
	void OnLookCard(const void * pBuffer, word wDataSize);
	void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScene(void* data, int dataSize);
	void OnPlayerScene(void* data, int dataSize);
	void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	//����
	void OnSubGiveUp(const void * pBuffer, word wDataSize);
	//�û���ע
	void onSubAddScore(const void * pBuffer, word wDataSize);
	//�û�����
	void onSubCompare(const void * pBuffer, word wDataSize);
	//��Ϸ����
	void onSubGameEnd(const void * pBuffer, word wDataSize);
#pragma endregion 

#pragma region �ͻ�����Ϣ
	void playerLookCard();
	void playerGiveUp();
	void playerBetting(byte cbLevel);
	void playerCompare(word wChairID);
	 CMD_GF_Private_Room_Info getRoomInfo();
#pragma endregion

#pragma region ������Ϣ
public:
	void updateRoomInfo();
	std::string getRoomInfoText(bool forShare /*= false*/);
	virtual bool IsInGame();
#pragma endregion 

#pragma region �����Ϣ
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

#pragma region ��Ϸ����
	void getGameRuleStr(std::vector<std::string>& kRuleList,dword dwGameRuleIdex);
	void pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow);
#pragma endregion 

	void TLJ_Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*);
	void showWeiXin();
	void hideWeiXin();
	
public:
	CMD_GF_Private_Room_Info	m_RoomInfo;							// ������Ϣ
private:
	TLJPlayer*					m_pPlayers[TLJ_GAME_PLAYER];			//����б�
	word						m_RoomCreator;						// ������
	word						m_BankerUser;						// ׯ��
	word						m_CurrentUser;						// ��ǰ���

	bool						m_IsInGame;
	int                         m_nPlayCount;

	byte						m_cbScoreLevel;						//��ǰע��
	word						m_wCurrentTimes;					//��ǰ����

	word						m_wGameState;						//��Ϸ״̬
	word						m_wCurrentTurn;						//��Ϸ����
	longlong					m_lTotalScore;						//��ע��
};
