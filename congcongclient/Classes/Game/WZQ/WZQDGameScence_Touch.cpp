#include "WZQDGameScence.h"
#include "WZQDPlayer.h"
#include "GAME.h"
#include "CMD_WZQ.h"
#include "WZQDPlayer.h"
#include UTILITY_WIDGET

class WZQTouchScence
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
	CREATE_FUNC(WZQTouchScence);
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
		return WZQDGameScence::Instance().ccTouchBegan(pTouch->getLocation());
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
		return WZQDGameScence::Instance().ccTouchMoved(pTouch->getDelta());
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
		return WZQDGameScence::Instance().ccTouchEnded(pTouch->getLocation());
	}
};

void WZQDGameScence::initTouch()
{
	WZQTouchScence* pTouchScence = new WZQTouchScence;
	pTouchScence->init();
	addChild(pTouchScence);
}

bool WZQDGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!IsInGame())
	{
		return false;
	}
	if (m_nGameState == WZQ_STATE_NULL || m_nGameState == WZQ_STATE_READY)
	{
		return false;
	}
	/*else if (m_nGameState == XZDD_STATE_HUANPAI)
	{
		onHuanPaiTouchEvent(kPos);
		return false;
	}*/
	else if (m_nGameState == WZQ_STATE_PLAYING)
	{
		//if(m_pLocal->GetChairID() != m_iCurrentUser)
		//{
		//	return false;
		//}
		//else
		//{
		//	m_pTouchCardNode = m_pLocal->getTouchCardNode(kPos);
		//	if (!m_pTouchCardNode)
		//	{
		//		m_pLocal->seatDownCard();
		//		return false;
		//	}
		//	cocos2d::Color3B kColor = m_pTouchCardNode->getDisplayedColor();
		//	if ( kColor == cocos2d::Color3B::GRAY)
		//	{
		//		return false;
		//	}
		//	m_kTouchSrcPos = m_pTouchCardNode->getPosition();
		//	if (m_kTouchSrcPos.y == WZQDPlayer::CARD_UP_POSY)
		//	{
		//		//SendOutCard(m_pTouchCardNode);
		//		return false;
		//	}
		//	else
		//	{
		//		m_pLocal->seatDownCard();
		//		m_pLocal->ClickCard(m_pTouchCardNode);
		//		return true;
		//	}
		//}
	}
	return false;
}
void WZQDGameScence::ccTouchMoved(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	m_pTouchCardNode->setPosition(kTempPos+kPos);
}
void WZQDGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	cocos2d::Vec2 kTempPos = m_pTouchCardNode->getPosition();
	if (kTempPos.y != WZQDPlayer::CARD_UP_POSY || kTempPos.x !=  m_kTouchSrcPos.x)
	{
		m_pTouchCardNode->setPosition(m_kTouchSrcPos);
	}
	if (kTempPos.y > WZQDPlayer::CARD_UP_POSY)
	{
		//SendOutCard(m_pTouchCardNode);//发送出牌信息
	}
}

//void WZQDGameScence::SendOutCard(cocos2d::Node* pCard)//出牌命令
//{
//	ASSERT(pCard && m_nGameState == XZDD_STATE_PLAYING);
//	if (pCard == NULL || m_nGameState != XZDD_STATE_PLAYING ) return;
//
//	CMD_C_OutCard OutCard;
//	OutCard.cbCardData = m_pLocal->getTouchCardVlaue(pCard);
//	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
//}
//
//void XZDDGameScence::onHuanPaiTouchEvent(cocos2d::Vec2 kPos)//换牌命令
//{
//	std::vector<byte> kCardDataList;
//	int nColor = m_pLocal->getUpCardList(kCardDataList);
//	int nUpCardNum = kCardDataList.size();
//
//	if (nColor != CARD_COLOR_NULL)
//	{
//		m_pLocal->blackHandCardByColor(nColor);
//	}
//	else
//	{
//		m_pLocal->blackAllHandCard(false);
//	}
//
//	Node* pTouchCard = m_pLocal->getTouchCardNode(kPos);
//	if (pTouchCard && pTouchCard->getDisplayedColor() == cocos2d::Color3B::GRAY)
//	{
//		return;
//	}
//	if (nUpCardNum >= XZDDPlayer::HUAN_CARD_NUM)
//	{
//		站起的牌满了3个后，其他牌不能站起
//
//		if ( pTouchCard && pTouchCard->getPositionY() == XZDDPlayer::CARD_UP_POSY)
//		{
//			m_pLocal->ClickCard(pTouchCard);
//		}
//	}
//	else if (pTouchCard)
//	{
//		m_pLocal->ClickCard(pTouchCard);
//	}
//
//	kCardDataList.clear();
//	nColor = m_pLocal->getUpCardList(kCardDataList);
//	if (nColor != CARD_COLOR_NULL)
//	{
//		m_pLocal->blackHandCardByColor(nColor);
//	}
//	else
//	{
//		m_pLocal->blackAllHandCard(false);
//	}
//}
//
