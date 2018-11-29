#include "SGDismissRoom.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/SG/GameScene/SGGameScence.h"

FV_SINGLETON_STORAGE(SGDismissRoom);
SGDismissRoom::SGDismissRoom()
{
    init();
}

SGDismissRoom::~SGDismissRoom()
{

}

bool SGDismissRoom::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();
    initData();

    return true;
}

void SGDismissRoom::initLayout()
{
    WidgetScenceXMLparse dismissRoom("SG/Script/SGDismissRoom.xml", this);
}

void SGDismissRoom::initButton()
{
    WidgetManager::addButtonCB("SGDismissRoom_ButtonConfirm", this, button_selector(SGDismissRoom::Button_Confirm));
    WidgetManager::addButtonCB("SGDismissRoom_ButtonRefuse", this, button_selector(SGDismissRoom::Button_Refuse));
    WidgetManager::addButtonCB("SGDismissRoom_ButtonClose", this, button_selector(SGDismissRoom::Button_Close));
}

void SGDismissRoom::initData()
{
    m_LabelConfig.fontFilePath = "Font/TTLNN.TTF";
    m_LabelConfig.fontSize = 32;
    m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
    m_LabelConfig.customGlyphs = nullptr;
    m_LabelConfig.distanceFieldEnabled = false;
    m_LabelConfig.outlineSize = 0;
}

void SGDismissRoom::show(SG_DismissRoom_Type type, CMD_GF_Private_Dismiss_Info* pInfo)
{
    setVisible(true);

	m_DismissRoomType = type;

    auto pNode = WidgetFun::getChildWidget(this, "SGDismissRoom_ContentNode");
    pNode->removeAllChildren();

    if(type == SG_DismissRoom_HostBeforeStart) {
        WidgetFun::setVisible(this, "SGDismissRoom_ButtonConfirm", true);
        WidgetFun::setVisible(this, "SGDismissRoom_ButtonRefuse", true);

        std::string text = "游戏尚未开始，解散房间将不会扣除钻石，确定解散房间吗？";
        auto label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 800);
        label->setLineSpacing(20);
        label->setPosition(Point(640, 360));
        label->setColor(cocos2d::Color3B(255,255,255));
        pNode->addChild(label);
    } else if(type == SG_DismissRoom_InGameSelf) {
        if(pInfo != nullptr) {
            WidgetFun::setVisible(this, "SGDismissRoom_ButtonConfirm", false);
            WidgetFun::setVisible(this, "SGDismissRoom_ButtonRefuse", false);
		} else {
			WidgetFun::setVisible(this, "SGDismissRoom_ButtonConfirm", true);
			WidgetFun::setVisible(this, "SGDismissRoom_ButtonRefuse", true);
		}

        std::string text = "玩家申请解散房间，等待其他玩家做出选择，超出五分钟未做选择，则默认同意，房间自动解散。";
        auto label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 800);
        label->setLineSpacing(20);
        label->setPosition(Point(640, 400));
        label->setColor(cocos2d::Color3B(255,255,255));
        pNode->addChild(label);
    } else {
        std::string text = "申请解散房间，等待其他玩家做出选择，超出五分钟未做选择，则默认同意，房间自动解散。";

        if(pInfo != nullptr) {
            text = utility::toString("玩家 ", SGGameScence::Instance().getPlayerByChairID(pInfo->dwDissChairID[0])->GetNickName(), " ", text);
        }

        auto label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 800);
        label->setLineSpacing(20);
        label->setPosition(Point(640, 400));
        label->setColor(cocos2d::Color3B(255,255,255));
        pNode->addChild(label);

        bool isPlayerChosed = false;

        for(size_t index = 0; pInfo != nullptr && index < pInfo->dwDissUserCout; ++index) {
            if(pInfo->dwDissChairID[index] == SGGameScence::Instance().getLocalPlayer()->GetChairID()) {
                isPlayerChosed = true;
                break;
            }
        }

        for(size_t index = 0; !isPlayerChosed && pInfo != nullptr && index < pInfo->dwNotAgreeUserCout; ++index) {
            if(pInfo->dwNotAgreeChairID[index] == SGGameScence::Instance().getLocalPlayer()->GetChairID()) {
                isPlayerChosed = true;
                break;
            }
        }

        if(isPlayerChosed) {
            WidgetFun::setVisible(this, "SGDismissRoom_ButtonConfirm", false);
            WidgetFun::setVisible(this, "SGDismissRoom_ButtonRefuse", false);
        } else {
            WidgetFun::setVisible(this, "SGDismissRoom_ButtonConfirm", true);
            WidgetFun::setVisible(this, "SGDismissRoom_ButtonRefuse", true);
        }
    }

	if (pInfo == nullptr) {
		return;
	}

	auto startPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode, "StartPos"));
	auto addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode, "AddPos"));
	auto addLinePos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode, "AddLinePos"));

	TTFConfig labelConfig;
	labelConfig.fontFilePath = "Font/TTLNN.TTF";
	labelConfig.fontSize = 28;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;
	labelConfig.outlineSize = 0;

	for (int chairIndex = 0; chairIndex < SGGameScence::MAX_PLAYER; chairIndex++) {
		SGPlayer* player = SGGameScence::Instance().getPlayerByChairID(chairIndex);
		if (player->isValid()) {
			bool isPlayerChosed = false;

			std::string text = utility::a_u8("待确认");
			for (int index = 0; index < pInfo->dwDissUserCout; ++index) {
				if (pInfo->dwDissChairID[index] == chairIndex) {
					isPlayerChosed = true;
					text = utility::a_u8("同意");
					break;
				}
			}

			for (int index = 0; !isPlayerChosed && index < pInfo->dwNotAgreeUserCout; ++index) {
				if (pInfo->dwNotAgreeChairID[index] == chairIndex) {
					isPlayerChosed = true;
					text = utility::a_u8("拒绝");
					break;
				}
			}

			std::string temp = player->GetNickName();
			text = temp.append(":").append(text);
			auto label = Label::createWithTTF(labelConfig, utility::a_u8(text));
			label->setAnchorPoint(cocos2d::Vec2(0, 0.5));
			label->setPosition(startPos + addPos * (chairIndex % 3) + addLinePos * (chairIndex / 3));
			label->setColor(cocos2d::Color3B(255,255,255));
			pNode->addChild(label);
		}
	}
}

void SGDismissRoom::hide()
{
    setVisible(false);
}

void SGDismissRoom::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
    GPGameLink::Instance().DismissRoom(true);
}

void SGDismissRoom::Button_Refuse(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_DismissRoomType == SG_DismissRoom_HostBeforeStart) {
		hide();
	} else {
		GPGameLink::Instance().DismissRoom(false);
	}
}

void SGDismissRoom::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
    hide();
}


