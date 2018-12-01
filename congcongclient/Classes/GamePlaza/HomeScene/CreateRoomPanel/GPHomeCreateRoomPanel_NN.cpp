#include "GPHomeCreateRoomPanel_NN.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNGameScene.h"
#include "../EditNumPanel/GPHomeEditNumPanel.h"

#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"

USING_NS_CC;
FV_SINGLETON_STORAGE(GPHomeCreateRoomPanel_NN);

GPHomeCreateRoomPanel_NN::GPHomeCreateRoomPanel_NN()
	: _nRatioRuleIndex(0)//牛牛的规则序号
	, _bShunZi(true)//顺子牛
	, _bTongHua(true)//同花牛
	, _bWuHua(true)//五花牛
	, _bHuLu(true)//葫芦牛
	, _bZhaDan(true)//炸弹牛
	, _bWuXiao(true)//五小牛
	, _bCanTuiZhu(false)
	, _nTuiZhuIndex(0)//闲家推注倍数序号
	, _nGameType(TTLNN::NNGameType::NNGameType_SnatchBankerShowCard)
	, _nRoundIndex(0)
	, _bSixRound(true)//六人场或者八人场
	, _nQiangZhuangBeiShu(1)
	, _dwBaseScore(1)
{
    init();
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
	//局数选择
	WidgetManager::addButtonCB("NN_CreateRoom_RoundCountBox1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_RoundBox1));
	WidgetManager::addButtonCB("NN_CreateRoom_RoundCountBox2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_RoundBox2));
	WidgetManager::addButtonCB("NN_CreateRoom_RoundCountBox3", this, button_selector(GPHomeCreateRoomPanel_NN::Button_RoundBox3));
	//推注倍数
	//WidgetManager::addButtonCB("Check_TuiZhu_1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_TuiZhu1));
	WidgetManager::addButtonCB("Check_TuiZhu_2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_TuiZhu2));
	WidgetManager::addButtonCB("Check_TuiZhu_3", this, button_selector(GPHomeCreateRoomPanel_NN::Button_TuiZhu3));
	WidgetManager::addButtonCB("Check_TuiZhu_4", this, button_selector(GPHomeCreateRoomPanel_NN::Button_TuiZhu4));

	WidgetManager::addButtonCB("Check_CanTuiZhu", this, button_selector(GPHomeCreateRoomPanel_NN::Button_CanTuiZhu));

	//抢庄倍数
	WidgetManager::addButtonCB("Check_QiangZhuang1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang1));
	WidgetManager::addButtonCB("Check_QiangZhuang2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang2));
	WidgetManager::addButtonCB("Check_QiangZhuang3", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang3));
	WidgetManager::addButtonCB("Check_QiangZhuang4", this, button_selector(GPHomeCreateRoomPanel_NN::Button_QiangZhuang4));
	//翻倍规则
	WidgetManager::addButtonCB("Check_Multiple_Rule1", this, button_selector(GPHomeCreateRoomPanel_NN::Button_MutipleRule1));
	WidgetManager::addButtonCB("Check_Multiple_Rule2", this, button_selector(GPHomeCreateRoomPanel_NN::Button_MutipleRule2));
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
	_nGameRuleIndex = 0;

	_bAllowedStrangerJoin = false;
	_dwEnterMatchNum = 1;
	_dwOutMatchNum = 1000000;
}

void GPHomeCreateRoomPanel_NN::show()
{
	//局数选择
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox1", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox2", false);
	WidgetFun::setChecked(this, "NN_CreateRoom_RoundCountBox3", false);
	switch (_nRoundIndex)
	{
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

	//闲家推注选择
	//WidgetFun::setChecked(this, "Check_TuiZhu_1", false);
	WidgetFun::setChecked(this, "Check_TuiZhu_2", false);
	WidgetFun::setChecked(this, "Check_TuiZhu_3", false);
	switch (_nTuiZhuIndex)
	{
	case 1:
		//WidgetFun::setChecked(this, "Check_TuiZhu_1", true);
		break;
	case 2:
		WidgetFun::setChecked(this, "Check_TuiZhu_2", true);
		break;
	case 3:
		WidgetFun::setChecked(this, "Check_TuiZhu_3", true);
		break;
	case 4:
		WidgetFun::setChecked(this, "Check_TuiZhu_4", true);
		break;
	}
	WidgetFun::setChecked(this, "Check_CanTuiZhu", _bCanTuiZhu);

	//特殊牌型选择
	if (_bShunZi)
		WidgetFun::setChecked(this, "Check_ShunZi", true);
	else
		WidgetFun::setChecked(this, "Check_ShunZi", false);

	if (_bTongHua)
		WidgetFun::setChecked(this, "Check_TongHua", true);
	else
		WidgetFun::setChecked(this, "Check_TongHua", false);

	if (_bWuHua)
		WidgetFun::setChecked(this, "Check_WuHua", true);
	else
		WidgetFun::setChecked(this, "Check_WuHua", false);

	if (_bHuLu)
		WidgetFun::setChecked(this, "Check_HuLu", true);
	else
		WidgetFun::setChecked(this, "Check_HuLu", false);

	if (_bZhaDan)
		WidgetFun::setChecked(this, "Check_ZhaDan", true);
	else
		WidgetFun::setChecked(this, "Check_ZhaDan", false);

	if (_bWuXiao)
		WidgetFun::setChecked(this, "Check_WuXiao", true);
	else
		WidgetFun::setChecked(this, "Check_WuXiao", false);

	if (_nRatioRuleIndex) 
	{
		WidgetFun::setChecked(this, "Check_Multiple_Rule1", false);
		WidgetFun::setChecked(this, "Check_Multiple_Rule2", true);
	}
	else
	{
		WidgetFun::setChecked(this, "Check_Multiple_Rule1", true);
		WidgetFun::setChecked(this, "Check_Multiple_Rule2", false);
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

	WidgetFun::setChecked(this, "Check_QiangZhuang1", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang2", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang3", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang4", false);
	switch (_nQiangZhuangBeiShu)
	{
	case 1:
		WidgetFun::setChecked(this, "Check_QiangZhuang1", true);
		break;
	case 2:
		WidgetFun::setChecked(this, "Check_QiangZhuang2", true);
		break;
	case 3:
		WidgetFun::setChecked(this, "Check_QiangZhuang3", true);
		break;
	case 4:
		WidgetFun::setChecked(this, "Check_QiangZhuang4", true);
		break;
	}

	_dwEnterMatchNum = 1;
	_dwOutMatchNum = 1;

	auto game_info = GPGameLink::Instance().getCurrentGameInfo();
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

	if (_bSixRound)
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
	createRoom.wCharNum =-1;

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

void GPHomeCreateRoomPanel_NN::Button_TuiZhu1(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_nTuiZhuIndex == 1) {
		WidgetFun::setChecked(this, "Check_TuiZhu_1", false);
		_nTuiZhuIndex = 0;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_TuiZhu_1", true);
		WidgetFun::setChecked(this, "Check_TuiZhu_2", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_3", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_4", false);
		_nTuiZhuIndex = 1;
	}
}

void GPHomeCreateRoomPanel_NN::Button_TuiZhu2(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_nTuiZhuIndex == 2) {
		WidgetFun::setChecked(this, "Check_TuiZhu_2", false);
		_nTuiZhuIndex = 0;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_TuiZhu_1", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_2", true);
		WidgetFun::setChecked(this, "Check_TuiZhu_3", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_4", false);
		_nTuiZhuIndex = 2;
	}
}

void GPHomeCreateRoomPanel_NN::Button_TuiZhu3(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_nTuiZhuIndex == 3) {
		WidgetFun::setChecked(this, "Check_TuiZhu_1", false);
		_nTuiZhuIndex = 0;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_TuiZhu_1", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_2", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_3", true);
		WidgetFun::setChecked(this, "Check_TuiZhu_4", false);
		_nTuiZhuIndex = 3;
	}
}

void GPHomeCreateRoomPanel_NN::Button_TuiZhu4(cocos2d::Ref*, WidgetUserInfo*)
{
	if (_nTuiZhuIndex == 4) {
		WidgetFun::setChecked(this, "Check_TuiZhu_1", false);
		_nTuiZhuIndex = 0;
	}
	else
	{
		WidgetFun::setChecked(this, "Check_TuiZhu_1", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_2", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_3", false);
		WidgetFun::setChecked(this, "Check_TuiZhu_4", true);
		_nTuiZhuIndex = 4;
	}
}

void GPHomeCreateRoomPanel_NN::Button_CanTuiZhu(cocos2d::Ref*, WidgetUserInfo*)
{
	_bCanTuiZhu = !_bCanTuiZhu;
	WidgetFun::setChecked(this, "Check_CanTuiZhu", _bCanTuiZhu);
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

void GPHomeCreateRoomPanel_NN::Button_MutipleRule1(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setChecked(this, "Check_Multiple_Rule1", true);
	WidgetFun::setChecked(this, "Check_Multiple_Rule2", false);
	_nRatioRuleIndex = 0;
}

void GPHomeCreateRoomPanel_NN::Button_MutipleRule2(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setChecked(this, "Check_Multiple_Rule1", false);
	WidgetFun::setChecked(this, "Check_Multiple_Rule2", true);
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
	WidgetFun::setChecked(this, "Check_QiangZhuang1", true);
	WidgetFun::setChecked(this, "Check_QiangZhuang2", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang3", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang4", false);
	_nQiangZhuangBeiShu = 1;
}

void GPHomeCreateRoomPanel_NN::Button_QiangZhuang2(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	WidgetFun::setChecked(this, "Check_QiangZhuang1", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang2", true);
	WidgetFun::setChecked(this, "Check_QiangZhuang3", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang4", false);
	_nQiangZhuangBeiShu = 2;
}

void GPHomeCreateRoomPanel_NN::Button_QiangZhuang3(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	WidgetFun::setChecked(this, "Check_QiangZhuang1", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang2", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang3", true);
	WidgetFun::setChecked(this, "Check_QiangZhuang4", false);
	_nQiangZhuangBeiShu = 3;
}

void GPHomeCreateRoomPanel_NN::Button_QiangZhuang4(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	WidgetFun::setChecked(this, "Check_QiangZhuang1", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang2", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang3", false);
	WidgetFun::setChecked(this, "Check_QiangZhuang4", true);
	_nQiangZhuangBeiShu = 4;
}

void GPHomeCreateRoomPanel_NN::Button_RoomType_MPQZ(cocos2d::Ref*, WidgetUserInfo* pInfo)
{
	_nGameType = TTLNN::NNGameType::NNGameType_SnatchBankerShowCard;
}

void GPHomeCreateRoomPanel_NN::Button_SixRound(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setVisible(this, "Btn_SixRound", false);
	WidgetFun::setVisible(this, "Pic_SixRound", true);
	WidgetFun::setVisible(this, "Btn_EightRound", true);
	WidgetFun::setVisible(this, "Pic_EightRound", false);
	_bSixRound = true;
}

void GPHomeCreateRoomPanel_NN::Button_EightRound(cocos2d::Ref*, WidgetUserInfo*)
{
	WidgetFun::setVisible(this, "Btn_SixRound", true);
	WidgetFun::setVisible(this, "Pic_SixRound", false);
	WidgetFun::setVisible(this, "Btn_EightRound", false);
	WidgetFun::setVisible(this, "Pic_EightRound", true);
	_bSixRound = false;
}

//////////////////////////////////////////////////////////////////////////

void GPHomeCreateRoomPanel_NN::updateScore(BYTE cbIndex,dword wScore)
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
