#include "TLJCalculatePanel.h"
#include "TLJCalculateDetailPanel.h"
#include "../TLJGameScence.h"
#include "Game/GameLib.h"
#include "time.h"

FV_SINGLETON_STORAGE(TLJCalculatePanel);
TLJCalculatePanel::TLJCalculatePanel()
{
	init();
}

TLJCalculatePanel::~TLJCalculatePanel()
{

}

bool TLJCalculatePanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	WidgetScenceXMLparse calculatePanel("Game/TLJ/Script/TLJCalculatePanel.xml", this);

	initButton();

	return true;
}

void TLJCalculatePanel::initButton()
{
	WidgetManager::addButtonCB("Btn_Close",this,button_selector(TLJCalculatePanel::Close));
	WidgetManager::addButtonCB("Btn_Back",this,button_selector(TLJCalculatePanel::Close));
	WidgetManager::addButtonCB("Btn_Share",this,button_selector(TLJCalculatePanel::Btn_Share));
}

void TLJCalculatePanel::Btn_Share(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{

}

void TLJCalculatePanel::Close(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{
	TLJGameScence::Instance().BackHomeScene();
}

void TLJCalculatePanel::showCalculateList(WORD wPlayerCount)
{
	auto pList = WidgetFun::getChildWidget(this,"CalculateItemList");
	auto pContainerNode = WidgetFun::getChildWidget(pList,"ListContainer");
	addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pList,"AddPos"));
	Vec2 itemSize = utility::parsePoint(WidgetFun::getWidgetUserInfo(pContainerNode,"ItemSize"));
	WORD cbMaxCount = wPlayerCount;
	Size contentSize = Size(itemSize.x,utility::Abs(addPos.y)*cbMaxCount);

	startPos = Vec2(contentSize.width/2,contentSize.height);
	pContainerNode->removeAllChildren();
	pContainerNode->setContentSize(contentSize);
}

void TLJCalculatePanel::showCalculateItem(TLJPlayer& player,WORD wIndex)
{
	auto pList = WidgetFun::getChildWidget(this,"CalculateItemList");
	auto pContainerNode = WidgetFun::getChildWidget(pList,"ListContainer");
	
	Node* pNode = WidgetManager::Instance().createWidget("CalculateItem",pContainerNode);
	WidgetFun::setText(pNode,"Name",player.GetNickName());
	WidgetFun::setText(pNode,"RoomId",TLJGameScence::Instance().m_RoomInfo.dwRoomNum);

	time_t tt = time(NULL);
	tm* t= localtime(&tt);
	std::string time = utility::toString(t->tm_hour,":",t->tm_min,":",t->tm_sec);
	WidgetFun::setText(pNode,"Time",time);
	WidgetFun::setText(pNode,"PlayCount",TLJGameScence::Instance().m_RoomInfo.dwPlayTotal);
	WidgetFun::setText(pNode,"TotalScore",player.getRoomScore());
	Vec2 pos = startPos+wIndex*addPos;
	pNode->setPosition(pos);

	showPlayerAvatar(player,pNode);
}

void TLJCalculatePanel::showPlayerAvatar(TLJPlayer& player,Node* pRootNode)
{
	auto playerAvatarNode = WidgetFun::getChildWidget(pRootNode, "TLJCalculateHead_Avatar");
	auto pClip = cocos2d::ClippingNode::create();
	auto sprite = cocos2d::Sprite::create("Game/TLJ/TLJPlayer/avatar_male.png");
	auto stencil = cocos2d::Sprite::create("Game/TLJ/CalculatePanel/Mask_CalculateHead.png");
	pClip->setStencil(stencil);
	pClip->addChild(sprite);
	pClip->setInverted(false);

	Size targetSize = stencil->getContentSize();;
	auto viewSize = sprite->getContentSize();

	auto scaleHeight = targetSize.height * 1.f / viewSize.height;
	auto scaleWidth = targetSize.width * 1.f / viewSize.width;
	auto targetScale = utility::Max(scaleWidth, scaleHeight);
	sprite->setScale(targetScale);

	if(playerAvatarNode != nullptr) {
		playerAvatarNode->removeAllChildren();
		playerAvatarNode->addChild(pClip);
	}

	ImagicDownManager::Instance().addDown(sprite, player.GetHeadHttp(), player.GetUserID());
}

void TLJCalculatePanel::showWinner(TLJPlayer& player)
{
	Node* pRootNode = WidgetFun::getChildWidget(this,"Winner");
	WidgetFun::setText(pRootNode,"Name",player.GetNickName());

	showPlayerAvatar(player,pRootNode);
}

void TLJCalculatePanel::showTotal(WORD wPlayerCount)
{
	show();
	showCalculateList(wPlayerCount);
}

void TLJCalculatePanel::show()
{
    setVisible(true);
}

void TLJCalculatePanel::hide()
{
    setVisible(false);
}