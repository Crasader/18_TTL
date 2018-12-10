#pragma once

#include "common.h"
#include "TLJPlayer.h"

class TLJCalculatePanel
	: public cocos2d::Node
	, public FvSingleton<TLJCalculatePanel>
{
public:
	TLJCalculatePanel();
	~TLJCalculatePanel();
public:
	bool init();
	void initButton();

	void Btn_Share(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);
	void Close(cocos2d::Ref* ref, WidgetUserInfo* widgetUserInfo);

	void showCalculateList(word wPlayerCount);
	void showCalculateItem(TLJPlayer& player,word wIndex);
	void showPlayerAvatar(TLJPlayer& player,Node* pRootNode);
	void showWinner(TLJPlayer& player);

	void showTotal(word wPlayerCount);

	void show();
	void hide();
private:
	Vec2 startPos;
	Vec2 addPos;
};