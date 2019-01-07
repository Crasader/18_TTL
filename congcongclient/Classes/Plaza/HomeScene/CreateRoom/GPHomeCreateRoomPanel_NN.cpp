#include "GPHomeCreateRoomPanel_NN.h"
#include "Plaza/GameManager/GPGameLink.h"
#include "Game/NN/NNGameScene.h"
#include "Plaza/HomeScene/GPHomeEditNumPanel.h"

#include "Game/NN/CMD_NN.h"
#include "UTILITY.h"
#include USERINFO

FV_SINGLETON_STORAGE(GPHomeCreateRoomPanel_NN);

GPHomeCreateRoomPanel_NN::GPHomeCreateRoomPanel_NN()
	: _nRatioRuleIndex(0)//牛牛的规则序号
	, _bShunZi(false)//顺子牛
	, _bTongHua(false)//同花牛
	, _bWuHua(false)//五花牛
	, _bHuLu(false)//葫芦牛
	, _bZhaDan(false)//炸弹牛
	, _bWuXiao(false)//五小牛
	, _bCanTuiZhu(false)
	, _nTuiZhuIndex(0)//闲家推注倍数序号
	, _nGameType(TTLNN::NNGameType::NNGameType_SnatchBankerShowCard)
	, _cbRoundCount(6)//六人场或者八人场
	, _nQiangZhuangBeiShu(1)
	, _dwBaseScore(1)
{
    init();

	_nRoundIndex = cocos2d::UserDefault::getInstance()->getIntegerForKey("roundIndex", 0);
	_nGameType = cocos2d::UserDefault::getInstance()->getIntegerForKey("gametype", TTLNN::NNGameType::NNGameType_SnatchBankerShowCard);
	std::string strRoomRules = cocos2d::UserDefault::getInstance()->getStringForKey("CreateRoomRules", "0");
	_nGameRuleIndex = utility::parseInt(strRoomRules);

}

GPHomeCreateRoomPanel_NN::~GPHomeCreateRoomPanel_NN()
{

}

bool GPHomeCreateRoomPanel_NN::init()
{
    if(!cocos2d::Node::init()) {
        return false;
    }

    initLayout();
    initButton();
	initData();

    return true;
}

void GPHomeCreateRoomPanel_NN::initLayout()
{
    WidgetScenceXMLparse createRoom_NN("GamePlaza/Script/GPHomeCreateRoomPanel_NN.xml", this);
}

void GPHomeCreateRoomPanel_NN::initButton()
{
    WidgetManager::addButtonCB("NN_CreateRoom_ButtonConfirm", this, button_selector(GPHomeCreateRoomPanel_NN::Button_Confirm));
    WidgetManager::addButtonCB("NN_CreateRoom_ButtonClose", this, button_selector(GPHomeCreateRoomPanel_NN::Button_Close));
	WidgetManager::addButtonCB("Check_CreateRoom_DiFen1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_DiFen1));
	WidgetManager::addButtonCB("Check_CreateRoom_DiFen2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_DiFen2));
	WidgetManager::addButtonCB("Check_CreateRoom_DiFen3", this, button_selector(GPHomeCreateRoomPanel_NN::Button_DiFen3));
	WidgetManager::addButtonCB("Check_CreateRoom_DiFen4", this, button_selector(GPHomeCreateRoomPanel_NN::Button_DiFen4));
	//基本规则
	WidgetManager::addButtonCB("btn_tongbi", this, button_selector(GPHomeCreateRoomPanel_NN::Button_TongBi));
	WidgetManager::addButtonCB("btn_ziyou", this, button_selector(GPHomeCreateRoomPanel_NN::Button_ZiYou));
	WidgetManager::addButtonCB("btn_mingpai", this, button_selector(GPHomeCreateRoomPanel_NN::Button_MingPai));
	//局数选择
	WidgetManager::addButtonCB("NN_CreateRoom_RoundCountBox1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_RoundBox1));
	WidgetManager::addButtonCB("NN_CreateRoom_RoundCountBox2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_RoundBox2));
	WidgetManager::addButtonCB("NN_CreateRoom_RoundCountBox3", this, button_selector(GPHomeCreateRoomPanel_NN::Button_RoundBox3));
	//推注倍数
	WidgetManager::addButtonCB("Check_CanTuiZhu", this, button_selector(GPHomeCreateRoomPanel_NN::Button_CanTuiZhu));
	//抢庄倍数
	WidgetManager::addButtonCB("Check_QiangZhuang1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang1));
	WidgetManager::addButtonCB("Check_QiangZhuang2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang2));
	WidgetManager::addButtonCB("Check_QiangZhuang3", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang3));
	WidgetManager::addButtonCB("Check_QiangZhuang4", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang4));
	//翻倍规则
	WidgetManager::addButtonCB("Btn_fanBei0", this, button_selector(GPHomeCreateRoomPanel_NN::Button_FanBei0));
	WidgetManager::addButtonCB("Btn_fanBei1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_FanBei1));
	WidgetManager::addButtonCB("Btn_fanBei2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_FanBei2));
	//特殊牌型
	WidgetManager::addButtonCB("Check_ShunZi", this, button_selector(GPHomeCreateRoomPanel_NN::Button_ShunZi));
	WidgetManager::addButtonCB("Check_TongHua", this, button_selector(GPHomeCreateRoomPanel_NN::Button_TongHua));
	WidgetManager::addButtonCB("Check_WuHua", this, button_selector(GPHomeCreateRoomPanel_NN::Button_WuHua));
	WidgetManager::addButtonCB("Check_HuLu", this, button_selector(GPHomeCreateRoomPanel_NN::Button_HuLu));
	WidgetManager::addButtonCB("Check_ZhaDan", this, button_selector(GPHomeCreateRoomPanel_NN::Button_ZhaDan));
	WidgetManager::addButtonCB("Check_WuXiao", this, button_selector(GPHomeCreateRoomPanel_NN::Button_WuXiao));
	WidgetManager::addButtonCB("Btn_SixRound", this, button_selector(GPHomeCreateRoomPanel_NN::Button_SixRound));
	WidgetManager::addButtonCB("Btn_EightRound", this, button_selector(GPHomeCreateRoomPanel_NN::Button_EightRound));
}

void GPHomeCreateRoomPanel_NN::initData()
{
	resetGameData();
}

void GPHomeCreateRoomPanel_NN::resetGameData()
{
	std::string strRoomRules = cocos2d::UserDefault::getInstance()->getStringForKey("CreateRoomRules", "0");
	_nGameRuleIndex = utility::parseInt(strRoomRules);

	_bAllowedStrangerJoin = false;
	_dwEnterMatchNum = 1;
	_dwOutMatchNum = 1000000;
}

void GPHomeCreateRoomPanel_NN::show()
{
	auto qiangzhuang = WidgetFun::getChildWidget(this, "qiangzhuang");
	auto tuizhu = WidgetFun::getChildWidget(this, "tuizhu");
	std::string strRoomRules = cocos2d::UserDefault::getInstance()->getStringForKey("CreateRoomRules", "0");

	switch (_nGameType)
	{
	case 	TTLNN::NNGameType::NNGameType_NNBanker://牛牛上庄
		break;
	case TTLNN::NNGameType::NNGameType_HostBanker://固定庄家
		break;
	case	TTLNN::NNGameType::NNGameType_SnatchBanker://自由抢庄

		WidgetFun::setVisible(this, "btn_ziyou", false);
		WidgetFun::setVisible(this, "btn_mingbai", true);
		WidgetFun::setVisible(this, "btn_tongbi", true);

		break;
	case	TTLNN::NNGameType::NNGameType_SnatchBankerShowCard://明牌抢庄

		WidgetFun::setVisible(this, "qiangzhuang", true);
		WidgetFun::setVisible(this, "tuizhu", true);

		WidgetFun::setVisible(this, "btn_ziyou", true);
		WidgetFun::setVisible(this, "btn_mingpai", false);
		WidgetFun::setVisible(this, "btn_tongbi", true);

		if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_TZ))) {
			_bCanTuiZhu = true;
		}
		WidgetFun::setChecked(tuizhu, "Check_CanTuiZhu", _bCanTuiZhu);

		//抢庄倍数
		if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_0))) {
			_nQiangZhuangBeiShu = 1;
		}
		else if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_1))) {
			_nQiangZhuangBeiShu = 2;
		}
		else if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_2))) {
			_nQiangZhuangBeiShu = 3;
		}
		else if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_3))) {
			_nQiangZhuangBeiShu = 4;
		}
		WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang1", false);
		WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang2", false);
		WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang3", false);
		WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang4", false);
		switch (_nQiangZhuangBeiShu)
		{
		case 1:
			WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang1", true);
			break;
		case 2:
			WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang2", true);
			break;
		case 3:
			WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang3", true);
			break;
		case 4:
			WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang4", true);
			break;
		}
		WidgetFun::setText(this, "Txt_CreateRoom_DiFen1", "1/2");
		WidgetFun::setText(this, "Txt_CreateRoom_DiFen2", "2/4");
		WidgetFun::setText(this, "Txt_CreateRoom_DiFen3", "4/8");
		WidgetFun::setText(this, "Txt_CreateRoom_DiFen4", "5/10");
		break;
	case	TTLNN::NNGameType::NNGameType_AllCompare://通比牛牛

		WidgetFun::setVisible(this, "btn_ziyou", true);
		WidgetFun::setVisible(this, "btn_mingpai", true);
		WidgetFun::setVisible(this, "btn_tongbi", false);

		WidgetFun::setText(this, "Txt_CreateRoom_DiFen1", "1");
		WidgetFun::setText(this, "Txt_CreateRoom_DiFen2", "2");
		WidgetFun::setText(this, "Txt_CreateRoom_DiFen3", "4");
		WidgetFun::setText(this, "Txt_CreateRoom_DiFen4", "5");

		WidgetFun::setVisible(this, "qiangzhuang", false);
		WidgetFun::setVisible(this, "tuizhu", false);
		break;
	case	TTLNN::NNGameType::NNGameType_NNRatio://牛几赔几
		break;
	case	TTLNN::NNGameType::NNGameType_HLN://葫芦牛
		break;
	}

	//局数选择
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox1", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox2", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox3", false);
	switch (_nRoundIndex) {
	case 0:
		WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox1", true);
		break;
	case 1:
		WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox2", true);
		break;
	case 2:
		WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox3", true);
		break;
	}

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_SZN))) {
		_bShunZi = true;
	}
	//特殊牌型选择
	if (_bShunZi)
		WidgetFun::setChecked(this, "Check_ShunZi", true);
	else
		WidgetFun::setChecked(this, "Check_ShunZi", false);

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_THN))) {
		_bTongHua = true;
	}
	if (_bTongHua)
		WidgetFun::setChecked(this, "Check_TongHua", true);
	else
		WidgetFun::setChecked(this, "Check_TongHua", false);

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_WHN))) {
		_bWuHua = true;
	}
	if (_bWuHua)
		WidgetFun::setChecked(this, "Check_WuHua", true);
	else
		WidgetFun::setChecked(this, "Check_WuHua", false);

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_HLN))) {
		_bHuLu = true;
	}
	if (_bHuLu)
		WidgetFun::setChecked(this, "Check_HuLu", true);
	else
		WidgetFun::setChecked(this, "Check_HuLu", false);

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_ZDN))) {
		_bZhaDan = true;
	}
	if (_bZhaDan)
		WidgetFun::setChecked(this, "Check_ZhaDan", true);
	else
		WidgetFun::setChecked(this, "Check_ZhaDan", false);

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_WXN))) {
		_bWuXiao = true;
	}
	if (_bWuXiao)
		WidgetFun::setChecked(this, "Check_WuXiao", true);
	else
		WidgetFun::setChecked(this, "Check_WuXiao", false);

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Ratio_0))) {
		_nRatioRuleIndex = 0;
	} else if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Ratio_1))) {
		_nRatioRuleIndex = 1;
	}
	if (_nRatioRuleIndex == 0) 
	{
		WidgetFun::setVisible(this, "Txt_FanBei0_1", true);
		WidgetFun::setVisible(this, "Txt_FanBei0_2", false);
	} else {
		WidgetFun::setVisible(this, "Txt_FanBei0_1", false);
		WidgetFun::setVisible(this, "Txt_FanBei0_2", true);
	}

	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_0))) {
		_dwBaseScore = 1;
	} else if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_1))) {
		_dwBaseScore = 2;
	} else if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_2))) {
		_dwBaseScore = 4;
	} else {
		_dwBaseScore = 5;
	}
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen1", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen2", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen3", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen4", false);
	switch (_dwBaseScore)
	{
	case 1:
		WidgetFun::setChecked(this, "Check_CreateRoom_DiFen1", true);
		break;
	case 2:
		WidgetFun::setChecked(this, "Check_CreateRoom_DiFen2", true);
		break;
	case 4:
		WidgetFun::setChecked(this, "Check_CreateRoom_DiFen3", true);
		break;
	case 5:
		WidgetFun::setChecked(this, "Check_CreateRoom_DiFen4", true);
		break;
	default:
		WidgetFun::setChecked(this, "Check_CreateRoom_DiFen1", true);
		break;
	}


	if (FvMask::HasAny(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SixRound))) {
		_cbRoundCount = 6;
	} else {
		_cbRoundCount = 8;
	}
	if (_cbRoundCount == 6) {
		WidgetFun::setChecked(this, "Btn_SixRound", true);
		WidgetFun::setChecked(this, "Btn_EightRound", false);
	} else if(_cbRoundCount == 8) {
		WidgetFun::setChecked(this, "Btn_SixRound", false);
		WidgetFun::setChecked(this, "Btn_EightRound", true);
	}

	int roundIndex = cocos2d::UserDefault::getInstance()->getIntegerForKey("roundIndex", -1);
	if (roundIndex == -1) {
		roundIndex = 0;
	}
	_nRoundIndex = roundIndex;
	_dwEnterMatchNum = 1;
	_dwOutMatchNum = 1;

	auto game_info = GPGameLink::Instance().privateGameInfo();
	std::string kStr;
	if(game_info.bPlayCout[0] <= 0)
		kStr = utility::toString(3, utility::a_u8("局"));
	else
		kStr = utility::toString(game_info.bPlayCout[0], utility::a_u8("局"));
	WidgetFun::setText(this, "NN_CreateRoom_RoundCount1", kStr);

	if (game_info.bPlayCout[1] <= 0)
		kStr = utility::toString(10, utility::a_u8("局"));
	else
		kStr = utility::toString(game_info.bPlayCout[1], utility::a_u8("局"));
	WidgetFun::setText(this, "NN_CreateRoom_RoundCount2", kStr);

	if (game_info.bPlayCout[2] <= 0)
		kStr = utility::toString(20, utility::a_u8("局"));
	else
		kStr = utility::toString(game_info.bPlayCout[2], utility::a_u8("局"));
	WidgetFun::setText(this, "NN_CreateRoom_RoundCount3", kStr);

    setVisible(true);
}

void GPHomeCreateRoomPanel_NN::hide()
{
    setVisible(false);
}

//////////////////////////////////////////////////////////////////////////

void GPHomeCreateRoomPanel_NN::Button_Confirm(cocos2d::Ref*, WidgetUserInfo*)
{
	_nGameRuleIndex = 0;

	switch (_dwBaseScore) {
		case 1:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_0));
			break;
		case 2:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_1));
			break;
		case 4:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_2));
			break;
		case 5:
			//FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_3));
			break;
		default:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Score_0));
			break;
	}

	switch (_nQiangZhuangBeiShu)
	{
		case 1:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_0));
			break;
		case 2:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_1));
			break;
		case 3:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_2));
			break;
		case 4:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SnatchRatio_3));
			break;
	}

	if (_cbRoundCount == 6)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SixRound));

	if (_nRatioRuleIndex == 0)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Ratio_0));
	else
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_Ratio_1));

	if(_bShunZi)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_SZN));
	if(_bTongHua)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_THN));
	if (_bWuHua)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_WHN));
	if (_bHuLu)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_HLN));
	if (_bZhaDan)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_ZDN));
	if (_bWuXiao)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_SpecialRule_WXN));

	if(_bCanTuiZhu)
		FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_TZ));

	switch (_nTuiZhuIndex)
	{
		default:
			break;
		case 1:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_TZRatio_0));
			break;
		case 2:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_TZRatio_1));
			break;
		case 3:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_TZRatio_2));
			break;
		case 4:
			FvMask::Add(_nGameRuleIndex, _MASK_(TTLNN::NNGameRule::NNGameRule_TZRatio_3));
			break;
	}

	cocos2d::UserDefault::getInstance()->setStringForKey("CreateRoomRules", utility::toString(_nGameRuleIndex));
	cocos2d::UserDefault::getInstance()->setIntegerForKey("roundIndex", _nRoundIndex);
	cocos2d::UserDefault::getInstance()->setIntegerForKey("gametype", _nGameType);

	CMD_GR_Create_Private createRoom;
	zeromemory(&createRoom, sizeof(createRoom));
	createRoom.cbGameType = Type_Private;
	createRoom.bPlayCoutIdex = _nRoundIndex;
	createRoom.bGameTypeIdex = _nGameType;
	createRoom.dwGameRuleIdex = _nGameRuleIndex;
	createRoom.bAllowedStrangerJoin = _bAllowedStrangerJoin;
	createRoom.dwEnterMatchNum = _dwEnterMatchNum; // utility::parseInt(WidgetFun::getText(this, "NN_CreateRoom_EnterNum_Txt"));
	createRoom.dwBaseScore = _dwBaseScore;//utility::parseInt(WidgetFun::getText(this,"NN_CreateRoom_GameScore_Txt"));
	createRoom.dwOutMatchNum = _dwOutMatchNum;// utility::parseInt(WidgetFun::getText(this, "NN_CreateRoom_OutNum_Txt"));

	createRoom.wCharNum = _cbRoundCount;

	GPGameLink::Instance().CreateRoom(NNGameScene::KIND_ID, createRoom);
}

void GPHomeCreateRoomPanel_NN::Button_Close(cocos2d::Ref*, WidgetUserInfo*)
{
    hide();
}

void GPHomeCreateRoomPanel_NN::Button_AllowedStranger(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	_bAllowedStrangerJoin = !_bAllowedStrangerJoin;
}

void GPHomeCreateRoomPanel_NN::Button_RoundBox1(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox1", true);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox2", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox3", false);
	_nRoundIndex = 0;
}

void GPHomeCreateRoomPanel_NN::Button_RoundBox2(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox1", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox2", true);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox3", false);
	_nRoundIndex = 1;
}

void GPHomeCreateRoomPanel_NN::Button_RoundBox3(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox1", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox2", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox3", true);
	_nRoundIndex = 2;
}

void GPHomeCreateRoomPanel_NN::Button_CanTuiZhu(cocos2d::Ref*, WidgetUserInfo*)
{
	_bCanTuiZhu = !_bCanTuiZhu;
	auto tuizhu = WidgetFun::getChildWidget(this, "tuizhu");
	WidgetFun::setChecked(tuizhu, "Check_CanTuiZhu", _bCanTuiZhu);
}

void GPHomeCreateRoomPanel_NN::Button_ShunZi(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_bShunZi) 
	{
		WidgetFun::setChecked(this, "Check_ShunZi", false);
		_bShunZi = false;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_ShunZi", true);
		_bShunZi = true;
	}
}

void GPHomeCreateRoomPanel_NN::Button_TongHua(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_bTongHua)
	{
		WidgetFun::setChecked(this, "Check_TongHua", false);
		_bTongHua = false;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_TongHua", true);
		_bTongHua = true;
	}
}

void GPHomeCreateRoomPanel_NN::Button_WuHua(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_bWuHua)
	{
		WidgetFun::setChecked(this, "Check_WuHua", false);
		_bWuHua = false;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_WuHua", true);
		_bWuHua = true;
	}
}

void GPHomeCreateRoomPanel_NN::Button_HuLu(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_bHuLu)
	{
		WidgetFun::setChecked(this, "Check_HuLu", false);
		_bHuLu = false;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_HuLu", true);
		_bHuLu = true;
	}
}

void GPHomeCreateRoomPanel_NN::Button_ZhaDan(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_bZhaDan)
	{
		WidgetFun::setChecked(this, "Check_ZhaDan", false);
		_bZhaDan = false;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_ZhaDan", true);
		_bZhaDan = true;
	}
}

void GPHomeCreateRoomPanel_NN::Button_WuXiao(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_bWuXiao)
	{
		WidgetFun::setChecked(this, "Check_WuXiao", false);
		_bWuXiao = false;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_WuXiao", true);
		_bWuXiao = true;
	}
}

void GPHomeCreateRoomPanel_NN::Button_FanBei0(cocos2d::Ref *, WidgetUserInfo *)
{
	auto pFanBei = WidgetFun::getChildWidget(this, "Img_fanBei");
	pFanBei->setVisible(true);
	if (_nRatioRuleIndex == 0) {
		WidgetFun::setVisible(pFanBei, "Btn_fanBei1_checked", true);
		WidgetFun::setVisible(pFanBei, "Btn_fanBei1_unchecked", false);
		WidgetFun::setVisible(pFanBei, "Btn_fanBei2_checked", false);
		WidgetFun::setVisible(pFanBei, "Btn_fanBei2_unchecked", true);
	} else {
		WidgetFun::setVisible(pFanBei, "Btn_fanBei1_checked", false);
		WidgetFun::setVisible(pFanBei, "Btn_fanBei1_unchecked", true);
		WidgetFun::setVisible(pFanBei, "Btn_fanBei2_checked", true);
		WidgetFun::setVisible(pFanBei, "Btn_fanBei2_unchecked", false);
	}
}

void GPHomeCreateRoomPanel_NN::Button_FanBei1(cocos2d::Ref *, WidgetUserInfo *)
{
	WidgetFun::setVisible(this, "Img_fanBei", false);
	WidgetFun::setVisible(this, "Txt_FanBei0_1", true);
	WidgetFun::setVisible(this, "Txt_FanBei0_2", false);
	_nRatioRuleIndex = 0;
}

void GPHomeCreateRoomPanel_NN::Button_FanBei2(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setVisible(this, "Img_fanBei", false);
	WidgetFun::setVisible(this, "Txt_FanBei0_1", false);
	WidgetFun::setVisible(this, "Txt_FanBei0_2", true);
	_nRatioRuleIndex = 1;
}

void GPHomeCreateRoomPanel_NN::Button_DiFen1(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen1", true);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen2", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen3", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen4", false);
	_dwBaseScore = 1;
}

void GPHomeCreateRoomPanel_NN::Button_DiFen2(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen1", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen2", true);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen3", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen4", false);
	_dwBaseScore = 2;
}

void GPHomeCreateRoomPanel_NN::Button_DiFen3(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen1", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen2", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen3", true);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen4", false);
	_dwBaseScore = 4;
}

void GPHomeCreateRoomPanel_NN::Button_DiFen4(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen1", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen2", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen3", false);
	WidgetFun::setChecked(this, "Check_CreateRoom_DiFen4", true);
	_dwBaseScore = 5;
}

void GPHomeCreateRoomPanel_NN::Button_QiangZhuang1(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	auto qiangzhuang = WidgetFun::getChildWidget(this, "qiangzhuang");
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang1", true);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang2", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang3", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang4", false);
	_nQiangZhuangBeiShu = 1;
}

void GPHomeCreateRoomPanel_NN::Button_QiangZhuang2(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	auto qiangzhuang = WidgetFun::getChildWidget(this, "qiangzhuang");
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang1", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang2", true);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang3", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang4", false);
	_nQiangZhuangBeiShu = 2;
}

void GPHomeCreateRoomPanel_NN::Button_QiangZhuang3(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	auto qiangzhuang = WidgetFun::getChildWidget(this, "qiangzhuang");
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang1", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang2", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang3", true);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang4", false);
	_nQiangZhuangBeiShu = 3;
}

void GPHomeCreateRoomPanel_NN::Button_QiangZhuang4(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	auto qiangzhuang = WidgetFun::getChildWidget(this, "qiangzhuang");
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang1", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang2", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang3", false);
	WidgetFun::setChecked(qiangzhuang, "Check_QiangZhuang4", true);
	_nQiangZhuangBeiShu = 4;
}

void GPHomeCreateRoomPanel_NN::Button_SixRound(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setChecked(this, "Btn_SixRound", true);
	WidgetFun::setChecked(this, "Btn_EightRound", false);
	_cbRoundCount = 6;
}

void GPHomeCreateRoomPanel_NN::Button_EightRound(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setChecked(this, "Btn_SixRound", false);
	WidgetFun::setChecked(this, "Btn_EightRound", true);
	_cbRoundCount = 8;
}

void GPHomeCreateRoomPanel_NN::Button_MingPai(cocos2d::Ref *, WidgetUserInfo *)
{
	WidgetFun::setVisible(this, "btn_mingpai", false);
	WidgetFun::setVisible(this, "btn_tongbi", true);
	WidgetFun::setVisible(this, "btn_ziyou", true);
	_nGameType = TTLNN::NNGameType::NNGameType_SnatchBankerShowCard;
	show();
}

void GPHomeCreateRoomPanel_NN::Button_TongBi(cocos2d::Ref *, WidgetUserInfo *)
{
	WidgetFun::setVisible(this, "btn_mingpai", true);
	WidgetFun::setVisible(this, "btn_tongbi", false);
	WidgetFun::setVisible(this, "btn_ziyou", true);
	_nGameType = TTLNN::NNGameType::NNGameType_AllCompare;
	show();
}

void GPHomeCreateRoomPanel_NN::Button_ZiYou(cocos2d::Ref *, WidgetUserInfo *)
{
	WidgetFun::setVisible(this, "btn_mingpai", true);
	WidgetFun::setVisible(this, "btn_tongbi", true);
	WidgetFun::setVisible(this, "btn_ziyou", false);
	_nGameType = TTLNN::NNGameType::NNGameType_SnatchBanker;
	show();
}

//////////////////////////////////////////////////////////////////////////

void GPHomeCreateRoomPanel_NN::updateScore(byte cbIndex,dword wScore)
{
	switch (cbIndex)
	{
	case 1:
		{
			//WidgetFun::setText(this,"NN_CreateRoom_GameScore_Txt",wScore);
			//WidgetFun::setText(this,"NN_CreateRoom_EnterNum_Txt",wScore*40);
			//WidgetFun::setText(this,"NN_CreateRoom_OutNum_Txt",wScore*30);
			break;
		}
	case 2:
		WidgetFun::setText(this,"NN_CreateRoom_EnterNum_Txt",wScore);
	break;
	case 3:
		WidgetFun::setText(this,"NN_CreateRoom_OutNum_Txt",wScore);
	break;
	default:
	break;
	}
}
