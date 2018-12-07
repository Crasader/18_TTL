#include "GPHomeRoomListPanel_TLJ.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/MENJI/TLJGameScence.h"
#include "ClientHN_THJ/Game/SRLF/CMD_SRLF.h"
#include "kernel/kernel/server/CServerItem.h"
#include "GamePlaza/HomeScene/EnterRoomPanel/GPHomeEnterRoomPanel.h"
#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeCreateRoomPanel_TLJ.h"

USING_NS_CC;

FV_SINGLETON_STORAGE(GPHomeRoomListPanel_TLJ);
GPHomeRoomListPanel_TLJ::GPHomeRoomListPanel_TLJ()
	: m_CurrentRuleIndex(0)
	, m_lastTime(0)
{
	init();
}

GPHomeRoomListPanel_TLJ::~GPHomeRoomListPanel_TLJ()
{
}

bool GPHomeRoomListPanel_TLJ::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	WidgetScenceXMLparse createRoomListPanel_TLJ("GamePlaza/Script/GPHomeRoomListPanel_TLJ.xml", this);
	initButton();
	initListView();
	return true;
}

void GPHomeRoomListPanel_TLJ::initListView(){
	cocos2d::ListViewEx* pListView = WidgetFun::getListViewWidget(this,"GameListView_TLJ");
	//pListView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	//pListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	//pListView->setGravity(ui::ListView::Gravity::TOP);
	pListView->setItemsMargin(20.0f);
	pListView->addEventListener((cocos2d::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeRoomListPanel_TLJ::onSelectedItemEvent,this));
	pListView->removeAllChildren();
}

void GPHomeRoomListPanel_TLJ::RefreshListView(void* data, size_t dataSize){
	cocos2d::log("GPHomeRoomListPanel_TLJ::RefreshListView");
    int offset = 0;
    void *pData = data;
    char* current_data = static_cast<char*>(pData);

    int iGameID = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwGameKindID;
    if(iGameID != TLJGameScence::KIND_ID)
    {
        return;
    }
    int iTableNum= (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwTablesTotal;

    offset = sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_HEAD);	pData = current_data + offset;
    CMD_GR_INQURE_TABLES_INFO_DATA_BODY* body;
    cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this,"GameListView_TLJ");
    pList->removeAllChildren();
    cocos2d::Node* pItemNode = nullptr;
    for(int i = 0; i < iTableNum; i++)
    {
        body = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_BODY*>(pData));

        if((i+1) % 2 == 1)
            pItemNode = WidgetManager::Instance().createWidget("GameRoomInfoListItem_TLJ", pList);
        if(pItemNode == nullptr) continue;
        //规则
        std::string strRule("");

		if(FvMask::HasAny(body->dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_COMMON))){
			strRule.append("普通模式");
		}

		if(FvMask::HasAny(body->dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_PASSION))){
			strRule.append("激情模式");
		}

		if(FvMask::HasAny(body->dwGameRuleIdex,_MASK_(GPHomeCreateRoomPanel_TLJ::GAME_RULE_BRAISED))){
			strRule.append("必焖三圈");
		}
        //if(body->bBloodFightMode) //普通模式
        //{
         //   strRule.append("普通模式 ");
        //}
        //if(body->bPassionationMode) //激情模式
        //{
         //   strRule.append("激情模式");
        //}
		//if(body->bPassionationMode) //必焖三圈
		//{
		//	strRule.append("必焖三圈");
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
        if((i+1) % 2 == 1)
        {
            WidgetFun::setText(pItemNode, "RuleText1", utility::a_u8( strRule.c_str()));
            WidgetFun::setText(pItemNode, "SitCountText1", strSitCout);
            WidgetFun::setText(pItemNode, "MinScoreText1", strMinSorce);
            //WidgetFun::setText(pItemNode, "EnterScoreText1", strEnterSorce);
            //WidgetFun::setText(pItemNode, "LiveScoreText1", strLiveSorce);
            //WidgetManager::addButtonCB("ButtonJionRomByIDLeft",this,button_selector(GPHomeRoomListPanel_TLJ::Button_JionRomByIDLeft));
            WidgetFun::setWidgetUserInfo(pItemNode,"ButtonJionRomByIDLeft_TLJ","RoomIDLeft", utility::toString(body->dwTableNum));
        
			//WidgetFun::setVisible(pItemNode, "RuleText1", true);
			//WidgetFun::setVisible(pItemNode, "SitCountText1", true);
			//WidgetFun::setVisible(pItemNode, "MinScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "EnterScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "LiveScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "ButtonJionRomByIDLeft_TLJ", true);

			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_TLJ_0", true);
		}
        else
        {
            WidgetFun::setText(pItemNode,"RuleText2", utility::a_u8( strRule.c_str()));
            WidgetFun::setText(pItemNode, "SitCountText2", strSitCout);
            WidgetFun::setText(pItemNode, "MinScoreText2", strMinSorce);
            //WidgetFun::setText(pItemNode, "EnterScoreText2", strEnterSorce);
            //WidgetFun::setText(pItemNode, "LiveScoreText2", strLiveSorce);
            //WidgetManager::addButtonCB("ButtonJionRomByIDRight",this,button_selector(GPHomeRoomListPanel_TLJ::Button_JionRomByIDRight));
            WidgetFun::setWidgetUserInfo(pItemNode,"ButtonJionRomByIDRight_TLJ","RoomIDRight", utility::toString(body->dwTableNum));
			
			//WidgetFun::setVisible(pItemNode, "RuleText2", true);
			//WidgetFun::setVisible(pItemNode, "SitCountText2", true);
			//WidgetFun::setVisible(pItemNode, "MinScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "EnterScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "LiveScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "ButtonJionRomByIDRight_TLJ", true);
			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_TLJ_1", true);
		}

        offset += sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_BODY);	pData = current_data + offset;
    }
    pList->forceDoLayout();
}

//UIListView适配函数
void  GPHomeRoomListPanel_TLJ::onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
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


void GPHomeRoomListPanel_TLJ::initButton()
{
	WidgetManager::addButtonCB("Btn_Close_TLJ", this, button_selector(GPHomeRoomListPanel_TLJ::Button_Close)); 
	WidgetManager::addButtonCB("Btn_Refresh_TLJ", this, button_selector(GPHomeRoomListPanel_TLJ::Button_Refresh)); 
	WidgetManager::addButtonCB("Btn_Join_TLJ", this, button_selector(GPHomeRoomListPanel_TLJ::Button_Join)); 
	WidgetManager::addButtonCB("Btn_createRoom_TLJ", this, button_selector(GPHomeRoomListPanel_TLJ::Button_CreateRoom)); 
	WidgetManager::addButtonCB("ButtonJionRomByIDRight_TLJ",this,button_selector(GPHomeRoomListPanel_TLJ::Button_JionRomByIDRight));
	WidgetManager::addButtonCB("ButtonJionRomByIDLeft_TLJ",this,button_selector(GPHomeRoomListPanel_TLJ::Button_JionRomByIDLeft));
}

void GPHomeRoomListPanel_TLJ::show()
{
	Button_Refresh(NULL,NULL);
	setVisible(true);
}

void GPHomeRoomListPanel_TLJ::hide()
{
	setVisible(false);
}

void GPHomeRoomListPanel_TLJ::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}
void GPHomeRoomListPanel_TLJ::Button_Refresh(cocos2d::Ref*, WidgetUserInfo*)
{
	time_t  curTime = time(0);
	if ( curTime - m_lastTime < DELAYTIME ) return;
	m_lastTime = curTime;
    //TODO: 发送请求房间列表消息
    CMD_GR_INQURE_TABLES_INFO m_gr_inqure_tables;
    m_gr_inqure_tables.bGetTableInfo = TLJGameScence::KIND_ID;
    GPGameLink::Instance().ConnectAndInqureTables(TLJGameScence::KIND_ID);
    GPGameLink::Instance().setCurrentRequestGameKindID(TLJGameScence::KIND_ID);
}

void GPHomeRoomListPanel_TLJ::Button_Join(cocos2d::Ref*, WidgetUserInfo*)
{
	//ToDo添加房间刷新界面
	hide();
	GPHomeEnterRoomPanel::Instance().show();
}

void GPHomeRoomListPanel_TLJ::Button_CreateRoom(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeCreateRoomPanel_TLJ::Instance().show();
}

void GPHomeRoomListPanel_TLJ::Button_JionRomByIDLeft(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RoomIDLeft"));

    CMD_GR_Join_Private kSendNet;
    kSendNet.dwRoomNum = iRecordChildID;
    CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));
    //WidgetFun::setVisible(this, "ShowRomPlane", false);
}

void GPHomeRoomListPanel_TLJ::Button_JionRomByIDRight(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RoomIDRight"));

    CMD_GR_Join_Private kSendNet;
    kSendNet.dwRoomNum = iRecordChildID;
    CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));

    //WidgetFun::setVisible(this, "ShowRomPlane", false);
}
