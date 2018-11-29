#pragma once

#include "CommonHeader/CommonHeader.h"
#include "../Player/TLJPlayer.h"

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

	void showCalculateList(WORD wPlayerCount);
	void showCalculateItem(TLJPlayer& player,WORD wIndex);
	void showPlayerAvatar(TLJPlayer& player,Node* pRootNode);
	void showWinner(TLJPlayer& player);

	void showTotal(WORD wPlayerCount);

	void show();
	void hide();
private:
	Vec2 startPos;
	Vec2 addPos;
};