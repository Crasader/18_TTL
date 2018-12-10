#include "NNDismissRoom.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "Game/NN/NNGameScene.h"

#include "UTILITY.h"

FV_SINGLETON_STORAGE(NNDismissRoom);
NNDismissRoom::NNDismissRoom()
{
    init();
}

NNDismissRoom::~NNDismissRoom()
{

}

bool NNDismissRoom::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();
    initData();

	hide();

    return true;
}

void NNDismissRoom::initLayout()
{
    WidgetScenceXMLparse dismissRoom("TTLNN/Script/NNDismissRoom.xml", this);
}

void NNDismissRoom::initButton()
{
    WidgetManager::addButtonCB("NNDismissRoom_ButtonConfirm", this, button_selector(NNDismissRoom::Button_Confirm));
    WidgetManager::addButtonCB("NNDismissRoom_ButtonRefuse", this, button_selector(NNDismissRoom::Button_Refuse));
    WidgetManager::addButtonCB("NNDismissRoom_ButtonClose", this, button_selector(NNDismissRoom::Button_Close));
}

void NNDismissRoom::initData()
{
    m_LabelConfig.fontFilePath = "Font/TTLNN.TTF";
    m_LabelConfig.fontSize = 32;
    m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
    m_LabelConfig.customGlyphs = nullptr;
    m_LabelConfig.distanceFieldEnabled = false;
    m_LabelConfig.outlineSize = 0;
}

void NNDismissRoom::show(NN_DismissRoom_Type type, CMD_GF_Private_Dismiss_Info* pInfo)
{
    setVisible(true);

	m_DismissRoomType = type;

    auto pNode = WidgetFun::getChildWidget(this, "NNDismissRoom_ContentNode");
    pNode->removeAllChildren();

    if(type == NN_DismissRoom_HostBeforeStart) {
        WidgetFun::setVisible(this, "NNDismissRoom_ButtonConfirm", true);
        WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", true);

        std::string text = script::getStr(utility::toString("DisMissRoom_Ask"));
        auto label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 800);
        label->setLineSpacing(20);
        label->setPosition(Point(640, 360));
        label->setColor(cocos2d::Color3B(255,255,255));
        pNode->addChild(label);
    } else if(type == NN_DismissRoom_InGameSelf) {
        if(pInfo != nullptr) {
            WidgetFun::setVisible(this, "NNDismissRoom_ButtonConfirm", false);
            WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", false);
		} else {
			WidgetFun::setVisible(this, "NNDismissRoom_ButtonConfirm", true);
			WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", true);
		}

        std::string text = script::getStr(utility::toString("DisMissRoom_Wait"));
        auto label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 800);
        label->setLineSpacing(20);
        label->setPosition(Point(640, 400));
        label->setColor(cocos2d::Color3B(255,255,255));
        pNode->addChild(label);

    } else {
        std::string text = script::getStr(utility::toString("DisMissRoom_Wait"));

        if(pInfo != nullptr) {
            text = utility::toString("玩家 ", NNGameScene::Instance().getPlayerByChairID(pInfo->dwDissChairID[0])->GetNickName(), " ", text);
        }

        auto label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 800);
        label->setLineSpacing(20);
        label->setPosition(Point(640, 400));
        label->setColor(cocos2d::Color3B(255,255,255));
        pNode->addChild(label);

        bool isPlayerChosed = false;

        for(dword index = 0; pInfo != nullptr && index < pInfo->dwDissUserCout; ++index) {
            if(pInfo->dwDissChairID[index] == NNGameScene::Instance().getLocalPlayer()->GetChairID()) {
                isPlayerChosed = true;
                break;
            }
        }

        for(dword index = 0; !isPlayerChosed && pInfo != nullptr && index < pInfo->dwNotAgreeUserCout; ++index) {
            if(pInfo->dwNotAgreeChairID[index] == NNGameScene::Instance().getLocalPlayer()->GetChairID()) {
                isPlayerChosed = true;
                break;
            }
        }

        if(isPlayerChosed) {
            WidgetFun::setVisible(this, "NNDismissRoom_ButtonConfirm", false);
            WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", false);
        } else {
            WidgetFun::setVisible(this, "NNDismissRoom_ButtonConfirm", true);
            WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", true);
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

	for (int chairIndex = 0; chairIndex < NNGameScene::MAX_PLAYER; chairIndex++) {
		NNPlayer* player = NNGameScene::Instance().getPlayerByChairID(chairIndex);
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

void NNDismissRoom::hide()
{
    setVisible(false);
}

void NNDismissRoom::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
    GPGameLink::Instance().DismissRoom(true);
}

void NNDismissRoom::Button_Refuse(cocos2d::Ref*, WidgetUserInfo*)
{
	if (m_DismissRoomType == NN_DismissRoom_HostBeforeStart) {
		hide();
	} else {
		GPGameLink::Instance().DismissRoom(false);
	}
}

void NNDismissRoom::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
    hide();
}


