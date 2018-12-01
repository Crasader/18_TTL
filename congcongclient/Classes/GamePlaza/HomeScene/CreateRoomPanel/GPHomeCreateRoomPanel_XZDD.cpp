
#include "GPHomeCreateRoomPanel_XZDD.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/XZDD/CMD_XZDD.h"
#include "ClientHN_THJ/Game/XZDD/XZDDGameScence.h"
#include "GamePlaza/HomeScene/EditNumPanel/GPHomeEditNumPanel.h"
#include "GameLib/Game/FV/FvMask.h"

USING_NS_CC;
FV_SINGLETON_STORAGE(GPHomeCreateRoomPanel_XZDD);

GPHomeCreateRoomPanel_XZDD::GPHomeCreateRoomPanel_XZDD()			  
{
	init();
}

GPHomeCreateRoomPanel_XZDD::~GPHomeCreateRoomPanel_XZDD()
{

}

bool GPHomeCreateRoomPanel_XZDD::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	updateScore(1,50);
	initButton();
	initData();
	return true;
}
void GPHomeCreateRoomPanel_XZDD::initListView()
{
	cocos2d::ListViewEx* pGamePlayerListView = WidgetFun::getListViewWidget(this,"GamePlayerListView_XZDD");
	//pGamePlayerListView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	//pGamePlayerListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	//pGamePlayerListView->setGravity(ui::ListView::Gravity::BOTTOM);
	//pGamePlayerListView->setItemsMargin(41.0f);
	//pGamePlayerListView->addEventListener((cocos2d::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeCreateRoomPanel_SRLF::onSelectedItemEvent,this));
	pGamePlayerListView->refreshView();
	pGamePlayerListView->jumpToPercentVertical(0);
}


void GPHomeCreateRoomPanel_XZDD::initLayout()
{
	WidgetScenceXMLparse createRoom_XZDD("GamePlaza/Script/GPHomeCreateRoomPanel_XZDD.xml", this);
}

void GPHomeCreateRoomPanel_XZDD::initButton()
{
	WidgetManager::addButtonCB("Btn_Close_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_Close));
	WidgetManager::addButtonCB("Btn_RuleCreateRoom_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_CreateRoom_XZDD));
	//�������
	WidgetManager::addButtonCB("AllowStrangerJoin_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	
	WidgetManager::addButtonCB("GuoShouHu_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("GouXiaoHuDa_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	
	WidgetManager::addButtonCB("ZiMoJiaDi_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("ZiMoJiaFan_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	
	WidgetManager::addButtonCB("HaiDiLao_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("HuanSanZhang_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));

	WidgetManager::addButtonCB("YaoJiu_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("JiangDui_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));


	WidgetManager::addButtonCB("MenQing_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("ZhongZhang_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	
	WidgetManager::addButtonCB("DianGangHua_DianPao_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("DianGangHua_ZiMo_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));


	WidgetManager::addButtonCB("SiFan_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("SanFan_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));
	WidgetManager::addButtonCB("LiangFan_CB_XZDD", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing));

	WidgetManager::addButtonCB("Btn_XZDD_CreateRoom_GameScore", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleDiZhuNum));
	WidgetManager::addButtonCB("Btn_XZDD_EnterNum_GameScore", this, button_selector(GPHomeCreateRoomPanel_XZDD::Button_GameRuleJinChangNum));
}

void GPHomeCreateRoomPanel_XZDD::show()
{
	m_GameRuleDiZhuNum=50;							 //��ע
	m_GameRuleLiChangNum=m_GameRuleDiZhuNum*70;		 //�볡�趨
	m_GameRuleJinChangNum=m_GameRuleDiZhuNum*80;	 //�����趨
	updateScore(1,50);
	setVisible(true);
}

void GPHomeCreateRoomPanel_XZDD::hide()
{
	setVisible(false);
}

void GPHomeCreateRoomPanel_XZDD::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

//����ʼ��
void GPHomeCreateRoomPanel_XZDD::initData()
{
	m_CurrentModeIndex = 0;
	m_CurrentRuleIndex = 0;
	m_CurrentScore = 5;
	m_CurrentGameCount = 0;
	m_GameRuleFanShuWanFa = 0;		 //����Ŀ
	m_GameRuleFanMaxNum = 0;		 //�������ֵ
	m_GameRuleDiZhuNum = 0;			 //��ע
	m_GameRuleLiChangNum = 0;		 //�볡�趨
	m_GameRuleJinChangNum = 0;		 //�����趨

	m_GameRuleJoinMoShenRen = 0;	//����İ���˼���	
	m_GameYouXiDianGangHua = 0;		//��ܻ�����/����
	m_GameYouXiSheDing = 0;			 //��Ϸ�趨/�¼�
	m_GameFangJian = 0;			//Ԫ������/�¼�
	m_GameFangJianSeDing = 0;		//�¼�
	initGameRuleSheDing();
}

void GPHomeCreateRoomPanel_XZDD::initGameRuleSheDing()
{
	//����İ���˼���
	m_GameRuleJoinMoShenRen = true;
	WidgetFun::setChecked(this,"AllowStrangerJoin_CB_XZDD", true);
	
	//���ֺ� ���� ��С����
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHU_));
	FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHUDA_));
	WidgetFun::setChecked(this, "GuoShouHu_CB_XZDD", true);
	WidgetFun::setChecked(this, "GouXiaoHuDa_CB_XZDD", false);
	
	//�����ӵ� ���� �����ӷ�
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIADI_));
	FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIAFAN_));
	WidgetFun::setChecked(this, "ZiMoJiaDi_CB_XZDD", true);
	WidgetFun::setChecked(this, "ZiMoJiaFan_CB_XZDD", false);

	//������
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HAI_DI_LAO_));
	WidgetFun::setChecked(this, "HaiDiLao_CB_XZDD", true);

	//������
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_));
	WidgetFun::setChecked(this, "HuanSanZhang_CB_XZDD", true);

	//�۾�
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_YI_JIU_));
	WidgetFun::setChecked(this, "YaoJiu_CB_XZDD", true);

	//�Խ�
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_Jiang_dui_));
	WidgetFun::setChecked(this, "JiangDui_CB_XZDD", true);
	
	//����	
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_MEN_Q_));
	WidgetFun::setChecked(this, "MenQing_CB_XZDD", true);

	//����
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZHONG_ZHANG_));
	WidgetFun::setChecked(this, "ZhongZhang_CB_XZDD", true);


	//��ܻ�(����)
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_ZIMO_));
	FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_DIANPAO_));
	WidgetFun::setChecked(this, "DianGangHua_ZiMo_CB_XZDD", true);
	WidgetFun::setChecked(this, "DianGangHua_DianPao_CB_XZDD", false);

	//4�� �� 3�� �� 2��
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN0_));
	FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN1_));
	FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN2_));
	WidgetFun::setChecked(this, "SiFan_CB_XZDD", true);
	WidgetFun::setChecked(this, "SanFan_CB_XZDD", false);
	WidgetFun::setChecked(this, "LiangFan_CB_XZDD", false);

	//����	
	//if (WidgetFun::isChecked(this, "ChaDaJiao_XZDD") == true)
	//{
	//	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_CHADAJIAO));
	//	WidgetFun::setChecked(this, "ChaDaJiao_XZDD", true);
	//}
	//else
	//{
	//	FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_CHADAJIAO));
	//	WidgetFun::setChecked(this, "ChaDaJiao_XZDD", false);
	//}
}

 //��Ϸ�趨
void GPHomeCreateRoomPanel_XZDD::Button_GameRuleSheDing(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	//����İ���˼���
	if (WidgetFun::isChecked(this, "AllowStrangerJoin_CB_XZDD") == true)
	{
		m_GameRuleJoinMoShenRen = true;
	}
	else
	{
		m_GameRuleJoinMoShenRen = false;
	}

	//���ֺ� ���� ��С����
	if(pInfo->kName =="GuoShouHu_CB_XZDD" || pInfo->kName =="GouXiaoHuDa_CB_XZDD" )
	{
		if (pInfo->kName =="GuoShouHu_CB_XZDD")
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHU_));
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHUDA_));
			WidgetFun::setChecked(this, "GuoShouHu_CB_XZDD", true);
			WidgetFun::setChecked(this, "GouXiaoHuDa_CB_XZDD", false);
		}else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHU_));
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_GUOSHOUHUDA_));
			WidgetFun::setChecked(this, "GuoShouHu_CB_XZDD", false);
			WidgetFun::setChecked(this, "GouXiaoHuDa_CB_XZDD", true);
		}
	}
	
	//�����ӵ� ���� �����ӷ�
	if(pInfo->kName =="ZiMoJiaDi_CB_XZDD" || pInfo->kName =="ZiMoJiaFan_CB_XZDD" )
	{
		if (pInfo->kName =="ZiMoJiaDi_CB_XZDD")
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIADI_));
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIAFAN_));
			WidgetFun::setChecked(this, "ZiMoJiaDi_CB_XZDD", true);
			WidgetFun::setChecked(this, "ZiMoJiaFan_CB_XZDD", false);
		}else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIADI_));
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZI_MO_JIAFAN_));
			WidgetFun::setChecked(this, "ZiMoJiaDi_CB_XZDD", false);
			WidgetFun::setChecked(this, "ZiMoJiaFan_CB_XZDD", true);
		}
	}
	if(pInfo->kName =="HaiDiLao_CB_XZDD")
	{
		//������	
		if (WidgetFun::isChecked(this, "HaiDiLao_CB_XZDD") == true)
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HAI_DI_LAO_));
		}
		else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HAI_DI_LAO_));
		}
	}


	//������
	if(pInfo->kName =="HuanSanZhang_CB_XZDD")
	{
		if (WidgetFun::isChecked(this, "HuanSanZhang_CB_XZDD") == true)
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_));
		}
		else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_));
		}
	}

	//�۾�
	if(pInfo->kName =="YaoJiu_CB_XZDD")
	{
		if (WidgetFun::isChecked(this, "YaoJiu_CB_XZDD") == true)
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_YI_JIU_));
		}
		else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_YI_JIU_));
		}
	}

	//�Խ�
	if(pInfo->kName =="JiangDui_CB_XZDD")
	{
		if (WidgetFun::isChecked(this, "JiangDui_CB_XZDD") == true)
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_Jiang_dui_));
		}
		else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_Jiang_dui_));
		}
	}

	//����
	if(pInfo->kName =="MenQing_CB_XZDD")
	{
		if (WidgetFun::isChecked(this, "MenQing_CB_XZDD") == true)
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_MEN_Q_));
		}
		else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_MEN_Q_));
		}
	}

	//����
	if(pInfo->kName =="ZhongZhang_CB_XZDD")
	{
		if (WidgetFun::isChecked(this, "ZhongZhang_CB_XZDD") == true)
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZHONG_ZHANG_));
		}
		else
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_ZHONG_ZHANG_));
		}
	}

	//��ܻ�(����) ���� ��ܻ�(����)
	if(pInfo->kName =="DianGangHua_DianPao_CB_XZDD" || pInfo->kName =="DianGangHua_ZiMo_CB_XZDD" )
	{
		if (pInfo->kName =="DianGangHua_DianPao_CB_XZDD")
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_ZIMO_));
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_DIANPAO_));
			WidgetFun::setChecked(this, "DianGangHua_DianPao_CB_XZDD", true);
			WidgetFun::setChecked(this, "DianGangHua_ZiMo_CB_XZDD", false);
		}else
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_ZIMO_));
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_D_G_H_DIANPAO_));
			WidgetFun::setChecked(this, "DianGangHua_DianPao_CB_XZDD", false);
			WidgetFun::setChecked(this, "DianGangHua_ZiMo_CB_XZDD", true);
		}
	}

	//4�� �� 3�� �� 2��
	if(pInfo->kName =="SiFan_CB_XZDD" || pInfo->kName =="SanFan_CB_XZDD"  || pInfo->kName =="LiangFan_CB_XZDD")
	{
		if (pInfo->kName =="SiFan_CB_XZDD")
		{
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN0_));
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN1_));
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN2_));
			WidgetFun::setChecked(this, "SiFan_CB_XZDD", true);
			WidgetFun::setChecked(this, "SanFan_CB_XZDD", false);
			WidgetFun::setChecked(this, "LiangFan_CB_XZDD", false);
		}else if (pInfo->kName =="SanFan_CB_XZDD")
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN0_));
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN1_));
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN2_));
			WidgetFun::setChecked(this, "SiFan_CB_XZDD", false);
			WidgetFun::setChecked(this, "SanFan_CB_XZDD", true);
			WidgetFun::setChecked(this, "LiangFan_CB_XZDD", false);
		}else if(pInfo->kName =="LiangFan_CB_XZDD")
		{
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN0_));
			FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN1_));
			FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DI_FEN2_));
			WidgetFun::setChecked(this, "SiFan_CB_XZDD", false);
			WidgetFun::setChecked(this, "SanFan_CB_XZDD", false);
			WidgetFun::setChecked(this, "LiangFan_CB_XZDD", true);
		}
	}

	//����	
	//if (WidgetFun::isChecked(this, "ChaDaJiao_XZDD") == true)
	//{
	//	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_CHADAJIAO));
	//	WidgetFun::setChecked(this, "ChaDaJiao_XZDD", true);
	//}
	//else
	//{
	//	FvMask::Del(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_CHADAJIAO));
	//	WidgetFun::setChecked(this, "ChaDaJiao_XZDD", false);
	//}
}

void  GPHomeCreateRoomPanel_XZDD::Button_GameRuleDiZhuNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeEditNumPanel::Instance().show("XZDD", 1);
}

void  GPHomeCreateRoomPanel_XZDD::Button_GameRuleLiChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeEditNumPanel::Instance().show("XZDD", 3);
}

void  GPHomeCreateRoomPanel_XZDD::Button_GameRuleJinChangNum(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeEditNumPanel::Instance().show("XZDD", 2);
}

//���õͷ�//
void GPHomeCreateRoomPanel_XZDD::updateScore(BYTE cbIndex, dword wScore)
{
	switch (cbIndex)
	{
	case 1:
	{
		m_GameRuleDiZhuNum = wScore;
		m_GameRuleJinChangNum = wScore * 80;
		m_GameRuleLiChangNum = wScore * 70;
		WidgetFun::setText(this, "XZDD_CreateRoom_GameScore_Txt", wScore);
		WidgetFun::setText(this, "XZDD_CreateRoom_EnterNum_Txt", wScore * 80);
		WidgetFun::setText(this, "XZDD_CreateRoom_OutNum_Txt", wScore * 70);
	}
	break;
	case 2:
	{
		m_GameRuleJinChangNum = wScore;
		WidgetFun::setText(this, "XZDD_CreateRoom_EnterNum_Txt", wScore);
	}
	break;
	case 3:
	{
		m_GameRuleLiChangNum = wScore;
		WidgetFun::setText(this, "XZDD_CreateRoom_OutNum_Txt", wScore);
	}
	break;
	default:
		break;
	}
}

void GPHomeCreateRoomPanel_XZDD::Button_CreateRoom_XZDD(cocos2d::Ref*, WidgetUserInfo*)
{
	FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_DIN_QUE_));
	//FvMask::Add(m_CurrentRuleIndex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_));
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet, sizeof(kSendNet));
	kSendNet.cbGameType = Type_Private;
	kSendNet.dwGameRuleIdex = m_CurrentRuleIndex; 
	kSendNet.bAllowedStrangerJoin =m_GameRuleJoinMoShenRen;//�Ƿ�����İ���˼���
	kSendNet.bPassionationMode = false;//����ģʽ����
	kSendNet.bBloodFightMode =false;//Ѫս���׿���
	//char							stHttpChannel[LEN_NICKNAME];			//http��ȡ
	kSendNet.dwBaseScore = m_GameRuleDiZhuNum;  //��ע
	kSendNet.dwEnterMatchNum = m_GameRuleJinChangNum;  //�볡����
	kSendNet.dwOutMatchNum = m_GameRuleLiChangNum;  //�볡����
	kSendNet.bGameTypeIdex = GAME_TYPE_SICHUAN;
	kSendNet.bPlayCoutIdex =0;/*(m_CurrentGameCount - 4) / 4*/
	kSendNet.wCharNum = 4;
	GPGameLink::Instance().CreateRoom(XZDDGameScence::KIND_ID, kSendNet);
}