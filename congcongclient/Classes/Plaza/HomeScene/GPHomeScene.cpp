//#include <cocos2d/cocos/editor-support/cocostudio/ActionTimeline/CSLoader.h>
#include <Game/Game/NoticeMsg.h>
#include <Game/Game/UserInfo.h>

#include "common.h"
#include "constant.h"
#include "GPHomeScene.h"
#include "../GameManager/GPGameLink.h"
#include "CreateRoom/GPHomeCreateRoom.h"
//#include "BtnTurnLayer.h"
#include "Game/NN/NNGameScene.h"
#include "Game/NN/NNCalculate.h"
#include UTILITY_CONVERT
#include UTILITY_LOG
#include IMAGE_DOWN

using namespace experimental;

FV_SINGLETON_STORAGE(GPHomeScene);

GPHomeScene::GPHomeScene()
	: m_selectedItemIndex(0)
	, _img_head(NULL)
	, _nCurrentGameKindID(ScriptData<int>("GameKind").Value())
{
	UserInfo::Instance().addUpPlayerInfoCB(this, QY_CALLFUNC_SELECTOR(GPHomeScene::flushUserInfo));
	UserInfo::Instance().addLoginSucessCB(this, QY_CALLFUNC_SELECTOR(GPHomeScene::onLogonSucess));
	init();
}

GPHomeScene::~GPHomeScene()
{
}

bool GPHomeScene::init()
{
	if (!cocos2d::Node::init()) 
	{
		return false;
	}

	utility::setDebugFileName(utility::toString("nft_test", time(nullptr)));

	WidgetScenceXMLparse homeScript("GamePlaza/Script/GPHomeScene.xml", this);
	initButton();
	initPaoMaDeng();
	initPopupPanels();

	if (_img_head == nullptr) {
		auto pHeadBG = WidgetFun::getChildWidgetByName(this, "Img_HeaderBG");

		auto pos = Vec2(pHeadBG->getContentSize().width/2, pHeadBG->getContentSize().height/2);
		std::string headPath = "GamePlaza/HomeScene/Tou_OutSide.png";
		std::string stencilPath = "GamePlaza/HomeScene/Tou_OutSide.png";

		_img_head = GPSceneManager::createCircleAvatar(WidgetFun::getChildWidgetByName(this,"Button_Header"), headPath, stencilPath, pos);
	}
	float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", Constant::DEFAULT_SOUND);
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", Constant::DEFAULT_EFFECT);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(sound_volume);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effect_volume);
	return true;
}

void GPHomeScene::showGameRoomList(void* data, size_t dataSize)
{
	cocos2d::log("GPHomeRoomListPanel_NN::RefreshListView");

	widget::ListViewEx* pList = WidgetFun::getListViewWidget(this, "View_RoomList");
	pList->removeAllChildren();

	cocos2d::Node* pItemNode = nullptr;
	cocos2d::Node* pRoomInfo = nullptr;
	CMD_GR_INQURE_TABLES_INFO_DATA_BODY* body;

	int offset = 0;
	void *pData = data;
	char* current_data = static_cast<char*>(pData);

	//����ͷ
	int iGameID = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwGameKindID;
	if (iGameID != _nCurrentGameKindID)
		return;
	int iTableNum = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwTablesTotal;
	if (iTableNum == 0) {
		WidgetFun::setVisible(this, "Img_RoomListEmpty", true);
		WidgetFun::setVisible(this, "Txt_RoomListEmpty", true);
		return;
	} else {
		WidgetFun::setVisible(this, "Img_RoomListEmpty", false);
		WidgetFun::setVisible(this, "Txt_RoomListEmpty", false);
	}
	offset = sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_HEAD);	pData = current_data + offset;

	//NNGameScene::pInstance()->clearRoomShareInfo();

	auto& game_info = GPGameLink::Instance().privateGameInfo();
	//����ÿ���ӿؼ�
	for (int i = 0; i < iTableNum; i++)
	{
		body = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_BODY*>(pData));

		pItemNode = WidgetManager::Instance().createWidget("GameRoomInfoListViewItem", pList);
		pRoomInfo = WidgetFun::getChildWidgetByName(pItemNode, "Btn_RoomList_Join");

		if (pItemNode == nullptr)
			continue;
		//�����
		std::string strRoomNo = utility::toString(body->dwTableNum);
		//����
		std::string strRoundCount = utility::toString(game_info.bPlayCout[body->bToTalPlayIndex]);
		//�׷�
		std::string strBaseSorce = utility::toString(body->dwBaseScore, "/", body->dwBaseScore * 2);
		//����
		std::string strSitCout = utility::toString((dword)body->bSitUserSum, "/", (dword)body->bChairSum, utility::a_u8("��"));

		CMD_GF_Private_Room_Info room_info;
		room_info.bCurPeopleNum = body->bSitUserSum;
		room_info.bMaxPeopleNum = body->bChairSum;
		room_info.dwGameRuleIdex = body->dwGameRuleIdex;
		room_info.dwBaseScore = body->dwBaseScore;
		room_info.dwCreateUserID = body->dwCreateUserID;
		room_info.dwRoomNum = body->dwTableNum;
		room_info.dwPlayCout = body->bCurPlayCount;
		room_info.bPlayCoutIdex = body->bToTalPlayIndex;
		room_info.bGameTypeIdex = body->bGameTypeIdex;

		NNGameScene::pInstance()->addRoomshareInfo(&room_info);

		WidgetFun::setText(pRoomInfo, "Txt_RoomNo", strRoomNo);
		WidgetFun::setText(pRoomInfo, "Txt_RoundCount", strRoundCount);
		WidgetFun::setText(pRoomInfo, "Txt_Score", strBaseSorce);
		WidgetFun::setText(pRoomInfo, "Txt_PlayerCount", strSitCout);

		if (i % 2 == 0) {
			WidgetFun::setVisible(pItemNode, "Btn_Invitation0", true);
			WidgetFun::setVisible(pItemNode, "Btn_Invitation1", false);
			WidgetFun::setWidgetUserInfo(pItemNode, "Btn_Invitation0", "RoomID", strRoomNo);
		} else {
			WidgetFun::setVisible(pItemNode, "Btn_Invitation0", false);
			WidgetFun::setVisible(pItemNode, "Btn_Invitation1", true);
			WidgetFun::setWidgetUserInfo(pItemNode, "Btn_Invitation1", "RoomID", strRoomNo);
		}

		WidgetFun::setWidgetUserInfo(pItemNode, "Btn_RoomList_Join", "RoomID", strRoomNo);

		WidgetFun::setVisible(pItemNode, "GameRoomInfoListViewItem", true);

		offset += sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_BODY);	pData = current_data + offset;
	}
	pList->forceDoLayout();
}

void GPHomeScene::initPaoMaDeng()
{
	playPaoMaDeng(WidgetFun::getChildWidgetByName(this,"Speaker"),"SpeakerTxt","SpeakerLayoutNode", utility::a_u8("�������ֻ�ӭ����"),NULL);
}

void GPHomeScene::flushPlayerLevel()
{
	int experInt = CGlobalUserInfo::GetInstance()->GetGlobalUserData()->dwExperience;
	std::string levelName ="����";
	if ( experInt >= 6400) 
		levelName ="̫ξ";
	else if ( experInt >= 3200) 
		levelName ="˾��";
	else if ( experInt >= 1600) 
		levelName ="�󽫾�";
	else if ( experInt >= 800) 
			levelName ="�ξ�";
	else if (experInt >= 400) 
		levelName = "��ξ";
	else if (experInt >= 200) 
		levelName = "����";
	else if (experInt >= 100) 
		levelName = "��ͷ";
	else if (experInt >= 50) 
		levelName = "��Ͻ";
	else levelName = "����";
	WidgetFun::setText(this,"Home_UserLV",utility::a_u8(utility::toString(levelName)));
}

void GPHomeScene::playPaoMaDeng(cocos2d::Node* pRootNode,std::string kTxtNode,std::string kLayoutNode,std::string kTxt,cocos2d::FiniteTimeAction* pNetAction)
{
	cocos2d::Node* pTxt = WidgetFun::getChildWidget(pRootNode,kTxtNode);
	cocos2d::Node* pLayoutNode = WidgetFun::getChildWidget(pRootNode,kLayoutNode);
	WidgetFun::setText(pTxt,kTxt);
	float fMaxHeight = pLayoutNode->getContentSize().height;
	float fMaxWidth = pLayoutNode->getContentSize().width;
	pTxt->setPositionX(fMaxWidth);
	pTxt->stopAllActions();
	pTxt->setVisible(true);
	auto funBack = cocos2d::CCCallFunc::create([=](){
		std::string msg = NoticeMsg::Instance().getSystemMessage();
		WidgetFun::setText(pTxt,utility::a_u8(msg));
		float fTxtWidth = pTxt->getContentSize().width;
		float fPosX =pTxt->getPositionX();
		if (fPosX < -fTxtWidth){
			pTxt->setPositionX(fMaxWidth);
		}else{
			pTxt->setPositionX(fPosX-1);
		}
	});
	cocos2d::CCAction * seq1 = cocos2d::CCSequence::create(
		cocos2d::CCDelayTime::create(0.001f),
		funBack,
		NULL);
	auto rep = cocos2d::CCRepeatForever::create(dynamic_cast<cocos2d::ActionInterval*>(seq1));
	pTxt->runAction(rep);
}

//////////////////////////////////////////////////////////////////////////

void GPHomeScene::onEnterScene()
{
	initPublicPanel();
	//////////////////////////////////////////////////////////////////////////
	hideAllPanels();
	flushUserInfo();

	GPGameLink::pInstance()->ConnectAndInqureTables(NNGameScene::KIND_ID);
	setButtonsEnable(true);
	SoundFun::Instance().playBackMusic("bgplay.mp3");
}

bool GPHomeScene::initPublicPanel()
{
	if (GPHomeSettingPanel::pInstance()->getParent() != this) {
		GPHomeSettingPanel::pInstance()->removeFromParent();
		addPanel(GPHomeSettingPanel::pInstance());
	}
	//if (GPHomeMallPanel::pInstance()->getParent() != this) {
	//	GPHomeMallPanel::pInstance()->removeFromParent();
	//	addPanel(GPHomeMallPanel::pInstance());
	//}
	if (GPHomeSharePanel::pInstance()->getParent() != this) {
		GPHomeSharePanel::pInstance()->removeFromParent();
		addPanel(GPHomeSharePanel::pInstance());
	}
	return true;
}

void GPHomeScene::initPopupPanels()
{
	initPublicPanel();

	//////////////////////////////////////////////////////////////////////////

	//addPanel(GPHomeUserInfoPanel::pInstance());
	addPanel(GPHomeEnterRoomPanel::pInstance());
	addPanel(GPHomeRecordPanel::pInstance());
	//addPanel(GPHomeRankPanel::pInstance());
	//addPanel(GPHomeEditNumPanel::pInstance());

	//////////////////////////////////////////////////////////////////////////

	addPanel(GPHomeCreateRoomPanel_NN::pInstance());
	//DONE:���ӽ������
	addPanel(NNCalculate::pInstance());
	NNCalculate::pInstance()->hide();

	//////////////////////////////////////////////////////////////////////////

	//DONE:û���õ�
	//addPanel(GPHomePromotionPanel::pInstance());
	//addPanel(GPHomeMorePanel::pInstance());
	//addPanel(GPHomeServicePanel::pInstance());
}

//DONE:ʵ��û�б����ù�
void GPHomeScene::initGamePanels()
{
	//��������
	addPanel(GPHomeRoomListPanel_SRLF::pInstance());
	addPanel(GPHomeCreateRoomPanel_SRLF::pInstance());
	//ţţ
	addPanel(GPHomeRoomListPanel_NN::pInstance());
	//addPanel(GPHomeCreateRoomPanel_NN::pInstance());
	//��������
	addPanel(GPHomeRoomListPanel_TLJ::pInstance());
	addPanel(GPHomeCreateRoomPanel_TLJ::pInstance());
	//��������
	addPanel(GPHomeRoomListPanel_SG::pInstance());
	addPanel(GPHomeCreateRoomPanel_SG::pInstance());
	//�����巿��
	addPanel(GPHomeRoomListPanel_WZQ::pInstance());
	//�����ŷ���
	addPanel(GPHomeRoomListPanel_XZDD::pInstance());
	addPanel(GPHomeCreateRoomPanel_XZDD::pInstance());
}

void GPHomeScene::addPanel(cocos2d::Node* pNode)
{
	this->addChild(pNode);
}

void GPHomeScene::hideAllPanels()
{
	if (GPHomeSettingPanel::pInstance()->getParent() == this) {
		GPHomeSettingPanel::pInstance()->hide();
	}
	if (GPHomeSharePanel::pInstance()->getParent() == this) {
		GPHomeSharePanel::pInstance()->hide();
	}
	//if (GPHomeUserInfoPanel::pInstance()->getParent() == this) {
	//	GPHomeUserInfoPanel::pInstance()->hide();
	//}
	if (GPHomeEnterRoomPanel::pInstance()->getParent() == this) {
		GPHomeEnterRoomPanel::pInstance()->hide();
	}
	//if (GPHomeMallPanel::pInstance()->getParent() == this) {
	//	GPHomeMallPanel::pInstance()->hide();
	//}
	if (GPHomeRecordPanel::pInstance()->getParent() == this) {
		GPHomeRecordPanel::pInstance()->hide();
	}
	//if (GPHomeRankPanel::pInstance()->getParent() == this) {
	//	GPHomeRankPanel::pInstance()->hide();
	//}
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_NN::pInstance()->hide();
	}
	
	//���뷿�����
	//if (GPHomeEditNumPanel::pInstance()->getParent() == this) {
	//	GPHomeEditNumPanel::pInstance()->hide();
	//}

	//DONE:����Ҫ���ؽ������
	//if (NNCalculate::pInstance()->getParent() == this) {
	//	NNCalculate::pInstance()->hide();
	//}

	//if (GPHomePromotionPanel::pInstance()->getParent() == this) {
	//	GPHomePromotionPanel::pInstance()->hide();
	//}
	//if (GPHomeMorePanel::pInstance()->getParent() == this) {
	//	GPHomeMorePanel::pInstance()->hide();
	//}
	//if (GPHomeServicePanel::pInstance()->getParent() == this) {
	//	GPHomeServicePanel::pInstance()->hide();
	//}
}

//DONE:û�����ù���
void GPHomeScene::hideGamePanels()
{
	//�������������б�
	if (GPHomeRoomListPanel_SRLF::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_SRLF::pInstance()->hide();
	}
	//�������������б�
	if (GPHomeCreateRoomPanel_SRLF::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_SRLF::pInstance()->hide();
	}
	//ţţ����
	if (GPHomeRoomListPanel_NN::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_NN::pInstance()->hide();
	}
	//ƴʮ��������
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_NN::pInstance()->hide();
	}
	//�������ŷ���
	if (GPHomeRoomListPanel_TLJ::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_TLJ::pInstance()->hide();
	}
	//�������Ŵ�������
	if (GPHomeCreateRoomPanel_TLJ::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_TLJ::pInstance()->hide();
	}
	//��������
	if (GPHomeRoomListPanel_SG::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_SG::pInstance()->hide();
	}
	if (GPHomeCreateRoomPanel_SG::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_SG::pInstance()->hide();
	}
	//�����巿��
	if (GPHomeRoomListPanel_WZQ::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_WZQ::pInstance()->hide();
	}
	//�����巿��
	if (GPHomeRoomListPanel_XZDD::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_XZDD::pInstance()->hide();
	}
	if (GPHomeCreateRoomPanel_XZDD::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_XZDD::pInstance()->hide();
	}
}

//DONE:û�����ù���
void GPHomeScene::removeAllPanels()
{
	if (GPHomeSettingPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeSettingPanel::pInstance());
	}
	if (GPHomeSharePanel::pInstance()->getParent() == this) {
		removeChild(GPHomeSharePanel::pInstance());
	}
	//if (GPHomeUserInfoPanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomeUserInfoPanel::pInstance());
	//}
	if (GPHomeEnterRoomPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeEnterRoomPanel::pInstance());
	}
	//if (GPHomeMallPanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomeMallPanel::pInstance());
	//}
	if (GPHomeRecordPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeRecordPanel::pInstance());
	}
	//if (GPHomeRankPanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomeRankPanel::pInstance());
	//}
	//ţţ����Ҫ�ɵ�
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_NN::pInstance());
	}

	//DONE:��Ҫɾ�������з����б�
	//removeGamepanels();
	//if (GPHomePromotionPanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomePromotionPanel::pInstance());
	//}
	//if (GPHomeMorePanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomeMorePanel::pInstance());
	//}
	//if (GPHomeServicePanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomeServicePanel::pInstance());
	//}
}

void GPHomeScene::showGameCalculate(CMD_GF_Private_End_Info* pNetInfo)
{
	NNCalculate::Instance().show(pNetInfo);
}

//DONE:û�����ù���
void GPHomeScene::removeGamepanels()
{
	if (GPHomeRoomListPanel_SRLF::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_SRLF::pInstance());
	}
	if (GPHomeCreateRoomPanel_SRLF::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_SRLF::pInstance());
	}
	if (GPHomeRoomListPanel_NN::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_NN::pInstance());
	}
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_NN::pInstance());
	}
	if (GPHomeRoomListPanel_TLJ::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_TLJ::pInstance());
	}
	if (GPHomeCreateRoomPanel_TLJ::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_TLJ::pInstance());
	}
	//��������
	if (GPHomeRoomListPanel_SG::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_SG::pInstance());
	}
	if (GPHomeCreateRoomPanel_SG::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_SG::pInstance());
	}
	//�����巿��
	if (GPHomeRoomListPanel_WZQ::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_WZQ::pInstance());
	}
	//�����巿��
	if (GPHomeRoomListPanel_XZDD::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_XZDD::pInstance());
	}
	if (GPHomeCreateRoomPanel_XZDD::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_XZDD::pInstance());
	}
	if (GPHomeEditNumPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeEditNumPanel::pInstance());
	}
}

void GPHomeScene::onLogonSucess()
{

}

void GPHomeScene::flushUserInfo()
{
	WidgetFun::setText(this, "Txt_UserID", utility::paseInt(UserInfo::Instance().getUserID(), 6));
	WidgetFun::setText(this, "Txt_UserName",UserInfo::Instance().getUserNicName());
	//WidgetFun::setText(this, "Home_RoomCardNum",UserInfo::Instance().getUserInsure());
	WidgetFun::setText(this, "Txt_GoldNum",UserInfo::Instance().getUserScore());
	//flushPlayerLevel();
	switch (UserInfo::Instance().getGender())
	{
	case UserSex::US_Male:
		WidgetFun::setVisible(this, "Img_UserSex_Male", true);
		WidgetFun::setVisible(this, "Img_UserSex_Female", false);
		break;
	case UserSex::US_Femal:
		WidgetFun::setVisible(this, "Img_UserSex_Male", false);
		WidgetFun::setVisible(this, "Img_UserSex_Female", true);
		break;
	}
	if (UserInfo::Instance().getHeadHttp() != "") {
		ImagicDownManager::Instance().addDown(_img_head,UserInfo::Instance().getHeadHttp(), UserInfo::Instance().getUserID());
		std::string headUrl = "";
		headUrl = cocos2d::UserDefault::getInstance()->getStringForKey("headimgurl", "");
		CCLOG("onGPLoginSuccess getHeadHttp = %s", headUrl.c_str());
		if (headUrl != "") {
			UserDefault::sharedUserDefault()->setStringForKey("headimgurl", "");
			UserInfo::Instance().modeHeadHttp(UserInfo::Instance().getHeadHttp());
		}
	} else {
		//TODO:�����Ѷ�Ϊ֮, ͬʱ����AccountInfo�����Individual������ܵ��º�����������Ѿ�������
		UserInfo::Instance().reqIndividual();
	}
}

#pragma region CGPopularizeSink

void GPHomeScene::onGPNoticeResult(tagGameMatch&	pGameMatchInfo, bool bSucess, const char* pStr)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}

//DONE:û�����ù���
void GPHomeScene::onGPPopularizeHttpRes(const CMD_GP_PopularizeHttpRes& pNetInfo)
{
	auto pNode = WidgetFun::getChildWidgetByName(this,"Img_NoticeView");
	auto pNode_Notice = WidgetFun::getChildWidgetByName(this,"Node_Notice");
	pNode->setVisible(false);
	auto Pos   = pNode->getPosition();
	return;

	Size visibleSize = Director::getInstance()->getVisibleSize();  
	Vec2 origin = Director::getInstance()->getVisibleOrigin();  
	Size winSize = Director::getInstance()->getWinSize(); 
	Size curPageSize = Size(300,419);
	auto pPageView = cocos2d::ui::PageView::create();
	pPageView->setTouchEnabled(true);  
	//pPageView->setSize(Size(winSize.width/2, winSize.height/2));  
	pPageView->setSize(curPageSize);  
	pPageView->setPosition(-Vec2(curPageSize.width/2,curPageSize.height/2));
	//����4��page  
	for (int i=1; i<6; i++) {  
		cocos2d::ui::Layout* layout = cocos2d::ui::Layout::create();  
		//layout->setSize(Size(winSize.width, winSize.height));  
		layout->setSize(curPageSize);
		auto kFileName = utility::toString(cocos2d::FileUtils::getInstance()->getWritablePath(),"GuangGao_",i,".png");
		cocos2d::ui::ImageView* imageView = cocos2d::ui::ImageView::create(kFileName);
		//ImagicDownManager::Instance().addDown(imageView,utility::toString(vStr[0],i,".png"),utility::toString("GuangGao_",i),true);
		//imageView->setScale9Enabled(true);  
		//imageView->setSize(Size(400, 400));  
		imageView->setPosition(Point(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));  
		layout->addChild(imageView);  
		pPageView->addPage(layout);  
	}
	//pPageView->addEventListener((cocos2d::ui::PageView::ccPageViewCallback)CC_CALLBACK_2(GPHomeScene::pageViewEvent,this));
	//pPageView->addEventListenerPageView(this,pagevieweventselector(GPHomeScene::pageViewEvent));  
	pNode_Notice->addChild(pPageView);  
}

#pragma endregion
