#include "GPHomeRoomListPanel_XZDD.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "kernel/kernel/server/CServerItem.h"
#include "Game/XZDD/XZDDGameScence.h"
#include "Game/XZDD/CMD_XZDD.h"
#include "Plaza/HomeScene/GPHomeEnterRoomPanel.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_XZDD.h"
#include "UTILITY.h"
 

FV_SINGLETON_STORAGE(GPHomeRoomListPanel_XZDD);
GPHomeRoomListPanel_XZDD::GPHomeRoomListPanel_XZDD()
	: m_CurrentRuleIndex(0)
{
	init();
}

GPHomeRoomListPanel_XZDD::~GPHomeRoomListPanel_XZDD()
{
}

bool GPHomeRoomListPanel_XZDD::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	WidgetScenceXMLparse createRoomListPanel_SG("GamePlaza/Script/GPHomeRoomListPanel_XZDD.xml", this);
	initButton();
	return true;
}

void GPHomeRoomListPanel_XZDD::initButton()
{
	WidgetManager::addButtonCB("Btn_Close_XZDD", this, button_selector(GPHomeRoomListPanel_XZDD::Button_Close));
	WidgetManager::addButtonCB("Btn_Refresh_XZDD", this, button_selector(GPHomeRoomListPanel_XZDD::Button_Refresh));
	WidgetManager::addButtonCB("Btn_Join_XZDD", this, button_selector(GPHomeRoomListPanel_XZDD::Button_Join));
	WidgetManager::addButtonCB("Btn_createRoom_XZDD", this, button_selector(GPHomeRoomListPanel_XZDD::Button_CreateRoom));
}

void GPHomeRoomListPanel_XZDD::show()
{
	Button_Refresh(NULL, NULL);
	setVisible(true);
}

void GPHomeRoomListPanel_XZDD::hide()
{
	setVisible(false);
}

void GPHomeRoomListPanel_XZDD::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeRoomListPanel_XZDD::Button_Join(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
	GPHomeEnterRoomPanel::Instance().show();
}

void GPHomeRoomListPanel_XZDD::Button_CreateRoom(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeCreateRoomPanel_XZDD::Instance().show();
}

void GPHomeRoomListPanel_XZDD::Button_Refresh(cocos2d::Ref*, WidgetUserInfo*)
{
	//TODO: 发送请求房间列表消息
	CMD_GR_INQURE_TABLES_INFO m_gr_inqure_tables;
	m_gr_inqure_tables.bGetTableInfo = 1;// XZDDGameScence::KIND_ID;
	//if( !CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_INQUIRE_TABLES, &m_gr_inqure_tables, sizeof(m_gr_inqure_tables)) )
	//{
	GPGameLink::Instance().ConnectAndInqureTables(XZDDGameScence::KIND_ID);
	//}
	GPGameLink::Instance().setCurrentRequestGameKindID(XZDDGameScence::KIND_ID);
}

void GPHomeRoomListPanel_XZDD::initListView()
{
	widget::ListViewEx* pListView = WidgetFun::getListViewWidget(this, "GameListView_XZDD");
	//pListView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	//pListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	//pListView->setGravity(ui::ListView::Gravity::TOP);
	pListView->setItemsMargin(20.0f);
	pListView->addEventListener((widget::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeRoomListPanel_XZDD::onSelectedItemEvent, this));
	pListView->removeAllChildren();
}

//UIListView适配函数
void  GPHomeRoomListPanel_XZDD::onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
	{
		cocos2d::ui::ListView*  listview = static_cast<cocos2d::ui::ListView*>(pSender);
		cocos2d::log("ON_SELECTED_ITEM_START:%ld", listview->getCurSelectedIndex());
		break;
	}
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
	{
		cocos2d::ui::ListView* listview = static_cast<cocos2d::ui::ListView*>(pSender);
		//if (m_selectedItemIndex == listview->getCurSelectedIndex())
		cocos2d::log("ON_SELECTED_ITEM_END:%ld", listview->getCurSelectedIndex());
		long int index = listview->getCurSelectedIndex();
	}
	default:
		break;
	}
}

void GPHomeRoomListPanel_XZDD::RefreshListView(void* data, size_t dataSize){
	cocos2d::log("GPHomeCreateRoomPanel_XZDD::RefreshListView");
	int offset = 0;
	void *pData = data;
	char* current_data = static_cast<char*>(pData);

	int iGameID = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwGameKindID;
	if (iGameID != XZDDGameScence::KIND_ID)
	{
		return;
	}
	int iTableNum = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwTablesTotal;

	offset = sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_HEAD);	pData = current_data + offset;

	CMD_GR_INQURE_TABLES_INFO_DATA_BODY* body;

	widget::ListViewEx* pList = WidgetFun::getListViewWidget(this, "GameListView_XZDD");
	pList->removeAllChildren();

	cocos2d::Node* pItemNode = nullptr;
	for (int i = 0; i < iTableNum; i++)
	{
		body = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_BODY*>(pData));

		if ((i + 1) % 2 == 1)
			pItemNode = WidgetManager::Instance().createWidget("GameRoomInfoListItem_XZDD", pList);
		if (pItemNode == nullptr) continue;
		//规则
		std::string strRule("");
		if (FvMask::HasAll(body->dwGameRuleIdex, _MASK_(GPHomeCreateRoomPanel_XZDD::GAME_RULE_HUAN3_))) //換三張
		{
			strRule = "血战换三张";
		}
		else{
			strRule = "血战麻将";
		}
		//if (body->bBloodFightMode) //私人房间
		//{
		//	strRule.append("私人房间");
		//}
		//if (body->bPassionationMode) //元宝房间
		//{
		//	strRule.append("元宝房间");
		//}
		//人数
		std::string strSitCout = utility::toString((dword)body->bSitUserSum, "/", (dword)body->bChairSum, utility::a_u8("人"));

		//底注
		std::string strMinSorce = utility::toString(utility::a_u8("底注:"), body->dwBaseScore);

		//入场
		std::string strEnterSorce = utility::toString(utility::a_u8("入场:"), body->dwEnterMatchNum);

		//离场
		std::string strLiveSorce = utility::toString(utility::a_u8("离场:"), body->dwOutMatchNum);
		strMinSorce = utility::toString(strMinSorce," ",strEnterSorce," ",strLiveSorce);
		if ((i + 1) % 2 == 1)
		{
			WidgetFun::setText(pItemNode, "RuleText1", utility::a_u8(strRule.c_str()));
			WidgetFun::setText(pItemNode, "SitCountText1", strSitCout);
			WidgetFun::setText(pItemNode, "MinScoreText1", strMinSorce);
			WidgetManager::addButtonCB("ButtonJionRomByIDLeft_XZDD", this, button_selector(GPHomeRoomListPanel_XZDD::Button_JionRomByIDLeft));
			WidgetFun::setWidgetUserInfo(pItemNode, "ButtonJionRomByIDLeft_XZDD", "RoomIDLeft", utility::toString(body->dwTableNum));
			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_XZDD_0", true);
		}
		else
		{
			WidgetFun::setText(pItemNode, "RuleText2", utility::a_u8(strRule.c_str()));
			WidgetFun::setText(pItemNode, "SitCountText2", strSitCout);
			WidgetFun::setText(pItemNode, "MinScoreText2", strMinSorce);
			WidgetManager::addButtonCB("ButtonJionRomByIDRight_XZDD", this, button_selector(GPHomeRoomListPanel_XZDD::Button_JionRomByIDRight));
			WidgetFun::setWidgetUserInfo(pItemNode, "ButtonJionRomByIDRight_XZDD", "RoomIDRight", utility::toString(body->dwTableNum));
			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_XZDD_1", true);
		}

		offset += sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_BODY);	pData = current_data + offset;
	}
	pList->forceDoLayout();
}

void GPHomeRoomListPanel_XZDD::Button_JionRomByIDLeft(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RoomIDLeft"));

	CMD_GR_Join_Private kSendNet;
	kSendNet.dwRoomNum = iRecordChildID;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE, &kSendNet, sizeof(kSendNet));

	//WidgetFun::setVisible(this, "ShowRomPlane", false);
}

void GPHomeRoomListPanel_XZDD::Button_JionRomByIDRight(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RoomIDRight"));

	CMD_GR_Join_Private kSendNet;
	kSendNet.dwRoomNum = iRecordChildID;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE, &kSendNet, sizeof(kSendNet));

	//WidgetFun::setVisible(this, "ShowRomPlane", false);
}