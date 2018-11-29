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
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("ţţ��ׯ"));
 //   }
 //   break;

 //   case SGGameType_HostBanker: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("�̶�ׯ��"));
 //   }
 //   break;

 //   case SGGameType_SnatchBanker: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("������ׯ"));
 //   }
 //   break;

 //   case SGGameType_SnatchBankerShowCard: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("������ׯ"));
 //   }
 //   break;

 //   case SGGameType_AllCompare: {
 //       WidgetFun::setText(this, "SGRoomDetail_BankerRule", utility::a_u8("ͨ��ţţ"));
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
	//	WidgetFun::setText(this, "SGRoomDetail_RatioRule", utility::a_u8("ţţx4 ţ��x3 ţ��x2 ţ��x2"));
	//} else {
	//	WidgetFun::setText(this, "SGRoomDetail_RatioRule", utility::a_u8("ţ���⼸"));
	//}

	//std::string roomRuleText = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Pay_0))){
	//	roomRuleText.append(utility::a_u8("����֧��"));
	//} else {
	//	roomRuleText.append(utility::a_u8("AA֧��"));
	//}
	//roomRuleText.append(" ");

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_ExpertSetting_1))) {
	//	roomRuleText.append(utility::a_u8("�м���ע "));
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_ExpertSetting_2))) {
	//	roomRuleText.append(utility::a_u8("��Ϸ��ʼ���ֹ����"));
	//}
	//WidgetFun::setText(this, "SGRoomDetail_RoomRule", roomRuleText);
	//
	//std::string specialRule = "";
	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_SpecialRule_1))) {
	//	specialRule.append(utility::a_u8("�廨ţ"));
	//	if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Ratio_0))) {
	//		specialRule.append(utility::a_u8("(5��) "));
	//	} else{
	//		specialRule.append(utility::a_u8("(12��) "));
	//	}
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_SpecialRule_2))) {
	//	specialRule.append(utility::a_u8("ը��ţ"));
	//	if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_Ratio_0))) {
	//		specialRule.append(utility::a_u8("(6��) "));
	//	} else{
	//		specialRule.append(utility::a_u8("(15��) "));
	//	}
	//}

	//if (FvMask::HasAny(info.bGameRuleIdex, _MASK_(TTLSG::SGGameRule_SpecialRule_3))) {
	//	specialRule.append(utility::a_u8("��Сţ(8��)"));
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

