#pragma once
#include "../TLJHeaders.h"
#include "TLJPlayer.h" 


class TLJPlayer_Panel
	: public cocos2d::Node
	, public FvSingleton<TLJPlayer_Panel>
{
public:
	TLJPlayer_Panel();
	~TLJPlayer_Panel();

public:
	bool init();

public:
	//��ʾ���
	void showPlayer(TLJPlayer& player);
	//�������
	void hidePlayer(WORD chairID);
	//��ʾ���ͷ��
	void showPlayerAvatar(TLJPlayer& player);
	//���߱�ʶ
	void showOffline(TLJPlayer& player,bool bShow = false);
	//׼����ʶ
	void showReady(TLJPlayer& player,bool bShow = false);
	//ׯ�ұ�ʶ
	void showBanker(TLJPlayer& player,bool bShow = false);
	//���Ʊ�ʶ
	//void showKanPai(TLJPlayer& player,bool bShow = false);
	//���Ʊ�ʶ
	//void showQiPai(TLJPlayer& player,bool bShow = false);
	//״̬��ʶ
	void showStateIdentification(TLJPlayer& player);
	//����Ͷע��
	void showCurrentMatchScore(TLJPlayer& player,bool bShow = false);
	//�������
	Vec2 getPlayerPosition(TLJPlayer& player);
	//��ұ���
	void PlayerCompare(WORD wVisioChairID);
	//��ұ���
	//void PlayerCompare(WORD wVisioChairID_0,WORD wVisoChairID_1,bool bResult);
	void PlayerCompare(TLJPlayer& player,TLJPlayer& player_2,bool bResult);
	//��ұ���
	void onPlayerTouched(WORD wVisioChairID);
	//���ƽ���
	void PlayerCompareEnd();
	//��ʾ
	void resetPanel();
	//��ȡ��������λ��
	int getTouchPlayerNode(cocos2d::Vec2 kPos);
	//�������
	void PlayerGiveUp(TLJPlayer& player);
	//���ͷ������ ��ʾ����
	void changeMaskVisible(WORD wVisioChairID,bool bVisible);
	//���ֱ䶯��ʾ
	void showAddGold(WORD wVisioChairID,int iGold);
	//��ʱ��
	void showTimes(WORD wVisioChairID,int times);
	//���ؼ�ʱ��
	void hideTimes();
	//�����ʾ�ڵ�
	Node* getSeatNode(WORD wVisioChairID);
private:
	WORD m_wInitiatorChairID;
	WORD m_wDestChairID;
	WORD m_wLoseID;
};
