#include "GPHomeRecordPanel.h"
#include "GPHomeScene.h"
#include "GAME.h"
#include IMAGE_DOWN
#include UTILITY_CONVERT
#include UTILITY_WIDGET

FV_SINGLETON_STORAGE(GPHomeRecordPanel);

GPHomeRecordPanel::GPHomeRecordPanel()
	: m_GameRecordMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
	, _bActive(false)
	, _touch_begin(0, 0)
	, _touch_end(0, 0)
	, _cur_score_info(nullptr)
	, _inqure_state_update(RIT_Null)
	, _inqure_state_view(RIT_Null)
	, _touch_state(RTS_Null)
	, _bg_offset(0.f)
{
	init();
	m_GameRecordMission.setMissionSink(this);
}

GPHomeRecordPanel::~GPHomeRecordPanel()
{

}

void GPHomeRecordPanel::update(float fpass)
{
	if (_bActive) {
		switch (_touch_state)
		{
		case RTS_Null:
			break;
		case RTS_Touched:
			break;
		case RTS_End:
		default:
			auto pView = WidgetFun::getChildWidget(this, "View");
			_bg_offset = pView->getPosition().y;
			_bg_offset = _bg_offset / 2.f;
			if (_bg_offset < 10.f) {
				_bg_offset = 0;
				_touch_state = RTS_Null;
			}
			WidgetFun::setPos(pView, cocos2d::Vec2(pView->getPosition().x, _bg_offset));
			break;
		}

		//�����Ҫ����
		if (_inqure_state_update != 0) {
			//������ڵ���������û����һ�ѵ�������
			if (!checkNextScoreInfo()) {
				sendRecordToTalList(_inqure_state_update);
			} else {
				initView();
			}
			_inqure_state_update = RIT_Null;
		}
	}
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

#pragma region �ط����ݻص�

void GPHomeRecordPanel::sendRecordToTalList(Record_Inqure_State nUpOrDown)
{
	CMD_GP_GetGameRecord_List kNetInfo;
	memset(&kNetInfo, 0, sizeof(kNetInfo));
	
	_inqure_state_view = nUpOrDown;
	if (checkNextScoreInfo()) {
		initView();
		return;
	}
	//���²���
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
		//����Ѿ��е�ǰ�ļ�¼��, ��鿴��ǰ��ҷ����Ƿ��Ѿ��ظ�
		if (itScore != _mpGameScores.end()) {
			auto& vctUserID = itScore->second.vctUserID;
			if (std::find(vctUserID.begin(), vctUserID.end(), record.dwUserID) == vctUserID.end()) {
				vctUserID.push_back(record.dwUserID);
			} else {
				continue;
			}
			itScore->second.vctNickName.push_back(record.strNickName);
			itScore->second.vctHeadHttp.push_back(record.strHeadHttp);
			itScore->second.vctScore.push_back(record.llScore);
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
			score_info.vctScore.push_back(record.llScore);
			score_info.vctUserID.push_back(record.dwUserID);
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
	//��һ�ŵ���ʾ
	auto pTopList = WidgetFun::getChildWidget(this, "Items_Score1");
	auto pButtomList = WidgetFun::getChildWidget(this, "Items_Score2");

	pTopList->removeAllChildren();
	pButtomList->removeAllChildren();

	if (_cur_score_info == nullptr) {
		WidgetFun::setVisible(this, "View", false);
		return;
	}

	//TODO:�������Ҫ�ɵ�
	WidgetFun::setVisible(this, "Button_Record", false);

	GameScoreInfo& kInfo = *_cur_score_info;

	WidgetFun::setText(this, "Txt_RoomNo", kInfo.dwTableID);
	WidgetFun::setText(this, "Txt_BaseScore", kInfo.dwBaseScore);
	WidgetFun::setText(this, "Txt_RoundSum", kInfo.wDrawCount);
	switch (kInfo.wGameType) {
	case 3:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("������ׯ"));
		break;
	default:
		WidgetFun::setText(this, "Txt_RoomType", utility::a_u8("������ׯ"));
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
		WidgetFun::setText(pItemNode, "Txt_Name", kInfo.vctNickName[idx]);
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
	//}
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

#pragma endregion �ط����ݻص�

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

bool GPHomeRecordPanel::onTouchBegan(Touch* touch, Event* pEvent)
{
	_touch_state = RTS_Touched;
	_touch_begin = touch->getLocation();
	return true;
}

void GPHomeRecordPanel::onTouchMoved(Touch *touch, Event *pEvent)
{
	_bg_offset = touch->getLocation().y - _touch_begin.y;

	auto pView = WidgetFun::getChildWidget(this, "View");
	WidgetFun::setPos(pView, cocos2d::Vec2(pView->getPosition().x, _bg_offset));
}

void GPHomeRecordPanel::onTouchEnded(Touch *touch, Event *pEvent)
{
	if (!_bActive) {
		return;
	}
	_touch_state = RTS_End;
	_touch_end = touch->getLocation();
	isNeedUpdateView();
}

int GPHomeRecordPanel::isNeedUpdateView()
{
	if (_touch_end.y - _touch_begin.y > 100) {
		_inqure_state_update = RIT_Down;
		_inqure_state_view = RIT_Down;
	} else if (_touch_end.y - _touch_begin.y < -100) {
		_inqure_state_update = RIT_Up;
		_inqure_state_view = RIT_Up;
	}
	_touch_begin = cocos2d::Vec2(0, 0);
	_touch_end = cocos2d::Vec2(0, 0);
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
