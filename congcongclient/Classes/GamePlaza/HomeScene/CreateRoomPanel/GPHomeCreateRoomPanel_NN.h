#pragma once

#include "CommonHeader/CommonHeader.h"
#include "Game/FV/FvMask.h"

class GPHomeCreateRoomPanel_NN
	:public cocos2d::Node
	,public FvSingleton<GPHomeCreateRoomPanel_NN>
{
public:

	GPHomeCreateRoomPanel_NN();
	~GPHomeCreateRoomPanel_NN();

public:
	bool init(); 
	void initLayout();
	void initButton();
	void initData();
	void resetGameData();

public:
	//void updateRatioPanel();

public:
	void show();
	void hide();

public:

	//////////////////////////////////////////////////////////////////////////
	void Button_Confirm(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

	void Button_AllowedStranger(cocos2d::Ref*, WidgetUserInfo*);

	void Button_SixRound(cocos2d::Ref*, WidgetUserInfo*);
	void Button_EightRound(cocos2d::Ref*, WidgetUserInfo*);

	//ѡ�������ť
	void Button_RoundBox1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_RoundBox2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_RoundBox3(cocos2d::Ref*, WidgetUserInfo*);

	//��ע����
	void Button_TuiZhu1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TuiZhu2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TuiZhu3(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TuiZhu4(cocos2d::Ref*, WidgetUserInfo*);

	void Button_CanTuiZhu(cocos2d::Ref*, WidgetUserInfo*);

	//��������
	void Button_ShunZi(cocos2d::Ref*, WidgetUserInfo*);
	void Button_TongHua(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WuHua(cocos2d::Ref*, WidgetUserInfo*);
	void Button_HuLu(cocos2d::Ref*, WidgetUserInfo*);
	void Button_ZhaDan(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WuXiao(cocos2d::Ref*, WidgetUserInfo*);

	//��������
	void Button_MutipleRule1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_MutipleRule2(cocos2d::Ref*, WidgetUserInfo*);

	//�׷ֱ���
	void Button_DiFen1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_DiFen2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_DiFen3(cocos2d::Ref*, WidgetUserInfo*);
	void Button_DiFen4(cocos2d::Ref*, WidgetUserInfo*);

	//��ׯ����
	void Button_QiangZhuang1(cocos2d::Ref*, WidgetUserInfo*);
	void Button_QiangZhuang2(cocos2d::Ref*, WidgetUserInfo*);
	void Button_QiangZhuang3(cocos2d::Ref*, WidgetUserInfo*);
	void Button_QiangZhuang4(cocos2d::Ref*, WidgetUserInfo*);

	//��������
	//������ׯ
	void Button_RoomType_MPQZ(cocos2d::Ref*, WidgetUserInfo*);
	//////////////////////////////////////////////////////////////////////////

	void updateScore(BYTE cbIndex,dword wScore);

private:

	int								_nRoundIndex;	//����ѡ�����
	dword							_dwEnterMatchNum;//�볡����
	dword							_dwOutMatchNum;//�볡����
	int								_nGameType;//��Ϸ����
	dword							_dwBaseScore;//��ע
	int								_nQiangZhuangBeiShu;//��ׯ����
	//��Ϸ����////////////////////////////////////////////////////////////////////////

	bool								_bCanTuiZhu;// �Ƿ���ע
	dword							_nGameRuleIndex;//��Ϸ����
	bool								_bSixRound;//���˳����߰��˳�
	int								_nTuiZhuIndex;//�м���ע�������
	int								_nRatioRuleIndex;//ţţ�Ĺ������0Ϊ1,1Ϊ2
	bool								_bShunZi;//˳��ţ
	bool								_bTongHua;//ͬ��ţ
	bool								_bWuHua;//�廨ţ
	bool								_bHuLu;//��«ţ
	bool								_bZhaDan;//ը��ţ
	bool								_bWuXiao;//��Сţ

	bool								_bAllowedStrangerJoin;//�Ƿ�����İ���˼���
};