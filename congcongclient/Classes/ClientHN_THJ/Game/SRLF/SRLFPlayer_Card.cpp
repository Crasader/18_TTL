#include "SRLFPlayer.h"
#include "SRLFGameLogic.h"
#include "SRLFSoundFun.h"
#include "SRLFGameScence.h"


void SRLFPlayer::setHandCard(BYTE* pCardData,int iCout)
{
	m_kHandCardCout = iCout;
	//if (iCout <= 0 || iCout > MAX_COUNT)
	int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
	if (iCout <= 0 || iCout > max_count)
	{
		return;
	}
	memcpy(m_kHandCardData,pCardData,sizeof(BYTE)*iCout);
	m_kHandCardCout = iCout;
}
void SRLFPlayer::setWeaveItem(SRLF::CMD_WeaveItem* pWeave,int iCout)
{
	m_kWeaveCount = 0;
	//if (iCout <= 0 || iCout >MAX_WEAVE)
	//{
	//	return;
	//}
	int max_weave = SRLFGameScence::Instance().GetWeaveMax();
	if (iCout <= 0 || iCout > max_weave )
	{
		return;
	}
	memcpy(m_kWeaveItemArray,pWeave,sizeof(SRLF::CMD_WeaveItem)*iCout);
	m_kWeaveCount = iCout;
}
void SRLFPlayer::setOperateResoult(SRLF::CMD_S_OperateResult* pNetInfo)
{
	bool cbPublicCard = false;
	word wOperateUser=pNetInfo->wOperateUser;
	BYTE cbOperateCard=pNetInfo->cbOperateCard;

	if (pNetInfo->cbOperateCode == WIK_PENG)
	{
		runAniPeng();
	}
	if ((pNetInfo->cbOperateCode&WIK_GANG)!=0)
	{
		//组合扑克
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_kWeaveCount;i++)
		{
			BYTE cbWeaveKind=m_kWeaveItemArray[i].cbWeaveKind;
			BYTE cbCenterCard=m_kWeaveItemArray[i].cbCenterCard;
			if ((cbCenterCard==pNetInfo->cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				cbPublicCard = true;
				m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=cbPublicCard;
				m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
				m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
				break;
			}
		}

		//组合扑克
		if (cbWeaveIndex==0xFF)
		{
			//暗杠判断
			cbPublicCard=(pNetInfo->wProvideUser==wOperateUser)?FALSE:TRUE;

			//设置扑克
			cbWeaveIndex=m_kWeaveCount++;
			m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
			m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;
		}

		//扑克设置
		if (m_iIdex == 0 || m_bGameRecord|| m_bMingPai )
		{
			 SRLFLOGIC::CGameLogic::Instance().RemoveValueCardAll(m_kHandCardData,m_kHandCardCout,pNetInfo->cbOperateCard);
		}
		
		//m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3 - 1;
		int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
		m_kHandCardCout = max_count - m_kWeaveCount*3 - 1;



		if ( !cbPublicCard)
		{
			runAniAnGang();
		}
		else
		{
			runAniMingGang();
		}
	}
	else if (pNetInfo->cbOperateCode!=WIK_NULL)
	{
		//设置组合
		BYTE cbWeaveIndex=m_kWeaveCount++;
		m_kWeaveItemArray[cbWeaveIndex].cbPublicCard=true;
		m_kWeaveItemArray[cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_kWeaveItemArray[cbWeaveIndex].cbWeaveKind=pNetInfo->cbOperateCode;
		m_kWeaveItemArray[cbWeaveIndex].wProvideUser=pNetInfo->wProvideUser;

		//组合界面
		//删除扑克
		if (m_iIdex == 0 || m_bGameRecord || m_bMingPai)
		{
			BYTE cbWeaveCard[4]={cbOperateCard,cbOperateCard,cbOperateCard,cbOperateCard};
			SRLFLOGIC::CGameLogic::Instance().RemoveValueCard(m_kHandCardData,m_kHandCardCout,cbWeaveCard,2);
			m_kHandCardCout -= 2;
		}
		else
		{
			//m_kHandCardCout = MAX_COUNT - m_kWeaveCount*3;
			int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
			m_kHandCardCout = max_count - m_kWeaveCount*3;
		}
	}
	showHandCard();
}
void SRLFPlayer::addNewInCard(int iCard)
{
	if(iCard != 0)
	m_kHandCardCout++;
	//m_kHandCardData[MAX_COUNT-1-m_kWeaveCount*3] = iCard;
	int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
	m_kHandCardData[max_count-1-m_kWeaveCount*3] = iCard;
	showHandCard();
}
void SRLFPlayer::sendOutCard(int iCard)
{
	m_pHandOutCard = addHandOutCard(iCard);
	if ( NULL == m_pHandOutCard) return;
	m_pHandOutCard->setVisible(false);
	WidgetFun::runWidgetAction(m_pHandOutCard,"OutAni");//等待2秒显示
	setActOutCard(iCard);
}

void SRLFPlayer::showMJPoint()
{
	if( NULL == m_pSeatNode ) return;
	if (m_pHandOutCard)
	{
		WidgetFun::setPos(m_pSeatNode,"MJPoint",m_pHandOutCard->getPosition());
		WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","Start");
	}
}

void SRLFPlayer::setActOutCard(int iCard)
{
	if( NULL == m_pSeatNode ) return;
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"BigOutNode");
	pNode->setVisible(iCard >= 0);

	if (iCard < 0)
	{
		WidgetFun::runWidgetAction(m_pSeatNode,"MJPoint","End");
		return;
	}
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);

	if (m_pHandOutCard != NULL)
	{
		//出牌动画
		cocos2d::Vec2 kEndPos = m_pHandOutCard->getPosition();
		pNode->setAnchorPoint(m_pHandOutCard->getAnchorPoint());
		WidgetFun::setWidgetUserInfo(pNode,"OutHandPos",utility::toString(kEndPos));
		WidgetFun::runWidgetAction(pNode,"Start");
		showMJPoint();
	}

	SRLFSoundFun::playCardByGender(GetGender(),iCard);

	if (m_iIdex == 0 || m_bGameRecord || m_bMingPai)
	{
		bool isRemove = SRLFLOGIC::CGameLogic::Instance().RemoveValueCardOne(m_kHandCardData,m_kHandCardCout,iCard);
		if(isRemove) m_kHandCardCout --;
		SRLFLOGIC::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
	}
	else
	{
		m_kHandCardCout -- ;
	}

	showHandCard();

}
cocos2d::Node* SRLFPlayer::addHandOutCard(int iCard)//出牌
{
	if( NULL == m_pSeatNode ) return NULL;
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode2");//找到放牌节点
	
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"OutPosNode")->getPosition();//获得出牌位置节点
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(m_pSeatNode,"OutPosNode","AddOder"));//获得节点的order
	cocos2d::Vec2 kOutAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(m_pSeatNode,"OutPosNode","OutAddPos"));//获得节点向上偏移位子
	cocos2d::Vec2 kNextAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(m_pSeatNode,"OutPosNode","NextAddPos"));//获得节点左偏移位置
	
	cocos2d::Vec2 kPos;
	int nLength = m_kOutCardList.size();
	kPos = kOutAddPos*(nLength%8)+kStartPos;
	kPos -= kNextAddPos*(nLength/8);
	
	std::string kHandSkin = utility::toString("SRLF_OUT_",iIdex);
	cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);//创建牌节点放入"CardNode2"节点下
	
	iAddOder = iAddOder*nLength+1;
	pNode->setLocalZOrder(iAddOder);//设置本地渲染顺序
	pNode->setPosition(kPos);//设置位子
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	setCardImagic(pNode,iCard,kImagic);
	m_kOutCardList.push_back(OutCardInfo(pNode,iCard));
	return pNode;
}
void SRLFPlayer::showHandCard()//
{
	showHandCard(m_kWeaveItemArray,m_kWeaveCount,m_kHandCardData,m_kHandCardCout);
}
void SRLFPlayer::showHandCard(SRLF::CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)//显示手牌
{
	if( NULL == m_pSeatNode) return;
	int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
	//bool bHaveNewIn = (iWeaveCout*3+iHandCout) == MAX_COUNT;
	bool bHaveNewIn = (iWeaveCout*3+iHandCout) == max_count;
	int iIdex = m_iIdex;
	cocos2d::Node* pCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");//手牌节点
	pCardNode->removeAllChildren();
	m_pCurTouchSprite = NULL;
	cocos2d::Vec2 kStartPos = WidgetFun::getChildWidget(m_pSeatNode,"HandPosNode")->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","AddOder"));
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","HandAddPos"));//牌位置
	cocos2d::Vec2 kNewInAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(
		m_pSeatNode,"HandPosNode","NewInAddPos"));//牌间隔
	std::string kPengSkin = utility::toString("SRLF_PENG_",iIdex);
	std::string kGangSkin = utility::toString("SRLF_GANG_",iIdex);
	std::string kHandSkin = utility::toString("SRLF_HAND_",iIdex);
	if (max_count == 8)
	{
		kStartPos += kHandAddPos*3;
	}
	int iOder = 0;

	float fValue =1.f;
	if (iIdex == 0)
	{
		fValue = 0.8f;
	}
	cocos2d::Vec2 kWeaveAddPos =  kHandAddPos*3*fValue;//移动3个排位
	for (int i = 0;i<iWeaveCout;i++)
	{
		SRLF::CMD_WeaveItem* pTemp = pWeave+i;
		int iCardValue = pTemp->cbCenterCard;
		if (pTemp->cbPublicCard == 0)
		{
			iCardValue = -1;
		}

		if (pTemp->cbWeaveKind == WIK_PENG)//加入3个节点
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kWeaveAddPos;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG)//加入4个节点
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kWeaveAddPos;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			if (m_iIdex == 0 || m_bGameRecord||m_bMingPai)
			{
				setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),pTemp->cbCenterCard,kImagic);
			}
			else
			{
				//setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),iCardValue,kImagic);
				setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),pTemp->cbCenterCard,kImagic);
			}
		}
		iOder+=iAddOder;
	}
	for (int i = 0;i<iHandCout;i++)
	{
		BYTE* pTemp = pHandCard+i;
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos);
		if (bHaveNewIn && i == (iHandCout - 2))
		{
			kStartPos += kNewInAddPos;
		}
		else
		{
			kStartPos += kHandAddPos;
		}
		int iCardValue = *pTemp;
		if (m_iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");

			setCardImagic(pNode,iCardValue,kImagic);
		}
		else if (m_bMingPai|| m_bGameRecord)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
			float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pNode,"HuScale"));
			pNode->setScale(fHuScale);
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
}
void SRLFPlayer::setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront)
{
	if (kValue > 0)
	{
		BYTE cbValue=((BYTE)kValue&MASK_VALUE);
		BYTE cbColor=(((BYTE)kValue&MASK_COLOR)>>4)+1;
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),false);
	}
	else
	{
		WidgetFun::setImagic(pNode,utility::toString(kImagicFront,0,0,".png"),false);
	}
}

bool SRLFPlayer::isTouchCardNode(cocos2d::Node* pNode)
{
	return (m_pCurTouchSprite == pNode && m_pCurTouchSprite != NULL);
}

cocos2d::Node* SRLFPlayer::getTouchCardNode(cocos2d::Vec2 kTouchPos)//获得选中的节点
{
	if( NULL == m_pSeatNode) return NULL;
	cocos2d::Node* pNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (utility::IsSpriteInTouch(kTouchPos,pSprite))
		{
			m_pCurTouchSprite = pSprite;
			return pSprite;
		}
	}
	return NULL;
}

BYTE SRLFPlayer::getTouchCardVlaue(cocos2d::Node* pNode)//获得点中的牌面
{
	if (m_pCurTouchSprite != pNode)
	{
		return NULL;
	}
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (pSprite == pNode)
		{
			return m_kHandCardData[iIdex];
		}
		iIdex++;
	}
	return NULL;
}
void SRLFPlayer::showHuPai(bool bGaizhu)//显示胡牌
{
	if( NULL == m_pSeatNode) return;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		
		if(bGaizhu)
		{
			setCardImagic(pSprite,0,kImagic);
		}
	}
}

void SRLFPlayer::sortHandCard()//手牌排序
{
	if (m_iIdex != 0 )return;
	SRLFLOGIC::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
}

void SRLFPlayer::showHuanPai(int nGameType /*= GAME_TYPE_CHENGDU*/)//显示换牌
{
	if (m_iIdex != 0 )return;

	sortHandCard();
	showHandCard();

	std::vector<SRLF::Card_Info> kCardInfoList;

	//SRLFLOGIC::CGameLogic::Instance().GetCardInfoList(m_kHandCardData,MAX_COUNT-1,kCardInfoList);
	int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
	SRLFLOGIC::CGameLogic::Instance().GetCardInfoList(m_kHandCardData,max_count-1,kCardInfoList);

	std::vector<BYTE> kCardDataList;
	if (nGameType == GAME_TYPE_SICHUAN)
	{
		kCardDataList = setHuanPaiSiChuan(kCardInfoList);
	}
	else if (nGameType == GAME_TYPE_CHENGDU)
	{
		kCardDataList = setHuanPaiChengDu(kCardInfoList);
		ASSERT((int)kCardDataList.size() == HUAN_CARD_NUM);
		int nColor = SRLFLOGIC::CGameLogic::Instance().GetCardColor(kCardDataList[0]);
		blackHandCardByColor(nColor);
	}

	m_pHuanCardNodeList.clear();
	getCardNodeByCard(kCardDataList,m_pHuanCardNodeList);

	for (int i=0;i<(int)m_pHuanCardNodeList.size();i++)
	{
		cocos2d::Node* pNode = m_pHuanCardNodeList[i];
		pNode->setPositionY(CARD_UP_POSY);
	}
}

std::vector<BYTE> SRLFPlayer::setHuanPaiSiChuan(std::vector<SRLF::Card_Info> kCardInfoList)//设置换牌川麻将
{
	std::vector<BYTE> kCardDataList;

	ASSERT(kCardInfoList.size() == CARD_COLOR_TIAO);  //花色数判断
	if (kCardInfoList.size() != CARD_COLOR_TIAO)return kCardDataList;

	int nLen1 = kCardInfoList[0].nLen;			//最少花色
	int nLen2 = kCardInfoList[1].nLen;			//中间花色
	int nLen3 = kCardInfoList[2].nLen;			//最多花色

	//ASSERT(nLen1 + nLen2 + nLen3 == MAX_COUNT-1 );
	//if (nLen1 + nLen2 + nLen3 != MAX_COUNT-1 ) return kCardDataList;
	int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
	ASSERT(nLen1 + nLen2 + nLen3 == max_count-1 );
	if (nLen1 + nLen2 + nLen3 != max_count-1 ) return kCardDataList;

	m_nMastKindColor = kCardInfoList[2].nColor;

	zeromemory(m_pHuanCards,sizeof(m_pHuanCards));

	int iIdex = 0;
	for (int i = 0;i<nLen1 && iIdex < 3;i++)
	{
		m_pHuanCards[iIdex]=kCardInfoList[0].kCardList[i];
		iIdex++;
	}
	for (int i = 0;i<nLen2 && iIdex < 3;i++)
	{
		m_pHuanCards[iIdex]=kCardInfoList[1].kCardList[i];
		iIdex++;
	}
	for (int i = 0;i<nLen3 && iIdex < 3;i++)
	{
		m_pHuanCards[iIdex]=kCardInfoList[2].kCardList[i];
		iIdex++;
	}

	kCardDataList.push_back(m_pHuanCards[0]);
	kCardDataList.push_back(m_pHuanCards[1]);
	kCardDataList.push_back(m_pHuanCards[2]);

	return kCardDataList;
}
std::vector<BYTE> SRLFPlayer::setHuanPaiChengDu(std::vector<SRLF::Card_Info> kCardInfoList)//设置换牌成都
{
	std::vector<BYTE> kCardDataList;

	ASSERT(kCardInfoList.size() == CARD_COLOR_TIAO);  //花色数判断
	if (kCardInfoList.size() != CARD_COLOR_TIAO)return kCardDataList;

	int nLen1 = kCardInfoList[0].nLen;			//最少花色
	int nLen2 = kCardInfoList[1].nLen;			//中间花色
	int nLen3 = kCardInfoList[2].nLen;			//最多花色

	//ASSERT(nLen1 + nLen2 + nLen3 == MAX_COUNT-1 );
	//if (nLen1 + nLen2 + nLen3 != MAX_COUNT-1 ) return kCardDataList;
	
	int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
	ASSERT(nLen1 + nLen2 + nLen3 == max_count-1 );
	if (nLen1 + nLen2 + nLen3 != max_count-1 ) return kCardDataList;


	m_nMastKindColor = kCardInfoList[2].nColor;

	zeromemory(m_pHuanCards,sizeof(m_pHuanCards));

	if (nLen1>=3)
	{
		m_pHuanCards[0]=kCardInfoList[0].kCardList[0];
		m_pHuanCards[1]=kCardInfoList[0].kCardList[1];
		m_pHuanCards[2]=kCardInfoList[0].kCardList[2];
	}
	else if (nLen2>=3)
	{
		m_pHuanCards[0]=kCardInfoList[1].kCardList[0];
		m_pHuanCards[1]=kCardInfoList[1].kCardList[1];
		m_pHuanCards[2]=kCardInfoList[1].kCardList[2];
	}
	else if (nLen3>=3)
	{
		m_pHuanCards[0]=kCardInfoList[2].kCardList[0];
		m_pHuanCards[1]=kCardInfoList[2].kCardList[1];
		m_pHuanCards[2]=kCardInfoList[2].kCardList[2];
	}

	kCardDataList.push_back(m_pHuanCards[0]);
	kCardDataList.push_back(m_pHuanCards[1]);
	kCardDataList.push_back(m_pHuanCards[2]);

	return kCardDataList;
}

void SRLFPlayer::standUpCard(BYTE cbCard,bool nOnly /*= true*/)//立着的牌
{
	cocos2d::Node* pCard = getCardNodeByCard(cbCard);
	pCard->setPositionY(CARD_UP_POSY);
}
void SRLFPlayer::seatDownCard()//倒下的牌
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		pSprite->setPositionY(CARD_DWON_POSY);
	}
}

cocos2d::Node* SRLFPlayer::getCardNodeByCard(BYTE cbCard)
{
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}

		if (cbCard == m_kHandCardData[iIdex])
		{
			return pSprite;
		}
		iIdex++;
	}
	return NULL;
}

void SRLFPlayer::getCardNodeByCard(std::vector<BYTE> CardDataList,std::vector<cocos2d::Node*>& kCardList)//获得换牌数据
{
	int nSize = CardDataList.size();

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");

	for (int n=0;n<nSize;n++)
	{
		int iIdex = 0;
		for (int i = 0;i<pRootNode->getChildrenCount();i++)
		{
			cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
			if (pSprite == NULL || pSprite->getTag() != 1)
			{
				continue;
			}
			if (CardDataList[n] == m_kHandCardData[iIdex]&& ! isHave<cocos2d::Node>(kCardList,pSprite))
			{
				kCardList.push_back(pSprite);
				break;
			}
			iIdex++;
		}
	}
}

void SRLFPlayer::setHuanPai(BYTE* nHuan)//设置换牌
{
	utility::log(utility::toString("SRLFPlayer::setHuanPai",(int)nHuan[0]," ",(int)nHuan[1]," ",(int)nHuan[2]).c_str());

	//SRLFLOGIC::CGameLogic::Instance().RemoveValueCard(m_kHandCardData,MAX_COUNT-1,m_pHuanCards,HUAN_CARD_NUM);
	//SRLFLOGIC::CGameLogic::Instance().ReplaceCardData(m_kHandCardData,MAX_COUNT-1,nHuan,HUAN_CARD_NUM);
	int max_count = SRLFGameScence::Instance().GetHandCardsMaxCount();
	SRLFLOGIC::CGameLogic::Instance().RemoveValueCard(m_kHandCardData,max_count-1,m_pHuanCards,HUAN_CARD_NUM);
	SRLFLOGIC::CGameLogic::Instance().ReplaceCardData(m_kHandCardData,max_count-1,nHuan,HUAN_CARD_NUM);

	sortHandCard(); //重新设置牌
	showHandCard();

	std::vector<BYTE> kCardDataList;
	kCardDataList.push_back(nHuan[0]);
	kCardDataList.push_back(nHuan[1]);
	kCardDataList.push_back(nHuan[2]);

	std::vector<cocos2d::Node*> kCardList;
	getCardNodeByCard(kCardDataList,kCardList);
 	for (int i=0;i<(int)kCardList.size();i++)
 	{
 		cocos2d::Node* pNode = kCardList[i];
 		assert(pNode);
 
 		cocos2d::Vec2 posEnd = pNode->getPosition();
 		WidgetFun::setWidgetUserInfo(pNode,"CardPos",utility::toString((int)posEnd.x," ",(int)posEnd.y));
 		WidgetFun::runWidgetAction(pNode,"HuanPaiAni");
 	}
}

void SRLFPlayer::saveRemoveHuanPai(BYTE* nHuan)//保存和删除换牌数据
{
	memcpy(m_pHuanCards,nHuan,sizeof(m_pHuanCards));
	//std::vector<BYTE> kCardDataList;
	//for (int i = 0;i<HUAN_CARD_NUM;i++)
	//{
	//	kCardDataList.push_back(nHuan[i]);
	//}
	//seatDownCard();
	//m_pHuanCardNodeList.clear();
	//getCardNodeByCard(kCardDataList,m_pHuanCardNodeList);

	//for (int i=0;i<(int)m_pHuanCardNodeList.size();i++)
	//{
	//	cocos2d::Node* pNode = m_pHuanCardNodeList[i];
	//	pNode->setPositionY(CARD_UP_POSY);
	//}
}
void SRLFPlayer::saveHuanPai(BYTE* nHuan)//保存换牌
{
	memcpy(m_pSaveHuanPai,nHuan,sizeof(m_pSaveHuanPai));
}

void SRLFPlayer::setHuanPai_ChengDu()//设置换牌
{
	setHuanPai(m_pSaveHuanPai);
}
void SRLFPlayer::blackAllHandCard(bool bBlack)//所有牌设置成黑色
{
	if ( NULL == m_pSeatNode) return;
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		if (bBlack)
		{
			pSprite->setColor(cocos2d::Color3B::GRAY);
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
		}
		iIdex++;
	}
}

void SRLFPlayer::blackHandCardByQueColor()//缺牌颜色设置成黑色
{
	if ( NULL == m_pSeatNode) return;
	bool bHaveQue = false;
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		//SRLFLOGIC::CGameLogic::Instance().SortCardList(m_kHandCardData,MAX_COUNT);
		BYTE nColor = SRLFLOGIC::CGameLogic::Instance().GetCardColor(m_kHandCardData[iIdex]);

		if (nColor == m_nQueColor)
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
			bHaveQue = true;
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::GRAY);
		}
		iIdex++;
	}

	if (!bHaveQue)
	{
		blackAllHandCard(false);
	}
}
void SRLFPlayer::blackHandCardByColor(int nColorWhite)
{
	if ( NULL == m_pSeatNode) return;
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		BYTE nColor = SRLFLOGIC::CGameLogic::Instance().GetCardColor(m_kHandCardData[iIdex]);

		if (nColor == nColorWhite)
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::GRAY);
		}
		iIdex++;
	}
}

void SRLFPlayer::ClickCard(cocos2d::Node* pCard)
{
	if (pCard == NULL )return;
	int nPosY = pCard->getPositionY();
	if (nPosY == CARD_DWON_POSY)
	{
		seatDownCard();
		pCard->setPositionY(CARD_UP_POSY);
	}
	else
	{
		pCard->setPositionY(CARD_DWON_POSY);
	}
}

int SRLFPlayer::getUpCardList(std::vector<BYTE>& kCardDataList)
{
	if ( NULL == m_pSeatNode) return 0;
	if (m_iIdex != 0)return 0;

	kCardDataList.clear();
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		
		int nPoy = pSprite->getPositionY();
		if (nPoy == CARD_UP_POSY)
		{
			kCardDataList.push_back(m_kHandCardData[iIdex]);
		}
		iIdex++;
	}
	if ((int)kCardDataList.size()>0)
	{
		return SRLFLOGIC::CGameLogic::Instance().GetCardColor(kCardDataList[0]);
	}
	else
	{
		return CARD_COLOR_NULL;
	}
}



bool SRLFPlayer::getHuanCards(BYTE nHuanCard[HUAN_CARD_NUM])
{
	std::vector<BYTE> kCardDataList;
	getUpCardList(kCardDataList);

	if (kCardDataList.size() != 3)
	{
		NoticeMsg::Instance().ShowTopMsgByScript("HuanPaiError0");
		return false;
	}

	for (int n=0;n<HUAN_CARD_NUM;n++)
	{
		nHuanCard[n]=kCardDataList[n];
	}
	memcpy(m_pHuanCards,nHuanCard,sizeof(m_pHuanCards));
	return true;
}


int SRLFPlayer::getDefuaultHuanValue(int nIndex)//获取第几个默认换牌的值
{
	ASSERT( nIndex>=0 && nIndex<HUAN_CARD_NUM);

	for (;nIndex<(int)m_pHuanCardNodeList.size();nIndex++)
	{
		if (m_pHuanCardNodeList[nIndex]->getPositionY() == CARD_DWON_POSY)
		{
			return getTouchCardVlaue(m_pHuanCardNodeList[nIndex]);
		}
	}
	ASSERT(false);
	return 0;
}
void SRLFPlayer::showCard()
{
	showCard(m_kHandCardData,m_kHandCardCout);
}
void SRLFPlayer::showCard(BYTE* cbCardData,BYTE cbCardCount)
{
	if ( NULL == m_pSeatNode) return;
	BYTE cbIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
		pSprite->setScale(fHuScale);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
		if (cbIdex < cbCardCount)
		{
			setCardImagic(pSprite,cbCardData[cbIdex],kImagic);
		}
		else
		{
			setCardImagic(pSprite,0,kImagic);
		}
		cbIdex++;
	}
	blackAllHandCard(false);
}

BYTE SRLFPlayer::getGangCard( BYTE currentCard)
{
	ASSERT(m_iIdex == 0);
	BYTE cardIndex[MAX_INDEX]={0};
	SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cardIndex);
	BYTE bValue = SRLFLOGIC::CGameLogic::Instance().EstimateGangCard(cardIndex,currentCard);
	if (bValue & WIK_GANG)//表示吃杠
	{
		return currentCard;
	}
	SRLFLOGIC::tagGangCardResult kRes;
	SRLFLOGIC::CGameLogic::Instance().AnalyseGangCard(cardIndex,m_kWeaveItemArray,m_kWeaveCount,kRes);
	if (kRes.cbCardCount>0)
	{
		return kRes.cbCardData[0];
	}
	//ASSERT(false);
	return 0;
}

void SRLFPlayer::getGangCardInfo(BYTE* gangCards,BYTE& count,BYTE currentCard)
{
	ASSERT(m_iIdex == 0);
	BYTE cardIndex[MAX_INDEX]={0};
	SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cardIndex);
	BYTE bValue = SRLFLOGIC::CGameLogic::Instance().EstimateGangCard(cardIndex,currentCard);
	if (bValue & WIK_GANG)//表示吃杠
	{
		return;
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cardIndex[i]==4)
		{
			gangCards[count++]=SRLFLOGIC::CGameLogic::Instance().SwitchToCardData(i);
		}
	}
	for (BYTE i=0;i<MAX_WEAVE;i++)
	{
		if (m_kWeaveItemArray[i].cbWeaveKind == WIK_PENG)
		{
			if(cardIndex[SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(m_kWeaveItemArray[i].cbCenterCard)]==1)
				gangCards[count++]=m_kWeaveItemArray[i].cbCenterCard;
		}
	}
}

void SRLFPlayer::showGangCards(BYTE* gangCards,BYTE gangCount)
{
	if ( NULL == m_pSeatNode) return;
	int iIdex = 0;
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
	for (int i = 0;i<pRootNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
		if (pSprite == NULL || pSprite->getTag() != 1)
		{
			continue;
		}
		bool bBlack = true;
		for (int j=0;j<gangCount;j++)
		{
			if(m_kHandCardData[iIdex] == gangCards[j])
			{
				bBlack =false;
				break;
			}
		}
		if (bBlack)
		{
			pSprite->setColor(cocos2d::Color3B::GRAY);
		}
		else
		{
			pSprite->setColor(cocos2d::Color3B::WHITE);
		}
		iIdex++;
	}
}

BYTE SRLFPlayer::isChiGangCard( BYTE currentCard)
{
	ASSERT(m_iIdex == 0);
	BYTE cardIndex[MAX_INDEX]={0};
	SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cardIndex);
	BYTE bValue = SRLFLOGIC::CGameLogic::Instance().EstimateGangCard(cardIndex,currentCard);
	return bValue&WIK_GANG;
}

void SRLFPlayer::removeHandOutCard( BYTE cbCardData )
{
	int nSize = m_kOutCardList.size();
	if (nSize <=0 )
	{
		return;
	}
	int i = nSize -1;
	if (m_kOutCardList[i].pCardNode != NULL && m_kOutCardList[i].nCardDarta == cbCardData)
	{
		m_kOutCardList[i].pCardNode->removeFromParent();
		m_kOutCardList[i].pCardNode = NULL;
		m_kOutCardList.pop_back();
	}

}
void SRLFPlayer::showjiesuanCard(cocos2d::Node* pCardNode)//
{
	showJieSuanCard(pCardNode,m_kWeaveItemArray, m_kWeaveCount, m_kHandCardData, m_kHandCardCout);
}

void SRLFPlayer::showJieSuanCard(cocos2d::Node* pCardNode,SRLF::CMD_WeaveItem* pWeave,int iWeaveCout,BYTE* pHandCard,int iHandCout)//结算
{
	ASSERT(pCardNode);
	pCardNode->removeAllChildren();
	int iIdex = 0;
	cocos2d::Vec2 kStartPos = pCardNode->getPosition();
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"AddOder"));
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanAddPos"));
	cocos2d::Vec2 kJieSuanWeaveGap = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanWeaveGap"));

	std::string kPengSkin = utility::toString("SRLF_PENG_",iIdex);
	std::string kGangSkin = utility::toString("SRLF_GANG_",iIdex);
	std::string kHandSkin = utility::toString("SRLF_HAND_JIESUAN",iIdex);
	int iOder = 0;
	
	for (int i = 0;i<iWeaveCout;i++)
	{
		SRLF::CMD_WeaveItem* pTemp = pWeave+i;
		int iCardValue = pTemp->cbCenterCard;

		if (pTemp->cbWeaveKind == WIK_PENG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kPengSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3 + kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
		}
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kGangSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos);
			kStartPos += kHandAddPos*3+kJieSuanWeaveGap;
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card0"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card1"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card2"),iCardValue,kImagic);
			setCardImagic(WidgetFun::getChildWidget(pNode,"Card3"),iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
	//表示该玩家胡牌
	bool isNewCard = false;
	if (SRLFGameScence::Instance().GetHandCardsMaxCount() == (iHandCout+ iWeaveCout*3))
	{
		isNewCard = true;
		BYTE tempIndex[MAX_INDEX];
		memset(tempIndex,0,sizeof(tempIndex));
		SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(pHandCard,iHandCout,tempIndex);
		memset(pHandCard,0,sizeof(pHandCard));
		tempIndex[SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(getChiHuCard())]--;
		iHandCout =(int)SRLFLOGIC::CGameLogic::Instance().SwitchToCardData(tempIndex,pHandCard);
	}
	kStartPos += cocos2d::Vec2(0, 14);
	for (int i = 0;i<iHandCout;i++)
	{
		BYTE* pTemp = pHandCard+i;
		int iCardValue = *pTemp;
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
		pNode->setTag(1);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos);
		{
			kStartPos += kHandAddPos;
		}
		if (iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
			setCardImagic(pNode,iCardValue,kImagic);
		}
		iOder+=iAddOder;
	}
	//胡牌的牌
	if (isNewCard)
	{
		BYTE cbChiHuCard = getChiHuCard();
		if (cbChiHuCard != 0)
		{
			cocos2d::Vec2 kChiHuPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pCardNode,"JieSuanChiHuPos"));
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHandSkin,pCardNode);
			pNode->setLocalZOrder(iOder);
			pNode->setPosition(kStartPos+kChiHuPos);
			std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"MingImagic");
			setCardImagic(pNode,cbChiHuCard,kImagic);
		}
	}
}
void SRLFPlayer::setChiHuCard(BYTE cbCard)//设置吃胡
{
	m_cbChiHuCard = cbCard;
	if (cbCard>0)
	{
		m_kHuCardList.push_back(cbCard);
		showChiHuCard();
	}
}

void SRLFPlayer::clearChiHuCard()//清除吃胡
{
	if ( NULL == m_pSeatNode) return;
	cocos2d::Node* pChiHuNode=	WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHu");
	pChiHuNode->removeAllChildren();
	m_kHuCardList.clear();
}
BYTE SRLFPlayer::getChiHuCard()//获得吃胡
{
	return m_cbChiHuCard;
}

void SRLFPlayer::showChiHuCard()
{
	if ( NULL == m_pSeatNode) return;
	cocos2d::Node* pChiHuNode=	WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHu");
	pChiHuNode->removeAllChildren();

	cocos2d::Vec2 kStartPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pChiHuNode,"StartPos"));
	int iAddOder = utility::parseInt(WidgetFun::getWidgetUserInfo(pChiHuNode,"AddOder"));
	cocos2d::Vec2 kOutAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pChiHuNode,"OutAddPos"));
	cocos2d::Color3B kColor = utility::parseColour(WidgetFun::getWidgetUserInfo(pChiHuNode,"CardColor"));
	std::string kHuCardSkin = utility::toString("SRLF_OUT_",m_iIdex);

	int iOder = 0;
	for (int i=0;i<(int)m_kHuCardList.size();i++)
	{
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(kHuCardSkin,pChiHuNode);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");

		setCardImagic(pNode,m_kHuCardList[i],kImagic);
		pNode->setLocalZOrder(iOder);
		pNode->setPosition(kStartPos);
		pNode->setColor(kColor);

		kStartPos += kOutAddPos;
		iOder+=iAddOder;
	}
}

void SRLFPlayer::removeHandCard(BYTE cbCard)
{
	if (m_iIdex == 0)
	{
		SRLFLOGIC::CGameLogic::Instance().RemoveValueCardOne(m_kHandCardData,m_kHandCardCout,cbCard);
		m_kHandCardCout --;
		SRLFLOGIC::CGameLogic::Instance().SortCardList(m_kHandCardData,m_kHandCardCout);
	}
	else
	{
		m_kHandCardCout -- ;
	}
	showHandCard();
}

void SRLFPlayer::setTing(bool isTing)
{
	m_isTing = isTing;
}

bool SRLFPlayer::getTing()
{
	return m_isTing;
}

bool SRLFPlayer::isTingCard2()
{
	if(isFirstCardSended())return false;
	LONGLONG LLOutCard[MAX_INDEX];
	memset(LLOutCard,0,sizeof(LLOutCard));
	BYTE cbCardIndex[MAX_INDEX];
	memset(cbCardIndex,0,sizeof(cbCardIndex));
	SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cbCardIndex);
	int maxNum = (int)SRLFLOGIC::CGameLogic::Instance().getAnyCardsCount(cbCardIndex);
	//打手上的牌，非当前摸牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE tempCbCardIndex[MAX_INDEX];
		memset(tempCbCardIndex,0,sizeof(tempCbCardIndex));
		memcpy(tempCbCardIndex,cbCardIndex,sizeof(cbCardIndex[0])*MAX_INDEX);
		int maxNum = (int)SRLFLOGIC::CGameLogic::Instance().getAnyCardsCount(tempCbCardIndex);
		if (maxNum%3 !=2)
		{
			if (tempCbCardIndex[i]>0 )
			{
				tempCbCardIndex[i]--;
				for (BYTE j=0;j<MAX_INDEX;j++)
				{
					BYTE temp2CbCardIndex[MAX_INDEX];
					memset(temp2CbCardIndex,0,sizeof(temp2CbCardIndex));
					memcpy(temp2CbCardIndex,tempCbCardIndex,sizeof(tempCbCardIndex[0])*MAX_INDEX);
					int maxNum = (int)SRLFLOGIC::CGameLogic::Instance().getAnyCardsCount(temp2CbCardIndex);
					if (maxNum%3!=2)
						temp2CbCardIndex[j]++;
					DWORD  dwChiHuKind = WIK_NULL;
					maxNum = (int)SRLFLOGIC::CGameLogic::Instance().getAnyCardsCount(temp2CbCardIndex);
					if (maxNum%3 == 2)
					{
						bool isHu =SRLFLOGIC::CGameLogic::Instance().IsTingCard(temp2CbCardIndex);
						if(isHu)
							LLOutCard[i]|=(LONGLONG(1)<<j);
						else
							LLOutCard[i]&=(~(0x1<<j));
					}
				}
			}
		}
	}
	for (int i=0;i<MAX_INDEX;i++)
	{
		if ( 0 != LLOutCard[i])
		{
			return true;
		}
	}
	return false;
}

bool SRLFPlayer::isTingCard()
{
	if(isFirstCardSended()) return false;
	LONGLONG LLOutCard[MAX_INDEX];
	memset(LLOutCard,0,sizeof(LLOutCard));
	BYTE cbCardIndex[MAX_INDEX];
	memset(cbCardIndex,0,sizeof(cbCardIndex));
	SRLFLOGIC::CGameLogic::Instance().SwitchToCardIndex(m_kHandCardData,m_kHandCardCout,cbCardIndex);
	int maxNum = (int)SRLFLOGIC::CGameLogic::Instance().getAnyCardsCount(cbCardIndex);
	//打手上的牌，非当前摸牌
	if (maxNum%3 !=2)
	{
		for (BYTE j=0;j<MAX_INDEX-7;j++)
		{
			BYTE temp2CbCardIndex[MAX_INDEX];
			memset(temp2CbCardIndex,0,sizeof(temp2CbCardIndex));
			memcpy(temp2CbCardIndex,cbCardIndex,sizeof(cbCardIndex[0])*MAX_INDEX);
			int maxNum = (int)SRLFLOGIC::CGameLogic::Instance().getAnyCardsCount(temp2CbCardIndex);
			if (maxNum%3!=2)
				temp2CbCardIndex[j]++;
			DWORD  dwChiHuKind = WIK_NULL;
			maxNum = (int)SRLFLOGIC::CGameLogic::Instance().getAnyCardsCount(temp2CbCardIndex);
			bool isHu = SRLFLOGIC::CGameLogic::Instance().IsTingCard(temp2CbCardIndex);
			if(isHu) 
			{
				return true;
			}
		}
	}
	return false;
}


void SRLFPlayer::setFirstCardSended(bool bFirstSended)
{
	m_bFirstSended = bFirstSended;
}
bool SRLFPlayer::isFirstCardSended()
{
	return m_bFirstSended;
}
