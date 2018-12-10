#include "SRLFGameScence.h"
#include "SRLFPlayer.h"
#include "GAME.h"
#include "CMD_SRLF.h"
#include "SRLFGameLogic.h"
#include "SRLFSoundFun.h"
#include "Game/Script/ScriptXMLparse.h"
#include "Game/FV/FvMask.h"
#include "SRLFRoomDetail.h"
//#include "SRLFSetting.h"
#include "Plaza/HomeScene/GPHomeSettingPanel.h"

#include "UTILITY.h"

FV_SINGLETON_STORAGE(SRLFGameScence);

SRLFGameScence::SRLFGameScence()
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_cbActionCard(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_bIsHuanPai(false)
	,m_nGameState(SRLF_STATE_NULL)
	,m_nGameType(GAME_TYPE_CHENGDU)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,m_bselectGangState(false)
	,GameBase(0,0)
{
	init();
}

SRLFGameScence::SRLFGameScence(int iGameType,int iOption)	
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_cbActionCard(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_bIsHuanPai(false)
	,m_nGameState(SRLF_STATE_NULL)
	,m_nGameType(GAME_TYPE_CHENGDU)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,m_dwGameRuleIdex(0)
	,m_bselectGangState(false)
	,GameBase(iGameType,iOption)
{
	init();
}

SRLFGameScence::~SRLFGameScence()
{

}
bool SRLFGameScence::init()//数据准备
{
	if (!GameBase::init())
	{
		return false;
	};
	//Node* pNode = WidgetFun::getChildWidget(this,"Player1");
	//Node* pIconNode = WidgetFun::getChildWidget(pNode, "QueIcon_Img");

	WidgetScenceXMLparse kSRLFScence("GameSRLF/xml/SRLFGameScence.xml",this);
	//cocos2d::ScriptXMLparse kSRLFScriptXml1("GameSRLF/xml/ScriptValueStr.xml");

 	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"GameTypeScenceNode");
	//聊天界面 和 设置界面
	WidgetScenceXMLparse kSRLFScence2("GameSRLF/xml/GameScence_THJ.xml",pNode);
	if (WidgetFun::getChildWidget(this,"GameTalkList"))
	{
		widget::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
		pTalkList->removeAllChildren();
		for (int i=0;i<10;i++)
		{
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("SRLFHNMJGameTalkListItem",pTalkList);
			std::string kTxt = script::getStr(utility::toString("GameTalkTxt",i));
			WidgetFun::setText(pItem,"TalkListItemTxt",kTxt);
			WidgetFun::setWidgetUserInfo(pItem,"SRLFButton_TalkDefine","Idex",utility::toString(i));
			WidgetFun::setWidgetUserInfo(pItem,"SRLFButton_TalkDefine","Txt",utility::toString(kTxt));
		}
		pTalkList->forceDoLayout();

		for (int m = 0;m<6;m++)
		{
			cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BiaoQing",m));
			for (int n = 0;n<8;n++)
			{
				std::string kFileName = utility::toString("GameSRLF/Talk/EE",m*8+n,".png");
				WidgetFun::setButtonImagic(pNode,utility::toString("HNMJButton_BiaoQing",n),kFileName,true);
				WidgetFun::setWidgetUserInfo(pNode,utility::toString("HNMJButton_BiaoQing",n),"File",kFileName);
			}
		}
	}

	initPrivate();
	m_pPlayer[0] = new SRLFPlayer(0,WidgetFun::getChildWidget(this,utility::toString("SRLFPlayer",0)));
	m_pPlayer[0]->setTingNode(WidgetFun::getChildWidget(this,utility::toString("BaoTing",0)));
	m_pPlayer[1] = new SRLFPlayer(2,WidgetFun::getChildWidget(this,utility::toString("SRLFPlayer",2)));
	m_pPlayer[1]->setTingNode(WidgetFun::getChildWidget(this,utility::toString("BaoTing",2)));
	m_pPlayer[2] = new SRLFPlayer(3,WidgetFun::getChildWidget(this,utility::toString("SRLFPlayer",3)));
	m_pPlayer[2]->setTingNode(WidgetFun::getChildWidget(this,utility::toString("BaoTing",3)));
	m_pLocal = m_pPlayer[0];
	WidgetFun::getChildWidget(this,utility::toString("SRLFPlayer",1))->setVisible(false);
	WidgetFun::getChildWidget(this,utility::toString("BaoTing",1))->setVisible(false);
	WidgetManager::addButtonCB("SRLFButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));
	initButton();
	initNet();
	initTouch();
	initSetButton();
	defaultState();
	if (SRLFRoomDetail::pInstance() == NULL)
		new SRLFRoomDetail;
	if (SRLFRoomDetail::pInstance()->getParent() == NULL)
	addChild(SRLFRoomDetail::pInstance());
		
	if (GPHomeSettingPanel::pInstance() == NULL)
		new GPHomeSettingPanel;
	if (GPHomeSettingPanel::pInstance()->getParent() != NULL)
		GPHomeSettingPanel::pInstance()->removeFromParent();
	addChild(GPHomeSettingPanel::pInstance());
	GPHomeSettingPanel::Instance().hide();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(false);
	
	if (GPHomeMallPanel::pInstance() == NULL)
		new GPHomeMallPanel;
	if (GPHomeMallPanel::pInstance()->getParent() != NULL)
		GPHomeMallPanel::pInstance()->removeFromParent();
	addChild(GPHomeMallPanel::pInstance());
	GPHomeMallPanel::Instance().hide();

	return true;
}
void SRLFGameScence::EnterScence()//开始场景
{
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"PrivateEndPlane",false);
	WidgetFun::setVisible(this,"GameSRLFPlane",true);
	defaultState();
	setVisible(true);
	SoundFun::Instance().playBackMusic("bgroom.mp3");
}
bool SRLFGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameSRLFPlane");
}
void SRLFGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameSRLFPlane",false);
	defaultPrivateState();
	defaultRecordState();
	defaultMaster();
}

void SRLFGameScence::exitRoom()
{
	//SRLFSoundFun::stopBackMusic();
	for (int iChairID=0;iChairID<MAX_PLAYER;iChairID++)
	{
		m_pPlayer[iChairID]->setValid(false);
		m_pPlayer[iChairID]->hide();
		m_pPlayer[iChairID]->EndGame();
	}
	GameBase::exitRoom();
}

void SRLFGameScence::defaultState()//清数据
{
	m_pTouchCardNode = NULL;
	m_bIsHuanPai = false;
	m_nGameState = SRLF_STATE_NULL;
	WidgetFun::setVisible(this,"TimeNode",true);
	WidgetFun::setVisible(this,"LastCardNode",true);
	changeFreeStateNodeState(false);
	WidgetFun::setVisible(this,"SaiZiNode",false);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	//WidgetFun::setVisible(this,"GameResoult1",false);
	WidgetFun::setVisible(this,"GameResoult2",false);
	WidgetFun::setVisible(this,"GameRoomUserInfo",false);
	WidgetFun::setVisible(this,"ExchangeCardNode",false);
	WidgetFun::setVisible(this,"GameSetPlane",false);
	//WidgetFun::setVisible(this,"JiaPiaoNode",false);
	defaultPrivateState();
	defaultRecordState();
	defaultMaster(true);
	SRLFRoomDetail::Instance().hide();
	if (GPHomeSettingPanel::pInstance() == NULL)
		new GPHomeSettingPanel;
	if (GPHomeSettingPanel::pInstance()->getParent() != NULL)
		GPHomeSettingPanel::pInstance()->removeFromParent();
	addChild(GPHomeSettingPanel::pInstance());
	GPHomeSettingPanel::Instance().hide();
}
std::string SRLFGameScence::getStringHuRight(dword kValue,int nGenCount)//获得胡牌
{
	dword dwRight[] = {	
		CHR_DA_DUI_ZI			,	
		CHR_QING_YI_SE		,		
		CHR_QI_XIAO_DUI		,
		CHR_LONG_QI_DUI		,
		CHR_QING_DUI			,
		CHR_DAI_YAO			,
		CHR_JIANG_DUI			,
		CHR_QING_QI_DUI		,
		CHR_QING_LONG_QI_DUI	,
		CHR_JIANG_QI_DUI		,
		CHR_TIAN_HU			,
		CHR_DI_HU			,
		CHR_QING_YAO_JIU		,
		CHR_QIANG_GANG		,	
		CHR_GANG_SHANG_PAO	,
		CHR_GANG_KAI			,
		CHR_JIN_GOU_DAO		,
		CHR_HAI_DI_HU			,
		CHR_HAI_DI_PAO		,

		CHR_PIN_HU			,
		CHR_ZI_MO			,	
		CHR_FANG_PAO			,	
		CHR_CHA_DA_JIAO		,

		CHR_PENGPENGHU	,
		CHR_QINGYISE	,
		CHR_QINGPENG	,
		CHR_MENQiNG	,
	};
	std::string pszRight[] = {
		ScriptData<std::string>("SCHuRight0").Value(),
		ScriptData<std::string>("SCHuRight1").Value(),
		ScriptData<std::string>("SCHuRight2").Value(),
		ScriptData<std::string>("SCHuRight3").Value(),
		ScriptData<std::string>("SCHuRight4").Value(),
		ScriptData<std::string>("SCHuRight5").Value(),
		ScriptData<std::string>("SCHuRight6").Value(),
		ScriptData<std::string>("SCHuRight7").Value(),
		ScriptData<std::string>("SCHuRight8").Value(),
		ScriptData<std::string>("SCHuRight9").Value(),
		ScriptData<std::string>("SCHuRight10").Value(),
		ScriptData<std::string>("SCHuRight11").Value(),
		ScriptData<std::string>("SCHuRight12").Value(),
		ScriptData<std::string>("SCHuRight13").Value(),
		ScriptData<std::string>("SCHuRight14").Value(),
		ScriptData<std::string>("SCHuRight15").Value(),
		ScriptData<std::string>("SCHuRight16").Value(),
		ScriptData<std::string>("SCHuRight17").Value(),
		ScriptData<std::string>("SCHuRight18").Value(),
		ScriptData<std::string>("SCHuRight19").Value(),
		ScriptData<std::string>("SCHuRight20").Value(),
		ScriptData<std::string>("SCHuRight21").Value(),
		ScriptData<std::string>("SCHuRight22").Value(),

		ScriptData<std::string>("SCHuRight24").Value(),
		ScriptData<std::string>("SCHuRight25").Value(),
		ScriptData<std::string>("SCHuRight26").Value(),
		ScriptData<std::string>("SCHuRight27").Value(),
	};
	std::string kTxt;
	SRLFLOGIC::CChiHuRight	kChiHuRight;	
	kChiHuRight.SetRightData(&kValue,MAX_RIGHT_COUNT );
	for( byte j = 0; j < CountArray(pszRight); j++ )
	{
		if( !(kChiHuRight&dwRight[j]).IsEmpty() )
		{
			kTxt+=pszRight[j];
		}
	}
	if (nGenCount>0)
	{
		kTxt += utility::toString(ScriptData<std::string>("HuRight_Gen").Value(),nGenCount,ScriptData<std::string>("HuRight_Fan").Value());
	}
	return kTxt;
}


std::string SRLFGameScence::getStringGang(int nChairID,SRLF::CMD_WeaveItem* pWeave,int iWeaveCout )//获得杠牌
{
	std::string kStr;
	for (int i = 0;i<iWeaveCout;i++)
	{
		SRLF::CMD_WeaveItem* pTemp = pWeave+i;
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			if ( pTemp->wProvideUser == nChairID && pTemp->cbPublicCard == 0)
			{
				kStr +=ScriptData<std::string>("AnGang").Value();
			}
			else if (pTemp->wProvideUser == nChairID && pTemp->cbPublicCard != 0)
			{
				kStr +=ScriptData<std::string>("ZiMoMingGang").Value();
			}
			else
			{
				kStr +=ScriptData<std::string>("MingGang").Value();
			}
		}

	}
	return kStr;
}


void SRLFGameScence::showSaiZi(unsigned int iValue)//塞子动画----------------
{
	word wSice1 = word(iValue>>16);
	word wSice2 = word(iValue);
	//if (wSice1 > 0)
	//{
	//	byte SiceFirst = (wSice1 >> 8);
	//	byte SiceSecond = (wSice1);
	//	std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
	//	WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
	//	WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
	//	WidgetFun::setWidgetUserInfo(this,"SaiZiNode","NextValue",utility::toString((int)wSice2));
	//	WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart1");
	//}
	//else if (wSice2 > 0)
	{
		byte SiceFirst = (wSice2 >> 8);
		byte SiceSecond = (wSice2);
		std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
		WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
		WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
		WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart2");
		getPlayerByChairID(m_iBankerUser)->setZhuang();
	}	
}

void SRLFGameScence::showSaiZi_HuanPai(word wValue)
{
	byte SiceFirst = (wValue >> 8);
	byte SiceSecond = (wValue);
	std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
	WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
	WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
	WidgetFun::runWidgetAction(this, "SaiZiNode", "ActionStart_HuanPai");//................................
	
	//WidgetFun::setVisible(pnode, true);
}
void SRLFGameScence::setCurNoticePlayer(int index )
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	pRootNode->setVisible(true);
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	if(index>=0 && index < 4 )
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",index),true);
	}
}

void SRLFGameScence::setCurrentPlayer(int iCurrentPlayer,int iUserAction,int icbCardData)
{
	WidgetFun::setVisible(this,"SelfActionNode",true);
	WidgetFun::setVisible(this,"SRLFButton_GuoAction",false);
	WidgetFun::setVisible(this,"SRLFButton_GangAction",false);
	WidgetFun::setVisible(this,"SRLFButton_PengAction",false);
	WidgetFun::setVisible(this,"SRLFButton_HuAction",false);
	SRLFPlayer* pPlyer = getPlayerByChairID(iCurrentPlayer);
	if( NULL == pPlyer) return;
	if ( pPlyer->getIdex() == -1) return;
	m_iCurrentUser = iCurrentPlayer;
	setCurNoticePlayer(pPlyer->getIdex());
	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		std::vector<cocos2d::Node*> vActionBtns;
		vActionBtns.clear();
		if (byte(iUserAction)&WIK_CHI_HU || byte(iUserAction)&WIK_ZI_MO)
			vActionBtns.push_back(WidgetFun::getChildWidgetByName(this,"SRLFButton_HuAction"));
		if (byte(iUserAction)&WIK_GANG)
			vActionBtns.push_back(WidgetFun::getChildWidgetByName(this,"SRLFButton_GangAction"));
		if (byte(iUserAction)&WIK_PENG)
			vActionBtns.push_back(WidgetFun::getChildWidgetByName(this,"SRLFButton_PengAction"));
		vActionBtns.push_back(WidgetFun::getChildWidgetByName(this,"SRLFButton_GuoAction"));
		Vec2 startPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(WidgetFun::getChildWidgetByName(this,"SelfActionNode"),"StartPos"));
		Vec2 addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(WidgetFun::getChildWidgetByName(this,"SelfActionNode"),"AddPos"));
		int count = vActionBtns.size();
		float centerCount = (float)count/2.0f;
		for (int index =0;index<count;index++)
		{
			float indexPos = (float)index - centerCount+0.5f;
			WidgetFun::setPos(vActionBtns.at(index),startPos + indexPos*addPos);
			WidgetFun::setVisible(vActionBtns.at(index),true);
		}
		m_pLocal->setTing(false);
	}
}
void SRLFGameScence::setGameResoultStateInfo(cocos2d::Node* pNode, std::string kName, std::string kDes, int lSocre)
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

void SRLFGameScence::showClock(int iTime,bool bShow /*= true*/)
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
	pNode->runAction(script::MoveExTxtTime::create(iTime,iTime,0));
}

void SRLFGameScence::showClockTimeCallBack( const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
	pNode->runAction(script::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}

void SRLFGameScence::killClock( const std::string& kName )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
}

void SRLFGameScence::setGameResoultPlayerInfo_shuju(const SRLF::AllEndInfo& pGameEnd, SRLFPlayer* pPlayer, cocos2d::Node* pNode)//玩家胡牌的信息
{
	pNode->removeAllChildren();

	int iChirID = pPlayer->GetChairID();
	int nWinGang = 0;

	std::string pai_sJ;

	for (int i = 0; i < (int)pGameEnd.kGangInfoList.size(); i++)
	{
		SRLF::CMD_S_GangScore kGangInfo = pGameEnd.kGangInfoList[i];

		int nScore = kGangInfo.lGangScore[iChirID];
		if (nScore > 0)
		{
			//setGameResoultStateInfo(pNode, pPlayerWin->GetNickName(), script::getStr("GFXY"), nScore);
			pai_sJ = pai_sJ + " " + script::getStr("GFXY");
		}
	}
	for (int i = 0; i < (int)pGameEnd.kChiHuInfoList.size(); i++)
	{
		SRLF::CMD_S_ChiHu kChiHuInfo = pGameEnd.kChiHuInfoList[i];
		int nScore = kChiHuInfo.lGameScore[iChirID];
		if (nScore > 0)
		{
			SRLFPlayer* pPlayerWin = getPlayerByChairID(kChiHuInfo.wChiHuUser);
			//pPlayerWin->setChiHuCard(kChiHuInfo.cbChiHuCard);
			std::string kTemp = getStringHuRight(kChiHuInfo.dwChiHuRight, kChiHuInfo.cbGenCount);
			pai_sJ = pai_sJ + " " + kTemp;

			if (pPlayerWin->isBaoTing())
			{
				pai_sJ = pai_sJ + " " + utility::a_u8("报叫");
			}

			/*if (pPlayerWin->isPiao())
			{
				pai_sJ = pai_sJ + " " + utility::a_u8("飘");
			}*/

		}
	}

	if (pGameEnd.lChaJiaoScore[iChirID] != 0)
	{
		std::string kTemp = ScriptData<std::string>("SCHuRight22").Value();
		pai_sJ = pai_sJ + " " + kTemp;
	}
	if (pGameEnd.lHuaZhuScore[iChirID] != 0)
	{
		std::string kTemp = ScriptData<std::string>("SCHuRight23").Value();
		pai_sJ = pai_sJ + " " + kTemp;
	}

	WidgetFun::setText(pNode,  pai_sJ);
	word selfChairID = m_pLocal->GetChairID();
	int lSelfSocre = pGameEnd.lGameScoreTotal[selfChairID];
	WidgetFun::setVisible(this, "WinTitle", lSelfSocre >= 0);
	WidgetFun::setVisible(this, "LoseTitle", lSelfSocre < 0);

}

void SRLFGameScence::setGameResoultPlayerInfo(const SRLF::AllEndInfo& pGameEnd,SRLFPlayer* pPlayer,cocos2d::Node* pNode)//设置玩家信息数据
{
	widget::ListViewEx* pList = (widget::ListViewEx*)pNode;//自动排版
	pList->removeAllChildren();

	int iChirID = pPlayer->GetChairID();
	int nWinGang = 0;

	for (int i=0;i<(int)pGameEnd.kGangInfoList.size();i++)
	{
		SRLF::CMD_S_GangScore kGangInfo = pGameEnd.kGangInfoList[i];

		int nScore = kGangInfo.lGangScore[iChirID];
		if (nScore !=0)
		{
			SRLFPlayer* pPlayerWin = getPlayerByChairID(kGangInfo.wChairId);
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget("SRLFGameResoultState",pList);
			setGameResoultStateInfo(pNode,pPlayerWin->GetNickName(),script::getStr("GFXY"),nScore);

		}
	}

	for (int i=0;i<(int)pGameEnd.kChiHuInfoList.size();i++)
	{
		SRLF::CMD_S_ChiHu kChiHuInfo = pGameEnd.kChiHuInfoList[i];
		int nScore = kChiHuInfo.lGameScore[iChirID];
		if (nScore != 0)
		{
			SRLFPlayer* pPlayerWin = getPlayerByChairID(kChiHuInfo.wChiHuUser);
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget("SRLFGameResoultState",pList);
			std::string kTemp = getStringHuRight(kChiHuInfo.dwChiHuRight,kChiHuInfo.cbGenCount);
			setGameResoultStateInfo(pNode,pPlayerWin->GetNickName(),kTemp,nScore);
		}
	}

	if (pGameEnd.lChaJiaoScore[iChirID] !=0)
	{
		std::string kTemp = ScriptData<std::string>("SCHuRight22").Value();
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("SRLFGameResoultState",pList);
		setGameResoultStateInfo(pNode,pPlayer->GetNickName(),kTemp,pGameEnd.lChaJiaoScore[iChirID]);
	}
	if (pGameEnd.lHuaZhuScore[iChirID] !=0)
	{
		std::string kTemp = ScriptData<std::string>("SCHuRight23").Value();
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("SRLFGameResoultState",pList);
		setGameResoultStateInfo(pNode,pPlayer->GetNickName(),kTemp,pGameEnd.lHuaZhuScore[iChirID]);
	}
	pList->refreshView();

	word selfChairID =  m_pLocal->GetChairID();
	int lSelfSocre  = pGameEnd.lGameScoreTotal[selfChairID];
	WidgetFun::setVisible(this,"WinTitle",lSelfSocre >= 0);
	WidgetFun::setVisible(this,"LoseTitle",lSelfSocre < 0);
}


void SRLFGameScence::XuanQueTimeOut()
{
	SRLF::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();

	cocos2d::log("Send XuanQue Color int :%d",(int)kInfo.nQueChoice);
	cocos2d::log("Send XuanQue Color byty:%d",kInfo.nQueChoice);

	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));

}

void SRLFGameScence::DoXuanQueNotice()
{
	//showClockTimeCallBack("ActPlayerLastTime",m_nLeftTime,CC_CALLBACK_0(SRLFGameScence::XuanQueTimeOut,this),0);
	for (int i=0;i<MAX_PLAYER;i++)
	{
		auto pPlayer = getPlayerByChairID(i);
		pPlayer->showXuanQue(true);
		pPlayer->blackAllHandCard(true);
	}
}


void SRLFGameScence::showRoomUserInfo( int nIndex )
{
	ASSERT(nIndex >=0 && nIndex<MAX_PLAYER);
	if (nIndex <0 || nIndex>=MAX_PLAYER)
	{
		return;
	}
	auto pPlayer = getPlayerByChairID(nIndex);
	WidgetFun::setText(this,"RoomUser_name",pPlayer->GetNickName());
	WidgetFun::setText(this,"RoomUser_ID",pPlayer->GetUserID());
	WidgetFun::setText(this,"RoomUser_Gold",pPlayer->GetUserScore());
	int nGender = pPlayer->GetGender();

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

void SRLFGameScence::setGameType(int nType)
{
	ASSERT(nType == GAME_TYPE_SICHUAN || nType == GAME_TYPE_CHENGDU);
	m_nGameType = nType;
}

void SRLFGameScence::defaultPlayerActionState()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	//WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	//WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
	showClock(0,true);
}

void SRLFGameScence::showPiao()
{
	bool isAllowJiaPiao = FvMask::HasAll(m_dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_JP)) && (m_nGameState == SRLF_STATE_NULL); 
	WidgetFun::setVisible(this,"JiaPiaoNode",isAllowJiaPiao);
}



