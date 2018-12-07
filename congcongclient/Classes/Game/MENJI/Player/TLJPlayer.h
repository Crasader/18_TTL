#pragma once
#include "../TLJHeaders.h"
#include "../CMD_TLJ.h"

using namespace TLJ;
struct HandCard
{
	BYTE cbCardData[TLJ_MAX_COUNT];
	BYTE cbCardCount;
	bool bHasLook;
};

struct TLJPlayer_Record {
	WORD chairID;
	DWORD userID;
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
	WORD						m_VisioChairID;
private:
	HandCard					m_HandCard;			//����
	WORD						m_wState;			//��Ϸ״̬

	bool						m_bAuto;			//�Զ���ע״̬
	WORD						m_wPlayCount;		//��Ϸ����
public:
	TLJPlayer();
	~TLJPlayer();
public:
	void init();
	void reset();

#pragma region �����Ϣ
public:
	WORD GetChairID();
	DWORD GetUserID();
	std::string GetNickName();
	std::string GetHeadHttp();

	virtual void upPlayerInfo();
	void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
	void upPlayerInfoAfterOperate();
	WORD getVisioChairID();

	bool isValid();
	void setValid(bool valid); 

	void setVariable();

	bool isSeatInLeft();

	int getRoomScore();

	void setGameScore(int gameScore);
	int getGameScore();
	//��Ϸ״̬
	WORD getState();
	void setState(WORD wState);
	//�Զ�Ͷע״̬
	void setAutoState(bool bState);
	bool getAutoState();
	//��Ϸ����
	void setPlayCount(WORD wCount);
	WORD getPlayCount();
#pragma endregion 

#pragma region �������
public:
	void setHandCard(BYTE cbCardData[TLJ_MAX_COUNT], bool haslook);
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
	void PlayerBetting(BYTE cbScoreLevel,WORD wTimes);
	//�������
	void PlayerGiveUp(WORD wState);
	//��ұ���
	void PlayerCompare();
#pragma endregion 
};

