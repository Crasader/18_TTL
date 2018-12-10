#pragma once

#include "common.h"
#include CMD_GAME

#include "Game/SG/SGPlayer.h"

class SGRoomInfo
	: public cocos2d::Node
	, public FvSingleton<SGRoomInfo>
{
public:
	SGRoomInfo();
	~SGRoomInfo();

#pragma region 初始化
	bool init();
	void initLayout();
	void initData();
#pragma endregion 初始化

#pragma region 显示与隐藏
	void show();
	void hide();
#pragma endregion 显示与隐藏

#pragma region 数据接收与刷新显示
	void setRoomInfo(CMD_GF_Private_Room_Info& roomInfo);
	void updateRoomInfo();

	bool isHostPlayer(SGPlayer& player);
	int getPlayerScore(SGPlayer& player);
	std::string getRoomInfoText(bool forShare = false);
	CMD_GF_Private_Room_Info getRoomInfo();
#pragma endregion 数据接收与刷新显示

private:
	CMD_GF_Private_Room_Info m_RoomInfo;
	TTFConfig m_LabelConfig;
};