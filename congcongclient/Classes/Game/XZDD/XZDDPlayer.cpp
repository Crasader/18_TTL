#include "XZDDPlayer.h"
#include "XZDDSoundFun.h"
#include "XZDDGameScence.h"
#include IMAGE_DOWN

XZDDPlayer::XZDDPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)//玩家头像
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

void XZDDPlayer::defaultState()//开始前的准备
{
	setGameRecord(false);
	setChiHuCard(0);
	m_pHandOutCard = NULL;
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	setActOutCard(-1);
	m_bPlayHSAni =false;
	WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);//设置按钮可见性
	WidgetFun::setVisible(m_pSeatNode, "Offline", false);
	WidgetFun::setVisible(m_pSeatNode, "BigOutNode", false);
	WidgetFun::setVisible(m_pSeatNode, "ReadyState", false);
	WidgetFun::getChildWidget(m_pSeatNode, "CardNode1")->removeAllChildren();//清楚所有子节点
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

void XZDDPlayer::setGameRecord(bool bRecord)//设置玩家记录
{
	m_bGameRecord = bRecord;
}

cocos2d::Node* XZDDPlayer::getPlayerNode()//获得玩家节点
{
	return m_pSeatNode;
}

void XZDDPlayer::startGame()//开始游戏 
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","Start");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

}

void XZDDPlayer::startGameNoTime()//开始准备时间结束
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);

}

void XZDDPlayer::EndGame()//结束游戏
{
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","End");
	if (m_pUserItem == NULL )
	{
		return;
	}
}

void XZDDPlayer::PlayerEnter()//玩家准备
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

void XZDDPlayer::PlayerLeave()//玩家离开
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

void XZDDPlayer::upPlayerInfo()//上传玩家信息
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

void XZDDPlayer::setZhuang(bool bZhuang)//设置庄
{
	m_bBanker = bZhuang;
	if (NULL == m_pSeatNode) return;
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",m_bBanker);
}

void XZDDPlayer::showAddGold(int iGold)//显示金币
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

void XZDDPlayer::showEffect(std::string kKey)//显示效果
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}

void XZDDPlayer::showStatusImagic(std::string kKey)//显示准备元素
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}

void XZDDPlayer::showXuanQue(bool bShow)//选缺
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

void XZDDPlayer::setQueColor( int nColor )//设置缺的颜色
{
	assert( nColor<= CARD_COLOR_TIAO);
	m_nQueColor = nColor;	
}

byte XZDDPlayer::getQueColor()//获得缺牌
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

void XZDDPlayer::showQueIconAni()//选缺动作
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni");
}

void XZDDPlayer::showQueIconAni_Reset()//重置选牌
{
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni_Reset");
}

void XZDDPlayer::upPlayerState()//玩家准备图片
{
	if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}

void XZDDPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)//显示对话
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

int XZDDPlayer::getMastKindColor()//最多张牌花色
{
	return m_nMastKindColor;
}

void XZDDPlayer::runAniDianPao()//点炮
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDDianPaoAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_FANGPAO);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_HU);
}

void XZDDPlayer::runAniZiMo()//自摸
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDZiMoAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_ZIMO);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_ZIMO);
}

void XZDDPlayer::runAniPeng()//碰牌动画
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDPengAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_PENG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_PENG);
}

void XZDDPlayer::runAniMingGang()//明杠动画
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDMingGangAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_GANG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_GANG);
}

void XZDDPlayer::runAniAnGang()//暗杠动画
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("XZDDAnGangAni",AnimateNode);

	XZDDSoundFun::playEffectEX(XZDDSoundFun::EX_GANG);
	XZDDSoundFun::playEffectByGender(GetGender(),XZDDSoundFun::EFFECT_AN_GANG);
}

XZDD::CMD_WeaveItem* XZDDPlayer::getWeaveItemArray()//排列牌
{
	return m_kWeaveItemArray;
}

byte XZDDPlayer::getWeaveCount()//获得拍的总数
{
	return m_kWeaveCount;
}
