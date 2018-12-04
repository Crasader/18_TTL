#pragma once

#include "CommonHeader/CommonHeader.h"
#include "ClientHN_THJ/Game/TTLNN/Player/NNPlayer.h"

class NNRoomInfo
	: public cocos2d::Node
	, public FvSingleton<NNRoomInfo>
{
public:
	NNRoomInfo();
	~NNRoomInfo();

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

	bool isCreaterPlayer(NNPlayer* player);
	bool isHostPlayer(NNPlayer* player);
	int getPlayerScore(NNPlayer* player);
	std::string getRoomInfoView(bool forShare /*= false*/);
	std::string getRoomInfoText(bool forShare = false);
	CMD_GF_Private_Room_Info getRoomInfo();
	void setRoomInfoBase(dword dwBaseScore);
#pragma endregion 数据接收与刷新显示

private:
	CMD_GF_Private_Room_Info m_RoomInfo;
	TTFConfig m_LabelConfig;
};