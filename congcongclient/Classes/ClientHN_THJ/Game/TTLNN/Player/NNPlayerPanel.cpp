#include "NNPlayerPanel.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNGameScene.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNRoomInfo.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"

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
}

void NNPlayerPanel::showPlayer(NNPlayer& player)
{
    int visioChairID = player.getVisioChairID();
    if (player.isValid() && visioChairID < NNGameScene::MAX_PLAYER) {
        auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNPlayer_", visioChairID));
        playerNode->setVisible(true);

        if (player.GetUserID() == UserInfo::Instance().getUserID()) {
            if (player.GetUserStatus() < US_READY && NNGameScene::Instance().getGameStatus() < TTLNN::NNGameStatus_Start) {
                NNGameScene::Instance().showReady();
            } else {
                NNGameScene::Instance().hideReady();
            }
        }

		player.setHeadNode(playerNode);

        WidgetFun::setVisible(playerNode, "NNPlayer_BankerBG", NNGameScene::Instance().isBankerUser(player));
        WidgetFun::setVisible(playerNode, "NNPlayer_Banker", NNGameScene::Instance().isBankerUser(player));
        WidgetFun::setVisible(playerNode, "NNPlayer_Ready", player.GetUserStatus() == US_READY);
        WidgetFun::setVisible(playerNode, "NNPlaye_BetsNode", player.getPlayerBets() != 0);
        WidgetFun::setVisible(playerNode, "NNPlayer_Snatch", player.getSnatchBankerRatio() != (WORD)TTLNN::NNSnatchBanker_Invalid);
        WidgetFun::setVisible(playerNode, "NNPlayer_Host", NNRoomInfo::Instance().isHostPlayer(&player));

        WidgetFun::setText(playerNode, "NNPlayer_Name", player.GetNickName());
		
		//DONE:设置玩家分数
		int nScore = NNRoomInfo::Instance().getRoomInfo().kWinLoseScore[visioChairID];
        WidgetFun::setText(playerNode, "NNPlayer_Score", nScore);
		
		WidgetFun::setText(playerNode, "NNPlayer_Bets", player.getPlayerBets());
		
		if (player.getSnatchBankerRatio() != (WORD)TTLNN::NNSnatchBanker_Invalid) {
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
//    WORD visioChairID = player.getVisioChairID();
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
	if (!player.isValid())return;
	WORD visioChairID = player.getVisioChairID();
	auto playerNode = WidgetFun::getChildWidget(this, utility::toString("NNPlayer_", visioChairID));
	std::string kStrNet = pNetInfo->strString;
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
	{
		if (!WidgetFun::getChildWidget(playerNode, "NNPlayer_TalkAction")) return;
		WidgetFun::runWidgetAction(playerNode, "NNPlayer_TalkAction", "Start");
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
	{
		if (!WidgetFun::getChildWidget(playerNode,"NNPlayer_TalkTxtNode"))return;
		utility::runPaoMaDeng(playerNode,"NNPlayer_TalkTxt","NNPlayer_LayoutNode",kStrNet,NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"NNPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
	{
		if (!WidgetFun::getChildWidget(playerNode,"NNPlayer_BiaoQingNode"))return;
		WidgetFun::setImagic(playerNode,"NNPlayer_BiaoQingPic",kStrNet);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"NNPlayer_BiaoQingNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(4.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		if (!WidgetFun::getChildWidget(playerNode, "NNPlayer_TalkAction")) return;
		WidgetFun::runWidgetAction(playerNode, "NNPlayer_TalkAction", "Start");
		std::vector<std::string> kStr = utility::split(kStrNet,":");
		ASSERT(kStr.size()==2);
		SoundFun::Instance().playEffect(kStr[0]);
		utility::runPaoMaDeng(playerNode,"NNPlayer_TalkTxt","NNPlayer_LayoutNode",kStr[1],NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"NNPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));

	}
}

