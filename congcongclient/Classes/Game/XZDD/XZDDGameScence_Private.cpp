#include "XZDDGameScence.h"
#include "GAME.h"
#include "XZDDPlayer.h"
#include "Game/Game/MissionWeiXin.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_XZDD.h"
#include UTILITY_CONVERT
#include IMAGE_DOWN

void XZDDGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("SCMJRule",(int)nRuleTag)).Value());
	}
}
void XZDDGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameXZDD/xml/HNGamePrivateScence.xml",this);

	////WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(XZDDGameScence::XZDDButton_WeiXinFriend));
	////WidgetManager::addButtonCB("Button_PrivateEndWeiXinShare",this,button_selector(XZDDGameScence::XZDDButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(XZDDGameScence::ButtonPlayerHeadClick));
}
void XZDDGameScence::defaultPrivateState()//�رշ���
{
	if (WidgetFun::getChildWidget(this,"PrivateDismissPlane"))
	{
		WidgetFun::setVisible(this,"PrivateDismissPlane",false);
		////WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",false);
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
void XZDDGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)//���ܵ�������Ϣ
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
	WidgetFun::setVisible(this,"MenuNode",false);
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
	////WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",!pNetInfo->bStartGame 
		////&& pNetInfo->dwCreateUserID == m_pLocal->GetUserID());
	////WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);

	for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
	{
		XZDDPlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		WidgetFun::setText(pPlayer->getPlayerNode(),
			"GoldTxt",pNetInfo->kWinLoseScore[i]);
	}


}

void XZDDGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)//���÷���˽����Ϣ
{
 	m_kPrivateRoomInfo = *pNetInfo;
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));//���÷����
	std::vector<std::string> vRuleNam;
	std::string temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_))?"Ѫս������":"Ѫս�齫";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHU_))?"���ֺ�":"��С����";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_ZIMO_))?"��ܻ�(����)":"��ܻ�(����)";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIADI_))?"�����ӵ�":"�����ӷ�";
	if(temp != "")
	vRuleNam.push_back(temp);
	//ruleNam += FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIAFAN_))?"�����ӷ�\n":"";
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HAI_DI_LAO_))?"������":"";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_MEN_Q_))?"����":"";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZHONG_ZHANG_))?"����":"";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_YI_JIU_))?"�۾�":"";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_Jiang_dui_))?"����":"";
	if(temp != "")
	vRuleNam.push_back(temp);
	temp = "4��";
	vRuleNam.push_back(temp);
	temp = utility::toString("��ע��", pNetInfo->dwBaseScore,"\n");
	vRuleNam.push_back(temp);
	std::string result("");
	//int line = 0;
	for (auto itor =vRuleNam.begin();itor !=vRuleNam.end();itor++)
	{
		//if (((result.length()+ itor->length())/25 > line))
		//{
		//	line += 1;
		//	result+= *itor+"\n";
		//}else{
			result= (result !="") ? result+","+ *itor:result + *itor;
		//}
	}
	//temp = utility::toString("��ע��", pNetInfo->dwBaseScore,"\n");
	//result= result+temp;
	WidgetFun::setText(this, "GameRoomType", utility::a_u8(result));
}

void XZDDGameScence::friendPrivateInfo()//���÷���˽����Ϣ
{
	//m_kPrivateRoomInfo = *pNetInfo;
	std::string resultRule ="";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_))?"Ѫս������":"Ѫս�齫";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHU_))?",���ֺ�":",��С����";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_ZIMO_))?",��ܻ�(����)":",��ܻ�(����)";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIADI_))?",�����ӵ�":",�����ӷ�";
	//ruleNam += FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIAFAN_))?"�����ӷ�\n":"";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HAI_DI_LAO_))?",������":"";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_MEN_Q_))?",����":"";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZHONG_ZHANG_))?",����":"";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_YI_JIU_))?",�۾�":"";
	resultRule +=FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_Jiang_dui_))?",����":"";
	resultRule +=",4��";
	resultRule +=utility::toString(",��ע��", m_kPrivateRoomInfo.dwBaseScore);
	std::string resultTitle =FvMask::HasAll(m_kPrivateRoomInfo.dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_))?"Ѫս������":"Ѫս�齫";
	std::string kCCWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
	utility::a_u8(resultTitle),(int)m_kPrivateRoomInfo.dwRoomNum,utility::a_u8(resultRule));
	MissionWeiXin::Instance().shareTextWeiXin(kCCWeiXinSharDes,MissionWeiXin::SHARE_SESSION);
	//MissionWeiXin::Instance().shareUrlWeiXin(script::getStr("CCWeiXinSharUrl"),
	//script::getStr("CCWeiXinSharTitle"),
	//kCCWeiXinSharDes,
	//MissionWeiXin::SHARE_SESSION);
}

void XZDDGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
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
		WidgetFun::setVisible(this,"PrivateEndPlane",true);
	}
	else
	{
		int iTime = utility::parseInt(WidgetFun::getWidgetUserInfo(pJieSuanNode,"Time"))+3;
		WidgetFun::getChildWidget(pRootNode,"PrivateEndPlane")->runAction(script::CCVisibleAction::create(iTime,true));
	}

	if(HaveOptionRule(GAME_OPTION_RULE_PRIVATAEND_RETURN_HOME))
	{
		WidgetFun::setVisible(pRootNode,"Button_PrivateReturnHome",true);
		WidgetFun::setVisible(pRootNode,"Button_PrivateAgagin",false);
	}
}

void XZDDGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)//��ɢ��Ϣ
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	pNode->setVisible(true);
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(script::CCVisibleAction::create(1.0f,false));
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

void XZDDGameScence::XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)//Ҫ��΢�ź�����Ϣ
{
	friendPrivateInfo();
	/*kHNWeiXinSharDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
	(int)m_kPrivateRoomInfo.dwRoomNum,(int)m_kPrivateRoomInfo.dwPlayTotal,kGameTypeStr);
	MissionWeiXin::Instance().shareUrlWeiXin(script::getStr("CCWeiXinSharUrl"),
	script::getStr("CCWeiXinSharTitle"),
	kHNWeiXinSharDes,
	MissionWeiXin::SHARE_SESSION);*/
}

void XZDDGameScence::XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static XZDDGameScence* pTemp = NULL;
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

void XZDDGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)//���ͷ��ť
{
	GamePlayer* pPlayer = getPlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),utility::paseInt(pPlayer->GetUserID(),6),pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}

void XZDDGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;//�����淨����
}

int XZDDGameScence::getPlayCount()
{
	return m_nPlayCount;
}