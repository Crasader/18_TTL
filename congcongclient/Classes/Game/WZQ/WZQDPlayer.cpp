#include "WZQDPlayer.h"
#include "WZQDSoundFun.h"
#include "WZQDGameScence.h"
#include IMAGE_DOWN

WZQDPlayer::WZQDPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)//���ͷ��
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
	,m_bGameRecord(false)
	,m_pHandOutCard(NULL)

{
	init();
}

WZQDPlayer::~WZQDPlayer()
{

}

void WZQDPlayer::init()
{
	PlayerLeave();
}
int  WZQDPlayer::getIdex()
{
	return m_iIdex;
}
void WZQDPlayer::defaultState()//��ʼǰ��׼��
{
	setGameRecord(false);
	m_pHandOutCard = NULL;
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	upPlayerState();
	m_kOutCardList.clear();
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);
}
void WZQDPlayer::setGameRecord(bool bRecord)//������Ҽ�¼
{
	m_bGameRecord = bRecord;
}

cocos2d::Node* WZQDPlayer::getPlayerNode()//�����ҽڵ�
{
	return m_pSeatNode;
}
void WZQDPlayer::startGame()//��ʼ��Ϸ 
{
	WidgetFun::setVisible(m_pSeatNode,"Ready_Icon",false);
}

void WZQDPlayer::startGameNoTime()//��ʼ׼��ʱ�����
{
	WidgetFun::setVisible(m_pSeatNode,"Ready_Icon",false);
}

void WZQDPlayer::EndGame()//������Ϸ
{
	if (m_pUserItem == NULL )
	{
		return;
	}
}

void WZQDPlayer::PlayerEnter()//���׼��
{
	WidgetFun::setVisible(m_pSeatNode,"Name",true);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",true);

	//if (WidgetFun::getChildWidget(m_pSeatNode,"ButtonPlayerHeadClick"))
	//{
	//	WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","UserID",utility::toString((int)GetUserID()));
	//}
}

void WZQDPlayer::PlayerLeave()//����뿪
{
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setVisible(m_pSeatNode,"Ready_Icon",false);
	WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","");
}

void WZQDPlayer::upPlayerInfo()//�ϴ������Ϣ
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	if (WZQDGameScence::Instance().HaveOptionRule(WZQDGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1) == false
		&&WZQDGameScence::Instance().HaveOptionRule(WZQDGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO) == false)
	{
		WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
	}
	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
}
void WZQDPlayer::setZhuang()//����ׯ
{
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}
void WZQDPlayer::showAddGold(int iGold)//��ʾ���
{
	if (iGold>0)
	{
		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtWin",utility::toString("+",iGold));
		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtWin","Start");
	}
	else if(iGold<0)
	{
		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtLose",utility::toString(iGold));
		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtLose","Start");
	}
}
void WZQDPlayer::showEffect(std::string kKey)//��ʾЧ��
{
	return;
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}
void WZQDPlayer::showStatusImagic(std::string kKey)//��ʾ׼��Ԫ��
{
	return;
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}

void WZQDPlayer::upPlayerState()//���׼��ͼƬ
{

	WidgetFun::setVisible(m_pSeatNode,"Ready_Icon",GetUserStatus() == US_READY);
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}

void WZQDPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)//��ʾ�Ի�
{
	return;
	std::string kStrNet = pNetInfo->strString;
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkFile"))
		{
			return;
		}
		WidgetFun::runWidgetAction(m_pSeatNode,"TalkFile","Start");
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode"))
		{
			return;
		}
		WidgetFun::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",
			kStrNet,NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(6.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"BiaoQingNode"))
		{
			return;
		}
		//WidgetFun::setImagic(m_pSeatNode,"BiaoQingPic",kStrNet);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"BiaoQingNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(4.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkFile"))
		{
			return;
		}
		std::vector<std::string> kStr = utility::split(kStrNet,":");

		ASSERT(kStr.size()==2);

		WidgetFun::runWidgetAction(m_pSeatNode,"TalkFile","Start");
		SoundFun::Instance().playEffect(kStr[0]);

		WidgetFun::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",
			kStr[1],NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(6.0f,false));

	}
}

void WZQDPlayer::setValid(bool valid)
{
	m_Valid = valid;
	m_pSeatNode->setVisible(valid);
}

