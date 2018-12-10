#pragma once

#include <cocos2d.h>

#include "GamePlayer.h"
#include "Platform/PFKernel/CGPReqPlayerInfoMission.h"
#include "Kernel/kernel/game/IClientKernelSink.h"

#define addNetCBDefine(iSubCommand,pScence,pCallBack) \
	addNetCB(iSubCommand,pScence,CC_CALLBACK_2(pCallBack,pScence),#pCallBack)

struct WidgetUserInfo;
class ScenceManagerBase;

struct NET_CBInfo
{
	NET_CBInfo()
	{
		pSrc1 = nullptr;
	}
	int32 iIdex;
	cocos2d::Ref* pSrc1;
	std::string kCallFunName;
	std::function<void(void*, int)> pCallBack;
};

typedef std::vector<NET_CBInfo> NET_CBInfoList;
struct tagMatchInfo;
struct tagMatchWaitTip;
struct tagMatchResult;

class GameBase
	:public cocos2d::Node
	,public IClientKernelSink
{
public:
	GameBase(unsigned int iType,unsigned int iOption);
	~GameBase();
	
//////////////////////////////////////////////////////////////////////////

public:
	bool init();
	void setOptionRule(unsigned int iRule);
	bool HaveOptionRule(int iRule);
	void Button_ExitGameBase(cocos2d::Ref*,WidgetUserInfo*);

	virtual void exitRoom();
	virtual void exitGame();

	//���ƽӿ�
public:
	//����
	virtual void clearInfo();
	//������Ϸ
	virtual bool SetupGameClient();
	//������Ϸ
	virtual void ResetGameClient();
	//�ر���Ϸ
	virtual void CloseGameClient();
	//�رշ���
	virtual void CloseRoomClient();

	//����¼�
public:
	//ϵͳ������Ϣ
	virtual bool OnGFTableMessage(const char* szMessage);
	//������Ϣ
	virtual bool OnGFMatchInfo(tagMatchInfo* pMatchInfo);
	//�����ȴ���ʾ
	virtual bool OnGFMatchWaitTips(tagMatchWaitTip* pMatchWaitTip);
	//�������
	virtual bool OnGFMatchResult(tagMatchResult* pMatchResult);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(void* data, int dataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(int sub, void* data, int dataSize);
	//���ͺ���
	bool SendSocketData(word wSubCmdID);
	//���ͺ���
	bool SendSocketData(word wSubCmdID, void * data, word dataSize);

	void sendReady();

	void addNetCB(int iIdex,cocos2d::Ref* pScence,std::function<void(void*,int)> pCallBack,std::string kCallName);

	//�û��¼�
public:
	//�û�����
	virtual void OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual void OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual void OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual void OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual void OnEventUserStatus(GamePlayer * pPlayer){};
	//�û�����
	virtual void OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�ͷ��
	virtual void OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//˽�˷�
public:
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);

	virtual bool BackKey();
	virtual bool IsInGame() = 0;
	void Button_CB_OnExit(cocos2d::Ref*,WidgetUserInfo*);
public:
	virtual bool RevTalk(void* data,int dataSize);
	bool RevTalk_File(CMD_GR_C_TableTalk* pNetInfo);
	bool RevTalk_String(CMD_GR_C_TableTalk* pNetInfo);
	bool RevTalk_TalkIdex(CMD_GR_C_TableTalk* pNetInfo);
	bool RevTalk_BiaoQing(CMD_GR_C_TableTalk* pNetInfo);
	void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void sendTalkFile(int iChair,std::string kFile);
	void sendTalkString(int iChair,std::string strString);
    void sendTalkDefine(int iChair,std::string strSoundPath);
	void sendTalkBiaoQing(int iChair,std::string strFilePath);

protected:
	GamePlayer* getPoolPlayer(IClientUserItem * pIClientUserItem);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
public:
	virtual void LocalPlayerEnter();
	virtual void upSelfPlayerInfo() {};
public:
	GamePlayer* getGamePlayerByUserItem(IClientUserItem * pIClientUserItem);
	GamePlayer* getPlayerByChairID( word wChairID );
	GamePlayer* getPlayerByUserID( dword wUserID );

	void addGamePlayerToList(GamePlayer* pPlayer);
	void removeGamePlayerToList(GamePlayer* pPlayer);
	void getGamePlayerToList(std::vector<GamePlayer*>* kPlayers);
	GamePlayer* getSelfGamePlayer();
	word getSelfChairID();
	bool IsPrivateGame();
public:
	virtual void	 OnGameServerItem(CGameServerItem* pGameServerItem){}
protected:
	GamePlayer* m_pSelfPlayer;
	std::vector<GamePlayer*> m_kPlayers;
	NET_CBInfoList m_kCBInfoList;

	//CGPReqPlayerInfoMission		m_kReqPlayerInfo;
	std::vector<GamePlayer*>	m_kPoolPlayer;

	unsigned int m_iGameType;
	unsigned int m_iGameOption;
};