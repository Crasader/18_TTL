#include "NNOperator.h"
#include "Game/Game/MissionWeiXin.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNGameScene.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNRoomInfo.h"
#include "ClientHN_THJ/Game/TTLNN/GameLogic/NNGameLogic.h"


FV_SINGLETON_STORAGE(NNOperator);
NNOperator::NNOperator()
	: m_byteSnatchRatio(0)
{
	zeromemory(&m_byteBet, sizeof(m_byteBet));
    init();
}

NNOperator::~NNOperator()
{

}

#pragma region 初始化

bool NNOperator::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();

    return true;
}

void NNOperator::initLayout()
{
    WidgetScenceXMLparse nnoperator("TTLNN/Script/NNOperator.xml", this);
}

void NNOperator::initButton()
{
    WidgetManager::addButtonCB("NNOperator_ButtonReady", this, button_selector(NNOperator::Button_Ready));
    WidgetManager::addButtonCB("NNOperator_ButtonContinue", this, button_selector(NNOperator::Button_Ready));
    //WidgetManager::addButtonCB("NNOperator_ButtonWeiXin", this, button_selector(NNOperator::Button_WeiXin));
    WidgetManager::addButtonCB("NNOperator_ButtonStartGame", this, button_selector(NNOperator::Button_StartGame));
    WidgetManager::addButtonCB("NNOperator_SntachBanker", this, button_selector(NNOperator::Button_SnatchBanker));
    WidgetManager::addButtonCB("NNOperator_Call", this, button_selector(NNOperator::Button_Call));
    WidgetManager::addButtonCB("NNOperator_ButtonShowCard", this, button_selector(NNOperator::Button_ShowCard));
	WidgetManager::addButtonCB("NNOperator_ButtonCuoCard", this, button_selector(NNOperator::Button_CuoCard));
    WidgetManager::addButtonCB("NNOperator_ButtonHint", this, button_selector(NNOperator::Button_Hint));
}
#pragma endregion 初始化

#pragma region 显示与隐藏

void NNOperator::show()
{
	m_byteSnatchRatio=0;
	m_byteBet=0;
    setVisible(true);
}

void NNOperator::hide()
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
	m_byteSnatchRatio=0;
	m_byteBet=0;
    setVisible(false);
}

void NNOperator::show(WORD status)
{
    switch(status) {
		case TTLNN::NNGameStatus_Free: {
			std::string strMasterName = "房主";
			if (NNGameScene::Instance().getMasterPlayer()) {
				strMasterName = NNGameScene::Instance().getMasterPlayer()->GetNickName();
			}
			std::string showText = utility::a_u8(utility::toString("等待", strMasterName, "确认开始游戏" ));
			showMessage(showText);

			if(NNRoomInfo::Instance().isHostPlayer(NNGameScene::Instance().getLocalPlayer())) {
				showStartGame();
			} else {
				hideStartGame();
			}
			break;
		}

		case TTLNN::NNGameStatus_HostConfirm: {
			std::string showText = utility::a_u8("等待其他玩家准备");
			showMessage(showText);
			hideStartGame();
			break;
		}

		case TTLNN::NNGameStatus_SnatchBanker: {
			if(NNGameScene::Instance().getLocalPlayer()->getPlayerStatus() != TTLNN::NNPlayerStatus_Playing) {
				std::string showText = utility::a_u8("请等待本局游戏结束");
				showMessage(showText);
				break;
			}

			if(NNGameScene::Instance().getLocalPlayer()->getSnatchBankerRatio() != (WORD)TTLNN::NNSnatchBanker_Invalid) {
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
			break;
		}

		case TTLNN::NNGameStatus_Call: {
			if(NNGameScene::Instance().getLocalPlayer()->getPlayerStatus() != TTLNN::NNPlayerStatus_Playing) {
				std::string showText = utility::a_u8("请等待本局游戏结束");
				showMessage(showText);
				break;
			}
			if(NNGameScene::Instance().isBankerUser(*NNGameScene::Instance().getLocalPlayer())) {
				std::string showText = utility::a_u8("等待其他玩家下注");
				showMessage(showText);
			} else {
				if(NNGameScene::Instance().getLocalPlayer()->getPlayerBets() != 0) {
					std::string showText = utility::a_u8("等待其他玩家下注");
					showMessage(showText);
					hideCallButtons();
				} else {
					std::string showText = utility::a_u8("请下注:");
					showMessage(showText);
					showCallButtons();
				}
			}
			break;
		}

		case TTLNN::NNGameStatus_SplitCard: {
			if( NULL != NNGameScene::Instance().getLocalPlayer() && NNGameScene::Instance().getLocalPlayer()->getPlayerStatus() != TTLNN::NNPlayerStatus_Playing) {
				std::string showText = utility::a_u8("请等待本局游戏结束");
				showMessage(showText);
				break;
			}

			if(NNGameScene::Instance().isSplitCard()) {
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
			break;
		}

		case TTLNN::NNGameStatus_Calculate: {
			if(NNGameScene::Instance().getLocalPlayer()->getPlayerStatus() != TTLNN::NNPlayerStatus_Playing) {
				std::string showText = utility::a_u8("请等待本局游戏结束");
				showMessage(showText);
				break;
			}

			hideMessage();
			hideTimes();
			showCalculate();
			break;
		}

		default:

			break;
    }

    show();
}

void NNOperator::showMessage(std::string showText)
{
    WidgetFun::setVisible(this, "NNOperator_MessageNode", true);
    WidgetFun::setText(this, "NNOperator_Message", showText);
}

void NNOperator::hideMessage()
{
    WidgetFun::setVisible(this, "NNOperator_MessageNode", false);
}

void NNOperator::showStartGame()
{
    WidgetFun::setVisible(this, "NNOperator_ButtonStartGame", true);
}

void NNOperator::hideStartGame()
{
    WidgetFun::setVisible(this, "NNOperator_ButtonStartGame", false);
}

void NNOperator::showReady()
{
    WidgetFun::setVisible(this, "NNOperator_ButtonReady", true);
	if(!WidgetFun::isWidgetVisble(this,"NNOperator_AlarmNode"))
	{
		showTimes(10);
	}
}

void NNOperator::hideReady()
{
    WidgetFun::setVisible(this, "NNOperator_ButtonReady", false);
	hideTimes();
}

void NNOperator::showContinue()
{
    WidgetFun::setVisible(this, "NNOperator_ButtonContinue", true);
	if(!WidgetFun::isWidgetVisble(this,"NNOperator_AlarmNode"))
	{
		showTimes(10);
	}
}

void NNOperator::hideContinue()
{
	WidgetFun::setVisible(this, "NNOperator_ButtonContinue", false);
	hideTimes();
}

void NNOperator::showWeiXin()
{
    //WidgetFun::setVisible(this, "NNOperator_ButtonWeiXin", true);
}

void NNOperator::hideWeiXin()
{
    //WidgetFun::setVisible(this, "NNOperator_ButtonWeiXin", false);
}

void NNOperator::showNoteTuiZhu(byte bTuiZHu)
{
	if (bTuiZHu > 0) {
		auto pTuiZhuNode = WidgetFun::getChildWidget(this, "NNOperator_TuiZhuNode");
		WidgetFun::setText(pTuiZhuNode, utility::a_u8(utility::toString("推注:", (int)bTuiZHu, "分")));
		WidgetFun::setVisible(pTuiZhuNode, true);
	}
}

void NNOperator::hideNoteTuiZhu()
{
	WidgetFun::setVisible(this, "NNOperator_TuiZhuNode", false);
}

void NNOperator::showSnatchButton()
{
    auto pSnatchNode = WidgetFun::getChildWidget(this, "NNOperator_SnatchBankerNode");
    pSnatchNode->removeAllChildren();

    auto centerPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "NNOperator_SnatchBankerNode", "CenterPos"));
    auto addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "NNOperator_SnatchBankerNode", "AddPos"));
    auto image = WidgetFun::getWidgetUserInfo(this, "NNOperator_SnatchBankerNode", "Image");

    int maxRatio = NNGameScene::Instance().getSnatchRatio();
    float centerIndex = maxRatio / 2.f;

    for(int index = 0; index <= maxRatio; ++index) {
        auto pSnatchButton = WidgetManager::Instance().createWidget("NNOperator_SntachBanker", pSnatchNode);
        float offsetIndex = index - centerIndex;
        pSnatchButton->setPosition(centerPos + addPos * offsetIndex);
        WidgetFun::setWidgetUserInfo(pSnatchButton, "NNOperator_SnatchRatio", utility::toString(index));
        WidgetFun::setButtonImagic(pSnatchButton, utility::toString(image, index, ".png"), true);
    }

    pSnatchNode->setVisible(true);
}

void NNOperator::hideSnatchButton()
{
    WidgetFun::getChildWidget(this, "NNOperator_SnatchBankerNode")->removeAllChildren();
    WidgetFun::setVisible(this, "NNOperator_SnatchBankerNode", false);
}

void NNOperator::showCallButtons()
{	
	auto pCallNode = WidgetFun::getChildWidget(this, "NNOperator_CallNode");
	pCallNode->removeAllChildren();
	auto centerPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "NNOperator_CallNode", "CenterPos"));
	auto addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "NNOperator_CallNode", "AddPos"));
	auto image = WidgetFun::getWidgetUserInfo(this, "NNOperator_CallNode", "Image");
	TTLNN::NNPlayerBet* temp = NNGameScene::Instance().getAllBets();

	for(int index = 0, posIndex = 0; index < MAX_STAKE_COUNT; ++index)
	{
		if (temp[index].wBet == 0)
			continue;
		posIndex++;

		//if (temp[index].wBetType != TTLNN::NNGameBetType::NNGBT_TuiZhu) {
			auto pSnatchButton = WidgetManager::Instance().createWidget("NNOperator_Call", pCallNode);
			pSnatchButton->setPosition(centerPos + addPos * posIndex);
			WidgetFun::setWidgetUserInfo(pSnatchButton, "NNOperator_Call_Bets", utility::toString(temp[index].wBet, "_", temp[index].wBetType));
			WidgetFun::setButtonImagic(pSnatchButton, utility::toString(image, temp[index].wBet, ".png"), true);
		//} else {
		//	auto pSnatchButton0 = WidgetManager::Instance().createWidget("NNOperator_Call_TuiZhu0", pCallNode);
		//	auto pSnatchButton1 = WidgetManager::Instance().createWidget("NNOperator_Call_TuiZhu1", pCallNode);
		//	auto pos = centerPos + addPos * posIndex;
		//	pSnatchButton0->setPosition(pos.x - 5, pos.y);
		//	pSnatchButton1->setPosition(pos.x + 5, pos.y);
		//	std::string strImagePath = WidgetFun::getWidgetUserInfo(pCallNode, "Image_TuiZhu");
		//	if (temp[index].wBet >= 10) {
		//		int first = temp[index].wBet / 10;
		//		int second = temp[index].wBet % 10;
		//		WidgetFun::setButtonImagic(pSnatchButton0, utility::toString(strImagePath, first, ".png"), false);
		//		auto* pNum2 = WidgetFun::getChildWidgetByName(pSnatchButton1, "TuiZhu_1");
		//		WidgetFun::setButtonImagic(pSnatchButton1, utility::toString(strImagePath, second, ".png"), false);
		//	}
		//	WidgetFun::setWidgetUserInfo(pSnatchButton0, "NNOperator_Call_Bets", utility::toString(temp[index].wBet, "_", temp[index].wBetType));
		//	WidgetFun::setWidgetUserInfo(pSnatchButton1, "NNOperator_Call_Bets", utility::toString(temp[index].wBet, "_", temp[index].wBetType));
		//}
	}

	pCallNode->setVisible(true);
}

void NNOperator::hideCallButtons()
{
    WidgetFun::setVisible(this, "NNOperator_CallNode", false);
}

void NNOperator::showTimes(int times)
{
    WidgetFun::getChildWidget(this, "NNOperator_AlarmNode")->stopAllActions();
    show();
    WidgetFun::setVisible(this, "NNOperator_AlarmNode", true);
    int first = times / 10;
    int second = times % 10;
    auto pAlarmNode = WidgetFun::getChildWidget(this, "NNOperator_AlarmNode");
    WidgetFun::setImagic(WidgetFun::getChildWidget(this, "NNAlarm_Timer_0"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), first, ".png"), false);
    WidgetFun::setImagic(WidgetFun::getChildWidget(this, "NNAlarm_Timer_1"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), second, ".png"), false);

    if(times > 0) {
        DelayTime* delay = DelayTime::create(1.f);
        CallFunc* func = CallFunc::create([ = ] {
            showTimes(times - 1);
        });
        Sequence* seq = Sequence::create(delay, func, NULL);
        WidgetFun::getChildWidget(this, "NNOperator_AlarmNode")->runAction(seq);
    }
}

void NNOperator::hideTimes()
{
    WidgetFun::getChildWidget(this, "NNOperator_AlarmNode")->stopAllActions();
    WidgetFun::setVisible(this, "NNOperator_AlarmNode", false);
}

void NNOperator::showSplitButton()
{
    WidgetFun::setVisible(this, "NNOperator_ButtonShowCard", true);
	WidgetFun::setVisible(this, "NNOperator_ButtonCuoCard", true);
    WidgetFun::setVisible(this, "NNOperator_ButtonHint", false);//不显提示牌按钮
}

void NNOperator::hideSplitButton()
{
    WidgetFun::setVisible(this, "NNOperator_ButtonShowCard", false);
	WidgetFun::setVisible(this, "NNOperator_ButtonCuoCard", false);
    WidgetFun::setVisible(this, "NNOperator_ButtonHint", false);
}

void NNOperator::showSplitCalculate()
{
    show();
    WidgetFun::setVisible(this, "NNOperator_CalculateNode", false);//不显示拆牌界面
}

void NNOperator::hideSplitCalculate()
{
    WidgetFun::setVisible(this, "NNOperator_CalculateNode", false);
}

void NNOperator::updateSplitCalculate()
{
    if(NNGameScene::Instance().getGameStatus() == TTLNN::NNGameStatus_SplitCard) {
        showSplitCalculate();
    } else {
        hideSplitCalculate();
        return;
    }

    NNPlayerCard_Entity cards = NNGameScene::Instance().getLocalPlayer()->getPlayerCards();
    std::vector<int> cardsSelected = NNGameScene::Instance().getSelectCards();

    int sum = 0;

    for(size_t i = 0; i < 3; i++) {
        if(i >= cardsSelected.size()) {
            WidgetFun::setVisible(this, utility::toString("NNOperator_CalculateNum_", i), false);
            continue;
        }

        auto pNode = WidgetFun::getChildWidget(this, utility::toString("NNOperator_CalculateNum_", i));
        pNode->setVisible(true);

        int cardValue = NNGameScene::Instance().getLocalPlayer()->getPlayerCards().cards[cardsSelected.at(i)] & MASK_VALUE;
        cardValue = (cardValue > 0xa ? 0xa : cardValue);
        WidgetFun::setText(pNode, cardValue);
        sum += cardValue;
    }

    if(sum == 0) {
        WidgetFun::setVisible(this, "NNOperator_CalculateNum_Sum", false);
    } else {
        WidgetFun::setVisible(this, "NNOperator_CalculateNum_Sum", true);
        WidgetFun::setText(this, "NNOperator_CalculateNum_Sum", sum);
    }
}

void NNOperator::showCalculate()
{
    NNPlayerSingleCalculate calculate = NNGameScene::Instance().getLocalPlayer()->getPlayerSingleCalculate();

    if(calculate.isValid) {
        WidgetFun::setVisible(this, "NNOperator_CalculateResult", true);
        auto imagePath = WidgetFun::getWidgetUserInfo(this, "NNOperator_CalculateResult", "Image");

        if(calculate.score >= 0) {
            WidgetFun::setImagic(this, "NNOperator_CalculateResult", utility::toString(imagePath, "Win.png"), false);
        } else {
            WidgetFun::setImagic(this, "NNOperator_CalculateResult", utility::toString(imagePath, "Lose.png"), false);
        }
    } else {
        WidgetFun::setVisible(this, "NNOperator_CalculateResult", false);
    }
}

void NNOperator::hideCalculate()
{
    WidgetFun::setVisible(this, "NNOperator_CalculateResult", false);
}
#pragma endregion 显示与隐藏

#pragma region 按钮逻辑处理
void NNOperator::Button_StartGame(cocos2d::Ref*, WidgetUserInfo*)
{
    NNGameScene::Instance().sendGameStart();
}

void NNOperator::Button_Ready(cocos2d::Ref*, WidgetUserInfo*)
{
	NNGameScene::Instance().onReady();
    NNGameScene::Instance().sendReady();
}

void NNOperator::Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*)
{
	std::string kCCWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		ScriptData<std::string>("CCWeiXinShare_Server_Name").Value(),
		(int)NNRoomInfo::Instance().getRoomInfo().dwRoomNum,
		NNRoomInfo::Instance().getRoomInfoView(true));
	MissionWeiXin::Instance().shareTextWeiXin(kCCWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
}

void NNOperator::Button_SnatchBanker(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	if(m_byteSnatchRatio == 0){
		int snatchRatio = utility::parseInt(pInfo->kUserInfo.at("NNOperator_SnatchRatio"));
		NNGameScene::Instance().sendSnatchBanker(snatchRatio);
		m_byteSnatchRatio = snatchRatio;
	}
	hideNoteTuiZhu();
}

void NNOperator::Button_Call(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	if(m_byteBet == 0) {
		//DONE:这里要设置下注的类型
		//utility::parseUserString(
		std::string strBet = pInfo->kUserInfo.at("NNOperator_Call_Bets");
		auto pos = strBet.find('_');
		if (pos != std::string::npos) {
			m_byteBet.wBet = utility::parseInt(strBet.substr(0, pos));
			m_byteBet.wBetType = utility::parseInt(strBet.substr(pos + 1, strBet.size() - pos));
		}
		NNGameScene::Instance().sendBet(m_byteBet);
		hideCallButtons();
	}
}

void NNOperator::Button_ShowCard(cocos2d::Ref*, WidgetUserInfo*)
{
    NNGameScene::Instance().sendShowCard();
}

void NNOperator::Button_CuoCard(cocos2d::Ref*, WidgetUserInfo*)
{
	NNPlayer* player = NNGameScene::Instance().getLocalPlayer();
	if (player->isValid() && player->getPlayerCards().isValid) {
		int index = MAX_HAND_CARD - 1;
		int indexCard =  NNGameLogic::getCardIndex(player->getPlayerCards().cards[index]);
		NNTurnCard::Instance().createTurnCard(indexCard);
	}
	else
	{
		NNTurnCard::Instance().createTurnCard(1);
	}
	//NNGameScene::Instance().sendShowCard();
}

void NNOperator::Button_Hint(cocos2d::Ref*, WidgetUserInfo*)
{
    NNPlayerCard_Entity cards = NNGameScene::Instance().getLocalPlayer()->getPlayerCards();
    TTLNN::NNCardType_Result result = NNGameLogic::checkNNType(cards.cards, NNRoomInfo::Instance().getRoomInfo().dwGameRuleIdex);

    std::vector<int> temp;
    temp.clear();

    for(int index = 0; index < MAX_HAND_CARD; ++index) {
        if(result.isCardSelected[index]) {
            temp.push_back(index);
        }
    }

    NNGameScene::Instance().setSelectCards(temp);
}
#pragma endregion 按钮逻辑处理