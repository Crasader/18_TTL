#include "TLJPlayer_Panel.h"
#include "../TLJGameScence.h"
#include "Game/GameLib.h"
#include "../Animation/TLJAnimation.h"

FV_SINGLETON_STORAGE(TLJPlayer_Panel);
TLJPlayer_Panel::TLJPlayer_Panel()
	:m_wInitiatorChairID(0)
	,m_wLoseID(0)
{
	init();
}

TLJPlayer_Panel::~TLJPlayer_Panel()
{
}

bool TLJPlayer_Panel::init()
{
	if(!cocos2d::Node::init()) {
		return false;
	}

	WidgetScenceXMLparse playerPanel("Game/TLJ/Script/TLJPlayerPanel.xml", this);

	for(int chairID = 0; chairID < TLJ_GAME_PLAYER; ++chairID) {
		WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", chairID))->setVisible(false);	
	}
	return true;
}
//显示玩家
void TLJPlayer_Panel::showPlayer(TLJPlayer& player)
{
	if(player.isValid()) {
		WORD visioChairID = player.getVisioChairID();
		cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID));
 		pRootNode->setVisible(true);
		pRootNode->setTag(visioChairID);
		auto playerName = WidgetFun::getChildWidget(pRootNode,"NameText");
		auto playerScore = WidgetFun::getChildWidget(pRootNode,"ScoreText");
		WidgetFun::setText(playerName,player.GetNickName());
		int score = player.GetUserScore();
		WidgetFun::setText(playerScore,utility::toString(score));
		//庄家标识
		showBanker(player,TLJGameScence::Instance().isBankerUser(player));
		//离线标识
		showOffline(player, player.GetUserStatus() == US_OFFLINE);
		//准备标识
		showReady(player, player.GetUserStatus() == US_READY);
		//看牌标识
		//showKanPai(player, player.getState() == PLAYER_STATE_MING);
		//弃牌标识
		//showQiPai(player, player.getState() == PLAYER_STATE_QI);
		//状态标识
		showStateIdentification(player);
		//单局投注数
		showCurrentMatchScore(player,player.getState() != PLAYER_STATE_NULL);
		//玩家头像
		showPlayerAvatar(player);
	}
}
//隐藏玩家
void TLJPlayer_Panel::hidePlayer(WORD chairID)
{
	WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", chairID))->setVisible(false);
}
//显示玩家头像
void TLJPlayer_Panel::showPlayerAvatar(TLJPlayer& player)
{
	WORD visioChairID = player.getVisioChairID();
	auto playerAvatarNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "TLJPlayerHeader");
	auto pClip = cocos2d::ClippingNode::create();
	auto sprite = cocos2d::Sprite::create("Game/TLJ/TLJPlayer/avatar_male.png");
	auto stencil = cocos2d::Sprite::create("GamePlaza/HomeScene/Avar.png");
	pClip->setStencil(stencil);
	pClip->addChild(sprite);
	pClip->setInverted(false);

	Size targetSize = Size(76,76);
	auto viewSize = sprite->getContentSize();
	if (viewSize.height < targetSize.height || viewSize.width < targetSize.width) {
		auto scaleHeight = targetSize.height * 1.f / viewSize.height;
		auto scaleWidth = targetSize.width * 1.f / viewSize.width;
		auto targetScale = utility::Max(scaleWidth, scaleHeight);
		sprite->setScale(targetScale);
	}

	if(playerAvatarNode != nullptr) {
		playerAvatarNode->removeAllChildren();
		playerAvatarNode->addChild(pClip);
	}

	ImagicDownManager::Instance().addDown(sprite, player.GetHeadHttp(), player.GetUserID());
}
//离线标识
void TLJPlayer_Panel::showOffline(TLJPlayer& player,bool bShow)
{
	WORD visioChairID = player.getVisioChairID();
	Node* pNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "PlayerOffline");
	WidgetFun::setVisible(pNode,bShow);
	if(!player.isSeatInLeft()){
		Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,"LeftPos"));
		pNode->setPosition(pos);
	}
}
//庄家标识
void TLJPlayer_Panel::showBanker(TLJPlayer& player,bool bShow)
{
	WORD visioChairID = player.getVisioChairID();
	Node* pNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "PlayerBanker");
	WidgetFun::setVisible(pNode,bShow);
	if(!player.isSeatInLeft()){
		Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,"LeftPos"));
		pNode->setPosition(pos);
	}
}
//看牌标识
//void TLJPlayer_Panel::showKanPai(TLJPlayer& player,bool bShow)
//{
//	WORD visioChairID = player.getVisioChairID();
//	Node* pNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "PlayerYiKan");
//	WidgetFun::setVisible(pNode,bShow);
//}
//弃牌标识
//void TLJPlayer_Panel::showQiPai(TLJPlayer& player,bool bShow)
//{
//	WORD visioChairID = player.getVisioChairID();
//	Node* pNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "PlayerYiQi");
//	WidgetFun::setVisible(pNode,bShow);
//}
//状态标识
void TLJPlayer_Panel::showStateIdentification(TLJPlayer& player)
{
	WORD visioChairID = player.getVisioChairID();
	Node* pRootNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "CurrentState");
	pRootNode->setVisible(true);
	Node* pStateTxt = WidgetFun::getChildWidget(pRootNode,"CurrentStateText");
	WORD wState = player.getState();
	switch (wState)
	{
	case PLAYER_STATE_MING:
		if (visioChairID != 0)
		{
			WidgetFun::setText(pStateTxt,utility::a_u8("已看牌"));
		}
		else
		{
			pRootNode->setVisible(false);
		}
		break;
	case PLAYER_STATE_QI:
		if (visioChairID != 0)
		{
			WidgetFun::setText(pStateTxt,utility::a_u8("已弃牌"));
		}
		else
		{
			pRootNode->setVisible(false);
		}
		break;
	case PLAYER_STATE_AN:
		if (visioChairID == 0)
		{
			WidgetFun::setText(pStateTxt,utility::a_u8("点击看牌"));
		}
		else
		{
			pRootNode->setVisible(false);
		}
		break;
	default:
		pRootNode->setVisible(false);
		break;
	}
	Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pRootNode,utility::toString("Pos_",visioChairID)));
	pRootNode->setPosition(pos);
	//WidgetFun::setVisible(pNode,bShow);
}
//单局投注数
void TLJPlayer_Panel::showCurrentMatchScore(TLJPlayer& player,bool bShow)
{
	auto visioChairID = player.getVisioChairID();
	Node* pNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "CurrentScoreNode");
	WidgetFun::setVisible(pNode,bShow);
	
	Node* pTxtNode = WidgetFun::getChildWidget(pNode,"CurrentScoreText");
	std::string kScore = utility::toString(player.getGameScore());
	WidgetFun::setText(pTxtNode,kScore);

	Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,utility::toString("Pos_",visioChairID)));
	pNode->setPosition(pos);
}
//准备标识
void TLJPlayer_Panel::showReady(TLJPlayer& player,bool bShow)
{
	auto visioChairID = player.getVisioChairID();
	Node* pNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID)), "Ready");
	WidgetFun::setVisible(pNode,bShow);
	if(player.isSeatInLeft()){
		Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,"LeftPos"));
		pNode->setPosition(pos);
	}
	if(visioChairID==0){
		TLJGameScence::Instance().refreshReadyStatus();
	}
}
//玩家坐标
Vec2 TLJPlayer_Panel::getPlayerPosition(TLJPlayer& player)
{
	WORD visioChairID = player.getVisioChairID();
	Node* pNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", visioChairID));
	Vec2 position = pNode->getPosition();
	return position;
}
//玩家比牌
void TLJPlayer_Panel::PlayerCompare(WORD wVisioChairID)
{
	m_wInitiatorChairID = wVisioChairID;
	m_wDestChairID = INVALID_CHAIR;
	for (WORD i = 1; i < TLJ_GAME_PLAYER; i ++ )
	{
		Node* pRootNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", i));
		Node* pNode = WidgetFun::getChildWidget(pRootNode, "TLJ_SelectFrame");
		auto pPlayer = TLJGameScence::Instance().getPlayerByVisioID(i);
		if(pPlayer==NULL)
		{
			continue;
		}
		WidgetFun::setVisible(pRootNode,"TLJ_SelectFrame",(pPlayer->getState()==PLAYER_STATE_AN||pPlayer->getState()==PLAYER_STATE_MING));
		
		Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode,utility::toString("Pos_",i)));
		pNode->setPosition(pos);
	}
	//WidgetFun::setVisible(this,"TLJPlayerPanel_Mask",true);
}
//玩家头像点击响应 （比牌）
void TLJPlayer_Panel::onPlayerTouched(WORD wVisioChairID)
{
	//状态校验
	WORD wState = TLJGameScence::Instance().getGameState();
	if(wState!=TLJ_STATUS_COMPARE){
		return;
	}
	if(wVisioChairID== m_wInitiatorChairID){
		return;
	}
	if(m_wDestChairID!=INVALID_CHAIR)
	{
		return;
	}
	m_wDestChairID = wVisioChairID;

	TLJPlayer* pPlayer = TLJGameScence::Instance().getPlayerByVisioID(wVisioChairID);
	if(pPlayer->getState()!=PLAYER_STATE_MING&&pPlayer->getState()!=PLAYER_STATE_AN)
	{
		return;
	}
	TLJGameScence::Instance().playerCompare(pPlayer->GetChairID());
}
//收到服务端消息后  开始播放比牌动画
//void TLJPlayer_Panel::PlayerCompare(WORD wVisioChairID_0,WORD wVisoChairID_1,bool bResult)
//{
//	m_wInitiatorChairID = wVisioChairID_0;
//	m_wLoseID = bResult?wVisoChairID_1:wVisioChairID_0;
//	Node* pInitiatorNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", wVisioChairID_0));
//	Node* pTargetNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", wVisoChairID_1));
//	TLJAnimation::Instance().runCompareAnimation(*pInitiatorNode,*pTargetNode,false);
//}
void TLJPlayer_Panel::PlayerCompare(TLJPlayer& player,TLJPlayer& player_2,bool bResult)
{
	if(bResult){
		m_wLoseID = player_2.getVisioChairID();
	}else{
		m_wLoseID = player.getVisioChairID();
	}
	TLJAnimation::Instance().runCompareAnimation(player,player_2,bResult);
}

//比牌结束
void TLJPlayer_Panel::PlayerCompareEnd()
{
	resetPanel();
	TLJPlayer* pPlayer = TLJGameScence::Instance().getPlayerByVisioID(m_wLoseID);
    if(pPlayer == NULL)
        return;
	pPlayer->PlayerGiveUp(PLAYER_STATE_COMPARE_LOSE);
	m_wInitiatorChairID = 0;
}

void TLJPlayer_Panel::resetPanel()
{
	TLJGameScence::Instance().setGameState(TLJ_STATUS_PLAYING);
	//WidgetFun::setVisible(this,"TLJPlayerPanel_Mask",false);
	for (WORD i = 0; i < TLJ_GAME_PLAYER; i ++ )
	{
		Node* pNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", i));
		auto pPlayer = TLJGameScence::Instance().getPlayerByVisioID(i);
		if(pPlayer==NULL)
		{
			continue;
		}
		WidgetFun::setVisible(pNode,"TLJ_SelectFrame",false);
	}
}
//获取点击玩家座位号
int TLJPlayer_Panel::getTouchPlayerNode(cocos2d::Vec2 kPos)
{
	int visioChairID = -1;
	auto avatar = Sprite::create("Game/TLJ/TLJPlayer/TLJ_SelectFrame.png");
	cocos2d::Size avatarSize = avatar->getContentSize();
	for (int chairID = 0; chairID < TLJGameScence::MAX_PLAYER; ++chairID) {
		auto playerNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", chairID));
		auto pNode = WidgetFun::getChildWidget(playerNode,"TLJ_SelectFrame");
		Vec2 pos = pNode->getPosition();
		if (!playerNode->isVisible())
			continue;
		cocos2d::Vec2 tPos = playerNode->convertToNodeSpace(kPos);
		if (((tPos.x - pos.x)* (tPos.x-pos.x) + (tPos.y -pos.y)* (tPos.y-pos.y)) < (avatarSize.width * avatarSize.height / 4))
			return chairID;
	}
	return visioChairID;
}
//玩家弃牌
void TLJPlayer_Panel::PlayerGiveUp(TLJPlayer& player)
{
	showPlayer(player);
	WORD visioChairID = player.getVisioChairID();
	changeMaskVisible(visioChairID,true);
}
//玩家头像遮罩 显示更改
void TLJPlayer_Panel::changeMaskVisible(WORD wVisioChairID,bool bVisible)
{
	Node* pNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", wVisioChairID));
	WidgetFun::setVisible(pNode,"Mask",bVisible);
}
//积分变动提示
void TLJPlayer_Panel::showAddGold(WORD wVisioChairID,int iGold)
{
	if(iGold==0)
		return;
	Node* pNode = WidgetFun::getChildWidget(this, utility::toString("TLJPlayerNode_", wVisioChairID));
	std::string kStr = utility::toString(iGold);
	if(iGold>0)
		kStr = utility::toString("+",kStr);
	WidgetFun::setText(pNode,"AddGoldTxt",kStr);
	WidgetFun::runWidgetAction(pNode,"AddGoldTxt","Start");
}
void TLJPlayer_Panel::showTimes(WORD wVisioChairID,int times)
{
	/*if(times==0){
		hideTimes();
		return;
	}*/
	int first = times / 10;
	int second = times % 10;
	auto pAlarmNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(this,utility::toString("TLJPlayerNode_", wVisioChairID)), "TLJPlayer_AlarmNode");
	pAlarmNode->stopAllActions();
	pAlarmNode->setVisible(true);
	if(wVisioChairID<3){
		Vec2 pos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pAlarmNode,"LeftPos"));
		pAlarmNode->setPosition(pos);
	}
	WidgetFun::setImagic(WidgetFun::getChildWidget(pAlarmNode, "TLJAlarm_Timer_0"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), first, ".png"), false);
	WidgetFun::setImagic(WidgetFun::getChildWidget(pAlarmNode, "TLJAlarm_Timer_1"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), second, ".png"), false);

	if(times > 0) {
		DelayTime* delay = DelayTime::create(1.f);
		CallFunc* func = CallFunc::create([ = ] {
			showTimes(wVisioChairID,times - 1);
		});
		Sequence* seq = Sequence::create(delay, func, NULL);
		pAlarmNode->runAction(seq);
	}
}
void TLJPlayer_Panel::hideTimes()
{
	for(int i=0;i<TLJGameScence::MAX_PLAYER;i++){
		Node* pNode = WidgetFun::getChildWidget(this,utility::toString("TLJPlayerNode_", i));
		if(WidgetFun::isWidgetVisble(this,utility::toString("TLJPlayerNode_", i))){
			auto pAlarmNode = WidgetFun::getChildWidget(pNode,"TLJPlayer_AlarmNode");
			pAlarmNode->stopAllActions();
			WidgetFun::setVisible(pAlarmNode,false);
		}
	}
}
//玩家显示节点
Node* TLJPlayer_Panel::getSeatNode(WORD wVisioChairID)
{
	return WidgetFun::getChildWidget(this,utility::toString("TLJPlayerNode_",wVisioChairID));
}