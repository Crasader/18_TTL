#include "GPHomeScene.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNRoomInfo.h"
#include "GameLib/Game/Game/MissionWeiXin.h"

USING_NS_CC;
using namespace experimental;

void GPHomeScene::initButton()
{
	WidgetManager::addButtonCB("Button_Header", this, button_selector(GPHomeScene::Button_Header));
	WidgetManager::addButtonCB("Button_Share", this, button_selector(GPHomeScene::Button_Share));
	WidgetManager::addButtonCB("Button_ADDGOLD", this, button_selector(GPHomeScene::Button_Mall));
	WidgetManager::addButtonCB("Btn_Mall", this, button_selector(GPHomeScene::Button_Mall));
	WidgetManager::addButtonCB("Button_Setting", this, button_selector(GPHomeScene::Button_Setting));
	//WidgetManager::addButtonCB("Btn_Rank", this, button_selector(GPHomeScene::Button_Rank));
	//WidgetManager::addButtonCB("Button_Rank", this, button_selector(GPHomeScene::Button_Rank));
	WidgetManager::addButtonCB("Button_Record", this, button_selector(GPHomeScene::Button_Record));
	WidgetManager::addButtonCB("Btn_Invitation", this, button_selector(GPHomeScene::Button_Invitation));
	WidgetManager::addButtonCB("Button_JoinGame", this, button_selector(GPHomeScene::Button_Join));
	WidgetManager::addButtonCB("Btn_RoomList_Join", this, button_selector(GPHomeScene::Button_RoomList_Join));
	WidgetManager::addButtonCB("Button_CreateGame", this, button_selector(GPHomeScene::Button_CreateGame));
}

void GPHomeScene::Button_Share(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeSharePanel::Instance().show();
}

void GPHomeScene::Button_Header(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	GPHomeUserInfoPanel::Instance().show();
}

void GPHomeScene::Button_Mall( cocos2d::Ref*, WidgetUserInfo* )
{
	GPHomeMallPanel::Instance().show();
}

void GPHomeScene::Button_Setting( cocos2d::Ref*, WidgetUserInfo* )
{
	GPHomeSettingPanel::Instance().show();
	GPHomeSettingPanel::Instance().hideOrShowQuitBtn(true);
}

void GPHomeScene::Button_Rank( cocos2d::Ref*, WidgetUserInfo* )
{
	GPHomeRankPanel::Instance().show();
}

void GPHomeScene::Button_Record( cocos2d::Ref*, WidgetUserInfo* )
{
	GPHomeRecordPanel::Instance().show();
}

//////////////////////////////////////////////////////////////////////////

void GPHomeScene::pageViewEvent(Ref* pSender, cocos2d::ui::PageViewEventType type)
{
	switch (type) {
	case cocos2d::ui::PageViewEventType::PAGEVIEW_EVENT_TURNING:
	{
		cocos2d::ui::PageView* pageView = dynamic_cast<cocos2d::ui::PageView*>(pSender);
		log("pageView->getCurPageIndex() = %ld", pageView->getCurrentPageIndex() + 1);
	}
	break;

	default:
		break;
	}
}

//DONE:需要删除掉下列房间列表
void GPHomeScene::initGamePanelList()
{
	cocos2d::ListViewEx* pListView = WidgetFun::getListViewWidget(this, "GameListView");
	WidgetFun::setVisible(this, "GameListView", true);
	pListView->setEnabled(true);
	pListView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
	pListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	pListView->setGravity(cocos2d::ui::ListView::Gravity::TOP);
	pListView->setItemsMargin(10.0f);
	pListView->addEventListener((cocos2d::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeScene::onSelectedItemEvent, this));
}

//UIListView适配函数
void  GPHomeScene::onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
	{
		cocos2d::ui::ListView*  listview = static_cast<cocos2d::ui::ListView*>(pSender);
		m_selectedItemIndex = listview->getCurSelectedIndex();
		log("ON_SELECTED_ITEM_START:%ld", listview->getCurSelectedIndex());
		break;
	}
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
	{
		cocos2d::ui::ListView* listview = static_cast<cocos2d::ui::ListView*>(pSender);
		//if (m_selectedItemIndex == listview->getCurSelectedIndex())
		log("ON_SELECTED_ITEM_END:%ld", listview->getCurSelectedIndex());
		long int index = listview->getCurSelectedIndex();
		switch (index) {
		case 0: {
			//抢庄牛牛
			Button_NN(NULL, NULL);
			break;
		}
		case 1: {
			Button_MenJi(NULL, NULL);//经典三张	
			break;
		}
		case 2: {
			Button_SG(NULL, NULL);//三公	
			break;
		}
		case 3: {
			Button_XZDD(NULL, NULL);//血战换三张
			break;
		}
		case 4: {
			Button_SRLF(NULL, NULL);//三人两房
			break;
		}
		case 5: {
			Button_WZQ(NULL, NULL);//五子棋
			break;
		}
		}
		break;
	}
	default:
		break;
	}
}

void GPHomeScene::Button_NN(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeRoomListPanel_NN::Instance().show();
}

void GPHomeScene::Button_SRLF(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeRoomListPanel_SRLF::Instance().show();
}

void GPHomeScene::Button_MenJi(cocos2d::Ref*, WidgetUserInfo*)
{
	//GPHomeCreateOperatePanel::Instance().show();
	//GPSceneManager::Instance().InHomeExScene(317);
	//GPHomeRoomListPanel_::Instance().show();
	GPHomeRoomListPanel_TLJ::Instance().show();
}

void GPHomeScene::Button_WZQ(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeRoomListPanel_WZQ::Instance().show();
}

void GPHomeScene::Button_SG(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeRoomListPanel_SG::Instance().show();
}

void GPHomeScene::Button_XZDD(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeRoomListPanel_XZDD::Instance().show();
}

void GPHomeScene::openSubGame(int curPage)
{
	switch (curPage) {
	case 0: {
		//抢庄牛牛
		Button_NN(NULL, NULL);
		break;
	}
	case 1: {
		Button_MenJi(NULL, NULL);//经典三张	
		break;
	}
	case 2: {
		Button_SG(NULL, NULL);//三公	
		break;
	}
	case 3: {
		Button_SRLF(NULL, NULL);//三人两房
		break;
	}
	case 4: {
		Button_WZQ(NULL, NULL);//五子棋
		break;
	}
	case 5: {
		Button_XZDD(NULL, NULL);//血战换三张麻将
		break;
	}
	default:
		//Button_NCCP(NULL,NULL);//南昌厂牌
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

void GPHomeScene::Button_Invitation(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RoomID"));
	//dword roomNO = NNRoomInfo::Instance().getRoomInfo().dwRoomNum;
	std::string strUrl = utility::a_u8(utility::toString("http://114.115.164.158:8080/evokeapp.html?refresh=0&room_id=", iRecordChildID));
	std::string strTitle = ScriptData<std::string>("CCWeiXinSharTitle").Value();
	std::string stDes = utility::getScriptReplaceValue("CCWeiXinSharDes",
		ScriptData<std::string>("CCWeiXinShare_Server_Name").Value(),
		(int)iRecordChildID,
		NNRoomInfo::Instance().getRoomInfoView(true));

	MissionWeiXin::Instance().shareUrlWeiXin(strUrl, strTitle, stDes);
}

void GPHomeScene::Button_Join(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RoomID"));
	//DONE:显示输入房号面板
	GPHomeEnterRoomPanel::Instance().show();
}

void GPHomeScene::Button_CreateGame(cocos2d::Ref*, WidgetUserInfo*)
{
	GPHomeCreateRoomPanel_NN::Instance().show();
}

void GPHomeScene::Button_RoomList_Join(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RoomID"));
	GPGameLink::Instance().JoinRoom(utility::toString(iRecordChildID));
}
