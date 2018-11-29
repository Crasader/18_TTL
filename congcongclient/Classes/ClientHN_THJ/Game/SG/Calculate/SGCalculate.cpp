#include "SGCalculate.h"
#include "ClientHN_THJ/Game/SG/GameScene/SGGameScence.h"
#include "ClientHN_THJ/Game/SG/GameScene/SGRoomInfo.h"
#include "ClientHN_THJ/Game/SG/Protocol/CMD_SG.h"
#include "GamePlaza/GPSceneManager.h"

FV_SINGLETON_STORAGE(SGCalculate);
SGCalculate::SGCalculate()
{
	init();
}

SGCalculate::~SGCalculate()
{

}

#pragma region 初始化
bool SGCalculate::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initButton();

	return true;
}

void SGCalculate::initLayout()
{
	WidgetScenceXMLparse calculate("TTLSG/Script/SGCalculate.xml", this);
}

void SGCalculate::initButton()
{
	WidgetManager::addButtonCB("SGCalculate_ButtonBackPlaza", this, button_selector(SGCalculate::Button_BackPlaza));
	WidgetManager::addButtonCB("SGCalculate_ButtonShare", this, button_selector(SGCalculate::Button_Share));
}
#pragma endregion 初始化

#pragma region 显示与隐藏
void SGCalculate::show(CMD_GF_Private_End_Info* pInfo)
{
	update(pInfo);
	setVisible(true);
}

void SGCalculate::hide()
{
	setVisible(false);
}

void SGCalculate::update(CMD_GF_Private_End_Info* pInfo)
{
	int nodeIndex = 0;
	for (int index = 0; index < SG_GAME_PLAYER; ++index) {
		SGPlayer* player = SGGameScence::Instance().getPlayerByChairID(index);
		if (player->isValid()) {
			auto playerNode = WidgetFun::getChildWidget(this, utility::toString("SGCalculate_", nodeIndex++));
			playerNode->setVisible(true);

			WidgetFun::setText(playerNode, "SGCalculate_Name", player->GetNickName());
			WidgetFun::setText(playerNode, "SGCalculate_ID", utility::toString("ID:", utility::paseInt(player->GetUserID(), 6)));
			int playerScore = 0;
			if (pInfo->lPlayerWinLose.size() > index) {
				playerScore = pInfo->lPlayerWinLose.at(index);
			} else {
				playerScore = SGRoomInfo::Instance().getPlayerScore(*player);
			}

			std::string score = utility::toString(playerScore > 0 ? "+" : "", playerScore);
			WidgetFun::setText(playerNode, "SGCalculate_Score", score);
			ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(playerNode, "SGCalculate_Avatar"), player->GetHeadHttp(), player->GetUserID());
		} else {
			WidgetFun::setVisible(this, utility::toString("SGCalculate_", index), false);
		}
	}
}
#pragma endregion 显示与隐藏

#pragma region 按钮事件
void SGCalculate::Button_BackPlaza(cocos2d::Ref*, WidgetUserInfo*)
{
	SGGameScence::Instance().Button_Exit(NULL, NULL);
	hide();
	GPSceneManager::Instance().InHomeScene();
}

void SGCalculate::Button_Share(cocos2d::Ref*, WidgetUserInfo*)
{
	SGGameScence::Instance().Button_Exit(NULL, NULL);
}
#pragma endregion 按钮事件