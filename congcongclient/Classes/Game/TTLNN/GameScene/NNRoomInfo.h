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

	bool isCreaterPlayer(NNPlayer* player);
	bool isHostPlayer(NNPlayer* player);
	int getPlayerScore(NNPlayer* player);
	std::string getRoomInfoView(bool forShare /*= false*/);
	std::string getRoomInfoText(bool forShare = false);
	CMD_GF_Private_Room_Info getRoomInfo();
	void setRoomInfoBase(dword dwBaseScore);
#pragma endregion ���ݽ�����ˢ����ʾ

private:
	CMD_GF_Private_Room_Info m_RoomInfo;
	TTFConfig m_LabelConfig;
};