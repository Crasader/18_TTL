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

enum Record_Touch_State {
	RTS_Null,
	RTS_Touched,
	RTS_End,
};

//����״̬
enum Record_Inqure_State {
	RIT_Null,
	RIT_Up = -1,
	RIT_Down = 1,
};

#pragma pack(1)
struct GameScoreInfo {
	dword dwKindID;
	dword wServerID;
	dword dwTableID;
	dword dwStartTime;
	dword dwBaseScore;
	dword dwRulesBytes;
	word wGameType;
	dword dwMinDrawID;
	dword dwMaxDrawID;
	word wDrawCount;
	std::vector<long long> vctScore;
	std::vector<dword> vctUserID;
	std::vector<std::string>vctHeadHttp;
	std::vector<std::string> vctNickName;
};
#pragma pack()

class GPHomeRecordPanel
	//: public cocos2d::Node
	: public cocos2d::Layer
	, public CGGameRecordSink
	, public FvSingleton<GPHomeRecordPanel>
{
public:
	GPHomeRecordPanel();
	~GPHomeRecordPanel();
public:
	void update(float fpass) override;
	bool init();
	void show();
	void hide();
	std::string timeToString(systemtime kSystem);
	std::string scoreToString(int iScore);
	void Button_Cancel(cocos2d::Ref*, WidgetUserInfo *);
	void initView();

#pragma region �ط����ݻص�

public:

	void sendRecordToTalList(Record_Inqure_State nUpOrDown=RIT_Null);

	void onGPBackGameRecordListEx(tagGameRecordListEx* pNetInfo);
	void onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo);
	void onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo);
	void onGPBackGameChildRecord(tagPrivateRandRecordChild* pNetInfo);
	void Button_StartGameRecord(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ShowTotal(cocos2d::Ref*, WidgetUserInfo*);

#pragma endregion �ط����ݻص�

//TouchEvent////////////////////////////////////////////////////////////////////////
	
	void initTouchEvent();
	bool onTouchBegan(Touch* touch, Event* pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	int isNeedUpdateView();
	//�Ƿ�����һ�ѵ�����
	bool checkNextScoreInfo();

private:
	//////////////////////////////////////////////////////////////////////////

	cocos2d::Vec2 _touch_begin;
	cocos2d::Vec2 _touch_end;
	//�Ƿ��Ǽ���״̬
	bool _bActive;
	//�Ƿ���Ҫ������ʾ, ����update����
	Record_Inqure_State _inqure_state_update;
	//�Ƿ���Ҫ������ʾ, ���ڱ������״̬
	Record_Inqure_State _inqure_state_view;
	GameScoreInfo* _cur_score_info;
	Record_Touch_State _touch_state;
	float _bg_offset;
	//////////////////////////////////////////////////////////////////////////

	CGPGameRecordMission m_GameRecordMission;
	tagPrivateRandTotalRecordList m_RecordList;
	tagPrivateRandTotalRecord m_kPrivateRandTotalRecord;
	std::map<dword, GameScoreInfo> _mpGameScores;
	std::list<dword> _lstScoreInfoIndex;
};