#include "WZQDGameScence.h"
#include "WZQDPlayer.h"
#include "Game/GameLib.h"
#include "CMD_WZQ.h"
#include "WZQDGameLogic.h"
#include "WZQDSoundFun.h"
#include "Game/Script/ScriptXMLparse.h"
using namespace WZQ;
FV_SINGLETON_STORAGE(WZQDGameScence);

WZQDGameScence::WZQDGameScence()
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_bIsHuanPai(false)
	,m_nGameState(WZQ_STATE_NULL)
	//,m_nGameType(GAME_TYPE_CHENGDU)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,m_WZQScore(0)
	,GameBase(0,0)
{
	init();
}

WZQDGameScence::WZQDGameScence(int iGameType,int iOption)	
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_bIsHuanPai(false)
	,m_nGameState(WZQ_STATE_NULL)
	//,m_nGameType(GAME_TYPE_CHENGDU)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,GameBase(iGameType,iOption)
{
	init();
}

WZQDGameScence::~WZQDGameScence()
{

}
bool WZQDGameScence::init()//数据准备
{
	if (!GameBase::init())
	{
		return false;
	};
	
	WidgetScenceXMLparse kScence("WZQ/XML/WZQGameScence.xml",this);
	
	if (WidgetFun::getChildWidget(this,"GameTalkList"))
	{
		cocos2d::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
		pTalkList->removeAllChildren();
		for (int i=0;i<10;i++)
		{
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("HNMJGameTalkListItem",pTalkList);
			std::string kTxt = utility::getScriptString(utility::toString("GameTalkTxt",i));
			WidgetFun::setText(pItem,"TalkListItemTxt",kTxt);
			WidgetFun::setWidgetUserInfo(pItem,"Button_TalkDefine","Idex",utility::toString(i));
			WidgetFun::setWidgetUserInfo(pItem,"Button_TalkDefine","Txt",utility::toString(kTxt));
		}
		pTalkList->forceDoLayout();

		for (int m = 0;m<6;m++)
		{
			cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BiaoQing",m));
			for (int n = 0;n<8;n++)
			{
				std::string kFileName = utility::toString("GameXZDD/Talk/EE",m*8+n,".png");
				WidgetFun::setButtonImagic(pNode,utility::toString("HNMJButton_BiaoQing",n),kFileName,true);
				WidgetFun::setWidgetUserInfo(pNode,utility::toString("HNMJButton_BiaoQing",n),"File",kFileName);
			}
		}
	}

	initPrivate();
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new WZQDPlayer(i,WidgetFun::getChildWidget(this,utility::toString("WZQPlayer",i)));
	}
	m_pLocal = m_pPlayer[0];

	initButton();
	initNet();
	initTouch();
	//initSetButton();
	defaultState();
	return true;
}
void WZQDGameScence::removeAllPanel()
{
	if (GPHomeEditNumPanel::pInstance() != nullptr && GPHomeEditNumPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeEditNumPanel::pInstance(),true);
	}
}
void WZQDGameScence::initPanel()
{
	if (GPHomeEditNumPanel::pInstance() == nullptr){
		new GPHomeEditNumPanel;
		addChild(GPHomeEditNumPanel::pInstance());
	}else{
		GPHomeEditNumPanel::pInstance()->removeFromParent();
		addChild(GPHomeEditNumPanel::pInstance());
	}
}

void WZQDGameScence::EnterScence()//开始场景
{
	removeAllPanel();
	initPanel();
	defaultState();
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->PlayerLeave();
	}
	setVisible(true);
}
bool WZQDGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameXZDDPlane");
}
void WZQDGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameXZDDPlane",false);
	//WidgetFun::setVisible(this,"GameJieSuanNode",false);
	defaultPrivateState();
}

void WZQDGameScence::exitRoom()
{
	//WZQSoundFun::stopBackMusic();
	GameBase::exitRoom();
	removeAllPanel();
}

void WZQDGameScence::defaultState()//清数据
{
	m_pTouchCardNode = NULL;
	m_bIsHuanPai = false;
	m_nGameState = WZQ_STATE_NULL;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
		m_pPlayer[i]->EndGame();
	}
	WidgetFun::setText(this,"Txt_ShuRuScore",utility::toString(utility::a_u8("学费："),0));
	m_WZQScore = 0;
	/*WidgetFun::setVisible(this,"TimeNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",false);
	WidgetFun::setVisible(this,"SaiZiNode",false);
	WidgetFun::setVisible(this,"GameRoomUserInfo",false);*/
	//WidgetFun::setVisible(this,"GameSetPlane",false);
	defaultPrivateState();
	//defaultRecordState();
	//defaultMaster(true);
	GPHomeEditNumPanel::Instance().hide();
	//WidgetFun::setVisible(this,"GameJieSuanNode",false);
	WidgetFun::setVisible(this, "GameResoult2", false);
	
}

void WZQDGameScence::setCurrentPlayer(int iCurrentPlayer,int iUserAction,int icbCardData)
{
	m_iCurrentUser = iCurrentPlayer;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");

	GamePlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->runAction(
		cocos2d::MoveExTxtTime::create(TIME_OPERATE_CARD));
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	if(pPlyer == m_pLocal )
	{
		WidgetFun::setVisible(this,"SelfActionNode",true);
		int iCout = 0;
		int iStartPos = 0;
		int iNextPos = 0;
	}
}
void WZQDGameScence::setGameResoultStateInfo(cocos2d::Node* pNode, std::string kName, std::string kDes, int lSocre)
{
	WidgetFun::setText(pNode,"Name",kName);
	WidgetFun::setText(pNode,"Des",kDes);
	if (lSocre>0)
	{
		WidgetFun::setText(pNode,"GoldTxt",utility::toString("+",lSocre));
	}
	else
	{
		WidgetFun::setText(pNode,"GoldTxt",utility::toString(lSocre));
	}
}

void WZQDGameScence::showClock(int iTime,bool bShow /*= true*/)
{
	iTime = iTime+1;
	WidgetFun::setVisible(this,"ActPlayerLastTime",bShow);
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"ActPlayerLastTime");
	if (!bShow)
	{
		pNode->setVisible(false);
		return;
	}
	pNode->setVisible(true);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTime::create(iTime,iTime,0));
}

void WZQDGameScence::showClockTimeCallBack( const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
	pNode->runAction(cocos2d::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}

void WZQDGameScence::killClock( const std::string& kName )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
}

//void WZQDGameScence::setGameResoultPlayerInfo_shuju(const AllEndInfo& pGameEnd, XZDDPlayer* pPlayer, cocos2d::Node* pNode)//玩家胡牌的信息
//{
//	pNode->removeAllChildren();
//
//	int iChirID = pPlayer->GetChairID();
//	int nWinGang = 0;
//
//	std::string pai_sJ;
//
//	for (int i = 0; i < (int)pGameEnd.kGangInfoList.size(); i++)
//	{
//		CMD_S_GangScore kGangInfo = pGameEnd.kGangInfoList[i];
//
//		int nScore = kGangInfo.lGangScore[iChirID];
//		if (nScore > 0)
//		{
//			//setGameResoultStateInfo(pNode, pPlayerWin->GetNickName(), utility::getScriptString("GFXY"), nScore);
//			pai_sJ = pai_sJ + " " + utility::getScriptString("GFXY");
//		}
//	}
//	for (int i = 0; i < (int)pGameEnd.kChiHuInfoList.size(); i++)
//	{
//		CMD_S_ChiHu kChiHuInfo = pGameEnd.kChiHuInfoList[i];
//		int nScore = kChiHuInfo.lGameScore[iChirID];
//		if (nScore > 0)
//		{
//			XZDDPlayer* pPlayerWin = getPlayerByChairID(kChiHuInfo.wChiHuUser);
//
//			std::string kTemp = getStringHuRight(kChiHuInfo.dwChiHuRight, kChiHuInfo.cbGenCount);
//			pai_sJ = pai_sJ + " " + kTemp;
//		}
//	}
//
//	if (pGameEnd.lChaJiaoScore[iChirID] != 0)
//	{
//		std::string kTemp = ScriptData<std::string>("SCHuRight22").Value();
//		pai_sJ = pai_sJ + " " + kTemp;
//	}
//	if (pGameEnd.lHuaZhuScore[iChirID] != 0)
//	{
//		std::string kTemp = ScriptData<std::string>("SCHuRight23").Value();
//		pai_sJ = pai_sJ + " " + kTemp;
//	}
//
//	WidgetFun::setText(pNode,  pai_sJ);
//	word selfChairID = m_pLocal->GetChairID();
//	int lSelfSocre = pGameEnd.lGameScoreTotal[selfChairID];
//	WidgetFun::setVisible(this, "WinTitle", lSelfSocre >= 0);
//	WidgetFun::setVisible(this, "LoseTitle", lSelfSocre < 0);
//
//}

//void WZQDGameScence::setGameResoultPlayerInfo(const AllEndInfo& pGameEnd,XZDDPlayer* pPlayer,cocos2d::Node* pNode)//设置玩家信息数据
//{
//	cocos2d::ListViewEx* pList = (cocos2d::ListViewEx*)pNode;//自动排版
//	pList->removeAllChildren();
//
//	int iChirID = pPlayer->GetChairID();
//	int nWinGang = 0;
//
//	for (int i=0;i<(int)pGameEnd.kGangInfoList.size();i++)
//	{
//		CMD_S_GangScore kGangInfo = pGameEnd.kGangInfoList[i];
//
//		int nScore = kGangInfo.lGangScore[iChirID];
//		if (nScore !=0)
//		{
//			XZDDPlayer* pPlayerWin = getPlayerByChairID(kGangInfo.wChairId);
//			cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
//			setGameResoultStateInfo(pNode,pPlayerWin->GetNickName(),utility::getScriptString("GFXY"),nScore);
//		}
//	}
//
//	for (int i=0;i<(int)pGameEnd.kChiHuInfoList.size();i++)
//	{
//		CMD_S_ChiHu kChiHuInfo = pGameEnd.kChiHuInfoList[i];
//		int nScore = kChiHuInfo.lGameScore[iChirID];
//		if (nScore != 0)
//		{
//			XZDDPlayer* pPlayerWin = getPlayerByChairID(kChiHuInfo.wChiHuUser);
//			cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
//			std::string kTemp = getStringHuRight(kChiHuInfo.dwChiHuRight,kChiHuInfo.cbGenCount);
//			setGameResoultStateInfo(pNode,pPlayerWin->GetNickName(),kTemp,nScore);
//		}
//	}
//
//	if (pGameEnd.lChaJiaoScore[iChirID] !=0)
//	{
//		std::string kTemp = ScriptData<std::string>("SCHuRight22").Value();
//		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
//		setGameResoultStateInfo(pNode,pPlayer->GetNickName(),kTemp,pGameEnd.lChaJiaoScore[iChirID]);
//	}
//	if (pGameEnd.lHuaZhuScore[iChirID] !=0)
//	{
//		std::string kTemp = ScriptData<std::string>("SCHuRight23").Value();
//		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
//		setGameResoultStateInfo(pNode,pPlayer->GetNickName(),kTemp,pGameEnd.lHuaZhuScore[iChirID]);
//	}
//	pList->refreshView();
//
//	word selfChairID =  m_pLocal->GetChairID();
//	int lSelfSocre  = pGameEnd.lGameScoreTotal[selfChairID];
//	WidgetFun::setVisible(this,"WinTitle",lSelfSocre >= 0);
//	WidgetFun::setVisible(this,"LoseTitle",lSelfSocre < 0);
//}


void WZQDGameScence::showRoomUserInfo( int nIndex )
{
	ASSERT(nIndex >=0 && nIndex<MAX_PLAYER);
	if (nIndex <0 || nIndex>=MAX_PLAYER)
	{
		return;
	}

	WidgetFun::setText(this,"RoomUser_name",m_pPlayer[nIndex]->GetNickName());
	WidgetFun::setText(this,"RoomUser_ID",m_pPlayer[nIndex]->GetUserID());
	WidgetFun::setText(this,"RoomUser_Gold",m_pPlayer[nIndex]->GetUserScore());
	int nGender = m_pPlayer[nIndex]->GetGender();

	if (nGender == 0)
	{
		WidgetFun::setVisible(this,"RoomUser_gender0",true);
		WidgetFun::setVisible(this,"RoomUser_gender1",false);
	}
	else
	{
		WidgetFun::setVisible(this,"RoomUser_gender0",false);
		WidgetFun::setVisible(this,"RoomUser_gender1",true);
	}
}

void WZQDGameScence::setGameType(int nType)
{
	ASSERT(nType == GAME_TYPE_SICHUAN || nType == GAME_TYPE_CHENGDU);
	m_nGameType = nType;
}

//void WZQDGameScence::defaultPlayerActionState()
//{
//	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
//	for (int i = 0;i<MAX_PLAYER;i++)
//	{
//		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
//	}
//	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
//	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
//}

