#include "TLJPlayer_Card.h"
#include "TLJPlayer_Panel.h"
#include "TLJAnimation.h"
#include "TLJGameLogic.h"
#include "TLJGameScence.h"
#include "GAME.h"
#include UTILITY_WIDGET

using namespace TLJ;
FV_SINGLETON_STORAGE(TLJPlayer_Card);
TLJPlayer_Card::TLJPlayer_Card()
{
	init();
}
TLJPlayer_Card::~TLJPlayer_Card(){

}
bool TLJPlayer_Card::init()
{
	if(!cocos2d::Node::init())
	{
		return false;
	}
	WidgetScenceXMLparse playerCard("Game/TLJ/Script/TLJPlayerCard.xml", this);
	WidgetScenceXMLparse chipContainer("Game/TLJ/Script/TLJChipContainer.xml", this);
	return true;
}

void TLJPlayer_Card::show()
{
	setVisible(true);
}

void TLJPlayer_Card::hide()
{
	setVisible(false);
}

void TLJPlayer_Card::showCard(TLJPlayer& player,bool afterOpterate)
{
	if(player.GetUserStatus()<US_PLAYING){
		return;
	}

	word visioChairID = player.getVisioChairID();
	if (visioChairID >= TLJGameScence::MAX_PLAYER) {
		return;
	}

	HandCard handCard = player.getHandCard();
	if(handCard.cbCardCount==0){
		return;
	}
	//玩家状态
	word playerState = player.getState();

	Node* pRootNode = WidgetFun::getChildWidget(this,utility::toString("HandCardNode_",visioChairID));
	pRootNode->removeAllChildren();
	pRootNode->setVisible(true);
	bool isOwn = player.GetUserID() == UserInfo::Instance().getUserID();
	Vec2 startPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pRootNode,"StartPos"));
	Vec2 addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pRootNode,"AddPos"));
	for(byte index = 0; index <	TLJ_MAX_COUNT; index++){
		Node* pNode = isOwn ? WidgetManager::Instance().createWidget("TLJCard",pRootNode):WidgetManager::Instance().createWidget("TLJCardOther",pRootNode);
		setCardImagic(pNode,handCard.cbCardData[index],isOwn);
		pNode->setPosition(startPos+addPos*index);
		if((playerState==PLAYER_STATE_QI||playerState==PLAYER_STATE_COMPARE_LOSE)&&handCard.cbCardData[index]>0){
			pNode->updateDisplayedColor(Color3B::GRAY);
		}
	}
}

void TLJPlayer_Card::hideCard(word visioChairID)
{
	if (visioChairID >= TLJGameScence::MAX_PLAYER) {
		return;
	}
	WidgetFun::setVisible(WidgetFun::getChildWidget(this,utility::toString("HandCardNode_",visioChairID)),false);
}

void TLJPlayer_Card::setCardImagic(Node* pNode,byte cbCardData,bool isOwn)
{
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	std::string name = "";
	if(cbCardData==0){
		//name = isOwn ? "Back":"Back1";
		name = "Back";
	}else if(cbCardData==99){
		name = "Invalid";
	}else{
		name = TLJ::TLJGameLogic::Instance().GetShowCardValue(cbCardData);
	}
	name = utility::toString(kImagic + name + ".png");
	WidgetFun::setImagic(pNode,name,true);
}

void TLJPlayer_Card::PlayerGiveUp(TLJPlayer& player)
{
	//word visioChairID = player.getVisioChairID();
	//Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("HandCardNode_",visioChairID));
	//TLJAnimation::Instance().runGiveUpAnimation(*pCardNode);
	showCard(player,true);
}

Node* TLJPlayer_Card::GetPlayerCardNode(word wVisioChairID)
{
	Node* pCardNode = WidgetFun::getChildWidget(this,utility::toString("HandCardNode_",wVisioChairID));
	return pCardNode;
}
//获得选中的节点
cocos2d::Node* TLJPlayer_Card::getTouchCardNode(cocos2d::Vec2 kTouchPos)
{
	Node* pNode = WidgetFun::getChildWidget(this,utility::toString("HandCardNode_",0));
	for (int i = 0;i<pNode->getChildrenCount();i++)
	{
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pNode->getChildren().at(i));
		if (pSprite == NULL )
		{
			continue;
		}
		if (utility::IsSpriteInTouch(kTouchPos,pSprite))
		{
			return pSprite;
		}
	}
	return NULL;
}