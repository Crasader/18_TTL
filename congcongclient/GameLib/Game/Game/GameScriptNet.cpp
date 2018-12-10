#include "GameScriptNet.h"
#include "GameLib/JniCross/JniFun.h"
#include "Tools/utilityString.h"
#include "Tools/utilityConvert.h"
#include "Game/Script/ScriptData.h"
#include "Game/Game/UserInfo.h"
#include "Game/Game/NoticeMsgBox.h"

using namespace script;

FV_SINGLETON_STORAGE(GameScriptNet);

GameScriptNet::GameScriptNet()
	: m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
	, _publicNotice(ScriptData<std::string>("public_notice_str").Value())
{
	m_kPublicNoticMission.setMissionSink(this);
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(GameScriptNet::LogonSucess));
}
GameScriptNet::~GameScriptNet()
{

}
void GameScriptNet::LogonSucess()
{
	m_kPublicNoticMission.PublicNotice(_publicNotice);
}
void GameScriptNet::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
	if (kKey == "GameScriptNotic" && iError == 0)
	{
		std::string kNetStr = utility::a_u8(szDescription);
		std::vector<std::string> kKeyValue = utility::split(kNetStr,"&");
		for (int i = 0;i<(int)kKeyValue.size();i++)
		{
			std::vector<std::string> kTempValue = utility::split(kKeyValue[i],"=");
			if (kTempValue.size() == 2)
			{
				ScriptDataManager::setValue(kTempValue[0],kTempValue[1]);
			}
		}
		OnGetConfig();
	}
}
void GameScriptNet::OnGetConfig()
{
	std::string kClientVersion = script::getStr("ClientVersion",false);
	std::string kServerVersion = script::getStr("ServerVersion",false);
	if (kServerVersion != "" && kClientVersion != kServerVersion)
	{
		std::string kDownUrl = script::getStr("DownUrl",false);
		NoticeMsgBox::Instance().setDownLoadURL(kDownUrl.c_str());
		NoticeMsgBox::Instance().show();
		//JniFun::showWebView(kDownUrl.c_str());
	}
}