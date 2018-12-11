#pragma once

#include "common.h"
#include "CMD_XZDD.h"
#include "Platform/PFDefine/df/Module.h"
#include "Plaza/HomeScene/GPHomeMallPanel.h"
#include CMD_GAME

class XZDDPlayer;
struct CMD_S_GameEnd;
struct XZDDGameRecord;
struct CMD_WeaveItem;

class XZDDGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<XZDDGameScence> 
{
public:
	const static int KIND_ID = 303;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//����汾
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 4;

	enum 
	{
		XZDD_STATE_NULL,
		XZDD_STATE_READY,
		XZDD_STATE_HUANPAI,
		XZDD_STATE_PLAYING,
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
	XZDDGameScence();
	XZDDGameScence(int iGameType,int iOption);
	~XZDDGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();
public:
	void EnterScence();
	void HideAll();
	virtual void exitRoom();
	void defaultState();
	void defaultPlayerActionState();
	void updataPlayerZhuangState();
	void showSaiZi(unsigned int iValue);
	void showSaiZi_HuanPai(word wValue);
	std::string getStringHuRight(dword kValue,int nGenCount);
	std::string getStringGang(int nChairID,XZDD::CMD_WeaveItem* pWeave,int iWeaveCout);
	void setCurrentPlayer(int iCurrentPlayer, int iUserAction, int icbCardData = 0);
	
	void setGameResoultPlayerInfo(const XZDD::AllEndInfo& pGameEnd, XZDDPlayer* pPlayer, cocos2d::Node* pNode);
	void setGameResoultStateInfo(cocos2d::Node* pNode,std::string kName,std::string kDes,int lSocre);

	void setGameResoultPlayerInfo_shuju(const XZDD::AllEndInfo& pGameEnd, XZDDPlayer* pPlayer, cocos2d::Node* pNode);

	void showClock(int iTime,bool bShow = true);
	void showClockTimeCallBack(const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime);
	void killClock(const std::string& kName);

	void showRoomUserInfo(int nIndex);

	void setGameType(int nType);

	void setPlayCount(int nCount);
	int getPlayCount();
public:
	void initButton();
	void XZDDButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_GangAction(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_PengAction(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_HuAction(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButtonAction_HuanPaiChengDu(cocos2d::Ref*,WidgetUserInfo*);

	void XZDDButton_ReChoice(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_OkChoice(cocos2d::Ref*,WidgetUserInfo*);

	void XZDDButton_GameExit(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_Set(cocos2d::Ref*,WidgetUserInfo*);

	void XZDDButton_QueWan(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_QueTong(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_QueTiao(cocos2d::Ref*,WidgetUserInfo*);

	void XZDDButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_OnceAgain(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_RoomHead(cocos2d::Ref*,WidgetUserInfo*);

	void XZDDButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void XZDDButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
	
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);

	void Button_Menu(cocos2d::Ref*, WidgetUserInfo*);//�¼�
	void Button_Exit(cocos2d::Ref*, WidgetUserInfo*);//�¼�
	void Button_Setting(cocos2d::Ref*, WidgetUserInfo*);//�¼�
	void Button_XDDRoom(cocos2d::Ref*, WidgetUserInfo*);//�¼�
	
	
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
	void OnHuanPaiScence(void* data, int dataSize);
	void OnXuanQueScence(void* data, int dataSize);
public:
	XZDDPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnEventUserStatus(GamePlayer * pPlayer);
public:
	virtual void	 OnGameServerItem(CGameServerItem* pGameServerItem);
	//˽�˳�
public:
	void initPrivate();
	void defaultPrivateState();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);
	void friendPrivateInfo();//���÷���˽����Ϣ
	void showJieSuanInfo(const XZDD::AllEndInfo& kEndInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);

public:
	void initNet();

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//�û�����
	void OnSubOutCard(const void * pBuffer, word wDataSize);
	//������Ϣ
	void OnSubSendCard(const void * pBuffer, word wDataSize);
	//������ʾ
	void OnSubOperateNotify(const void * pBuffer, word wDataSize);
	//�������
	void OnSubOperateResult(const void * pBuffer, word wDataSize);
	//��Ϸ����
	void OnSubGameEnd(const void * pBuffer, word wDataSize);
	//�û��й�
	void OnSubTrustee(const void * pBuffer,word wDataSize);
	//�Ժ���Ϣ
	void OnSubUserChiHu( const void *pBuffer,word wDataSize );
	//�ܵ÷�
	void OnSubGangScore( const void *pBuffer, word wDataSize );

	//����
	void OnSubHuanPai(const void *pBuffer, word wDataSize );
	void OnSubHuanPaiChengDu(const void *pBuffer, word wDataSize );
	//ѡȱ֪ͨ
	void OnSubXuanQueNotice(const void *pBuffer, word wDataSize );
	//ѡȱ
	void OnSubXuanQue(const void *pBuffer, word wDataSize );
	//����֪ͨ
	void OnSubHuanPaiNotice(const void *pBuffer, word wDataSize );

	void OnSubTimerNotice(const void *pBuffer, word wDataSize);
public:
	void Command_PlaceBet(int iArea,int iBetScore);
	void SendOutCard(cocos2d::Node* pCard);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);

	void onHuanPaiTouchEvent(cocos2d::Vec2 kPos);
public:
	void XuanQueTimeOut();
	void DoXuanQueNotice();
public:
	void initRecord();
	void defaultRecordState();
	bool StartRecord(datastream kDataStream);
	void NextRecordAction();
	virtual void onGPAccountInfoHttpIP(dword dwUserID, std::string strIP,std::string strHttp);

	void Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*);

public:
	void remmoveAutoAction();
	void OnAutoOperate(int cbAction);
	void OnAutoOutCard(int cbCardData);
	void showHuanPai();
public:
	void initSetButton();
	void HNMJ_Button_Hua(cocos2d::Ref*,WidgetUserInfo*);
	void SetBarSider1(cocos2d::Ref*,WidgetUserInfo*);
	void SetBarSider2(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HideSet(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo);
public:
	static void pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow);
public:
	void Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo*);
public:
	void OnMasterHandCard( const void *pBuffer, word wDataSize );
	void OnMasterLeftCard( const void *pBuffer, word wDataSize );
	void updateUserRight();
	void showMater();
	void initMaster();
	void defaultMaster(bool bRestZhaNiao = false);
	void setMasterCheakCard(byte cbCard);
	void HNMJButton_Master(cocos2d::Ref*,WidgetUserInfo*);
	void Button_MasterClose(cocos2d::Ref*,WidgetUserInfo*);
	void NHMJ_MASTER_LEFTCARD(cocos2d::Ref*,WidgetUserInfo*);
	void MasterZhaNiao(cocos2d::Ref*,WidgetUserInfo*);

	int HuoQuPaiTag(int paitag);
	void Setpaidui();
	void Setpaidui_fasle();
	bool isXZDDGameStatusHSZ();//Ѫս������

protected:
	word						m_iBankerUser;						//ׯ���û�
	word						m_iCurrentUser;						//��ǰ�û�
	dword						m_iUserAction;						//��Ҷ���
	byte                        m_cbOutCardData;                    //����������������
	byte                        m_cbActionCardData;					//���ܺ�����������
	byte                        m_cbSendCardData;					//��������

	XZDDPlayer*					m_pLocal;
	XZDDPlayer*					m_pPlayer[MAX_PLAYER];

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Node*				m_pTouchCardNode_old;//
	cocos2d::Vec2				m_kTouchSrcPos;

	Node*                       pnode;//�ƶ�
	int                         paitag;
	int                         QiSiWeiZ;//������ʼλ��
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
	XZDD::XZDDGameRecord*		m_pGameRecord;
	CMD_GF_Private_Room_Info	m_kPrivateRoomInfo;

	int                         m_nPlayCount;
	int                         m_Touch_CS; //��������
	bool						m_bselectGangState;						//��Ҷ���
	byte                        m_nGangCards[MAX_WEAVE];
	byte                        m_nGangCardsCount;
};
