#include "SRLFGameScence.h"
#include "SRLFPlayer.h"
#include "Game/GameLib.h"
#include "CMD_SRLF.h"
#include "SRLFPlayer.h"

class SRLFTouchScence
	:public cocos2d::CCLayer
{
public:
	virtual bool init()
	{
		if ( !cocos2d::Layer::init() )
		{
			return false;
		}
		setTouchEnabled(true);
		setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
		return true;
	}
public:
	CREATE_FUNC(SRLFTouchScence);
public:
	virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return false;
		}
		if (pTouch->getID() != 0)
		{
			return false;
		}
		return SRLFGameScence::Instance().ccTouchBegan(pTouch->getLocation());
	}
	virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return SRLFGameScence::Instance().ccTouchMoved(pTouch->getDelta());
	}
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return SRLFGameScence::Instance().ccTouchEnded(pTouch->getLocation());
	}
};

void SRLFGameScence::initTouch()
{
	SRLFTouchScence* pTouchScence = new SRLFTouchScence;
	pTouchScence->init();
	addChild(pTouchScence);
}

bool SRLFGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!IsInGame())
	{
		return false;
	}
	if (m_nGameState == SRLF_STATE_NULL || m_nGameState == SRLF_STATE_READY)
	{
		return false;
	}
	else if (m_nGameState == SRLF_STATE_HUANPAI)
	{
		onHuanPaiTouchEvent(kPos);
		return false;
	}
	else if (m_nGameState == SRLF_STATE_PLAYING)
	{
		if(m_pLocal->GetChairID() != m_iCurrentUser)
		{
			return false;
		}
		else
		{
			m_pTouchCardNode = m_pLocal->getTouchCardNode(kPos);
			if (!m_pTouchCardNode)
			{
				m_pLocal->seatDownCard();
				return false;
			}
			cocos2d::Color3B kColor = m_pTouchCardNode->getDisplayedColor();
			if ( kColor == cocos2d::Color3B::GRAY)
			{
				return false;
			}
			m_kTouchSrcPos = m_pTouchCardNode->getPosition();
			if (m_kTouchSrcPos.y == SRLFPlayer::CARD_UP_POSY)
			{
				if (m_bselectGangState)
				{
					SRLF::CMD_C_OperateCard OperateCard;
					OperateCard.cbOperateCode=0x10;
					OperateCard.cbOperateCard = m_pLocal->getTouchCardVlaue(m_pTouchCardNode);
					SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
					m_bselectGangState =false;
					WidgetFun::setVisible(this,"SelfActionNode",false);
				}
				else
				{
					SendOutCard(m_pTouchCardNode);
				}
				return false;
			}
			m_pLocal->ClickCard(m_pTouchCardNode);
		}
	}
	return true;
}
void SRLFGameScence::ccTouchMoved(cocos2d::Vec2 kPos)
{
	if(m_pLocal->isTouchCardNode(m_pTouchCardNode)== false)return;
	if(m_pTouchCardNode == NULL )return;
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	m_pTouchCardNode->setPosition(kTempPos+kPos);
}
void SRLFGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	if(m_pLocal->isTouchCardNode(m_pTouchCardNode)== false)return;
	if(m_pTouchCardNode == NULL)return;
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	if (kTempPos.y != SRLFPlayer::CARD_UP_POSY || kTempPos.x !=  m_kTouchSrcPos.x)
	{
		m_pTouchCardNode->setPosition(m_kTouchSrcPos);
	}
	if (kTempPos.y > SRLFPlayer::CARD_UP_POSY)
	{
		if (m_bselectGangState)
		{
			SRLF::CMD_C_OperateCard OperateCard;
			OperateCard.cbOperateCode=0x10;
			OperateCard.cbOperateCard = m_pLocal->getTouchCardVlaue(m_pTouchCardNode);
			SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));
			WidgetFun::setVisible(this,"SelfActionNode",false);
			m_bselectGangState =false;
		}
		else
		{
			SendOutCard(m_pTouchCardNode);
		}
	}
}

void SRLFGameScence::SendOutCard(cocos2d::Node* pCard)
{
	ASSERT(pCard && m_nGameState == SRLF_STATE_PLAYING);
	if (pCard == NULL || m_nGameState != SRLF_STATE_PLAYING ) return;

	SRLF::CMD_C_OutCard OutCard;
	OutCard.cbCardData = m_pLocal->getTouchCardVlaue(pCard);
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
}

void SRLFGameScence::onHuanPaiTouchEvent(cocos2d::Vec2 kPos)
{
	std::vector<BYTE> kCardDataList;
	int nColor = m_pLocal->getUpCardList(kCardDataList);
	int nUpCardNum = kCardDataList.size();

	if (nColor != CARD_COLOR_NULL)
	{
		m_pLocal->blackHandCardByColor(nColor);
	}
	else
	{
		m_pLocal->blackAllHandCard(false);
	}

	Node* pTouchCard = m_pLocal->getTouchCardNode(kPos);
	if (pTouchCard && pTouchCard->getDisplayedColor() == cocos2d::Color3B::GRAY)
	{
		return;
	}
	if (nUpCardNum >= SRLFPlayer::HUAN_CARD_NUM)
	{
		//站起的牌满了3个后，其他牌不能站起

		if ( pTouchCard && pTouchCard->getPositionY() == SRLFPlayer::CARD_UP_POSY)
		{
			m_pLocal->ClickCard(pTouchCard);
		}
	}
	else if (pTouchCard)
	{
		m_pLocal->ClickCard(pTouchCard);
	}

	kCardDataList.clear();
	nColor = m_pLocal->getUpCardList(kCardDataList);
	if (nColor != CARD_COLOR_NULL)
	{
		m_pLocal->blackHandCardByColor(nColor);
	}
	else
	{
		m_pLocal->blackAllHandCard(false);
	}
}

