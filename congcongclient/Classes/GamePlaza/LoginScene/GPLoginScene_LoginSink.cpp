#include "GPLoginScene.h"

void GPLoginScene::onGPLoginSuccess()
{
	cocos2d::log("kuili:-----GPLogonScence::onGPLoginSuccess call---------");
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
	cocos2d::UserDefault::getInstance()->setStringForKey("Accounts", pGlobalUserData->szAccounts);
	cocos2d::UserDefault::getInstance()->setStringForKey("Password", m_kPssword);
	GPSceneManager::Instance().InHomeScene();
	UserInfo::Instance().modeHeadHttp(UserInfo::Instance().getHeadHttp());
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
	NoticeMsg::Instance().ShowTopMsg(utility::getScriptString("NetError"));
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
	for (int i=startIndex;i<=endIndex;i++) 
	{
		auto pNode = cocos2d::Sprite::create();
		ImagicDownManager::Instance().addDown(pNode,utility::toString(vStr[0],i,".png"),utility::toString("GuangGao_",i),true);
	}
}
