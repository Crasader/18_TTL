#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"
#include "GameLib/JniCross/JniFun.h"
#include "GameLib/Game/Script/WindowInfo.h"
#include "GameLib/Game/Game/MissionWeiXin.h"
#include "GamePlaza/HomeScene/SettingPanel/GPHomeSettingPanel.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "GamePlaza/GPSceneManager.h"

#include "NNGameScene.h"
#include "NNRoomInfo.h"
#include "NNRoomDetail.h"
#include "ClientHN_THJ/Game/TTLNN/Player/NNPlayerPanel.h"
#include "ClientHN_THJ/Game/TTLNN/Player/NNPlayerCard.h"
#include "ClientHN_THJ/Game/TTLNN/Operator/NNOperator.h"
#include "ClientHN_THJ/Game/TTLNN/Touch/NNGameScene_Touch.h"
#include "ClientHN_THJ/Game/TTLNN/GameLogic/NNGameLogic.h"
#include "ClientHN_THJ/Game/TTLNN/Sound/NNSound.h"
#include "ClientHN_THJ/Game/TTLNN/DismissRoom/NNDismissRoom.h"
#include "ClientHN_THJ/Game/TTLNN/Calculate/NNCalculate.h"
#include "ClientHN_THJ/Game/TTLNN/TurnCardAction/TurnCardNode.h"

FV_SINGLETON_STORAGE(NNGameScene);

//最小说话时间1秒,少于1秒可能会引起崩溃
#define MIN_SPEAK_TIME 1.f
//最大说话时间
#define MAX_SPEAK_TIME 5.f
//说话的时间间隔, 间隔太短会引起初始化的问题
#define INTERVAL_SPEAK_TIME 1.f

dword NNGameScene::_dwSpeak_time_begin = 0;
dword NNGameScene::_dwSpeak_time_end = 0;
dword NNGameScene::_dwSpeak_time_interval = 0;

NNGameScene::NNGameScene()
	:GameBase(NULL, NULL)
{
	zeromemory(m_Players, sizeof(m_Players));
	init();
}

NNGameScene::~NNGameScene()
{

}

#pragma region 初始化
bool NNGameScene::init()
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
	initTalkSence();

    return true;
}

void NNGameScene::initTalkSence()
{
	WidgetScenceXMLparse nnChatScence("TTLNN/Script/NNGameTalkScence.xml", this);
	if (WidgetFun::getChildWidget(this, "GameTalkList")) {
		cocos2d::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this, "GameTalkList");
		pTalkList->removeAllChildren();
		for (int i = 0; i < 10; i++) {
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("NNHNMJGameTalkListItem", pTalkList);
			std::string kTxt = utility::getScriptString(utility::toString("GameTalkTxt", i));
			WidgetFun::setText(pItem, "TalkListItemTxt", kTxt);
			WidgetFun::setWidgetUserInfo(pItem, "NNButton_TalkDefine", "Idex", utility::toString(i));
			WidgetFun::setWidgetUserInfo(pItem, "NNButton_TalkDefine", "Txt", utility::toString(kTxt));
		}
		pTalkList->forceDoLayout();
	}

	for (int m = 0; m < 6; m++) {
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this, utility::toString("BiaoQing", m));
		for (int n = 0; n < 8; n++) {
			std::string kFileName = utility::toString("Talk/EE", m * 8 + n, ".png");
			WidgetFun::setButtonImagic(pNode, utility::toString("HNMJButton_BiaoQing", n), kFileName, true);
			WidgetFun::setWidgetUserInfo(pNode, utility::toString("HNMJButton_BiaoQing", n), "File", kFileName);
		}
	}
}

void NNGameScene::initLayout()
{
    WidgetScenceXMLparse nnGameScene("TTLNN/Script/NNGameScene.xml", this);
}

void NNGameScene::initButton()
{
    WidgetManager::addButtonCB("NNGameScene_ButtonMenu", this, button_selector(NNGameScene::Button_Menu));
	WidgetManager::addButtonCB("NNGameScene_ButtonHelp", this, button_selector(NNGameScene::Button_Help));
    WidgetManager::addButtonCB("NNGameScene_ButtonLeave", this, button_selector(NNGameScene::Button_Exit));
	WidgetManager::addButtonCB("NNGameScene_ButtonDismiss", this, button_selector(NNGameScene::Button_Dismiss));
	WidgetManager::addButtonCB("NNGameScene_ButtonSetting", this, button_selector(NNGameScene::Button_Setting));
	WidgetManager::addButtonCBBegin("NNGameScene_ButtonTalk", this, button_selector(NNGameScene::Button_TalkBegin));
	WidgetManager::addButtonCB("NNGameScene_ButtonTalk", this, button_selector(NNGameScene::Button_TalkEnd));
	WidgetManager::addButtonCB("NNGameScene_ButtonDropBanker", this, button_selector(NNGameScene::Button_DropBanker));
	WidgetManager::addButtonCB("NNGameScene_ButtonMessage", this, button_selector(NNGameScene::Button_ButtonMessage));
	WidgetManager::addButtonCB("NNButton_TalkDefine",this,button_selector(NNGameScene::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(NNGameScene::Button_Send_TalkStr));
	WidgetManager::addButtonCB("Button_TalkType_List", this, button_selector(NNGameScene::Button_TalkType_List));
	WidgetManager::addButtonCB("Button_TalkType_BiaoQing", this, button_selector(NNGameScene::Button_TalkType_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(NNGameScene::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("NNGameScene_ChongZhi",this,button_selector(NNGameScene::Button_ChongZhi));
	WidgetManager::addButtonCB("NNGameScene_ButtonShare",this,button_selector(NNGameScene::Button_Share));
	
}

void NNGameScene::initPublicPanel()
{
	//全局面板
	if (GPHomeSettingPanel::pInstance()->getParent() != this) {
		GPHomeSettingPanel::pInstance()->removeFromParent();
		addChild(GPHomeSettingPanel::pInstance());
	}
	if (GPHomeMallPanel::pInstance()->getParent() != this) {
		GPHomeMallPanel::pInstance()->removeFromParent();
		addChild(GPHomeMallPanel::pInstance());
	}
}

void NNGameScene::initPanel()
{
	initPublicPanel();

	//////////////////////////////////////////////////////////////////////////

    addChild(NNRoomInfo::pInstance());
	addChild(NNPlayerCard::pInstance());
    addChild(NNPlayerPanel::pInstance());
	addChild(NNOperator::pInstance());
	addChild(NNRoomDetail::pInstance());
	addChild(NNDismissRoom::pInstance());
	addChild(NNTurnCard::pInstance());

	NNTurnCard::Instance().hide();
}

void NNGameScene::initData()
{
	m_GameStatus = TTLNN::NNGameStatus_Free;
    m_BankerUser = INVALID_CHAIR;
	m_SelectCards.clear();
	zeromemory(&m_AllBets, sizeof(m_AllBets));

    for(int  palyerIndex = 0; palyerIndex < MAX_PLAYER; palyerIndex++) {
        if(m_Players[palyerIndex] == nullptr) {
            m_Players[palyerIndex] = new NNPlayer;
        }
    }
}

void NNGameScene::initNet()
{
	addNetCBDefine(SUB_S_GAME_START, this, NNGameScene::onGameStart);
	addNetCBDefine(SUB_S_HOST_CONFIRM_START, this, NNGameScene::onHostConfirmStart);
	addNetCBDefine(SUB_S_SNATCH_BANKER, this, NNGameScene::onSnatchBanker);
	addNetCBDefine(SUB_S_USER_SNATCH_BANKER, this, NNGameScene::onUserSnatchBanker);
	addNetCBDefine(SUB_S_BANKER_INFO, this, NNGameScene::onBankerInfo);
	addNetCBDefine(SUB_S_USER_CALL, this, NNGameScene::onUserCall);
	addNetCBDefine(SUB_S_SPLIT_CARD, this, NNGameScene::onSplitCard);
	addNetCBDefine(SUB_S_SEND_CARD_ALL, this, NNGameScene::onSendCardAll);
	addNetCBDefine(SUB_S_SEND_CARD_ADD, this, NNGameScene::onSendCardAdd);
	addNetCBDefine(SUB_S_USER_SHOW_CARD, this, NNGameScene::onUserShowCard);
	addNetCBDefine(SUB_S_CALCULATE, this, NNGameScene::onCalculate);
	addNetCBDefine(SUB_S_NOTIFY_GAME_END, this, NNGameScene::onGameEnd);
	addNetCBDefine(SUB_S_CALCULATE_TOTAL, this, NNGameScene::onCalculateToTal);
}

#pragma endregion 初始化

#pragma region 显示与隐藏
void NNGameScene::show()
{
	initPublicPanel();

	GPHomeSettingPanel::Instance().hide();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);

    setVisible(true);
}

void NNGameScene::hide()
{
    setVisible(false);
}

void NNGameScene::showReady()
{
	if (NNRoomInfo::Instance().getRoomInfo().dwPlayCout == 0) {
		NNOperator::Instance().hideContinue();
		NNOperator::Instance().showReady();
	} else if (NNRoomInfo::Instance().getRoomInfo().dwPlayCout != NNRoomInfo::Instance().getRoomInfo().dwPlayTotal) {
		NNOperator::Instance().hideReady();
		NNOperator::Instance().showContinue();
	} else {
		NNOperator::Instance().hideReady();
		NNOperator::Instance().hideContinue();
	}
}

void NNGameScene::hideReady()
{
	NNOperator::Instance().hideReady();
	NNOperator::Instance().hideContinue();
}

void NNGameScene::onReady()
{
	m_SelectCards.clear();
	if (NNRoomInfo::Instance().getRoomInfo().dwPlayCout != 0) {
		for (int index = 0; index < MAX_PLAYER; ++index) {
			m_Players[index]->clearGameData();
			m_Players[index]->upPlayerInfo();
		}
	}
	if (NNRoomInfo::Instance().getRoomInfo().dwPlayCout != 0) {
		NNOperator::Instance().hide();
	}
	NNOperator::Instance().show(m_GameStatus);
	NNOperator::Instance().hideCalculate();
	NNOperator::Instance().hideReady();
}

void NNGameScene::sendReady()
{
	GameBase::sendReady();
}

void NNGameScene::showDropBanker()
{
	WidgetFun::setVisible(this, "NNGameScene_ButtonDropBanker", true);
}

void NNGameScene::hideDropBanker()
{
	WidgetFun::setVisible(this, "NNGameScene_ButtonDropBanker", false);
}

//扣税收显示(仅首局显示)
void NNGameScene::showGameTax(int iGameTax)
{
	if(iGameTax==0)
		return;
	std::string kStr = utility::toString(utility::a_u8("本局服务费"),iGameTax);
	WidgetFun::setText(this,"GameTax",kStr);
	WidgetFun::runWidgetAction(this,"GameTax","Start");
}
#pragma endregion 显示与隐藏


#pragma region 进入场景
void NNGameScene::enterScene()
{
    resetData();
    clearPanel();
    show();
	SoundFun::Instance().playBackMusic("bgroom.mp3");
	//NNSound::playBackMusic("BackMusic.mp3");
}

void NNGameScene::clearPanel()
{
	hideDropBanker();
    hideMenuPanel();
	NNOperator::Instance().hide();
	NNDismissRoom::Instance().hide();
	NNTurnCard::Instance().hide();
	//NNSetting::Instance().hide();
	GPHomeSettingPanel::Instance().hide();
	NNRoomDetail::Instance().hide();
}

void NNGameScene::hideMenuPanel()
{
    if(WidgetFun::isWidgetVisble(this, "NNGameScene_MenuNode")) {
        WidgetFun::setVisible(this, "NNGameScene_MenuNode", false);
    }
}

void NNGameScene::resetData()
{
	m_GameStatus = TTLNN::NNGameStatus_Free;
	m_BankerUser = INVALID_CHAIR;
	m_SelectCards.clear();
	zeromemory(m_AllBets, sizeof(m_AllBets));
	for (int playerIndex = 0; playerIndex < MAX_PLAYER; playerIndex++) {
		m_Players[playerIndex]->setValid(false);
		m_Players[playerIndex]->clearGameData();
		NNPlayerPanel::Instance().hidePlayer(playerIndex);
		NNPlayerCard::Instance().hidePlayer(playerIndex);
	}
}
#pragma endregion 进入场景

#pragma region 玩家相关
GamePlayer* NNGameScene::CreatePlayer(IClientUserItem * pIClientUserItem)
{
    if(pIClientUserItem->GetChairID() >= MAX_PLAYER) {
        return NULL;
    }

    GamePlayer* player = m_Players[pIClientUserItem->GetChairID()];
    player->setUserItem(pIClientUserItem);
    return player;
}

void NNGameScene::updateUserInfo()
{
    for(int index = 0; index < MAX_PLAYER; ++index) {
        m_Players[index]->upPlayerInfo();
    }
}

int NNGameScene::getGamePlayerCount()
{
	int count = 0;
	for (int index = 0; index < MAX_PLAYER; ++index) {
		if (m_Players[index]->isValid()) {
			count++;
		}
	}
	return count;
}

NNPlayer* NNGameScene::getLocalPlayer()
{
    for(int index = 0; index < MAX_PLAYER; ++index) {
        NNPlayer* player = m_Players[index];

        if(player->GetUserID() == UserInfo::Instance().getUserID()) {
            return player;
        }
    }

    return NULL;
}

NNPlayer* NNGameScene::getPlayerByChairID(WORD chairID)
{
    if(chairID < MAX_PLAYER) {
        return m_Players[chairID];
    }

    return NULL;
}

int NNGameScene::getVisioChairID(NNPlayer& player)
{
// 	if (m_InRecord) {
// 		return (MAX_PLAYER * MAX_PLAYER - player.GetChairID() + getLocalPlayer()->GetChairID()) % MAX_PLAYER;
// 	}

    ASSERT(getLocalPlayer()->getUserItem(false) && player.getUserItem(false));
    return (MAX_PLAYER * MAX_PLAYER - player.GetChairID() + getLocalPlayer()->GetChairID()) % MAX_PLAYER;
}

bool NNGameScene::isBankerUser(NNPlayer& player)
{
    return player.GetChairID() == m_BankerUser && m_BankerUser != INVALID_CHAIR;
}
#pragma endregion 玩家相关

#pragma region 游戏状态
bool NNGameScene::IsInGame()
{
    return false;
}

WORD NNGameScene::getGameStatus()
{
	return m_GameStatus;
}

TTLNN::NNPlayerBet* NNGameScene::getAllBets()
{
	return m_AllBets;
}

WORD NNGameScene::getSnatchRatio()
{
	return m_MaxRatio;
}

WORD NNGameScene::getBankerRatio()
{
	return m_BankerRatio;
}


#pragma endregion 游戏状态

#pragma region 按钮消息
void NNGameScene::Button_Menu(cocos2d::Ref*, WidgetUserInfo*)
{
    if(WidgetFun::isWidgetVisble(this, "NNGameScene_MenuNode")) {
        WidgetFun::setVisible(this, "NNGameScene_MenuNode", false);
    } else {
        WidgetFun::setVisible(this, "NNGameScene_MenuNode", true);
    }
}

void NNGameScene::Button_Help(cocos2d::Ref*, WidgetUserInfo*)
{
	NNRoomDetail::Instance().show();
}

void NNGameScene::Button_Exit(cocos2d::Ref*, WidgetUserInfo*)
{
#ifdef ENABLE_DISMISS_ROOM
	//解散房间
	//1.只有房主在房间不解散, 直接回到大厅
	auto self_player = getLocalPlayer();
	bool isHost = NNRoomInfo::Instance().isHostPlayer(*self_player);//房主
	if (getGamePlayerCount() == 1 && isHost) {
		Button_ExitGameBase(NULL, NULL);
		return;
	}
	//2.如果游戏还没有开始
	if (NNRoomInfo::Instance().getRoomInfo().dwPlayCout == 0) {
		if (isHost) {
			NNDismissRoom::Instance().show();
		} else {
			Button_ExitGameBase(NULL, NULL);
		}
		return;
	}
	//3.如果游戏已经开始
	NNDismissRoom::Instance().show(NN_DismissRoom_InGameSelf);
	//if (NNRoomInfo::Instance().getRoomInfo().dwPlayCout != NNRoomInfo::Instance().getRoomInfo().dwPlayTotal) {
	//} else {
	//	Button_ExitGameBase(NULL, NULL);
	//}
#else
	Button_ExitGameBase(NULL, NULL);
#endif // ENABLE_DISMISS_ROOM
}

void NNGameScene::Button_Dismiss(cocos2d::Ref* ref, WidgetUserInfo* pinfo)
{
	auto self_player = getLocalPlayer();
	bool isHost = NNRoomInfo::Instance().isHostPlayer(*self_player);//房主
	if (isHost) {
		NNDismissRoom::Instance().show(NN_DismissRoom_InGameSelf);
	} else {
		Button_Exit(ref, pinfo);
	}
}

void NNGameScene::Button_Setting(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
}

void NNGameScene::Button_TalkBegin(cocos2d::Ref*, WidgetUserInfo*)
{
	//说话已经开始
	if (_dwSpeak_time_begin != 0) {
		return;
	}
	//间隔太短
	if (_dwSpeak_time_interval != 0) {
		return;
	}
	if (_dwSpeak_time_begin == 0) {
		_dwSpeak_time_begin = time(nullptr);
	}

	SoundFun::Instance().PaseBackMusic();
	JniFun::startSoundRecord();
	//最大时间到了自动结束说话
	int iTimeID_max = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NNGameScene::TalkEnd, this), MAX_SPEAK_TIME)->iIdex;
	WidgetFun::setWidgetUserInfo(this, "NNGameScene_ButtonTalk", "iTimeID_max", utility::toString(iTimeID_max));
}

void NNGameScene::TalkEnd()
{
	Button_TalkEnd(NULL, NULL);
}

void NNGameScene::TalkInterval()
{
	_dwSpeak_time_interval = 0;
	std::string strTimeIDInterval = WidgetFun::getWidgetUserInfo(this, "NNGameScene_ButtonTalk", "iTimeID_interval");
	if (strTimeIDInterval != WidgetNotFindUserInfo) {
		TimeManager::Instance().removeByID(utility::parseInt(strTimeIDInterval));
		CCLOG("removeByID(utility::parseInt(strTimeIDInterval))");
	}
}

void NNGameScene::Button_TalkEnd(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_dwSpeak_time_begin == 0) {
		return;
	}
	_dwSpeak_time_end = time(nullptr);
	if (_dwSpeak_time_end - _dwSpeak_time_begin < MIN_SPEAK_TIME) {
		CCLOG("TalkEnd _dwSpeak_time_end - _dwSpeak_time_begin < MIN_SPEAK_TIME");
		std::string strTimeIDMin = WidgetFun::getWidgetUserInfo(this, "NNGameScene_ButtonTalk", "iTimeID_min");
		//如果狂点, 可能会多次触发最小时间计时器, 只让他生效一次
		if (strTimeIDMin == WidgetNotFindUserInfo) {
			int nTimeID_min = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NNGameScene::TalkEnd, this), MIN_SPEAK_TIME)->iIdex;
			WidgetFun::setWidgetUserInfo(this, "NNGameScene_ButtonTalk", "iTimeID_min", utility::toString(nTimeID_min));
		}
		return;
	}
	std::string TimeID_min = WidgetFun::getWidgetUserInfo(this, "NNGameScene_ButtonTalk", "iTimeID_min");
	if (TimeID_min != WidgetNotFindUserInfo) {
		TimeManager::Instance().removeByID(utility::parseInt(TimeID_min));
		CCLOG("removeByID(utility::parseInt(TimeID_min))");
	}
	std::string strTimeID_max = WidgetFun::getWidgetUserInfo(this, "NNGameScene_ButtonTalk", "iTimeID_max");
	if (strTimeID_max != WidgetNotFindUserInfo) {
		TimeManager::Instance().removeByID(utility::parseInt(strTimeID_max));
		CCLOG("removeByID(utility::parseInt(strTimeID_max))");
	}
	_dwSpeak_time_begin = 0;
	_dwSpeak_time_end = 0;
	_dwSpeak_time_interval = INTERVAL_SPEAK_TIME;
	int nTimeID_interval = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(NNGameScene::TalkInterval, this), _dwSpeak_time_interval)->iIdex;
	WidgetFun::setWidgetUserInfo(this, "NNGameScene_ButtonTalk", "iTimeID_interval", utility::toString(nTimeID_interval));

	SoundFun::Instance().ResumeBackMusic();
	std::string kFileName = JniFun::stopSoundRecord();
	if (kFileName != "") {
		sendTalkFile(getLocalPlayer()->GetChairID(), kFileName);
	}
}

void NNGameScene::Button_DropBanker(cocos2d::Ref*, WidgetUserInfo*)
{
	SendSocketData(SUB_C_USER_DROP_BANKER);
}

void NNGameScene::Button_ButtonMessage(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"NNGameTalkPlane",true);
}

void NNGameScene::Button_TalkType_List(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setVisible(this, "TalkTypePlane0", true);
	WidgetFun::setVisible(this, "TalkTypePlane1", false);
}

void NNGameScene::Button_TalkType_BiaoQing(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setVisible(this, "TalkTypePlane0", false);
	WidgetFun::setVisible(this, "TalkTypePlane1", true);
}

#pragma region 聊天相关
void NNGameScene::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = NNSound::getDefineSound(UserInfo::Instance().getGender(),utility::toString(iUserIdex));
	sendTalkDefine(getLocalPlayer()->GetChairID(),utility::toString(kTxt,":",kTaskStr));
	WidgetFun::setVisible(this,"NNGameTalkPlane",false);
}

void NNGameScene::Button_Send_TalkStr( cocos2d::Ref*,WidgetUserInfo* )
{
		return;
	std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
	sendTalkString(getLocalPlayer()->GetChairID(),kTxt);
	WidgetFun::setVisible(this,"NNGameTalkPlane",false);
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}

void NNGameScene::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(getLocalPlayer()->GetChairID(),kFile);
	WidgetFun::setVisible(this,"NNGameTalkPlane",false);
}

void NNGameScene::Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*)
{
	if (GPHomeMallPanel::pInstance() != nullptr)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());
	GPHomeMallPanel::Instance().show();
}

void NNGameScene::Button_Share(cocos2d::Ref*, WidgetUserInfo*)
{
	std::string strUrl = utility::a_u8(utility::toString("http://114.115.164.158:8080/evokeapp.html?refresh=0&room_id=", (int)m_RoomInfo.dwRoomNum));

	std::string strPepleNum = utility::toString(getGamePlayerCount(), "/", (int)m_RoomInfo.bMaxPeopleNum);
	std::string strRoomType = "";
	switch (m_RoomInfo.bGameTypeIdex) {
		case TTLNN::NNGameType_NNBanker: {
			strRoomType.append(utility::a_u8("牛牛上庄"));
			break;
		}
		case TTLNN::NNGameType_HostBanker: {
			strRoomType.append(utility::a_u8("固定庄家"));
			break;
		}
		case TTLNN::NNGameType_SnatchBanker: {
			strRoomType.append(utility::a_u8("自由抢庄"));
			break;
		}
		case TTLNN::NNGameType_SnatchBankerShowCard: {
			strRoomType.append(utility::a_u8("明牌抢庄"));
			break;
		}
		case TTLNN::NNGameType_AllCompare: {
			strRoomType.append(utility::a_u8("通比牛牛"));
			break;
		}
		case TTLNN::NNGameType_NNRatio: {
			strRoomType.append(utility::a_u8("牛几赔几"));
			break;
		}
		default:
			break;
	}

	std::string strBaseScore = "";
	if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_0))) {
		strBaseScore.append("1");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			strBaseScore.append("/2");
		}
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_1))) {
		strBaseScore.append("2");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			strBaseScore.append("/4");
		}
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_2))) {
		strBaseScore.append("4");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			strBaseScore.append("/8");
		}
	} else {
		strBaseScore.append("5");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			strBaseScore.append("/10");
		}
	}
	auto game_info = GPGameLink::Instance().getCurrentGameInfo();
	std::string strRound = "";
	strRound = utility::toString(game_info.bPlayCout[m_RoomInfo.bPlayCoutIdex]);

	std::string strTitle = utility::getScriptReplaceValue("CCWeiXinSharTitle",
		ScriptData<std::string>("CCWeiXinShare_Server_Name").Value(),
		m_RoomInfo.dwRoomNum,
		strPepleNum,
		strRoomType,
		strBaseScore,
		strRound); 

	//倍数
	int bNNBeiShu;
	//牛牛4倍
	if(FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Ratio_0))) {
		bNNBeiShu = 4;
	} else {
		bNNBeiShu = 3;
	}
	
	int nTuiZhu = 0;
	if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_0))) {
		nTuiZhu = TuiZhuBeiShu_0;
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_1))) {
		nTuiZhu = TuiZhuBeiShu_1;
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_2))) {
		nTuiZhu = TuiZhuBeiShu_2;
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_3))) {
		nTuiZhu = TuiZhuBeiShu_3;
	}

	auto* player =  getLocalPlayer();
	std::string stDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		player->GetNickName(),
		bNNBeiShu,
		nTuiZhu);

	MissionWeiXin::Instance().shareUrlWeiXin(strUrl, strTitle, stDes);
}

#pragma endregion 按钮消息

#pragma region 房间信息
void NNGameScene::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	m_RoomInfo = *pNetInfo;

	NNRoomInfo::Instance().setRoomInfo(*pNetInfo);
	NNRoomInfo::Instance().updateRoomInfo();
	NNRoomInfo::Instance().show();

	updateUserInfo();
	NNOperator::Instance().show(m_GameStatus);


	auto self_player = getLocalPlayer();
	bool isHost = NNRoomInfo::Instance().isHostPlayer(*self_player);//房主
	//如果是房主
	if (isHost) {
		WidgetFun::setVisible(this, "NNGameScene_ButtonDismiss", true);
	} else {
		WidgetFun::setVisible(this, "NNGameScene_ButtonDismiss", false);
	}

}

void NNGameScene::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	if (NNRoomInfo::Instance().getRoomInfo().dwPlayCout == 0) {
		return;
	}

	if (pNetInfo->dwNotAgreeUserCout != 0) {
		std::string info = utility::a_u8("玩家");
		info.append(m_Players[pNetInfo->dwNotAgreeChairID[0]]->GetNickName());
		info.append(utility::a_u8("拒绝解散游戏!"));
		NoticeMsg::Instance().ShowTopMsg(info);
	}

	if (pNetInfo->dwDissUserCout == 0) {
		NNDismissRoom::Instance().hide();
		return;
	}

	if (pNetInfo->dwDissChairID[0] == getLocalPlayer()->GetChairID()) {
		NNDismissRoom::Instance().show(NN_DismissRoom_InGameSelf, pNetInfo);
	} else {
		NNDismissRoom::Instance().show(NN_DismissRoom_InGameOthers, pNetInfo);
	}
}

void NNGameScene::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	GPSceneManager::Instance().InNNCalculate(pNetInfo);
}
#pragma endregion 房间信息

#pragma region 分牌触摸消息处理
void NNGameScene::initTouch()
{
	NNGameScene_Touch* pTouchScene = new NNGameScene_Touch;
	pTouchScene->init();
	addChild(pTouchScene);
}

bool NNGameScene::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!isSplitCard()) {
		return false;
	}

	int cardIndex = NNPlayerCard::Instance().getTouchCard(kPos);

	if (-1 == cardIndex) {
		return false;
	}

	std::vector<int>::iterator itor = find(m_SelectCards.begin(), m_SelectCards.end(),cardIndex);

	if (itor != m_SelectCards.end()) {
		m_SelectCards.erase(itor);
		getLocalPlayer()->upPlayerInfo();
		NNOperator::Instance().updateSplitCalculate();
		return true;
	}

	if (m_SelectCards.size() >= 3) {
		return false;
	}

	m_SelectCards.push_back(cardIndex);
	std::sort(m_SelectCards.begin(), m_SelectCards.end());
	getLocalPlayer()->upPlayerInfo();
	NNOperator::Instance().updateSplitCalculate();
	return true;
}

bool NNGameScene::isSplitCard()
{
	return m_GameStatus == TTLNN::NNGameStatus_SplitCard && getLocalPlayer()->getPlayerCardType().type == TTLNN::NNCardType_Invalid;
}

void NNGameScene::setSelectCards(std::vector<int> selectCards)
{
	std::swap(m_SelectCards, selectCards);
	sort(m_SelectCards.begin(), m_SelectCards.end());
	NNOperator::Instance().updateSplitCalculate();
	updateUserInfo();
}

std::vector<int> NNGameScene::getSelectCards()
{
	return m_SelectCards;
}
#pragma endregion 触摸消息处理

#pragma region 游戏消息收发
void NNGameScene::sendGameStart()
{
	SendSocketData(SUB_C_HOST_ALLOW_START);
}

void NNGameScene::sendSnatchBanker(BYTE snatchRatio)
{
	TTLNN::CMD_C_UserSnatchBanker snatchBanker;
	zeromemory(&snatchBanker, sizeof(snatchBanker));
	snatchBanker.ratio = snatchRatio;
	SendSocketData(SUB_C_USER_SNATCH_BANKER, &snatchBanker, sizeof(snatchBanker));
}

void NNGameScene::sendBet(TTLNN::NNPlayerBet& bet)
{
	TTLNN::CMD_C_UserCall userCall;
	zeromemory(&userCall, sizeof(userCall));
	userCall.playerBets = bet;
	SendSocketData(SUB_C_USER_CALL, &userCall, sizeof(userCall));
}

void NNGameScene::sendShowCard()
{
	//亮牌移除最后一张牌
	//NNPlayerCard_Entity cards = NNGameScene::Instance().getLocalPlayer()->getPlayerCards();
	//TTLNN::NNCardType_Result result = NNGameLogic::checkNNType(cards.cards, NNRoomInfo::Instance().getRoomInfo().dwGameRuleIdex);
	TTLNN::CMD_C_UserShowCard showCard;
	zeromemory(&showCard, sizeof(showCard));

	//if (result.type < TTLNN::NNCardType_WHN && result.type != TTLNN::NNCardType_None && m_SelectCards.size() == 3) {
	//	int sum = 0;
	//	for (int index = 0; index < (int)m_SelectCards.size(); ++index) {
	//		int cardValue = cards.cards[m_SelectCards.at(index)] & MASK_VALUE;
	//		cardValue = cardValue > 0xa ? 0xa : cardValue;
	//		sum += cardValue;
	//	}

	//	if (sum % 10 == 0) {
	//		zeromemory(result.isCardSelected, sizeof(result.isCardSelected));
	//		result.isCardSelected[m_SelectCards.at(0)] = true;
	//		result.isCardSelected[m_SelectCards.at(1)] = true;
	//		result.isCardSelected[m_SelectCards.at(2)] = true;
	//	}
	//}
	//showCard.result = result;
	SendSocketData(SUB_C_USER_SHOW_CARD, &showCard, sizeof(showCard));
}

void NNGameScene::onGameStart(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_GameStart));
	if (wDataSize != sizeof(TTLNN::CMD_S_GameStart)) {
		return;
	}

	m_GameStatus = TTLNN::NNGameStatus_Start;
	TTLNN::CMD_S_GameStart* pGameStart = (TTLNN::CMD_S_GameStart*)pBuffer;
	for (int index = 0; index < MAX_PLAYER; ++index)
	{
		m_Players[index]->setPlayerStatus(pGameStart->status[index]);
	}

	NNPlayer* pLocalPlayer = getPlayerByChairID(m_pSelfPlayer->GetChairID());
	pLocalPlayer->setPlayCount(pLocalPlayer->getPlayCount()+1);

#ifdef SHOW_SERVICE_CORE
	//扣税收显示(仅首局显示)
	if(pLocalPlayer->getPlayCount()==1){
		int iGameTax = -0.3 * m_RoomInfo.dwBaseScore;
		showGameTax(iGameTax);
	}
#endif

}

void NNGameScene::onHostConfirmStart(const void * pBuffer, word wDataSize)
{
	m_GameStatus = TTLNN::NNGameStatus_HostConfirm;
	NNOperator::Instance().show(m_GameStatus);
	NNSound::playEffect(NNSound::BUTTON_EFFECT);
}

void NNGameScene::onSnatchBanker(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_SnatchBanker));
	if (wDataSize != sizeof(TTLNN::CMD_S_SnatchBanker)) {
		return;
	}

	m_GameStatus = TTLNN::NNGameStatus_SnatchBanker;
	
	TTLNN::CMD_S_SnatchBanker* pInfo = (TTLNN::CMD_S_SnatchBanker*) pBuffer;
	
	for (int index = 0; index < MAX_PLAYER; ++index) {
		if (pInfo->status[index] == TTLNN::NNPlayerStatus_Playing) {
			m_Players[index]->setPlayerCards(pInfo->cards[index], pInfo->cardCount);
		}
	}

	m_MaxRatio = pInfo->maxRatio;
	NNSound::playEffect(NNSound::GAME_START);
	NNPlayerCard::Instance().sendPlayerCard();
}

void NNGameScene::onUserSnatchBanker(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_UserSnatchBanker));
	if (wDataSize != sizeof(TTLNN::CMD_S_UserSnatchBanker)) {
		return;
	}

	TTLNN::CMD_S_UserSnatchBanker* pInfo = (TTLNN::CMD_S_UserSnatchBanker*) pBuffer;
	m_Players[pInfo->userChairID]->setSnatchBankerRatio(pInfo->ratio);
	m_Players[pInfo->userChairID]->upPlayerInfo();
	NNOperator::Instance().show(m_GameStatus);
	NNSound::playEffect(NNSound::BUTTON_EFFECT);
}

void NNGameScene::onBankerInfo(const void * pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_BankerInfo));
	if (wDataSize != sizeof(TTLNN::CMD_S_BankerInfo)) {
		return;
	}
	
	m_GameStatus = TTLNN::NNGameStatus_Call;

	TTLNN::CMD_S_BankerInfo* pInfo = (TTLNN::CMD_S_BankerInfo*) pBuffer;
	m_BankerUser = pInfo->bankerChairID;
	m_BankerRatio = pInfo->bankerRatio;

	int local_chair_id = 0;
	auto player = getLocalPlayer();
	if (player)
		local_chair_id = player->GetChairID();

	ASSERT(sizeof(pInfo->bets[local_chair_id]) == sizeof(m_AllBets));
	memcpy(m_AllBets, pInfo->bets[local_chair_id], sizeof(m_AllBets));

	if (pInfo->bankerRatio > TTLNN::NNSnatchBanker_None) {
		for (int index = 0; index < MAX_PLAYER; ++index) {
			m_Players[index]->setSnatchBankerRatio(TTLNN::NNSnatchBanker_Invalid);
		}

		if (m_BankerUser < MAX_PLAYER) {
			m_Players[m_BankerUser]->setSnatchBankerRatio(pInfo->bankerRatio);
		}
	}

	if (NNRoomInfo::Instance().getRoomInfo().bGameTypeIdex == TTLNN::NNGameType_AllCompare) {
		for (int index = 0; index < MAX_PLAYER; ++index) {
			m_Players[index]->setPlayerBets(m_AllBets[0]);
		}
	}
	updateUserInfo();
	NNOperator::Instance().show(m_GameStatus);
	NNOperator::Instance().showTimes(TIME_FOR_USER_CALL);
	NNSound::playEffect(NNSound::GAME_START);
}

void NNGameScene::onUserCall(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_UserCall));
	if (wDataSize != sizeof(TTLNN::CMD_S_UserCall)) {
		return;
	}

	TTLNN::CMD_S_UserCall* pInfo = (TTLNN::CMD_S_UserCall*) pBuffer;
	if (pInfo->chairID >= MAX_PLAYER) {
		return;
	}

	m_Players[pInfo->chairID]->setPlayerBets(pInfo->playerBets);
	updateUserInfo();
	NNOperator::Instance().show(m_GameStatus);
	NNSound::playEffect(NNSound::USER_CALL);
}

void NNGameScene::onSplitCard(const void* pBuffer, word wDataSize)
{
	m_GameStatus = TTLNN::NNGameStatus_SplitCard;
	NNOperator::Instance().show(m_GameStatus);
}

void NNGameScene::onSendCardAll(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_SendCardAll));
	if (wDataSize != sizeof(TTLNN::CMD_S_SendCardAll)) {
		return;
	}

	TTLNN::CMD_S_SendCardAll* pInfo = (TTLNN::CMD_S_SendCardAll*) pBuffer;
	for (int index = 0; index < MAX_PLAYER; ++index) {
		if (pInfo->playerStatus[index] == TTLNN::NNPlayerStatus_Playing) {
			m_Players[index]->setPlayerCards(pInfo->playerCards[index], MAX_HAND_CARD);
		}
	}

	m_GameStatus = TTLNN::NNGameStatus_SplitCard;
	NNPlayerCard::Instance().sendPlayerCard();
}

void NNGameScene::onSendCardAdd(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_SendCardAdd));
	if (wDataSize != sizeof(TTLNN::CMD_S_SendCardAdd)) {
		return;
	}

	TTLNN::CMD_S_SendCardAdd* pInfo = (TTLNN::CMD_S_SendCardAdd*) pBuffer;
	for (int index = 0; index < MAX_PLAYER; ++index) {
		if (pInfo->playerStatus[index] == TTLNN::NNPlayerStatus_Playing) {
			NNPlayerCard_Entity entity = m_Players[index]->getPlayerCards();
			m_Players[index]->addPlayerCard(pInfo->playerCards[index]);
		}
	}

	m_GameStatus = TTLNN::NNGameStatus_SplitCard;
	NNPlayerCard::Instance().sendPlayerCardAdd();
}

void NNGameScene::onUserShowCard(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_UserShowCard));
	if (wDataSize != sizeof(TTLNN::CMD_S_UserShowCard)) {
		return;
	}

	TTLNN::CMD_S_UserShowCard* pInfo = (TTLNN::CMD_S_UserShowCard*) pBuffer;
	m_Players[pInfo->chairID]->setPlayerCards(pInfo->playerCards, MAX_HAND_CARD);
	m_Players[pInfo->chairID]->setPlayerCardType(pInfo->result);
	m_Players[pInfo->chairID]->upPlayerInfo();
	if(m_Players[pInfo->chairID]->GetUserID() == getLocalPlayer()->GetUserID())
	{	
		NNTurnCard::Instance().hide();
		NNOperator::Instance().show(m_GameStatus);
	}
	NNSound::playNN(pInfo->result.type,m_Players[pInfo->chairID]);
}

void NNGameScene::onCalculate(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_Calculate));
	if (wDataSize != sizeof(TTLNN::CMD_S_Calculate)) {
		return;
	}

	TTLNN::CMD_S_Calculate* pInfo = (TTLNN::CMD_S_Calculate*) pBuffer;
	NNTurnCard::Instance().hide();
	for (int index = 0; index < MAX_PLAYER; ++index) {
		m_Players[index]->setPlayerSingleCalculate(pInfo->playerScores[index]);
	}
	m_GameStatus = TTLNN::NNGameStatus_Calculate;
	NNOperator::Instance().show(m_GameStatus);

	if (getLocalPlayer()->getPlayerSingleCalculate().score >= 0) {
		NNSound::playEffect(NNSound::NN_WIN);
	} else {
		NNSound::playEffect(NNSound::NN_LOSE);
	}
	updateUserInfo();
}

void NNGameScene::onGameEnd(const void* pBuffer, word wDataSize)
{
	m_GameStatus = TTLNN::NNGameStatus_HostConfirm;
	updateUserInfo();
}

void NNGameScene::onCalculateToTal(const void* pBuffer, word wDataSize)
{
	ASSERT(wDataSize == sizeof(TTLNN::CMD_S_CalculateTotal));
	if (wDataSize != sizeof(TTLNN::CMD_S_CalculateTotal)) {
		CCAssert(false, "wDataSize != sizeof(TTLNN::CMD_S_CalculateTotal)");
		return;
	}
	//NNCalculate::Instance().show();
}

bool NNGameScene::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	ASSERT(dataSize == sizeof(TTLNN::CMD_S_GameScene));
	if (dataSize != sizeof(TTLNN::CMD_S_GameScene)) {
		return false;
	}
	TTLNN::CMD_S_GameScene* pGameScene = (TTLNN::CMD_S_GameScene*)data;
	m_GameStatus = pGameScene->gameStatus;
	m_BankerUser = pGameScene->bankerChairID;
	m_MaxRatio = pGameScene->maxRatio;
	NNRoomInfo::Instance().setRoomInfoBase(pGameScene->dwBaseScore);
	m_BankerRatio= pGameScene->bankerRatio;
	if (m_BankerUser < MAX_PLAYER && pGameScene->bankerRatio != 0) {
		m_Players[m_BankerUser]->setSnatchBankerRatio(pGameScene->bankerRatio);
	}
	memcpy(m_AllBets, pGameScene->playerBetsAllow, sizeof(m_AllBets));
	NNOperator::Instance().hide();
    for(int index = 0; index < MAX_PLAYER; ++index) {
        m_Players[index]->clearGameData();
        m_Players[index]->setPlayerStatus(pGameScene->playerStatus[index]);
        if (m_Players[index]->getPlayerStatus() == TTLNN::NNPlayerStatus_Playing) {
            m_Players[index]->setPlayerBets(pGameScene->playerBet[index]);
            m_Players[index]->setPlayerCardType(pGameScene->playerCardTypes[index]);
            if (pGameScene->cardCount > 0) {
                m_Players[index]->setPlayerCards(pGameScene->playerCards[index], pGameScene->cardCount);
            }
        }
        if(m_GameStatus == TTLNN::NNGameStatus_SnatchBanker) {
            m_Players[index]->setSnatchBankerRatio(pGameScene->playerSnatchRatio[index]);
        }

        m_Players[index]->upPlayerInfo();
    }
	NNOperator::Instance().show(m_GameStatus);
	return true;
}
#pragma endregion 游戏消息收发
