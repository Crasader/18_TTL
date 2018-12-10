#pragma once

#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"
#include "Game/Game/UserInfo.h"
#include "Game/Game/GameBase.h"
 
#include "Game/Script/TimeNode.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "Platform/PFKernel/CGPGameRecord.h"
#include "Plaza/GameManager/GPGameManager.h"

class GPHomeRecordPanel
	: public cocos2d::Node
	, public CGGameRecordSink
	, public FvSingleton<GPHomeRecordPanel>
{
public:
	GPHomeRecordPanel();
	~GPHomeRecordPanel();
public:
	bool init();
	void show();
	void hide();
	std::string timeToString(systemtime kSystem);
	std::string scoreToString(int iScore);
	void Button_Cancel(cocos2d::Ref*, WidgetUserInfo *);

#pragma region 回放数据回调
public:
	void onGPBackGameRecordListEx(tagGameRecordListEx* pNetInfo);
	void onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo);
	void onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo);
	void onGPBackGameChildRecord(tagPrivateRandRecordChild* pNetInfo);
	void Button_StartGameRecord(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ShowTotal(cocos2d::Ref*, WidgetUserInfo*);

#pragma endregion 回放数据回调

private:
	CGPGameRecordMission m_GameRecordMission;
	tagPrivateRandTotalRecordList m_RecordList;
	tagPrivateRandTotalRecord m_kPrivateRandTotalRecord;
};