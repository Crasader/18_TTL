#include "GPHomeMallPanel.h"
#include "Game/Widget/ListViewEx.h"
#include "Game/Game/UserInfo.h"
#include "GameLib/JniCross/JniFun.h"
#include "GameLib/Tools/core/MD5.h"

FV_SINGLETON_STORAGE(GPHomeMallPanel);
GPHomeMallPanel::GPHomeMallPanel()
{
	init();
}

GPHomeMallPanel::~GPHomeMallPanel()
{

}

bool GPHomeMallPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initButton();
	return true;
}

void GPHomeMallPanel::initLayout()
{
	WidgetScenceXMLparse mallPanel("GamePlaza/Script/GPHomeMallPanel.xml", this);
}

void GPHomeMallPanel::initButton()
{
	WidgetManager::addButtonCB("Mall_ButtonClose", this, button_selector(GPHomeMallPanel::Button_Close));
	WidgetManager::addButtonCB("MallContentDetail_Button_1", this, button_selector(GPHomeMallPanel::Button_Buy));
	WidgetManager::addButtonCB("MallContentDetail_Button_2", this, button_selector(GPHomeMallPanel::Button_Buy));
	WidgetManager::addButtonCB("MallContentDetail_Button_3", this, button_selector(GPHomeMallPanel::Button_Buy));
	WidgetManager::addButtonCB("MallContentDetail_Button_4", this, button_selector(GPHomeMallPanel::Button_Buy));
	WidgetManager::addButtonCB("MallContentDetail_Button_5", this, button_selector(GPHomeMallPanel::Button_Buy));
	WidgetManager::addButtonCB("MallContentDetail_Button_6", this, button_selector(GPHomeMallPanel::Button_Buy));
}

void GPHomeMallPanel::show(bool isDiamond)
{
	setVisible(true);
}

void GPHomeMallPanel::hide()
{
	setVisible(false);
}

void GPHomeMallPanel::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

void GPHomeMallPanel::Button_Buy(cocos2d::Ref* pRef, WidgetUserInfo* pInfo)
{
	//TODO:发送购买请求到原生代码层
	int amount = utility::parseInt(WidgetFun::getWidgetUserInfo(this,pInfo->kName,"amount"));
    //WeiXinPayInfo kWeiXinPayInfo;
    //kWeiXinPayInfo.appid = "wx26ebc7a3307ea22a";
    //kWeiXinPayInfo.attach = "attach";//附加数据
    //kWeiXinPayInfo.body = "APPzhifu";//商品描述
    //kWeiXinPayInfo.mch_id = "1374879202";//商户号
    //kWeiXinPayInfo.nonce_str = kTimeStr;//随机字符串
    //kWeiXinPayInfo.notify_url = "182.110.11.40";
    //kWeiXinPayInfo.out_trade_no = kTimeStr;//商户订单号
    //kWeiXinPayInfo.spbill_create_ip = "182.110.11.40";
    //kWeiXinPayInfo.total_fee = "1";//总金额
    //kWeiXinPayInfo.trade_type = "APP";
    //kWeiXinPayInfo.key = "bosengame12345679123456789123456";//微信商户密钥设置

    std::string appID = "0000001168";
    std::string appKey = "23c700ceb48401216f52ef136dba6a9b";
    std::string UserID = utility::toString(UserInfo::Instance().getUserID());

    std::string mchntOrderNo = UserID + utility::toString(time(NULL));

    std::string md5_str = appID + "*" + appKey + "*"  + mchntOrderNo; 
    MD5 md5 = MD5(md5_str);

    std::string extra = md5.hexdigest();
    extra = UserID + "_" + extra;

    std::string notifyUrl = "http://39.108.94.75:8888/";

    std::string gameGold = utility::getScriptString(utility::toString("GameGold"));
    std::string gameAddGold = utility::getScriptString(utility::toString("GameAddGold"));

    cocos2d::log("ON_SELECTED_ITEM_START to do");
}

void GPHomeMallPanel::initListView()
{
    cocos2d::ListViewEx* pListView = WidgetFun::getListViewWidget(this,"MallContentListSkin");
	//pListView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
	//pListView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::NONE);
	////pListView->setGravity(ui::ListView::Gravity::TOP);
	//pListView->setItemsMargin(41.0f);
	pListView->addEventListener((cocos2d::ListViewEx::ccListViewCallback)CC_CALLBACK_2(GPHomeMallPanel::onSelectedItemEvent,this));
}

//UIListView适配函数
void  GPHomeMallPanel::onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
		{
			cocos2d::ui::ListView*  listview = static_cast<cocos2d::ui::ListView* >(pSender);
			//m_selectedItemIndex = listview->getCurSelectedIndex();
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
