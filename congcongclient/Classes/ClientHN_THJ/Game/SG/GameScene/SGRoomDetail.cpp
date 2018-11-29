#include "SGRoomDetail.h"
#include "SGRoomInfo.h"
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"

using namespace SG;

FV_SINGLETON_STORAGE(SGRoomDetail);
SGRoomDetail::SGRoomDetail()
{
    init();
}

SGRoomDetail::~SGRoomDetail()
{

}

bool SGRoomDetail::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();

    return true;
}

void SGRoomDetail::initLayout()
{
    WidgetScenceXMLparse roomDetail("SG/Script/SGRoomDetail.xml", this);
}

void SGRoomDetail::initButton()
{
	WidgetManager::addButtonCB("SGOperator_ButtonClose", this, button_selector(SGRoomDetail::Button_Close));
}

void SGRoomDetail::show()
{
    const CMD_GF_Private_Room_Info& info = SGRoomInfo::Instance().getRoomInfo();

 //   switch(info.bGameTypeIdex) {
 //   case SGGameType_SGBanker: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("牛牛上庄"));
 //   }
 //   break;

 //   case SGGameType_HostBanker: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("固定庄家"));
 //   }
 //   break;

 //   case SGGameType_SnatchBanker: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("自由抢庄"));
 //   }
 //   break;

 //   case SGGameType_SnatchBankerShowCard: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("明牌抢庄"));
 //   }
 //   break;

 //   case SGGameType_AllCompare: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("通比牛牛"));
 //   }
 //   break;
 //   }

	//std::string scoreText = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Score_0))){
	//	scoreText.append("1");
	//	if (info.bGameTypeIdex != TTLSG::SGGameType_AllCompare) {
	//		scoreText.append("/2");
	//	}
	//} else if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Score_1))){
	//	scoreText.append("2");
	//	if (info.bGameTypeIdex != TTLSG::SGGameType_AllCompare) {
	//		scoreText.append("/4");
	//	}
	//} else if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Score_2))){
	//	scoreText.append("4");
	//	if (info.bGameTypeIdex != TTLSG::SGGameType_AllCompare) {
	//		scoreText.append("/8");
	//	}
	//}
	//WidgetFun::setText(this, "SGRoomDetail_ScoreInfo", scoreText);

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Ratio_0))) {
	//	WidgetFun::setText(this, "SGRoomDetail_RatioRule", utility::a_u8("牛牛x4 牛九x3 牛八x2 牛七x2"));
	//} else {
	//	WidgetFun::setText(this, "SGRoomDetail_RatioRule", utility::a_u8("牛几赔几"));
	//}

	//std::string roomRuleText = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Pay_0))){
	//	roomRuleText.append(utility::a_u8("房主支付"));
	//} else {
	//	roomRuleText.append(utility::a_u8("AA支付"));
	//}
	//roomRuleText.append(" ");

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_ExpertSetting_1))) {
	//	roomRuleText.append(utility::a_u8("闲家推注 "));
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_ExpertSetting_2))) {
	//	roomRuleText.append(utility::a_u8("游戏开始后禁止进入"));
	//}
	//WidgetFun::setText(this, "SGRoomDetail_RoomRule", roomRuleText);
	//
	//std::string specialRule = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_SpecialRule_1))) {
	//	specialRule.append(utility::a_u8("五花牛"));
	//	if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Ratio_0))) {
	//		specialRule.append(utility::a_u8("(5倍) "));
	//	} else{
	//		specialRule.append(utility::a_u8("(12倍) "));
	//	}
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_SpecialRule_2))) {
	//	specialRule.append(utility::a_u8("炸弹牛"));
	//	if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Ratio_0))) {
	//		specialRule.append(utility::a_u8("(6倍) "));
	//	} else{
	//		specialRule.append(utility::a_u8("(15倍) "));
	//	}
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_SpecialRule_3))) {
	//	specialRule.append(utility::a_u8("五小牛(8倍)"));
	//}
	//WidgetFun::setText(this, "SGRoomDetail_SpecialRule", specialRule);

    setVisible(true);
}

void SGRoomDetail::hide()
{
    setVisible(false);
}

void SGRoomDetail::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
}

