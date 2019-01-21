#include "NNPlayerPanel.h"
#include "Game/NN/NNGameScene.h"
#include "Game/NN/NNRoomInfo.h"
#include "Game/NN/CMD_NN.h"
#include USERINFO
#include UTILITY_CONVERT

FV_SINGLETON_STORAGE(NNPlayerPanel);

NNPlayerPanel::NNPlayerPanel()
{
    init();
}

NNPlayerPanel::~NNPlayerPanel()
{

}

bool NNPlayerPanel::init()
{
    if (!cocos2d::Node::init()) {
        return false;
    }

    initLayout();

    return true;
}

void NNPlayerPanel::initLayout()
{
    WidgetScenceXMLparse playerPanel("TTLNN/Script/NNPlayerPanel.xml", this);

	WidgetManager::addButtonCB("Btn_Head", this, button_selector(NNPlayerPanel::Button_Head));
}

long fcurTime = 0;
long fendTime = 0;

void NNPlayerPanel::Button_Head(cocos2d::Ref*, WidgetUserInfo*)
{
	fcurTime = time(nullptr);
	if (fcurTime - fendTime >= 1) {
		fendTime = fcurTime;
	} else {
		return;
	}
	auto pServ = IServerItem::get();
	if (pServ && NNGameStatus::NNGameStatus_Free <= NNGameScene::Instance().getGameStatus()) {
		CMD_C_CHEAT cheat;
		cheat.dwPlayerID = UserInfo::Instance().getUserID();
		cheat.dwTableID = NNGameScene::Instance().m_RoomInfo.dwRoomNum;
		pServ->SendSocketData(MDM_GF_GAME, SUB_C_CHEAT, &cheat, sizeof(CMD_C_CHEAT));
	}
}

void NNPlayerPanel::setBanker(NNPlayer& player, bool flag)
{
	int visioChairID = player.getVisioChairID();
	if (player.isValid() && visioChairID < NNGameScene::MAX_PLAYER) {
		auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNPlayer_", visioChairID));
		WidgetFun::setVisible(playerNode, "NNPlayer_BankerBG", flag);
	}
}

void NNPlayerPanel::showPlayer(NNPlayer& player)
{
    int visioChairID = player.getVisioChairID();
    if (player.isValid() && visioChairID < NNGameScene::MAX_PLAYER) {
        auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNPlayer_", visioChairID));
        playerNode->setVisible(true);

		WidgetFun::setVisible(playerNode, "Img_TuiZhu", false);
		WidgetFun::setVisible(playerNode, "Txt_TuiZhu", false);

        if (player.GetUserID() == UserInfo::Instance().getUserID()) {
            if (player.GetUserStatus() < US_READY && NNGameScene::Instance().getGameStatus() < TTLNN::NNGameStatus_Start) {
                NNGameScene::Instance().showReady();
            } else {
                NNGameScene::Instance().hideReady();
            }
			if (player.getTuiZhu() > 0) {
				WidgetFun::setVisible(playerNode, "Img_TuiZhu", true);
				WidgetFun::setVisible(playerNode, "Txt_TuiZhu", true);
				WidgetFun::setText(WidgetFun::getChildWidget(playerNode, "Txt_TuiZhu"), utility::a_u8(utility::toString("推 ", player.getTuiZhu())));
			}
        }

		player.setHeadNode(playerNode);
        WidgetFun::setVisible(playerNode, "NNPlayer_BankerBG", NNGameScene::Instance().isBankerUser(player));
        WidgetFun::setVisible(playerNode, "NNPlayer_Banker", NNGameScene::Instance().isBankerUser(player));

        WidgetFun::setVisible(playerNode, "NNPlayer_Ready", player.GetUserStatus() == US_READY);
        WidgetFun::setVisible(playerNode, "NNPlaye_BetsNode", player.getPlayerBets() != 0);
        WidgetFun::setVisible(playerNode, "NNPlayer_Snatch", player.getSnatchBankerRatio() != (word)TTLNN::NNSnatchBanker_Invalid);
		//DONE: 不需要显示房主图标
		WidgetFun::setVisible(playerNode, "NNPlayer_Host", false);// NNRoomInfo::Instance().isHostPlayer(&player));
        WidgetFun::setText(playerNode, "NNPlayer_Name", player.GetNickName());

		//DONE:设置玩家分数
		if (player.GetChairID() >= 0 && player.GetChairID() < NNRoomInfo::Instance().getRoomInfo().kWinLoseScore.size()) {
			int nScore = NNRoomInfo::Instance().getRoomInfo().kWinLoseScore[player.GetChairID()];
			WidgetFun::setText(playerNode, "NNPlayer_Score", nScore);
		}

		if(NNRoomInfo::Instance().getRoomInfo().dwBaseScore != 0)
			WidgetFun::setText(playerNode, "NNPlayer_Bets", player.getPlayerBets() / NNRoomInfo::Instance().getRoomInfo().dwBaseScore);

		if (player.getSnatchBankerRatio() != (word)TTLNN::NNSnatchBanker_Invalid) {
            WidgetFun::setImagic(playerNode, "NNPlayer_Snatch", utility::toString(WidgetFun::getWidgetUserInfo(playerNode, "NNPlayer_Snatch", "Image"), (int)player.getSnatchBankerRatio(), ".png"), false);
        }
    }
}

void NNPlayerPanel::hidePlayer(int playerVisioChairID)
{
    if (playerVisioChairID < NNGameScene::MAX_PLAYER) {
        WidgetFun::setVisible(this, utility::toString("NNPlayer_", playerVisioChairID), false);
    }
}

//void NNPlayerPanel::playerTalk(NNPlayer& player, CMD_GR_C_TableTalk* pInfo)
//{
//    if (!player.isValid()) {
//        return;
//    }
//
//    word visioChairID = player.getVisioChairID();
//	auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNPlayer_", visioChairID));
//
//	switch (pInfo->cbType) {
//	case CMD_GR_C_TableTalk::TYPE_FILE: {
//		if (!WidgetFun::getChildWidget(playerNode, "NNPlayer_TalkAction")) {
//			return;
//		}
//
//		WidgetFun::runWidgetAction(playerNode, "NNPlayer_TalkAction", "Start");
//		return;
//										}
//
//	default:
//		break;
//	}
//}

void NNPlayerPanel::playerTalk(NNPlayer& player,CMD_GR_C_TableTalk* pNetInfo)//显示对话
{
	if (!player.isValid())
		return;
	word visioChairID = player.getVisioChairID();
	auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNPlayer_", visioChairID));
	std::string kStrNet = pNetInfo->strString;
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
	{
		if (!WidgetFun::getChildWidget(playerNode, "NNPlayer_TalkAction"))
			return;
		WidgetFun::runWidgetAction(playerNode, "NNPlayer_TalkAction", "Start");
		WidgetFun::setVisible(playerNode, true);
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
	{
		if (!WidgetFun::getChildWidget(playerNode,"NNPlayer_TalkTxtNode"))
			return;
		WidgetFun::runPaoMaDeng(playerNode,"NNPlayer_TalkTxt","NNPlayer_LayoutNode",kStrNet,NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"NNPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(6.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
	{
		if (!WidgetFun::getChildWidget(playerNode,"NNPlayer_BiaoQingNode"))
			return;
		WidgetFun::setImagic(playerNode,"NNPlayer_BiaoQingPic",kStrNet);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"NNPlayer_BiaoQingNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(4.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		if (!WidgetFun::getChildWidget(playerNode, "NNPlayer_TalkAction"))
			return;
		WidgetFun::runWidgetAction(playerNode, "NNPlayer_TalkAction", "Start");
		std::vector<std::string> kStr = utility::split(kStrNet,":");
		ASSERT(kStr.size()==2);
		SoundFun::Instance().playEffect(kStr[0]);
		WidgetFun::runPaoMaDeng(playerNode,"NNPlayer_TalkTxt","NNPlayer_LayoutNode",kStr[1],NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"NNPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(6.0f,false));
	}
}
