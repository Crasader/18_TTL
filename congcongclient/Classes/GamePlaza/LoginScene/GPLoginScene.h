#ifndef __GPLOGIN_SCENE_H_
#define __GPLOGIN_SCENE_H_
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/MissionWeiXin.h"
#include "Platform/PFKernel/CGPLoginMission.h"
#include "ui/UIButton.h"
#include "Game/GameLib.h"
#include "SimpleAudioEngine.h"
#include "../GPSceneManager.h"
#include "GameLib/Platform/PFKernel/CGPopularizeMission.h"

class GPLoginScene
	: public cocos2d::Node
	, public IGPLoginMissionSink
	, public IWeiXinMissionSink
	, public CGPopularizeSink
	, public FvSingleton<GPLoginScene>
{
public:
	GPLoginScene();
	~GPLoginScene();
public:
	bool init();
	void EnterScene();
	void RegisterAccount();
public:
	virtual void onGPLoginSuccess();
	virtual void onGPLoginComplete();
	virtual void onGPLoginFailure(unsigned int iErrorCode, const char* szDescription);
	virtual void onGPError(int err);
	virtual void onGPPopularizeHttpRes(const CMD_GP_PopularizeHttpRes& pNetInfo);
public:
	virtual void onWxLoginSuccess(WxUserInfo kWxUserInfo);
	virtual void onWxLoginFail(std::string kError);
public:
	std::string GetWxLoginWin32();
	void Button_WeiXinLogon(cocos2d::Ref*, WidgetUserInfo*);
	void Button_UserXieYiCheak(cocos2d::Ref*, WidgetUserInfo*);
	void Button_UserXieYi(cocos2d::Ref*, WidgetUserInfo*);

private:
	CGPLoginMission m_kLoginMission;
	std::string m_kPssword;
	WxUserInfo m_kWeiXinUserInfo;
	CGPopularizeMission m_kPopularizeMission;
	bool isChecked;
};
#endif // !__GPLOGIN_SCENE_H_
