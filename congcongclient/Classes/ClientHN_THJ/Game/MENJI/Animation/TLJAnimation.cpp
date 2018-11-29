#include "TLJAnimation.h"
#include "Game/GameLib.h"
#include "../Player/TLJPlayer_Panel.h"
#include "../TLJGameScence.h"
#include "../Player/TLJPlayer_Card.h"
#include "../ChipContainer/TLJChipContainer.h"

FV_SINGLETON_STORAGE(TLJAnimation);
TLJAnimation::TLJAnimation()
{
	init();
}

TLJAnimation::~TLJAnimation()
{
}

bool TLJAnimation::init()
{
	if(!Node::init()){
		return false;
	}
	WidgetScenceXMLparse animation("Game/TLJ/Script/TLJAnimation.xml",this);

	return true;
}
//播放下注动画
void TLJAnimation::runBettingAnimation(Node& pChip)
{
	pChip.setScale(0.1f);
	ScaleTo* pScaleTo = ScaleTo::create(0.5f,1.0f,1.0f);
	Vec2 endPos = Vec2(570 + 18 * random(0,10),320 + 10 * random(0,10));
	MoveTo* pMoveTo = MoveTo::create(0.5f,endPos);
	RotateBy* pRotateBy = RotateBy::create(0.5f,Vec3(0,0,random(-720,720)));
	Spawn* pActions = Spawn::create(pScaleTo,pMoveTo,pRotateBy,NULL);
	Sequence* pSeq = Sequence::create(pActions,NULL);
	pChip.runAction(pSeq);

}
//播放弃牌动画
void TLJAnimation::runGiveUpAnimation(Node& pCardNode)
{
	Vec2 desPos = Vec2(640,640);

	WORD wCardCount = pCardNode.getChildrenCount();
	for (WORD i = 0;i<wCardCount;i++)
	{
		Sprite* pCard = dynamic_cast<Sprite*>(pCardNode.getChildren().at(i));
		CCDelayTime* pDelay = CCDelayTime::create(0.15f*i);
		MoveTo* pMoveTo = MoveTo::create(0.5f,desPos);
		RotateBy* pRotateBy = RotateBy::create(0.5f,Vec3(0,0,360));
		Spawn* pActions = Spawn::create(pMoveTo,pRotateBy,NULL);
		CallFunc* pCallBack = CallFunc::create([pCard]{
			TLJAnimation::Instance().giveUpAniCallBack(*pCard);
		});
		Sequence* pSeq = Sequence::create(pDelay,pActions,pCallBack,NULL);
		pCard->runAction(pSeq);
	}
}
//弃牌动画 回调
void TLJAnimation::giveUpAniCallBack(Node& pCard)
{
	pCard.removeFromParent();
}


//比牌动画  比牌玩家进场
void TLJAnimation::runCompareAnimation(TLJPlayer& player,TLJPlayer& player_2,bool bResult)
{
	WidgetFun::setVisible(this,"TLJAnimation_Bg",true);
	show();
	Node* pRootNode = WidgetFun::getChildWidget(this,"TLJ_CompareAni");
	WidgetFun::setVisible(pRootNode,true);
	WidgetFun::setVisible(pRootNode,"Ani",false);
	//左侧玩家
	Node* leftPlayer = WidgetFun::getChildWidget(pRootNode,"LeftPlayer");
	WidgetFun::setVisible(leftPlayer,"ResultLogo",false);
	leftPlayer->updateDisplayedColor(Color3B::WHITE);
	Vec2 leftSrcPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(leftPlayer,"SrcPos"));
	Vec2 leftDestPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(leftPlayer,"DestPos"));
	leftPlayer->setPosition(leftSrcPos);

	auto leftName = WidgetFun::getChildWidget(leftPlayer,"NameText");
	auto leftScore = WidgetFun::getChildWidget(leftPlayer,"ScoreText");
	WidgetFun::setText(leftName,player.GetNickName());
	int score = player.GetUserScore();
	WidgetFun::setText(leftScore,utility::toString(score));
	showComparePlayerAvatar(player,"LeftPlayer");

	CCActionInterval* moveLeft = CCMoveTo::create(0.7f, leftDestPos);
	CCActionInterval* easeInLeft= CCEaseExponentialOut::create(moveLeft);
	leftPlayer->runAction(easeInLeft);

	//右侧玩家
	Node* rightPlayer = WidgetFun::getChildWidget(pRootNode,"RightPlayer");
	WidgetFun::setVisible(rightPlayer,"ResultLogo",false);
	rightPlayer->updateDisplayedColor(Color3B::WHITE);
	Vec2 rightSrcPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(rightPlayer,"SrcPos"));
	Vec2 rightDestPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(rightPlayer,"DestPos"));
	rightPlayer->setPosition(rightSrcPos);

	auto rightName = WidgetFun::getChildWidget(rightPlayer,"NameText");
	auto rightScore = WidgetFun::getChildWidget(rightPlayer,"ScoreText");
	WidgetFun::setText(rightName,player_2.GetNickName());
	int score_2 = player_2.GetUserScore();
	WidgetFun::setText(rightScore,utility::toString(score_2));
	showComparePlayerAvatar(player_2,"RightPlayer");

	CCActionInterval* moveRight = CCMoveTo::create(0.7f, rightDestPos);
	CCActionInterval* easeInRight= CCEaseExponentialOut::create(moveRight);
	rightPlayer->runAction(easeInRight);

	DelayTime* pDelay = DelayTime::create(0.5f);
	CallFunc* pCallBack = CallFunc::create([&player,&player_2,bResult]{
		TLJAnimation::Instance().runCompareAnimation_2(player,player_2,bResult);
	});
	Sequence* sequence = Sequence::create(pDelay,pCallBack,NULL);
	runAction(sequence);

}
//比牌动画  比牌特效
void TLJAnimation::runCompareAnimation_2(TLJPlayer& player,TLJPlayer& player_2,bool bResult)
{
	Node* pRootNode = WidgetFun::getChildWidget(this,"TLJ_CompareAni");
	Node* pNode = WidgetFun::getChildWidget(pRootNode,"Ani");
	pNode->setVisible(true);
	WORD wCount = utility::parseInt(WidgetFun::getWidgetUserInfo(pNode,"MaxCount"));

	WidgetFun::setVisible(pNode,true);
	Sprite* pSprite = dynamic_cast<Sprite*>(pNode);

	auto animation = Animation::create();
	for (int i=1; i<wCount; i++)
	{
		char szName[100] = {0};
		sprintf(szName, "Game/TLJ/Animation/Compare/CompareAni_%02d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	animation->setDelayPerUnit(1.0f / 10);			
	animation->setRestoreOriginalFrame(false);      
	auto animate = Animate::create(animation);

	CallFunc* pCallBack = CallFunc::create([&player,&player_2,bResult]{
		TLJAnimation::Instance().runCompareAnimation_3(player,player_2,bResult);
	});

	Sequence* sequence = Sequence::create(animate,pCallBack,NULL);
	pSprite->runAction(sequence);
}
//比牌动画  输赢效果
void TLJAnimation::runCompareAnimation_3(TLJPlayer& player,TLJPlayer& player_2,bool bResult)
{
	Node* pRootNode = WidgetFun::getChildWidget(this,"TLJ_CompareAni");
	Node* pNode = WidgetFun::getChildWidget(pRootNode,"Ani");
	pNode->setVisible(false);

	Node* rightPlayer = WidgetFun::getChildWidget(pRootNode,"RightPlayer");
	Node* rightLogo = WidgetFun::getChildWidget(rightPlayer,"ResultLogo");
	rightLogo->setVisible(true);

	Node* leftPlayer = WidgetFun::getChildWidget(pRootNode,"LeftPlayer");
	Node* leftLogo = WidgetFun::getChildWidget(leftPlayer,"ResultLogo");
	leftLogo->setVisible(true);

	std::string winImg = WidgetFun::getWidgetUserInfo(rightLogo,"winImagic");
	std::string loseImg = WidgetFun::getWidgetUserInfo(rightLogo,"loseImagic");
	if(bResult){
		WidgetFun::setImagic(leftLogo,winImg,true);
		WidgetFun::setImagic(rightLogo,loseImg,true);
	}
	else
	{
		WidgetFun::setImagic(rightLogo,winImg,true);
		WidgetFun::setImagic(leftLogo,loseImg,true);
	}

	DelayTime* pDelay = DelayTime::create(1.0f);
	CallFunc* pCallBack = CallFunc::create([]{
		TLJAnimation::Instance().CompareAnimationCallBack();
	});
	Sequence* pSeq = Sequence::create(pDelay,pCallBack,NULL);
	pNode->runAction(pSeq);
}
//比牌动画  回调函数
void TLJAnimation::CompareAnimationCallBack()
{
	WidgetFun::setVisible(this,"TLJAnimation_Bg",false);
	Node* pRootNode = WidgetFun::getChildWidget(this,"TLJ_CompareAni");
	pRootNode->setVisible(false);
	TLJPlayer_Panel::Instance().PlayerCompareEnd();
}

void TLJAnimation::showComparePlayerAvatar(TLJPlayer& player,std::string kValue)
{
	Node* pRootNode = WidgetFun::getChildWidget(this,"TLJ_CompareAni");
	Node* playerAvatarNode = WidgetFun::getChildWidget(WidgetFun::getChildWidget(pRootNode,kValue), "TLJPlayerHeader");

	auto pClip = cocos2d::ClippingNode::create();
	auto sprite = cocos2d::Sprite::create("Game/TLJ/TLJPlayer/avatar_male.png");
	auto stencil = cocos2d::Sprite::create("Game/TLJ/TLJPlayer/Avatar.png");
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

//开局动画
void TLJAnimation::runGameStartAnimation(TLJPlayer& player)
{
	Vec2 SrcPos = Vec2(640,640);
	WORD wVisioChairID = player.getVisioChairID();
	Node* pCardNode = TLJPlayer_Card::Instance().GetPlayerCardNode(wVisioChairID);
	for(WORD index = 0; index < TLJ_MAX_COUNT; index++)
	{
		Sprite* pCard = dynamic_cast<Sprite*>(pCardNode->getChildren().at(index));
		Vec2 DestPos = pCard->getPosition();
		pCard->setPosition(SrcPos);
		pCard->setVisible(true);
		pCard->setScale(0.4f);
		ScaleTo* pScaleTo = ScaleTo::create(0.5f,1.0f,1.0f);
		CCDelayTime* pDelay = CCDelayTime::create(0.3f*index);
		MoveBy* pMoveBy = MoveBy::create(0.5f,DestPos-SrcPos);
		RotateBy* pRotateBy = RotateBy::create(0.5f,360);
		Spawn* pActions = Spawn::create(pMoveBy,pRotateBy,pScaleTo,NULL);
		Sequence* pSeq = Sequence::create(pDelay,pActions,NULL);
		pCard->runAction(pSeq);
	}
}

//结束动画
void TLJAnimation::runGameEndAnimation(TLJPlayer& player)
{
	Vec2 DestPos = TLJPlayer_Panel::Instance().getPlayerPosition(player);
	Node* pRootNode = TLJChipContainer::Instance().getChipContainer();
	WORD wCount = pRootNode->getChildrenCount();
	float fDelayTimeCell = 0.3f/wCount;
	for(WORD index = 0; index < wCount; index ++ ){
		Node* pChip = dynamic_cast<Node*>(pRootNode->getChildren().at(index));
		CCDelayTime* pDelay = CCDelayTime::create(fDelayTimeCell*index+0.5f);
		MoveTo* pMoveTo = MoveTo::create(0.5f,DestPos);
		ScaleTo* pScaleTo = ScaleTo::create(0.3f,0.2f,0.2f);
		Spawn* pActions = Spawn::create(pMoveTo,pScaleTo,NULL);
		Sequence* pSeq = NULL;
		if(index == wCount-1){
			CallFunc* pCallBack = CallFunc::create([]{
				TLJAnimation::Instance().GameEndAniCallBack();
			});
			pSeq = Sequence::create(pDelay,pActions,pCallBack,NULL);
		}else{
			pSeq = Sequence::create(pDelay,pActions,NULL);
		}
		pChip->runAction(pSeq);
	}
}

void TLJAnimation::GameEndAniCallBack()
{
	TLJChipContainer::Instance().clearChips();
}

void TLJAnimation::hide()
{
	setVisible(false);
}
void TLJAnimation::show()
{
	setVisible(true);
}