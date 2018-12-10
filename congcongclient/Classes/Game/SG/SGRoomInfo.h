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

#pragma region ��ʼ��
	bool init();
	void initLayout();
	void initData();
#pragma endregion ��ʼ��

#pragma region ��ʾ������
	void show();
	void hide();
#pragma endregion ��ʾ������

#pragma region ���ݽ�����ˢ����ʾ
	void setRoomInfo(CMD_GF_Private_Room_Info& roomInfo);
	void updateRoomInfo();

	bool isHostPlayer(SGPlayer& player);
	int getPlayerScore(SGPlayer& player);
	std::string getRoomInfoText(bool forShare = false);
	CMD_GF_Private_Room_Info getRoomInfo();
#pragma endregion ���ݽ�����ˢ����ʾ

private:
	CMD_GF_Private_Room_Info m_RoomInfo;
	TTFConfig m_LabelConfig;
};