#include "NNDismissRoom.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "Game/NN/NNGameScene.h"
#include "UTILITY.h"

FV_SINGLETON_STORAGE(NNDismissRoom);
NNDismissRoom::NNDismissRoom()
	: _fPassBegin(0.f)
	, _fPassEnd(0.f)
	, _nDismissCounter(0)
{
	_dismiss_info = new CMD_GF_Private_Dismiss_Info();
	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(NNDismissRoom::update, this), this, 0.0f, false, "GameManagerBase");
    init();
}

NNDismissRoom::~NNDismissRoom()
{
	if (_dismiss_info)
		delete _dismiss_info;
	_dismiss_info = nullptr;
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
	WidgetManager::addButtonCB("NNDismissRoom_ButtonOK", this, button_selector(NNDismissRoom::Button_OK));
    WidgetManager::addButtonCB("NNDismissRoom_ButtonAgree", this, button_selector(NNDismissRoom::Button_Agree));
    WidgetManager::addButtonCB("NNDismissRoom_ButtonRefuse", this, button_selector(NNDismissRoom::Button_Refuse));
	WidgetManager::addButtonCB("NNDismissRoom_ButtonCancle", this, button_selector(NNDismissRoom::Button_Cancle));
    //WidgetManager::addButtonCB("NNDismissRoom_ButtonClose", this, button_selector(NNDismissRoom::Button_Close));
}

void NNDismissRoom::initData()
{
}

void NNDismissRoom::update(float ftime)
{
	_fPassEnd += ftime;
	if (static_cast<int>(_fPassEnd - _fPassBegin) < 1) {
		return;
	} else {
		_fPassBegin = _fPassEnd;
	}

	switch (_eDismissRoomType)
	{
	case NN_DismissRoom_None:
		break;
	case NN_DismissRoom_HostBeforeStart:
		hostBeforeStart();
		break;
	case NN_DismissRoom_ApplyForDismiss:
		applyForDismiss();
		break;
	case NN_DismissRoom_ReplyDismiss:
		_nDismissCounter--;
		if (_nDismissCounter < 0) {
			_nDismissCounter = 0;
			_eDismissRoomType = NN_DismissRoom_None;
		}
		replyDismiss();
		break;
	default:
		break;
	}
}

void NNDismissRoom::hostBeforeStart()
{
	auto pNode = WidgetFun::getChildWidget(this, "NNDismissRoom_ContentNode");
	std::string text = script::getStr("DisMissRoom_Ask");
	WidgetFun::setVisible(this, "NNDismissRoom_ButtonAgree", true);
	WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", true);
	WidgetFun::setVisible(this, "NNDismissRoom_ButtonCancle", false);

	TTFConfig m_LabelConfig;
	m_LabelConfig.fontFilePath = "Font/true_type.ttf";
	m_LabelConfig.fontSize = 36;
	m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
	m_LabelConfig.customGlyphs = nullptr;
	m_LabelConfig.distanceFieldEnabled = false;
	m_LabelConfig.outlineSize = 0;

	cocos2d::Label* label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 800);
	label->setLineSpacing(20);
	label->setPosition(Point(640, 560));
	label->setColor(cocos2d::Color3B(255, 255, 255));

	pNode->removeAllChildren();
	pNode->addChild(label);
}

void NNDismissRoom::applyForDismiss()
{
	auto pNode = WidgetFun::getChildWidget(this, "NNDismissRoom_ContentNode");
	pNode->removeAllChildren();

	std::string text = utility::getScriptReplaceValue("DisMissRoom_Wait_1", " ");

	WidgetFun::setVisible(this, "NNDismissRoom_ButtonOK", true);
	WidgetFun::setVisible(this, "NNDismissRoom_ButtonCancle", true);

	WidgetFun::setVisible(this, "NNDismissRoom_ButtonAgree", false);
	WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", false);

	TTFConfig m_LabelConfig;
	m_LabelConfig.fontFilePath = "Font/true_type.ttf";
	m_LabelConfig.fontSize = 48;
	m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
	m_LabelConfig.customGlyphs = nullptr;
	m_LabelConfig.distanceFieldEnabled = false;
	m_LabelConfig.outlineSize = 0;

	cocos2d::Label* label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 1000);
	label->setLineSpacing(20);
	label->setPosition(Point(640, 360));
	label->setColor(cocos2d::Color3B(158, 88, 52));

	pNode->removeAllChildren();
	pNode->addChild(label);
}

void NNDismissRoom::replyDismiss()
{
	auto pNode = WidgetFun::getChildWidget(this, "NNDismissRoom_ContentNode");
	auto player = NNGameScene::Instance().getPlayerByChairID(_dismiss_info->dwDissChairID[0]);
	std::string text;
	if (player) {
		std::string name = utility::a_u8("玩家【") + player->GetNickName() + utility::a_u8("】");
		text = utility::getScriptReplaceValue("DisMissRoom_Wait_1", name);
	} else {
		text = utility::getScriptReplaceValue("DisMissRoom_Wait_1", "");
	}

	TTFConfig m_LabelConfig;
	m_LabelConfig.fontFilePath = "Font/true_type.ttf";
	m_LabelConfig.fontSize = 36;
	m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
	m_LabelConfig.customGlyphs = nullptr;
	m_LabelConfig.distanceFieldEnabled = false;
	m_LabelConfig.outlineSize = 0;

	auto label = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 1000);
	label->setLineSpacing(10);
	label->setPosition(Point(640, 540));
	label->setColor(cocos2d::Color3B(158, 88, 52));

	m_LabelConfig.fontSize = 36;
	m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
	m_LabelConfig.customGlyphs = nullptr;
	m_LabelConfig.distanceFieldEnabled = false;
	m_LabelConfig.outlineSize = 0;

	text = ScriptData<std::string>("DisMissRoom_Wait_2").Value();
	auto label2 = Label::createWithTTF(m_LabelConfig, utility::a_u8(text), TextHAlignment::LEFT, 1000);
	label2->setLineSpacing(10);
	label2->setPosition(Point(640, 445));
	label2->setColor(cocos2d::Color3B(224, 21, 4));

	m_LabelConfig.fontSize = 64;
	m_LabelConfig.glyphs = GlyphCollection::DYNAMIC;
	m_LabelConfig.customGlyphs = nullptr;
	m_LabelConfig.distanceFieldEnabled = false;
	m_LabelConfig.outlineSize = 1;

	auto label3 = Label::createWithTTF(m_LabelConfig, utility::a_u8(utility::toString(_nDismissCounter, "秒")), TextHAlignment::LEFT, 800);
	label3->setLineSpacing(20);
	label3->setPosition(Point(640, 360));
	label3->setColor(cocos2d::Color3B(224, 21, 4));

	pNode->removeAllChildren();
	pNode->addChild(label);
	pNode->addChild(label2);
	pNode->addChild(label3);

	bool isPlayerChosed = false;
	auto local = NNGameScene::Instance().getSelf();

	for (dword index = 0; _dismiss_info != nullptr && index < _dismiss_info->dwDissUserCout; ++index) {
		if (local && _dismiss_info->dwDissChairID[index] == local->GetChairID()) {
			isPlayerChosed = true;
			break;
		}
	}

	for (dword index = 0; !isPlayerChosed && _dismiss_info != nullptr && index < _dismiss_info->dwNotAgreeUserCout; ++index) {
		if (local && _dismiss_info->dwNotAgreeChairID[index] == local->GetChairID()) {
			isPlayerChosed = true;
			break;
		}
	}

	if (isPlayerChosed) {
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonOK", false);
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonAgree", false);
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", false);
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonCancle", false);
	} else {
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonAgree", true);
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonRefuse", true);
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonOK", false);
		WidgetFun::setVisible(this, "NNDismissRoom_ButtonCancle", false);
	}

	auto startPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode, "StartPos"));
	auto addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode, "AddPos"));
	auto addLinePos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNode, "AddLinePos"));

	TTFConfig labelConfig;
	labelConfig.fontFilePath = "Font/true_type.ttf";
	labelConfig.fontSize = 28;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;
	labelConfig.outlineSize = 0;

	for (int chairIndex = 0; chairIndex < NNGameScene::MAX_PLAYER; chairIndex++) {
		NNPlayer* player = NNGameScene::Instance().getPlayerByChairID(chairIndex);
		if (player->isValid()) {
			bool isPlayerChosed = false;

			std::string text = "待确认";
			for (dword index = 0; index < _dismiss_info->dwDissUserCout; ++index) {
				if (_dismiss_info->dwDissChairID[index] == chairIndex) {
					isPlayerChosed = true;
					text = "同意";
					break;
				}
			}

			for (dword index = 0; !isPlayerChosed && index < _dismiss_info->dwNotAgreeUserCout; ++index) {
				if (_dismiss_info->dwNotAgreeChairID[index] == chairIndex) {
					isPlayerChosed = true;
					text = "拒绝";
					_nDismissCounter = 60;
					break;
				}
			}

			auto label = Label::createWithTTF(labelConfig, utility::a_u8("【") +player->GetNickName() + utility::a_u8("】 : ") + utility::a_u8(text));
			label->setAnchorPoint(cocos2d::Vec2(0, 0.5));
			label->setPosition(startPos + addPos * (chairIndex % 3) + addLinePos * (chairIndex / 3));
			label->setColor(cocos2d::Color3B(158, 88, 52));
			pNode->addChild(label);
		}
	}
}

void NNDismissRoom::show(NN_DismissRoom_Type type, CMD_GF_Private_Dismiss_Info* pInfo)
{
	_eDismissRoomType = type;
	if (pInfo) {
		_eDismissRoomType = NN_DismissRoom_ReplyDismiss;
		memcpy(_dismiss_info, pInfo, sizeof(CMD_GF_Private_Dismiss_Info));
		_nDismissCounter = pInfo->dwValue2;
		replyDismiss();
	} else {
		_eDismissRoomType = NN_DismissRoom_ApplyForDismiss;
		applyForDismiss();
	}
	setVisible(true);
}

void NNDismissRoom::hide()
{
    setVisible(false);
}

void NNDismissRoom::Button_OK(cocos2d::Ref *, WidgetUserInfo *)
{
	GPGameLink::Instance().DismissRoom(true);
	_eDismissRoomType = NN_DismissRoom_None;
}

void NNDismissRoom::Button_Agree(cocos2d::Ref*, WidgetUserInfo*)
{
    GPGameLink::Instance().DismissRoom(true);
	_eDismissRoomType = NN_DismissRoom_None;
}

void NNDismissRoom::Button_Refuse(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_eDismissRoomType == NN_DismissRoom_HostBeforeStart) {
		hide();
	} else {
		GPGameLink::Instance().DismissRoom(false);
	}
	_eDismissRoomType = NN_DismissRoom_None;
}

void NNDismissRoom::Button_Cancle(cocos2d::Ref *, WidgetUserInfo *)
{
	hide();
	_eDismissRoomType = NN_DismissRoom_None;
}

void NNDismissRoom::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_eDismissRoomType == NN_DismissRoom_HostBeforeStart) {
		hide();
	} else {
		GPGameLink::Instance().DismissRoom(false);
	}
	_eDismissRoomType = NN_DismissRoom_None;
}
