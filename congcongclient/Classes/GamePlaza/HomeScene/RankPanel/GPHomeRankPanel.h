#pragma once
#include "../GPHomeHeader.h"
#include "GameLib/Platform/PFKernel/CGPopularizeMission.h"

class GPHomeRankPanel
	:public cocos2d::Node
	, public CGPopularizeSink
	, public FvSingleton<GPHomeRankPanel>
{
public:
	GPHomeRankPanel();
	~GPHomeRankPanel();
public:
	bool init();
public:
	void initLayout();
	void initButton();
	void initStatus();
public:
	void show();
	void hide();

	void onGPPopularizeListRes(CMD_GP_PopularizeListRes* pNetInfo);
public:
	void Button_Selected(cocos2d::Ref*, WidgetUserInfo*);
	void ButtonClose(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	int m_CurrentIndex;
	cocos2d::Node* m_pRootNode;
	CGPopularizeMission m_PopularizeMission;
};