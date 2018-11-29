#include "TLJChipContainer.h"
#include "../TLJGameScence.h"
#include "../Player/TLJPlayer_Panel.h"
#include "../Animation/TLJAnimation.h"

FV_SINGLETON_STORAGE(TLJChipContainer);
TLJChipContainer::TLJChipContainer()
{
	init();
}

TLJChipContainer::~TLJChipContainer()
{
}

bool TLJChipContainer::init()
{
	if(!cocos2d::Node::init()){
		return false;
	}
	WidgetScenceXMLparse chipContainer("Game/TLJ/Script/TLJChipContainer.xml", this);
	return true;
}
//Ìí¼Ó³ïÂë
void TLJChipContainer::addChip(BYTE cbScoreLevel,WORD wTimes,WORD chipCount,DWORD dwBaseScore)
{
	show();

//	DWORD dwBaseScore = TLJGameScence::Instance().getBaseScore();
	int iScore = dwBaseScore*wTimes*(int)cbScoreLevel;
	int iTempScore = iScore;
	WORD count = 0;
	while (iTempScore!=0)
	{
		iTempScore = iTempScore/10;
		count++;
	}
	int iFontSize = 34-count*3;
	if(iFontSize<=0) iFontSize=1; 

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"chipContainer");

	for (int i = 0; i < chipCount; i++)
	{
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("TLJChip", pRootNode);
		std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
		WidgetFun::setImagic(pNode,"TLJChip_Img",utility::toString(kImagic,wTimes,"_",(int)cbScoreLevel,".png"),true);

		auto txtNode = WidgetFun::getChildWidget(pNode,"TLJChip_Txt");
		WidgetFun::setText(txtNode,utility::toString(iScore));
		WidgetManager::Instance().changeTxtProperty(txtNode,"FontSize",utility::toString(iFontSize));

		Vec2 pos = Vec2(570 + 18 * random(0,10),320 + 10 * random(0,10));
		pNode->setPosition(pos);
	}
}
//Ìí¼Ó³ïÂë
void TLJChipContainer::addChip(TLJPlayer& player,BYTE cbScoreLevel,WORD wTimes)
{
	show();
	//wTimes = player.getState()==PLAYER_STATE_AN?1:wTimes;
	Vec2 pos = TLJPlayer_Panel::Instance().getPlayerPosition(player);
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"chipContainer");
	cocos2d::Node* pNode = WidgetManager::Instance().createWidget("TLJChip", pRootNode);
	std::string kImagic = WidgetFun::getWidgetUserInfo(pNode,"Imagic");
	WidgetFun::setImagic(pNode,"TLJChip_Img",utility::toString(kImagic,wTimes,"_",(int)cbScoreLevel,".png"),true);
	DWORD dwBaseScore = TLJGameScence::Instance().getBaseScore();

	auto txtNode = WidgetFun::getChildWidget(pNode,"TLJChip_Txt");
	int iScore = dwBaseScore*wTimes*(int)cbScoreLevel;
	WidgetFun::setText(txtNode,utility::toString(iScore));
	WORD count = 0;
	while (iScore!=0)
	{
		iScore = iScore/10;
		count++;
	}
	int iFontSize = 34-count*3;
	if(iFontSize<=0) iFontSize=1; 
	WidgetManager::Instance().changeTxtProperty(txtNode,"FontSize",utility::toString(iFontSize));

	pNode->setPosition(pos); 
	TLJAnimation::Instance().runBettingAnimation(*pNode);
}


//ÇåÀí³ïÂë
void TLJChipContainer::clearChips()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this, "chipContainer");
	pRootNode->removeAllChildren();
}
//³ïÂëÈÝÆ÷
Node* TLJChipContainer::getChipContainer()
{
	Node* pNode = WidgetFun::getChildWidget(this,"chipContainer");
	return pNode;
}

void TLJChipContainer::hide()
{
	clearChips();
	setVisible(false);
}
void TLJChipContainer::show()
{
	setVisible(true);
}