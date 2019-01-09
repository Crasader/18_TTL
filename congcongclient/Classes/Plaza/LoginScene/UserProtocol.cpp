#include "UserProtocol.h"
#include "UTILITY.h"
#include "GPLoginScene.h"
#include CMD_GAME

FV_SINGLETON_STORAGE(UserProtocol);

UserProtocol::UserProtocol()
	: _touch_state(RTS_Null)
	, _TouchBegbin(0 ,0)
	, _TouchMoved(0, 0)
	, _cur_title_position(0, 0)
	, _original_title_position(0, 0)
	, _max_move_h(0.f)
	, _actived(false)
{
    init();
}

UserProtocol::~UserProtocol()
{

}

bool UserProtocol::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();
	initView();
	initEvent();

    return true;
}

void UserProtocol::initView()
{
	std::string strFontNameContent = WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "FontNameContent");
	std::string strFontNameTitle = WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "FontNameTitle");
	auto BeginXTitle = utility::parseFloat(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "BeginXTitle"));
	auto BeginXContent = utility::parseFloat(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "BeginXContent"));
	auto BeginY = utility::parseFloat(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "BeginY"));
	auto Titlecolor = utility::parseColour(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "FontColorTilte"));
	auto FontColorContent = utility::parseColour(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "FontColorContent"));
	auto nFontSizeTitle = utility::parseInt(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "FontSizeTitle"));
	auto nFontSizeContent = utility::parseInt(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "FontSizeContent"));
	auto nLineInterval = utility::parseInt(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "LineInterval"));
	auto nLineWidthContent = utility::parseInt(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "LineWidthContent"));
	auto nMaxLineNumber = utility::parseInt(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "MaxLineNumber"));
	auto ViewPortBegin = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "ViewPortBegin"));
	auto ViewPortSize = utility::parseSize(WidgetFun::getWidgetUserInfo(this, "Arrange_Rules", "ViewPortSize"));

	//////////////////////////////////////////////////////////////////////////

	TTFConfig TitleConfig;
	TitleConfig.fontFilePath = strFontNameTitle;
	TitleConfig.fontSize = nFontSizeTitle;
	TitleConfig.glyphs = GlyphCollection::DYNAMIC;
	TitleConfig.customGlyphs = nullptr;
	TitleConfig.distanceFieldEnabled = false;
	TitleConfig.outlineSize = 1;

	TTFConfig ContentConfig;
	ContentConfig.fontFilePath = strFontNameContent;
	ContentConfig.fontSize = nFontSizeContent;
	ContentConfig.glyphs = GlyphCollection::DYNAMIC;
	ContentConfig.customGlyphs = nullptr;
	ContentConfig.distanceFieldEnabled = false;
	ContentConfig.outlineSize = 0;

	float offsetH = 0.f;

	auto Clipper = ClippingNode::create();
	auto ClipperRect = DrawNode::create();
	cocos2d::Color4F color = {1.f, 0, 0, 1.f};
	cocos2d::Rect rect(ViewPortBegin , ViewPortSize);
	cocos2d::Point points[4] = { ViewPortBegin , 
												Point(ViewPortBegin.x, ViewPortBegin.y + ViewPortSize.height),
												Point(ViewPortBegin.x + ViewPortSize.width, ViewPortBegin.y + ViewPortSize.height),
												Point(ViewPortBegin.x + ViewPortSize.width, ViewPortBegin.y)};
	ClipperRect->drawPolygon(points, 4, color, 0, color);
	//this->addChild(ClipperRect);
	Clipper->setStencil(ClipperRect);

	std::string strTitlePaiXing = WidgetFun::getWidgetUserInfo(this, "Txts_Title", "1_0");
	auto label = Label::createWithTTF(TitleConfig, strTitlePaiXing, TextHAlignment::CENTER, nLineWidthContent);
	label->setPosition(640, BeginY - offsetH);
	label->setColor(Titlecolor);
	label->setAnchorPoint(cocos2d::Vec2(.5, 0.5));
	_original_title_position = { BeginXTitle, BeginY - offsetH };
	_cur_title_position = _original_title_position;
	_plable.push_back(label);
	_vct_lable_riginal_pos.push_back(label->getPosition());
	Clipper->addChild(label);
	offsetH += label->getContentSize().height;

	offsetH += nLineInterval * 2;

	//////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < nMaxLineNumber; i++) {
		std::string strContent = WidgetFun::getWidgetUserInfo(this, "Txts_ZongGao", utility::toString("1_", i));
		if (strContent == "Not Find") {
			break;
		}
		auto label = Label::createWithTTF(ContentConfig, strContent, TextHAlignment::LEFT, nLineWidthContent);
		label->setPosition(640, BeginY - offsetH);
		label->setColor(FontColorContent);
		offsetH += label->getContentSize().height;
		offsetH += nLineInterval;
		_plable.push_back(label);
		label->setAnchorPoint(cocos2d::Vec2(0.5f, 1.f));
		_vct_lable_riginal_pos.push_back(label->getPosition());
		Clipper->addChild(label);
	}

	offsetH += nLineInterval * 3;

	//////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < nMaxLineNumber; i++) {
		std::string strContent = WidgetFun::getWidgetUserInfo(this, "Txts_Welcome", utility::toString("1_", i));
		if (strContent == "Not Find") {
			break;
		}
		auto label = Label::createWithTTF(ContentConfig, strContent, TextHAlignment::LEFT, nLineWidthContent);
		label->setPosition(BeginXContent, BeginY - offsetH);
		label->setColor(FontColorContent);
		label->setAnchorPoint(cocos2d::Vec2(0.f, 1.f));
		offsetH += label->getContentSize().height;
		offsetH += nLineInterval;
		_plable.push_back(label);
		_vct_lable_riginal_pos.push_back(label->getPosition());
		Clipper->addChild(label);
	}

	offsetH += nLineInterval * 3;

	//////////////////////////////////////////////////////////////////////////

	std::string strTitleZhuangJia = WidgetFun::getWidgetUserInfo(this, "Txts_DingYi", "1_0");
	label = Label::createWithTTF(TitleConfig, strTitleZhuangJia, TextHAlignment::CENTER, nLineWidthContent);
	label->setPosition(BeginXTitle, BeginY - offsetH);
	label->setColor(Titlecolor);
	label->setAnchorPoint(cocos2d::Vec2(0.f, 0.5f));
	_plable.push_back(label);
	_vct_lable_riginal_pos.push_back(label->getPosition());
	Clipper->addChild(label);
	offsetH += label->getContentSize().height;

	offsetH += nLineInterval * 2;

	for (int i = 1; i < nMaxLineNumber; i++) {
		std::string strContent = WidgetFun::getWidgetUserInfo(this, "Txts_DingYi", utility::toString("1_", i));
		if (strContent == "Not Find") {
			break;
		}
		auto label = Label::createWithTTF(ContentConfig, strContent, TextHAlignment::LEFT, nLineWidthContent);
		label->setPosition(BeginXContent, BeginY - offsetH);
		label->setColor(FontColorContent);
		label->setAnchorPoint(cocos2d::Vec2(0.f, 1.f));
		offsetH += label->getContentSize().height;
		offsetH += nLineInterval;
		_plable.push_back(label);
		_vct_lable_riginal_pos.push_back(label->getPosition());
		Clipper->addChild(label);
	}

	offsetH += nLineInterval * 3;

	//////////////////////////////////////////////////////////////////////////

	std::string strTitleTuiZhu = WidgetFun::getWidgetUserInfo(this, "Txts_ZhangHao", "1_0");
	label = Label::createWithTTF(TitleConfig, strTitleTuiZhu, TextHAlignment::CENTER, nLineWidthContent);
	label->setPosition(BeginXTitle, BeginY - offsetH);
	label->setColor(Titlecolor);
	label->setAnchorPoint(cocos2d::Vec2(0.f, 0.5f));
	_plable.push_back(label);
	_vct_lable_riginal_pos.push_back(label->getPosition());
	Clipper->addChild(label);
	offsetH += label->getContentSize().height;

	offsetH += nLineInterval * 2;

	for (int i = 1; i < nMaxLineNumber; i++) {
		std::string strContent = WidgetFun::getWidgetUserInfo(this, "Txts_ZhangHao", utility::toString("1_", i));
		if (strContent == "Not Find") {
			break;
		}
		auto label = Label::createWithTTF(ContentConfig, strContent, TextHAlignment::LEFT, nLineWidthContent);
		label->setPosition(BeginXContent, BeginY - offsetH);
		label->setColor(FontColorContent);
		label->setAnchorPoint(cocos2d::Vec2(0.f, 1.f));
		offsetH += label->getContentSize().height;
		offsetH += nLineInterval;
		_plable.push_back(label);
		_vct_lable_riginal_pos.push_back(label->getPosition());
		Clipper->addChild(label);
	}

	_max_move_h = offsetH - 400;
	this->addChild(Clipper);
}

void UserProtocol::initLayout()
{
    WidgetScenceXMLparse user_protocol("Script/UserProtocol.xml", this);
}

void UserProtocol::initButton()
{
	WidgetManager::addButtonCB("Button_Close", this, button_selector(UserProtocol::Button_Close));
}

void UserProtocol::initEvent()
{
	scheduleUpdate();

	_eventDispatcher->setEnabled(true);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(UserProtocol::onTouchBegin, this);
	listener->onTouchMoved = CC_CALLBACK_2(UserProtocol::onTouchMove, this);
	listener->onTouchEnded = CC_CALLBACK_2(UserProtocol::onTouchEnd, this);
	listener->onTouchCancelled = CC_CALLBACK_2(UserProtocol::onTouchCancel, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

#define UPDATE_INTERVAL 0.10

void UserProtocol::update(float fpass)
{
	if (_plable.size() == 0 || !_actived) {
		return;
	}
}

bool UserProtocol::onTouchBegin(Touch * touch, Event * ev)
{
	if (_plable.size() == 0 || !_actived) {
		return false;
	}
	_touch_state = RTS_Touched;
	_TouchBegbin = touch->getLocation();
	_cur_title_position.y = _plable[0]->getPosition().y;
	return true;
}

void UserProtocol::onTouchMove(Touch * touch, Event * ev)
{
	if (_plable.size() == 0 || !_actived) {
		return;
	}
	_touch_state = RTS_Moved;
	_TouchMoved = touch->getLocation();

	float flabel_offset = _TouchMoved.y - _TouchBegbin.y;
	if (_cur_title_position.y + flabel_offset < _original_title_position.y) {
		flabel_offset = _original_title_position.y - _cur_title_position.y;
	} else if (_cur_title_position.y + flabel_offset > _original_title_position.y + _max_move_h) {
		flabel_offset = (_original_title_position.y + _max_move_h) - (_cur_title_position.y) ;
	}

	CCLOG("flabel_offset =%f", flabel_offset);

	auto& pos_title = _plable[0]->getPosition();
	_plable[0]->setPosition(pos_title.x, _cur_title_position.y + flabel_offset);
	auto offset = 0.f;
	for (size_t idx = 1; idx < _plable.size(); idx++) {
		auto& pos = _plable[idx]->getPosition();
		offset += _vct_lable_riginal_pos[idx].y - _vct_lable_riginal_pos[idx - 1].y;
		_plable[idx]->setPosition(pos.x, pos_title.y + offset);
	}
}

void UserProtocol::onTouchEnd(Touch * touch, Event * ev)
{
	if (_plable.size() == 0 || !_actived) {
		return;
	}
	_touch_state = RTS_End;
}

void UserProtocol::onTouchCancel(Touch * touch, Event * ev)
{
	_touch_state = RTS_End;
}

void UserProtocol::show()
{
    setVisible(true);
	_actived = true;
}

void UserProtocol::hide()
{
    setVisible(false);
	_actived = false;
	GPLoginScene::Instance().setEnableButtons(true);
}

void UserProtocol::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
	setVisible(false);
	_actived = false;
	GPLoginScene::Instance().setEnableButtons(true);
}
