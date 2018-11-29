#include "SGPlayerCard.h"
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"
#include "ClientHN_THJ/Game/SG/GameLogic/SGGameLogic.h"
#include "ClientHN_THJ/Game/SG/GameScene/SGGameScence.h"
#include "ClientHN_THJ/Game/SG/Operator/SGOperator.h"

FV_SINGLETON_STORAGE(SGPlayerCard);
SGPlayerCard::SGPlayerCard()
{
    init();
}

SGPlayerCard::~SGPlayerCard()
{

}

#pragma region 初始化
bool SGPlayerCard::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();

    return true;
}

void SGPlayerCard::initLayout()
{
    WidgetScenceXMLparse playerCard("SG/Script/SGPlayerCard.xml", this);
}

#pragma endregion 初始化

#pragma region 显示与隐藏
void SGPlayerCard::show()
{
    setVisible(true);
}

void SGPlayerCard::hide()
{
    setVisible(false);
}

void SGPlayerCard::showPlayer(SGPlayer& player)
{
    WORD visioChairID = player.getVisioChairID();

    if(visioChairID < SG_GAME_PLAYER) {
        SGPlayerCard_Entity playerCards = player.getPlayerCards();

        if(player.isValid() && playerCards.isValid) {
            SG::SGCardType_Result result = player.getPlayerCardType();

            if(result.type == SG::SGCardType_Invalid) {
                auto pNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", visioChairID));
                pNode->setVisible(true);
                WidgetFun::setVisible(pNode, "PlayerCardReultNode", false);

                for(int index = 0; index < playerCards.cardCount; ++index) {
                    auto pCard = WidgetFun::getChildWidget(pNode, utility::toString("Card_", index));
                    pCard->setVisible(true);
                    WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), SGGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);

                    if(&player == SGGameScence::Instance().getLocalPlayer()) {
                        if(!SGGameScence::Instance().isSplitCard()) {
                            WidgetFun::getChildWidget(pNode, utility::toString("Card_", index))->setPositionY(CARD_DWON_POSY);
                            continue;
                        }

                        std::vector<int> selectCards = SGGameScence::Instance().getSelectCards();
                        std::vector<int>::iterator itor = find(selectCards.begin(), selectCards.end(), index);

                        if(itor != selectCards.end()) {
                            WidgetFun::getChildWidget(pNode, utility::toString("Card_", index))->setPositionY(CARD_UP_POSY);
                        } else {
                            WidgetFun::getChildWidget(pNode, utility::toString("Card_", index))->setPositionY(CARD_DWON_POSY);
                        }
                    }
                }

                for(int index = playerCards.cardCount; index < MAX_HAND_CARD; ++index) {
                    WidgetFun::setVisible(pNode, utility::toString("Card_", index), false);
                    WidgetFun::getChildWidget(pNode, utility::toString("Card_", index))->setPositionY(CARD_DWON_POSY);
                }
            } else if(result.type > SG::SGCardType_None && result.type < SG::SGCardType_DSG) {
                WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
                WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), false);
                WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), true);

                auto pNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCardResultHasNiu_", visioChairID));
                int cardIndex = 0;

                for(int index = 0; index < playerCards.cardCount; ++index) {
                   // if(result.isCardSelected[index]) {
                        auto pCard = WidgetFun::getChildWidget(pNode, utility::toString("Card_", cardIndex++));
                        pCard->setVisible(true);
                        WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), SGGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
                    //}
                }

               /* for(int index = 0; index < playerCards.cardCount; ++index) {
                    if(!result.isCardSelected[index]) {
                        auto pCard = WidgetFun::getChildWidget(pNode, utility::toString("Card_", cardIndex++));
                        pCard->setVisible(true);
                        WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), SGGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
                    }
                }*/
				if (result.type == SG::SGCardType_None )
				{
					WidgetFun::setImagic(pNode, "PlayerCardType", utility::toString(WidgetFun::getWidgetUserInfo(pNode, "CardTypeImage"),"0_", (int)result.cardPoint, ".png"), false);
				}else if (result.type != SG::SGCardType_None && result.type != SG::SGCardType_Invalid ){
				    WidgetFun::setImagic(pNode, "PlayerCardType", utility::toString(WidgetFun::getWidgetUserInfo(pNode, "CardTypeImage"), result.type - SG::SGCardType_None, ".png"), false);
				}
				auto winColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "WinColor");
				auto loseColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "LoseColor");
				if (player.getPlayerSingleCalculate().isValid) {
					WidgetFun::setVisible(pNode, "PlayerSingleCalculate", true);
					auto pScore = WidgetFun::getChildWidget(pNode, "PlayerSingleCalculate");
					int score = player.getPlayerSingleCalculate().score;
					std::string text = "";
					if (score > 0) {
						text.append(utility::a_u8("胜:"));
						text.append("+");
						text.append(utility::toString(score));
						WidgetFun::setText(pScore, text);
						WidgetFun::setTextColor(pScore, winColor);
					} else if (score < 0) {
						text.append(utility::a_u8("负:"));
						text.append(utility::toString(score));
						WidgetFun::setText(pScore, text);
						WidgetFun::setTextColor(pScore, loseColor);
					} else {
						text.append(utility::a_u8("平:"));
						text.append(utility::toString(score));
						WidgetFun::setText(pScore, text);
						WidgetFun::setTextColor(pScore, loseColor);
					}
				} else {
					WidgetFun::setVisible(pNode, "PlayerSingleCalculate", false);
				}
			} else {
                WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
                WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), true);
                WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), false);

                auto pNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCardResultNone_", visioChairID));

                for(int index = 0; index < playerCards.cardCount; ++index) {
                    auto pCard = WidgetFun::getChildWidget(pNode, utility::toString("Card_", index));
                    pCard->setVisible(true);
                    WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), SGGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
                }

				if (result.type == SG::SGCardType_None )
				{
					WidgetFun::setImagic(pNode, "PlayerCardType", utility::toString(WidgetFun::getWidgetUserInfo(pNode, "CardTypeImage"),"0_",(int)result.cardPoint, ".png"), false);
				}else if (result.type != SG::SGCardType_None && result.type != SG::SGCardType_Invalid ){
					WidgetFun::setImagic(pNode, "PlayerCardType", utility::toString(WidgetFun::getWidgetUserInfo(pNode, "CardTypeImage"),result.type - SG::SGCardType_None, ".png"), false);
				}
				auto winColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "WinColor");
				auto loseColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "LoseColor");
				if (player.getPlayerSingleCalculate().isValid) {
					WidgetFun::setVisible(pNode, "PlayerSingleCalculate", true);
					auto pScore = WidgetFun::getChildWidget(pNode, "PlayerSingleCalculate");
					int score = player.getPlayerSingleCalculate().score;
					std::string text = "";
					if (score > 0) {
						text.append(utility::a_u8("胜:"));
						text.append("+");
						text.append(utility::toString(score));
						WidgetFun::setText(pScore, text);
						WidgetFun::setTextColor(pScore, winColor);
					} else if (score < 0) {
						text.append(utility::a_u8("负:"));
						text.append(utility::toString(score));
						WidgetFun::setText(pScore, text);
						WidgetFun::setTextColor(pScore, loseColor);
					} else {
						text.append(utility::a_u8("平:"));
						text.append(utility::toString(score));
						WidgetFun::setText(pScore, text);
						WidgetFun::setTextColor(pScore, loseColor);
					}
				} else {
					WidgetFun::setVisible(pNode, "PlayerSingleCalculate", false);
				}
            }
        } else {
            WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
            WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), false);
            WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), false);
        }
    }
}

void SGPlayerCard::hidePlayer(WORD visioChairID)
{
    if(visioChairID < SG_GAME_PLAYER) {
        WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
        WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), false);
        WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), false);
    }
}

#pragma endregion 显示与隐藏

#pragma region 发牌动作
void SGPlayerCard::sendPlayerCard()
{
	for (int index = 0; index < SG_GAME_PLAYER; ++index) {
		hidePlayer(index);
	}

	m_TempCards.clear();
	float totalDelay = 0;
	float cardCount = 0;
	float singleDelay = 0.1f;
	std::vector<Sprite*> selfTempCard = std::vector<Sprite*>();
	selfTempCard.clear();
	for (int cardIndex = 0; cardIndex < MAX_HAND_CARD; ++cardIndex) {
		for (int index = 0; index < SG_GAME_PLAYER; ++index) {
			SGPlayer* player = SGGameScence::Instance().getPlayerByChairID(index);
			if (player->isValid() && player->getPlayerCards().isValid) {
					if (player->getPlayerCards().cardCount == cardIndex) {
						break;
					}
					int visioChairID = player->getVisioChairID();
					auto pCardNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", visioChairID));
					++cardCount;
					DelayTime* delay = DelayTime::create(singleDelay * cardCount);

					auto sprite = Sprite::create("SG/PlayerCard/poker_0.png");
					sprite->setPosition(Vec2(640, 360));
					sprite->setAnchorPoint(Vec2(0.5, 0.5));
					sprite->setScale(0.5);
					addChild(sprite);
					m_TempCards.push_back(sprite);
					if (0 == visioChairID) {
						selfTempCard.push_back(sprite);
					}
					WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->setPositionY(CARD_DWON_POSY);
					auto targetPos = WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->getPosition()*pCardNode->getScale() + pCardNode->getPosition() - Vec2(pCardNode->getScaleX() *pCardNode->getContentSize().width * pCardNode->getAnchorPoint().x, pCardNode->getScaleY() * pCardNode->getContentSize().height * pCardNode->getAnchorPoint().y);
					CCActionInterval * moveTo = CCMoveTo::create(0.5, targetPos);
					CCActionInterval * scaleTo = CCScaleTo::create(0.5, pCardNode->getScale());
					CCActionInterval * rotateTo = CCRotateTo::create(0.5, 720);
					sprite->runAction(CCSequence::create(delay, CCSpawn::create(moveTo, scaleTo, rotateTo, nullptr), nullptr));
				}
		}
	}

	totalDelay = singleDelay * cardCount + 0.5;
	SGPlayer* player = SGGameScence::Instance().getLocalPlayer();
	if (player->isValid() && player->getPlayerCards().isValid) {
		auto pCardNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", 0));
		for (int index = 0; index < player->getPlayerCards().cardCount; index++){
			auto targetPos = WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", index))->getPosition()*pCardNode->getScale() + pCardNode->getPosition() - Vec2(pCardNode->getScaleX() *pCardNode->getContentSize().width * pCardNode->getAnchorPoint().x, pCardNode->getScaleY() * pCardNode->getContentSize().height * pCardNode->getAnchorPoint().y);
			totalDelay += singleDelay;
			DelayTime* delay = DelayTime::create(totalDelay);

			CallFunc* func = CallFunc::create([=] {
				startOrbitAction(selfTempCard.at(index), SGGameLogic::getCardIndex(player->getPlayerCards().cards[index]));
			});

			selfTempCard.at(index)->runAction(CCSequence::create(delay, func, nullptr));
		}
	}

	totalDelay += 1.5;
	DelayTime* delay = DelayTime::create(totalDelay);
	CallFunc* func = CallFunc::create([=] {
		SGGameScence::Instance().updateUserInfo();
		SGOperator::Instance().show(SGGameScence::Instance().getGameStatus());
		SGOperator::Instance().showTimes(TIME_FOR_USER_SPLIT_CARD);
		for (int index = 0; index < m_TempCards.size(); ++index) {
			if (this == m_TempCards.at(index)->getParent()) {
				removeChild(m_TempCards.at(index));
			}
		}
		m_TempCards.clear();
	});
	runAction(CCSequence::create(delay, func, nullptr));
}

void SGPlayerCard::sendPlayerCardAdd()
{
	m_TempCards.clear();
	float totalDelay = 0;
	float cardCount = 0;
	float singleDelay = 0.1f;
	std::vector<Sprite*> selfTempCard = std::vector<Sprite*>();
	selfTempCard.clear();
	int cardIndex = MAX_HAND_CARD - 1;
	for (int index = 0; index < SG_GAME_PLAYER; ++index) {
		SGPlayer* player = SGGameScence::Instance().getPlayerByChairID(index);
		if (player->isValid() && player->getPlayerCards().isValid) {
			if (player->getPlayerCards().cardCount == cardIndex) {
				break;
			}
			int visioChairID = player->getVisioChairID();
			auto pCardNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", visioChairID));
			++cardCount;
			DelayTime* delay = DelayTime::create(singleDelay * cardCount);

			auto sprite = Sprite::create("SG/PlayerCard/poker_0.png");
			sprite->setPosition(Vec2(640, 360));
			sprite->setAnchorPoint(Vec2(0.5, 0.5));
			sprite->setScale(0.5);
			addChild(sprite);
			m_TempCards.push_back(sprite);
			if (0 == visioChairID) {
				selfTempCard.push_back(sprite);
			}
			WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->setPositionY(CARD_DWON_POSY);
			auto targetPos = WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->getPosition()*pCardNode->getScale() + pCardNode->getPosition() - Vec2(pCardNode->getScaleX() *pCardNode->getContentSize().width * pCardNode->getAnchorPoint().x, pCardNode->getScaleY() * pCardNode->getContentSize().height * pCardNode->getAnchorPoint().y);
			CCActionInterval * moveTo = CCMoveTo::create(0.5, targetPos);
			CCActionInterval * scaleTo = CCScaleTo::create(0.5, pCardNode->getScale());
			CCActionInterval * rotateTo = CCRotateTo::create(0.5, 720);
			sprite->runAction(CCSequence::create(delay, CCSpawn::create(moveTo, scaleTo, rotateTo, nullptr), nullptr));
		}
	}

	totalDelay = singleDelay * cardCount + 0.5;
	SGPlayer* player = SGGameScence::Instance().getLocalPlayer();
	if (player->isValid() && player->getPlayerCards().isValid) {
		auto pCardNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", 0));
		int index = MAX_HAND_CARD - 1;
		auto targetPos = WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", index))->getPosition()*pCardNode->getScale() + pCardNode->getPosition() - Vec2(pCardNode->getScaleX() *pCardNode->getContentSize().width * pCardNode->getAnchorPoint().x, pCardNode->getScaleY() * pCardNode->getContentSize().height * pCardNode->getAnchorPoint().y);
		totalDelay += singleDelay;
		DelayTime* delay = DelayTime::create(totalDelay);

		CallFunc* func = CallFunc::create([=] {
			startOrbitAction(selfTempCard.at(0), SGGameLogic::getCardIndex(player->getPlayerCards().cards[index]));
		});
        if(selfTempCard.size() > 0){
            selfTempCard.at(0)->runAction(CCSequence::create(delay, func, nullptr));
        }
	}

	totalDelay += 1.5;
	DelayTime* delay = DelayTime::create(totalDelay);
	CallFunc* func = CallFunc::create([=] {
		SGGameScence::Instance().updateUserInfo();
		SGOperator::Instance().show(SGGameScence::Instance().getGameStatus());
		SGOperator::Instance().showTimes(TIME_FOR_USER_SPLIT_CARD);
		for (int index = 0; index < m_TempCards.size(); ++index) {
			if (this == m_TempCards.at(index)->getParent()) {
				removeChild(m_TempCards.at(index));
			}
		}
		m_TempCards.clear();
	});
	runAction(CCSequence::create(delay, func, nullptr));
}

//翻牌动作  
void SGPlayerCard::startOrbitAction(cocos2d::Sprite* sprite, int cardIndex)
{
	//扑克牌反面  
	CCSprite* pokerBack = sprite;

	//扑克牌正面  
	CCSprite* pokerFront = Sprite::create(utility::toString("SG/PlayerCard/poker_", cardIndex, ".png"));
	pokerFront->setPosition(sprite->getPosition());
	pokerFront->setVisible(false);
	addChild(pokerFront);
	m_TempCards.push_back(pokerFront);
	
	float orbitTime = 0.25;
	//第一个参数是旋转的时间，第二个参数是起始半径，第三个参数半径差，第四个参数是起始Z角，第五个参数是旋转Z角差，第六个参数是起始X角，最后一个参数旋转X角差，  
	if (pokerFront->isVisible() == false) {
		//第一个参数是旋转的时间，第二个参数是起始半径，第三个参数半径差，第四个参数是起始Z角，第五个参数是旋转Z角差，第六个参数是起始X角，最后一个参数旋转X角差，  
		CCOrbitCamera* orbitFront = CCOrbitCamera::create(orbitTime, 1, 0, 270, 90, 0, 0);
		CCSequence* sequenceFront = CCSequence::createWithTwoActions(CCShow::create(), orbitFront);
		CCTargetedAction* targetFront = CCTargetedAction::create(pokerFront, sequenceFront);

		CCOrbitCamera* orbitBack = CCOrbitCamera::create(orbitTime, 1, 0, 0, 90, 0, 0);
		pokerBack->runAction(CCSequence::create(orbitBack, CCHide::create(), targetFront, NULL));
	} else if (pokerFront->isVisible() == true){
		CCOrbitCamera* orbitFront = CCOrbitCamera::create(orbitTime, 1, 0, 270, 90, 0, 0);
		CCSequence* sequenceFront = CCSequence::createWithTwoActions(CCShow::create(), orbitFront);
		CCTargetedAction* targetFront = CCTargetedAction::create(pokerBack, sequenceFront);

		CCOrbitCamera* orbitBack = CCOrbitCamera::create(orbitTime, 1, 0, 0, 90, 0, 0);
		pokerFront->runAction(CCSequence::create(orbitBack, CCHide::create(), targetFront,  NULL));
	}
}
#pragma endregion 发牌动作

#pragma region 摸牌判断
int SGPlayerCard::getTouchCard(cocos2d::Vec2 kTouchPos)
{
    cocos2d::Node* pNode = WidgetFun::getChildWidget(this, "PlayerCard_0");

    for(int i = MAX_HAND_CARD - 1; i >= 0; --i) {
        cocos2d::Sprite* pSprite = dynamic_cast<Sprite*>(WidgetFun::getChildWidget(pNode, utility::toString("Card_", i)));

        if(pSprite == NULL) {
            continue;
        }

        if(utility::IsSpriteInTouch(kTouchPos, pSprite)) {
            return i;
        }
    }

    return -1;
}
#pragma endregion 摸牌判断

