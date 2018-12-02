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
	cocos2d::Node* createCircleAvatar(cocos2d::Node* pRootNode, const std::string& filename, const std::string& stencilPath, const cocos2d::Vec2& position);

	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

protected:
	//Íæ¼ÒÍ·Ïñ
	cocos2d::Node* _spUserHread;
};