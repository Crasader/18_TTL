#include "CGPReqPlayerInfoMission.h"
#include "Game/Script/utility.h"

CGPReqPlayerInfoMission::CGPReqPlayerInfoMission(const char* url, int port)
{
	mIGPReqPlayerInfoSink = nullptr;
	_CallMission = new CCallMission("CGPReqPlayerInfoMission", url, port);
	
	_CallMission->addNetCall(CC_CALLBACK_2(CGPReqPlayerInfoMission::ON_GP_USER_INDIVIDUAL,this), SUB_GP_USER_INDIVIDUAL);
	_CallMission->addNetCall(CC_CALLBACK_2(CGPReqPlayerInfoMission::onUserInfoSingle, this), SUB_GP_QUERY_INDIVIDUAL_SINGLE_RET);
}

CGPReqPlayerInfoMission::~CGPReqPlayerInfoMission()
{
	if (_CallMission) {
		delete _CallMission;
		_CallMission = nullptr;
	}
}

// 设置回调接口
void CGPReqPlayerInfoMission::setMissionSink(ICGPReqPlayerInfoSink* pReqPlayerInfoSink)
{
	mIGPReqPlayerInfoSink = pReqPlayerInfoSink;
}
void CGPReqPlayerInfoMission::QUERY_GP_USER_INDIVIDUAL(int iAccountID)
{
	utility::log(utility::toString("CGPReqPlayerInfoMission::query, id=", iAccountID).c_str());
	if (_CallMission) {
		_CallMission->addLinkCallFun(CC_CALLBACK_0(CGPReqPlayerInfoMission::CB_GP_USER_INDIVIDUAL, this, iAccountID));
		_CallMission->start();
	}
}
void CGPReqPlayerInfoMission::CB_GP_USER_INDIVIDUAL(int iAccountID)
{
	CCLOG("CGPReqPlayerInfoMission::CB_queryInfo\n");
	if (!_CallMission) {
		return;
	}
	CMD_GP_QueryIndividual QueryIndividual;
	zeromemory(&QueryIndividual,sizeof(QueryIndividual));
	QueryIndividual.dwUserID = iAccountID;
	_CallMission->send(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INDIVIDUAL, &QueryIndividual, sizeof(QueryIndividual));
}
void CGPReqPlayerInfoMission::ON_GP_USER_INDIVIDUAL(void* data, int dataSize)
{
	CCLOG("CGPReqPlayerInfoMission::Net_queryInfo\n");
	if (mIGPReqPlayerInfoSink) {
		mIGPReqPlayerInfoSink->on_GP_UserIndividual(data, dataSize);
	}
	_CallMission->stop();
}
void CGPReqPlayerInfoMission::requreUserInfoSingle(byte infoType)
{
	CCLOG("CGPReqPlayerInfoMission::requreUserInfoSingle %d", (int)infoType);
	if (!_CallMission) {
		return;
	}
	tagUserInfoSingle singleUserInfo;
	singleUserInfo.bInfoType = infoType;
	_CallMission->send(MDM_GP_USER_SERVICE, SUB_GP_QUERY_INDIVIDUAL_SINGLE, &singleUserInfo, sizeof(singleUserInfo));
	_CallMission->start();
}
void CGPReqPlayerInfoMission::onUserInfoSingle(void* data, int size)
{
	if(mIGPReqPlayerInfoSink)
		mIGPReqPlayerInfoSink->onUserInfoSingle(data, size);
	_CallMission->stop();
}
