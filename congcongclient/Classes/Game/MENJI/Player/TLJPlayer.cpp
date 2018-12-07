#include "TLJPlayer.h"
#include "TLJPlayer_Card.h"
#include "TLJPlayer_Panel.h"
#include "../TLJGameScence.h"
#include "../ChipContainer/TLJChipContainer.h"
#include "../SoundFun/TLJSoundFun.h"
#include "GameLib/Game/Script/SoundFun.h"

TLJPlayer::TLJPlayer()
	:GamePlayer(NULL)
	,m_InRecord(false)
	,m_Valid(false)
	,m_wState(0)
	,m_bAuto(false)
	,m_GameScore(0)
{
	init();
}

TLJPlayer::~TLJPlayer()
{
}


void TLJPlayer::init()
{
	memset(&m_HandCard,0,sizeof(m_HandCard));
	m_VisioChairID = 0;
	m_wPlayCount = 0;
}

void TLJPlayer::reset()
{
	memset(&m_HandCard,0,sizeof(m_HandCard));
	m_wState = PLAYER_STATE_NULL;
	m_GameScore = 0;
}

WORD TLJPlayer::GetChairID()
{
	return GamePlayer::GetChairID();
}

void TLJPlayer::upPlayerInfo()
{
	if (isValid())
	{
		TLJPlayer_Panel::Instance().showPlayer(*this);
		TLJPlayer_Card::Instance().showCard(*this,false);
	}
}

void TLJPlayer::upPlayerInfoAfterOperate()
{
	if (isValid()) 
	{
		TLJPlayer_Panel::Instance().showPlayer(*this);
		TLJPlayer_Card::Instance().showCard(*this, true);
	}
}

void TLJPlayer::upPlayerState()
{
	upPlayerInfo();
}

bool TLJPlayer::isValid()
{
	return m_Valid;
}

void TLJPlayer::setValid(bool valid)
{
	m_Valid = valid; 
}

WORD TLJPlayer::getVisioChairID()
{
	return m_VisioChairID;
}

int TLJPlayer::getRoomScore()
{
	return TLJGameScence::Instance().getRoomScore(*this);
}

void TLJPlayer::setVariable()
{

}

bool TLJPlayer::isSeatInLeft()
{
	if(m_VisioChairID<3){
		return true;
	}
	return false;
}

void TLJPlayer::setGameScore(int gameScore)
{
	m_GameScore = gameScore;
}
int TLJPlayer::getGameScore()
{
	return m_GameScore;
}

WORD TLJPlayer::getState()
{
	return m_wState;
}

void TLJPlayer::setState(WORD wState)
{
	m_wState = wState;
}

void TLJPlayer::setAutoState(bool bState)
{
	m_bAuto = bState;
}

bool TLJPlayer::getAutoState()
{
	return m_bAuto;
}
//��Ϸ����
void TLJPlayer::setPlayCount(WORD wCount)
{
	m_wPlayCount = wCount;
}
WORD TLJPlayer::getPlayCount()
{
	return m_wPlayCount;
}
void TLJPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)//��ʾ�Ի�
{
	//m_pSeatNode;
	WORD wVisioChairID = getVisioChairID();
	m_pSeatNode = TLJPlayer_Panel::Instance().getSeatNode(wVisioChairID);
	if(!(isSeatInLeft())){
		Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"public_9sprite");
		pNode->setRotation3D(Vec3(180,0,180));
		Node* pTalkFileNode = WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_TalkFile");
		Vec2 rightPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pTalkFileNode,"Pos_2"));
		pTalkFileNode->setPosition(rightPos);

		Node* pTalkTxtNode = WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_TalkTxtNode");
		Vec2 rightPos2 = utility::parsePoint(WidgetFun::getWidgetUserInfo(pTalkTxtNode,"Pos_2"));
		pTalkTxtNode->setPosition(rightPos2);

		Node* pBiaoQingNode = WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_BiaoQingNode");
		Vec2 rightPos3 = utility::parsePoint(WidgetFun::getWidgetUserInfo(pBiaoQingNode,"Pos_2"));
		pBiaoQingNode->setPosition(rightPos3);
	}
	
	std::string kStrNet = pNetInfo->strString;
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_TalkFile"))return;
		WidgetFun::runWidgetAction(m_pSeatNode,"TLJPlayer_TalkFile","Start");
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_TalkTxtNode"))return;
		utility::runPaoMaDeng(m_pSeatNode,"TLJPlayer_TalkTxt","TLJPlayer_LayoutNode",kStrNet,NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_BiaoQingNode"))return;
		WidgetFun::setImagic(m_pSeatNode,"TLJPlayer_BiaoQingPic",kStrNet);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_BiaoQingNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(4.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_TalkFile"))return;
		std::vector<std::string> kStr = utility::split(kStrNet,":");
		ASSERT(kStr.size()==2);
		WidgetFun::runWidgetAction(m_pSeatNode,"TLJPlayer_TalkFile","Start");
		TLJSoundFun::playEffect(kStr[0]);
		SoundFun::Instance().playEffect(kStr[0]);
		utility::runPaoMaDeng(m_pSeatNode,"TLJPlayer_TalkTxt","TLJPlayer_LayoutNode",
		kStr[1],NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TLJPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
	}
}

DWORD TLJPlayer::GetUserID()
{
	return GamePlayer::GetUserID();
}

std::string TLJPlayer::GetNickName()
{
	return GamePlayer::GetNickName();
}

std::string TLJPlayer::GetHeadHttp()
{
	return GamePlayer::GetHeadHttp();
}

void TLJPlayer::setHandCard(BYTE cbCardData[TLJ_MAX_COUNT], bool haslook)
{
	memset(&m_HandCard,0,sizeof(m_HandCard));
	if(haslook){
		for(BYTE index = 0; index < TLJ_MAX_COUNT; index++){
			m_HandCard.cbCardData[index] = cbCardData[index];
		}
	}
	m_HandCard.cbCardCount = TLJ_MAX_COUNT;
	m_HandCard.bHasLook = haslook;
}
HandCard TLJPlayer::getHandCard(){
	return m_HandCard;
}

//���뷿��
void TLJPlayer::PlayerEnter()
{
	m_InRecord = false;
	m_Valid = true;
	m_wPlayCount = 0;
	m_VisioChairID = TLJGameScence::Instance().getVisioChairID(*this);
	m_wState = PLAYER_STATE_NULL;
	upPlayerInfo();
	/*if (GetUserID() == UserInfo::Instance().getUserID()) 
	{
		TLJGameScence::Instance().sendReady();
	}*/
}
//�뿪����
void TLJPlayer::PlayerLeave()
{
	TLJPlayer_Panel::Instance().hidePlayer(m_VisioChairID);
	TLJPlayer_Card::Instance().hideCard(m_VisioChairID);
	m_Valid = false;
	m_VisioChairID = INVALID_CHAIR;
	m_pSeatNode = NULL;
}
//��ұ䶯
void TLJPlayer::showAddGold(int iGold)
{
	TLJPlayer_Panel::Instance().showAddGold(m_VisioChairID,iGold);
}
//��ע����ע
void TLJPlayer::PlayerBetting(BYTE cbScoreLevel,WORD wTimes)
{
	wTimes = getState()==PLAYER_STATE_AN?1:wTimes;

	TLJChipContainer::Instance().addChip(*this,cbScoreLevel,wTimes);
	DWORD dwBaseScore = TLJGameScence::Instance().getBaseScore();

	//int iScore = dwBaseScore*cbScoreLevel*wTimes;
	//showAddGold(-iScore);
}
//��ʱ��
void TLJPlayer::showTimes(int times)
{
	TLJPlayer_Panel::Instance().hideTimes();
	TLJPlayer_Panel::Instance().showTimes(m_VisioChairID,times);
}

//�������
void TLJPlayer::PlayerGiveUp(WORD wState)
{
	setState(wState);
	//memset(&m_HandCard,0,sizeof(m_HandCard));
	for (int index = 0; index < m_HandCard.cbCardCount; index ++)
	{
		if (m_HandCard.cbCardData[index] == 0)
		{
			m_HandCard.cbCardData[index] = 99;
		}
	}
	TLJPlayer_Card::Instance().PlayerGiveUp(*this);
	TLJPlayer_Panel::Instance().PlayerGiveUp(*this);
}
//��ұ���
void TLJPlayer::PlayerCompare()
{
	WORD wVisioChairId = getVisioChairID();
	TLJPlayer_Panel::Instance().PlayerCompare(wVisioChairId);
}
