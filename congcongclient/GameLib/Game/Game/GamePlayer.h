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
	//用户昵称
	std::string GetNickName();
	//用户信息
	tagUserInfo * GetUserInfo();
public:
	//头像////////////////////////////////////////////////////////////////////////
	//头像的图片,消息来了才下载
	cocos2d::Node* pHeadNode;
	void setHeadNode(cocos2d::Node* pHeadNode);
	//更新信息
	void requesUserInfo();
	//更新头像信息
	void requestHeadInfo();
	//更新玩家信息
	virtual void on_GP_UserIndividual(void* data, int size);
	//更新单条信息
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