#pragma once

#include "common.h"
#include "TLJPlayer.h"

class TLJCalculateDetailPanel
	: public cocos2d::Node
	, public FvSingleton<TLJCalculateDetailPanel>
{
public:
	TLJCalculateDetailPanel();
	~TLJCalculateDetailPanel();
public:
	bool init();

	void show();
	void hide();
};