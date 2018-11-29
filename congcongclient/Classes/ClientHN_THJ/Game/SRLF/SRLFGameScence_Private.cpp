#include "SRLFGameScence.h"
#include "Game/GameLib.h"
#include "SRLFPlayer.h"
#include "Game/Game/MissionWeiXin.h"

void SRLFGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("SCMJRule",(int)nRuleTag)).Value());
	}
}
void SRLFGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameSRLF/xml/HNGamePrivateScence.xml",this);

	//WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(SRLFGameScence::SRLFButton_WeiXinFriend));
	//WidgetManager::addButtonCB("Button_PrivateEndWeiXinShare",this,button_selector(SRLFGameScence::SRLFButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(SRLFGameScence::ButtonPlayerHeadClick));
}
void SRLFGameScence::defaultPrivateState()//关闭房间
{
	if (WidgetFun::getChildWidget(this,"PrivateDismissPlane"))
	{
		WidgetFun::setVisible(this,"PrivateDismissPlane",false);
		//WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",false);
		WidgetFun::setVisible(this,"TalkImagic",false);
	}
	if (WidgetFun::getChildWidget(this,"PrivateEndPlane"))
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	if (WidgetFun::getChildWidget(this,"GameJieSuanNode"))
	{
		WidgetFun::setVisible(this,"GameJieSuanNode",false);
	}
}
void SRLFGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)//接受到房间信息
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
	setPlayCount(pNetInfo->dwPlayCout);
	setPrivateInfo(pNetInfo);
}

void SRLFGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)//设置房间私有信息
{
	m_kPrivateRoomInfo = *pNetInfo;
	WidgetFun::setText(this,"RoomID",utility::toString(utility::a_u8("房间号："),pNetInfo->dwRoomNum));//设置房间号
	std::string roomRule ="";
	int renShu = 0;
	int cardShu = 0;
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN))) //人数
		renShu = 3;
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN))) //人数
		renShu = 2;
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_14CARDS))) //14张牌
		cardShu = 13;
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_8CARDS))) //8张牌
		cardShu = 7;
	std::string ruleName ="";
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_XZDD))) //
		ruleName =" 血战";
	else if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_TDH))) //
		ruleName =" 推倒";

	std::string ziMoName ="";
	if (FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIADI))) 
		ziMoName ="自摸底";
	else if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIAFAN))) 
		ziMoName ="自摸番";
	
	WidgetFun::setText(this,"RoomNum",utility::a_u8(utility::toString(renShu,"人",cardShu,"张",ruleName," ",ziMoName)));
	WidgetFun::setVisible(this,"RoomNum",true);
	
	std::string gangHuaName ="";
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_DP))) //
		gangHuaName ="点杠炮";
	else if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_ZM))) //
		gangHuaName ="点杠花";

	std::string jiaPiaoName ="";
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_JP))) //
		jiaPiaoName ="加飘";

	std::string xiaYuName ="";
	if (FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_GFXY))) 
		xiaYuName ="下雨";

	std::string haiDiName ="";
	if (FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_LAO)))
		haiDiName ="海底";

	WidgetFun::setText(this,"RoomGameRule0",utility::a_u8(utility::toString(gangHuaName," ",jiaPiaoName," ",xiaYuName)));
	WidgetFun::setVisible(this,"RoomGameRule0",true);

	WidgetFun::setVisible(this,"RoomGameRule1",true);
	WidgetFun::setText(this,"RoomGameRule1",utility::a_u8(utility::toString("底注：",pNetInfo->dwBaseScore)));
	
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN))) //人数
		m_pPlayerCount = 3;
	if(FvMask::HasAll(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN))) //人数
		m_pPlayerCount = 2;
	m_dwGameRuleIdex = m_kPrivateRoomInfo.bGameRuleIdex;
	//showPiao();
}

void SRLFGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != m_pPlayerCount
		||pNetInfo->lPlayerAction.size() != m_pPlayerCount*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != m_pPlayerCount");
		return;
	}

	int nMaxPao = 0;
	int nMaxWin = 0;
	int nMaxPaoID = -1;
	int nMaxWinID = -1;
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		GamePlayer* pPlayer = m_pPlayer[i];
		if (!pPlayer)
		{
			continue;
		}
		int iChairID = pPlayer->GetChairID();
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",i)));
		WidgetFun::setText(pNode,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pNode,"ID",(int)pPlayer->GetUserID());
		WidgetFun::setText(pNode,"ActionTxt0",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+0]));
		WidgetFun::setText(pNode,"ActionTxt1",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+1]));
		WidgetFun::setText(pNode,"ActionTxt2",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+2]));
		WidgetFun::setText(pNode,"ActionTxt3",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+3]));
		WidgetFun::setText(pNode,"ActionTxt4",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+4]));
		WidgetFun::setText(pNode,"TotalScore",utility::toString((int)pNetInfo->lPlayerWinLose[iChairID]));
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
		WidgetFun::setVisible(pNode,"DaYingJia",false);
		WidgetFun::setVisible(pNode,"ZuiJiaPaoShou",false);

		int nPaoNum = (int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+2];
		int nWinNum = (int)pNetInfo->lPlayerWinLose[iChairID];
		if (nPaoNum>nMaxPao)
		{
			nMaxPao = nPaoNum;
			nMaxPaoID = i;
		}
		if (nWinNum>nMaxWin)
		{
			nMaxWin = nWinNum;
			nMaxWinID = i;
		}
	}

	if (nMaxPao>0)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",nMaxPaoID)));
		WidgetFun::setVisible(pNode,"ZuiJiaPaoShou",true);
	}
	if (nMaxWin>0)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",nMaxWinID)));
		WidgetFun::setVisible(pNode,"DaYingJia",true);
	}
	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	if (pJieSuanNode->getActionByTag(1) == NULL 
		&& pJieSuanNode->isVisible() == false)
	{
		WidgetFun::setVisible(pRootNode,"PrivateEndPlane",true);
	}
	else
	{
		int iTime = utility::parseInt(WidgetFun::getWidgetUserInfo(pJieSuanNode,"Time"))+3;
		WidgetFun::getChildWidget(pRootNode,"PrivateEndPlane")->runAction(cocos2d::CCVisibleAction::create(iTime,true));
	}

	if(HaveOptionRule(GAME_OPTION_RULE_PRIVATAEND_RETURN_HOME))
	{
		WidgetFun::setVisible(pRootNode,"Button_PrivateReturnHome",true);
		WidgetFun::setVisible(pRootNode,"Button_PrivateAgagin",false);
	}
}
void SRLFGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)//解散信息
{
	Button_ExitGameBase(NULL,NULL);
	return;
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	pNode->setVisible(true);
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(1.0f,false));
		return;
	}
	int kChairID[4] = {1,1,1,1};

	bool bShowSelfAction = true;
	int iIdex = 0;
	for (int i = 0;i<(int)pNetInfo->dwDissUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwDissChairID[i]);
		kChairID[pNetInfo->dwDissChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text0",kName);
		iIdex++;
	}
	for (int i = 0;i<(int)pNetInfo->dwNotAgreeUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwNotAgreeChairID[i]);
		kChairID[pNetInfo->dwNotAgreeChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text1",kName);
		iIdex++;
	}
	for (int i = 0;i<4;i++)
	{
		if (kChairID[i] == 0)
		{
			continue;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),"");
		WidgetFun::setText(pNode,utility::toString("DissPrivateState",iIdex),"");
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		iIdex++;
	}
	WidgetFun::setVisible(pNode,"PrivateDismissAction",bShowSelfAction);
}
void SRLFGameScence::SRLFButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)//要请微信好友信息
{
	std::string kHNWeiXinSharDes,kGameTypeStr;

	if (FvMask::HasAny(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GAME_RULE_XZ)))
	{
		kGameTypeStr = ScriptData<std::string>("SCMJRule2").Value();
	}
	if (FvMask::HasAny(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GAME_RULE_XL)))
	{
		kGameTypeStr = ScriptData<std::string>("SCMJRule3").Value();
	}
	//kHNWeiXinSharDes = utility::getScriptReplaceValue("SCWeiXinSharDes",
	//	(int)m_kPrivateRoomInfo.dwRoomNum,(int)m_kPrivateRoomInfo.dwPlayTotal,kGameTypeStr);
	//MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("WeiXinSharUrl"),
	//	utility::getScriptString("SCWeiXinSharTitle"),
	//	kHNWeiXinSharDes,
	//	MissionWeiXin::SHARE_SESSION);
}
void SRLFGameScence::SRLFButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static SRLFGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
			pTemp->Button_ExitGameBase(NULL,NULL);
			pTemp->exitRoom();
			WidgetFun::setVisible(pTemp,"PrivateEndPlane",false); 
		}
	},"screenshot.png");
}
void SRLFGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)//点击头像按钮
{
	GamePlayer* pPlayer = getPlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),
		utility::paseInt(pPlayer->GetUserID(),6),
		pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}
void SRLFGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;//设置玩法局数
}
int SRLFGameScence::getPlayCount()
{
	return m_nPlayCount;
}
//扣税收显示(仅首局显示)
void SRLFGameScence::showGameTax(int iGameTax)
{
	if(iGameTax==0)
		return;
	std::string kStr = utility::toString(utility::a_u8("本局服务费"),iGameTax);
	WidgetFun::setText(this,"GameTax",kStr);
	WidgetFun::runWidgetAction(this,"GameTax","Start");
}

void SRLFGameScence::updataPlayerSeat()
{
	for (int i=0;i<MAX_PLAYER;i++)
	{
		auto pPlayer= getPlayerByChairID(i);
		if ( NULL == pPlayer ) continue;
			pPlayer->getValid() ? pPlayer->show():pPlayer->hide();
	}
	if (m_pLocal) m_pLocal->show();
}