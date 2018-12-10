#pragma once
#include "TLJHeaders.h"
#include "CMD_TLJ.h"

using namespace TLJ;

struct HandCard
{
	byte cbCardData[TLJ_MAX_COUNT];
	byte cbCardCount;
	bool bHasLook;
};

struct TLJPlayer_Record {
	word chairID;
	dword userID;
	std::string nickName;
	std::string headHttp;
};

class TLJPlayer
	:public GamePlayer
{
private:
	bool						m_InRecord;
	bool						m_Valid;
	int							m_GameScore;
public:
	int							m_index;
	cocos2d::Node*				m_pSeatNode;
	word						m_VisioChairID;
private:
	HandCard					m_HandCard;			//����
	word						m_wState;			//��Ϸ״̬

	bool						m_bAuto;			//�Զ���ע״̬
	word						m_wPlayCount;		//��Ϸ����
public:
	TLJPlayer();
	~TLJPlayer();
public:
	void init();
	void reset();

#pragma region �����Ϣ
public:
	word GetChairID();
	dword GetUserID();
	std::string GetNickName();
	std::string GetHeadHttp();

	virtual void upPlayerInfo();
	void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
	void upPlayerInfoAfterOperate();
	word getVisioChairID();

	bool isValid();
	void setValid(bool valid); 

	void setVariable();

	bool isSeatInLeft();

	int getRoomScore();

	void setGameScore(int gameScore);
	int getGameScore();
	//��Ϸ״̬
	word getState();
	void setState(word wState);
	//�Զ�Ͷע״̬
	void setAutoState(bool bState);
	bool getAutoState();
	//��Ϸ����
	void setPlayCount(word wCount);
	word getPlayCount();
#pragma endregion 

#pragma region �������
public:
	void setHandCard(byte cbCardData[TLJ_MAX_COUNT], bool haslook);
	HandCard getHandCard();
#pragma endregion 

#pragma region ��Ҳ���
public:
	//���뷿��
	virtual void PlayerEnter();
	//�뿪����
	virtual void PlayerLeave();
	//��ұ䶯
	void showAddGold(int iGold);
	//��ʱ��
	void showTimes(int times);
	//��ע����ע
	void PlayerBetting(byte cbScoreLevel,word wTimes);
	//�������
	void PlayerGiveUp(word wState);
	//��ұ���
	void PlayerCompare();
#pragma endregion 
};

