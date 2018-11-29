#include "WZQDGameScence.h"
#include "Game/GameLib.h"
#include "WZQDPlayer.h"
#include "Game/Game/MissionWeiXin.h"

void WZQDGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("SCMJRule",(int)nRuleTag)).Value());
	}
}
void WZQDGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameXZDD/xml/HNGamePrivateScence.xml",this);

	WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(WZQDGameScence::XZDDButton_WeiXinFriend));
	WidgetManager::addButtonCB("Button_PrivateEndWeiXinShare",this,button_selector(WZQDGameScence::XZDDButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(WZQDGameScence::ButtonPlayerHeadClick));
}
void WZQDGameScence::defaultPrivateState()//关闭房间
{
	//if (WidgetFun::getChildWidget(this,"PrivateDismissPlane"))
	//{
	//	//WidgetFun::setVisible(this,"PrivateDismissPlane",false);
	//	//WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",false);
	//	//WidgetFun::setVisible(this,"TalkImagic",false);
	//}
	//if (WidgetFun::getChildWidget(this,"PrivateEndPlane"))
	//{
	//	WidgetFun::setVisible(this,"PrivateEndPlane",false);
	//}
	//if (WidgetFun::getChildWidget(this,"GameJieSuanNode"))
	//{
	//	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	//}
}
void WZQDGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)//接受到房间信息
{
	//WidgetFun::setVisible(this,"PrivateInfo",true);
	//WidgetFun::setVisible(this,"MenuNode",false);
	setPrivateInfo(pNetInfo);
	setPlayCount(pNetInfo->dwPlayCout);
	bool bShowDismiss = false;
	if (pNetInfo->dwCreateUserID == m_pLocal->GetUserID())
	{
		if (pNetInfo->bStartGame || pNetInfo->dwPlayCout == 0)
		{
			bShowDismiss = true;
		}
	}
	if (pNetInfo->bStartGame)
	{
		bShowDismiss = true;
	}
	//WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",!pNetInfo->bStartGame 
	//	&& pNetInfo->dwCreateUserID == m_pLocal->GetUserID());
	//WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);

	for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
	{
		WZQDPlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		WidgetFun::setText(pPlayer->getPlayerNode(),
			"GoldTxt",(int)pNetInfo->kWinLoseScore[i]);
	}
}

void WZQDGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)//设置房间私有信息
{
 	m_kPrivateRoomInfo = *pNetInfo;
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));//设置房间号
	//if (pNetInfo->bPlayCoutIdex == 0)
	//{
	//	WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));//游戏局数和游戏总局数
	//}
	//else
	//{
	//	WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));
	//}
	WidgetFun::setVisible(this,"RoomNum",false);

	//std::vector<std::string> kRuleList;
	//pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_XZ,true);//判断并推入相应的游戏规则
	//pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_XL,true);

	for (int i=0;i<MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),false);
	}
	/*for (int i=0;i<(int)kRuleList.size() && i < MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),true);
		WidgetFun::setText(this,utility::toString("RoomGameRule",i),kRuleList[i]);
	}*/
}

void WZQDGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != MAX_PLAYER
		||pNetInfo->lPlayerAction.size() != MAX_PLAYER*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != MAX_PLAYER");
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
void WZQDGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)//解散信息
{
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
void WZQDGameScence::XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)//要请微信好友信息
{
	return;
	std::string kHNWeiXinSharDes,kGameTypeStr;

	if (FvMask::HasAny(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GAME_RULE_XZ)))
	{
		kGameTypeStr = ScriptData<std::string>("SCMJRule2").Value();
	}
	if (FvMask::HasAny(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GAME_RULE_XL)))
	{
		kGameTypeStr = ScriptData<std::string>("SCMJRule3").Value();
	}
	kHNWeiXinSharDes = utility::getScriptReplaceValue("SCWeiXinSharDes",
		(int)m_kPrivateRoomInfo.dwRoomNum,(int)m_kPrivateRoomInfo.dwPlayTotal,kGameTypeStr);
	MissionWeiXin::Instance().shareTextWeiXin(kHNWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
	//MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("WeiXinSharUrl"),
	//	utility::getScriptString("SCWeiXinSharTitle"),
	//	kHNWeiXinSharDes,
	//	MissionWeiXin::SHARE_SESSION);
}
void WZQDGameScence::XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static WZQDGameScence* pTemp = NULL;
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
void WZQDGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)//点击头像按钮
{
	GamePlayer* pPlayer = getPlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),utility::paseInt(pPlayer->GetUserID(),6),pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}
void WZQDGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;//设置玩法局数
}
int WZQDGameScence::getPlayCount()
{
	return m_nPlayCount;
}

void WZQDGameScence::updateScore(BYTE cbIndex,dword wScore)
{
	WidgetFun::setText(this,"Txt_ShuRuScore",utility::a_u8(utility::toString("学费：",wScore)));
	WZQ::CMD_C_SetStudyPay studyPay;
	studyPay.lStudyPay = wScore;
	SendSocketData(SUB_C_SET_XUEFEI,&studyPay,sizeof(studyPay));
}