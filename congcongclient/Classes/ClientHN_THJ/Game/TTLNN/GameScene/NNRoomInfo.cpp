#include "NNRoomInfo.h"
#include "NNGameScene.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"
#include "ClientHN_THJ/Game/TTLNN/Operator/NNOperator.h"
#include "GamePlaza/GameManager/GPGameLink.h"

FV_SINGLETON_STORAGE(NNRoomInfo);
NNRoomInfo::NNRoomInfo()
{
    init();
}

NNRoomInfo::~NNRoomInfo()
{

}

#pragma region ��ʼ��
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
    m_LabelConfig.fontFilePath = "Font/TTLNN.TTF";
    m_LabelConfig.fontSize = 20;
    m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
    m_LabelConfig.customGlyphs = nullptr;
    m_LabelConfig.distanceFieldEnabled = false;
    m_LabelConfig.outlineSize = 0;
	for (size_t i = 0; i < NN_GAME_PLAYER; i++) {
		m_RoomInfo.kWinLoseScore.push_back(0);
	}
}
#pragma endregion ��ʼ��

#pragma region ��ʾ������
void NNRoomInfo::show()
{
    setVisible(true);
}

void NNRoomInfo::hide()
{
    setVisible(false);
}
#pragma endregion ��ʾ������

#pragma region ���ݽ�����ˢ����ʾ
void NNRoomInfo::setRoomInfo(CMD_GF_Private_Room_Info& roomInfo)
{
	printf("������Ϣ��ע��Ϣ%u\n",roomInfo.dwBaseScore);
	m_RoomInfo = roomInfo;
}

void NNRoomInfo::updateRoomInfo()
{
    removeAllChildren();

    auto label = Label::createWithTTF(m_LabelConfig, getRoomInfoText(), TextHAlignment::LEFT);
    label->setLineSpacing(5);
    label->setAnchorPoint(cocos2d::Vec2(0, 1));
    label->setPosition(Point(10, 710));
    addChild(label);
	//NNOperator::Instance().showWeiXin();
	//if (m_RoomInfo.dwPlayCout == 0) {
	//	NNOperator::Instance().showWeiXin();
	//} else {
	//	NNOperator::Instance().hideWeiXin();
	//}

	if (m_RoomInfo.bGameTypeIdex == TTLNN::NNGameType_HostBanker &&
		m_RoomInfo.dwPlayCout > 3 &&
		UserInfo::Instance().getUserID() == m_RoomInfo.dwCreateUserID &&
		NNGameScene::Instance().getGameStatus() < TTLNN::NNGameStatus_Start) {
		NNGameScene::Instance().showDropBanker();
	} else {
		NNGameScene::Instance().hideDropBanker();
	}
}

bool NNRoomInfo::isCreaterPlayer(NNPlayer* player)
{
	return player && player->GetUserID() == m_RoomInfo.dwCreateUserID;
}

bool NNRoomInfo::isHostPlayer(NNPlayer* player)
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
			resultStr += utility::a_u8("ţţ��ׯ");
			break;
		}
		case TTLNN::NNGameType_HostBanker: {
			resultStr += utility::a_u8("�̶�ׯ��");
			break;
		}
		case TTLNN::NNGameType_SnatchBanker: {
			resultStr += utility::a_u8("������ׯ");
			break;
		}
		case TTLNN::NNGameType_SnatchBankerShowCard: {
			resultStr += utility::a_u8("������ׯ");
			break;
		}
		case TTLNN::NNGameType_AllCompare: {
			resultStr += utility::a_u8("ͨ��ţţ");
			break;
		}
		case TTLNN::NNGameType_NNRatio: {
			resultStr += utility::a_u8("ţ���⼸");
			break;
		}
	}
	resultStr += utility::a_u8(" ����:");
	auto game_info = GPGameLink::Instance().getCurrentGameInfo();
	resultStr += utility::a_u8(utility::toString(game_info.bPlayCout[m_RoomInfo.bPlayCoutIdex]));
	return resultStr;
}

std::string NNRoomInfo::getRoomInfoText(bool forShare /*= false*/)
{
    std::string split = (forShare ? " " : "\n");

    std::string text = "";
    text.append(utility::a_u8("����:"));
    text.append(utility::toString(m_RoomInfo.dwRoomNum));
    text.append(split);
    text.append(utility::a_u8("ׯλ:"));
	//text.append(utility::a_u8("������ׯ"));

    switch(m_RoomInfo.bGameTypeIdex) {
    case TTLNN::NNGameType_NNBanker: {
        text.append(utility::a_u8("ţţ��ׯ"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_HostBanker: {
        text.append(utility::a_u8("�̶�ׯ��"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_SnatchBanker: {
        text.append(utility::a_u8("������ׯ"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_SnatchBankerShowCard: {
        text.append(utility::a_u8("������ׯ"));
        text.append(split);
    }
    break;

    case TTLNN::NNGameType_AllCompare: {
        text.append(utility::a_u8("ͨ��ţţ"));
        text.append(split);
    }
    break;

	case TTLNN::NNGameType_NNRatio: {
		text.append(utility::a_u8("ţ���⼸"));
		text.append(split);
	}
	break;

    default:
        break;
    }

	text.append(utility::a_u8("�׷�:"));
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
	auto game_info = GPGameLink::Instance().getCurrentGameInfo();
	text.append(utility::a_u8("����:"));
	text.append(utility::toString(m_RoomInfo.dwPlayCout));
	text.append(utility::a_u8("/"));
	text.append(utility::toString(game_info.bPlayCout[m_RoomInfo.bPlayCoutIdex]));
	text.append(split);

	//ţţ4��
	if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_Ratio_0))) {
		text.append(utility::a_u8("ţţ4��"));
	} else {
		text.append(utility::a_u8("ţţ3��"));
	}
	text.append(split);

	if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_0))) {
		text.append(utility::a_u8(utility::toString(TuiZhuBeiShu_0, "����ע")));
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_1))) {
		text.append(utility::a_u8(utility::toString(TuiZhuBeiShu_1, "����ע")));
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_2))) {
		text.append(utility::a_u8(utility::toString(TuiZhuBeiShu_2, "����ע")));
	} else if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_TZRatio_3))) {
		text.append(utility::a_u8(utility::toString(TuiZhuBeiShu_3, "����ע")));
	}
	text.append(split);

	if (m_RoomInfo.bGameTypeIdex == TTLNN::NNGameType_HostBanker) {
		text.append(utility::a_u8("ׯ�ҷ���:"));
		if (FvMask::HasAny(m_RoomInfo.dwGameRuleIdex, _MASK_(TTLNN::NNGameRule_BankerScore_0))){
			text.append(utility::a_u8("����"));
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

#pragma endregion ���ݽ�����ˢ����ʾ
