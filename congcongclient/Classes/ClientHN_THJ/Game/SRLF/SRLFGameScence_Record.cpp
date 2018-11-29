#include "SRLFGameScence.h"
#include "Game/GameLib.h"
#include "SRLFPlayer.h"
#include "Game/Game/MissionWeiXin.h"

void SRLFGameScence::initRecord()
{
	WidgetScenceXMLparse kScence("Script/HNGameRecordChild.xml",this);

	WidgetManager::addButtonCB("Button_GameRecordPlay",this,button_selector(SRLFGameScence::Button_GameRecordPlay));
	WidgetManager::addButtonCB("Button_GameRecordPase",this,button_selector(SRLFGameScence::Button_GameRecordPase));
	WidgetManager::addButtonCB("Button_GameRecordLeft",this,button_selector(SRLFGameScence::Button_GameRecordLeft));
	WidgetManager::addButtonCB("Button_GameRecordRight",this,button_selector(SRLFGameScence::Button_GameRecordRight));
	WidgetManager::addButtonCB("Button_GameRecordExit",this,button_selector(SRLFGameScence::Button_GameRecordExit));
}
void SRLFGameScence::defaultRecordState()
{
	if (WidgetFun::getChildWidget(this,"GameRecordControlPlane"))
	{
		WidgetFun::setVisible(this,"GameRecordControlPlane",false);
	}
}
void SRLFGameScence::Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*)
{
	if (m_iActRecordIdex >= (int)m_pGameRecord->kAction.size())
	{
		WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
		WidgetFun::setVisible(this,"Button_GameRecordPase",false);
		return;
	}
	WidgetFun::setVisible(this,"Button_GameRecordPlay",false);
	WidgetFun::setVisible(this,"Button_GameRecordPase",true);
	TimeManager::Instance().removeByID(m_RecordTimeID);
	m_RecordTimeID = TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SRLFGameScence::NextRecordAction,this),m_RecordTime)->iIdex;
}
void SRLFGameScence::Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
	WidgetFun::setVisible(this,"Button_GameRecordPase",false);
	TimeManager::Instance().removeByID(m_RecordTimeID);
}
void SRLFGameScence::Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime += 0.5f;
}
void SRLFGameScence::Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*)
{
	m_RecordTime -= 0.5f;
	if (m_RecordTime < 0.2f)
	{
		m_RecordTime = 0.5f;
	}
}
void SRLFGameScence::Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*)
{
	TimeManager::Instance().removeByID(m_RecordTimeID);
	exitRoom();
}
bool SRLFGameScence::StartRecord(datastream kDataStream)
{
	if (m_pGameRecord == NULL)
	{
		m_pGameRecord = new SRLF::SRLFGameRecord;
		initRecord();
	}
	defaultState();
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"GameRecordControlPlane",true);
	WidgetFun::setText(this,"GameRecordPercentTxt","0%");
	*m_pGameRecord = SRLF::SRLFGameRecord();
	WidgetFun::setVisible(this,"Button_GameRecordPlay",true);
	WidgetFun::setVisible(this,"Button_GameRecordPase",false);
	m_iActRecordIdex = 0;
	m_RecordTime = 2.0f;
	m_pGameRecord->StreamValue(kDataStream,false);
	if (m_pGameRecord->kPlayers.size() != MAX_PLAYER)
	{
		exitRoom();
		return false;
	}	
	m_wRecordSelfChairID = 0;

	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		SRLF::SRLFGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
		if (kRecordPlayer.dwUserID == UserInfo::Instance().getUserID())
		{
			m_wRecordSelfChairID = i;
		}
	}
	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		SRLF::SRLFGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];

		int iChairID = (m_wRecordSelfChairID-i+MAX_PLAYER)%MAX_PLAYER;
		SRLFPlayer* pPlayer =getPlayerByChairID(iChairID);//m_pPlayer[iChairID];
		if (!pPlayer)
		{
			return false;
		}
		m_kReqPlayerInfo.query(kRecordPlayer.dwUserID);
		pPlayer->defaultState();
		pPlayer->setGameRecord(true);
		pPlayer->PlayerEnter();
		pPlayer->startGameNoTime();
		BYTE cbCardData[MAX_COUNT];
		for (int i = 0;i<MAX_COUNT;i++)
		{
			cbCardData[i] = kRecordPlayer.cbCardData[i];
		}
		pPlayer->setHandCard(cbCardData,13);
		pPlayer->showHandCard();
		pPlayer->showCard();
		WidgetFun::setText(pPlayer->getPlayerNode(),"Name",utility::a_u8(kRecordPlayer.kNickName));
		WidgetFun::setVisible(pPlayer->getPlayerNode(),"GoldImagic",false);
	}
	Button_GameRecordPlay(NULL,NULL);
	return true;
}
void SRLFGameScence::NextRecordAction()
{
	if (m_iActRecordIdex >= (int)m_pGameRecord->kAction.size() || !isVisible())
	{
		Button_GameRecordPase(NULL,NULL);
		WidgetFun::setText(this,"GameRecordPercentTxt","100%");
		return;
	}
	SRLF::SRLFGameRecordOperateResult& kAction = m_pGameRecord->kAction[m_iActRecordIdex];
	int iChairID = (m_wRecordSelfChairID-kAction.wOperateUser+MAX_PLAYER)%MAX_PLAYER;
	if (kAction.cbActionType == SRLF::SRLFGameRecordOperateResult::TYPE_OperateResult)
	{
		SRLFPlayer* pPlayer = m_pPlayer[iChairID];
		SRLF::CMD_S_OperateResult kTempCMD;
		kTempCMD.cbOperateCard = kAction.cbOperateCard;
		kTempCMD.cbOperateCode = kAction.cbOperateCode;
		kTempCMD.wOperateUser = kAction.wOperateUser;
		kTempCMD.wProvideUser = kAction.wProvideUser;
		pPlayer->setOperateResoult(&kTempCMD);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == SRLF::SRLFGameRecordOperateResult::TYPE_SendCard)
	{
		SRLFPlayer* pPlayer = m_pPlayer[iChairID];
		if (kAction.cbOperateCard != 0)
		{
			pPlayer->addNewInCard(kAction.cbOperateCard);
		}
		pPlayer->showCard();
	}
	if (kAction.cbActionType == SRLF::SRLFGameRecordOperateResult::TYPE_OutCard)
	{
		SRLFPlayer* pPlayer = getPlayerByChairID(iChairID);
		for (int i = 0;i<MAX_PLAYER;i++)
		{
			getPlayerByChairID(i)->setActOutCard(-1);
		}
		pPlayer->sendOutCard(kAction.cbOperateCard);
		pPlayer->showCard();
	}
	if (kAction.cbActionType == SRLF::SRLFGameRecordOperateResult::TYPE_ChiHu)
	{
		
		SRLFPlayer* pPlayer = getPlayerByChairID(iChairID);
		for (int i = 0;i<MAX_PLAYER;i++)
		{

			getPlayerByChairID(i)->setActOutCard(-1);
		}

		pPlayer->setChiHuCard(kAction.cbOperateCard);
		pPlayer->showEffect("Hu");
		if (kAction.wOperateUser != kAction.wProvideUser)
		{
			SRLFPlayer* pDestPlayer = getPlayerByChairID(iChairID); 
			pPlayer->showHandCard();
			pPlayer->showStatusImagic("Hu");

			pDestPlayer->runAniDianPao();
		}
		else
		{
			pPlayer->showStatusImagic("ZiMo");
			pPlayer->runAniZiMo();
		}
		pPlayer->showCard();
	}

	m_iActRecordIdex++;
	Button_GameRecordPlay(NULL,NULL);

	WidgetFun::setText(this,"GameRecordPercentTxt",utility::toString(m_iActRecordIdex*100/m_pGameRecord->kAction.size(),"%"));
}
void SRLFGameScence::onGPAccountInfoHttpIP(dword dwUserID, std::string strIP,std::string strHttp)
{
	GameBase::onGPAccountInfoHttpIP(dwUserID,strIP,strHttp);

	if (!m_pGameRecord)
	{
		return;
	}
	if (!WidgetFun::isWidgetVisble(this,"GameRecordControlPlane"))
	{
		return;
	}
	int iChairID = -1;
	for (int i = 0;i<(int)m_pGameRecord->kPlayers.size();i++)
	{
		SRLF::SRLFGameRecordPlayer& kRecordPlayer = m_pGameRecord->kPlayers[i];
		if (kRecordPlayer.dwUserID == dwUserID)
		{
			iChairID = i;
		}
	}
	if (iChairID == -1)
	{
		return;
	}
	iChairID = (m_wRecordSelfChairID-iChairID+MAX_PLAYER)%MAX_PLAYER;
	SRLFPlayer* pDestPlayer = m_pPlayer[iChairID];

	ImagicDownManager::Instance().addDown(
		WidgetFun::getChildWidget(
		pDestPlayer->getPlayerNode()
		,"HeadImagic"),
		strHttp,dwUserID);
}