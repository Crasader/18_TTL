#include "TLJDismissRoomConfirm.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "../TLJGameScence.h"

FV_SINGLETON_STORAGE(TLJDismissRoomConfirm);
TLJDismissRoomConfirm::TLJDismissRoomConfirm()
{
	init();
}

TLJDismissRoomConfirm::~TLJDismissRoomConfirm()
{

}

bool TLJDismissRoomConfirm::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initLayout();
	initButton();
	return true;
}

void TLJDismissRoomConfirm::initLayout()
{
	WidgetScenceXMLparse dismissRoom("Game/TLJ/Script/DismissRoomConfirmPanel.xml", this);
}

void TLJDismissRoomConfirm::initButton()
{
	WidgetManager::addButtonCB("DismissRoomConfirm_ButtonRefuse", this, button_selector(TLJDismissRoomConfirm::Button_Refuse));
	WidgetManager::addButtonCB("DismissRoomConfirm_ButtonConfirm", this, button_selector(TLJDismissRoomConfirm::Button_Confirm));
}

void TLJDismissRoomConfirm::updateInfo()
{
	for (int index = 0; index < TLJGameScence::MAX_PLAYER; ++index){
		TLJPlayer* player = TLJGameScence::Instance().getPlayerByChairID(index);
		
		int agreeType = -1;
		auto playerNode = WidgetFun::getChildWidget(this, utility::toString("DismissConfirmPlayer_", (int)(player->getVisioChairID())));
		if (!player->isValid()) {
			playerNode->setVisible(false);
			continue;
		}else{
			playerNode->setVisible(true);
		}

		for (unsigned int agreeIndex = 0; agreeIndex < m_DismissInfo.dwDissUserCout; ++agreeIndex) {
			if (index == m_DismissInfo.dwDissChairID[agreeIndex]) {
				agreeType = 1;
				break;
			}
		}

		for (unsigned int disagreeIndex = 0; disagreeIndex < m_DismissInfo.dwNotAgreeUserCout; ++disagreeIndex) {
			if (index == m_DismissInfo.dwNotAgreeChairID[disagreeIndex]) {
				agreeType = 0;
				break;
			}
		}

		if (agreeType == 1) {
			WidgetFun::setText(WidgetFun::getChildWidget(playerNode, "Dismiss_ConfirmStatus"), utility::a_u8("Í¬Òâ"));
		} else if (agreeType == 0) {
			WidgetFun::setText(WidgetFun::getChildWidget(playerNode, "Dismiss_ConfirmStatus"), utility::a_u8("¾Ü¾ø"));
		}

		if (player->GetUserID() == UserInfo::Instance().getUserID()) {
			if (agreeType == -1) {
				WidgetFun::setVisible(this, "DismissRoomConfirm_ButtonConfirm", true);
				WidgetFun::setVisible(this, "DismissRoomConfirm_ButtonRefuse", true);
			} else {
				WidgetFun::setVisible(this, "DismissRoomConfirm_ButtonConfirm", false);
				WidgetFun::setVisible(this, "DismissRoomConfirm_ButtonRefuse", false);
			}
		}

		auto playerAvatarNode = WidgetFun::getChildWidget(playerNode, "DismissConfirmPlayer_Avatar");
		if (playerAvatarNode != nullptr) {
			auto pClip = cocos2d::ClippingNode::create();
			auto sprite = cocos2d::Sprite::create("Game/TLJ/TLJPlayer/avatar_male.png");
			auto stencil = cocos2d::Sprite::create("Game/TLJ/TLJPlayer/Avatar.png");
			auto size = stencil->getContentSize();
			stencil->setScale(playerAvatarNode->getContentSize().width / size.width * 0.75f);
			pClip->setAlphaThreshold(.7f);
			pClip->setStencil(stencil);
			pClip->addChild(sprite);
			pClip->setInverted(false);
			playerAvatarNode->removeAllChildren();
			playerAvatarNode->addChild(pClip);
			playerAvatarNode->addChild(cocos2d::Sprite::create("Game/TLJ/DismissRoom/DismissAvatarBorder.png"));
			ImagicDownManager::Instance().addDown(sprite, player->GetHeadHttp(), player->GetUserID());
		}
	}
}

void TLJDismissRoomConfirm::show(CMD_GF_Private_Dismiss_Info& dismissInfo)
{
	m_DismissInfo = dismissInfo;
	removeAllChildren();
	initLayout();
	initButton();
	updateInfo();
	setVisible(true);
}

void TLJDismissRoomConfirm::hide()
{
	setVisible(false);
}

void TLJDismissRoomConfirm::Button_Refuse(cocos2d::Ref*, WidgetUserInfo*)
{
	GPGameLink::Instance().DismissRoom(false);
}

void TLJDismissRoomConfirm::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
	GPGameLink::Instance().DismissRoom();
}
