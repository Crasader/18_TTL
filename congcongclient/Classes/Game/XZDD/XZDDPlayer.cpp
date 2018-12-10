#include "XZDDPlayer.h"
#include "XZDDSoundFun.h"
#include "XZDDGameScence.h"
#include IMAGE_DOWN

XZDDPlayer::XZDDPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)//���ͷ��
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
	,m_bGameRecord(false)
	,m_nQueColor(CARD_COLOR_NULL)
	,m_pHandOutCard(NULL)
	,m_bMingPai(false)
	,m_bPlayHSAni(false)
	,m_bBanker(false)
	,m_bHSZInitCards(false)
{
	init();
}

XZDDPlayer::~XZDDPlayer()
{

}

void XZDDPlayer::init()
{
	PlayerLeave();
}

int  XZDDPlayer::getIdex()
{
	return m_iIdex;
}

void XZDDPlayer::defaultState()//��ʼǰ��׼��
{
	setGameRecord(false);
	setChiHuCard(0);
	m_pHandOutCard = NULL;
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	setActOutCard(-1);
	m_bPlayHSAni =false;
	WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);//���ð�ť�ɼ���
	WidgetFun::setVisible(m_pSeatNode, "Offline", false);
	WidgetFun::setVisible(m_pSeatNode, "BigOutNode", false);
	WidgetFun::setVisible(m_pSeatNode, "ReadyState", false);
	WidgetFun::getChildWidget(m_pSeatNode, "CardNode1")->removeAllChildren();//��������ӽڵ�
	WidgetFun::getChildWidget(m_pSeatNode, "CardNode2")->removeAllChildren();
	WidgetFun::getChildWidget(m_pSeatNode, "AnimateNode")->removeAllChildren();

	zeromemory(m_pHuanCards,sizeof(m_pHuanCards));
	zeromemory(m_pSaveHuanPai,sizeof(m_pSaveHuanPai));
	
	m_pHuanCardNodeList.clear();

	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",false);
	showXuanQue(false);
	upPlayerState();
	m_kOutCardList.clear();
	clearChiHuCard();
	setQueColor(CARD_COLOR_NULL);
	setMingPai(false);
	setZhuang(false);
}

void XZDDPlayer::setGameRecord(bool bRecord)//������Ҽ�¼
{
	m_bGameRecord = bRecord;
}

cocos2d::Node* XZDDPlayer::getPlayerNode()//�����ҽڵ�
{
	return m_pSeatNode;
}

void XZDDPlayer::startGame()//��ʼ��Ϸ 
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","Start");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

}

void XZDDPlayer::startGameNoTime()//��ʼ׼��ʱ�����
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

}

void XZDDPlayer::EndGame()//������Ϸ
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","End");
	if (m_pUserItem == NULL )
	{
		return;
	}
}

void XZDDPlayer::PlayerEnter()//���׼��
{
	WidgetFun::setVisible(m_pSeatNode,"Name",true);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",true);

	if (WidgetFun::getChildWidget(m_pSeatNode,"ButtonPlayerHeadClick"))
	{
		WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","UserID",utility::toString((int)GetUserID()));
	}
}

void XZDDPlayer::PlayerLeave()//����뿪
{
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","");
}

void XZDDPlayer::upPlayerInfo()//�ϴ������Ϣ
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());

	if (XZDDGameScence::Instance().HaveOptionRule(XZDDGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1) == false
		&&XZDDGameScence::Instance().HaveOptionRule(XZDDGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO) == false)
	{
		WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
	}


	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
	//WidgetFun::setVisible(m_pSeatNode,"Zhuang",m_bBanker);
}

void XZDDPlayer::setZhuang(bool bZhuang)//����ׯ
{
	m_bBanker = bZhuang;
	if (NULL == m_pSeatNode) return;
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",m_bBanker);
}

void XZDDPlayer::showAddGold(int iGold)//��ʾ���
{
	if (NULL == m_pSeatNode) return;
	if (iGold > 0)
	{
		WidgetFun::setAtlasTxt(m_pSeatNode, "AddGoldTxtWin", utility::toString("+", iGold));
		WidgetFun::runWidgetAction(m_pSeatNode, "AddGoldTxtWin", "Start");
	}
	else if (iGold < 0)
	{
		WidgetFun::setAtlasTxt(m_pSeatNode, "AddGoldTxtLose", utility::toString(iGold));
		WidgetFun::runWidgetAction(m_pSeatNode, "AddGoldTxtLose", "Start");
	}
// 	cocos2d::Node* pnode = WidgetFun::getChildWidget(m_pSeatNode, "Head");
// 	if (iGold>0)
// 	{
// 		cocos2d::Vec2 pos  = utility::parsePoint(WidgetFun::getWidgetUserInfo(m_pSeatNode,"Head","AddGoldTxtWinPos"));
// 		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtWin",utility::toString("+",iGold));
// 		WidgetFun::setPos(pnode,"AddGoldTxtWin",pos);
// 		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtWin","Start");
// 	}
// 	else if(iGold<0)
// 	{
// 		cocos2d::Vec2 pos  = utility::parsePoint(WidgetFun::getWidgetUserInfo(m_pSeatNode,"Head","AddGoldTxtLosePos"));
// 		WidgetFun::setAtlasTxt(m_pSeatNode,"AddGoldTxtLose",utility::toString(iGold));
// 		WidgetFun::setPos(pnode,"AddGoldTxtLose",pos);
// 		WidgetFun::runWidgetAction(m_pSeatNode,"AddGoldTxtLose","Start");
// 	}
}

void XZDDPlayer::showEffect(std::string kKey)//��ʾЧ��
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}

void XZDDPlayer::showStatusImagic(std::string kKey)//��ʾ׼��Ԫ��
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}

void XZDDPlayer::showXuanQue(bool bShow)//ѡȱ
{
	WidgetFun::setVisible(m_pSeatNode,"XuanQueNode",bShow);

	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	if (m_iIdex == 0)
	{
		WidgetFun::setVisible(m_pSeatNode,"Img_QueColor2",false);
		WidgetFun::setVisible(m_pSeatNode,"Img_QueColor1",false);
		WidgetFun::setVisible(m_pSeatNode,"Img_QueColor3",false);

		std::string  kColorShow = utility::toString("Img_QueColor",(int)m_nQueColor);
		WidgetFun::setVisible(m_pSeatNode,kColorShow,true);
	}
}

void XZDDPlayer::setQueColor( int nColor )//����ȱ����ɫ
{
	assert( nColor<= CARD_COLOR_TIAO);
	m_nQueColor = nColor;	
}

byte XZDDPlayer::getQueColor()//���ȱ��
{
	return m_nQueColor;
}

void XZDDPlayer::showQueIcon_NoAni()
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColor_NoAni");
}

void XZDDPlayer::showQueIconAni()//ѡȱ����
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni");
}

void XZDDPlayer::showQueIconAni_Reset()//����ѡ��
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni_Reset");
}

void XZDDPlayer::upPlayerState()//���׼��ͼƬ
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}

void XZDDPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)//��ʾ�Ի�
{
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
		WidgetFun::setImagic(m_pSeatNode,"BiaoQingPic",kStrNet);
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

int XZDDPlayer::getMastKindColor()//������ƻ�ɫ
{
	return m_nMastKindColor;
}

void XZDDPlayer::runAniDianPao()//����
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDDianPaoAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_FANGPAO);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_HU);
}

void XZDDPlayer::runAniZiMo()//����
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDZiMoAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_ZIMO);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_ZIMO);
}

void XZDDPlayer::runAniPeng()//���ƶ���
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDPengAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_PENG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_PENG);
}

void XZDDPlayer::runAniMingGang()//���ܶ���
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDMingGangAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_GANG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_GANG);
}

void XZDDPlayer::runAniAnGang()//���ܶ���
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDAnGangAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_GANG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_AN_GANG);
}

XZDD::CMD_WeaveItem* XZDDPlayer::getWeaveItemArray()//������
{
	return m_kWeaveItemArray;
}

byte XZDDPlayer::getWeaveCount()//����ĵ�����
{
	return m_kWeaveCount;
}
