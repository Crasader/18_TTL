#include "NNRoomInfo.h"
#include "NNGameScene.h"
#include "Game/NN/CMD_NN.h"
#include "Game/NN/NNOperator.h"
#include "Plaza/GameManager/GPGameLink.h"

#include "UTILITY.h"
#include USERINFO

FV_SINGLETON_STORAGE(NNRoomInfo);
NNRoomInfo::NNRoomInfo()
{
    init();
}

NNRoomInfo::~NNRoomInfo()
{

}

#pragma region 初始化
bool NNRoomInfo::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initData();

    return true;
}

void NNRoomInfo::initLayout()
{
	
}

void NNRoomInfo::initData()
{
    m_LabelConfig.fontFilePath = "Font/true_type.ttf";
    m_LabelConfig.fontSize = 22;
    m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
    m_LabelConfig.customGlyphs = nullptr;
    m_LabelConfig.distanceFieldEnabled = false;
    m_LabelConfig.outlineSize = 0;
	for (size_t i = 0; i < NN_GAME_PLAYER; i++) {
		m_RoomInfo.kWinLoseScore.push_back(0);
	}
	m_RoomInfo.dwBaseScore = 1;
}
#pragma endregion 初始化

#pragma region 显示与隐藏
void NNRoomInfo::show()
{
    setVisible(true);
}

void NNRoomInfo::hide()
{
    setVisible(false);
}
#pragma endregion 显示与隐藏

#pragma region 数据接收与刷新显示
void NNRoomInfo::setRoomInfo(CMD_GF_Private_Room_Info& roomInfo)
{
	CCLOG("NNRoomInfo::setRoomInfo roomInfo.dwBaseScore=%ld\n",roomInfo.dwBaseScore);
	m_RoomInfo = roomInfo;
}

void NNRoomInfo::updateRoomInfo()
{
    removeAllChildren();

    auto label = Label::createWithTTF(m_LabelConfig, getRoomInfoText(), TextHAlignment::LEFT);
    label->setLineSpacing(3);
    label->setAnchorPoint(cocos2d::Vec2(0, 1));
    label->setPosition(Point(10, 710));
	label->setColor(cocos2d::Color3B(255, 225, 60));
    addChild(label);

	if (m_RoomInfo.bGameTypeIdex == TTLNN::NNGameType_HostBanker &&
		m_RoomInfo.dwPlayCout > 3 &&
		UserInfo::Instance().getUserID() == m_RoomInfo.dwMasterUserID &&
		NNGameScene::Instance().getGameStatus() < TTLNN::NNGameStatus_Start)
	{
		NNGameScene::Instance().showDropBanker();
	} else {
		NNGameScene::Instance().hideDropBanker();
	}
}

bool NNRoomInfo::isCreater(NNPlayer* player)
{
	return player && player->GetUserID() == m_RoomInfo.dwCreateUserID;
}

bool NNRoomInfo::isMaster(NNPlayer* player)
{
	return player && (player->GetUserID() == m_RoomInfo.dwMasterUserID);
}

int NNRoomInfo::getPlayerScore(NNPlayer* player)
{
    return player ? player->GetUserScore() : 0;
}

std::string NNRoomInfo::getRoomInfoView(bool forShare /*= false*/)
{
	std::string resultStr;
	switch (m_RoomInfo.bGameTypeIdex) {
		case TTLNN::NNGameType_NNBanker: {
			resultStr += utility::a_u8("牛牛上庄");
			break;
		}
		case TTLNN::NNGameType_HostBanker: {
			resultStr += utility::a_u8("固定庄家");
			break;
		}
		case TTLNN::NNGameType_SnatchBanker: {
			resultStr += utility::a_u8("自由抢庄");
			break;
		}
		case TTLNN::NNGameType_SnatchBankerShowCard: {
			resultStr += utility::a_u8("明牌抢庄");
			break;
		}
		case TTLNN::NNGameType_AllCompare: {
			resultStr += utility::a_u8("通比牛牛");
			break;
		}
		case TTLNN::NNGameType_NNRatio: {
			resultStr += utility::a_u8("牛几赔几");
			break;
		}
	}
	resultStr += utility::a_u8(" 局数:");
	auto game_info = GPGameLink::Instance().privateGameInfo();
	resultStr += utility::a_u8(utility::toString(game_info.bPlayCout[m_RoomInfo.bPlayCoutIdex]));
	return resultStr;
}

std::string NNRoomInfo::getRoomInfoText(bool forShare /*= false*/)
{
    std::string split = (forShare ? " " : "\n");

    std::string text = "";
    text.append(utility::a_u8("房号:"));
    text.append(utility::toString(m_RoomInfo.dwRoomNum));
    text.append(split);
    text.append(utility::a_u8("庄位:"));
	//text.append(utility::a_u8("明牌抢庄"));

    switch(m_RoomInfo.bGameTypeIdex) {
    case TTLNN::NNGameType_NNBanker: {
        text.append(utility::a_u8("牛牛上庄"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_HostBanker: {
        text.append(utility::a_u8("固定庄家"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_SnatchBanker: {
        text.append(utility::a_u8("自由抢庄"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_SnatchBankerShowCard: {
        text.append(utility::a_u8("明牌抢庄"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_AllCompare: {
        text.append(utility::a_u8("通比牛牛"));
        text.append(split);
    }
    break;

	case TTLNN::NNGameType_NNRatio: {
		text.append(utility::a_u8("牛几赔几"));
		text.append(split);
	}
	break;

    default:
        break;
    }

	text.append(utility::a_u8("底分:"));
	if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_0))){
		text.append("1");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			text.append("/2");
		}
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_1))){
		text.append("2");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			text.append("/4");
		}
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Score_2))){
		text.append("4");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			text.append("/8");
		}
	} else {
		text.append("5");
		if (m_RoomInfo.bGameTypeIdex != TTLNN::NNGameType_AllCompare) {
			text.append("/10");
		}
	}
	text.append(split);
	auto game_info = GPGameLink::Instance().privateGameInfo();
	text.append(utility::a_u8("局数:"));
	text.append(utility::toString(m_RoomInfo.dwPlayCout));
	text.append(utility::a_u8("/"));
	text.append(utility::toString(game_info.bPlayCout[m_RoomInfo.bPlayCoutIdex]));
	text.append(split);

	//牛牛4倍
	if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Ratio_0))) {
		text.append(utility::a_u8("牛牛4倍"));
	} else {
		text.append(utility::a_u8("牛牛3倍"));
	}
	text.append(split);

	if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZ))) {
		text.append(utility::a_u8("闲家推注"));
	}
	text.append(split);

	if (m_RoomInfo.bGameTypeIdex == TTLNN::NNGameType_HostBanker) {
		text.append(utility::a_u8("庄家分数:"));
		if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_BankerScore_0))){
			text.append(utility::a_u8("不限"));
		} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_BankerScore_1))){
			text.append(utility::toString(100 + m_RoomInfo.kWinLoseScore.at(0), "/100"));
		} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_BankerScore_2))){
			text.append(utility::toString(150 + m_RoomInfo.kWinLoseScore.at(0), "/150"));
		} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_BankerScore_3))){
			text.append(utility::toString(200 + m_RoomInfo.kWinLoseScore.at(0), "/200"));
		}	
	}

	return text;
}

CMD_GF_Private_Room_Info NNRoomInfo::getRoomInfo()
{
	return m_RoomInfo;
}

void NNRoomInfo::setRoomInfoBase(dword dwBaseScore)
{
	m_RoomInfo.dwBaseScore = dwBaseScore;
}

#pragma endregion 数据接收与刷新显示
