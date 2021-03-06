#include <cocos2d.h>

#include "CCallMission.h"
#include "Tools/utilityLog.h"
#include "Tools/utilityString.h"

CCallMission::CCallMission(std::string kName,const char* url, int port)
	:CSocketMission(url,port)
	,m_kClassName(kName)
{
}

CCallMission::~CCallMission()
{
	stop();
	clearCall();
}

void CCallMission::clearCall()
{
	for (int i = 0;i<(int)m_kLinkCallFun.size();i++)
	{
		delete m_kLinkCallFun[i];
	}
	m_kLinkCallFun.clear();
}
bool CCallMission::start()
{
	if (isAlive())
	{
		return true;
	}
	return CSocketMission::start();
}
void CCallMission::stop()
{
	if (m_kLinkCallFun.size())
	{
		onEventTCPSocketLink();
	}
	else
	{
		CSocketMission::stop();
	}
}
void CCallMission::onEventTCPSocketLink()
{
	CCLOG("%s onEventTCPSocketLink self", m_kClassName.c_str());

	if (m_kLinkCallFun.size() == 0)
	{
		return;
	}
	CallBase* pCall = (*m_kLinkCallFun.begin());
	pCall->CallFun();
	m_kLinkCallFun.erase(m_kLinkCallFun.begin());
	delete pCall;
}
void CCallMission::onEventTCPSocketShut()
{
	CCLOG("%s onEventTCPSocketShut self", m_kClassName.c_str());

	if (m_kLinkCallFun.size())
	{
		start();
	}
}
void CCallMission::onEventTCPSocketError(int errorCode)
{
	utility::filelog(utility::toString(m_kClassName,":onEventTCPSocketShut errorCode=", errorCode).c_str());

	if (m_kLinkCallFun.size())
	{
		start();
	}
}
bool CCallMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	utility::filelog(utility::toString(m_kClassName,":onEventTCPSocketRead main =", main, ", sub=", sub).c_str());

	for (int i = 0;i<(int)m_kNetCallFun.size();i++)
	{
		NetCall& kNetInfo = m_kNetCallFun[i];
		if (kNetInfo.iSub == sub)
		{
			kNetInfo.pFun(data,dataSize);
			return true;
		}
	}
	CCASSERT(false,"");
	return false;
}