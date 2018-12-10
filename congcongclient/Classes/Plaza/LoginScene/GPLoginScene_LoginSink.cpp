#include "GPLoginScene.h"

#include USERINFO
#include UTILITY_LOG

void GPLoginScene::onGPLoginSuccess()
{
	utility::filelog("GPLogonScence::onGPLoginSuccess");
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
	cocos2d::UserDefault::getInstance()->setStringForKey("Accounts", pGlobalUserData->szAccounts);
	cocos2d::UserDefault::getInstance()->setStringForKey("Password", m_kPssword);
	GPSceneManager::Instance().InHomeScene();
}

void GPLoginScene::onGPLoginComplete()
{
	
}

void GPLoginScene::onGPLoginFailure(unsigned int iErrorCode, const char* szDescription)
{
	//RegisterAccount();
	//return;
	if (iErrorCode == 3 || iErrorCode == 1) {
		RegisterAccount();
	}
	else
	{
		NoticeMsg::Instance().ShowTopMsg((szDescription));
	}
}

void GPLoginScene::onGPError(int err)
{
	NoticeMsg::Instance().ShowTopMsg(script::getStr("NetError"));
}

void GPLoginScene::onGPPopularizeHttpRes(const CMD_GP_PopularizeHttpRes& pNetInfo)
{
	std::string kFileName = cocos2d::FileUtils::getInstance()->getWritablePath();
	for(int i=1;i<=5;i++) 
	{
		remove(utility::toString(kFileName,"GuangGao_",i,".png").c_str());
	}
	auto vStr = utility::split(pNetInfo.szPopularizeHttp,";");
	int startIndex = utility::parseInt(vStr[1]);
	int endIndex =utility::parseInt(vStr[2]);
	CCAssert(false, "called onGPPopularizeHttpRes");
	for (int i=startIndex;i<=endIndex;i++) 
	{
		auto pNode = cocos2d::Sprite::create();
		//ImagicDownManager::Instance().addDown(pNode,utility::toString(vStr[0],i,".png"),utility::toString("GuangGao_",i));
	}
}
