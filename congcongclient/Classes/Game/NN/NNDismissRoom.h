#pragma once

#include "common.h"

enum NN_DismissRoom_Type{
	NN_DismissRoom_None,
	NN_DismissRoom_HostBeforeStart,
	NN_DismissRoom_ApplyForDismiss,
	NN_DismissRoom_ReplyDismiss,
};

class NNDismissRoom
	: public cocos2d::Node
	, public FvSingleton<NNDismissRoom>
{
public:
	NNDismissRoom();
	~NNDismissRoom();

public:
	bool init();
	void initLayout();
	void initButton();
	void initData();

public:
	void update(float ftime) override;
	void hostBeforeStart();
	void applyForDismiss();
	void replyDismiss();
	void show(NN_DismissRoom_Type type = NN_DismissRoom_HostBeforeStart, CMD_GF_Private_Dismiss_Info* pInfo = nullptr);
	void hide();

public:
	void Button_OK(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Agree(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Refuse(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Cancle(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

private:
	//µ¹¼ÆÊ±
	float _fPassBegin;
	float _fPassEnd;
	int _nDismissCounter;
	NN_DismissRoom_Type _eDismissRoomType;
	CMD_GF_Private_Dismiss_Info* _dismiss_info;
};