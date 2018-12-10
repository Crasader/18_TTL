#pragma once
#include <Game/Script/TimeNode.h>

#include "common.h"
#include "CMD_SRLF.h"
#include CMD_GAME

#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_SRLF.h"
#include "Plaza/HomeScene/GPHomeMallPanel.h"

class SRLFPlayer;
struct CMD_S_GameEnd;
struct SRLFGameRecord;
struct CMD_WeaveItem;

class SRLFGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<SRLFGameScence> 
{
public:
	const static int KIND_ID = 302;//3人
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 3; //三人两房 //对应控件个数

	enum 
	{
		SRLF_STATE_NULL,
		SRLF_STATE_READY,
		SRLF_STATE_HUANPAI,
		SRLF_STATE_PLAYING,
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
	SRLFGameScence();
	SRLFGameScence(int iGameType,int iOption);
	~SRLFGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();
public:
	void EnterScence();
	void HideAll();
	virtual void exitRoom();
	void defaultState();
	void defaultPlayerActionState();
	void showPiao();
	void showSaiZi(unsigned int iValue);
	void showSaiZi_HuanPai(word wValue);
	void setCurNoticePlayer(int index );
	std::string getStringHuRight(dword kValue,int nGenCount);
	std::string getStringGang(int nChairID,SRLF::CMD_WeaveItem* pWeave,int iWeaveCout);
	void setCurrentPlayer(int iCurrentPlayer,int iUserAction,int icbCardData = 0);
	void setGameResoultPlayerInfo(const SRLF::AllEndInfo& pGameEnd,SRLFPlayer* pPlayer,cocos2d::Node* pNode);
	void setGameResoultStateInfo(cocos2d::Node* pNode,std::string kName,std::string kDes,int lSocre);


	void setGameResoultPlayerInfo_shuju(const SRLF::AllEndInfo& pGameEnd, SRLFPlayer* pPlayer, cocos2d::Node* pNode);

	void showClock(int iTime,bool bShow = true);
	void showClockTimeCallBack(const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime);
	void killClock(const std::string& kName);

	void showRoomUserInfo(int nIndex);

	void setGameType(int nType);

	void setPlayCount(int nCount);
	int getPlayCount();
public:
	void initButton();
	void SRLFButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_GuoAction(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_GangAction(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_PengAction(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_HuAction(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_TingAction(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_TingActionNo(cocos2d::Ref*,WidgetUserInfo*);

	void SRLFButtonAction_ShowNext(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButtonAction_ShowCard(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButtonAction_HuanPaiChengDu(cocos2d::Ref*,WidgetUserInfo*);

	void SRLFButton_ReChoice(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_OkChoice(cocos2d::Ref*,WidgetUserInfo*);

	void SRLFButton_GameExit(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_Set(cocos2d::Ref*,WidgetUserInfo*);

	void SRLFButton_QueWan(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_QueTong(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_QueTiao(cocos2d::Ref*,WidgetUserInfo*);

	void SRLFButton_JieSuanShare(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_JieSuanStart(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_JieSuanClose(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_OnceAgain(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_RoomHead(cocos2d::Ref*,WidgetUserInfo*);

	void SRLFButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
	void endButtonTalk();
	void SRLFButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
	
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void SRLFButton_friendPrivateInfo(cocos2d::Ref*,WidgetUserInfo*);
	void changeFreeStateNodeState(bool bVisible);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);
	void OnHuanPaiScence(void* data, int dataSize);
	void OnXuanQueScence(void* data, int dataSize);
public:
	SRLFPlayer* getPlayerByChairID(int iChairID);
	void upOtherPlayerInfo();
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnEventUserStatus(GamePlayer * pPlayer);
public:
	virtual void	 OnGameServerItem(CGameServerItem* pGameServerItem);
	//私人场
public:
	void initPrivate();
	void defaultPrivateState();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);
	void showJieSuanInfo(const SRLF::AllEndInfo& kEndInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);

public:
	void initNet();

	void OnSubGameStart(const void * pBuffer, word wDataSize);
	//用户出牌
	void OnSubOutCard(const void * pBuffer, word wDataSize);
	//发牌消息
	void OnSubSendCard(const void * pBuffer, word wDataSize);
	//操作提示
	void OnSubOperateNotify(const void * pBuffer, word wDataSize);
	//操作结果
	void OnSubOperateResult(const void * pBuffer, word wDataSize);
	//游戏结束
	void OnSubGameEnd(const void * pBuffer, word wDataSize);
	//用户托管
	void OnSubTrustee(const void * pBuffer,word wDataSize);
	//吃胡消息
	void OnSubUserChiHu( const void *pBuffer,word wDataSize );
	//杠得分
	void OnSubGangScore( const void *pBuffer, word wDataSize );

	//换牌
	void OnSubHuanPai(const void *pBuffer, word wDataSize );
	void OnSubHuanPaiChengDu(const void *pBuffer, word wDataSize );
	//选缺通知
	void OnSubXuanQueNotice(const void *pBuffer, word wDataSize );
	//选缺
	void OnSubXuanQue(const void *pBuffer, word wDataSize );
	//换牌通知
	void OnSubHuanPaiNotice(const void *pBuffer, word wDataSize );
	void OnSubPiaoState(const void *pBuffer, word wDataSize);
	void OnSubTingFirst(const void *pBuffer, word wDataSize);
	void OnSubPiaoNotice(const void *pBuffer, word wDataSize);
	void OnSubBaoTingNotice(const void *pBuffer, word wDataSize);
	void OnLeftTimer(const void *pBuffer, word wDataSize);
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
	void HNMJButton_DismissRoom(cocos2d::Ref*, WidgetUserInfo* pUserInfo);
	void HNMJButton_SureeDismissPrivate(cocos2d::Ref*, WidgetUserInfo* pUserInfo);
	void HNMJButton_SureeDismissPrivateNot(cocos2d::Ref*, WidgetUserInfo* pUserInfo);
public:
	static void pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow);
public:
	void Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo*);
	void HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo*);

	void Button_Menu(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Help(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Exit(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Setting(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*);
	void Button_JiaPiao(cocos2d::Ref* pRef, WidgetUserInfo* pUserInfo);

	void showReadyTimes(int times);
	void hideReadyTimes();
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

	int GetHandCardsMaxCount();
	int GetWeaveMax();

	//扣税收显示(仅首局显示)
	void showGameTax(int iGameTax);

	void updataPlayerSeat();
protected:
	int							m_iBankerUser;						//庄家用户
	int							m_iCurrentUser;						//当前用户
	int							m_iUserAction;						//玩家动作
	int							m_cbActionCard;						//操作牌克

	SRLFPlayer*					m_pLocal;
	SRLFPlayer*					m_pPlayer[MAX_PLAYER];				//玩家数组 0 2 3
	int							m_pPlayerCount;						//玩家数组数目

	cocos2d::Node*				m_pTouchCardNode;
	cocos2d::Vec2				m_kTouchSrcPos;

	Node*                       pnode;//牌堆
	int                         paitag;

	int                         m_nLeftTime;
	bool                        m_bIsHuanPai;
	word						m_wResumeUser;
	int                         m_nGameState;
	int                         m_nGameType;                  //玩法类型 四川麻将或者成都麻将

	word						m_wRecordSelfChairID;
	int							m_iActRecordIdex;
	int							m_RecordTimeID;
	float						m_RecordTime;
	SRLF::SRLFGameRecord*		m_pGameRecord;
	CMD_GF_Private_Room_Info	m_kPrivateRoomInfo;
	int                         m_nPlayCount;
	dword						m_dwGameRuleIdex;
	bool						m_bselectGangState;
	byte                        m_nGangCards[MAX_WEAVE];
	byte                        m_nGangCardsCount;
};