#include "GPHomeRoomListPanel_NN.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "Game/NN/NNGameScene.h"
#include "Game/NN/CMD_NN.h"
#include "kernel/kernel/server/CServerItem.h"
#include "Plaza/HomeScene/GPHomeEnterRoomPanel.h"
#include "Plaza/HomeScene/CreateRoom/GPHomeCreateRoomPanel_NN.h"

#include "UTILITY.h"

FV_SINGLETON_STORAGE(GPHomeRoomListPanel_NN);
GPHomeRoomListPanel_NN::GPHomeRoomListPanel_NN()
	: m_CurrentRuleIndex(0)
	, m_lastTime(0)
{
	init();
}

GPHomeRoomListPanel_NN::~GPHomeRoomListPanel_NN()
{
}

bool GPHomeRoomListPanel_NN::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}

	WidgetScenceXMLparse createRoomListPanel_NN("GamePlaza/Script/GPHomeRoomListPanel_NN.xml", this);
	initButton();
	initListView();
	return true;
}

void GPHomeRoomListPanel_NN::initButton()
{
	WidgetManager::addButtonCB("Btn_Close_NN", this, button_selector(GPHomeRoomListPanel_NN::Button_Close));
	WidgetManager::addButtonCB("Btn_Refresh_NN", this, button_selector(GPHomeRoomListPanel_NN::Button_Refresh));
	WidgetManager::addButtonCB("Btn_Join_NN", this, button_selector(GPHomeRoomListPanel_NN::Button_Join));
	WidgetManager::addButtonCB("Btn_createRoom_NN", this, button_selector(GPHomeRoomListPanel_NN::Button_CreateRoom));
	WidgetManager::addButtonCB("ButtonJionRomByIDRight_NN", this, button_selector(GPHomeRoomListPanel_NN::Button_JionRomByIDRight));
	WidgetManager::addButtonCB("ButtonJionRomByIDLeft_NN", this, button_selector(GPHomeRoomListPanel_NN::Button_JionRomByIDLeft));
	WidgetManager::addButtonCB("ButtonJionRomByIDLeft_NN", this, button_selector(GPHomeRoomListPanel_NN::Button_JionRomByIDLeft));
}

void GPHomeRoomListPanel_NN::initListView(){
	widget::ListViewEx* pListView = WidgetFun::getListViewWidget(this,"GameListView_NN");
	//pListView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	//pListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	//pListView->setGravity(ui::ListView::Gravity::TOP);
	pListView->setItemsMargin(20.0f);
	pListView->addEventListener((widget::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeRoomListPanel_NN::onSelectedItemEvent,this));
	pListView->removeAllChildren();
}

void GPHomeRoomListPanel_NN::show()
{
	Button_Refresh(NULL, NULL);
	setVisible(true);
}

void GPHomeRoomListPanel_NN::hide()
{
	setVisible(false);
}

//////////////////////////////////////////////////////////////////////////

void GPHomeRoomListPanel_NN::RefreshListView(void* data, size_t dataSize){
	cocos2d::log("GPHomeRoomListPanel_NN::RefreshListView");
    int offset = 0;
    void *pData = data;
    char* current_data = static_cast<char*>(pData);

    int iGameID = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwGameKindID;
    if(iGameID != NNGameScene::KIND_ID)
    {
        return;
    }
    int iTableNum= (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwTablesTotal;

    offset = sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_HEAD);	pData = current_data + offset;
    CMD_GR_INQURE_TABLES_INFO_DATA_BODY* body;
    widget::ListViewEx* pList = WidgetFun::getListViewWidget(this,"GameListView_NN");
    pList->removeAllChildren();
    cocos2d::Node* pItemNode = nullptr;
    for(int i = 0; i < iTableNum; i++)
    {
        body = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_BODY*>(pData));

        if((i+1) % 2 == 1)
            pItemNode = WidgetManager::Instance().createWidget("GameRoomInfoListItem_NN", pList);
        if(pItemNode == nullptr) continue;
        //规则
        std::string strRule("拼十");
        //if(body->bBloodFightMode) //血战到底
        //{
        //    strRule.append("抢庄牛牛 ");
        //}
        //if(body->bPassionationMode) //激情模式
        //{
        //    strRule.append("抢庄牛牛");
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
            //WidgetManager::addButtonCB("ButtonJionRomByIDLeft_NN",this,button_selector(GPHomeRoomListPanel_NN::Button_JionRomByIDLeft));
            WidgetFun::setWidgetUserInfo(pItemNode,"ButtonJionRomByIDLeft_NN","RoomIDLeft", utility::toString(body->dwTableNum));
        
			//WidgetFun::setVisible(pItemNode, "RuleText1", true);
			//WidgetFun::setVisible(pItemNode, "SitCountText1", true);
			//WidgetFun::setVisible(pItemNode, "MinScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "EnterScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "LiveScoreText1", true);
			//WidgetFun::setVisible(pItemNode, "ButtonJionRomByIDLeft_NN", true);

			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_NN_0", true);
		}
        else
        {
            WidgetFun::setText(pItemNode,"RuleText2", utility::a_u8( strRule.c_str()));
            WidgetFun::setText(pItemNode, "SitCountText2", strSitCout);
            WidgetFun::setText(pItemNode, "MinScoreText2", strMinSorce);
            //WidgetFun::setText(pItemNode, "EnterScoreText2", strEnterSorce);
            //WidgetFun::setText(pItemNode, "LiveScoreText2", strLiveSorce);
            //WidgetManager::addButtonCB("ButtonJionRomByIDRight_NN",this,button_selector(GPHomeRoomListPanel_NN::Button_JionRomByIDRight));
            WidgetFun::setWidgetUserInfo(pItemNode,"ButtonJionRomByIDRight_NN","RoomIDRight", utility::toString(body->dwTableNum));
			
			//WidgetFun::setVisible(pItemNode, "RuleText2", true);
			//WidgetFun::setVisible(pItemNode, "SitCountText2", true);
			//WidgetFun::setVisible(pItemNode, "MinScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "EnterScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "LiveScoreText2", true);
			//WidgetFun::setVisible(pItemNode, "ButtonJionRomByIDRight_NN", true);
			WidgetFun::setVisible(pItemNode, "GameRoomInfoListItem_NN_1", true);
		}

        offset += sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_BODY);	pData = current_data + offset;
    }
    pList->forceDoLayout();
}

//UIListView适配函数
void  GPHomeRoomListPanel_NN::onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
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

void GPHomeRoomListPanel_NN::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}
void GPHomeRoomListPanel_NN::Button_Refresh(cocos2d::Ref*, WidgetUserInfo*)
{
	time_t  curTime = time(0);
	if ( curTime - m_lastTime < DELAYTIME ) return;
	m_lastTime = curTime;
    CMD_GR_INQURE_TABLES_INFO m_gr_inqure_tables;
    m_gr_inqure_tables.bGetTableInfo = NNGameScene::KIND_ID;
    //if( !CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_INQUIRE_TABLES, &m_gr_inqure_tables, sizeof(m_gr_inqure_tables)) )
    //{
        GPGameLink::Instance().ConnectAndInqureTables(NNGameScene::KIND_ID);
    //}
    GPGameLink::Instance().setCurrentRequestGameKindID(NNGameScene::KIND_ID);
}

void GPHomeRoomListPanel_NN::Button_Join(cocos2d::Ref*, WidgetUserInfo*)
{
	//ToDo添加房间刷新界面
	hide();
	GPHomeEnterRoomPanel::Instance().show();
}

void GPHomeRoomListPanel_NN::Button_CreateRoom(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeCreateRoomPanel_NN::Instance().show();
}

void GPHomeRoomListPanel_NN::Button_JionRomByIDLeft(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RoomIDLeft"));

    CMD_GR_Join_Private kSendNet;
    kSendNet.dwRoomNum = iRecordChildID;
    CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));
    //WidgetFun::setVisible(this, "ShowRomPlane", false);
}

void GPHomeRoomListPanel_NN::Button_JionRomByIDRight(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RoomIDRight"));

    CMD_GR_Join_Private kSendNet;
    kSendNet.dwRoomNum = iRecordChildID;
    CServerItem::get()->SendSocketData(MDM_GR_PRIVATE, SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));

    //WidgetFun::setVisible(this, "ShowRomPlane", false);
}

void GPHomeRoomListPanel_NN::setGameRuleIdex(int iIdex)
{

}
