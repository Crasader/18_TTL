#pragma once
#include "CMD_TLJ.h"
#include "TLJHeaders.h"
#include "TLJPlayer.h"

class TLJAnimation
	:public cocos2d::Node
	,public FvSingleton<TLJAnimation>
{
public:
	TLJAnimation();
	~TLJAnimation();
public:
	bool init();
	//������ע����
	void runBettingAnimation(Node& pChip);
	//�������ƶ���
	void runGiveUpAnimation(Node& pCardNode);
	//���ƶ��� �ص�
	void giveUpAniCallBack(Node& pCard);
	//���ƶ���	������ҽ���
	void runCompareAnimation(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//���ƶ���	������Ч
	void runCompareAnimation_2(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//���ƶ���  ��ӮЧ��
	void runCompareAnimation_3(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//���ƶ���  �ص�����
	void CompareAnimationCallBack();
	//�������ͷ��
	void showComparePlayerAvatar(TLJPlayer& player,std::string kValue);
	//���ֶ���
	void runGameStartAnimation(TLJPlayer& player);
	//��������
	void runGameEndAnimation(TLJPlayer& player);

	void GameEndAniCallBack();

	void hide();
	void show();
};

