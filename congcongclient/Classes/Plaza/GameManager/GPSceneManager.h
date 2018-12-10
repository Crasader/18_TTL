#ifndef __GPSCENEMANAGER_H_
#define __GPSCENEMANAGER_H_

#include "common.h"
#include "Game/Game/ScenceManagerBase.h"
#include "Plaza/GameManager/GPGameManager.h"

class GPSceneManager
	: public TimeNode
	, public ScenceManagerBase
	, public FvSingleton<GPSceneManager>
{
	FRIEND_SINGLETON(GPSceneManager)

public:

	cocos2d::Node* _pRootNode;
	void setParent(CCNode* pNode);
	cocos2d::Node* getRootNode();

public:

	virtual void init();
	void addGameNode(cocos2d::Node* pNode);
	void HideAll();

public:
	void InLogonScene();
	void InHomeScene();
	void InHomeExScene(int gameKindID);
	void InNNCalculate(CMD_GF_Private_End_Info* pInfo);
	void InSGCalculate(CMD_GF_Private_End_Info* pInfo);

public:
	virtual void GameBackScence();

	static Node* createCircleAvatar(cocos2d::Node* pRootNode, const std::string& filename, const std::string& stencilPath, const Vec2& position);

protected:
	int m_iScenceTimeID;
	bool m_bInBackground;
};
#endif //__GPSCENEMANAGER_H_