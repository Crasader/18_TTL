#include "NNPlayerCard.h"
#include "Game/NN/CMD_NN.h"
#include "Game/NN/NNGameLogic.h"
#include "Game/NN/NNGameScene.h"
#include "Game/NN/NNOperator.h"
#include "Game/NN/NNRoomInfo.h"

#include "UTILITY.h"

FV_SINGLETON_STORAGE(NNPlayerCard);

NNPlayerCard::NNPlayerCard()
	:_b_fan(false)
{
    init();
}

NNPlayerCard::~NNPlayerCard()
{

}

#pragma region 初始化
bool NNPlayerCard::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();

    return true;
}

void NNPlayerCard::initLayout()
{
    WidgetScenceXMLparse playerCard("TTLNN/Script/NNPlayerCard.xml", this);
}

#pragma endregion 初始化

#pragma region 显示与隐藏

void NNPlayerCard::show()
{
    setVisible(true);
}

void NNPlayerCard::hide()
{
    setVisible(false);
}

void NNPlayerCard::showPlayer(NNPlayer& player)
{
    word visioChairID = player.getVisioChairID();
	auto self = NNGameScene::Instance().getSelf();
	auto game_type = static_cast<NNGameType>(NNRoomInfo::Instance().getRoomInfo().bGameTypeIdex);

    if(visioChairID < NNGameScene::MAX_PLAYER) {
        NNPlayerCard_Entity playerCards = player.getPlayerCards();

        if(player.isValid() && playerCards.isValid) {
            TTLNN::NNCardType_Result result = player.getPlayerCardType();

            if(result.type == TTLNN::NNCardType_Invalid) {
                auto pNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", visioChairID));
                pNode->setVisible(true);
                WidgetFun::setVisible(pNode, "PlayerCardReultNode", false);

                for(int index = 0; index < playerCards.cardCount; ++index) {
                    auto pCard = WidgetFun::getChildWidget(pNode, utility::toString("Card_", index));
                    pCard->setVisible(true);
					switch (game_type)
					{
					case TTLNN::NNGameType_SnatchBanker:
						if (self && self->GetUserID() == player.GetUserID()) {
							if (_b_fan == true) {
								WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
							} else {
								WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), "0.png"), false);
							}
						}
						break;
					case TTLNN::NNGameType_AllCompare:
						if (self && self->GetUserID() == player.GetUserID()) {
							if (_b_fan == true) {
								WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
							} else if (!NNGameScene::Instance().isStatusSplitCard()) {
								WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
							} else {
								WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), "0.png"), false);
							}
						}
						break;
					case TTLNN::NNGameType_NNBanker:
						break;
					case TTLNN::NNGameType_HostBanker:
						break;
					case TTLNN::NNGameType_SnatchBankerShowCard:
						WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
						//最后一张牌
						if (index == 4) {
							//没有拆牌
							if (!NNGameScene::Instance().isStatusSplitCard()) {
								WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
							} else {
								//已经翻牌
								if (self && self->GetUserID() == player.GetUserID() && _b_fan == true) {
									WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
								} else {
									WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), "0.png"), false);
								}
							}
						}
						break;
					case TTLNN::NNGameType_NNRatio:
						break;
					case TTLNN::NNGameType_HLN:
						break;
					default:
						break;
					}

                    if(&player == NNGameScene::Instance().getSelf()) {
                        if(!NNGameScene::Instance().isStatusSplitCard()) {
                            WidgetFun::getChildWidget(pNode, utility::toString("Card_", index))->setPositionY(CARD_DWON_POSY);
                            continue;
                        }

                        std::vector<int> selectCards = NNGameScene::Instance().getSelectCards();
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
            } else if(result.type > TTLNN::NNCardType_None && result.type < TTLNN::NNCardType_SZN) {
                WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
                WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), false);
                WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), true);

                auto pNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCardResultHasNiu_", visioChairID));
                int cardIndex = 0;

                for(int index = 0; index < playerCards.cardCount; ++index) {
                    if(result.isCardSelected[index]) {
                        auto pCard = WidgetFun::getChildWidget(pNode, utility::toString("Card_", cardIndex++));
                        pCard->setVisible(true);
                        WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
                    }
                }

                for(int index = 0; index < playerCards.cardCount; ++index) {
                    if(!result.isCardSelected[index]) {
                        auto pCard = WidgetFun::getChildWidget(pNode, utility::toString("Card_", cardIndex++));
                        pCard->setVisible(true);
                        WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
                    }
                }

                WidgetFun::setImagic(pNode, "PlayerCardType", utility::toString(WidgetFun::getWidgetUserInfo(pNode, "CardTypeImage"), result.type - TTLNN::NNCardType_None, ".png"), false);

				auto winColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "WinColor");
				auto loseColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "LoseColor");
				if (player.getPlayerSingleCalculate().isValid) {
					WidgetFun::setVisible(pNode, "PlayerSingleCalculate", true);
					auto pScore = WidgetFun::getChildWidget(pNode, "PlayerSingleCalculate");
					int score = player.getPlayerSingleCalculate().score;
					std::string text = "";
					if (score > 0) {
						text.append(utility::a_u8("胜:"));
						//text.append("+");
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
                    WidgetFun::setImagic(pCard, utility::toString(WidgetFun::getWidgetUserInfo(pNode, "Image"), NNGameLogic::getCardIndex(playerCards.cards[index]), ".png"), false);
                }

                WidgetFun::setImagic(pNode, "PlayerCardType", utility::toString(WidgetFun::getWidgetUserInfo(pNode, "CardTypeImage"), result.type - TTLNN::NNCardType_None, ".png"), false);

				auto winColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "WinColor");
				auto loseColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "LoseColor");
				if (player.getPlayerSingleCalculate().isValid) {
					WidgetFun::setVisible(pNode, "PlayerSingleCalculate", true);
					auto pScore = WidgetFun::getChildWidget(pNode, "PlayerSingleCalculate");
					int score = player.getPlayerSingleCalculate().score;
					std::string text = "";
					if (score > 0) {
						text.append(utility::a_u8("胜:"));
						//text.append("+");
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

void NNPlayerCard::calculatePlayer(NNPlayer& player)
{
	word visioChairID = player.getVisioChairID();
	if(visioChairID < NNGameScene::MAX_PLAYER) {
		NNPlayerCard_Entity playerCards = player.getPlayerCards();
		if(player.isValid() && playerCards.isValid) {
			TTLNN::NNCardType_Result result = player.getPlayerCardType();
			if(result.type == TTLNN::NNCardType_Invalid) {
				auto pNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", visioChairID));
				pNode->setVisible(true);
				WidgetFun::setVisible(pNode, "PlayerCardReultNode", false);
				return;
			} 
			if(result.type > TTLNN::NNCardType_None && result.type < TTLNN::NNCardType_WHN) {
				WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
				WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), false);
				WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), true);
			} else {
				WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
				WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), true);
				WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), false);
			}
			bool isHasNiu = (result.type > TTLNN::NNCardType_None && result.type < TTLNN::NNCardType_ZDN);
			auto pNode =  WidgetFun::getChildWidget(this, utility::toString(isHasNiu?"PlayerCardResultHasNiu_":"PlayerCardResultNone_", visioChairID));
			auto winColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "WinColor");
			auto loseColor = WidgetFun::getWidgetUserInfo(pNode, "PlayerSingleCalculate", "LoseColor");
			if (player.getPlayerSingleCalculate().isValid) {
				WidgetFun::setVisible(pNode, "PlayerSingleCalculate", true);
				auto pScore = WidgetFun::getChildWidget(pNode, "PlayerSingleCalculate");
				int score = player.getPlayerSingleCalculate().score;
				std::string text = "";
				if (score > 0) {
					text.append(utility::a_u8("胜:"));
					//text.append("+");
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
			WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), false);
			WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), false);
		}
	}
}

void NNPlayerCard::hidePlayer(word visioChairID)
{
    if(visioChairID < NNGameScene::MAX_PLAYER) {
        WidgetFun::setVisible(this, utility::toString("PlayerCard_", visioChairID), false);
        WidgetFun::setVisible(this, utility::toString("PlayerCardResultNone_", visioChairID), false);
        WidgetFun::setVisible(this, utility::toString("PlayerCardResultHasNiu_", visioChairID), false);
    }
}

#pragma endregion 显示与隐藏

#pragma region 发牌动作

using namespace TTLNN;

void NNPlayerCard::onSendPlayerCard()
{
	for (int index = 0; index < NNGameScene::MAX_PLAYER; ++index) {
		hidePlayer(index);
	}

	auto game_type = static_cast<NNGameType>(NNRoomInfo::Instance().getRoomInfo().bGameTypeIdex);
	std::vector<cocos2d::Sprite*> selfTempCard;

	float totalDelay = 0;
	float cardCount = 0;
	float singleDelay = 0.1f;

	//发牌动作
	for (int cardIndex = 0; cardIndex < MAX_HAND_CARD; ++cardIndex) {
		for (int index = 0; index < NN_GAME_PLAYER; ++index) {
			NNPlayer* player = NNGameScene::Instance().getPlayerByChairID(index);
			if (player->isValid() && player->getPlayerCards().isValid) {
					if (player->getPlayerCards().cardCount == cardIndex) {
						break;
					}
					int visioChairID = player->getVisioChairID();
					auto pCardNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", visioChairID));
					++cardCount;
					DelayTime* delay = DelayTime::create(singleDelay * cardCount);

					auto sprite = Sprite::create("TTLNN/PlayerCard/poker_0.png");
					sprite->setPosition(Vec2(640, 360));
					sprite->setAnchorPoint(Vec2(0.5, 0.5));
					sprite->setScale(0.5);
					addChild(sprite);
					_turning_cards.push_back(sprite);
					if (0 == visioChairID) {
						selfTempCard.push_back(sprite);
					}
					WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->setPositionY(CARD_DWON_POSY);
					auto targetPos = WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->getPosition() * pCardNode->getScale() +
												pCardNode->getPosition() -
												Vec2(pCardNode->getScaleX() *pCardNode->getContentSize().width * pCardNode->getAnchorPoint().x,
														pCardNode->getScaleY() * pCardNode->getContentSize().height * pCardNode->getAnchorPoint().y);
					CCActionInterval * moveTo = CCMoveTo::create(0.5, targetPos);
					CCActionInterval * scaleTo = CCScaleTo::create(0.5, pCardNode->getScale());
					CCActionInterval * rotateTo = CCRotateTo::create(0.5, 720);
					sprite->runAction(CCSequence::create(delay, CCSpawn::create(moveTo, scaleTo, rotateTo, nullptr), nullptr));
				}
		}
	}

	//翻拍动作
	//如果不是同比牛牛, 则翻牌
	if (game_type != NNGameType::NNGameType_AllCompare &&
		game_type != NNGameType::NNGameType_SnatchBanker
		) {
		totalDelay = singleDelay * cardCount + 0.5;
		NNPlayer* player = NNGameScene::Instance().getSelf();
		if (player->isValid() && player->getPlayerCards().isValid) {
			auto pCardNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", 0));
			for (word index = 0; index < player->getPlayerCards().cardCount; index++){
				auto targetPos = WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", index))->getPosition() * pCardNode->getScale() +
											pCardNode->getPosition() -
											Vec2(pCardNode->getScaleX() *pCardNode->getContentSize().width * pCardNode->getAnchorPoint().x,
													pCardNode->getScaleY() * pCardNode->getContentSize().height * pCardNode->getAnchorPoint().y);
				totalDelay += singleDelay;
				DelayTime* delay = DelayTime::create(totalDelay);

				CallFunc* func = CallFunc::create([=] {
					startOrbitAction(selfTempCard.at(index), NNGameLogic::getCardIndex(player->getPlayerCards().cards[index]), &_turning_cards);
				});
				selfTempCard.at(index)->runAction(CCSequence::create(delay, func, nullptr));
			}
		}
	}

	//清理掉翻拍创建的临时精灵
	totalDelay += 1.5;
	DelayTime* delay = DelayTime::create(totalDelay);

	CallFunc* func = CallFunc::create([=] {
		NNGameScene::Instance().updateUserInfo();
		NNOperator::Instance().show(NNGameScene::Instance().getGameStatus());
		NNOperator::Instance().showTimes(TIME_FOR_USER_SPLIT_CARD);
		for (size_t index = 0; index < _turning_cards.size(); ++index) {
			if (this == _turning_cards.at(index)->getParent()) {
				removeChild(_turning_cards.at(index));
			}
		}
		_turning_cards.clear();
	});
	runAction(CCSequence::create(delay, func, nullptr));
}

void NNPlayerCard::onSendPlayerCardAdd()
{
	clearTurningCard();
	float totalDelay = 0;
	float cardCount = 0;
	float singleDelay = 0.1f;
	std::vector<Sprite*> selfTempCard = std::vector<Sprite*>();
	selfTempCard.clear();
	int cardIndex = MAX_HAND_CARD - 1;
	for (int index = 0; index < NNGameScene::MAX_PLAYER; ++index) {
		NNPlayer* player = NNGameScene::Instance().getPlayerByChairID(index);
		if (player->isValid() && player->getPlayerCards().isValid) {
			if (player->getPlayerCards().cardCount == cardIndex) {
				break;
			}
			int visioChairID = player->getVisioChairID();
			auto pCardNode = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", visioChairID));
			++cardCount;
			DelayTime* delay = DelayTime::create(singleDelay * cardCount);

			auto sprite = Sprite::create("TTLNN/PlayerCard/poker_0.png");
			sprite->setPosition(Vec2(640, 360));
			sprite->setAnchorPoint(Vec2(0.5, 0.5));
			sprite->setScale(0.6f);
			addChild(sprite);
			_turning_cards.push_back(sprite);
			if (0 == visioChairID) {
				selfTempCard.push_back(sprite);
			}
			WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->setPositionY(CARD_DWON_POSY);
			auto targetPos = WidgetFun::getChildWidget(pCardNode, utility::toString("Card_", cardIndex))->getPosition() * pCardNode->getScale() +
										pCardNode->getPosition() -
										Vec2(pCardNode->getScaleX() *pCardNode->getContentSize().width * pCardNode->getAnchorPoint().x,
												pCardNode->getScaleY() * pCardNode->getContentSize().height * pCardNode->getAnchorPoint().y);
			CCActionInterval * moveTo = CCMoveTo::create(0.5, targetPos);
			CCActionInterval * scaleTo = CCScaleTo::create(0.5, pCardNode->getScale());
			CCActionInterval * rotateTo = CCRotateTo::create(0.5, 720);
			sprite->runAction(CCSequence::create(delay, CCSpawn::create(moveTo, scaleTo, rotateTo, nullptr), nullptr));
		}
	}

	totalDelay += 1.5;
	DelayTime* delay = DelayTime::create(totalDelay);

	CallFunc* func = CallFunc::create([=] {
		NNGameScene::Instance().updateUserInfo();
		NNOperator::Instance().show(NNGameScene::Instance().getGameStatus());
		NNOperator::Instance().showTimes(TIME_FOR_USER_SPLIT_CARD);
		clearTurningCard();
	});
	runAction(CCSequence::create(delay, func, nullptr));
}

void NNPlayerCard::fanCard(int index)
{
	NNPlayer* player = NNGameScene::Instance().getSelf();
	if (!player) {
		return;
	}

	float fDelayTime = 0.f;
	cocos2d::Sprite* spAction = nullptr;
	cocos2d::Sprite* targetCard = nullptr;

	auto game_type = static_cast<NNGameType>(NNRoomInfo::Instance().getRoomInfo().bGameTypeIdex);
	auto cards = WidgetFun::getChildWidget(this, utility::toString("PlayerCard_", player->getVisioChairID()));

	switch (game_type)
	{
	case TTLNN::NNGameType_NNBanker:
		break;
	case TTLNN::NNGameType_HostBanker:
		break;
	case TTLNN::NNGameType_SnatchBankerShowCard:
		targetCard = dynamic_cast<cocos2d::Sprite*>(WidgetFun::getChildWidget(cards, "Card_4"));
		if (targetCard) {
			auto targetPos = targetCard->getPosition() * cards->getScale() +
										cards->getPosition() -
										Vec2(cards->getScaleX() * cards->getContentSize().width * cards->getAnchorPoint().x,
												cards->getScaleY() * cards->getContentSize().height * cards->getAnchorPoint().y);
			spAction = startOrbitAction(targetCard,
				NNGameLogic::getCardIndex(player->getPlayerCards().cards[index]),
				&_turning_cards,
				&targetPos);
			fDelayTime += 0.25;
		}
		break;
	case TTLNN::NNGameType_SnatchBanker:
	case TTLNN::NNGameType_AllCompare:
		for (word index = 0; index < player->getPlayerCards().cardCount; index++) {
			targetCard = dynamic_cast<cocos2d::Sprite*>(WidgetFun::getChildWidget(cards, utility::toString("Card_", static_cast<int>(index))));
			auto targetPos = WidgetFun::getChildWidget(cards, utility::toString("Card_", index))->getPosition() * cards->getScale() +
																				cards->getPosition() -
																				Vec2(cards->getScaleX() * cards->getContentSize().width * cards->getAnchorPoint().x,
																					cards->getScaleY() * cards->getContentSize().height * cards->getAnchorPoint().y);
			startOrbitAction(targetCard,
				NNGameLogic::getCardIndex(player->getPlayerCards().cards[index]),
				&_turning_cards,
				&targetPos);
			fDelayTime += 0.13;
		}
		break;
	case TTLNN::NNGameType_NNRatio:
		break;
	case TTLNN::NNGameType_HLN:
		break;
	default:
		break;
	}

	DelayTime* delay = DelayTime::create(fDelayTime);
	CallFunc* func = CallFunc::create([=] {
		_b_fan = true;
		player->upPlayerInfo();
		clearTurningCard();
	});
	runAction(CCSequence::create(delay, func, nullptr));
}

void NNPlayerCard::removeLastCard(NNPlayer& player)
{
	/*for (auto itor = m_TempCards.begin(); itor !=  m_TempCards.end(); itor++) {
		if ( == itor->() ) {
			removeChild(m_TempCards.at(index));
		}
	}
	m_TempCards.clear();*/
}

void NNPlayerCard::setFanPai(bool flag)
{
	_b_fan = flag;
}

bool NNPlayerCard::getFanPai()
{
	return _b_fan;
}

//翻牌动作
cocos2d::Sprite* NNPlayerCard::startOrbitAction(cocos2d::Sprite* sprite, int cardIndex, std::vector<cocos2d::Sprite*>* spList, cocos2d::Vec2* offset, float fobTime)
{
	//扑克牌反面
	CCSprite* pokerBack = sprite;
	//扑克牌正面
	CCSprite* pokerFront = Sprite::create(utility::toString("TTLNN/PlayerCard/poker_", cardIndex, ".png"));

	if (offset) {
		pokerFront->setPosition(*offset);
	} else {
		pokerFront->setPosition(sprite->getPosition());
	}
	if (spList) {
		spList->push_back(pokerFront);
	}

	pokerFront->setVisible(false);
	this->addChild(pokerFront);

	//第一个参数是旋转的时间，第二个参数是起始半径，第三个参数半径差，第四个参数是起始Z角，第五个参数是旋转Z角差，第六个参数是起始X角，最后一个参数旋转X角差，  
	CCOrbitCamera* orbitBack_1 = CCOrbitCamera::create(fobTime, 1, 0, -180, 90, 0, 0);
	CCOrbitCamera* orbitBack_2 = CCOrbitCamera::create(fobTime, 1, 0, -90, 90, 0, 0);

	CCOrbitCamera* orbitFront_1 = CCOrbitCamera::create(fobTime, 1, 0, -180, 90, 0, 0);
	CCOrbitCamera* orbitFront_2 = CCOrbitCamera::create(fobTime, 1, 0, -90, 90, 0, 0);

	pokerBack->runAction(CCSequence::create(orbitBack_1, CCHide::create(), orbitBack_2, nullptr));
	pokerFront->runAction(CCSequence::create(CCHide::create(), orbitFront_1, CCShow::create(), orbitFront_2,nullptr));

	return pokerFront;
}

#pragma endregion 发牌动作

#pragma region 摸牌判断

int NNPlayerCard::getTouchCard(cocos2d::Vec2 kTouchPos)
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

void NNPlayerCard::clearTurningCard()
{
	for (size_t idx = 0; idx < _turning_cards.size(); idx++) {
		if (_turning_cards[idx]) {
			_turning_cards[idx]->setVisible(false);
			this->removeChild(_turning_cards[idx]);
		}
	}
	_turning_cards.clear();
}

#pragma endregion 摸牌判断
