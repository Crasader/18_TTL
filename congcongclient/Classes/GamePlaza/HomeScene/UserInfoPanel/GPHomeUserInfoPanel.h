#pragma once

#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetScenceXmlParse.h"

class GPHomeUserInfoPanel
	: public cocos2d::Node
	, public FvSingleton<GPHomeUserInfoPanel>
{
public:
	GPHomeUserInfoPanel();
	~GPHomeUserInfoPanel();
public:
	bool init();
	void show();
	void hide();
	void onUserInfo();
	cocos2d::Node* getCircleAvatar(cocos2d::Node* pRootNode, const std::string& filename, const cocos2d::Size& targetSize,  const cocos2d::Vec2& position);

	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);
};