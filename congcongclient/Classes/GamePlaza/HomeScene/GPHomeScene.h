#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetScenceXmlParse.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetFun.h"
#include "Game/Game/NoticeMsg.h"
#include "SimpleAudioEngine.h"
#include "Platform/PFKernel/CGPLoginMission.h"
#include "GameLib/GameNet/ImagicDownManager.h"
#include "Game/Game/UserInfo.h"
#include "SettingPanel/GPHomeSettingPanel.h"
#include "SharePanel/GPHomeSharePanel.h"
#include "UserInfoPanel/GPHomeUserInfoPanel.h"
//#include "RaidersPanel/GPHomeRaidersPanel.h"
#include "EnterRoomPanel/GPHomeEnterRoomPanel.h"
#include "EditNumPanel/GPHomeEditNumPanel.h"
#include "Promotion/GPHomePromotionPanel.h"
#include "MorePanel/GPHomeMorePanel.h"
#include "ServicePanel/GPHomeServicePanel.h"
#include "MallPanel/GPHomeMallPanel.h"
#include "RecordPanel/GPHomeRecordPanel.h"
#include "RankPanel/GPHomeRankPanel.h"

#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeRoomListPanel_SRLF.h"
#include"GamePlaza/HomeScene/CreateRoomPanel/GPHomeCreateRoomPanel_SRLF.h"

#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeCreateRoomPanel_TLJ.h"
#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeRoomListPanel_TLJ.h"

#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeCreateRoomPanel_NN.h"
#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeRoomListPanel_NN.h"
#include "ClientHN_THJ/Game/TTLNN/Calculate/NNCalculate.h"

#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeRoomListPanel_SG.h"
#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeCreateRoomPanel_SG.h"

#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeRoomListPanel_WZQ.h"

#include "GamePlaza/HomeScene/BtnTurnLayer.h"

#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeRoomListPanel_XZDD.h"
#include "GamePlaza/HomeScene/CreateRoomPanel/GPHomeCreateRoomPanel_XZDD.h"

#include "GameLib/Platform/PFKernel/CGPopularizeMission.h"

class GPHomeScene
	: public cocos2d::Node
	, public CGPopularizeSink 
	, public FvSingleton<GPHomeScene>
{
public:
	GPHomeScene();
	~GPHomeScene();

public:
	virtual void onGPNoticeResult(tagGameMatch&	pGameMatchInfo, bool bSucess, const char* pStr);
	
	virtual void onGPPopularizeHttpRes(const CMD_GP_PopularizeHttpRes& pNetInfo);

public:
	void pageViewEvent(Ref* pSender,cocos2d::ui::PageViewEventType type);

	bool init();
	//触发进入大厅
	void onEnterScene();
	//跑马灯
	void initPaoMaDeng();
	//刷新玩家等级
	void flushPlayerLevel();
	//展示跑马灯
	void playPaoMaDeng(cocos2d::Node* pRootNode,std::string kTxtNode,std::string kLayoutNode,std::string kTxt,cocos2d::FiniteTimeAction* pNetAction);
	//隐藏所有面板
	void hideAllPanels();
	//删除所有的面板
	void removeAllPanels();
	//显示总结算面板
	void showGameCalculate(CMD_GF_Private_End_Info* pNetInfo);
#pragma region 游戏面板处理
public:
	//打开子游戏面板
	void openSubGame(int curPage);
	//牛翻天新加的////////////////////////////////////////////////////////////////////////
	//展示房间列表
	void showGameRoomList(void* data, size_t dataSize);
protected:
	//初始化子游戏列表控件
	void initGamePanelList();
	//子游戏面板
	void initGamePanels();
	void hideGamePanels();
	//删除子游戏面板
	void removeGamepanels();
	//牛翻天新加的////////////////////////////////////////////////////////////////////////

#pragma endregion

#pragma region 弹出面板处理
		//初始化公用面板
	bool initPublicPanel();
	void initPopupPanels();
	void addPanel(cocos2d::Node*);
	//void showExitPanel();
#pragma endregion

#pragma region 用户信息
	void onLogonSucess();
	void flushUserInfo();
#pragma endregion

#pragma region 按钮
	//初始化按钮
	void initButton();
	void onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);

	void Button_Share(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Header(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);
	void Button_Mall(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Setting(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Rank( cocos2d::Ref*, WidgetUserInfo* );
	void Button_Record( cocos2d::Ref*, WidgetUserInfo* );

	void Button_NN(cocos2d::Ref*, WidgetUserInfo*);
	void Button_SRLF(cocos2d::Ref*, WidgetUserInfo*);
	void Button_MenJi(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WZQ(cocos2d::Ref*, WidgetUserInfo*);
	void Button_SG(cocos2d::Ref*, WidgetUserInfo* );
	void Button_XZDD(cocos2d::Ref*, WidgetUserInfo*);

	//新加的按钮////////////////////////////////////////////////////////////////////////
	void Button_Invitation(cocos2d::Ref*, WidgetUserInfo*);
	void Button_CreateGame(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Join(cocos2d::Ref*, WidgetUserInfo*);
	void Button_RoomList_Join(cocos2d::Ref*, WidgetUserInfo*);

#pragma endregion

private:
	cocos2d::Node* _img_head;
	ssize_t m_selectedItemIndex;
	CGPopularizeMission m_kPopularizeMission;

	int _nCurrentGameKindID;

	//std::map<dword, RoomShareInfo> _mpShareInfo;
};
