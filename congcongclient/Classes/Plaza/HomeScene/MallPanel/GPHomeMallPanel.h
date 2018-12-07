#pragma once
#include "../GPHomeHeader.h"

class GPHomeMallPanel
	:public cocos2d::Node
	, public FvSingleton<GPHomeMallPanel>
{
public:
	const static int MAX_PRODUCT = 4;
public:
	GPHomeMallPanel();
	~GPHomeMallPanel();
public:
	bool init();
public:
	void initLayout();
	void initButton();
	void initListView();
public:
	void show(bool isDiamond = true);
	void hide();
public:
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Buy(cocos2d::Ref*, WidgetUserInfo*);
	void onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);
public:



};