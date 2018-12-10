#include "SGRoomInfo.h"
#include "SGGameScence.h"
#include "Game/SG/CMD_SG.h"
#include "Game/SG/SGOperator.h"

#include "UTILITY.h"
#include USERINFO

FV_SINGLETON_STORAGE(SGRoomInfo);
SGRoomInfo::SGRoomInfo()
{
    init();
}

SGRoomInfo::~SGRoomInfo()
{

}

#pragma region 初始化
bool SGRoomInfo::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initData();

    return true;
}

void SGRoomInfo::initLayout()
{
	
}

void SGRoomInfo::initData()
{
    m_LabelConfig.fontFilePath = "Font/TTLNN.TTF";
    m_LabelConfig.fontSize = 20;
    m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
    m_LabelConfig.customGlyphs = nullptr;
    m_LabelConfig.distanceFieldEnabled = false;
    m_LabelConfig.outlineSize = 0;
}
#pragma endregion 初始化

#pragma region 显示与隐藏
void SGRoomInfo::show()
{
    setVisible(true);
}

void SGRoomInfo::hide()
{
    setVisible(false);
}
#pragma endregion 显示与隐藏

#pragma region 数据接收与刷新显示
void SGRoomInfo::setRoomInfo(CMD_GF_Private_Room_Info& roomInfo)
{
    m_RoomInfo = roomInfo;
}

void SGRoomInfo::updateRoomInfo()
{
    removeAllChildren();

	auto label = Label::createWithTTF(m_LabelConfig, getRoomInfoText(), TextHAlignment::LEFT);
    label->setLineSpacing(5);
    label->setAnchorPoint(cocos2d::Vec2(0, 1));
    label->setPosition(Point(10, 710));
    addChild(label);

	//if (m_RoomInfo.dwPlayCout == 0) {
	//	SGOperator::Instance().showWeiXin();
	//} else {
	//	SGOperator::Instance().hideWeiXin();
	//}
	//SGOperator::Instance().showWeiXin();
	if (m_RoomInfo.bGameTypeIdex == SG::SGGameType_HostBanker && m_RoomInfo.dwPlayCout > 3 && UserInfo::Instance().getUserID() == m_RoomInfo.dwCreateUserID && SGGameScence::Instance().getGameStatus() < SG::SGGameStatus_Start) {
		SGGameScence::Instance().showDropBanker();
	} else {
		SGGameScence::Instance().hideDropBanker();
	}
}

bool SGRoomInfo::isHostPlayer(SGPlayer& player)
{
	return player.GetUserID() == m_RoomInfo.dwCreateUserID;
}

int SGRoomInfo::getPlayerScore(SGPlayer& player)
{
//     word chairID = player.GetChairID();
// 
//     if(m_RoomInfo.kWinLoseScore.size() > chairID) {
//         return m_RoomInfo.kWinLoseScore.at(player.GetChairID());
//     }

    return player.GetUserScore();
}

std::string SGRoomInfo::getRoomInfoText(bool forShare /*= false*/)
{
    std::string split = (forShare ? " " : "\n");
    std::string text = "";
    //text.append(utility::a_u8("房号:"));
    //text.append(utility::toString(m_RoomInfo.dwRoomNum));
    //text.append(split);
	text.append(utility::a_u8("庄位:"));
	text.append(utility::a_u8("自由抢庄"));
	text.append(split);
	text.append(utility::a_u8("底分:"));
	text.append(utility::toString(m_RoomInfo.dwBaseScore));
	text.append(split);
	text.append(utility::a_u8("局数:"));
	text.append(utility::toString(m_RoomInfo.dwPlayCout));
	text.append(utility::a_u8("/"));
	text.append(utility::toString(m_RoomInfo.dwPlayCout));
	text.append(split);
	
	//text.append(utility::a_u8("局数:"));
	//text.append(utility::toString(m_RoomInfo.dwPlayCout, "/", m_RoomInfo.dwPlayTotal));
	//text.append(split);
	return text;
}

CMD_GF_Private_Room_Info SGRoomInfo::getRoomInfo()
{
	return m_RoomInfo;
}

#pragma endregion 数据接收与刷新显示
