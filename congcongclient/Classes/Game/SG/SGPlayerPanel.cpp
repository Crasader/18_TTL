#include "SGPlayerPanel.h"
#include "Game/SG/SGGameScence.h"
#include "Game/SG/SGRoomInfo.h"
#include "Game/SG/CMD_SG.h"
#include USERINFO
#include IMAGE_DOWN

FV_SINGLETON_STORAGE(SGPlayerPanel);
SGPlayerPanel::SGPlayerPanel()
{
    init();
}

SGPlayerPanel::~SGPlayerPanel()
{

}

bool SGPlayerPanel::init()
{
    if (!cocos2d::Node::init()) {
        return false;
    }

    initLayout();

    return true;
}

void SGPlayerPanel::initLayout()
{
    WidgetScenceXMLparse playerPanel("SG/Script/SGPlayerPanel.xml", this);
}

void SGPlayerPanel::showPlayer(SGPlayer& player)
{
    int visioChairID = player.getVisioChairID();
    if (player.isValid()) {
        auto playerNode = WidgetFun::getChildWidget(this, utility::toString("SGPlayer_", visioChairID));
        playerNode->setVisible(true);

        if (player.GetUserID() == UserInfo::Instance().getUserID()) {
            if (player.GetUserStatus() < US_READY && SGGameScence::Instance().getGameStatus() < SG::SGGameStatus_Start) {
                SGGameScence::Instance().showReady();
            } else {
                SGGameScence::Instance().hideReady();
            }
        }

        ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(playerNode, "SGPlayer_Avatar"), player.GetHeadHttp(), player.GetUserID());

        WidgetFun::setVisible(playerNode, "SGPlayer_BankerBG", SGGameScence::Instance().isBankerUser(player));
        WidgetFun::setVisible(playerNode, "SGPlayer_Banker", SGGameScence::Instance().isBankerUser(player));
        WidgetFun::setVisible(playerNode, "SGPlayer_Ready", player.GetUserStatus() == US_READY);
        WidgetFun::setVisible(playerNode, "SGPlaye_BetsNode", player.getPlayerBets() != 0);
        //WidgetFun::setVisible(playerNode, "SGPlayer_Snatch", player.getSnatchBankerRatio() != (word)SG::SGSnatchBanker_Invalid);
        WidgetFun::setVisible(playerNode, "SGPlayer_Host", SGRoomInfo::Instance().isHostPlayer(player));

        WidgetFun::setText(playerNode, "SGPlayer_Name", player.GetNickName());
        WidgetFun::setText(playerNode, "SGPlayer_Score", SGRoomInfo::Instance().getPlayerScore(player));
		
		WidgetFun::setText(playerNode, "SGPlayer_Bets", player.getPlayerBets());
		//dword baseScore  = (dword)SGRoomInfo::Instance().getRoomInfo().dwBaseScore;
		//dword bankerScore= (dword)SGGameScene::Instance().getBankerRatio();
		//dword playerBet  = (dword)player.getPlayerBets(); 
		//dword totalbets  = baseScore*bankerScore*playerBet;
		//WidgetFun::setText(playerNode, "SGPlayer_Bets",(int)totalbets);
		
		if (player.getSnatchBankerRatio() != (word)SG::SGSnatchBanker_Invalid) {
            //WidgetFun::setImagic(playerNode, "SGPlayer_Snatch", utility::toString(WidgetFun::getWidgetUserInfo(playerNode, "SGPlayer_Snatch", "Image"), (int)player.getSnatchBankerRatio(), ".png"), false);
			WidgetFun::setImagic(playerNode, "SGPlayer_Snatch", utility::toString(WidgetFun::getWidgetUserInfo(playerNode, "SGPlayer_Snatch", "Image"),1, ".png"), false);
        }
		WidgetFun::setVisible(playerNode, "SGPlayer_Snatch",false);
    }
}

void SGPlayerPanel::playEffectSGPlayer_Snatch(SGPlayer& player,bool isVisible)
{
	int visioChairID = player.getVisioChairID();
	if (player.isValid()) {
		auto playerNode = WidgetFun::getChildWidget(this, utility::toString("SGPlayer_", visioChairID));
		WidgetFun::setVisible(playerNode, "SGPlayer_Snatch",false);
		//if(player.getSnatchBankerRatio() == (word)SG::SGSnatchBanker_None) return;
		auto pTempNode = WidgetFun::getChildWidgetByName(playerNode, "SGPlayer_Snatch");
		WidgetFun::setImagic(playerNode, "SGPlayer_Snatch",utility::toString(WidgetFun::getWidgetUserInfo(pTempNode,"Image"),player.getSnatchBankerRatio(),".png"),false);
		if( NULL != pTempNode )
		{
			pTempNode->setVisible(isVisible);
			//pTempNode->stopAllActions();
			//pTempNode->runAction(cocos2d::Sequence::create(CCShow::create(),DelayTime::create(1.f),CCHide::create(),NULL));
		}
	}
}

void SGPlayerPanel::hidePlayer(int playerVisioChairID)
{
    if (playerVisioChairID < SGGameScence::MAX_PLAYER) {
        WidgetFun::setVisible(this, utility::toString("SGPlayer_", playerVisioChairID), false);
    }
}

//void SGPlayerPanel::playerTalk(SGPlayer& player, CMD_GR_C_TableTalk* pInfo)
//{
//    if (!player.isValid()) {
//        return;
//    }
//
//    word visioChairID = player.getVisioChairID();
//    auto playerNode = WidgetFun::getChildWidget(this, utility::toString("SGPlayer_", visioChairID));
//
//    switch (pInfo->cbType) {
//        case CMD_GR_C_TableTalk::TYPE_FILE: {
//            if (!WidgetFun::getChildWidget(playerNode, "SGPlayer_TalkAction")) {
//                return;
//            }
//
//            WidgetFun::runWidgetAction(playerNode, "SGPlayer_TalkAction", "Start");
//            return;
//        }
//
//        default:
//            break;
//    }
//}

void SGPlayerPanel::playerTalk(SGPlayer& player,CMD_GR_C_TableTalk* pNetInfo)//ÏÔÊ¾¶Ô»°
{
	if (!player.isValid()) {
		return;
	}

	word visioChairID = player.getVisioChairID();
	auto playerNode = WidgetFun::getChildWidget(this, utility::toString("SGPlayer_", visioChairID));
	std::string kStrNet = pNetInfo->strString;
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
	{
		if (!WidgetFun::getChildWidget(playerNode,"SGPlayer_TalkAction"))
		{
			return;
		}
		WidgetFun::runWidgetAction(playerNode,"SGPlayer_TalkAction","Start");
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
	{
		if (!WidgetFun::getChildWidget(playerNode,"SGPlayer_TalkTxtNode"))
		{
			return;
		}
		WidgetFun::runPaoMaDeng(playerNode,"SGPlayer_TalkTxt","SGPlayer_LayoutNode",
			kStrNet,NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"SGPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(6.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
	{
		if (!WidgetFun::getChildWidget(playerNode,"SGPlayer_BiaoQingNode"))
		{
			return;
		}
		WidgetFun::setImagic(playerNode,"SGPlayer_BiaoQingPic",kStrNet);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"SGPlayer_BiaoQingNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(4.0f,false));
	}
	if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
	{
		if (!WidgetFun::getChildWidget(playerNode,"SGPlayer_TalkAction"))
		{
			return;
		}
		std::vector<std::string> kStr = utility::split(kStrNet,":");

		ASSERT(kStr.size()==2);

		WidgetFun::runWidgetAction(playerNode,"SGPlayer_TalkAction","Start");
		SoundFun::Instance().playEffect(kStr[0]);

		WidgetFun::runPaoMaDeng(playerNode,"SGPlayer_TalkTxt","SGPlayer_LayoutNode",kStr[1],NULL);
		cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(playerNode,"SGPlayer_TalkTxtNode");
		pTalkNode->setVisible(true);
		pTalkNode->stopAllActions();
		pTalkNode->runAction(script::CCVisibleAction::create(6.0f,false));

	}
}

