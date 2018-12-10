#include "GPHomeCreateRoomPanel_SRLF.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "Game/SRLF/SRLFGameScence.h"
#include "Game/SRLF/CMD_SRLF.h"
#include "kernel/kernel/server/CServerItem.h"
#include "Plaza/HomeScene/GPHomeEnterRoomPanel.h"
#include "Plaza/HomeScene/GPHomeEditNumPanel.h"

 

FV_SINGLETON_STORAGE(GPHomeCreateRoomPanel_SRLF);
GPHomeCreateRoomPanel_SRLF::GPHomeCreateRoomPanel_SRLF()
	:m_CurrentModeIndex(0)
	, m_CurrentRuleIndex(0)
	, m_CurrentScore(5)
	, m_CurrentGameCount(8)

	,  m_GameRuleJoinMoShenRen(true)	 //允许陌生人加入	
	,  m_GameRuleCardsNum(0)		 //牌张数		
	,  m_GameRuleHuPaiWanFa(0)		 //胡牌
	,  m_GameRuleDianGangHua(0)		 //点杠花
	,  m_GameRuleFanShuWanFa(0)		 //番数目
	,  m_GameRulePlayerNum(0)		 //人数
	,  m_GameRuleFanMaxNum(0)		 //番数最大值
	,  m_GameRuleDiZhuNum(0)		 //底注
	,  m_GameRuleLiChangNum(0)		 //离场设定
	,  m_GameRuleJinChangNum(0)		 //进场设定


{
	init();
}

GPHomeCreateRoomPanel_SRLF::~GPHomeCreateRoomPanel_SRLF()
{
}

bool GPHomeCreateRoomPanel_SRLF::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	WidgetScenceXMLparse createRoomPanel_SRLF("GamePlaza/Script/GPHomeCreateRoomPanel_SRLF.xml", this);
	initButton();
	initListView();
	initData();
	//WidgetFun::setVisible(this,"JiaPiao_CB",false);
	//WidgetFun::setVisible(this,"ZiMoJiaDi_CB",false);
	//WidgetFun::setVisible(this,"ZiMoJiaFan_CB",false);
	//WidgetFun::setVisible(this,"HaiDiLao_CB",false);
	//WidgetFun::setVisible(this,"HaiDiPao_CB",false);
	WidgetFun::setVisible(this,"SanFan_CB",false);
	WidgetFun::setVisible(this,"LiangFan_CB",false);
	return true;
}

void GPHomeCreateRoomPanel_SRLF::initData()
{
	FvMask::Add(m_GameRuleJoinMoShenRen, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_YXMSRJOIN));//允许陌生人加入	
	WidgetFun::setChecked(this,"FangJianSeDing_CB",true);

	FvMask::Add(m_GameRuleCardsNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_14CARDS)); //牌张数
	FvMask::Del(m_GameRuleCardsNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_8CARDS));
	WidgetFun::setChecked(this,"ShiSiZhang_CB",true);
	WidgetFun::setChecked(this,"BaZhang_CB",false);

	FvMask::Add(m_GameRuleHuPaiWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_XZDD)); //胡牌	
	FvMask::Del(m_GameRuleHuPaiWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_TDH));
	WidgetFun::setChecked(this,"TuiDaoHu_CB",false);
	WidgetFun::setChecked(this,"XueZanDaoDi_CB",true);

	FvMask::Add(m_GameRuleDianGangHua, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_DP));//点杠花
	FvMask::Del(m_GameRuleDianGangHua, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_ZM));
	WidgetFun::setChecked(this,"DianGangHuaDianPao_CB",true);
	WidgetFun::setChecked(this,"DianGangHuaZhiMo_CB",false);

	FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_GFXY)); //番数目
	WidgetFun::setChecked(this,"GuaFengXiaYu_CB",true);
	FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_JP));
	WidgetFun::setChecked(this,"JiaPiao_CB",true);

	FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIADI));
	FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIAFAN));
	WidgetFun::setChecked(this,"ZiMoJiaDi_CB",true);
	WidgetFun::setChecked(this,"ZiMoJiaFan_CB",false);

	FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_LAO));
	FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_PAO));
	WidgetFun::setChecked(this,"HaiDiLao_CB",true);
	//WidgetFun::setChecked(this,"HaiDiPao_CB",false);

	FvMask::Add(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN)); //人数
	FvMask::Del(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN));
	WidgetFun::setChecked(this,"SanRen_CB",true);
	WidgetFun::setChecked(this,"LiangRen_CB",false);
	
	FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_FAN));//番数最大值
	FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_FAN));
	FvMask::Add(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_4_FAN));
	WidgetFun::setChecked(this,"LiangFan_CB",false);
	WidgetFun::setChecked(this,"SanFan_CB",false);
	WidgetFun::setChecked(this,"SiFan_CB",true);

	m_GameRuleDiZhuNum=50;							 //底注
	m_GameRuleLiChangNum=m_GameRuleDiZhuNum*30;		 //离场设定
	m_GameRuleJinChangNum=m_GameRuleDiZhuNum*40;	 //进场设定
	WidgetFun::setText(this,"SRLF_CreateRoom_OutNum_Txt",m_GameRuleLiChangNum);
	WidgetFun::setText(this,"SRLF_CreateRoom_GameScore_Txt",m_GameRuleDiZhuNum);
	WidgetFun::setText(this,"SRLF_CreateRoom_EnterNum_Txt",m_GameRuleJinChangNum);
}


void GPHomeCreateRoomPanel_SRLF::initListView()
{
	widget::ListViewEx* pGamePlayerListView = WidgetFun::getListViewWidget(this,"GamePlayerListView");
	pGamePlayerListView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	pGamePlayerListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	pGamePlayerListView->setGravity(cocos2d::ui::ListView::Gravity::BOTTOM);
	//pGamePlayerListView->setItemsMargin(41.0f);
	pGamePlayerListView->addEventListener((widget::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeCreateRoomPanel_SRLF::onSelectedItemEvent,this));
	pGamePlayerListView->refreshView();
	pGamePlayerListView->jumpToPercentVertical(0);
}

//UIListView适配函数
void  GPHomeCreateRoomPanel_SRLF::onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
		{
			cocos2d::ui::ListView*  listview = static_cast<cocos2d::ui::ListView* >(pSender);
			cocos2d::log("ON_SELECTED_ITEM_START:%ld",listview->getCurSelectedIndex());
			break;
		}
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
		{
			cocos2d::ui::ListView* listview = static_cast<cocos2d::ui::ListView*>(pSender);
			//if (m_selectedItemIndex == listview->getCurSelectedIndex())
			cocos2d::log("ON_SELECTED_ITEM_END:%ld",listview->getCurSelectedIndex());
			long int index = listview->getCurSelectedIndex();
		}
	default:
		break;
	}
}


void GPHomeCreateRoomPanel_SRLF::initButton()
{
	WidgetManager::addButtonCB("Btn_Close", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_Close));
	WidgetManager::addButtonCB("Btn_RuleCreateRoom", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_Confirm));
	
	WidgetManager::addButtonCB("FangJianSeDing_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleJoinMoShenRen));

	WidgetManager::addButtonCB("ShiSiZhang_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleCardsNum));
	WidgetManager::addButtonCB("BaZhang_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleCardsNum));

	WidgetManager::addButtonCB("XueZanDaoDi_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleHuPaiWanFa));
	WidgetManager::addButtonCB("TuiDaoHu_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleHuPaiWanFa));
	
	WidgetManager::addButtonCB("DianGangHuaDianPao_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleDianGangHua));
	WidgetManager::addButtonCB("DianGangHuaZhiMo_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleDianGangHua));

	WidgetManager::addButtonCB("GuaFengXiaYu_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleFanShuWanFa));
	WidgetManager::addButtonCB("JiaPiao_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleFanShuWanFa));
	
	WidgetManager::addButtonCB("SanRen_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRulePlayerNum));
	WidgetManager::addButtonCB("LiangRen_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRulePlayerNum));

	WidgetManager::addButtonCB("LiangFan_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleFanMaxNum));
	WidgetManager::addButtonCB("SanFan_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleFanMaxNum));
	WidgetManager::addButtonCB("SiFan_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleFanMaxNum));

	WidgetManager::addButtonCB("ZiMoJiaDi_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleZiMoJiaScore));
	WidgetManager::addButtonCB("ZiMoJiaFan_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleZiMoJiaScore));

	WidgetManager::addButtonCB("HaiDiLao_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleHaiDiScore));
	WidgetManager::addButtonCB("HaiDiPao_CB", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleHaiDiScore));
	WidgetFun::setVisible(this,"HaiDiPao_CB",false);

	WidgetManager::addButtonCB("Btn_SRLF_CreateRoom_GameScore", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleDiZhuNum));
	WidgetManager::addButtonCB("Btn_SRLF_EnterNum_GameScore", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleJinChangNum));
	//WidgetManager::addButtonCB("Btn_CreateRoom_OutNum", this, button_selector(GPHomeCreateRoomPanel_SRLF::Button_GameRuleLiChangNum));

	//DiZhuText 
	//LiChangSeDingText
	//RuChangSeDingText
}

void GPHomeCreateRoomPanel_SRLF::changeGameTypeBtnTexture(std::string target, std::string value)
{
	auto _cNode = WidgetFun::getChildWidget(this, target);
	std::string _Texture = WidgetFun::getWidgetUserInfo(_cNode, value);
	WidgetFun::setButtonImagic(_cNode, _Texture, false);
}

void GPHomeCreateRoomPanel_SRLF::show()
{
	setVisible(true);
}

void GPHomeCreateRoomPanel_SRLF::hide()
{
	setVisible(false);
}

void GPHomeCreateRoomPanel_SRLF::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}



//允许陌生人加入;
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleJoinMoShenRen(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	//允许陌生人加入	
	if(WidgetFun::isChecked(this,"FangJianSeDing_CB") == true){ 
		FvMask::Add(m_GameRuleJoinMoShenRen, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_YXMSRJOIN));
		WidgetFun::setChecked(this,"FangJianSeDing_CB",true);
	}else{
		FvMask::Del(m_GameRuleJoinMoShenRen, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_YXMSRJOIN));
		WidgetFun::setChecked(this,"FangJianSeDing_CB",false);
	}
}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleCardsNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	//牌张数
	if (pInfo->kName == "ShiSiZhang_CB")
	{
		FvMask::Add(m_GameRuleCardsNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_14CARDS));
		FvMask::Del(m_GameRuleCardsNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_8CARDS));
		WidgetFun::setChecked(this,"ShiSiZhang_CB",true);
		WidgetFun::setChecked(this,"BaZhang_CB",false);
	}
	if(pInfo->kName == "BaZhang_CB"){
		FvMask::Del(m_GameRuleCardsNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_14CARDS));
		FvMask::Add(m_GameRuleCardsNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_8CARDS));
		WidgetFun::setChecked(this,"ShiSiZhang_CB",false);
		WidgetFun::setChecked(this,"BaZhang_CB",true);
	}
	
}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleHuPaiWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	if (pInfo->kName == "TuiDaoHu_CB")
	{
		FvMask::Add(m_GameRuleHuPaiWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_TDH));
		FvMask::Del(m_GameRuleHuPaiWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_XZDD));
		WidgetFun::setChecked(this,"TuiDaoHu_CB",true);
		WidgetFun::setChecked(this,"XueZanDaoDi_CB",false);
	}
	if(pInfo->kName == "XueZanDaoDi_CB"){
		FvMask::Del(m_GameRuleHuPaiWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_TDH));
		FvMask::Add(m_GameRuleHuPaiWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_XZDD));
		WidgetFun::setChecked(this,"TuiDaoHu_CB",false);
		WidgetFun::setChecked(this,"XueZanDaoDi_CB",true);
	}

}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleDianGangHua(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	if (pInfo->kName == "DianGangHuaDianPao_CB")
	{
		FvMask::Add(m_GameRuleDianGangHua, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_DP));
		FvMask::Del(m_GameRuleDianGangHua, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_ZM));
		WidgetFun::setChecked(this,"DianGangHuaDianPao_CB",true);
		WidgetFun::setChecked(this,"DianGangHuaZhiMo_CB",false);
	}
	if(pInfo->kName == "DianGangHuaZhiMo_CB"){
		FvMask::Del(m_GameRuleDianGangHua, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_DP));
		FvMask::Add(m_GameRuleDianGangHua, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_DGH_ZM));
		WidgetFun::setChecked(this,"DianGangHuaDianPao_CB",false);
		WidgetFun::setChecked(this,"DianGangHuaZhiMo_CB",true);
	}

}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleFanShuWanFa(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	if (pInfo->kName == "GuaFengXiaYu_CB")
	{
		if(WidgetFun::isChecked(this,"GuaFengXiaYu_CB") == false)
			FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_GFXY));
		else
			FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_GFXY));
	}
	if(pInfo->kName == "JiaPiao_CB"){
		if(WidgetFun::isChecked(this,"JiaPiao_CB") == false)
			FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_JP));
		else
			FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_JP));
	}

}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRulePlayerNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	if (pInfo->kName == "SanRen_CB")
	{
		FvMask::Add(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN));
		FvMask::Del(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN));
		WidgetFun::setChecked(this,"SanRen_CB",true);
		WidgetFun::setChecked(this,"LiangRen_CB",false);
	}
	if(pInfo->kName == "LiangRen_CB"){
		FvMask::Del(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN));
		FvMask::Add(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN));
		WidgetFun::setChecked(this,"SanRen_CB",false);
		WidgetFun::setChecked(this,"LiangRen_CB",true);
	}
}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleFanMaxNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	if (pInfo->kName == "LiangFan_CB")
	{
		FvMask::Add(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_FAN));
		FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_FAN));
		FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_4_FAN));
		WidgetFun::setChecked(this,"LiangFan_CB",true);
		WidgetFun::setChecked(this,"SanFan_CB",false);
		WidgetFun::setChecked(this,"SiFan_CB",false);
	}
	if(pInfo->kName == "SanFan_CB"){
		FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_FAN));
		FvMask::Add(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_FAN));
		FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_4_FAN));
		WidgetFun::setChecked(this,"LiangFan_CB",false);
		WidgetFun::setChecked(this,"SanFan_CB",true);
		WidgetFun::setChecked(this,"SiFan_CB",false);
	}
	if(pInfo->kName == "SiFan_CB"){
		FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_FAN));
		FvMask::Del(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_FAN));
		FvMask::Add(m_GameRuleFanMaxNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_4_FAN));
		WidgetFun::setChecked(this,"LiangFan_CB",false);
		WidgetFun::setChecked(this,"SanFan_CB",false);
		WidgetFun::setChecked(this,"SiFan_CB",true);
	}
}

void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleZiMoJiaScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	if (pInfo->kName == "ZiMoJiaDi_CB")
	{
		FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIADI));
		FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIAFAN));
		WidgetFun::setChecked(this,"ZiMoJiaDi_CB",true);
		WidgetFun::setChecked(this,"ZiMoJiaFan_CB",false);
	}
	if(pInfo->kName == "ZiMoJiaFan_CB"){
		FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIADI));
		FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_ZIMO_JIAFAN));
		WidgetFun::setChecked(this,"ZiMoJiaDi_CB",false);
		WidgetFun::setChecked(this,"ZiMoJiaFan_CB",true);
	}
}

void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleHaiDiScore(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	if (pInfo->kName == "HaiDiLao_CB")
	{
		//FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_LAO));
		//FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_PAO));
		//WidgetFun::setChecked(this,"HaiDiLao_CB",true);
		//WidgetFun::setChecked(this,"HaiDiPao_CB",false);

		if(WidgetFun::isChecked(this,"HaiDiLao_CB") == false)
		{
			FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_LAO));
			FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_PAO));
		}else{
			FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_LAO));
			FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_PAO));
		}
	}
	/*if(pInfo->kName == "HaiDiPao_CB"){
		FvMask::Del(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_LAO));
		FvMask::Add(m_GameRuleFanShuWanFa, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_HAIDI_PAO));
		WidgetFun::setChecked(this,"HaiDiLao_CB",false);
		WidgetFun::setChecked(this,"HaiDiPao_CB",true);
	}*/
}

void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleDiZhuNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeEditNumPanel::Instance().show("SRLF",1);
}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleLiChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeEditNumPanel::Instance().show("SRLF",3);
}
void  GPHomeCreateRoomPanel_SRLF::Button_GameRuleJinChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeEditNumPanel::Instance().show("SRLF",2);
}

void GPHomeCreateRoomPanel_SRLF::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet, sizeof(kSendNet));
	kSendNet.cbGameType = Type_Private;
	kSendNet.dwGameRuleIdex = 0;
	FvMask::Add(kSendNet.dwGameRuleIdex, m_GameRuleJoinMoShenRen );//允许陌生人加入	
	FvMask::Add(kSendNet.dwGameRuleIdex, m_GameRuleCardsNum );//牌张数	
	FvMask::Add(kSendNet.dwGameRuleIdex, m_GameRuleHuPaiWanFa );//胡牌	
	FvMask::Add(kSendNet.dwGameRuleIdex, m_GameRuleDianGangHua );//点杠花	
	FvMask::Add(kSendNet.dwGameRuleIdex, m_GameRuleFanShuWanFa );//番数目	
	FvMask::Add(kSendNet.dwGameRuleIdex, m_GameRulePlayerNum );//人数	
	FvMask::Add(kSendNet.dwGameRuleIdex, m_GameRuleFanMaxNum );//番数最大值
	kSendNet.bAllowedStrangerJoin = FvMask::HasAny(m_GameRuleJoinMoShenRen,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_YXMSRJOIN));//是否允许陌生人加入
	//kSendNet.bPassionationMode = false;//激情模式开关
	//kSendNet.bBloodFightMode =FvMask::HasAny(m_GameRuleHuPaiWanFa,_MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_XZDD));//血战到底开关
	//char							stHttpChannel[LEN_NICKNAME];			//http获取
	kSendNet.dwBaseScore = m_GameRuleDiZhuNum;  //底注
	kSendNet.dwEnterMatchNum = m_GameRuleJinChangNum;  //入场限制
	kSendNet.dwOutMatchNum = m_GameRuleLiChangNum;  //离场限制
	kSendNet.bGameTypeIdex = GAME_TYPE_SICHUAN;
	kSendNet.bPlayCoutIdex =0;/*(m_CurrentGameCount - 4) / 4*/
	if(FvMask::HasAny(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_3_REN)))
		kSendNet.wCharNum = 3;
	else if(FvMask::HasAny(m_GameRulePlayerNum, _MASK_(GPHomeCreateRoomPanel_SRLF::GAME_RULE_2_REN)))
		kSendNet.wCharNum = 2;
	//SRLFGameScence::MAX_PLAYER =kSendNet.wCharNum;
	GPGameLink::Instance().CreateRoom(SRLFGameScence::KIND_ID, kSendNet);
}


void GPHomeCreateRoomPanel_SRLF::updateScore(byte cbIndex,dword wScore)
{
	switch (cbIndex)
	{
	case 1:
		{
			m_GameRuleDiZhuNum =wScore;
			m_GameRuleJinChangNum =wScore*40;
			m_GameRuleLiChangNum =wScore*30;
			WidgetFun::setText(this,"SRLF_CreateRoom_GameScore_Txt",wScore);
			WidgetFun::setText(this,"SRLF_CreateRoom_EnterNum_Txt",wScore*40);
			WidgetFun::setText(this,"SRLF_CreateRoom_OutNum_Txt",wScore*30);
		}
		break;
	case 2:
		{
			m_GameRuleJinChangNum =wScore;
			WidgetFun::setText(this,"SRLF_CreateRoom_EnterNum_Txt",wScore);
		}
		break;
	case 3:
		{
			m_GameRuleLiChangNum =wScore;
			WidgetFun::setText(this,"SRLF_CreateRoom_OutNum_Txt",wScore);
		}
		break;
	default:
		break;
	}
}





















