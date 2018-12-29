#include "GPHomeRecordPanel.h"
#include "GPHomeScene.h"
#include "GAME.h"
#include "UTILITY.h"
#include IMAGE_DOWN

FV_SINGLETON_STORAGE(GPHomeRecordPanel);

GPHomeRecordPanel::GPHomeRecordPanel()
	: m_GameRecordMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
	, _bActive(false)
	, _touch_begin(0, 0)
	, _touch_end(0, 0)
	, _cur_score_info(nullptr)
	, _inqure_state_update(RIT_Null)
	, _inqure_state_view(RIT_Null)
	, _view_touch_begin_y(0.f)
	, _move_state(RMS_Null)
	, _bg_offset(0.f)
	, _view_original_y(0.f)
{
	init();
	m_GameRecordMission.setMissionSink(this);
}

GPHomeRecordPanel::~GPHomeRecordPanel()
{

}

bool GPHomeRecordPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}

	initTouchEvent();

	WidgetScenceXMLparse exitPanel("GamePlaza/Script/GPHomeRecordPanel.xml", this);
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeRecordPanel::Button_Cancel));
	return true;
}

void GPHomeRecordPanel::show()
{
	_bActive = true;
	setVisible(true);
}

void GPHomeRecordPanel::hide()
{
	_bActive = false;
	GPHomeScene::Instance().setActiveButtons(true);
	setVisible(false);
}

#pragma region 回放数据回调

void GPHomeRecordPanel::sendRecordToTalList(Record_Inqure_State nUpOrDown)
{
	CMD_GP_GetGameRecord_List kNetInfo;
	memset(&kNetInfo, 0, sizeof(kNetInfo));
	
	_inqure_state_view = nUpOrDown;
	if (checkNextScoreInfo()) {
		initView();
		return;
	}
	//向下查找
	switch(nUpOrDown)
	{
	case RIT_Down:
		kNetInfo.bUpOrDown = 1;
		kNetInfo.dwDrawID = _cur_score_info ? _cur_score_info->dwMinDrawID : 0;
		break;
	case RIT_Up:
		kNetInfo.bUpOrDown = 2;
		kNetInfo.dwDrawID = _cur_score_info ? _cur_score_info->dwMaxDrawID : 0;
		break;
	}
	_inqure_state_view = nUpOrDown;
	kNetInfo.dwUserID = UserInfo::Instance().getUserID();
	m_GameRecordMission.GetGameRecordListEx(kNetInfo);
}

void GPHomeRecordPanel::onGPBackGameRecordListEx(tagGameRecordListEx* pNetInfo)
{
	for (int i = 0; i < (int)pNetInfo->kList.size(); i++) {
		tagGameRecordListItem& record = pNetInfo->kList[i];
		auto itScore = _mpGameScores.find(record.dwStartTime);
		//如果已经有当前的记录了, 则查看当前玩家分数是否已经重复
		if (itScore != _mpGameScores.end()) {
			auto& vctUserID = itScore->second.vctUserID;
			auto it_userID = std::find(vctUserID.begin(), vctUserID.end(), record.dwUserID);
			if (it_userID == vctUserID.end()) {
				vctUserID.push_back(record.dwUserID);
			} else {
				utility::filelog("record.dwUserID = %d", static_cast<int>(record.dwUserID));
				utility::filelog("it_userID = %d", static_cast<int>(*it_userID));
				continue;
			}
			itScore->second.vctNickName.push_back(record.strNickName);
			itScore->second.vctHeadHttp.push_back(record.strHeadHttp);
			itScore->second.vctScore.push_back(static_cast<long long>(record.llScore));
		} else {
			GameScoreInfo score_info;
			score_info.dwKindID = record.dwKindID;
			score_info.wServerID = record.wServerID;
			score_info.dwTableID = record.dwTableID;
			score_info.dwStartTime = record.dwStartTime;
			score_info.dwBaseScore = record.dwBaseScore;
			score_info.dwRulesBytes = record.dwRulesBytes;
			score_info.wGameType = record.wGameType;
			score_info.dwMinDrawID = record.dwMinDrawID;
			score_info.dwMaxDrawID = record.dwMaxDrawID;
			score_info.wDrawCount = record.wDrawCount;
			score_info.vctUserID.push_back(static_cast<dword>(record.dwUserID));
			score_info.vctScore.push_back(static_cast<long long>(record.llScore));
			score_info.vctNickName.push_back(record.strNickName);
			score_info.vctHeadHttp.push_back(record.strHeadHttp);
			_lstScoreInfoIndex.push_back(record.dwStartTime);
			_lstScoreInfoIndex.sort();
			_mpGameScores.insert(std::make_pair(record.dwStartTime, score_info));
		}
	}
	checkNextScoreInfo();
	initView();
}

void GPHomeRecordPanel::initView()
{
	auto pView = WidgetFun::getChildWidget(this, "View");

	auto pClipNode = WidgetFun::getChildWidget(this, "Clp_Panal");
	auto vertext = utility::parsePoints(WidgetFun::getWidgetUserInfo(pClipNode, "ClipeRect"));
	auto pCliper = dynamic_cast<cocos2d::ClippingNode*>(pClipNode);
	auto pDrawNode = dynamic_cast<cocos2d::DrawNode*>(pCliper->getStencil());

	pDrawNode->drawPolygon(&vertext[0], vertext.size(), cocos2d::Color4F::BLACK, 0, cocos2d::Color4F::BLACK);
	pCliper->setStencil(pDrawNode);

	//pCliper->setInverted(true);

	//第一排的显示
	auto pTopList = WidgetFun::getChildWidget(pView, "Items_Score1");
	auto pButtomList = WidgetFun::getChildWidget(pView, "Items_Score2");

	pTopList->removeAllChildren();
	pButtomList->removeAllChildren();

	if (_cur_score_info == nullptr) {
		pView->setVisible(false);
		return;
	}

	GameScoreInfo& kInfo = *_cur_score_info;

	WidgetFun::setText(this, "Txt_RoomNo", kInfo.dwTableID);
	WidgetFun::setText(this, "Txt_BaseScore", kInfo.dwBaseScore);
	WidgetFun::setText(this, "Txt_RoundSum", kInfo.wDrawCount);
	switch (kInfo.wGameType) {
	case 3:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("明牌抢庄"));
		break;
	default:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("明牌抢庄"));
		break;
	}

	time_t tt = kInfo.dwStartTime;
	char tmp[64] = { 0 };
	strftime(tmp, sizeof(tmp), "%Y/%m/%d/ %H:%M:%S", localtime(&tt));
	WidgetFun::setText(this, "Txt_Time", tmp);

	cocos2d::Node* pItemNode = nullptr;
	cocos2d::Vec2 beginPos;
	cocos2d::Vec2 addPos;

	SCORE dayingjia = 0;
	SCORE tuhao = 0;
	for (size_t idx = 0; idx < kInfo.vctScore.size(); idx++) {
		if (dayingjia < kInfo.vctScore[idx]) {
			dayingjia = kInfo.vctScore[idx];
		}
		if (tuhao > kInfo.vctScore[idx]) {
			tuhao = kInfo.vctScore[idx];
		}
	}

	for (size_t idx = 0; idx < kInfo.vctNickName.size(); idx++) {
		if (idx <= 4) {
			pItemNode = WidgetManager::Instance().createWidget("GameReocrdScoreItem", pTopList);
			beginPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "Items_Score1", utility::toString("BeginPos")));
			addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "Items_Score1", utility::toString("Offset"))) * (idx);
		} else {
			pItemNode = WidgetManager::Instance().createWidget("GameReocrdScoreItem", pButtomList);
			beginPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "Items_Score2", utility::toString("BeginPos")));
			addPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(this, "Items_Score2", utility::toString("Offset"))) * (idx);
		}
		WidgetFun::setText(pItemNode, "Txt_Name", utility::a_u8(kInfo.vctNickName[idx]));
		std::string str_id;
		std::string idsmall = utility::toString(kInfo.vctUserID[idx]);
		int nBuWei = 6 - idsmall.size();
		while (nBuWei-- > 0) {
			str_id += "0";
		}
		str_id += idsmall;
		WidgetFun::setText(pItemNode, "Txt_ID", str_id);
		WidgetFun::setText(pItemNode, "Txt_Score", scoreToString(kInfo.vctScore[idx]));
		if (kInfo.vctScore[idx] == dayingjia) {
			WidgetFun::setVisible(pItemNode, "Img_DaYingJia", true);
		} else if (kInfo.vctScore[idx] == tuhao) {
			WidgetFun::setVisible(pItemNode, "Img_TuHao", true);
		}

		auto pHeadNode = WidgetFun::getChildWidget(pItemNode, "head");
		ImagicDownManager::Instance().addDown(pHeadNode, kInfo.vctHeadHttp[idx], kInfo.vctUserID[idx]);

		pItemNode->setPosition(beginPos + addPos);
		pItemNode->setVisible(true);
	}
	_view_original_y = 0;
	_view_touch_begin_y = pView->getPosition().y;
}

void GPHomeRecordPanel::onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo)
{
	m_RecordList = *pNetInfo;	
	widget::ListViewEx* pList = WidgetFun::getListViewWidget(this, "RecordList0");
	pList->removeAllChildren();
	for (int i = 0; i < (int)pNetInfo->kList.size(); i++) {
		tagPrivateRandTotalRecord& kInfo = pNetInfo->kList[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdListItem", pList);
		WidgetFun::setWidgetUserInfo(pItemNode, "Button_ShowDetail", "RecordID", utility::toString(kInfo.iRecordID));
		//WidgetFun::setText(pItemNode, "ChallengeTime", timeToString(kInfo.kPlayTime));
		WidgetFun::setText(pItemNode, "RoomNum", utility::toString(kInfo.iRoomNum));
		if (kInfo.kNickName.size() != kInfo.kScore.size()) {
			continue;
		}
		for (int m = 0; m < (int)kInfo.kNickName.size(); m++) {
			if (m >= 4) {
				continue;
			}
			WidgetFun::setText(pItemNode, utility::toString("PlayerScore", m),scoreToString(kInfo.kScore[m]));
			WidgetFun::setText(pItemNode, utility::toString("PlayerTxt", m),utility::toString(utility::a_u8(kInfo.kNickName[m])));
		}
	}
	pList->forceDoLayout();
}

void GPHomeRecordPanel::onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo)
{
	m_kPrivateRandTotalRecord = *pNetInfo;
	cocos2d::Node* pRootPlane = WidgetFun::getChildWidget(this, "RecordTotalPlane");
	widget::ListViewEx* pList = WidgetFun::getListViewWidget(pRootPlane, "RecordList1");
	pList->removeAllChildren();

	for (int i = 0; i < (int)pNetInfo->kNickName.size(); i++) {
		if (i < 4) {
			WidgetFun::setText(pRootPlane, utility::toString("PlayerName", i), utility::a_u8(pNetInfo->kNickName[i]));
		}
	}

	for (int i = 0; i < (int)pNetInfo->kRecordChild.size(); i++) {
		tagPrivateRandRecordChild& kInfo = pNetInfo->kRecordChild[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdTotalItem", pList);
		WidgetFun::setText(pItemNode, "IdexTxt", utility::toString(i + 1));
		WidgetFun::setText(pItemNode, "PlayTime", timeToString(kInfo.kPlayTime));
		WidgetFun::setWidgetUserInfo(pItemNode, "Button_StartGameRecord", "RecordChildID", utility::toString(kInfo.iRecordChildID));

		for (int m = 0; m < (int)kInfo.kScore.size(); m++) {
			if (m >= 4) {
				continue;
			}

			WidgetFun::setText(pItemNode, utility::toString("PlayerTxt", m), scoreToString(kInfo.kScore[m]));
		}

	}

	pList->forceDoLayout();
}

void GPHomeRecordPanel::onGPBackGameChildRecord(tagPrivateRandRecordChild* pNetInfo)
{
	GPGameManager::Instance().StartHNMJRecord(pNetInfo->kRecordGame);
}

#pragma endregion 回放数据回调

void GPHomeRecordPanel::Button_Cancel(cocos2d::Ref* pRef, WidgetUserInfo * pUserInfo)
{
	hide();
}

std::string GPHomeRecordPanel::timeToString(systemtime kSystem)
{
	std::string kStr;
	kStr = utility::toString(kStr, kSystem.wYear);
	kStr = utility::toString(kStr, "-");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wMonth, 2));
	kStr = utility::toString(kStr, "-");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wDay, 2));
	kStr = utility::toString(kStr, '\n');
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wHour, 2));
	kStr = utility::toString(kStr, ":");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wMinute, 2));
	kStr = utility::toString(kStr, ":");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wSecond, 2));
	return kStr;
}

std::string GPHomeRecordPanel::scoreToString(int iScore)
{
	if (iScore > 0) {
		return utility::toString("+", iScore);
	}

	return utility::toString(iScore);
}

void GPHomeRecordPanel::Button_ShowTotal(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	//WidgetFun::setVisible(this, "RecordTotalPlane", true);
	//WidgetFun::setVisible(this, "RecordListPlane", false);
	//m_GameRecordMission.GetPrivateRandTotalRecord(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RecordID")));
}

void GPHomeRecordPanel::Button_StartGameRecord(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RecordChildID"));
	for (int i = 0; i < (int)m_kPrivateRandTotalRecord.kRecordChild.size(); i++) {
		tagPrivateRandRecordChild& kChild = m_kPrivateRandTotalRecord.kRecordChild[i];
		if (kChild.iRecordChildID == iRecordChildID) {
			GPGameManager::Instance().StartHNMJRecord(kChild.kRecordGame);
		}
	}
}

void GPHomeRecordPanel::initTouchEvent()
{
	scheduleUpdate();

	auto listenerT = EventListenerTouchOneByOne::create();
	listenerT->onTouchBegan = CC_CALLBACK_2(GPHomeRecordPanel::onTouchBegan, this);
	listenerT->onTouchMoved = CC_CALLBACK_2(GPHomeRecordPanel::onTouchMoved, this);
	listenerT->onTouchEnded = CC_CALLBACK_2(GPHomeRecordPanel::onTouchEnded, this);
	listenerT->setSwallowTouches(false);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerT, this);
}

#define auto_move_speed 30.f
#define screen_high 720.f

void GPHomeRecordPanel::update(float fpass)
{
	if (!_bActive)
		return;
	auto pView = WidgetFun::getChildWidget(this, "View");
	if (!pView) return;
	switch (_move_state)
	{
	case RMS_Null:
		break;
	case RMS_TurnPage:
		switch (_inqure_state_update)
		{
		case RIT_Null:
			break;
		case RIT_Up:
			_bg_offset = pView->getPosition().y;
			if (_bg_offset < 0) {
				_bg_offset -= auto_move_speed;
				if (_bg_offset < -screen_high) {
					_bg_offset = screen_high;
				}
			} else if (_bg_offset >= 0) {
				_bg_offset = (_bg_offset * 4 / 5);
				if (utility::fAbs(_bg_offset) <= 10) {
					_bg_offset = 0;
					_move_state = RMS_Null;
					_inqure_state_update = RIT_Null;
				}
			}
			pView->setPosition(pView->getPosition().x, _bg_offset);
			break;
		case RIT_Down:
			_bg_offset = pView->getPosition().y;
			if (_bg_offset > 0) {
				_bg_offset += auto_move_speed;
				if (_bg_offset > screen_high) {
					_bg_offset = -screen_high;
				}
			} else if (_bg_offset <= 0) {
				_bg_offset = (_bg_offset * 4 / 5);
				if (utility::fAbs(_bg_offset) <= 10) {
					_bg_offset = 0;
					_move_state = RMS_Null;
					_inqure_state_update = RIT_Null;
				}
			}
			pView->setPosition(pView->getPosition().x, _bg_offset);
			break;
		default:
			break;
		}
		break;
	case RMS_RollBack:
		_bg_offset = (pView->getPosition().y * 4 / 5);
		if (utility::fAbs(_bg_offset) <= 10) {
			_bg_offset = 0;
			_move_state = RMS_Null;
		}
		pView->setPosition(pView->getPosition().x, _bg_offset);
		break;
	case RMS_Touch:
		break;
	default:
		break;
	}
}

bool GPHomeRecordPanel::onTouchBegan(Touch* touch, Event* pEvent)
{
	if (!_bActive)
		return false;
	auto pView = WidgetFun::getChildWidget(this, "View");
	switch (_move_state)
	{
	case RMS_Null:
		_move_state = RMS_Touch;
		_touch_begin = cocos2d::Vec2(0, 0);
		_touch_end = cocos2d::Vec2(0, 0);
		_bg_offset = 0.f;

		_touch_begin = touch->getLocation();
		CCLOG("onTouchBegan _touch_begin = %f, _touch_end.y = %f, _bg_offset =%f", _touch_begin.y, _touch_end.y, _bg_offset);
		if (pView) {
			_view_touch_begin_y = pView->getPosition().y;
		}
		break;
	case RMS_TurnPage:
		break;
	case RMS_Touch:
		break;
	default:
		break;
	}
	return true;
}

void GPHomeRecordPanel::onTouchMoved(Touch *touch, Event *pEvent)
{
	if (!_bActive)
		return;
	auto pView = WidgetFun::getChildWidget(this, "View");
	switch (_move_state)
	{
	case RMS_Null:
		break;
	case RMS_TurnPage:
		break;
	case RMS_Touch:
		_touch_end = touch->getLocation();
		_bg_offset = _touch_end.y - _touch_begin.y;
		//CCLOG("onTouchMoved _touch_begin = %f, _touch_end.y = %f, _bg_offset =%f", _touch_begin.y, _touch_end.y, _bg_offset);
		pView->setPosition(pView->getPosition().x, _view_touch_begin_y + _bg_offset);
		break;
	default:
		break;
	}
}

void GPHomeRecordPanel::onTouchEnded(Touch *touch, Event *pEvent)
{
	if (!_bActive)
		return;
	auto pView = WidgetFun::getChildWidget(this, "View");
	float offset = 0.f;
	switch (_move_state)
	{
	case RMS_Null:
		break;
	case RMS_TurnPage:
		break;
	case RMS_Touch:
		//CCLOG("onTouchEnded _touch_begin = %f, _touch_end.y = %f, _bg_offset =%f", _touch_begin.y, _touch_end.y, _bg_offset);
		//已经移动的距离
		if (isNeedUpdateView()) {
			_move_state = RMS_TurnPage;
		} else if (pView) {
			_move_state = RMS_RollBack;
		}
		break;
	default:
		break;
	}
}

int GPHomeRecordPanel::isNeedUpdateView()
{
	auto pView = WidgetFun::getChildWidget(this, "View");
	if (pView->getPosition().y > (screen_high / 3)) {
		_inqure_state_update = RIT_Down;
		_inqure_state_view = RIT_Down;
		_touch_begin = cocos2d::Vec2(0, 0);
		_touch_end = cocos2d::Vec2(0, 0);
		if (!checkNextScoreInfo()) {
			sendRecordToTalList(_inqure_state_update);
		} else {
			initView();
		}
	} else if (pView->getPosition().y < -(screen_high / 3)) {
		_inqure_state_update = RIT_Up;
		_inqure_state_view = RIT_Up;
		_touch_begin = cocos2d::Vec2(0, 0);
		_touch_end = cocos2d::Vec2(0, 0);
		if (!checkNextScoreInfo()) {
			sendRecordToTalList(_inqure_state_update);
		} else {
			initView();
		}
	}

	return _inqure_state_update;
}

bool GPHomeRecordPanel::checkNextScoreInfo()
{
	if (_mpGameScores.size() == 1) {
		_cur_score_info = &(_mpGameScores.begin()->second);
	}
	auto rit_score = _lstScoreInfoIndex.rbegin();
	auto it_score = _lstScoreInfoIndex.begin();
	bool bfind = false;
	switch (_inqure_state_view) {
	case RIT_Down:
		rit_score = _lstScoreInfoIndex.rbegin();
		while (rit_score != _lstScoreInfoIndex.rend()) {
			if (_cur_score_info && (*rit_score) == _cur_score_info->dwStartTime) {
				if ((++rit_score) != _lstScoreInfoIndex.rend()) {
					auto it_score_info = _mpGameScores.find(*rit_score);
					if (it_score_info != _mpGameScores.end()) {
						_cur_score_info = &(it_score_info->second);
						bfind = true;
					}
				}
				break;
			}
			rit_score++;
		}
		break;
	case RIT_Up:
		it_score = _lstScoreInfoIndex.begin();
		while (it_score != _lstScoreInfoIndex.end()) {
			if (_cur_score_info && (*it_score) == _cur_score_info->dwStartTime) {
				if ((++it_score) != _lstScoreInfoIndex.end()) {
					auto it_score_info = _mpGameScores.find(*it_score);
					if (it_score_info != _mpGameScores.end()) {
						_cur_score_info = &(it_score_info->second);
						bfind = true;
					}
				}
				break;
			}
			it_score++;
		}
		break;
	}
	_inqure_state_view = RIT_Null;
	return bfind;
}
