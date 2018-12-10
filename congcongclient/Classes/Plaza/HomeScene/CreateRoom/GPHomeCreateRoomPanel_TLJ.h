#pragma once
#include <cocos2d.h>
#include "common.h"


class GPHomeCreateRoomPanel_TLJ
	:public cocos2d::Node
	,public FvSingleton<GPHomeCreateRoomPanel_TLJ>
{

public:
	const static int GAME_RULE_COMMON = 1;	//��ͨģʽ
	const static int GAME_RULE_PASSION = 2; //����ģʽ
	const static int GAME_RULE_BRAISED = 3; //����Ȧ
public:
	GPHomeCreateRoomPanel_TLJ();
	~GPHomeCreateRoomPanel_TLJ();

public:
	bool init(); 
	void initLayout();
	void initButton();
	void initData();
	void resetGameData();

public:
	void updateRatioPanel();

public:
	void show();
	void hide();
public:
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

	void Button_AllowStrangerJoin(cocos2d::Ref*, WidgetUserInfo*);
	void Button_GameRules(cocos2d::Ref*, WidgetUserInfo*);

	void Button_ChangeGameScore(cocos2d::Ref*, WidgetUserInfo*);
	void updateScore(byte cbIndex,dword wScore);
private:
	int								m_GameRuleIndex;			//��Ϸ����
	bool							m_bAllowedStrangerJoin;		//�Ƿ�����İ���˼���
	dword                           dwBaseScore;				//��ע
	dword                           dwEnterMatchNum;			//�볡����
	dword                           dwOutMatchNum;				//�볡����
};