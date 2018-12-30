#pragma once

#include "GAME.h"

#include "GPHomeSettingPanel.h"
#include "GPHomeSharePanel.h"
//#include "GPHomeUserInfoPanel.h"
#include "GPHomeEnterRoomPanel.h"
#include "GPHomeEditNumPanel.h"
//#include "GPHomePromotionPanel.h"
//#include "GPHomeMorePanel.h"
//#include "GPHomeServicePanel.h"
//#include "GPHomeMallPanel.h"
#include "GPHomeRecordPanel.h"
//#include "GPHomeRankPanel.h"

class GPHomeScene
	: public cocos2d::Node
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
	//�����������
	void onEnterScene();
	//�����
	void initPaoMaDeng();
	//ˢ����ҵȼ�
	void flushPlayerLevel();
	//չʾ�����
	void playPaoMaDeng(cocos2d::Node* pRootNode,std::string kTxtNode,std::string kLayoutNode,std::string kTxt,cocos2d::FiniteTimeAction* pNetAction);
	//�����������
	void hideAllPanels();
	//ɾ�����е����
	void removeAllPanels();
	//��ʾ�ܽ������
	void showGameCalculate(CMD_GF_Private_End_Info* pNetInfo);

#pragma region ��Ϸ��崦��

public:
	//������Ϸ���
	void openSubGame(int curPage);
	//ţ�����¼ӵ�////////////////////////////////////////////////////////////////////////
	//չʾ�����б�
	void showGameRoomList(void* data, size_t dataSize);
	//���ť
	void setButtonsEnable(bool flag);

protected:
	//��ʼ������Ϸ�б�ؼ�
	void initGamePanelList();
	//����Ϸ���
	void initGamePanels();
	void hideGamePanels();
	//ɾ������Ϸ���
	void removeGamepanels();
	//ţ�����¼ӵ�////////////////////////////////////////////////////////////////////////

#pragma endregion

#pragma region ������崦��
		//��ʼ���������
	bool initPublicPanel();
	void initPopupPanels();
	void addPanel(cocos2d::Node*);

#pragma endregion

#pragma region �û���Ϣ
	void onLogonSucess();
	void flushUserInfo();
#pragma endregion

#pragma region ��ť
	//��ʼ����ť
	void initButton();
	void onSelectedItemEvent(cocos2d::Ref *pSender, cocos2d::ui::ListView::EventType type);

	void Button_Share(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Header(cocos2d::Ref* pRef, WidgetUserInfo* pInfo);
	void Button_Mall(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Setting(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Rank( cocos2d::Ref*, WidgetUserInfo* );
	void Button_Record( cocos2d::Ref*, WidgetUserInfo* );
	void Button_Rule(cocos2d::Ref*, WidgetUserInfo*);

	void Button_NN(cocos2d::Ref*, WidgetUserInfo*);
	void Button_SRLF(cocos2d::Ref*, WidgetUserInfo*);
	void Button_MenJi(cocos2d::Ref*, WidgetUserInfo*);
	void Button_WZQ(cocos2d::Ref*, WidgetUserInfo*);
	void Button_SG(cocos2d::Ref*, WidgetUserInfo* );
	void Button_XZDD(cocos2d::Ref*, WidgetUserInfo*);

	//�¼ӵİ�ť////////////////////////////////////////////////////////////////////////
	void Button_Invitation(cocos2d::Ref*, WidgetUserInfo*);
	void Button_CreateGame(cocos2d::Ref*, WidgetUserInfo*);
	void Button_Join(cocos2d::Ref*, WidgetUserInfo*);
	void Button_RoomList_Join(cocos2d::Ref*, WidgetUserInfo*);

#pragma endregion

private:

	cocos2d::Node* _img_head;
	ssize_t m_selectedItemIndex;
	int _nCurrentGameKindID;

	cocos2d::Node* pNodeList;
};
