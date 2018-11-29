#include "NNRoomDetail.h"
#include "NNRoomInfo.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"

using namespace TTLNN;

FV_SINGLETON_STORAGE(NNRoomDetail);
NNRoomDetail::NNRoomDetail()
{
    init();
}

NNRoomDetail::~NNRoomDetail()
{

}

bool NNRoomDetail::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();

    return true;
}

void NNRoomDetail::initLayout()
{
    WidgetScenceXMLparse roomDetail("TTLNN/Script/NNRoomDetail.xml", this);
}

void NNRoomDetail::initButton()
{
	WidgetManager::addButtonCB("NNOperator_ButtonClose", this, button_selector(NNRoomDetail::Button_Close));
}

void NNRoomDetail::show()
{
    const CMD_GF_Private_Room_Info& info = NNRoomInfo::Instance().getRoomInfo();

 //   switch(info.bGameTypeIdex) {
 //   case NNGameType_NNBanker: {
 //       WidgetFun::setText(this, "NNRoomDetail_BankerRule", utility::a_u8("牛牛上庄"));
 //   }
 //   break;

 //   case NNGameType_HostBanker: {
 //       WidgetFun::setText(this, "NNRoomDetail_BankerRule", utility::a_u8("固定庄家"));
 //   }
 //   break;

 //   case NNGameType_SnatchBanker: {
 //       WidgetFun::setText(this, "NNRoomDetail_BankerRule", utility::a_u8("自由抢庄"));
 //   }
 //   break;

 //   case NNGameType_SnatchBankerShowCard: {
 //       WidgetFun::setText(this, "NNRoomDetail_BankerRule", utility::a_u8("明牌抢庄"));
 //   }
 //   break;

 //   case NNGameType_AllCompare: {
 //       WidgetFun::setText(this, "NNRoomDetail_BankerRule", utility::a_u8("通比牛牛"));
 //   }
 //   break;
 //   }

	//std::string scoreText = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_0))){
	//	scoreText.append("1");
	//	if (info.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
	//		scoreText.append("/2");
	//	}
	//} else if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_1))){
	//	scoreText.append("2");
	//	if (info.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
	//		scoreText.append("/4");
	//	}
	//} else if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_2))){
	//	scoreText.append("4");
	//	if (info.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
	//		scoreText.append("/8");
	//	}
	//}
	//WidgetFun::setText(this, "NNRoomDetail_ScoreInfo", scoreText);

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_Ratio_0))) {
	//	WidgetFun::setText(this, "NNRoomDetail_RatioRule", utility::a_u8("牛牛x4 牛九x3 牛八x2 牛七x2"));
	//} else {
	//	WidgetFun::setText(this, "NNRoomDetail_RatioRule", utility::a_u8("牛几赔几"));
	//}

	//std::string roomRuleText = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_Pay_0))){
	//	roomRuleText.append(utility::a_u8("房主支付"));
	//} else {
	//	roomRuleText.append(utility::a_u8("AA支付"));
	//}
	//roomRuleText.append(" ");

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_ExpertSetting_1))) {
	//	roomRuleText.append(utility::a_u8("闲家推注 "));
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_ExpertSetting_2))) {
	//	roomRuleText.append(utility::a_u8("游戏开始后禁止进入"));
	//}
	//WidgetFun::setText(this, "NNRoomDetail_RoomRule", roomRuleText);
	//
	//std::string specialRule = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_SpecialRule_1))) {
	//	specialRule.append(utility::a_u8("五花牛"));
	//	if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_Ratio_0))) {
	//		specialRule.append(utility::a_u8("(5倍) "));
	//	} else{
	//		specialRule.append(utility::a_u8("(12倍) "));
	//	}
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_SpecialRule_2))) {
	//	specialRule.append(utility::a_u8("炸弹牛"));
	//	if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_Ratio_0))) {
	//		specialRule.append(utility::a_u8("(6倍) "));
	//	} else{
	//		specialRule.append(utility::a_u8("(15倍) "));
	//	}
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLNN::NNGameRule_SpecialRule_3))) {
	//	specialRule.append(utility::a_u8("五小牛(8倍)"));
	//}
	//WidgetFun::setText(this, "NNRoomDetail_SpecialRule", specialRule);

    setVisible(true);
}

void NNRoomDetail::hide()
{
    setVisible(false);
}

void NNRoomDetail::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

