#include "SGGameScence.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"
#include "SGRoomInfo.h"
#include "SGRoomDetail.h"
#include "ClientHN_THJ/Game/SG/Player/SGPlayerPanel.h"
#include "ClientHN_THJ/Game/SG/Player/SGPlayerCard.h"
#include "ClientHN_THJ/Game/SG/Operator/SGOperator.h"
#include "ClientHN_THJ/Game/SG/Touch/SGGameScene_Touch.h"
#include "ClientHN_THJ/Game/SG/GameLogic/SGGameLogic.h"
//#include "ClientHN_THJ/Game/SG/Setting/SGSetting.h"
#include "ClientHN_THJ/Game/SG/Sound/SGSound.h"
#include "ClientHN_THJ/Game/SG/DismissRoom/SGDismissRoom.h"
#include "ClientHN_THJ/Game/SG/Calculate/SGCalculate.h"
#include "GamePlaza/GPSceneManager.h"
#include "GameLib/JniCross/JniFun.h"
#include "GamePlaza/HomeScene/SettingPanel/GPHomeSettingPanel.h"
#include "GameLib/Game/Game/MissionWeiXin.h"


FV_SINGLETON_STORAGE(SGGameScence);
SGGameScence::SGGameScence()
	:GameBase(NULL, NULL)
{
	zeromemory(m_Players, sizeof(m_Players));
	init();
}

SGGameScence::~SGGameScence()
{

}

#pragma region 初始化
bool SGGameScence::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

	initData();
    initLayout();
	initTouch();
    initButton();
	initPanel();
	initNet();
	WidgetScenceXMLparse SGChatScence("SG/Script/SGGameScence_HZ.xml",this);
	if (WidgetFun::getChildWidget(this,"GameTalkList"))
	{
		cocos2d::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
		pTalkList->removeAllChildren();
		for (int i=0;i<10;i++)
		{
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("SGHNMJGameTalkListItem",pTalkList);
			std::string kTxt = utility::getScriptString(utility::toString("GameTalkTxt",i));
			WidgetFun::setText(pItem,"TalkListItemTxt",kTxt);
			WidgetFun::setWidgetUserInfo(pItem,"SGButton_TalkDefine","Idex",utility::toString(i));
			WidgetFun::setWidgetUserInfo(pItem,"SGButton_TalkDefine","Txt",utility::toString(kTxt));
		}
		pTalkList->forceDoLayout();

		for (int m = 0;m<6;m++)
		{
			cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BiaoQing",m));
			for (int n = 0;n<8;n++)
			{
				std::string kFileName = utility::toString("Talk/EE",m*8+n,".png");
				WidgetFun::setButtonImagic(pNode,utility::toString("HNMJButton_BiaoQing",n),kFileName,true);
				WidgetFun::setWidgetUserInfo(pNode,utility::toString("HNMJButton_BiaoQing",n),"File",kFileName);
			}
		}
	}
	
    return true;
}

void SGGameScence::initLayout()
{
    WidgetScenceXMLparse SGGameScene("SG/Script/SGGameScene.xml", this);
	
}

void SGGameScence::initButton()
{
    WidgetManager::addButtonCB("SGGameScene_ButtonMenu", this, button_selector(SGGameScence::Button_Menu));
	WidgetManager::addButtonCB("SGGameScene_ButtonHelp", this, button_selector(SGGameScence::Button_Help));
    WidgetManager::addButtonCB("SGGameScene_ButtonLeave", this, button_selector(SGGameScence::Button_Exit));
	WidgetManager::addButtonCB("SGGameScene_ButtonDismiss", this, button_selector(SGGameScence::Button_Exit));
	WidgetManager::addButtonCB("SGGameScene_ButtonSetting", this, button_selector(SGGameScence::Button_Setting));
	WidgetManager::addButtonCBBegin("SGGameScene_ButtonTalk", this, button_selector(SGGameScence::Button_TalkBegin));
	WidgetManager::addButtonCB("SGGameScene_ButtonTalk", this, button_selector(SGGameScence::Button_TalkEnd));
	WidgetManager::addButtonCB("SGGameScene_ButtonDropBanker", this, button_selector(SGGameScence::Button_DropBanker));
	WidgetManager::addButtonCB("SGGameScene_ButtonMessage", this, button_selector(SGGameScence::Button_ButtonMessage));

	WidgetManager::addButtonCB("SGButton_TalkDefine",this,button_selector(SGGameScence::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(SGGameScence::Button_Send_TalkStr));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(SGGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("SGGameScene_ChongZhi",this,button_selector(SGGameScence::Button_ChongZhi));
	WidgetManager::addButtonCB("SGGameScene_ButtonFriend",this,button_selector(SGGameScence::Button_WeiXin));
	//WidgetFun::setVisible(this,"SGGameScene_ChongZhi",false);
}


void SGGameScence::Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*)
{
	std::string kCCWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		utility::a_u8("三公"),(int)SGRoomInfo::Instance().getRoomInfo().dwRoomNum,utility::a_u8(SGRoomInfo::Instance().getRoomInfoText(true)));
	MissionWeiXin::Instance().shareTextWeiXin(kCCWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
}

void SGGameScence::initPanel()
{
    if(SGRoomInfo::pInstance() == nullptr) {
        new SGRoomInfo;
    }
    addChild(SGRoomInfo::pInstance());

	if (SGPlayerCard::pInstance() == nullptr) {
		new SGPlayerCard;
	}
	addChild(SGPlayerCard::pInstance());

    if(SGPlayerPanel::pInstance() == nullptr) {
        new SGPlayerPanel;
    }
    addChild(SGPlayerPanel::pInstance());

	if (SGOperator::pInstance() == nullptr) {
		new SGOperator;
	}
	addChild(SGOperator::pInstance());

	if (SGRoomDetail::pInstance() == nullptr) {
		new SGRoomDetail;
	}
	addChild(SGRoomDetail::pInstance());

	//if (SGSetting::pInstance() == nullptr) {
	//	new SGSetting;
	//}
	//addChild(SGSetting::pInstance());
	//SGSetting::Instance().hide();
	
	if (SGDismissRoom::pInstance() == nullptr) {
		new SGDismissRoom;
	}
	addChild(SGDismissRoom::pInstance());
	SGDismissRoom::Instance().hide();

	if (GPHomeSettingPanel::pInstance() == NULL)
		new GPHomeSettingPanel;
	if (GPHomeSettingPanel::pInstance()->getParent() != NULL)
		GPHomeSettingPanel::pInstance()->removeFromParent();
	addChild(GPHomeSettingPanel::pInstance());
	GPHomeSettingPanel::Instance().hide();

	if (GPHomeMallPanel::pInstance() == nullptr)
		new GPHomeMallPanel;
	if ( GPHomeMallPanel::pInstance()->getParent() != nullptr)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());
}

void SGGameScence::initData()
{
	m_GameStatus = SG::SGGameStatus_Free;
    m_BankerUser = INVALID_CHAIR;
	m_SelectCards.clear();
	zeromemory(&m_AllBets, sizeof(m_AllBets));

    for(int  palyerIndex = 0; palyerIndex < MAX_PLAYER; palyerIndex++) {
        if(m_Players[palyerIndex] == nullptr) {
            m_Players[palyerIndex] = new SGPlayer;
        }
    }
}

void SGGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START, this, SGGameScence::onGameStart);
	addNetCBDefine(SUB_S_HOST_CONFIRM_START, this, SGGameScence::onHostConfirmStart);
	addNetCBDefine(SUB_S_SNATCH_BANKER, this, SGGameScence::onSnatchBanker);
	addNetCBDefine(SUB_S_USER_SNATCH_BANKER, this, SGGameScence::onUserSnatchBanker);
	addNetCBDefine(SUB_S_BANKER_INFO, this, SGGameScence::onBankerInfo);
	addNetCBDefine(SUB_S_USER_CALL, this, SGGameScence::onUserCall);
	addNetCBDefine(SUB_S_SPLIT_CARD, this, SGGameScence::onSplitCard);
	addNetCBDefine(SUB_S_SEND_CARD_ALL, this, SGGameScence::onSendCardAll);
	addNetCBDefine(SUB_S_SEND_CARD_ADD, this, SGGameScence::onSendCardAdd);
	addNetCBDefine(SUB_S_USER_SHOW_CARD, this, SGGameScence::onUserShowCard);
	addNetCBDefine(SUB_S_CALCULATE, this, SGGameScence::onCalculate);
	addNetCBDefine(SUB_S_NOTIFY_GAME_END, this, SGGameScence::onGameEnd);
}
#pragma endregion 初始化

#pragma region 显示与隐藏
void SGGameScence::show()
{
	if (GPHomeSettingPanel::pInstance() == NULL)
		new GPHomeSettingPanel;
	if (GPHomeSettingPanel::pInstance()->getParent() != NULL)
		GPHomeSettingPanel::pInstance()->removeFromParent();
	addChild(GPHomeSettingPanel::pInstance());
	GPHomeSettingPanel::Instance().hide();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);

	if (GPHomeMallPanel::pInstance() == nullptr)
		new GPHomeMallPanel;
	if ( GPHomeMallPanel::pInstance()->getParent() != nullptr)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());

    setVisible(true);
}

void SGGameScence::hide()
{
    setVisible(false);
}

void SGGameScence::showReady()
{
	if (SGRoomInfo::Instance().getRoomInfo().dwPlayCout == 0) {
		SGOperator::Instance().hideContinue();
		SGOperator::Instance().showReady();
	} else if (SGRoomInfo::Instance().getRoomInfo().dwPlayCout != SGRoomInfo::Instance().getRoomInfo().dwPlayTotal){
		SGOperator::Instance().hideReady();
		SGOperator::Instance().showContinue();
	} else {
		SGOperator::Instance().hideReady();
		SGOperator::Instance().hideContinue();
	}
}

void SGGameScence::hideReady()
{
	SGOperator::Instance().hideReady();
	SGOperator::Instance().hideContinue();
}

void SGGameScence::sendReady()
{
	GameBase::sendReady();
	m_SelectCards.clear();
	if (SGRoomInfo::Instance().getRoomInfo().dwPlayCout != 0) {
		for (int index = 0; index < MAX_PLAYER; ++index) {
			m_Players[index]->clearGameData();
			m_Players[index]->upPlayerInfo();
		}
	}

	if (SGRoomInfo::Instance().getRoomInfo().dwPlayCout != 0) {
		SGOperator::Instance().hide();
	}
	SGOperator::Instance().show(m_GameStatus);
}

void SGGameScence::showDropBanker()
{
	WidgetFun::setVisible(this, "SGGameScene_ButtonDropBanker", true);
}

void SGGameScence::hideDropBanker()
{
	WidgetFun::setVisible(this, "SGGameScene_ButtonDropBanker", false);
}

//扣税收显示(仅首局显示)
void SGGameScence::showGameTax(int iGameTax)
{
	if(iGameTax==0)
		return;
	std::string kStr = utility::toString(utility::a_u8("本局服务费"),iGameTax);
	WidgetFun::setText(this,"GameTax",kStr);
	WidgetFun::runWidgetAction(this,"GameTax","Start");
}
#pragma endregion 显示与隐藏


#pragma region 进入场景
void SGGameScence::enterScene()
{
    resetData();
    clearPanel();
    show();
	SoundFun::Instance().playBackMusic("bgroom.mp3");

	//SGSound::playBackMusic("BackMusic.mp3");
}

void SGGameScence::clearPanel()
{
	hideDropBanker();
    hideMenuPanel();
	SGOperator::Instance().hide();
	SGDismissRoom::Instance().hide();
	//SGSetting::Instance().hide();
	
	GPHomeSettingPanel::Instance().hide();
	SGRoomDetail::Instance().hide();
}

void SGGameScence::hideMenuPanel()
{
    if(WidgetFun::isWidgetVisble(this, "SGGameScene_MenuNode")) {
        WidgetFun::setVisible(this, "SGGameScene_MenuNode", false);
    }
}

void SGGameScence::resetData()
{
	m_GameStatus = SG::SGGameStatus_Free;
	m_BankerUser = INVALID_CHAIR;
	m_SelectCards.clear();
	zeromemory(m_AllBets, sizeof(m_AllBets));
	for (int playerIndex = 0; playerIndex < MAX_PLAYER; playerIndex++) {
		m_Players[playerIndex]->setValid(false);
		m_Players[playerIndex]->clearGameData();
		SGPlayerPanel::Instance().hidePlayer(playerIndex);
		SGPlayerCard::Instance().hidePlayer(playerIndex);
	}
}
#pragma endregion 进入场景

#pragma region 玩家相关
GamePlayer* SGGameScence::CreatePlayer(IClientUserItem * pIClientUserItem)
{
    if(pIClientUserItem->GetChairID() >= MAX_PLAYER) {
        return NULL;
    }

    GamePlayer* player = m_Players[pIClientUserItem->GetChairID()];
    player->setUserItem(pIClientUserItem);
    return player;
}

void SGGameScence::updateUserInfo()
{
    for(int index = 0; index < MAX_PLAYER; ++index) {
        m_Players[index]->upPlayerInfo();
		SGPlayerPanel::Instance().playEffectSGPlayer_Snatch(*m_Players[index],false);
    }
}

SGPlayer* SGGameScence::getLocalPlayer()
{
    for(int index = 0; index < MAX_PLAYER; ++index) {
        SGPlayer* player = m_Players[index];

        if(player->GetUserID() == UserInfo::Instance().getUserID()) {
            return player;
        }
    }

    return NULL;
}

SGPlayer* SGGameScence::getPlayerByChairID(WORD chairID)
{
    if(chairID < MAX_PLAYER) {
        return m_Players[chairID];
    }

    return NULL;
}

int SGGameScence::getVisioChairID(SGPlayer& player)
{
// 	if (m_InRecord) {
// 		return (MAX_PLAYER * MAX_PLAYER - player.GetChairID() + getLocalPlayer()->GetChairID()) % MAX_PLAYER;
// 	}

    ASSERT(getLocalPlayer()->getUserItem(false) && player.getUserItem(false));
    return (MAX_PLAYER * MAX_PLAYER - player.GetChairID() + getLocalPlayer()->GetChairID()) % MAX_PLAYER;
}

bool SGGameScence::isBankerUser(SGPlayer& player)
{
    return player.GetChairID() == m_BankerUser && m_BankerUser != INVALID_CHAIR;
}
#pragma endregion 玩家相关

#pragma region 游戏状态
bool SGGameScence::IsInGame()
{
    return false;
}

WORD SGGameScence::getGameStatus()
{
	return m_GameStatus;
}

WORD* SGGameScence::getAllBets()
{
	return m_AllBets;
}

WORD SGGameScence::getSnatchRatio()
{
	return m_MaxRatio;
}

WORD SGGameScence::getBankerRatio()
{
	return m_BankerRatio;
}


#pragma endregion 游戏状态

#pragma region 按钮消息
void SGGameScence::Button_Menu(cocos2d::Ref*, WidgetUserInfo*)
{
    if(WidgetFun::isWidgetVisble(this, "SGGameScene_MenuNode")) {
        WidgetFun::setVisible(this, "SGGameScene_MenuNode", false);
    } else {
        WidgetFun::setVisible(this, "SGGameScene_MenuNode", true);
    }
}

void SGGameScence::Button_Help(cocos2d::Ref*, WidgetUserInfo*)
{
	SGRoomDetail::Instance().show();
}

void SGGameScence::Button_Exit(cocos2d::Ref*, WidgetUserInfo*)
{
	Button_ExitGameBase(NULL, NULL);
	/*if (SGRoomInfo::Instance().getRoomInfo().dwPlayCout == 0){
	if (SGRoomInfo::Instance().isHostPlayer(*getLocalPlayer())) {
	SGDismissRoom::Instance().show();
	} else {
	Button_ExitGameBase(NULL, NULL);
	}
	} else if (SGRoomInfo::Instance().getRoomInfo().dwPlayCout != SGRoomInfo::Instance().getRoomInfo().dwPlayTotal){
	SGDismissRoom::Instance().show(SG_DismissRoom_InGameSelf);
	} else {
	Button_ExitGameBase(NULL, NULL);
	}*/
}

void SGGameScence::Button_Setting(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
}

void SGGameScence::Button_TalkBegin(cocos2d::Ref*, WidgetUserInfo*)
{
	if (cocos2d::UserDefault::getInstance()->getBoolForKey("isRecordStart", false)) {
		JniFun::stopSoundRecord();
		cocos2d::UserDefault::getInstance()->setBoolForKey("isRecordStart", false);
		return;
	}
	SoundFun::Instance().PaseBackMusic();

	cocos2d::UserDefault::getInstance()->setBoolForKey("isRecordStart", true);
	JniFun::startSoundRecord();
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SGGameScence::TalkEnd, this), 5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this, "SGGameScene_ButtonTalk", "TimeID", utility::toString(iTimeID));
}

void SGGameScence::TalkEnd()
{
	Button_TalkEnd(NULL, NULL);
}

void SGGameScence::Button_TalkEnd(cocos2d::Ref*, WidgetUserInfo*)
{
	SoundFun::Instance().ResumeBackMusic();
	if (!cocos2d::UserDefault::getInstance()->getBoolForKey("isRecordStart", false)) {
		return;
	}
	cocos2d::UserDefault::getInstance()->setBoolForKey("isRecordStart", false);
	int iTimeID = utility::parseInt(WidgetFun::getWidgetUserInfo(this, "SG_GameScene_ButtonTalk", "TimeID"));
	TimeManager::Instance().removeByID(iTimeID);
	std::string kFileName = JniFun::stopSoundRecord();
	sendTalkFile(getLocalPlayer()->GetChairID(), kFileName);
}

void SGGameScence::Button_DropBanker(cocos2d::Ref*, WidgetUserInfo*)
{
	SendSocketData(SUB_C_USER_DROP_BANKER);
}

void SGGameScence::Button_ButtonMessage(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"SGGameTalkPlane",true);
}

#pragma region 聊天相关
void SGGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = SGSound::getDefineSound(UserInfo::Instance().getGender(),utility::toString(iUserIdex));
	sendTalkDefine(getLocalPlayer()->GetChairID(),utility::toString(kTxt,":",kTaskStr));
	WidgetFun::setVisible(this,"SGGameTalkPlane",false);
}

void SGGameScence::Button_Send_TalkStr( cocos2d::Ref*,WidgetUserInfo* )
{
		return;
	std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
	sendTalkString(getLocalPlayer()->GetChairID(),kTxt);
	WidgetFun::setVisible(this,"SGGameTalkPlane",false);
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}

void SGGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	return;
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(getLocalPlayer()->GetChairID(),kFile);
	WidgetFun::setVisible(this,"SGGameTalkPlane",false);
}

void SGGameScence::Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*)
{
	if (GPHomeMallPanel::pInstance() == nullptr)
		new GPHomeMallPanel;
	if (GPHomeMallPanel::pInstance() != nullptr)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());
	GPHomeMallPanel::Instance().show();
}
#pragma endregion 按钮消息

#pragma region 房间信息
void SGGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	memset(&m_RoomInfo, 0, sizeof(m_RoomInfo));
	m_RoomInfo = *pNetInfo;

	SGRoomInfo::Instance().setRoomInfo(*pNetInfo);
	SGRoomInfo::Instance().updateRoomInfo();
	SGRoomInfo::Instance().show();

	updateUserInfo();
	SGOperator::Instance().show(m_GameStatus);
}

void SGGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	if (SGRoomInfo::Instance().getRoomInfo().dwPlayCout == 0) {
		return;
	}

	if (pNetInfo->dwNotAgreeUserCout != 0) {
		std::string info = utility::a_u8("玩家");
		info.append(m_Players[pNetInfo->dwNotAgreeChairID[0]]->GetNickName());
		info.append(utility::a_u8("拒绝解散游戏!"));
		NoticeMsg::Instance().ShowTopMsg(info);
	}

	if (pNetInfo->dwDissUserCout == 0) {
		SGDismissRoom::Instance().hide();
		return;
	}

	if (pNetInfo->dwDissChairID[0] == getLocalPlayer()->GetChairID()) {
		SGDismissRoom::Instance().show(SG_DismissRoom_InGameSelf, pNetInfo);
	} else {
		SGDismissRoom::Instance().show(SG_DismissRoom_InGameOthers, pNetInfo);
	}
}

void SGGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	GPSceneManager::Instance().InSGCalculate(pNetInfo);
}
#pragma endregion 房间信息

#pragma region 分牌触摸消息处理
void SGGameScence::initTouch()
{
	SGGameScene_Touch* pTouchScene = new SGGameScene_Touch;
	pTouchScene->init();
	addChild(pTouchScene);
}

bool SGGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!isSplitCard()) {
		return false;
	}

	int cardIndex = SGPlayerCard::Instance().getTouchCard(kPos);

	if (-1 == cardIndex) {
		return false;
	}

	std::vector<int>::iterator itor = find(m_SelectCards.begin(), m_SelectCards.end(),cardIndex);

	if (itor != m_SelectCards.end()) {
		m_SelectCards.erase(itor);
		getLocalPlayer()->upPlayerInfo();
		SGOperator::Instance().updateSplitCalculate();
		return true;
	}

	if (m_SelectCards.size() >= 3) {
		return false;
	}

	m_SelectCards.push_back(cardIndex);
	std::sort(m_SelectCards.begin(), m_SelectCards.end());
	getLocalPlayer()->upPlayerInfo();
	SGOperator::Instance().updateSplitCalculate();
	return true;
}

bool SGGameScence::isSplitCard()
{
	return m_GameStatus == SG::SGGameStatus_SplitCard && getLocalPlayer()->getPlayerCardType().type == SG::SGCardType_Invalid;
}

void SGGameScence::setSelectCards(std::vector<int> selectCards)
{
	std::swap(m_SelectCards, selectCards);
	sort(m_SelectCards.begin(), m_SelectCards.end());
	SGOperator::Instance().updateSplitCalculate();
	updateUserInfo();
}

std::vector<int> SGGameScence::getSelectCards()
{
	return m_SelectCards;
}
#pragma endregion 触摸消息处理

#pragma region 游戏消息收发
void SGGameScence::sendGameStart()
{
	SendSocketData(SUB_C_HOST_ALLOW_START);
}

void SGGameScence::sendSnatchBanker(BYTE snatchRatio)
{
	SG::CMD_C_UserSnatchBanker snatchBanker;
	zeromemory(&snatchBanker, sizeof(snatchBanker));
	snatchBanker.ratio = snatchRatio;
	SendSocketData(SUB_C_USER_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
}

void SGGameScence::sendBet(WORD bet)
{
	SG::CMD_C_UserCall userCall;
	zeromemory(&userCall, sizeof(userCall));
	userCall.playerBets = bet;
	SendSocketData(SUB_C_USER_CALL, &userCall, sizeof(userCall));
}

void SGGameScence::sendShowCard()
{
	SGPlayerCard_Entity cards = SGGameScence::Instance().getLocalPlayer()->getPlayerCards();
	SG::SGCardType_Result result = SGGameLogic::checkSGType(cards.cards, SGRoomInfo::Instance().getRoomInfo().dwGameRuleIdex);
	SG::CMD_C_UserShowCard showCard;
	zeromemory(&showCard, sizeof(showCard));
	showCard.result = result;
	SendSocketData(SUB_C_USER_SHOW_CARD, &showCard, sizeof(showCard));
}

void SGGameScence::onGameStart(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_GameStart));
	if (wDataSize != sizeof(SG::CMD_S_GameStart)) {
		return;
	}

	m_GameStatus = SG::SGGameStatus_Start;
	SG::CMD_S_GameStart* pGameStart = (SG::CMD_S_GameStart*)pBuffer;
	for (int index = 0; index < MAX_PLAYER; ++index)
	{
		m_Players[index]->setPlayerStatus(pGameStart->status[index]);
	}

	SGPlayer* pLocalPlayer = getPlayerByChairID(m_pSelfPlayer->GetChairID());
	pLocalPlayer->setPlayCount(pLocalPlayer->getPlayCount()+1);

	//DONE: 取消扣除服务费
#ifdef SHOW_SERVICE_CORE
	//扣税收显示(仅首局显示)
	if(pLocalPlayer->getPlayCount()==1){
		int iGameTax = -0.3 * m_RoomInfo.dwBaseScore;
		showGameTax(iGameTax);
	}
#endif

}

void SGGameScence::onHostConfirmStart(const void * pBuffer, word wDataSize)
{
	m_GameStatus = SG::SGGameStatus_HostConfirm;
	SGOperator::Instance().show(m_GameStatus);
	SGSound::playEffect(SGSound::BUTTON_EFFECT);
}

void SGGameScence::onSnatchBanker(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_SnatchBanker));
	if (wDataSize != sizeof(SG::CMD_S_SnatchBanker)) {
		return;
	}

	m_GameStatus = SG::SGGameStatus_SnatchBanker;
	
	SG::CMD_S_SnatchBanker* pInfo = (SG::CMD_S_SnatchBanker*) pBuffer;
	
	for (int index = 0; index < MAX_PLAYER; ++index) {
		if (pInfo->status[index] == SG::SGPlayerStatus_Playing) {
			m_Players[index]->setPlayerCards(pInfo->cards[index], pInfo->cardCount);
		}
	}

	m_MaxRatio = pInfo->maxRatio;
	SGSound::playEffect(SGSound::GAME_START);
	SGPlayerCard::Instance().sendPlayerCard();
}

void SGGameScence::onUserSnatchBanker(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_UserSnatchBanker));
	if (wDataSize != sizeof(SG::CMD_S_UserSnatchBanker)) {
		return;
	}

	SG::CMD_S_UserSnatchBanker* pInfo = (SG::CMD_S_UserSnatchBanker*) pBuffer;
	m_Players[pInfo->userChairID]->setSnatchBankerRatio(pInfo->ratio);
	m_Players[pInfo->userChairID]->upPlayerInfo();
	SGOperator::Instance().show(m_GameStatus);
	SGSound::playEffect(SGSound::BUTTON_EFFECT);
	SGPlayerPanel::Instance().playEffectSGPlayer_Snatch(*m_Players[pInfo->userChairID],true);
}

void SGGameScence::onBankerInfo(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_BankerInfo));
	if (wDataSize != sizeof(SG::CMD_S_BankerInfo)) {
		return;
	}
	
	m_GameStatus = SG::SGGameStatus_Call;

	SG::CMD_S_BankerInfo* pInfo = (SG::CMD_S_BankerInfo*) pBuffer;
	m_BankerUser = pInfo->bankerChairID;
	m_BankerRatio = pInfo->bankerRatio;
	ASSERT(sizeof(pInfo->bets) == sizeof(m_AllBets));
	memcpy(m_AllBets, pInfo->bets, sizeof(m_AllBets));
	if (pInfo->bankerRatio > SG::SGSnatchBanker_None) {
		for (int index = 0; index < MAX_PLAYER; ++index) {
			m_Players[index]->setSnatchBankerRatio(SG::SGSnatchBanker_Invalid);
		}

		if (m_BankerUser < MAX_PLAYER) {
			m_Players[m_BankerUser]->setSnatchBankerRatio(pInfo->bankerRatio);
		}
	}

	if (SGRoomInfo::Instance().getRoomInfo().bGameTypeIdex == SG::SGGameType_AllCompare) {
		for (int index = 0; index < MAX_PLAYER; ++index) {
			m_Players[index]->setPlayerBets(m_AllBets[0]);
		}
	}
	updateUserInfo();
	SGOperator::Instance().show(m_GameStatus);
	SGOperator::Instance().showTimes(TIME_FOR_USER_CALL);
	SGSound::playEffect(SGSound::GAME_START);
}

void SGGameScence::onUserCall(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_UserCall));
	if (wDataSize != sizeof(SG::CMD_S_UserCall)) {
		return;
	}

	SG::CMD_S_UserCall* pInfo = (SG::CMD_S_UserCall*) pBuffer;
	if (pInfo->chairID >= MAX_PLAYER) {
		return;
	}

	m_Players[pInfo->chairID]->setPlayerBets(pInfo->playerBets);
	updateUserInfo();
	SGOperator::Instance().show(m_GameStatus);
	SGSound::playEffect(SGSound::USER_CALL);
	if (getLocalPlayer()->GetChairID() == pInfo->chairID )
	{
		SGOperator::Instance().hideCallButtons();
	}
}

void SGGameScence::onSplitCard(const void* pBuffer, word wDataSize)
{
	m_GameStatus = SG::SGGameStatus_SplitCard;
	SGOperator::Instance().show(m_GameStatus);
	SGOperator::Instance().hideCallButtons();
}

void SGGameScence::onSendCardAll(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_SendCardAll));
	if (wDataSize != sizeof(SG::CMD_S_SendCardAll)) {
		return;
	}

	SG::CMD_S_SendCardAll* pInfo = (SG::CMD_S_SendCardAll*) pBuffer;
	for (int index = 0; index < MAX_PLAYER; ++index) {
		if (pInfo->playerStatus[index] == SG::SGPlayerStatus_Playing) {
			m_Players[index]->setPlayerCards(pInfo->playerCards[index], MAX_HAND_CARD);
		}
	}

	m_GameStatus = SG::SGGameStatus_SplitCard;
	SGPlayerCard::Instance().sendPlayerCard();
}

void SGGameScence::onSendCardAdd(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_SendCardAdd));
	if (wDataSize != sizeof(SG::CMD_S_SendCardAdd)) {
		return;
	}

	SG::CMD_S_SendCardAdd* pInfo = (SG::CMD_S_SendCardAdd*) pBuffer;
	for (int index = 0; index < MAX_PLAYER; ++index) {
		if (pInfo->playerStatus[index] == SG::SGPlayerStatus_Playing) {
			SGPlayerCard_Entity entity = m_Players[index]->getPlayerCards();
			m_Players[index]->addPlayerCard(pInfo->playerCards[index]);
		}
	}

	m_GameStatus = SG::SGGameStatus_SplitCard;
	SGPlayerCard::Instance().sendPlayerCardAdd();
	SGOperator::Instance().hideCallButtons();
}

void SGGameScence::onUserShowCard(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_UserShowCard));
	if (wDataSize != sizeof(SG::CMD_S_UserShowCard)) {
		return;
	}

	SG::CMD_S_UserShowCard* pInfo = (SG::CMD_S_UserShowCard*) pBuffer;
	m_Players[pInfo->chairID]->setPlayerCards(pInfo->playerCards, MAX_HAND_CARD);
	m_Players[pInfo->chairID]->setPlayerCardType(pInfo->result);
	m_Players[pInfo->chairID]->upPlayerInfo();
	SGOperator::Instance().show(m_GameStatus);
	SGOperator::Instance().hideCallButtons();
	SGSound::playSG(pInfo->result,m_Players[pInfo->chairID]);
}

void SGGameScence::onCalculate(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(SG::CMD_S_Calculate));
	if (wDataSize != sizeof(SG::CMD_S_Calculate)) {
		return;
	}

	SG::CMD_S_Calculate* pInfo = (SG::CMD_S_Calculate*) pBuffer;
	for (int index = 0; index < MAX_PLAYER; ++index) {
		m_Players[index]->setPlayerSingleCalculate(pInfo->playerScores[index]);
	}
	m_GameStatus = SG::SGGameStatus_Calculate;
	SGOperator::Instance().show(m_GameStatus);

	if (getLocalPlayer()->getPlayerSingleCalculate().score >= 0) {
		SGSound::playEffect(SGSound::SG_WIN);
	} else {
		SGSound::playEffect(SGSound::SG_LOSE);
	}
	updateUserInfo();
}

void SGGameScence::onGameEnd(const void* pBuffer, word wDataSize)
{
	m_GameStatus = SG::SGGameStatus_HostConfirm;
	updateUserInfo();
}

bool SGGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	ASSERT(dataSize == sizeof(SG::CMD_S_GameScene));
	if (dataSize != sizeof(SG::CMD_S_GameScene)) {
		return false;
	}

	SG::CMD_S_GameScene* pGameScene = (SG::CMD_S_GameScene*)data;
	m_GameStatus = pGameScene->gameStatus;
	m_BankerUser = pGameScene->bankerChairID;
	m_MaxRatio = pGameScene->maxRatio;
	
	for (int index = 0; index < MAX_PLAYER; ++index) {
		m_Players[index]->setSnatchBankerRatio(SG::SGSnatchBanker_Invalid);
	}

	if (m_BankerUser < MAX_PLAYER && pGameScene->bankerRatio != 0) {
		m_Players[m_BankerUser]->setSnatchBankerRatio(pGameScene->bankerRatio);
	}

	memcpy(m_AllBets, pGameScene->playerBetsAllow, sizeof(m_AllBets));

	SGOperator::Instance().hide();
    for(int index = 0; index < MAX_PLAYER; ++index) {
        m_Players[index]->clearGameData();
        m_Players[index]->setPlayerStatus(pGameScene->playerStatus[index]);

        if (m_Players[index]->getPlayerStatus() == SG::SGPlayerStatus_Playing) {
            m_Players[index]->setPlayerBets(pGameScene->playerBet[index]);
            m_Players[index]->setPlayerCardType(pGameScene->playerCardTypes[index]);
            if (pGameScene->cardCount > 0) {
                m_Players[index]->setPlayerCards(pGameScene->playerCards[index], pGameScene->cardCount);
            }
        }

        if(m_GameStatus == SG::SGGameType_SnatchBanker) {
            m_Players[index]->setSnatchBankerRatio(pGameScene->playerSnatchRatio[index]);
        }

        m_Players[index]->upPlayerInfo();
    }

	SGOperator::Instance().show(m_GameStatus);
	
	return true;
}
#pragma endregion 游戏消息收发
