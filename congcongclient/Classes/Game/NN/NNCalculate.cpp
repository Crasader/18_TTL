#include "NNCalculate.h"
#include "Game/NN/NNGameScene.h"
#include "Game/NN/NNRoomInfo.h"
#include "Game/NN/CMD_NN.h"
#include "Plaza/GameManager/GPSceneManager.h"
#include "Game/Game/MissionWeiXin.h"
#include IMAGE_DOWN
#include UTILITY_CONVERT

FV_SINGLETON_STORAGE(NNCalculate);

NNCalculate::NNCalculate()
{
	init();
}

NNCalculate::~NNCalculate()
{

}

#pragma region 初始化
bool NNCalculate::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initButton();

	return true;
}

void NNCalculate::initLayout()
{
	WidgetScenceXMLparse calculate("TTLNN/Script/NNCalculate.xml", this);
}

void NNCalculate::initButton()
{
	WidgetManager::addButtonCB("NNCalculate_ButtonBackPlaza", this, button_selector(NNCalculate::Button_BackPlaza));
	WidgetManager::addButtonCB("NNCalculate_ButtonShare", this, button_selector(NNCalculate::Button_Share));
}
#pragma endregion 初始化

#pragma region 显示与隐藏
void NNCalculate::show(CMD_GF_Private_End_Info* pInfo)
{
	update(pInfo);
	setVisible(true);
}

void NNCalculate::hide()
{
	setVisible(false);
}

void NNCalculate::update(CMD_GF_Private_End_Info* pInfo)
{
	auto room_info = NNRoomInfo::Instance().getRoomInfo();

	WidgetFun::setText(this, "Txt_RoomNo", room_info.dwRoomNum);
	WidgetFun::setText(this, "Txt_BaseScore", room_info.dwBaseScore);
	WidgetFun::setText(this, "Txt_RoundSum", room_info.dwPlayCout);

	switch (room_info.bGameTypeIdex) {
	case 3:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("明牌抢庄"));
		break;
	case 4:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("通比牛牛"));
		break;
	case 2:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("自由抢庄"));
		break;
	default:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("明牌抢庄"));
		break;
	}
	time_t tt = time(nullptr);
	char tmp[64] = { 0 };
	strftime(tmp, sizeof(tmp), "%Y/%m/%d/ %H:%M:%S", localtime(&tt));
	WidgetFun::setText(this, "Txt_Time", tmp);

	int nodeIndex = 0;
	int score_DaYingJia = 0;
	int score_TuHao = 0;

	for (int index = 0; index < NN_GAME_PLAYER; ++index) {
		NNPlayer* player = NNGameScene::Instance().getPlayerByChairID(index);
		if (!player->isValid()) {
			continue;
		}

		if (score_DaYingJia < pInfo->lPlayerWinLose[index]) {
			score_DaYingJia = pInfo->lPlayerWinLose[index];
		}
		if (score_TuHao > pInfo->lPlayerWinLose[index]) {
			score_TuHao = pInfo->lPlayerWinLose[index];
		}
	}

	for (int index = 0; index < NN_GAME_PLAYER; ++index) {
		NNPlayer* player = NNGameScene::Instance().getPlayerByChairID(index);
		if (!player->isValid()) {
			WidgetFun::setVisible(this, utility::toString("NNCalculate_", index), false);
			continue;
		}

		auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNCalculate_", nodeIndex++));
		playerNode->setVisible(true);

		WidgetFun::setText(playerNode, "NNCalculate_Name", player->GetNickName());
		WidgetFun::setText(playerNode, "NNCalculate_ID", utility::toString(utility::paseInt(player->GetUserID(), 6)));
		int playerScore = 0;
		if (pInfo->lPlayerWinLose.size() > index) {
			playerScore = pInfo->lPlayerWinLose.at(index);
		} else {
			playerScore = NNRoomInfo::Instance().getPlayerScore(player);
		}

		std::string score = utility::toString(playerScore > 0 ? "+" : "", playerScore);
		WidgetFun::setText(playerNode, "NNCalculate_Score", score);

		WidgetFun::setVisible(playerNode, "Img_DaYingJia", false);
		WidgetFun::setVisible(playerNode, "Img_TuHao", false);
		if (score_DaYingJia == playerScore && score_DaYingJia != 0) {
			WidgetFun::setVisible(playerNode, "Img_DaYingJia", true);
		} else if(score_TuHao == playerScore && score_TuHao != 0) {
			WidgetFun::setVisible(playerNode, "Img_TuHao", true);
		}

		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(playerNode, "NNCalculate_Avatar"), player->GetHeadHttp(), player->GetUserID());
	}
}

#pragma endregion 显示与隐藏

#pragma region 按钮事件

void NNCalculate::Button_BackPlaza(cocos2d::Ref*, WidgetUserInfo*)
{
	//DONE:实际上已经回到大厅了
	//NNGameScene::Instance().Button_Exit(NULL, NULL);
	hide();
	GPSceneManager::Instance().InHomeScene();
}

void NNCalculate::Button_Share(cocos2d::Ref*, WidgetUserInfo*)
{
	//static auto pNode = this;
	cocos2d::utils::captureScreen([](bool b, std::string name) {
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
			//pNode->hide();
		}
	}, "screenshot.png");
}

#pragma endregion 按钮事件