#include "SRLFPlayer.h"
#include "SRLFSoundFun.h"
#include "SRLFGameScence.h"


SRLFPlayer::SRLFPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)//玩家头像
	,m_pTingNode(NULL)
	,m_kHandCardCout(0)
	,m_kWeaveCount(0)
	,m_bGameRecord(false)
	,m_nQueColor(CARD_COLOR_NULL)
	,m_pHandOutCard(NULL)
	,m_pCurTouchSprite(NULL)
	,m_bMingPai(false)
	,m_bPiao(false)
	,m_bBanker(false)
	,m_isTing(false)
	,m_bBaoTing(false)
	,m_bFirstSended(true)
	,m_kNewHandCard(0)
	,m_bValid(false)
{
	init();
}

SRLFPlayer::~SRLFPlayer()
{

}

void SRLFPlayer::setTingNode(cocos2d::Node* pTingNode)
{
	m_pTingNode = pTingNode;
}

void SRLFPlayer::init()
{
	m_wPlayCount = 0;
	PlayerLeave();
}
int  SRLFPlayer::getIdex()
{
	return m_iIdex;
}
void SRLFPlayer::defaultState()//开始前的准备
{
	if (NULL == m_pSeatNode) return;
	setGameRecord(false);
	setChiHuCard(0);
	m_pHandOutCard = NULL;
	m_kHandCardCout = 0;
	m_kWeaveCount = 0;
	setActOutCard(-1);
	WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",false);//设置按钮可见性
	WidgetFun::setVisible(m_pSeatNode, "Offline", false);
	WidgetFun::setVisible(m_pSeatNode, "BigOutNode", false);
	WidgetFun::setVisible(m_pSeatNode, "ReadyState", false);
	WidgetFun::setVisible(m_pSeatNode, "ReadyIcon", false);
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
	setZhuangVale(false);
	//setBaoTing(false);
	//setPiao(false);
}
void SRLFPlayer::setGameRecord(bool bRecord)//设置玩家记录
{
	m_bGameRecord = bRecord;
}

cocos2d::Node* SRLFPlayer::getPlayerNode()//获得玩家节点
{
	return m_pSeatNode;
}
void SRLFPlayer::startGame()//开始游戏 
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","Start");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::setVisible(m_pSeatNode, "ReadyIcon", false);
}

void SRLFPlayer::startGameNoTime()//开始准备时间结束
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::setVisible(m_pSeatNode, "ReadyIcon", false);

}

void SRLFPlayer::EndGame()//结束游戏
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::runWidgetAction(m_pSeatNode,"Head","End");
	if (m_pUserItem == NULL )
	{
		return;
	}
}

void SRLFPlayer::PlayerEnter()//玩家准备
{
	m_wPlayCount = 0;
	resetPlayer();
}
void SRLFPlayer::resetPlayer()//
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::setVisible(m_pSeatNode,"Name",true);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",true);

	if (WidgetFun::getChildWidget(m_pSeatNode,"ButtonPlayerHeadClick"))
	{
		WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","UserID",utility::toString((int)GetUserID()));
	}
}

void SRLFPlayer::PlayerLeave()//玩家离开
{
	if ( NULL == m_pSeatNode ) return;
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::setVisible(m_pSeatNode, "ReadyIcon", false);
	WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","");
}

void SRLFPlayer::upPlayerInfo()//上传玩家信息
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());

	if (SRLFGameScence::Instance().HaveOptionRule(SRLFGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE1) == false
		&&SRLFGameScence::Instance().HaveOptionRule(SRLFGameScence::GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO) == false)
	{
		WidgetFun::setText(m_pSeatNode,"GoldTxt",(int)GetUserScore());
	}
	if (GetHeadHttp() != "")
	{
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",m_bBanker);
	upPlayerState();
	updataPiao();
	upBaoTing();
}
void SRLFPlayer::setZhuang()//设置庄
{
	if ( NULL == m_pSeatNode ) return;
	m_bBanker = true;
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}

void SRLFPlayer::updataPiao()//飘
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::setVisible(m_pSeatNode,"Piao",m_bPiao == true);
}

void SRLFPlayer::setZhuangVale(bool bBanker)//设置庄
{
	m_bBanker = bBanker;
}
void SRLFPlayer::showAddGold(int iGold)//显示金币
{
	if ( NULL == m_pSeatNode ) return;
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
void SRLFPlayer::showEffect(std::string kKey)//显示效果
{
	if ( NULL == m_pSeatNode ) return;
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"EffectImagic");
	WidgetFun::setPlaceImagicKey(pNode,kKey);
	pNode->stopAllActions();
	WidgetFun::runWidgetAction(pNode,"Start");
}
void SRLFPlayer::showStatusImagic(std::string kKey)//显示准备元素
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",true);
}

void SRLFPlayer::showXuanQue(bool bShow)//选缺
{
	if ( NULL == m_pSeatNode ) return;
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


void SRLFPlayer::setQueColor( int nColor )//设置缺的颜色
{
	assert( nColor<= CARD_COLOR_TIAO);
	m_nQueColor = nColor;	
}
BYTE SRLFPlayer::getQueColor()//获得缺牌
{
	return m_nQueColor;
}

void SRLFPlayer::showQueIcon_NoAni()
{
	if ( NULL == m_pSeatNode ) return;
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColor_NoAni");

}
void SRLFPlayer::showQueIconAni()//选缺动作
{
	if ( NULL == m_pSeatNode ) return;
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni");
}
void SRLFPlayer::showQueIconAni_Reset()//重置选牌
{
	if ( NULL == m_pSeatNode ) return;
	if (m_nQueColor == CARD_COLOR_NULL)
	{
		return;
	}

	WidgetFun::setPlaceImagicKey(m_pSeatNode,"QueIcon_Img",utility::toString("QueIcon",(int)m_nQueColor));
	WidgetFun::setVisible(m_pSeatNode,"QueIcon_Img",true);
	WidgetFun::runWidgetAction(m_pSeatNode,"QueIcon_Img","QueColorAni_Reset");
}

void SRLFPlayer::upPlayerState()//玩家准备图片
{
	if ( NULL == m_pSeatNode ) return;
	WidgetFun::setVisible(m_pSeatNode, "ReadyIcon", GetUserStatus() == US_READY);
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}

void SRLFPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)//显示对话
{
	if ( NULL == m_pSeatNode ) return;
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
		utility::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",
			kStrNet,NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
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
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(4.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkFile"))return;
		std::vector<std::string> kStr = utility::split(kStrNet,":");
		ASSERT(kStr.size()==2);
	    WidgetFun::runWidgetAction(m_pSeatNode,"TalkFile","Start");
		SoundFun::Instance().playEffect(kStr[0]);
		utility::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",kStr[1],NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
	}
}

int SRLFPlayer::getMastKindColor()//最多张牌花色
{
	return m_nMastKindColor;
}

void SRLFPlayer::runAniDianPao()//点炮
{
	if ( NULL == m_pSeatNode ) return;
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("SRLFDianPaoAni",AnimateNode);

	SRLFSoundFun::playEffectEX(SRLFSoundFun::EX_FANGPAO);
	SRLFSoundFun::playEffectByGender(GetGender(),SRLFSoundFun::EFFECT_HU);
}
void SRLFPlayer::runAniZiMo()//自摸
{
	if ( NULL == m_pSeatNode ) return;
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("SRLFZiMoAni",AnimateNode);

	SRLFSoundFun::playEffectEX(SRLFSoundFun::EX_ZIMO);
	SRLFSoundFun::playEffectByGender(GetGender(),SRLFSoundFun::EFFECT_ZIMO);
}
void SRLFPlayer::runAniPeng()//碰牌动画
{
	if ( NULL == m_pSeatNode ) return;
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("SRLFPengAni",AnimateNode);

	SRLFSoundFun::playEffectEX(SRLFSoundFun::EX_PENG);
	SRLFSoundFun::playEffectByGender(GetGender(),SRLFSoundFun::EFFECT_PENG);
}
void SRLFPlayer::runAniMingGang()//明杠动画
{
	if ( NULL == m_pSeatNode ) return;
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("SRLFMingGangAni",AnimateNode);

	SRLFSoundFun::playEffectEX(SRLFSoundFun::EX_GANG);
	SRLFSoundFun::playEffectByGender(GetGender(),SRLFSoundFun::EFFECT_GANG);
}
void SRLFPlayer::runAniAnGang()//暗杠动画
{
	if ( NULL == m_pSeatNode ) return;
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("SRLFAnGangAni",AnimateNode);

	SRLFSoundFun::playEffectEX(SRLFSoundFun::EX_GANG);
	SRLFSoundFun::playEffectByGender(GetGender(),SRLFSoundFun::EFFECT_AN_GANG);
}
SRLF::CMD_WeaveItem* SRLFPlayer::getWeaveItemArray()//排列牌
{
	return m_kWeaveItemArray;
}
BYTE SRLFPlayer::getWeaveCount()//获得拍的总数
{
	return m_kWeaveCount;
}

void SRLFPlayer::setPiao(bool isPiao)
{
	m_bPiao = isPiao;
}

bool SRLFPlayer::isPiao()
{
	return m_bPiao;
}

void SRLFPlayer::setBaoTing(bool isBaoTing)
{
	m_bBaoTing = isBaoTing;
}

bool SRLFPlayer::isBaoTing()
{
	return m_bBaoTing;
}

void SRLFPlayer::upBaoTing()
{
	if ( NULL == m_pTingNode ) return;
	m_pTingNode->setVisible(m_bBaoTing);
}



