#pragma once
#include <cocos2d.h>
#include "Kernel/kernel/user/IClientUserItem.h"
#include "Platform/PFKernel/CGPReqPlayerInfoMission.h"

struct CMD_GR_C_TableTalk;

class GamePlayer : public ICGPReqPlayerInfoSink
{
public:
	GamePlayer(IClientUserItem* pUserItem);
	~GamePlayer();
	void clear();
public:
	void setUserItem(IClientUserItem* pItem);
	IClientUserItem* getUserItem(bool bAssert = true);
public:
	word GetTableID();
	word GetChairID();
	byte GetUserStatus();
	SCORE GetUserScore();
	word GetFaceID();
	byte GetGender();
	dword GetUserID();
	dword GetGameID();
	std::string GetHeadHttp();
	//�û��ǳ�
	std::string GetNickName();
	//�û���Ϣ
	tagUserInfo * GetUserInfo();
public:
	//ͷ��////////////////////////////////////////////////////////////////////////
	//ͷ���ͼƬ,��Ϣ���˲�����
	cocos2d::Node* pHeadNode;
	void setHeadNode(cocos2d::Node* pHeadNode);
	//������Ϣ
	void requesUserInfo();
	//����ͷ����Ϣ
	void requestHeadInfo();
	//���������Ϣ
	virtual void on_GP_UserIndividual(void* data, int size);
	//���µ�����Ϣ
	virtual void onUserInfoSingle(void* data, int size);
	//////////////////////////////////////////////////////////////////////////
public:
	virtual void PlayerEnter(){};
	virtual void PlayerLeave(){};
	virtual void upPlayerInfo(){};
	virtual void upPlayerState(){};
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo){}
protected:
	IClientUserItem* m_pUserItem;
	CGPReqPlayerInfoMission* m_kRequestMissionHead;
};