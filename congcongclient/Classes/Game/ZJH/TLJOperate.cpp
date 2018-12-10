#include "TLJOperate.h"
#include "TLJGameScence.h"
#include "TLJGameLogic.h"
#include "Game/Game/NoticeMsg.h"
USING_NS_CC_EXT;

FV_SINGLETON_STORAGE(TLJOperate);
TLJOperate::TLJOperate()
	:m_iMode(0)
{
	init();
}

TLJOperate::~TLJOperate()
{
}

bool TLJOperate::init()
{
	if(!cocos2d::Node::init()) {
		return false;
	}

	WidgetScenceXMLparse settingPanel("Game/TLJ/Script/Operate.xml", this);

	initButton();

	return true;
}

void TLJOperate::initButton()
{
	WidgetManager::addButtonCB("TLJ_Operate_BiPai", this, button_selector(TLJOperate::TLJ_Operate_BiPai));
	WidgetManager::addButtonCB("TLJ_Operate_Gen", this, button_selector(TLJOperate::TLJ_Operate_Gen));
	WidgetManager::addButtonCB("TLJ_Operate_GenDaoDi", this, button_selector(TLJOperate::TLJ_Operate_GenDaoDi));
	WidgetManager::addButtonCB("TLJ_Operate_Cancle", this, button_selector(TLJOperate::TLJ_Operate_Cancle));
	WidgetManager::addButtonCB("TLJ_Operate_JiaZhu", this, button_selector(TLJOperate::TLJ_Operate_JiaZhu));
	WidgetManager::addButtonCB("TLJ_Operate_QiPai", this, button_selector(TLJOperate::TLJ_Operate_QiPai));

	WidgetManager::addButtonCB("TLJ_Operate_AddScore_1", this, button_selector(TLJOperate::TLJ_Operate_Betting));
	WidgetManager::addButtonCB("TLJ_Operate_AddScore_2", this, button_selector(TLJOperate::TLJ_Operate_Betting));
	WidgetManager::addButtonCB("TLJ_Operate_AddScore_3", this, button_selector(TLJOperate::TLJ_Operate_Betting));
	WidgetManager::addButtonCB("TLJ_Operate_AddScore_4", this, button_selector(TLJOperate::TLJ_Operate_Betting));
	WidgetManager::addButtonCB("TLJ_Operate_AddScore_5", this, button_selector(TLJOperate::TLJ_Operate_Betting));

	WidgetManager::addButtonCB("TLJ_Operate_AddScore_Cancle", this, button_selector(TLJOperate::TLJ_Operate_AddScore_Cancle));
}

void TLJOperate::changeOperateMode(int iMode)
{
	m_iMode = iMode; 
	show();
	WidgetFun::setVisible(this,"TLJ_Operate_BiPai",iMode==NORMAL_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_Gen",iMode==NORMAL_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_GenDaoDi",iMode==NORMAL_MODE||iMode==WAIT_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_Cancle",iMode==AUTO_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_JiaZhu",iMode==NORMAL_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_QiPai",iMode==NORMAL_MODE||iMode==WAIT_MODE||iMode==AUTO_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_AddScore_1",iMode==ADD_SCORE_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_AddScore_2",iMode==ADD_SCORE_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_AddScore_3",iMode==ADD_SCORE_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_AddScore_4",iMode==ADD_SCORE_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_AddScore_5",iMode==ADD_SCORE_MODE);
	WidgetFun::setVisible(this,"TLJ_Operate_AddScore_Cancle",iMode==ADD_SCORE_MODE);
}

int TLJOperate::getOperateMode()
{
	return m_iMode;
}

void TLJOperate::TLJ_Operate_BiPai(cocos2d::Ref* ref, WidgetUserInfo* pInfo)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	//操作状态校验
	if(m_iMode==AUTO_MODE){
		return;
	}

	if(TLJGameScence::Instance().checkUserScore(0)){
		changeOperateMode(WAIT_MODE);
		TLJGameScence::Instance().Button_BiPai(ref,pInfo);
	}
}

void TLJOperate::TLJ_Operate_Gen(cocos2d::Ref* ref, WidgetUserInfo* pInfo)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	/*if(m_iMode==AUTO_MODE){
		return;
	}
	*/
	if(TLJGameScence::Instance().checkUserScore(0)){
		if(m_iMode == NORMAL_MODE)
		{
			changeOperateMode(WAIT_MODE);
		}
		TLJGameScence::Instance().PlayerGenPai();
	}
	else if(m_iMode == AUTO_MODE)
	{
		changeOperateMode(NORMAL_MODE);
	}
	
}
void TLJOperate::TLJ_Operate_GenDaoDi(cocos2d::Ref* ref, WidgetUserInfo* pInfo)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	if(m_iMode==AUTO_MODE){
		return;
	}
	TLJGameScence::Instance().getLocalPlayer()->setAutoState(true);
	if(m_iMode==NORMAL_MODE){
		TLJGameScence::Instance().PlayerGenPai();
	}
	changeOperateMode(AUTO_MODE);
}
void TLJOperate::TLJ_Operate_Cancle(cocos2d::Ref*, WidgetUserInfo*)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	TLJGameScence::Instance().getLocalPlayer()->setAutoState(false);
	if(m_iMode == AUTO_MODE){
		changeOperateMode(WAIT_MODE);
	}
}

void TLJOperate::TLJ_Operate_JiaZhu(cocos2d::Ref*, WidgetUserInfo*)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	if(m_iMode==AUTO_MODE){
		return;
	}
	refreshBettingBtn();
	changeBettingBtnImagic();
	changeOperateMode(ADD_SCORE_MODE);
}

void TLJOperate::TLJ_Operate_QiPai(cocos2d::Ref* ref, WidgetUserInfo* pInfo)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	changeOperateMode(INVALID_MODE);
	TLJGameScence::Instance().Button_QiPai(ref,pInfo);
}

void TLJOperate::TLJ_Operate_Betting(cocos2d::Ref* ref, WidgetUserInfo* pInfo)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	if(m_iMode==AUTO_MODE){
		return;
	}
	byte cbLevel = utility::parseChar(pInfo->kUserInfo.at("Index"));

	if(TLJGameScence::Instance().checkUserScore(cbLevel)){
		changeOperateMode(WAIT_MODE);
		TLJGameScence::Instance().playerBetting(cbLevel);
	}
	else
	{
		changeOperateMode(NORMAL_MODE);
	}

}
//取消加注
void TLJOperate::TLJ_Operate_AddScore_Cancle(cocos2d::Ref* ref, WidgetUserInfo* pInfo)
{
	//游戏状态校验
	if(TLJGameScence::Instance().getGameState() != TLJ_STATUS_PLAYING)
	{
		return;
	}
	if(m_iMode!=ADD_SCORE_MODE){
		return;
	}
	changeOperateMode(NORMAL_MODE);
}
//刷新投注按钮 可点击状态
void TLJOperate::refreshBettingBtn()
{
	byte cbLevel = TLJGameScence::Instance().getScoreLevel();
	Node* pBtnNode = NULL;
	for (word index = 1; index < 6; index ++ )
	{
		pBtnNode = WidgetFun::getChildWidget(this,utility::toString("TLJ_Operate_AddScore_",index));
		if(cbLevel>index){
			WidgetFun::setEnable(pBtnNode,false);
		}else{
			WidgetFun::setEnable(pBtnNode,true);
		}
	}
}
//刷新投注按钮  倍数显示
void TLJOperate::changeBettingBtnImagic()
{
	word wTimes = TLJGameScence::Instance().getCurrentTimes();
	auto pLocalPlayer = TLJGameScence::Instance().getLocalPlayer();
	if(pLocalPlayer){
		bool bAnPai = (pLocalPlayer->getState() == PLAYER_STATE_AN);
		wTimes = bAnPai?1:wTimes;
	}
	dword dwBaseScore = TLJGameScence::Instance().getBaseScore();
	Node* pBtnNode = NULL;
	std::string normalImg = "";
	std::string disableImg = "";
	for (word index = 1; index < 6; index ++ )
	{
		pBtnNode = WidgetFun::getChildWidget(this,utility::toString("TLJ_Operate_AddScore_",index));
		WidgetManager::Instance().changeButtonProperty(pBtnNode,"ShowTxt",utility::toString(dwBaseScore*wTimes*index));
	}
}
void TLJOperate::show()
{
	setVisible(true);
}

void TLJOperate::hide()
{
	setVisible(false);
}