#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include "Game/Script/TimeNode.h"
#include "CMD_WZQ.h"
#include "GamePlaza/HomeScene/EditNumPanel/GPHomeEditNumPanel.h"


class WZQDPlayer;
struct CMD_S_GameEnd;
struct GameRecord;
struct CMD_WeaveItem;

class WZQDGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<WZQDGameScence> 
{
public:
	const static int KIND_ID = 888;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//����汾
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 2;

	enum 
	{
		WZQ_STATE_NULL,
		WZQ_STATE_READY,
		WZQ_STATE_HUANPAI,
		WZQ_STATE_PLAYING,
	};
	enum 
	{
		GAME_OPTION_RULE_AUTO_CARD = 1,
		GAME_OPTION_RULE_SHOW_WIN_LOSE1,
		GAME_OPTION_RULE_AUTO_HU ,
		GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO,

		GAME_OPTION_RULE_HIDE_GAMETYPE,
		
		GAME_OPTION_RULE_HIDE_GUO,
		GAME_OPTION_RULE_PRIVATAEND_RETURN_HOME,

		GAME_OPTION_RULE_SHOW_TING_CARD,
		GAME_OPTION_RULE_USERID_ADD,
	};
public:
	WZQDGameScence();
	WZQDGameScence(int iGameType,int iOption);
	~WZQDGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();
public:
	void removeAllPanel();
	void initPanel();
	void EnterScence();
	void HideAll();
    virtual void exitRoom();
	void defaultState();
	void setGameResoultPlayerInfo(const WZQ::AllEndInfo& pGameEnd,WZQDPlayer* pPlayer,cocos2d::Node* pNode);
	void setGameResoultStateInfo(cocos2d::Node* pNode,std::string kName,std::string kDes,int lSocre);
	void setCurrentPlayer(int iCurrentPlayer,int iUserAction,int icbCardData);
//	void setGameResoultPlayerInfo_shuju(const AllEndInfo& pGameEnd, WZQDPlayer* pPlayer, cocos2d::Node* pNode);

	void showClock(int iTime,bool bShow = true);
	void showClockTimeCallBack(const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime);
	void killClock(const std::string& kName);
	void showRoomUserInfo(int nIndex);
	void setGameType(int nType);

	void setPlayCount(int nCount);
	int getPlayCount();
	void updateScore(BYTE cbIndex,dword wScore);
public:
	void initButton();
	void WZQButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButtonAction_HuanPaiChengDu(cocos2d::Ref*,WidgetUserInfo*);

	void WZQButton_SHURu(cocos2d::Ref*,WidgetUserInfo*);
	void WZQButton_GameExit(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_Set(cocos2d::Ref*,WidgetUserInfo*);


	void XZDDButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*);

	void XZDDButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();

	void WZQButton_RENshu(cocos2d::Ref*,WidgetUserInfo*);//���䰴ť
	void WZQButtonWeiXin(cocos2d::Ref*,WidgetUserInfo*);
	
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
    void OnPlayScence(void* data, int dataSize);
//	void OnHuanPaiScence(void* data, int dataSize);
//	void OnXuanQueScence(void* data, int dataSize);
public:
	WZQDPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnEventUserStatus(GamePlayer * pPlayer);
//public:
//	virtual void	 OnGameServerItem(CGameServerItem* pGameServerItem);
//	//˽�˳�
//public:
	void initPrivate();
	void defaultPrivateState();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);
//	void showJieSuanInfo(const AllEndInfo& kEndInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
//
public:
	void initNet();

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//�û�����
	void OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	void OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	void OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//�������
	void OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	void OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��й�
	void OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//�O�ÌW�ֳɹ�
	void OnSubSetXueFei(const void * pBuffer, WORD wDataSize);

	//�Ժ���Ϣ
	void OnSubUserChiHu( const void *pBuffer,WORD wDataSize );
	//�ܵ÷�
	void OnSubGangScore( const void *pBuffer, WORD wDataSize );

	//����
	void OnSubHuanPai(const void *pBuffer, WORD wDataSize );
	void OnSubHuanPaiChengDu(const void *pBuffer, WORD wDataSize );
	//ѡȱ֪ͨ
	void OnSubXuanQueNotice(const void *pBuffer, WORD wDataSize );
	//ѡȱ
	void OnSubXuanQue(const void *pBuffer, WORD wDataSize );
	//����֪ͨ
	void OnSubHuanPaiNotice(const void *pBuffer, WORD wDataSize );
//public:
//	void Command_PlaceBet(int iArea,int iBetScore);
//	void SendOutCard(cocos2d::Node* pCard);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);
//
//	void onHuanPaiTouchEvent(cocos2d::Vec2 kPos);
//public:
//	void XuanQueTimeOut();
//	void DoXuanQueNotice();
//public:
//	void initRecord();
//	void defaultRecordState();
//	bool StartRecord(datastream kDataStream);
//	void NextRecordAction();
//	virtual void onGPAccountInfoHttpIP(dword dwUserID, std::string strIP,std::string strHttp);
//
//	void Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*);
//	void Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*);
//	void Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*);
//	void Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*);
//	void Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*);
//
//public:
//	void remmoveAutoAction();
//	void OnAutoOperate(int cbAction);
//	void OnAutoOutCard(int cbCardData);
//	void showHuanPai();
//public:
//	void initSetButton();
//	void HNMJ_Button_Hua(cocos2d::Ref*,WidgetUserInfo*);
//	void SetBarSider1(cocos2d::Ref*,WidgetUserInfo*);
//	void SetBarSider2(cocos2d::Ref*,WidgetUserInfo*);
//	void Button_HideSet(cocos2d::Ref*,WidgetUserInfo*);
//	void Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo);
public:
	static void pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow);
protected:
	int							m_iBankerUser;						//ׯ���û�
	int							m_iCurrentUser;						//��ǰ�û�
	int							m_iUserAction;						//��Ҷ���

	WZQDPlayer*					m_pLocal;
	WZQDPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Vec2				m_kTouchSrcPos;

	int                         m_GameZT;

	int                         m_nLeftTime;
	bool                        m_bIsHuanPai;
	word						m_wResumeUser;
	int                         m_nGameState;
	int                         m_nGameType;                  //�淨���� �Ĵ��齫���߳ɶ��齫

	word						m_wRecordSelfChairID;
	int							m_iActRecordIdex;
	int							m_RecordTimeID;
	float						m_RecordTime;
	WZQ::GameRecord*				m_pGameRecord;
	CMD_GF_Private_Room_Info	m_kPrivateRoomInfo;

	int                         m_nPlayCount;
	long long                   m_WZQScore;
};