#pragma once

#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
 
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "DEFINE.h"

class GPHomeCreateRoomPanel_SRLF
	: public cocos2d::Node
	, public FvSingleton<GPHomeCreateRoomPanel_SRLF>
{
public:
	const static dword GAME_RULE_YXMSRJOIN = 1; //����İ���˼���
	const static dword GAME_RULE_14CARDS = 2; //ʮ����
	const static dword GAME_RULE_8CARDS = 3; //����
	const static dword GAME_RULE_XZDD = 4; //Ѫս����
	const static dword GAME_RULE_TDH = 5; //�Ƶ���
	const static dword GAME_RULE_DGH_DP = 6; // ��ܻ������ڣ�
	const static dword GAME_RULE_DGH_ZM = 7; // ��ܻ���������
	const static dword GAME_RULE_GFXY = 8;//�η�����
	const static dword GAME_RULE_JP = 9;//��Ʈ
	const static dword GAME_RULE_3_REN = 10;//3��
	const static dword GAME_RULE_2_REN = 11;//2��
	const static dword GAME_RULE_2_FAN = 12;//2��
	const static dword GAME_RULE_3_FAN = 13;//3��
	const static dword GAME_RULE_4_FAN = 14;//4��
	const static dword GAME_RULE_HAIDI_LAO = 15;//������
	const static dword GAME_RULE_HAIDI_PAO = 16;//������
	const static dword GAME_RULE_ZIMO_JIADI = 17;//�����ӵ�
	const static dword GAME_RULE_ZIMO_JIAFAN = 18;//�����ӷ�
public:
	GPHomeCreateRoomPanel_SRLF();
	~GPHomeCreateRoomPanel_SRLF();
public:
	bool init();
	void initListView();
	void initButton();
	//void initModeStatus();
	//void initRuleStatus();
	//void initGameScore();
	//void initGameCount(); 
	void initData();
public:
	void show();
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	
	//void Button_PlayerRule(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);

	void Button_GameRuleJoinMoShenRen(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//����İ���˼���;
	void Button_GameRuleCardsNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//������		
	void Button_GameRuleHuPaiWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//����
	void Button_GameRuleDianGangHua(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//��ܻ�
	void Button_GameRuleFanShuWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//����Ŀ
	void Button_GameRuleZiMoJiaScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�����ҵ� �����ӷ�
	void Button_GameRuleHaiDiScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//������ �ͺ�����
	void Button_GameRulePlayerNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//����
	void Button_GameRuleFanMaxNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�������ֵ
	void Button_GameRuleDiZhuNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//��ע
	void Button_GameRuleLiChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�볡�趨
	void Button_GameRuleJinChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�����趨

	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Mode(cocos2d::Ref*, WidgetUserInfo*);
	void updateScore(byte cbIndex,dword wScore);



	void Button_PrivatePlayCout(cocos2d::Ref*, WidgetUserInfo*);
	void ButtonGameRule(cocos2d::Ref*, WidgetUserInfo*);
	void Button_PrivatePlayFanbei(cocos2d::Ref*, WidgetUserInfo*);

	void onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);

public:
	void changeGameTypeBtnTexture(std::string target, std::string value);

private:
	int m_CurrentModeIndex;                //֧����ʽ
	int m_CurrentRuleIndex;                //����
	int m_CurrentScore;                    //�׷�
	int m_CurrentGameCount;                //����

	dword  m_GameRuleJoinMoShenRen;		  //����İ���˼���	
	dword  m_GameRuleCardsNum;			  //������		
	dword  m_GameRuleHuPaiWanFa;		  //����
	dword  m_GameRuleDianGangHua;		  //��ܻ�
	dword  m_GameRuleFanShuWanFa;		  //����Ŀ
	dword  m_GameRulePlayerNum;			  //����
	dword  m_GameRuleFanMaxNum;			  //�������ֵ
	dword  m_GameRuleDiZhuNum;			  //��ע
	dword  m_GameRuleLiChangNum;		  //�볡�趨
	dword  m_GameRuleJinChangNum;		  //�����趨
};
