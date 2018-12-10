#include "NNCalculate.h"
#include "Game/NN/NNGameScene.h"
#include "Game/NN/NNRoomInfo.h"
#include "Game/NN/CMD_NN.h"
#include "Plaza/GameManager/GPSceneManager.h"
#include "Game/Game/MissionWeiXin.h"
#include IMAGE_DOWN

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
	int nodeIndex = 0;
	for (int index = 0; index < NN_GAME_PLAYER; ++index) {
		NNPlayer* player = NNGameScene::Instance().getPlayerByChairID(index);
		if (player->isValid()) {
			auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNCalculate_", nodeIndex++));
			playerNode->setVisible(true);

			WidgetFun::setText(playerNode, "NNCalculate_Name", player->GetNickName());
			WidgetFun::setText(playerNode, "NNCalculate_ID", utility::toString("ID:", utility::paseInt(player->GetUserID(), 6)));
			int playerScore = 0;
			if (pInfo->lPlayerWinLose.size() > index) {
				playerScore = pInfo->lPlayerWinLose.at(index);
			} else {
				playerScore = NNRoomInfo::Instance().getPlayerScore(player);
			}

			std::string score = utility::toString(playerScore > 0 ? "+" : "", playerScore);
			WidgetFun::setText(playerNode, "NNCalculate_Score", score);
			ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(playerNode, "NNCalculate_Avatar"), player->GetHeadHttp(), player->GetUserID());
		} else {
			WidgetFun::setVisible(this, utility::toString("NNCalculate_", index), false);
		}
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