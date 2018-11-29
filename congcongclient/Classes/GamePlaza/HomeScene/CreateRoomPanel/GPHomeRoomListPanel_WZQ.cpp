#include "GPHomeRoomListPanel_WZQ.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/SRLF/CMD_SRLF.h"
#include "kernel/kernel/server/CServerItem.h"
#include "GamePlaza/HomeScene/EnterRoomPanel/GPHomeEnterRoomPanel.h"
#include "ClientHN_THJ/Game/WZQ/WZQDGameScence.h"

USING_NS_CC;

FV_SINGLETON_STORAGE(GPHomeRoomListPanel_WZQ);
GPHomeRoomListPanel_WZQ::GPHomeRoomListPanel_WZQ()
	: m_CurrentRuleIndex(0)
{
	init();
}

GPHomeRoomListPanel_WZQ::~GPHomeRoomListPanel_WZQ()
{
}

bool GPHomeRoomListPanel_WZQ::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	WidgetScenceXMLparse createRoomListPanel_SG("GamePlaza/Script/GPHomeRoomListPanel_WZQ.xml", this);
	initButton();
	initListView();
	return true;
}

void GPHomeRoomListPanel_WZQ::initListView(){
	cocos2d::ListViewEx* pListView = WidgetFun::getListViewWidget(this,"GameListView_WZQ");
	//pListView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	//pListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	//pListView->setGravity(ui::ListView::Gravity::TOP);
	pListView->setItemsMargin(20.0f);
	pListView->addEventListener((cocos2d::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeRoomListPanel_WZQ::onSelectedItemEvent,this));
	pListView->removeAllChildren();
}

void GPHomeRoomListPanel_WZQ::RefreshListView(void* data, size_t dataSize){
	cocos2d::log("GPHomeRoomListPanel_WZQ::RefreshListView");
    int offset = 0;
    void *pData = data;
    char* current_data = static_cast<char*>(pData);

    int iGameID = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwGameKindID;
    //if(iGameID != TLJGameScence::KIND_ID)
    //{
    //    return;
    //}
    int iTableNum= (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwTablesTotal;

    offset = sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_HEAD);	pData = current_data + offset;
    CMD_GR_INQURE_TABLES_INFO_DATA_BODY* body;
    cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this,"GameListView_WZQ");
    pList->removeAllChildren();
    cocos2d::Node* pItemNode = nullptr;
    for(int i = 0; i < iTableNum; i++)
    {
        body = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_BODY*>(pData));

        if((i+1) % 2 == 1)
            pItemNode = WidgetManager::Instance().createWidget("GameRoomInfoListItem_WZQ", pList);
        if(pItemNode == nullptr) continue;
        //规则
        std::string strRule;
        if(body->bBloodFightMode) //血战到底
        {
            //strRule.append("五子棋 ");
        }
        if(body->bPassionationMode) //激情模式
        {
            //strRule.append("五子棋");
        }
		strRule.append("五子棋");
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
            //WidgetManager::addButtonCB("ButtonJionRomByIDLeft_WZQ",this,button_selector(GPHomeRoomListPanel_WZQ::Button_JionRomByIDLeft));
            WidgetFun::setWidgetUserInfo(pItemNode,"ButtonJionRomByIDLeft_WZQ","RoomIDLeft", utility::toString(body->dwTableNum));
        
			//WidgetFun::setVisible(pItemNode, "RuleText1", true);
			//WidgetFun::setVisible(pItemNode, "SitCountText1", true);
			//WidgetFun::setVisible(pItemNode, "MinScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "EnterScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "LiveScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "ButtonJionRomByIDLeft", true);
			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_TLJ_1", true);
		}
        else
        {
            WidgetFun::setText(pItemNode,"RuleText2", utility::a_u8( strRule.c_str()));
            WidgetFun::setText(pItemNode, "SitCountText2", strSitCout);
            WidgetFun::setText(pItemNode, "MinScoreText2", strMinSorce);
            //WidgetFun::setText(pItemNode, "EnterScoreText2", strEnterSorce);
            //WidgetFun::setText(pItemNode, "LiveScoreText2", strLiveSorce);
            //WidgetManager::addButtonCB("ButtonJionRomByIDRight_WZQ",this,button_selector(GPHomeRoomListPanel_WZQ::Button_JionRomByIDRight));
            WidgetFun::setWidgetUserInfo(pItemNode,"ButtonJionRomByIDRight_WZQ","RoomIDRight", utility::toString(body->dwTableNum));
			
			//WidgetFun::setVisible(pItemNode, "RuleText2", true);
			//WidgetFun::setVisible(pItemNode, "SitCountText2", true);
			//WidgetFun::setVisible(pItemNode, "MinScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "EnterScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "LiveScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "ButtonJionRomByIDRight", true);
			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_TLJ_1", true);
		}

        offset += sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_BODY);	pData = current_data + offset;
    }
    pList->forceDoLayout();
}

//UIListView适配函数
void  GPHomeRoomListPanel_WZQ::onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
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


void GPHomeRoomListPanel_WZQ::initButton()
{
	WidgetManager::addButtonCB("Btn_Close_WZQ", this, button_selector(GPHomeRoomListPanel_WZQ::Button_Close)); 
	WidgetManager::addButtonCB("Btn_Refresh_WZQ", this, button_selector(GPHomeRoomListPanel_WZQ::Button_Refresh)); 
	WidgetManager::addButtonCB("Btn_Join_WZQ", this, button_selector(GPHomeRoomListPanel_WZQ::Button_Join)); 
	WidgetManager::addButtonCB("Btn_createRoom_WZQ", this, button_selector(GPHomeRoomListPanel_WZQ::Button_CreateRoom)); 
    WidgetManager::addButtonCB("ButtonJionRomByIDLeft_WZQ",this,button_selector(GPHomeRoomListPanel_WZQ::Button_JionRomByIDLeft));
	WidgetManager::addButtonCB("ButtonJionRomByIDRight_WZQ",this,button_selector(GPHomeRoomListPanel_WZQ::Button_JionRomByIDRight));
}

void GPHomeRoomListPanel_WZQ::show()
{
	Button_Refresh(NULL,NULL);
	setVisible(true);
}

void GPHomeRoomListPanel_WZQ::hide()
{
	setVisible(false);
}

void GPHomeRoomListPanel_WZQ::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}
void GPHomeRoomListPanel_WZQ::Button_Refresh(cocos2d::Ref*, WidgetUserInfo*)
{
	return;
    //TODO: 发送请求房间列表消息
    CMD_GR_INQURE_TABLES_INFO m_gr_inqure_tables;
    m_gr_inqure_tables.bGetTableInfo = WZQDGameScence::KIND_ID;
    //if( !CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_INQUIRE_TABLES, &m_gr_inqure_tables, sizeof(m_gr_inqure_tables)) )
    //{
        GPGameLink::Instance().ConnectAndInqureTables(WZQDGameScence::KIND_ID);
    //}
    GPGameLink::Instance().setCurrentRequestGameKindID(WZQDGameScence::KIND_ID);
}

void GPHomeRoomListPanel_WZQ::Button_Join(cocos2d::Ref*, WidgetUserInfo*)
{
	//ToDo添加房间刷新界面
	hide();
	GPHomeEnterRoomPanel::Instance().show();
}

void GPHomeRoomListPanel_WZQ::Button_CreateRoom(cocos2d::Ref*, WidgetUserInfo*)
{
	CMD_GR_Create_Private kSendNet;
	zeromemory(&kSendNet, sizeof(kSendNet));
	kSendNet.cbGameType = Type_Private;
	kSendNet.bGameRuleIdex = 0;
	kSendNet.bAllowedStrangerJoin =true;//是否允许陌生人加入
	kSendNet.bPassionationMode = false;//激情模式开关
	kSendNet.bBloodFightMode =false;//血战到底开关
	//char							stHttpChannel[LEN_NICKNAME];			//http获取
	kSendNet.dwBaseScore = 0;  //底注
	kSendNet.dwEnterMatchNum = 0;  //入场限制
	kSendNet.dwOutMatchNum = 0;  //离场限制
	kSendNet.bGameTypeIdex = 0;
	kSendNet.bPlayCoutIdex =0;
	kSendNet.wCharNum =-1;
	GPGameLink::Instance().CreateRoom(WZQDGameScence::KIND_ID, kSendNet);
}

void GPHomeRoomListPanel_WZQ::Button_JionRomByIDLeft(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RoomIDLeft"));

    CMD_GR_Join_Private kSendNet;
    kSendNet.dwRoomNum = iRecordChildID;
    CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));
    //WidgetFun::setVisible(this, "ShowRomPlane", false);
}

void GPHomeRoomListPanel_WZQ::Button_JionRomByIDRight(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RoomIDRight"));

    CMD_GR_Join_Private kSendNet;
    kSendNet.dwRoomNum = iRecordChildID;
    CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));

    //WidgetFun::setVisible(this, "ShowRomPlane", false);
}
