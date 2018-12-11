#pragma once
#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"
#include "Game/FV/FvMask.h"
 
#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "DEFINE.h"

class GPHomeCreateRoomPanel_XZDD
	:public cocos2d::Node
	, public FvSingleton<GPHomeCreateRoomPanel_XZDD>
{
public:
	//const static dword GAME_RULE_YXMSRJOIN = 1;			//����İ���˼���
	const static dword GAME_RULE_NO_WAIT_ = 1;
	const static dword GAME_RULE_XZ_ = 2;
	const static dword GAME_RULE_XL_ = 3;
	const static dword GAME_RULE_HUAN3_ =4;	//������
	const static dword GAME_RULE_DIN_QUE_=5;	//��ȱ
	const static dword GAME_RULE_DI_FEN0_=6;	//2���ⶥ
	const static dword GAME_RULE_DI_FEN1_=7;	//3���ⶥ
	const static dword GAME_RULE_DI_FEN2_ =8;	//4���ⶥ
	const static dword GAME_RULE_G_S_P_=9;	//����ת��
	const static dword GAME_RULE_ZI_MO_JIAFAN_=10;	//�����ӷ�
	const static dword GAME_RULE_D_G_H_ZIMO_=11;//��ܻ�������
	const static dword GAME_RULE_Jiang_dui_=12;//����
	const static dword GAME_RULE_MEN_Q_=13;//����
	const static dword GAME_RULE_T_D_HU_=14;//��غ�
	const static dword GAME_RULE_YI_JIU_=15;//�۾�
	const static dword GAME_RULE_ZHONG_ZHANG_=16;//����
	const static dword GAME_RULE_GUOSHOUHU_=17;//���ֺ�
	const static dword GAME_RULE_GUOSHOUHUDA_=18;//���ֺ���

	const static dword GAME_RULE_ZI_MO_JIADI_ =19;//�����ҵ�
	const static dword GAME_RULE_HAI_DI_LAO_ =20;//������ 
	const static dword GAME_RULE_D_G_H_DIANPAO_=21;//��ܻ�������
public:
	GPHomeCreateRoomPanel_XZDD();
	~GPHomeCreateRoomPanel_XZDD();

public:
	bool init();
	void initListView();
	void initLayout();
	void initButton();
	void show();
	void hide();

	void Button_Close(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�ر�
	void Button_CreateRoom_XZDD(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�ر�
	void Button_GameRuleDiZhuNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//��ע
	void Button_GameRuleLiChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�볡�趨
	void Button_GameRuleJinChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�����趨
	void updateScore(byte cbIndex, dword wScore);
private:
	void initData();//˽����ʾ
	void initGameRuleSheDing();
private:
	//˽��
	void Button_GameRuleSheDing(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//��Ϸ�趨

	void Button_GameRuleFanShuWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//����Ŀ
	void Button_GameRuleZiMoJiaScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);//�����ҵ� �����ӷ�
private:
	int m_CurrentModeIndex;                //֧����ʽ
	dword m_CurrentRuleIndex;                //����
	int m_CurrentScore;                    //�׷�
	int m_CurrentGameCount;                //����
	
	bool  m_GameRuleJoinMoShenRen;		  //����İ���˼���	
	dword  m_GameRuleFanShuWanFa;		  //����Ŀ//�淨ѡ��
	dword  m_GameRuleFanMaxNum;			  //�������ֵ//����
	dword  m_GameRuleDiZhuNum;			  //��ע
	dword  m_GameRuleLiChangNum;		  //�볡�趨
	dword  m_GameRuleJinChangNum;		  //�����趨
	dword  m_GameYouXiSheDing;			  //��Ϸ�趨
	dword  m_GameYouXiDianGangHua;		  //��ܻ�����/����
	byte   m_GameFangJian;				  //˽�˷�Ϊ1/Ԫ������Ϊ2
	dword  m_GameFangJianSeDing;		  //�����趨/�¼�
};
