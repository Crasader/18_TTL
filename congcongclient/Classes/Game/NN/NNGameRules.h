#pragma once

#include "common.h"

enum Rules_Touch_State {
	RTS_Null,
	RTS_Touched,
	RTS_Moved,
	RTS_End,
};

class NNGameRules
	:public cocos2d::Node
	, public FvSingleton<NNGameRules>
{
public:
	NNGameRules();
	~NNGameRules();

#pragma region 初始化
public:
	bool init();
	void initView();
	void initLayout();
	void initButton();
	void initEvent();
#pragma endregion 初始化

	void update(float fpass) override;

	bool onTouchBegin(Touch* touch, Event* ev);
	void onTouchMove(Touch* touch, Event* ev);
	void onTouchEnd(Touch* touch, Event* ev);
	void onTouchCancel(Touch* touch, Event* ev);

	void show();
	void hide();
	void Button_Close(cocos2d::Ref*, WidgetUserInfo*);

private:

	float updateInterval;

	cocos2d::Vec2 _TouchBegbin;
	cocos2d::Vec2 _TouchMoved;

	Rules_Touch_State _touch_state;
	std::vector<cocos2d::Label*> _plable;
	std::vector<cocos2d::Vec2> _vct_lable_riginal_pos;
	cocos2d::Vec2 _original_title_position;
	cocos2d::Vec2 _cur_title_position;

	float _max_move_h;
	bool _actived;
};