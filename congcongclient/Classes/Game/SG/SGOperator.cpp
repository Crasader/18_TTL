#include "SGOperator.h"
#include "Game/Game/MissionWeiXin.h"
#include "Game/SG/CMD_SG.h"
#include "Game/SG/SGGameScence.h"
#include "Game/SG/SGRoomInfo.h"
#include "Game/SG/SGGameLogic.h"
#include "UTILITY.h"

FV_SINGLETON_STORAGE(SGOperator);
SGOperator::SGOperator()
{
    init();
}

SGOperator::~SGOperator()
{

}

#pragma region 初始化

bool SGOperator::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();

    return true;
}

void SGOperator::initLayout()
{
    WidgetScenceXMLparse SGoperator("SG/Script/SGOperator.xml", this);
}

void SGOperator::initButton()
{
    WidgetManager::addButtonCB("SGOperator_ButtonReady", this, button_selector(SGOperator::Button_Ready));
    WidgetManager::addButtonCB("SGOperator_ButtonContinue", this, button_selector(SGOperator::Button_Ready));
    //WidgetManager::addButtonCB("SGOperator_ButtonWeiXin", this, button_selector(SGOperator::Button_WeiXin));
    WidgetManager::addButtonCB("SGOperator_ButtonStartGame", this, button_selector(SGOperator::Button_StartGame));
    WidgetManager::addButtonCB("SGOperator_SntachBanker", this, button_selector(SGOperator::Button_SnatchBanker));
    WidgetManager::addButtonCB("SGOperator_Call", this, button_selector(SGOperator::Button_Call));
    WidgetManager::addButtonCB("SGOperator_ButtonShowCard", this, button_selector(SGOperator::Button_ShowCard));
    WidgetManager::addButtonCB("SGOperator_ButtonHint", this, button_selector(SGOperator::Button_Hint));
}
#pragma endregion 初始化

#pragma region 显示与隐藏

void SGOperator::show()
{
    setVisible(true);
}

void SGOperator::hide()
{
    hideMessage();
    hideReady();
    hideWeiXin();
    hideContinue();
    hideStartGame();
    hideSnatchButton();
    hideCallButtons();
    hideTimes();
    hideSplitButton();
    hideSplitCalculate();
    hideCalculate();
    setVisible(false);
}

void SGOperator::show(word status)
{

    switch(status) {
    case SG::SGGameStatus_Free: {
        std::string showText = utility::a_u8("等待房主确认开始游戏");
        showMessage(showText);

        if(SGRoomInfo::Instance().isHostPlayer(*SGGameScence::Instance().getLocalPlayer())) {
            showStartGame();
        } else {
            hideStartGame();
        }
    }
    break;

    case SG::SGGameStatus_HostConfirm: {
        std::string showText = utility::a_u8("等待所有玩家准备");
        showMessage(showText);
        hideStartGame();
    }
    break;

    case SG::SGGameStatus_SnatchBanker: {
        if(SGGameScence::Instance().getLocalPlayer()->getPlayerStatus() != SG::SGPlayerStatus_Playing) {
			std::string showText = utility::a_u8("请等待本局游戏结束");
			showMessage(showText);
            break;
        }

        if(SGGameScence::Instance().getLocalPlayer()->getSnatchBankerRatio() != (word)SG::SGSnatchBanker_Invalid) {
            std::string showText = utility::a_u8("等待抢庄结果");
            showMessage(showText);
            hideSnatchButton();
            showTimes(TIME_FOR_SNATCH_BANKER);
        } else {
            std::string showText = utility::a_u8("开始抢庄");
            showMessage(showText);
            showSnatchButton();
            hideStartGame();
            showTimes(TIME_FOR_SNATCH_BANKER);
        }
    }
    break;

    case SG::SGGameStatus_Call: {
        if(SGGameScence::Instance().getLocalPlayer()->getPlayerStatus() != SG::SGPlayerStatus_Playing) {
			std::string showText = utility::a_u8("请等待本局游戏结束");
			showMessage(showText);
            break;
        }
        word* temp = SGGameScence::Instance().getAllBets();
        if(SGGameScence::Instance().isBankerUser(*SGGameScence::Instance().getLocalPlayer())) {
            std::string showText = utility::a_u8("等待其他玩家下注");
            showMessage(showText);
        } else {
            if(SGGameScence::Instance().getLocalPlayer()->getPlayerBets() != 0) {
                std::string showText = utility::a_u8("等待其他玩家下注");
                showMessage(showText);
                hideCallButtons();
            } else {
                std::string showText = utility::a_u8("请下注:");
                showMessage(showText);
                showCallButtons();
            }
        }
    }
    break;

    case SG::SGGameStatus_SplitCard: {
        if(SGGameScence::Instance().getLocalPlayer()->getPlayerStatus() != SG::SGPlayerStatus_Playing) {
			std::string showText = utility::a_u8("请等待本局游戏结束");
			showMessage(showText);
            break;
        }

        if(SGGameScence::Instance().isSplitCard()) {
            std::string showText = utility::a_u8("请拆牌");
            //showMessage(showText);
			hideMessage();
            showSplitButton();
            updateSplitCalculate();
        } else {
            std::string showText = utility::a_u8("请等待其他玩家亮牌");
            showMessage(showText);
            hideSplitButton();
            hideSplitCalculate();
        }
    }
    break;

    case SG::SGGameStatus_Calculate: {
        if(SGGameScence::Instance().getLocalPlayer()->getPlayerStatus() != SG::SGPlayerStatus_Playing) {
			std::string showText = utility::a_u8("请等待本局游戏结束");
			showMessage(showText);
			break;
        }

        hideMessage();
        hideTimes();
        showCalculate();
    }
    break;

    default:
        break;
    }

    show();
}

void SGOperator::showMessage(std::string showText)
{
    WidgetFun::setVisible(this, "SGOperator_MessageNode", true);
    WidgetFun::setText(this, "SGOperator_Message", showText);
}

void SGOperator::hideMessage()
{
    WidgetFun::setVisible(this, "SGOperator_MessageNode", false);
}

void SGOperator::showStartGame()
{
    WidgetFun::setVisible(this, "SGOperator_ButtonStartGame", true);
}

void SGOperator::hideStartGame()
{
    WidgetFun::setVisible(this, "SGOperator_ButtonStartGame", false);
}

void SGOperator::showReady()
{
    WidgetFun::setVisible(this, "SGOperator_ButtonReady", true);
	if(!WidgetFun::isWidgetVisble(this,"SGOperator_AlarmNode"))
	{
		showTimes(10);
	}
}

void SGOperator::hideReady()
{
    WidgetFun::setVisible(this, "SGOperator_ButtonReady", false);
	hideTimes();
}

void SGOperator::showContinue()
{
    WidgetFun::setVisible(this, "SGOperator_ButtonContinue", true);
	if(!WidgetFun::isWidgetVisble(this,"SGOperator_AlarmNode"))
	{
		showTimes(10);
	}
}

void SGOperator::hideContinue()
{
	WidgetFun::setVisible(this, "SGOperator_ButtonContinue", false);
	hideTimes();
}

void SGOperator::showWeiXin()
{
    //WidgetFun::setVisible(this, "SGOperator_ButtonWeiXin", true);
}

void SGOperator::hideWeiXin()
{
    //WidgetFun::setVisible(this, "SGOperator_ButtonWeiXin", false);
}

void SGOperator::showSnatchButton()
{
    auto pSnatchNode = WidgetFun::getChildWidget(this, "SGOperator_SnatchBankerNode");
    pSnatchNode->removeAllChildren();

    auto centerPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "SGOperator_SnatchBankerNode", "CenterPos"));
    auto addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "SGOperator_SnatchBankerNode", "AddPos"));
    auto image = WidgetFun::getWidgetUserInfo(this, "SGOperator_SnatchBankerNode", "Image");

    int maxRatio = SGGameScence::Instance().getSnatchRatio();
	float centerIndex = 0;
	int maxCount =2;
    //float centerIndex = maxRatio / 2.f;
    //for(int index = 0; index <= maxRatio; ++index) {
	for(int index = 0; index < maxCount; ++index) {
        auto pSnatchButton = WidgetManager::Instance().createWidget("SGOperator_SntachBanker", pSnatchNode);
		if(maxCount%2 ==0)
		{
			int del = index - maxCount/2;
			if(del<0)
			{
				pSnatchButton->setPosition(centerPos + addPos*del);
			}else
			{
				pSnatchButton->setPosition(centerPos + addPos*(del+1));
			}
		}else
		{
			pSnatchButton->setPosition(centerPos + addPos *(index - maxCount/2));
		}
        WidgetFun::setWidgetUserInfo(pSnatchButton, "SGOperator_SnatchRatio", utility::toString(index));
        WidgetFun::setButtonImagic(pSnatchButton, utility::toString(image, index, ".png"), true);
    }

    pSnatchNode->setVisible(true);
}

void SGOperator::hideSnatchButton()
{
    WidgetFun::getChildWidget(this, "SGOperator_SnatchBankerNode")->removeAllChildren();
    WidgetFun::setVisible(this, "SGOperator_SnatchBankerNode", false);
}

void SGOperator::showCallButtons()
{	
	auto pCallNode = WidgetFun::getChildWidget(this, "SGOperator_CallNode");
	pCallNode->removeAllChildren();
	auto centerPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "SGOperator_CallNode", "CenterPos"));
	auto addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "SGOperator_CallNode", "AddPos"));
	auto image = WidgetFun::getWidgetUserInfo(this, "SGOperator_CallNode", "Image");
	word* temp = SGGameScence::Instance().getAllBets();

	byte cbCount = 0;
	for (byte i = 0;i<MAX_STAKE_COUNT;++i)
	{
		if (temp[i] == 0)
		{
			break;
		}
		cbCount++;
	}

	for(int index = 0; index < cbCount; ++index)
	{
		auto pSnatchButton = WidgetManager::Instance().createWidget("SGOperator_Call", pCallNode);
		if(cbCount%2 ==0)
		{
			auto startPos = centerPos-(cbCount-1)*addPos/2;
			pSnatchButton->setPosition(startPos + addPos*index);
		}else
		{
			pSnatchButton->setPosition(centerPos + addPos *(index - cbCount/2));
		}
		WidgetFun::setWidgetUserInfo(pSnatchButton, "SGOperator_Call_Bets", utility::toString(temp[index]));
		WidgetFun::setButtonImagic(pSnatchButton, utility::toString(image, index, ".png"), true);
	}
	pCallNode->setVisible(true);
}

void SGOperator::hideCallButtons()
{
    WidgetFun::setVisible(this, "SGOperator_CallNode", false);
}

void SGOperator::showTimes(int times)
{
    WidgetFun::getChildWidget(this, "SGOperator_AlarmNode")->stopAllActions();
    show();
    WidgetFun::setVisible(this, "SGOperator_AlarmNode", true);
    int first = times / 10;
    int second = times % 10;
    auto pAlarmNode = WidgetFun::getChildWidget(this, "SGOperator_AlarmNode");
    WidgetFun::setImagic(WidgetFun::getChildWidget(this, "SGAlarm_Timer_0"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), first, ".png"), false);
    WidgetFun::setImagic(WidgetFun::getChildWidget(this, "SGAlarm_Timer_1"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), second, ".png"), false);

    if(times > 0) {
        DelayTime* delay = DelayTime::create(1.f);
        CallFunc* func = CallFunc::create([ = ] {
            showTimes(times - 1);
        });
        Sequence* seq = Sequence::create(delay, func, NULL);
        WidgetFun::getChildWidget(this, "SGOperator_AlarmNode")->runAction(seq);
    }
}

void SGOperator::hideTimes()
{
    WidgetFun::getChildWidget(this, "SGOperator_AlarmNode")->stopAllActions();
    WidgetFun::setVisible(this, "SGOperator_AlarmNode", false);
}

void SGOperator::showSplitButton()
{
    WidgetFun::setVisible(this, "SGOperator_ButtonShowCard", true);
    WidgetFun::setVisible(this, "SGOperator_ButtonHint", false);//不显提示牌按钮
}

void SGOperator::hideSplitButton()
{
    WidgetFun::setVisible(this, "SGOperator_ButtonShowCard", false);
    WidgetFun::setVisible(this, "SGOperator_ButtonHint", false);
}

void SGOperator::showSplitCalculate()
{
    show();
    WidgetFun::setVisible(this, "SGOperator_CalculateNode", false);//不显示拆牌界面
}

void SGOperator::hideSplitCalculate()
{
    WidgetFun::setVisible(this, "SGOperator_CalculateNode", false);
}

void SGOperator::updateSplitCalculate()
{
    if(SGGameScence::Instance().getGameStatus() == SG::SGGameStatus_SplitCard) {
        showSplitCalculate();
    } else {
        hideSplitCalculate();
        return;
    }

    SGPlayerCard_Entity cards = SGGameScence::Instance().getLocalPlayer()->getPlayerCards();
    std::vector<int> cardsSelected = SGGameScence::Instance().getSelectCards();

    int sum = 0;

    for(size_t i = 0; i < 3; i++) {
        if(i >= cardsSelected.size()) {
            WidgetFun::setVisible(this, utility::toString("SGOperator_CalculateNum_", i), false);
            continue;
        }

        auto pNode = WidgetFun::getChildWidget(this, utility::toString("SGOperator_CalculateNum_", i));
        pNode->setVisible(true);

        int cardValue = SGGameScence::Instance().getLocalPlayer()->getPlayerCards().cards[cardsSelected.at(i)] & MASK_VALUE;
        cardValue = (cardValue > 0xa ? 0xa : cardValue);
        WidgetFun::setText(pNode, cardValue);
        sum += cardValue;
    }

    if(sum == 0) {
        WidgetFun::setVisible(this, "SGOperator_CalculateNum_Sum", false);
    } else {
        WidgetFun::setVisible(this, "SGOperator_CalculateNum_Sum", true);
        WidgetFun::setText(this, "SGOperator_CalculateNum_Sum", sum);
    }
}

void SGOperator::showCalculate()
{
    SGPlayerSingleCalculate calculate = SGGameScence::Instance().getLocalPlayer()->getPlayerSingleCalculate();

    if(calculate.isValid) {
        WidgetFun::setVisible(this, "SGOperator_CalculateResult", true);
        auto imagePath = WidgetFun::getWidgetUserInfo(this, "SGOperator_CalculateResult", "Image");

        if(calculate.score >= 0) {
            WidgetFun::setImagic(this, "SGOperator_CalculateResult", utility::toString(imagePath, "Win.png"), false);
        } else {
            WidgetFun::setImagic(this, "SGOperator_CalculateResult", utility::toString(imagePath, "Lose.png"), false);
        }
    } else {
        WidgetFun::setVisible(this, "SGOperator_CalculateResult", false);
    }
}

void SGOperator::hideCalculate()
{
    WidgetFun::setVisible(this, "SGOperator_CalculateResult", false);
}
#pragma endregion 显示与隐藏

#pragma region 按钮逻辑处理
void SGOperator::Button_StartGame(cocos2d::Ref*, WidgetUserInfo*)
{
    SGGameScence::Instance().sendGameStart();
}

void SGOperator::Button_Ready(cocos2d::Ref*, WidgetUserInfo*)
{
    SGGameScence::Instance().sendReady();
}

void SGOperator::Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*)
{
	std::string kCCWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		utility::a_u8("三公"),(int)SGRoomInfo::Instance().getRoomInfo().dwRoomNum,utility::a_u8(SGRoomInfo::Instance().getRoomInfoText(true)));
	MissionWeiXin::Instance().shareTextWeiXin(kCCWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
}

void SGOperator::Button_SnatchBanker(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
    int snatchRatio = utility::parseInt(pInfo->kUserInfo.at("SGOperator_SnatchRatio"));
    SGGameScence::Instance().sendSnatchBanker(snatchRatio);
}

void SGOperator::Button_Call(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
    int bet = utility::parseInt(pInfo->kUserInfo.at("SGOperator_Call_Bets"));
    SGGameScence::Instance().sendBet((word)(bet));
	hideCallButtons(); 
}

void SGOperator::Button_ShowCard(cocos2d::Ref*, WidgetUserInfo*)
{
    SGGameScence::Instance().sendShowCard();
}

void SGOperator::Button_Hint(cocos2d::Ref*, WidgetUserInfo*)
{
    SGPlayerCard_Entity cards = SGGameScence::Instance().getLocalPlayer()->getPlayerCards();
    SG::SGCardType_Result result = SGGameLogic::checkSGType(cards.cards, SGRoomInfo::Instance().getRoomInfo().dwGameRuleIdex);

    //std::vector<int> temp;
    //temp.clear();

    //for(int index = 0; index < MAX_HAND_CARD; ++index) {
    //    if(result.isCardSelected[index]) {
    //        temp.push_back(index);
    //    }
    //}

    //SGGameScence::Instance().setSelectCards(temp);
}
#pragma endregion 按钮逻辑处理