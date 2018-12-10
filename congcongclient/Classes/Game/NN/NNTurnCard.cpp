#include "NNTurnCard.h"
#include "Game/NN/CMD_NN.h"
#include "Game/NN/TurnCardAction/CardTurnAction.h"
#include "Game/NN/TurnCardAction/TurnCardNode.h"
#include "Game/Script/WindowInfo.h"
#include "Game/NN/NNGameScene.h"
#include "Game/NN/NNGameLogic.h"
#include "Game/Script/TimeManager.h"

using namespace TTLNN;
FV_SINGLETON_STORAGE(NNTurnCard);

void* glTurnCardFinish(void* pram)
{
	NNTurnCard::Instance().delayHide();
	return nullptr;
}


NNTurnCard::NNTurnCard()
	:m_pTurnCard(NULL)
{
    init();
}

NNTurnCard::~NNTurnCard()
{

}

bool NNTurnCard::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }
    initLayout();
    initButton();

    return true;
}

void NNTurnCard::initLayout()
{
    WidgetScenceXMLparse NNTurnCardxml("TTLNN/Script/NNTurnCard.xml", this);
}

void NNTurnCard::initButton()
{
	//WidgetManager::addButtonCB("NNTurnCard_ButtonClose", this, button_selector(NNTurnCard::Button_Close));
}

void NNTurnCard::show()
{
    setVisible(true);
}

void NNTurnCard::hide()
{
    setVisible(false);
	//removeTurnCard();
}

void NNTurnCard::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}
void NNTurnCard::removeTurnCard()
{
	if(NULL!=m_pTurnCard)
		m_pTurnCard->removeFromParent();
	m_pTurnCard =NULL;
}

void NNTurnCard::createTurnCard(int cardIndex)
{
	removeTurnCard();
	show();
	Size visibleSize = WindowInfo::Instance().getUseSize();// Director::getInstance()->getVisibleSize();
	m_pTurnCard = TurnCardNode::create();
	auto pNode = WidgetFun::getChildWidgetByName(this,"NNTurnCardNode");
	pNode->addChild(m_pTurnCard);
	//m_pTurnCard->setPosition(0,0);
	std::string strBackPoker = utility::toString("TTLNN/CardTurn/poker_",0,".material");
	std::string strFrontPoker = utility::toString("TTLNN/CardTurn/poker_",cardIndex,".material");
	std::string strCylinderPoker = utility::toString("TTLNN/CardTurn/poker_",cardIndex,"_cylinder.material");
	std::string strFrontPokerEnd = utility::toString("TTLNN/CardTurnEnd/poker_",cardIndex,".material");
	m_pTurnCard->setTurnCardData(
		strBackPoker,
		strFrontPoker,
		strFrontPokerEnd,
		strCylinderPoker,
		Vec2(visibleSize.width/2 ,visibleSize.height/2),
		8,
		glTurnCardFinish);
		NNPlayerCard_Entity playerCards = NNGameScene::Instance().getLocalPlayer()->getPlayerCards();
		for (int index=0;index<4;index++)
		{
			auto pTempNode =WidgetFun::getChildWidgetByName(this,utility::toString("CardImg_",index));
			WidgetFun::setImagic(pTempNode,utility::toString(WidgetFun::getWidgetUserInfo(pTempNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"),false);
			pTempNode->setVisible(false);
		}
}

void NNTurnCard::delayHide()
{
	auto funBack = cocos2d::CCCallFunc::create([=](){
		NNGameScene::Instance().sendShowCard();
		hide();
	});
	cocos2d::CCAction * seq1 = cocos2d::CCSequence::create( 
		cocos2d::CCDelayTime::create(0.5f),
		funBack,
		NULL);
	runAction(seq1);
}

