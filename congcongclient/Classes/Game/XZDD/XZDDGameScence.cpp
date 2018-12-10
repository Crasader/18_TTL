#include "XZDDGameScence.h"
#include "XZDDPlayer.h"
#include "GAME.h"
#include "CMD_XZDD.h"
#include "XZDDGameLogic.h"
#include "XZDDSoundFun.h"
#include "Game/Script/ScriptXMLparse.h"
#include "Plaza/HomeScene/GPHomeSettingPanel.h"

FV_SINGLETON_STORAGE(XZDDGameScence);

XZDDGameScence::XZDDGameScence()
	:m_iBankerUser(INVALID_CHAIR)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_cbOutCardData(0)
	,m_cbActionCardData(0)
	,m_pTouchCardNode(NULL)
	,m_pTouchCardNode_old(NULL)
	,m_bIsHuanPai(false)
	,m_nGameState(XZDD_STATE_NULL)
	,m_nGameType(GAME_TYPE_CHENGDU)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,m_bselectGangState(false)
	,m_nGangCardsCount(0)
	,GameBase(0,0)
{
	init();
}

XZDDGameScence::XZDDGameScence(int iGameType,int iOption)	
	:m_iBankerUser(INVALID_CHAIR)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_cbOutCardData(0)
	,m_cbActionCardData(0)
	,m_pTouchCardNode(NULL)
	,m_pTouchCardNode_old(NULL)
	,m_bIsHuanPai(false)
	,m_nGameState(XZDD_STATE_NULL)
	,m_nGameType(GAME_TYPE_CHENGDU)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,m_bselectGangState(false)
	,m_nGangCardsCount(0)
	,GameBase(iGameType,iOption)
{
	init();
}

XZDDGameScence::~XZDDGameScence()
{

}

bool XZDDGameScence::init()//数据准备
{
	if (!GameBase::init())
	{
		return false;
	};

	m_Touch_CS = 0;
	//Node* pNode = WidgetFun::getChildWidget(this,"Player1");
	//Node* pIconNode = WidgetFun::getChildWidget(pNode, "QueIcon_Img");

	WidgetScenceXMLparse kScence("GameXZDD/xml/XZDDGameScence.xml",this);
	//cocos2d::ScriptXMLparse kScriptXml1("GameXZDD/xml/ScriptValueStr.xml");

 	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"GameTypeScenceNode");
	WidgetScenceXMLparse kScence2("GameXZDD/xml/GameScence_THJ.xml",pNode);


    pnode = WidgetFun::getChildWidget(this, "Cardpaidui");
	if (pnode)
		pnode->removeAllChildren();

	int tag = 1000;
	for (int i = 0; i < 28; ++i)
	{
		if (i < 14)
		{
			std::string paidui11 = utility::toString("paidui");
			Node* paidui = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui, cocos2d::Vec2(paidui->getPosition().x + 37 * i, paidui->getPosition().y));
			paidui->setTag(tag + 13 - i );
			paidui11 = utility::toString("paidui_3");
			Node* paidui1 = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui1, cocos2d::Vec2(paidui1->getPosition().x + 28 * i, paidui1->getPosition().y ));
			paidui1->setTag(tag * 3 + i);
			paidui1->setZOrder(10);
		}
		else
		{
			std::string paidui11 = utility::toString("paidui");
			Node* paidui = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui, cocos2d::Vec2(paidui->getPosition().x + 37 * (i % 14), paidui->getPosition().y + 15));
			paidui->setTag(tag + 113 - (i % 14));
			paidui11 = utility::toString("paidui_3");
			Node* paidui1 = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui1, cocos2d::Vec2(paidui1->getPosition().x + 28 * (i % 14), paidui1->getPosition().y  + 10));
			paidui1->setTag(tag * 3 + 100 + (i % 14));
			paidui1->setZOrder(10);
		}
	}
	for (int i = 0; i < 26; ++i)
	{
		if (i < 13)
		{
			std::string paidui11 = utility::toString("paidui_1");
			Node* paidui = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui, cocos2d::Vec2(paidui->getPosition().x - 6 * (12 - i), paidui->getPosition().y + 24 * (12 - i)));
			paidui->setTag(tag * 4  + i);
			paidui11 = utility::toString("paidui_2");
			Node* paidui1 = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui1, cocos2d::Vec2(paidui1->getPosition().x - 6 * i, paidui1->getPosition().y + 24 * (12 - i)));
			paidui1->setTag(tag * 2 + (12 - i));
		}
		else
		{
			std::string paidui11 = utility::toString("paidui_1");
			Node* paidui = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui, cocos2d::Vec2(paidui->getPosition().x + 6 - 6 * (12 - (i % 13)), paidui->getPosition().y + 12 + 24 * (12 - (i % 13))));
			paidui->setTag(tag * 4 + 100 + (i % 13));
			paidui11 = utility::toString("paidui_2");
			Node* paidui1 = WidgetManager::Instance().createWidget(paidui11, pnode);
			WidgetFun::setPos(paidui1, cocos2d::Vec2(paidui1->getPosition().x - 6 - (6 * (i % 13)), paidui1->getPosition().y + 12 + 24 * (12 - (i % 13))));
			paidui1->setTag(tag * 2 + 112 - (i % 13));
		}
	}
	
	WidgetFun::setVisible(pnode, true);
	/*int tag = 0;
	Node* pRootNode;
	pRootNode->getChildByTag(tag);*/

	if (WidgetFun::getChildWidget(this,"GameTalkList"))
	{
		widget::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
		pTalkList->removeAllChildren();
		for (int i=0;i<10;i++)
		{
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("HNMJGameTalkListItem",pTalkList);
			std::string kTxt = script::getStr(utility::toString("GameTalkTxt",i));
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
		m_pPlayer[i] = new XZDDPlayer(i,WidgetFun::getChildWidget(this,utility::toString("XZDDPlayer",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("XZDDButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	initSetButton();
	defaultState();
	return true;
}

void XZDDGameScence::EnterScence()//开始场景
{
	if (GPHomeSettingPanel::pInstance())
	{
		GPHomeSettingPanel::pInstance()->removeFromParent();
	}
	addChild(GPHomeSettingPanel::pInstance());
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"PrivateEndPlane",false);
	WidgetFun::setVisible(this,"GameXZDDPlane",true);
	defaultState();
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->PlayerLeave();
	}

	WidgetFun::setVisible(this,"MenuNode",true);
	setVisible(true);
	SoundFun::Instance().playBackMusic("bgroom.mp3");
}

bool XZDDGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameXZDDPlane");
}

void XZDDGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameXZDDPlane",false);
	defaultPrivateState();
	defaultRecordState();
	defaultMaster();
}

void XZDDGameScence::exitRoom()
{
	XZDDSoundFun::stopBackMusic();
	GameBase::exitRoom();

}

void XZDDGameScence::defaultState()//清数据
{
	m_pTouchCardNode = NULL;
	m_bIsHuanPai = false;
	m_nGameState = XZDD_STATE_NULL;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
		m_pPlayer[i]->EndGame();
	}

	WidgetFun::setVisible(this,"TimeNode",true);//改
	WidgetFun::setVisible(this, "LastCardNode", false);
	WidgetFun::setVisible(this,"FreeStateNode",false);
	WidgetFun::setVisible(this,"SaiZiNode",false);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	//WidgetFun::setVisible(this,"GameResoult1",false);
	WidgetFun::setVisible(this,"GameResoult2",false);
	WidgetFun::setVisible(this,"GameRoomUserInfo",false);
	WidgetFun::setVisible(this,"ExchangeCardNode",false);
	WidgetFun::setVisible(this,"GameSetPlane",false);

	

	defaultPrivateState();
	defaultRecordState();
	defaultMaster(true);
}

std::string XZDDGameScence::getStringHuRight(dword kValue,int nGenCount)//获得胡牌
{
	dword dwRight[] = {	
		CHR_DA_DUI_ZI		,	
		CHR_QING_YI_SE		,		
		CHR_QI_XIAO_DUI		,
		CHR_LONG_QI_DUI		,
		CHR_QING_DUI		,
		CHR_DAI_YAO			,
		CHR_JIANG_DUI		,
		CHR_QING_QI_DUI		,
		CHR_QING_LONG_QI_DUI,
		CHR_JIANG_QI_DUI	,
		CHR_TIAN_HU			,
		CHR_DI_HU			,
		CHR_QING_YAO_JIU	,
		CHR_QIANG_GANG		,	
		CHR_GANG_SHANG_PAO	,
		CHR_GANG_KAI		,
		CHR_JIN_GOU_DAO		,
		CHR_HAI_DI_HU		,
		CHR_HAI_DI_PAO		,

		CHR_PIN_HU			,
		CHR_ZI_MO			,	
		CHR_FANG_PAO		,
		CHR_CHA_HUA_ZHU		,
		CHR_CHA_DA_JIAO		,				
	
		CHR_PENGPENGHU	,
		CHR_QINGYISE	,
		CHR_QINGPENG	,
		CHR_MENQiNG		,
		CHR_CHA_ZHONG_ZHANG,
	};
	std::string pszRight[] = {
		ScriptData<std::string>("XZDDSCHuRight0").Value(),
		ScriptData<std::string>("XZDDSCHuRight1").Value(),
		ScriptData<std::string>("XZDDSCHuRight2").Value(),
		ScriptData<std::string>("XZDDSCHuRight3").Value(),
		ScriptData<std::string>("XZDDSCHuRight4").Value(),

		ScriptData<std::string>("XZDDSCHuRight5").Value(),
		ScriptData<std::string>("XZDDSCHuRight6").Value(),
		ScriptData<std::string>("XZDDSCHuRight7").Value(),
		ScriptData<std::string>("XZDDSCHuRight8").Value(),
		ScriptData<std::string>("XZDDSCHuRight9").Value(),

		ScriptData<std::string>("XZDDSCHuRight10").Value(),
		ScriptData<std::string>("XZDDSCHuRight11").Value(),
		ScriptData<std::string>("XZDDSCHuRight12").Value(),
		ScriptData<std::string>("XZDDSCHuRight13").Value(),
		ScriptData<std::string>("XZDDSCHuRight14").Value(),

		ScriptData<std::string>("XZDDSCHuRight15").Value(),
		ScriptData<std::string>("XZDDSCHuRight16").Value(),
		ScriptData<std::string>("XZDDSCHuRight17").Value(),
		ScriptData<std::string>("XZDDSCHuRight18").Value(),
		ScriptData<std::string>("XZDDSCHuRight19").Value(),

		ScriptData<std::string>("XZDDSCHuRight20").Value(),
		ScriptData<std::string>("XZDDSCHuRight21").Value(),
		ScriptData<std::string>("XZDDSCHuRight22").Value(),
		ScriptData<std::string>("XZDDSCHuRight23").Value(),

		ScriptData<std::string>("XZDDSCHuRight24").Value(),
		ScriptData<std::string>("XZDDSCHuRight25").Value(),
		ScriptData<std::string>("XZDDSCHuRight26").Value(),
		ScriptData<std::string>("XZDDSCHuRight27").Value(),
		ScriptData<std::string>("XZDDSCHuRight28").Value(),
	};
	std::string kTxt;
	XZDD::CChiHuRight	kChiHuRight;
	kChiHuRight.SetRightData(&kValue,MAX_RIGHT_COUNT );
	for( byte j = 0; j < CountArray(pszRight); j++ )
	{
		if( !(kChiHuRight&dwRight[j]).IsEmpty() && dwRight[j]!=CHR_PIN_HU && dwRight[j]!=CHR_FANG_PAO )
		{
			kTxt+=pszRight[j].c_str();
		}
	}
	if (nGenCount>0)
	{
		kTxt += utility::toString(ScriptData<std::string>("HuRight_Gen").Value().c_str(),nGenCount,ScriptData<std::string>("HuRight_Fan").Value().c_str());
	}
	return kTxt;
}

std::string XZDDGameScence::getStringGang(int nChairID, XZDD::CMD_WeaveItem* pWeave, int iWeaveCout)//获得杠牌
{
	std::string kStr;
	for (int i = 0;i<iWeaveCout;i++)
	{
		XZDD::CMD_WeaveItem* pTemp = pWeave + i;
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

void XZDDGameScence::showSaiZi(unsigned int iValue)//塞子动画----------------
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
	}	
}

void XZDDGameScence::showSaiZi_HuanPai(word wValue)
{
	/*byte SiceFirst = (wValue >> 8);
	byte SiceSecond = (wValue);
	std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
	WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
	WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));*/
	WidgetFun::runWidgetAction(this, "SaiZiNode", "ActionStart_HuanPai");//................................

	//WidgetFun::setVisible(pnode, true);
}

void XZDDGameScence::setCurrentPlayer(int iCurrentPlayer,int iUserAction,int icbCardData)
{
	m_iCurrentUser = iCurrentPlayer;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");

	XZDDPlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
	////倒计时15秒
	//WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->runAction(
	//	script::MoveExTxtTime::create(TIME_OPERATE_CARD));
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	if (iUserAction == WIK_NULL)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",pPlyer->getIdex()),true);
	}

	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		WidgetFun::setVisible(this,"SelfActionNode",true);
		int iCout = 0;
		int iStartPos = 0;
		int iNextPos = 0;
		if (byte(iUserAction)&WIK_GANG)
		{
			iCout++;
		}
		if (byte(iUserAction)&WIK_PENG)
		{
			iCout++;
		}
		if (byte(iUserAction)&byte(iUserAction)&WIK_CHI_HU || byte(iUserAction)&WIK_ZI_MO)
		{
			iCout++;
		}
		if (iCout == 1)
		{
			iStartPos = -140;
			iNextPos = 280;
		}
		if (iCout == 2)
		{
			iStartPos = -200;
			iNextPos = 280;
		}
		if (iCout == 3)
		{
			iStartPos = -300;
			iNextPos = 280;
		}
		if (byte(iUserAction)&WIK_CHI_HU || byte(iUserAction)&WIK_ZI_MO)
		{
			WidgetFun::setPos(this,"XZDDButton_HuAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (byte(iUserAction)&WIK_GANG)
		{
			WidgetFun::setPos(this,"XZDDButton_HuAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		if (byte(iUserAction)&WIK_PENG)
		{
			WidgetFun::setPos(this,"XZDDButton_PengAction",cocos2d::Vec2(iStartPos,0));
			iStartPos+=iNextPos;
		}
		WidgetFun::setPos(this,"XZDDButton_GuoAction",cocos2d::Vec2(iStartPos,0));
		WidgetFun::setVisible(this,"XZDDButton_GangAction",byte(iUserAction)&WIK_GANG);
		WidgetFun::setVisible(this,"XZDDButton_PengAction",byte(iUserAction)&WIK_PENG);
		WidgetFun::setVisible(this,"XZDDButton_HuAction",byte(iUserAction)&WIK_CHI_HU || byte(iUserAction)&WIK_ZI_MO);
		//WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString(icbCardData));
	}
}

void XZDDGameScence::setGameResoultStateInfo(cocos2d::Node* pNode, std::string kName, std::string kDes, int lSocre)
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

void XZDDGameScence::showClock(int iTime,bool bShow /*= true*/)
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

void XZDDGameScence::showClockTimeCallBack( const std::string& kName,int nSecond,const std::function<void()>& pCallBack,float fCheakTime )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
	pNode->runAction(script::MoveExTxtTimeCallBack::create(nSecond,nSecond,0,pCallBack,fCheakTime));
}

void XZDDGameScence::killClock( const std::string& kName )
{
	Node* pNode = WidgetFun::getChildWidget(this,kName);
	pNode->stopAllActions();
}

void XZDDGameScence::setGameResoultPlayerInfo_shuju(const XZDD::AllEndInfo& pGameEnd, XZDDPlayer* pPlayer, cocos2d::Node* pNode)//玩家胡牌的信息
{
	pNode->removeAllChildren();

	int iChirID = pPlayer->GetChairID();
	int nWinGang = 0;

	std::string pai_sJ;
	int mk  = 0;
	for (int i = 0; i < (int)pGameEnd.kGangInfoList.size(); i++)
	{
		XZDD::CMD_S_GangScore kGangInfo = pGameEnd.kGangInfoList[i];
		int nScore = kGangInfo.lGangScore[iChirID];
		if (nScore > 0)
		{
			mk++;
		}
	}
	if(mk > 0)
		pai_sJ = pai_sJ + script::getStr("GFXY")+ utility::toString("x",mk);

	for (int i = 0; i < (int)pGameEnd.kChiHuInfoList.size(); i++)
	{
		XZDD::CMD_S_ChiHu kChiHuInfo = pGameEnd.kChiHuInfoList[i];
		int nScore = kChiHuInfo.lGameScore[iChirID];
		if (nScore > 0)
		{
			XZDDPlayer* pPlayerWin = getPlayerByChairID(kChiHuInfo.wChiHuUser);
			std::string kTemp = getStringHuRight(kChiHuInfo.dwChiHuRight, kChiHuInfo.cbGenCount);
			pai_sJ = pai_sJ + " " + kTemp;
		}
	}

	if (pGameEnd.lChaJiaoScore[iChirID] > 0 || pGameEnd.lHuaZhuScore[iChirID] > 0)
	{
		std::string kTemp = getStringHuRight(pGameEnd.lChaJiaoMaxRight[iChirID], pGameEnd.cbGenCount[iChirID]);
		pai_sJ = pai_sJ + " " + kTemp;
	}
	if (pGameEnd.lChaJiaoScore[iChirID] != 0)
	{
		std::string kTemp = ScriptData<std::string>("XZDDSCHuRight22").Value();
		if (pGameEnd.lChaJiaoScore[iChirID] < 0) 
			pai_sJ = pai_sJ + kTemp;
	}

	if (pGameEnd.lHuaZhuScore[iChirID] != 0 )
	{
		std::string kTemp = ScriptData<std::string>("XZDDSCHuRight23").Value();
		if (pGameEnd.lHuaZhuScore[iChirID] < 0) 
			pai_sJ = pai_sJ + kTemp;
	}
	WidgetFun::setText(pNode,  pai_sJ);
	//WidgetFun::setVisible(pNode, false);
	word selfChairID = m_pLocal->GetChairID();
	int lSelfSocre = pGameEnd.lGameScoreTotal[selfChairID];
	WidgetFun::setVisible(this, "WinTitle", lSelfSocre >= 0);
	WidgetFun::setVisible(this, "LoseTitle", lSelfSocre < 0);

}

void XZDDGameScence::setGameResoultPlayerInfo(const XZDD::AllEndInfo& pGameEnd, XZDDPlayer* pPlayer, cocos2d::Node* pNode)//设置玩家信息数据
{
	widget::ListViewEx* pList = (widget::ListViewEx*)pNode;//自动排版
	pList->removeAllChildren();

	int iChirID = pPlayer->GetChairID();
	int nWinGang = 0;

	for (int i=0;i<(int)pGameEnd.kGangInfoList.size();i++)
	{
		XZDD::CMD_S_GangScore kGangInfo = pGameEnd.kGangInfoList[i];

		int nScore = kGangInfo.lGangScore[iChirID];
		if (nScore !=0)
		{
			XZDDPlayer* pPlayerWin = getPlayerByChairID(kGangInfo.wChairId);
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
			setGameResoultStateInfo(pNode,pPlayerWin->GetNickName(),script::getStr("GFXY"),nScore);
		}
	}

	for (int i=0;i<(int)pGameEnd.kChiHuInfoList.size();i++)
	{
		XZDD::CMD_S_ChiHu kChiHuInfo = pGameEnd.kChiHuInfoList[i];
		int nScore = kChiHuInfo.lGameScore[iChirID];
		if (nScore != 0)
		{
			XZDDPlayer* pPlayerWin = getPlayerByChairID(kChiHuInfo.wChiHuUser);
			cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
			std::string kTemp = getStringHuRight(kChiHuInfo.dwChiHuRight,kChiHuInfo.cbGenCount);
			setGameResoultStateInfo(pNode,pPlayerWin->GetNickName(),kTemp,nScore);
		}
	}

	if (pGameEnd.lChaJiaoScore[iChirID] !=0)
	{
		std::string kTemp = ScriptData<std::string>("SCHuRight22").Value();
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
		setGameResoultStateInfo(pNode,pPlayer->GetNickName(),kTemp,pGameEnd.lChaJiaoScore[iChirID]);
	}
	if (pGameEnd.lHuaZhuScore[iChirID] !=0)
	{
		std::string kTemp = ScriptData<std::string>("SCHuRight23").Value();
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget("XZDDGameResoultState",pList);
		setGameResoultStateInfo(pNode,pPlayer->GetNickName(),kTemp,pGameEnd.lHuaZhuScore[iChirID]);
	}
	pList->refreshView();

	word selfChairID =  m_pLocal->GetChairID();
	int lSelfSocre  = pGameEnd.lGameScoreTotal[selfChairID];
	WidgetFun::setVisible(this,"WinTitle",lSelfSocre >= 0);
	WidgetFun::setVisible(this,"LoseTitle",lSelfSocre < 0);
}

void XZDDGameScence::XuanQueTimeOut()//选缺时间到确定选缺
{
	XZDD::CMD_C_XuanQue kInfo;
	kInfo.nQueChoice = m_pLocal->getQueColor();

	cocos2d::log("Send XuanQue Color int :%d",(int)kInfo.nQueChoice);
	cocos2d::log("Send XuanQue Color byty:%d",kInfo.nQueChoice);

	SendSocketData(SUB_C_XUANQUE,&kInfo,sizeof(kInfo));

}

void XZDDGameScence::DoXuanQueNotice()//选缺消息
{
	//showClock(m_nLeftTime,true);
	//showClockTimeCallBack("ActPlayerLastTime",m_nLeftTime,CC_CALLBACK_0(XZDDGameScence::XuanQueTimeOut,this),0);
	for (int i=0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->showXuanQue(true);
		m_pPlayer[i]->blackAllHandCard(true);
	}
}

void XZDDGameScence::showRoomUserInfo( int nIndex )
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

void XZDDGameScence::setGameType(int nType)
{
	ASSERT(nType == GAME_TYPE_SICHUAN || nType == GAME_TYPE_CHENGDU);
	m_nGameType = nType;
}

void XZDDGameScence::defaultPlayerActionState()
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

void XZDDGameScence::updataPlayerZhuangState()
{
	for (word wChairID =0;wChairID<MAX_PLAYER;wChairID++)
	{
		auto pPlayer = getPlayerByChairID(wChairID);
		if(pPlayer)
			pPlayer->setZhuang(m_iBankerUser==wChairID);
	}
}
