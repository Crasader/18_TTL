#include "TLJGameScence.h"
#include "Animation/TLJAnimation.h"
#include "Calculate/TLJCalculatePanel.h"
#include "Calculate/TLJCalculateDetailPanel.h"
#include "ChipContainer/TLJChipContainer.h"
#include "DismissRoom/TLJDismissRoom.h"
#include "DismissRoom/TLJDismissRoomConfirm.h"
#include "Logic/TLJGameLogic.h"
#include "Player/TLJPlayer.h"
#include "Player/TLJPlayer_Card.h"
#include "Player/TLJPlayer_Panel.h"
//#include "Setting/TLJSettingPanel.h"
#include "SoundFun/TLJSoundFun.h"
#include "Touch/TLJGameScence_Touch.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "GamePlaza/GameManager/GPGameManager.h"
#include "GamePlaza/GPSceneManager.h"
#include "Game/Game/MissionWeiXin.h"
#include "SimpleAudioEngine.h"
#include "Info/TLJInfoPanel.h"
#include "Operate/TLJOperate.h"
//#include "Setting/TLJSettingPanel.h"
#include "HelpInfo/TLJRoomDetail.h"
#include "GameLib/JniCross/JniFun.h"
#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeCreateRoomPanel_TLJ.h"
#include "GamePlaza/HomeScene/MallPanel/GPHomeMallPanel.h"
#include "GamePlaza/HomeScene/SettingPanel/GPHomeSettingPanel.h"

FV_SINGLETON_STORAGE(TLJGameScence);
TLJGameScence::TLJGameScence()
	:GameBase(0, 0)
	, m_CurrentUser(INVALID_CHAIR)
	, m_BankerUser(INVALID_CHAIR)
	, m_IsInGame(false)
	, m_cbScoreLevel(0)
	, m_wCurrentTimes(0)
	, m_wGameState(TLJ_STATUS_NULL)
	, m_wCurrentTurn(0)
	, m_lTotalScore(0)
{
	init();
}
TLJGameScence::~TLJGameScence()
{

}
bool TLJGameScence::init()
{
	if (!GameBase::init()) {
		return false;
	};

	WidgetScenceXMLparse gameScene("Game/TLJ/Script/TLJGameScence.xml", this);

	initPanel();
	initTouch();
	initButton();
	initNet();

	WidgetScenceXMLparse kScence("Game/TLJ/Script/ZJMJGamePrivateScence.xml",this);
	WidgetScenceXMLparse kScence2("Game/TLJ/xml/GameScence_HZ.xml",this);
	for(int i=0;i<TLJ_GAME_PLAYER;i++)
	{
		m_pPlayers[i] = new TLJPlayer;
	}

	if (WidgetFun::getChildWidget(this,"GameTalkList"))
	{
		cocos2d::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
		pTalkList->removeAllChildren();
		for (int i=0;i<10;i++)
		{
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("TLJHNMJGameTalkListItem",pTalkList);
			std::string kTxt = utility::getScriptString(utility::toString("GameTalkTxt",i));
			WidgetFun::setText(pItem,"TalkListItemTxt",kTxt);
			WidgetFun::setWidgetUserInfo(pItem,"TLJButton_TalkDefine","Idex",utility::toString(i));
			WidgetFun::setWidgetUserInfo(pItem,"TLJButton_TalkDefine","Txt",utility::toString(kTxt));
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

#pragma region 游戏场景
//显示场景
void TLJGameScence::show()
{

	if (GPHomeSettingPanel::pInstance()->getParent() != nullptr)
		GPHomeSettingPanel::pInstance()->removeFromParent();
	addChild(GPHomeSettingPanel::pInstance());
	GPHomeSettingPanel::Instance().hide();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
	setVisible(true); 
}
//隐藏场景
void TLJGameScence::hide()
{
	setVisible(false);
}
//进入场景
void TLJGameScence::EnterScence()
{
	m_IsInGame = false;
	hideAllPanels();
	ResetGameClient();
	show();
	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Game/TLJ/sound/csmj.mp3", true);

	//TLJCalculatePanel::Instance().show();
	SoundFun::Instance().playBackMusic("bgroom.mp3");
}
//重置场景
void TLJGameScence::resetScene()
{
	for (int index = 0; index < MAX_PLAYER; ++index) 
	{
		m_pPlayers[index]->setValid(false);
		m_pPlayers[index]->reset();
		TLJPlayer_Card::Instance().hideCard(index);
	}
}
//重置游戏
void TLJGameScence::ResetGameClient()
{
	for (int index = 0; index < MAX_PLAYER; ++index) {
		m_pPlayers[index]->reset();
		m_pPlayers[index]->upPlayerInfo();
		TLJPlayer_Card::Instance().hideCard(index);
		TLJPlayer_Panel::Instance().changeMaskVisible(index,false);
	}
	m_wCurrentTimes = 1;
	m_cbScoreLevel = 1;
	m_wCurrentTurn = 0;
	m_lTotalScore = 0;
	setGameState(TLJ_STATUS_NULL);
}
#pragma endregion 

#pragma region 游戏界面
//界面初始化
void TLJGameScence::initPanel()
{
	addChild(TLJPlayer_Card::pInstance());
	addChild(TLJChipContainer::pInstance());
	addChild(TLJOperate::pInstance());
	addChild(TLJPlayer_Panel::pInstance());
	addChild(TLJAnimation::pInstance());
	addChild(TLJDismissRoom::pInstance());
	addChild(TLJDismissRoomConfirm::pInstance());
	if (GPHomeSettingPanel::pInstance()->getParent() != nullptr)
		GPHomeSettingPanel::pInstance()->removeFromParent();
	addChild(GPHomeSettingPanel::pInstance());
	GPHomeSettingPanel::Instance().hide();
	addChild(TLJCalculatePanel::pInstance());
	addChild(TLJCalculateDetailPanel::pInstance());
	addChild(TLJInfoPanel::pInstance());
	addChild(TLJRoomDetail::pInstance());
	if (GPHomeMallPanel::pInstance() != nullptr)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());
	hideAllPanels();
}
//隐藏界面
void TLJGameScence::hideAllPanels()
{
	TLJChipContainer::Instance().hide();
	TLJAnimation::Instance().hide();
	TLJCalculatePanel::Instance().hide();
	TLJDismissRoom::Instance().hide();
	TLJDismissRoomConfirm::Instance().hide();
	TLJCalculateDetailPanel::Instance().hide();
	//TLJSettingPanel::Instance().hide();
	GPHomeSettingPanel::Instance().hide();
	TLJOperate::Instance().hide();
	for (int index = 0; index < MAX_PLAYER; ++index)
	{
		TLJPlayer_Panel::Instance().hidePlayer(index);
	}
	TLJInfoPanel::Instance().hide();
	TLJRoomDetail::Instance().hide();
	WidgetFun::setVisible(this, "TLJGameScene_MenuNode", false);
	GPHomeMallPanel::Instance().hide();
}
//结算显示
void TLJGameScence::showCalculate()
{
	WORD wTotal = getPlayerCount();
	TLJCalculatePanel::Instance().showTotal(wTotal);
	WORD wCount = 0;
	int dwMaxScore = 0;
	TLJPlayer* pWinner = NULL;
	for(WORD index=0;index<GAME_PLAYER;index++){
		if(m_pPlayers[index]->isValid()){
			TLJCalculatePanel::Instance().showCalculateItem(*m_pPlayers[index],wCount++);
			int iScore = m_pPlayers[index]->getRoomScore();
			if(pWinner == NULL){
				pWinner = m_pPlayers[index];
				dwMaxScore = m_pPlayers[index]->getRoomScore();
			}else if(iScore>dwMaxScore){
				pWinner = m_pPlayers[index];
				dwMaxScore = m_pPlayers[index]->getRoomScore();
			}
		}
	}
	if(pWinner!=NULL){
		TLJCalculatePanel::Instance().showWinner(*pWinner);
	}
}
#pragma endregion 

#pragma region 触摸事件
void TLJGameScence::initTouch()
{
	TLJTouchScence* pTouchScene = new TLJTouchScence;
	pTouchScene->init();
	addChild(pTouchScene);
}

bool TLJGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	int onTouchPlayer = TLJPlayer_Panel::Instance().getTouchPlayerNode(kPos);
	if (onTouchPlayer != -1) {
		//int onTouchPlayer = ZJMJPlayer_Panel::Instance().getTouchPlayerNode(kPos);
		for (size_t i = 0; i < MAX_PLAYER; i++)
		{
			if (m_pPlayers[i]->isValid() && m_pPlayers[i]->getVisioChairID() == onTouchPlayer) {
				TLJPlayer_Panel::Instance().onPlayerTouched(onTouchPlayer);
				break;
			}
		}
	}
	Node* pNode = TLJPlayer_Card::Instance().getTouchCardNode(kPos);
	if(pNode!=NULL){
		playerLookCard();
	}
	return false;
}
void TLJGameScence::ccTouchMoved(cocos2d::Vec2 lPos,cocos2d::Vec2 pPos)
{

}
void TLJGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	return;
}
#pragma endregion 

#pragma region 场景按钮
void TLJGameScence::initButton()
{
	
	WidgetManager::addButtonCB("Buttion_Background", this, button_selector(TLJGameScence::Buttion_Background));
	WidgetManager::addButtonCB("Button_TLJ_Menu", this, button_selector(TLJGameScence::Button_Menu));
	WidgetManager::addButtonCB("Button_TLJ_Setting", this, button_selector(TLJGameScence::Button_Setting));
	WidgetManager::addButtonCB("Button_TLJ_Leave", this, button_selector(TLJGameScence::Button_Exit));
	
	WidgetManager::addButtonCB("Button_TLJ_Msg",this,button_selector(TLJGameScence::Button_Send_TalkStr2));
	//WidgetManager::addButtonCB("Button_Info",this,button_selector(TLJGameScence::Button_Info));

	WidgetManager::addButtonCB("Button_Ready",this,button_selector(TLJGameScence::Button_Ready));

	WidgetManager::addButtonCB("TLJButton_TalkDefine",this,button_selector(TLJGameScence::Button_TalkDefine));
	WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(TLJGameScence::Button_Send_TalkStr));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));
	WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(TLJGameScence::HNMJButton_BiaoQing));

	WidgetManager::addButtonCB("Button_TLJ_YuYin", this, button_selector(TLJGameScence::Button_TalkEnd));
	WidgetManager::addButtonCBBegin("Button_TLJ_YuYin", this, button_selector(TLJGameScence::Button_TalkBegin));
	WidgetManager::addButtonCB("TLJGameScene_ButtonHelp",this,button_selector(TLJGameScence::Button_Help));
	WidgetManager::addButtonCB("Btton_TLJ_ChongZhi",this,button_selector(TLJGameScence::Button_ChongZhi));
	//WidgetFun::setVisible(this,"Btton_TLJ_ChongZhi",false);
	WidgetManager::addButtonCB("TLJGameScene_ButtonFriend",this,button_selector(TLJGameScence::TLJ_Button_WeiXin));
}

void TLJGameScence::Button_Exit(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{
	//TLJOperate::Instance().changeOperateMode(TLJOperate::NORMAL_MODE);
	Button_ExitGameBase(NULL, NULL);
	/*if (m_RoomInfo.dwPlayCout == 0)
	{
	if (m_RoomInfo.dwCreateUserID == UserInfo::Instance().getUserID()) 
	{
	TLJDismissRoom::Instance().show(false);
	}
	else
	{
	Button_ExitGameBase(NULL, NULL);
	}
	}
	else 
	{
	TLJDismissRoom::Instance().show(true);
	}
	return;*/
}
void TLJGameScence::Button_Setting(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{
	//TLJSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
}
void TLJGameScence::BackHomeScene()
{
	stopAllActions();
	GPSceneManager::Instance().InHomeScene();
}
//用户请求看牌
void TLJGameScence::Button_KanPai(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{
	playerLookCard();
}
void TLJGameScence::Button_TalkBegin(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{
	if (WidgetFun::isWidgetVisble(this, "TalkImagic"))
	{
		JniFun::stopSoundRecord();
		WidgetFun::setVisible(this, "TalkImagic", false);
		return;
	}
	SoundFun::Instance().PaseBackMusic();
	WidgetFun::setVisible(this, "TalkImagic", true);
	JniFun::startSoundRecord();
	int iTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(TLJGameScence::endButtonTalk, this), 5.0f)->iIdex;
	WidgetFun::setWidgetUserInfo(this, "TalkImagic", "TimeID", utility::toString(iTimeID));
	return;
}

void TLJGameScence::endButtonTalk()
{
	Button_TalkEnd(NULL, NULL);
}

void TLJGameScence::Button_TalkEnd(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{
	SoundFun::Instance().ResumeBackMusic();
	if (!WidgetFun::isWidgetVisble(this, "TalkImagic"))
	{
		return;
	}
	int iTimeID = utility::parseInt(WidgetFun::getWidgetUserInfo(this, "TalkImagic", "TimeID"));
	TimeManager::Instance().removeByID(iTimeID);
	WidgetFun::setVisible(this, "TalkImagic", false);
	std::string kFileName = JniFun::stopSoundRecord();
	sendTalkFile(getLocalPlayer()->GetChairID(), kFileName);
	return;
}
//菜单按钮响应
void TLJGameScence::Button_Menu(cocos2d::Ref*, WidgetUserInfo*)
{
	if(WidgetFun::isWidgetVisble(this, "TLJGameScene_MenuNode")) {
		WidgetFun::setVisible(this, "TLJGameScene_MenuNode", false);
	} else {
		WidgetFun::setVisible(this, "TLJGameScene_MenuNode", true);
	}
}

void TLJGameScence::Button_ChongZhi(cocos2d::Ref*, WidgetUserInfo*)
{
	if (GPHomeMallPanel::pInstance() == nullptr)
		new GPHomeMallPanel;
	if (GPHomeMallPanel::pInstance() != nullptr)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());
	GPHomeMallPanel::Instance().show();
}

void TLJGameScence::Button_Betting(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo)
{
	DWORD dwChairId = getLocalPlayer()->GetChairID();
	if(m_CurrentUser == dwChairId){
		BYTE cbLevel = 0;
		std::map<std::string,std::string>::iterator it;
		for(it=widgetUserInfo->kUserInfo.begin();it!=widgetUserInfo->kUserInfo.end();it++){
			if(it->first=="Level"){
				cbLevel = utility::parseChar(it->second);
			}
		}
		playerBetting(cbLevel);
	}
}

//弃牌
void TLJGameScence::Button_QiPai(cocos2d::Ref*, WidgetUserInfo*)
{
	TLJPlayer * pLocalPlayer = getLocalPlayer();
	DWORD dwChairId = pLocalPlayer->GetChairID();
	//if(m_CurrentUser == dwChairId){
		WORD wState = pLocalPlayer->getState();
		if(wState==PLAYER_STATE_AN||wState==PLAYER_STATE_MING){
			SendSocketData(SUB_C_GIVE_UP, NULL, 0);
		}
	//}
}
//跟注
void TLJGameScence::PlayerGenPai()
{
	DWORD dwChairId = getLocalPlayer()->GetChairID();
	if(m_CurrentUser == dwChairId){
		playerBetting(m_cbScoreLevel);
	}
}
void TLJGameScence::Button_BiPai(cocos2d::Ref*, WidgetUserInfo*)
{
	TLJPlayer * pLocalPlayer = getLocalPlayer();
	if(pLocalPlayer->GetChairID()!=m_CurrentUser){
		return;
	}

	//只剩两人  则直接和另一个玩家比牌
	WORD wChairId = INVALID_CHAIR;
	WORD wPlayerCount = 0;
	for (int index = 0; index < MAX_PLAYER; ++index)
	{
		WORD wState = m_pPlayers[index]->getState();
		if(wState == PLAYER_STATE_MING || wState == PLAYER_STATE_AN)
		{
			wPlayerCount++;
			if(m_pPlayers[index]->GetChairID() != getLocalPlayer()->GetChairID())
			{
				wChairId = m_pPlayers[index]->GetChairID();
			}
		}
	}
	if(wPlayerCount == 2 && wChairId != INVALID_CHAIR)
	{
		playerCompare(wChairId);
		return;
	}
	///////////////////////////////////
	if(pLocalPlayer->getState() == PLAYER_STATE_MING || pLocalPlayer->getState() == PLAYER_STATE_AN){
		setGameState(TLJ_STATUS_COMPARE);
		pLocalPlayer->PlayerCompare();
	}
}
//准备按钮响应
void TLJGameScence::Button_Ready(cocos2d::Ref*, WidgetUserInfo*)
{
	sendReady();
	//准备状态
	WidgetFun::setVisible(WidgetFun::getChildWidget(this,"Button_Ready"),false);
	if(getGameState()==TLJ_STATUS_NULL){
		for (int index = 0; index < MAX_PLAYER; ++index) {
			m_pPlayers[index]->reset();
			m_pPlayers[index]->upPlayerInfo();
			TLJPlayer_Card::Instance().hideCard(index);
			TLJPlayer_Panel::Instance().changeMaskVisible(index,false);
		}
	}
}
#pragma endregion
void TLJGameScence::Button_Help(cocos2d::Ref*, WidgetUserInfo*)
{
	TLJRoomDetail::Instance().show();
}

#pragma  region 辅助函数
void TLJGameScence::changeCurrentPlayer(WORD wChairId)
{
	WORD wState = 0;
	m_CurrentUser = wChairId;
	if(m_CurrentUser!=INVALID_CHAIR)
	{
		m_pPlayers[wChairId]->showTimes(WAIT_TIME);
		wState = getLocalPlayer()->getState();
	}
	//当前玩家自动跟注
	if(wChairId==getLocalPlayer()->GetChairID()){
		if(getLocalPlayer()->getAutoState()){
			DelayTime* pDelay = DelayTime::create(0.5f);
			CallFunc* pCallBack = CallFunc::create([]{
				TLJOperate::Instance().changeOperateMode(TLJOperate::AUTO_MODE);
				//TLJGameScence::Instance().PlayerGenPai();
				TLJOperate::Instance().TLJ_Operate_Gen(NULL,NULL);
			});
			Sequence* pSeq = Sequence::create(pDelay,pCallBack,NULL);
			runAction(pSeq);
			return;
		}
	}
	//刷新操作按钮显示
	if(isMyTurn()){
		TLJOperate::Instance().changeOperateMode(TLJOperate::NORMAL_MODE);
	}
	else if(wState==PLAYER_STATE_QI||wState==PLAYER_STATE_NULL||wState==PLAYER_STATE_COMPARE_LOSE)
	{
		TLJOperate::Instance().changeOperateMode(TLJOperate::INVALID_MODE);
	}
	else if(TLJOperate::Instance().getOperateMode()!=TLJOperate::AUTO_MODE)
	{
		TLJOperate::Instance().changeOperateMode(TLJOperate::WAIT_MODE);
	}
}
//游戏状态
void TLJGameScence::setGameState(WORD wState)
{
	m_wGameState = wState;
}
//游戏状态
WORD TLJGameScence::getGameState()
{
	return m_wGameState;
}
//玩家数
WORD TLJGameScence::getPlayerCount()
{
	WORD wResult = 0;
	for (BYTE i = 0;i<GAME_PLAYER;i++)
	{
		if(m_pPlayers[i]->isValid()){
			wResult++;
		}
	}
	return wResult;
}
//准备下一局
void TLJGameScence::readyToNextMatch()
{
	m_wCurrentTimes = 1;
	m_cbScoreLevel = 1;
	m_wCurrentTurn = 0;
	m_lTotalScore = 0;
	setGameState(TLJ_STATUS_NULL);

	//refreshReadyStatus();
	//sendReady();
}
//准备状态
void TLJGameScence::refreshReadyStatus()
{
	if(getLocalPlayer()->GetUserStatus()>=US_READY){
		WidgetFun::setVisible(this,"TLJOperator_AlarmNode",false);
		WidgetFun::setVisible(WidgetFun::getChildWidget(this,"Button_Ready"),false);
		hideReadyTimes();
	}else{
		//if(!WidgetFun::isWidgetVisble(this,"TLJOperator_AlarmNode"))
		//{
		showReadyTimes(10);
		//}
		WidgetFun::setVisible(WidgetFun::getChildWidget(this,"Button_Ready"),true);
	}
}
//金币校验
bool TLJGameScence::checkUserScore(BYTE cbLevel)
{
	if(cbLevel == 0){
		cbLevel = m_cbScoreLevel;
	}
	TLJPlayer* pLocalPlayer = getLocalPlayer();
	LONGLONG lUserScore = pLocalPlayer->GetUserScore();
	DWORD dwScore = TLJGameLogic::Instance().GetBettingScore(cbLevel,pLocalPlayer->getState()==PLAYER_STATE_MING,m_wCurrentTimes,getBaseScore());
	if(lUserScore<dwScore){
		NoticeMsg::Instance().ShowTopMsg(utility::a_u8("金币不足！"));
		return false;
	}
	return true;
}
//准备倒计时
void TLJGameScence::showReadyTimes(int times)
{
	WidgetFun::getChildWidget(this, "TLJOperator_AlarmNode")->stopAllActions();

	WidgetFun::setVisible(this, "TLJOperator_AlarmNode", true);
	int first = times / 10;
	int second = times % 10;
	auto pAlarmNode = WidgetFun::getChildWidget(this, "TLJOperator_AlarmNode");
	WidgetFun::setImagic(WidgetFun::getChildWidget(this, "TLJAlarm_Timer_0"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), first, ".png"), false);
	WidgetFun::setImagic(WidgetFun::getChildWidget(this, "TLJAlarm_Timer_1"), utility::toString(WidgetFun::getWidgetUserInfo(pAlarmNode, "Image"), second, ".png"), false);

	if(times > 0) {
		DelayTime* delay = DelayTime::create(1.f);
		CallFunc* func = CallFunc::create([ = ] {
			showReadyTimes(times - 1);
		});
		Sequence* seq = Sequence::create(delay, func, NULL);
		WidgetFun::getChildWidget(this, "TLJOperator_AlarmNode")->runAction(seq);
	}
}
//准备倒计时
void TLJGameScence::hideReadyTimes()
{
	WidgetFun::getChildWidget(this, "TLJOperator_AlarmNode")->stopAllActions();
	WidgetFun::setVisible(this, "TLJOperator_AlarmNode", false);
}

//扣税收显示(仅首局显示)
void TLJGameScence::showGameTax(int iGameTax)
{
	if(iGameTax==0)
		return;
	std::string kStr = utility::toString(utility::a_u8("本局服务费"),iGameTax);
	WidgetFun::setText(this,"GameTax",kStr);
	WidgetFun::runWidgetAction(this,"GameTax","Start");
}
#pragma endregion

 #pragma region 游戏消息
void TLJGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START_TLJ, this, TLJGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_LOOK_CARD_TLJ, this, TLJGameScence::OnLookCard);
	addNetCBDefine(SUB_S_GIVE_UP_TLJ, this, TLJGameScence::OnSubGiveUp);
	addNetCBDefine(SUB_S_ADD_SCORE_TLJ, this, TLJGameScence::onSubAddScore);
	addNetCBDefine(SUB_S_COMPARE_CARD_TLJ, this, TLJGameScence::onSubCompare);
	//addNetCBDefine(SUB_S_OPEN_CARD_TLJ, this, TLJGameScence::onSubGameEnd);
	addNetCBDefine(SUB_S_GAME_END_TLJ, this, TLJGameScence::onSubGameEnd);
}
//游戏开始
void TLJGameScence::OnSubGameStart(const void * pBuffer, word wDataSize)
{
	//效验数据
	auto size = sizeof(CMD_S_GameStart);
	ASSERT(wDataSize == sizeof(CMD_S_GameStart));
	//重置场景
	ResetGameClient();
	//更改游戏状态
	setGameState(TLJ_STATUS_PLAYING);
	//解析数据
	CMD_S_GameStart* pNetInfo = (CMD_S_GameStart*)pBuffer;
	//刷新客户端数据
	//m_CurrentUser = pNetInfo->wCurrentUser;
	m_BankerUser = pNetInfo->wBankerUser;
	m_cbScoreLevel = pNetInfo->cbScoreLevel;
	m_wCurrentTimes = pNetInfo->wCurrentTimes;
	m_wCurrentTurn = pNetInfo->wCurrentTurn;
	m_lTotalScore = pNetInfo->lTotalScore;
	//刷新客户端显示
	changeBettingBtnImagic(m_wCurrentTimes);
	refreshBettingBtn(m_cbScoreLevel);
	//玩家游戏局数
	TLJPlayer* pLocalPlayer = getLocalPlayer();
	pLocalPlayer->setPlayCount(pLocalPlayer->getPlayCount()+1);

	for(BYTE i=0;i<MAX_PLAYER;i++){
		if(!m_pPlayers[i]->isValid()){
			continue;
		}
		//重置玩家数据
		m_pPlayers[i]->reset();
		m_pPlayers[i]->setState(PLAYER_STATE_AN);
		//设置玩家手牌
		BYTE cbCardData[MAX_COUNT];
		memset(&cbCardData,0,sizeof(cbCardData));
		m_pPlayers[i]->setHandCard(cbCardData, false);
		//刷新玩家显示
		m_pPlayers[i]->upPlayerInfo();
		TLJAnimation::Instance().runGameStartAnimation(*m_pPlayers[i]);
		
		//玩家下底分
		m_pPlayers[i]->PlayerBetting(m_cbScoreLevel,m_wCurrentTimes);

		m_pPlayers[i]->setAutoState(false);
	}

	//DONE: 取消扣除服务费
#ifdef SHOW_SERVICE_CORE
	//扣税收显示(仅首局显示)
	if(getLocalPlayer()->getPlayCount()==1){
		showGameTax(-pNetInfo->wGameTax);
	}
#endif

	changeCurrentPlayer(pNetInfo->wCurrentUser);

	updateRoomInfo();
}

//看牌结果
void TLJGameScence::OnLookCard(const void * pBuffer, word wDataSize)
{
	//效验数据
	auto size = sizeof(CMD_S_LookCard);
	ASSERT(wDataSize == sizeof(CMD_S_LookCard));

	CMD_S_LookCard* pNetInfo = (CMD_S_LookCard*)pBuffer;

	TLJPlayer* pCurrentPlayer= getPlayerByChairID(pNetInfo->wLookCardUser);
	pCurrentPlayer->setHandCard(pNetInfo->cbCardData, true);
	pCurrentPlayer->setState(PLAYER_STATE_MING);
	pCurrentPlayer->upPlayerInfoAfterOperate();

	//音效
	//TLJSoundFun::playEffectByKind(getLocalPlayer()->GetGender(),TLJSoundFun::EFFECT_KANPAI);
	TLJSoundFun::playEffectByKind(pCurrentPlayer->GetGender(),TLJSoundFun::EFFECT_KANPAI);
	changeBettingBtnImagic(m_wCurrentTimes);

	//加注时  刷新按钮显示
	if(TLJOperate::Instance().getOperateMode() == TLJOperate::ADD_SCORE_MODE)
	{
		TLJOperate::Instance().changeBettingBtnImagic();
	}
}

//弃牌
void TLJGameScence::OnSubGiveUp(const void * pBuffer, word wDataSize)
{
	//效验数据
	auto size = sizeof(CMD_S_GiveUp);
	ASSERT(wDataSize == sizeof(CMD_S_GiveUp));
	//解析数据
	CMD_S_GiveUp* pNetInfo = (CMD_S_GiveUp*)pBuffer;

	m_wCurrentTurn = pNetInfo->wCurrentTurn;
	//比牌状态  重置显示
	if(getGameState()==TLJ_STATUS_COMPARE){
		TLJPlayer_Panel::Instance().resetPanel();
	}

	m_pPlayers[pNetInfo->wGiveUpUser]->PlayerGiveUp(PLAYER_STATE_QI);
	changeCurrentPlayer(pNetInfo->wCurrentUser);
	//本地玩家弃牌  刷新操作界面显示
	if(pNetInfo->wGiveUpUser==getLocalPlayer()->GetChairID())
		TLJOperate::Instance().changeOperateMode(TLJOperate::INVALID_MODE);

	updateRoomInfo();
	//TLJSoundFun::playEffectByKind(getLocalPlayer()->GetGender(),TLJSoundFun::EFFECT_QIPAI);
	TLJSoundFun::playEffectByKind(m_pPlayers[pNetInfo->wGiveUpUser]->GetGender(),TLJSoundFun::EFFECT_QIPAI);
}
//用户加注
void TLJGameScence::onSubAddScore(const void * pBuffer, word wDataSize)
{
	//效验数据
	auto size = sizeof(CMD_S_AddScore);
	ASSERT(wDataSize == sizeof(CMD_S_AddScore));
	//解析数据
	CMD_S_AddScore* pNetInfo = (CMD_S_AddScore*)pBuffer;
	//客户端显示刷新
	if(pNetInfo->wCurrentTimes>m_wCurrentTimes&&getLocalPlayer()->getState()!=PLAYER_STATE_AN)
	{
		changeBettingBtnImagic(pNetInfo->wCurrentTimes);
	} 
	std::string kEffectType = TLJSoundFun::EFFECT_GENZHU;
	if(pNetInfo->cbScoreLevel>m_cbScoreLevel)
	{
		refreshBettingBtn(pNetInfo->cbScoreLevel);
		kEffectType = TLJSoundFun::EFFECT_ZHANGJIA;
	}
	//客户端数据刷新
	m_wCurrentTimes = pNetInfo->wCurrentTimes;
	m_cbScoreLevel = pNetInfo->cbScoreLevel;
	m_wCurrentTurn = pNetInfo->wCurrentTurn;
	m_lTotalScore = pNetInfo->lTotalScore;

	//播放下注动画
	m_pPlayers[pNetInfo->wAddScoreUser]->PlayerBetting(pNetInfo->cbScoreLevel,m_wCurrentTimes);
	//音效
	TLJSoundFun::playEffectByKind(m_pPlayers[pNetInfo->wAddScoreUser]->GetGender(),kEffectType);
	bool bMingPai = m_pPlayers[pNetInfo->wAddScoreUser]->getState() == PLAYER_STATE_MING;
	DWORD dwScore = TLJGameLogic::Instance().GetBettingScore(m_cbScoreLevel,bMingPai,m_wCurrentTimes,m_RoomInfo.dwBaseScore);
	m_pPlayers[pNetInfo->wAddScoreUser]->setGameScore(m_pPlayers[pNetInfo->wAddScoreUser]->getGameScore()+dwScore);
	m_pPlayers[pNetInfo->wAddScoreUser]->upPlayerInfo();
	changeCurrentPlayer(pNetInfo->wCurrentUser);
	updateRoomInfo();
}
//用户比牌
void TLJGameScence::onSubCompare(const void * pBuffer, word wDataSize)
{
	//效验数据
	auto size = sizeof(CMD_S_CompareCard);
	ASSERT(wDataSize == sizeof(CMD_S_CompareCard));
	//解析数据
	CMD_S_CompareCard* pNetInfo = (CMD_S_CompareCard*)pBuffer;

	WORD wVisioChairID_0 = m_pPlayers[pNetInfo->wCompareUser[0]]->getVisioChairID();
	WORD wVisioChairID_1 = m_pPlayers[pNetInfo->wCompareUser[1]]->getVisioChairID();
	bool bResult = false;
	if(pNetInfo->wLostUser == pNetInfo->wCompareUser[1])
	{
		bResult = true;
	}

	TLJPlayer_Panel::Instance().PlayerCompare(*m_pPlayers[pNetInfo->wCompareUser[0]],*m_pPlayers[pNetInfo->wCompareUser[1]],bResult);

	m_pPlayers[pNetInfo->wCompareUser[0]]->PlayerBetting(m_cbScoreLevel,m_wCurrentTimes);

	m_wCurrentTurn = pNetInfo->wCurrentTurn;
	m_lTotalScore = pNetInfo->lTotalScore;

	bool bMingPai = m_pPlayers[pNetInfo->wCompareUser[0]]->getState() == PLAYER_STATE_MING;
	DWORD dwScore = TLJGameLogic::Instance().GetBettingScore(m_cbScoreLevel,bMingPai,m_wCurrentTimes,m_RoomInfo.dwBaseScore);
	m_pPlayers[pNetInfo->wCompareUser[0]]->setGameScore(m_pPlayers[pNetInfo->wCompareUser[0]]->getGameScore()+dwScore);
	m_pPlayers[pNetInfo->wCompareUser[0]]->upPlayerInfo();
	//切换当前玩家
	changeCurrentPlayer(pNetInfo->wCurrentUser);
	//本地玩家比牌失败   刷新操作显示
	if(getLocalPlayer()->GetChairID() == pNetInfo->wLostUser){
		TLJOperate::Instance().changeOperateMode(TLJOperate::INVALID_MODE);
	}
	if (bMingPai)
		TLJSoundFun::playEffectByKind(m_pPlayers[pNetInfo->wCompareUser[0]]->GetGender(),TLJSoundFun::EFFECT_BIPAI);
	else
		TLJSoundFun::playEffectByKind(m_pPlayers[pNetInfo->wCompareUser[0]]->GetGender(),TLJSoundFun::EFFECT_MENBI);
	updateRoomInfo();
}
//游戏结束
void TLJGameScence::onSubGameEnd(const void * pBuffer, word wDataSize)
{
	//效验数据
	auto size = sizeof(TLJ::CMD_S_GameEnd);
	ASSERT(wDataSize == sizeof(TLJ::CMD_S_GameEnd));
	//解析数据
	TLJ::CMD_S_GameEnd* pNetInfo = (TLJ::CMD_S_GameEnd*)pBuffer;
	WORD pLocalId = getLocalPlayer()->GetChairID();
	//刷新手牌显示
	for (BYTE index = 0; index < TLJ_GAME_PLAYER; index ++ )
	{
		bool bMing = false;
		for(BYTE i = 0; i < 5; i ++ ){
			if(pNetInfo->wCompareUser[pLocalId][i]==index){
				bMing = true;
				break;
			}
		}
		//开牌结束 或 自己及比牌对手  显示手牌
		if(index==pLocalId||bMing||pNetInfo->wEndState==1)
			m_pPlayers[index]->setHandCard(pNetInfo->cbCardData[index],true);
		m_pPlayers[index]->upPlayerInfo();
		m_pPlayers[index]->setAutoState(false);
		m_pPlayers[index]->setState(PLAYER_STATE_NULL);
	}
	//胜者收取所有筹码
	TLJAnimation::Instance().runGameEndAnimation(*m_pPlayers[pNetInfo->wWinner]);
	//胜者金币增加
	m_pPlayers[pNetInfo->wWinner]->showAddGold(pNetInfo->lTotalScore);
	//隐藏计时器
	TLJPlayer_Panel::Instance().hideTimes();
	//当前玩家置空   隐藏操作按钮
	changeCurrentPlayer(INVALID_CHAIR);

	DelayTime* pDelayTime = DelayTime::create(1.0f);
	CallFunc* pCallBack = CallFunc::create([]{
		TLJGameScence::Instance().readyToNextMatch();
	});
	Sequence* pSeq = Sequence::create(pDelayTime,pCallBack,NULL);
	runAction(pSeq);
}
//游戏状态
bool TLJGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	//WidgetFun::setVisible(this,"PrivateEndPlane",false);
	for(WORD i = 0;i<GAME_PLAYER;++i)
	{
		if (m_pPlayers[i] == NULL)
		{
			continue;
		}
		TLJPlayer_Panel::Instance().showPlayer(*m_pPlayers[i]);
	}

	if (cbGameStatus == GAME_STATUS_FREE){
		OnFreeScene(data, dataSize);
	}else if (cbGameStatus == GAME_STATUS_PLAY ){
		OnPlayerScene(data, dataSize);
	}
	return true;
}
//房间消息
void TLJGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	memset(&m_RoomInfo, 0, sizeof(m_RoomInfo));
	m_RoomInfo = *pNetInfo;
	m_RoomInfo.kWinLoseScore.clear();
	for (size_t index = 0; index < MAX_PLAYER && index < pNetInfo->kWinLoseScore.size(); ++index) {
		m_RoomInfo.kWinLoseScore.push_back(pNetInfo->kWinLoseScore[index]);
	}
	updateRoomInfo();
	updateUserInfo();
}
//重连
void TLJGameScence::OnFreeScene(void* data, int dataSize)
{
	//refreshReadyStatus();
}
//重连
void TLJGameScence::OnPlayerScene(void* data, int dataSize)
{
	//效验数据
	auto size = sizeof(CMD_S_StatusPlay);
	ASSERT(dataSize == sizeof(CMD_S_StatusPlay));
	if (dataSize != sizeof(CMD_S_StatusPlay))
	{ 
		return;
	}
	//更改游戏状态
	m_IsInGame = true;
	setGameState(TLJ_STATUS_PLAYING);
	//解析数据
	CMD_S_StatusPlay* pNetInfo = (CMD_S_StatusPlay*)data;
	//刷新客户端数据
	//m_CurrentUser = pNetInfo->wCurrentUser;
	m_BankerUser = pNetInfo->wBankerUser;
	m_wCurrentTimes = pNetInfo->wCurrentTimes;
	m_cbScoreLevel = pNetInfo->cbScoreLevel;
	m_wCurrentTurn = pNetInfo->wCurrentTurn;
	m_lTotalScore = pNetInfo->lTotalScore;

	WORD wChairId = getLocalPlayer()->getVisioChairID();
	TLJPlayer_Card::Instance().show();
	BYTE cbHandCardData[TLJ_MAX_COUNT];
	memset(&cbHandCardData,0,sizeof(cbHandCardData));
	for (int index = 0; index < MAX_PLAYER; ++index)	
	{
		//设置玩家状态
		if(pNetInfo->cbPlayStatus[index]==4){
			m_pPlayers[index]->setState(PLAYER_STATE_QI);
		}else if(pNetInfo->cbPlayStatus[index]==3&&pNetInfo->bMingStatus[index]){
			m_pPlayers[index]->setState(PLAYER_STATE_MING);
		}else if(pNetInfo->cbPlayStatus[index]==3&&(!pNetInfo->bMingStatus[index])){
			m_pPlayers[index]->setState(PLAYER_STATE_AN);
		}else{
			m_pPlayers[index]->setState(PLAYER_STATE_NULL);
		}

		if(m_pPlayers[index]==getLocalPlayer()){
			m_pPlayers[index]->setHandCard(pNetInfo->cbHandCardData,pNetInfo->bMingStatus[index]);
		}else{
			m_pPlayers[index]->setHandCard(cbHandCardData,pNetInfo->bMingStatus[index]);
		}

		m_pPlayers[index]->setGameScore(pNetInfo->lUserScore[index]);

		m_pPlayers[index]->upPlayerInfo();
		if(m_pPlayers[index]->getState()==PLAYER_STATE_QI){
			m_pPlayers[index]->PlayerGiveUp(PLAYER_STATE_QI);
		}
	}
	//刷新筹码显示
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if(pNetInfo->wChips[i][j]==0){
				continue;
			}
			TLJChipContainer::Instance().addChip(j+1,i+1,pNetInfo->wChips[i][j],pNetInfo->dwBaseScore);
		}
	}

	//当前玩家切换
	changeCurrentPlayer(pNetInfo->wCurrentUser);

	//刷新投注按钮显示
	changeBettingBtnImagic(m_wCurrentTimes);
	refreshBettingBtn(m_cbScoreLevel);
	//准备状态
	//refreshReadyStatus();
	
	updateRoomInfo();
}
//解散房间消息处理
void TLJGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	if (!TLJDismissRoomConfirm::Instance().isVisible()) {
		if (pNetInfo->dwDissUserCout != 0) {
			NoticeMsg::Instance().ShowTopMsg(utility::a_u8(utility::toString("玩家", m_pPlayers[pNetInfo->dwDissChairID[0]]->GetNickName(), "发起了解散房间请求！")));
		}
	}

	if (pNetInfo->dwNotAgreeUserCout != 0) {
		NoticeMsg::Instance().ShowTopMsg(utility::a_u8(utility::toString("玩家", m_pPlayers[pNetInfo->dwNotAgreeChairID[0]]->GetNickName(), "拒绝解散游戏！")));
	}

	if (pNetInfo->dwDissUserCout == 0) {
		TLJDismissRoomConfirm::Instance().hide();
		return;
	}
	if (m_RoomInfo.dwPlayCout != 0)
		TLJDismissRoomConfirm::Instance().show(*pNetInfo);
}
#pragma endregion 


#pragma region 房间信息
void TLJGameScence::updateRoomInfo()
{
	std::string kRoomId = utility::toString("房间号:",m_RoomInfo.dwRoomNum);
	WidgetFun::setText(WidgetFun::getChildWidget(this,"RoomId"),utility::a_u8(kRoomId));
	changeBettingBtnImagic(m_wCurrentTimes);

	//if(m_RoomInfo.bGameRuleIdex))
	if(FvMask::HasAny(m_RoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_COMMON))){
		WidgetFun::setVisible(this,"Rule1",true);
	}else{
		WidgetFun::setVisible(this,"Rule1",false);
	}

	if(FvMask::HasAny(m_RoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_PASSION))){
		WidgetFun::setVisible(this,"Rule2",true);
	}else{
		WidgetFun::setVisible(this,"Rule2",false);
	}

	if(FvMask::HasAny(m_RoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_BRAISED))){
		WidgetFun::setVisible(this,"Rule3",true);
	}else{
		WidgetFun::setVisible(this,"Rule3",false);
	}
	m_wCurrentTurn = m_wCurrentTurn>20?20:m_wCurrentTurn;
	std::string kTurnInfo = utility::toString("第",m_wCurrentTurn,"/20轮");
	WidgetFun::setText(WidgetFun::getChildWidget(this,"CurrentTurn"),utility::a_u8(kTurnInfo));

	std::string kTotalScore = utility::toString("总注数:",m_lTotalScore);
	WidgetFun::setText(WidgetFun::getChildWidget(this,"TotalScore"),utility::a_u8(kTotalScore));

	std::string kBaseScore = utility::toString("底注:",m_RoomInfo.dwBaseScore);
	WidgetFun::setText(WidgetFun::getChildWidget(this,"BaseScore"),utility::a_u8(kBaseScore));

	std::string kMaxSingleScore = utility::toString("封顶:",m_RoomInfo.dwBaseScore*5*m_wCurrentTimes);
	WidgetFun::setText(WidgetFun::getChildWidget(this,"MaxSingleScore"),utility::a_u8(kMaxSingleScore));
}

std::string TLJGameScence::getRoomInfoText(bool forShare /*= false*/)
{
	std::string resultStr("");
	if(FvMask::HasAny(m_RoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_COMMON))){
		resultStr+="普通模式";
	}
	if(FvMask::HasAny(m_RoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_PASSION))){
		resultStr+="激情模式";
	}
	if(FvMask::HasAny(m_RoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_BRAISED))){
		resultStr+="必焖三圈";
	}
	resultStr+=utility::toString("底注:",m_RoomInfo.dwBaseScore);
	return resultStr;
}


bool TLJGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this, "Game/TLJPlane");
}
#pragma endregion 

#pragma region 玩家信息
//刷新玩家信息
void TLJGameScence::updateUserInfo()
{
	for (int index = 0; index < MAX_PLAYER; ++index) 
	{
		m_pPlayers[index]->upPlayerInfo();
	}
}
//当前玩家判断
bool TLJGameScence::isCurrentUser(TLJPlayer& player)
{
	return player.GetChairID() == m_CurrentUser && m_CurrentUser != INVALID_CHAIR;
}
//庄家判断
bool TLJGameScence::isBankerUser(TLJPlayer& player)
{
	return player.GetChairID() == m_BankerUser && m_BankerUser != INVALID_CHAIR;
}
//创建玩家
GamePlayer* TLJGameScence::CreatePlayer(IClientUserItem * pIClientUserItem)
{
	if (pIClientUserItem->GetChairID() >= MAX_PLAYER) {
		return NULL;
	}

	GamePlayer* player = m_pPlayers[pIClientUserItem->GetChairID()];
	player->setUserItem(pIClientUserItem);
	return player;
}
//玩家查找
TLJPlayer* TLJGameScence::getPlayerByChairID(WORD chairID)
{
	if (chairID < MAX_PLAYER) {
		return m_pPlayers[chairID];
	}
	return NULL;
}
TLJPlayer* TLJGameScence::getPlayerByVisioID(WORD wVIsioID)
{
	for (WORD i=0;i<MAX_PLAYER;i++)
	{
		if(m_pPlayers[i]->getVisioChairID()==wVIsioID){
			return m_pPlayers[i];
		}
	}
	return NULL;

}
//本地玩家
TLJPlayer* TLJGameScence::getLocalPlayer()
{
	for (int index = 0; index < MAX_PLAYER; ++index) 
	{
		TLJPlayer* player = m_pPlayers[index];
		if (player->GetUserID() == UserInfo::Instance().getUserID()) 
		{
			return player;
		}
	}
	return NULL;
}
//视图座位号计算
int TLJGameScence::getVisioChairID(TLJPlayer& player)
{
	ASSERT(getLocalPlayer()->getUserItem(false) && player.getUserItem(false));
	return (MAX_PLAYER * MAX_PLAYER - player.GetChairID() + getLocalPlayer()->GetChairID()) % MAX_PLAYER;
}
//玩家分数获取
int TLJGameScence::getRoomScore(TLJPlayer& player)
{
	if (m_RoomInfo.kWinLoseScore.size() > player.GetChairID()) {
		return m_RoomInfo.kWinLoseScore[player.GetChairID()];
	}
	return 0;
}
//游戏底分
DWORD TLJGameScence::getBaseScore()
{
	return m_RoomInfo.dwBaseScore;
}
//当前注等
BYTE TLJGameScence::getScoreLevel()
{
	return m_cbScoreLevel;
}
//当前倍率
WORD TLJGameScence::getCurrentTimes()
{
	return m_wCurrentTimes;
}
int TLJGameScence::getPlayCount()
{
	return m_nPlayCount;
}
void TLJGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;
}

bool TLJGameScence::isMyTurn()
{
	WORD wChairId = getLocalPlayer()->GetChairID();
	if(wChairId==m_CurrentUser){
		return true;
	}
	else
	{
		return false;
	}
}
#pragma endregion

#pragma region 游戏规则
void TLJGameScence::getGameRuleStr( std::vector<std::string>& kRuleList,dword dwGameRuleIdex )
{
	/*pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_JIADI,	true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_BUJIADI,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_FENGDING_48,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_FENGDING_96,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_ZIMO_JIABEI,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_FANGPAO_PEI,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_GUI,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_SANZHAOHU,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_JUNTAN,true);
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_LZ_WINPAY,true);*/
}

void TLJGameScence::pushGameRule( std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow )
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("MJRule",(int)nRuleTag)).Value());
	}
}
#pragma endregion

#pragma region 聊天相关
void TLJGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
	std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
	std::string kTxt = TLJSoundFun::getDefineSound(UserInfo::Instance().getGender(),utility::toString(iUserIdex));
	sendTalkDefine(getLocalPlayer()->GetChairID(),utility::toString(kTxt,":",kTaskStr));
	WidgetFun::setVisible(this,"TLJGameTalkPlane",false);
}

void TLJGameScence::Button_Send_TalkStr2( cocos2d::Ref*,WidgetUserInfo* )
{
    WidgetFun::setVisible(this,"TLJGameTalkPlane",true);
}

void TLJGameScence::Button_Send_TalkStr( cocos2d::Ref*,WidgetUserInfo* )
{
	std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
	sendTalkString(getLocalPlayer()->GetChairID(),kTxt);
	WidgetFun::setVisible(this,"TLJGameTalkPlane",false);
	WidgetFun::setEditeText(this,"GameTalkEdit","");
}

void TLJGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
	sendTalkBiaoQing(getLocalPlayer()->GetChairID(),kFile);
	WidgetFun::setVisible(this,"TLJGameTalkPlane",false);
}

void TLJGameScence::HideButton(bool bHide)
{
	if (bHide)
	{
		WidgetFun::setVisible(this, "TLJButton_Talk", false);
		WidgetFun::setVisible(this, "Button_PrivateWeiXinFriend", false);
		WidgetFun::setVisible(this, "TLJ_Exit", false);
		WidgetFun::setVisible(this, "TLJ_Setting", false);
		WidgetFun::getChildWidget(this, "Button_LiaoTian")->setVisible(false);
	}
	else
	{
		WidgetFun::setVisible(this, "TLJButton_Talk", true);
		WidgetFun::setVisible(this, "Button_PrivateWeiXinFriend", true);
		WidgetFun::setVisible(this, "TLJ_Exit", true);
		WidgetFun::setVisible(this, "TLJ_Setting", true);
		WidgetFun::getChildWidget(this, "Button_LiaoTian")->setVisible(true);
	}
}
#pragma endregion 

void TLJGameScence::Button_Info(cocos2d::Ref*, WidgetUserInfo*)
{
	TLJInfoPanel::Instance().show();
}

void TLJGameScence::Buttion_Background(cocos2d::Ref*, WidgetUserInfo*)
{
	TLJInfoPanel::Instance().hide();
}

//刷新投注按钮 可点击状态
void TLJGameScence::refreshBettingBtn(BYTE cbLevel)
{
	/*Node* pBtnNode = NULL;
	for (WORD index = 1; index < 5; index ++ )
	{
		pBtnNode = WidgetFun::getChildWidget(this,utility::toString("Button_Betting_",index));
		if(cbLevel>index){
			WidgetFun::setEnable(pBtnNode,false);
		}else{
			WidgetFun::setEnable(pBtnNode,true);
		}
	}*/
}
//刷新投注按钮  倍数显示
void TLJGameScence::changeBettingBtnImagic(WORD wTimes)
{
	/*auto pLocalPlayer = getLocalPlayer();
	if(pLocalPlayer){
		bool bAnPai = (pLocalPlayer->getState() == PLAYER_STATE_AN);
		wTimes = bAnPai?1:wTimes;
	}
	DWORD dwBaseScore = m_RoomInfo.dwBaseScore;
	Node* pBtnNode = NULL;
	std::string normalImg = "";
	std::string disableImg = "";
	for (WORD index = 1; index < 5; index ++ )
	{
		pBtnNode = WidgetFun::getChildWidget(this,utility::toString("Button_Betting_",index));
		normalImg = WidgetFun::getWidgetUserInfo(pBtnNode,"NormalImg");
		disableImg = WidgetFun::getWidgetUserInfo(pBtnNode,"DisableImg");
		WidgetManager::Instance().changeButtonProperty(pBtnNode,"NomalTexture",utility::toString(normalImg,wTimes,"_",index,".png"));
		WidgetManager::Instance().changeButtonProperty(pBtnNode,"DisableTexture",utility::toString(disableImg,wTimes,"_",index,".png"));
		WidgetManager::Instance().changeButtonProperty(pBtnNode,"ShowTxt",utility::toString(dwBaseScore*wTimes*index));
	}*/
}
#pragma region 客户端消息
//玩家看牌请求
void TLJGameScence::playerLookCard()
{
	/*if(TLJOperate::Instance().getOperateMode() == TLJOperate::ADD_SCORE_MODE)
	{
		NoticeMsg::Instance().ShowNoticeMsg(utility::a_u8("加注状态不可看牌!"));
		return;
	}*/
	if(getLocalPlayer()->getState()==PLAYER_STATE_AN)
	{
		SendSocketData(SUB_C_LOOK_CARD, NULL, 0);
	}
}
//玩家弃牌请求
void TLJGameScence::playerGiveUp()
{
	WORD wState = getLocalPlayer()->getState();
	if(wState==PLAYER_STATE_MING||wState==PLAYER_STATE_AN){
		SendSocketData(SUB_C_GIVE_UP, NULL, 0);
	}
}
//玩家下注请求
void TLJGameScence::playerBetting(BYTE cbLevel)
{
	TLJPlayer* pLocalPlayer = getLocalPlayer();
	WORD pLocalID = pLocalPlayer->GetChairID();
	if (pLocalID!=m_CurrentUser)
	{
		return;
	}

	LONGLONG lUserScore = pLocalPlayer->GetUserScore();
	DWORD dwScore = TLJGameLogic::Instance().GetBettingScore(m_cbScoreLevel,pLocalPlayer->getState()==PLAYER_STATE_MING,m_wCurrentTimes,getBaseScore());
	if(lUserScore<dwScore){
		NoticeMsg::Instance().ShowTopMsg(utility::a_u8("金币不足！"));
		return;
	}

	CMD_C_AddScore addScore;
	addScore.cbScoreLevel = cbLevel;
	WORD wState = pLocalPlayer->getState();
	if(wState==PLAYER_STATE_AN||wState == PLAYER_STATE_MING){
		if(m_CurrentUser == pLocalPlayer->GetChairID()){
			SendSocketData(SUB_C_ADD_SCORE,&addScore,sizeof(addScore));
		}
	}
}
void TLJGameScence::playerCompare(WORD wChairID)
{
	CMD_C_CompareCard compareData;
	compareData.wCompareUser = wChairID;
	SendSocketData(SUB_C_COMPARE_CARD,&compareData,sizeof(compareData));
}

//游戏底分
 CMD_GF_Private_Room_Info TLJGameScence::getRoomInfo()
{
	return m_RoomInfo;
}

 void TLJGameScence::TLJ_Button_WeiXin(cocos2d::Ref*, WidgetUserInfo*)
 {
	 std::string kCCWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		 utility::a_u8("经典三张"),(int)m_RoomInfo.dwRoomNum,utility::a_u8(getRoomInfoText(true)));
	 MissionWeiXin::Instance().shareTextWeiXin(kCCWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
 }

 void TLJGameScence::showWeiXin()
 {
	 //WidgetFun::setVisible(this, "TLJ_Operate_ButtonWeiXin", true);
 }

 void TLJGameScence::hideWeiXin()
 {
	 //WidgetFun::setVisible(this, "TLJ_Operate_ButtonWeiXin", false);
 }


#pragma endregion
