#include "StdAfx.h"
#include "PrivateTableInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���캯��
PrivateTableInfo::PrivateTableInfo()
	:pITableFrame(NULL)
{
	restValue();
}

PrivateTableInfo::~PrivateTableInfo(void)
{
	//�رն�ʱ��
}

void PrivateTableInfo::restValue(int nCharCount)
{
	bStart = false;
	bInEnd = false;
	fOfflinePastTime = 0;
	bPlayCoutIdex = 0;
	bGameTypeIdex = 0;
	dwGameRuleIdex = 0;
	bRoomType = Type_Private;
	dwPlayCout = 0;
	dwRoomNum = 0;
	//dwCreaterUserID = 0;
	fDismissPastTime = 0;
	dwFinishPlayCout = 0;
	dwStartPlayCout = 0;
	dwPlayCost = 0;
	fAgainPastTime = 0.0f;
	fEmptyRoomLiveTime = 0.f;
	kHttpChannel = "";
	bAllowedStrangerJoin = false;//�Ƿ�����İ���˼���
	dwBaseScore = 0;//��ע
	dwEnterMatchNum = 0;//�볡����
	dwOutMatchNum = 0;//�볡����
#if defined(PLATFORM_CONGCONG)
	bPassionationMode = false;	    //����ģʽ����
	bBloodFightMode = false;        	//Ѫս���׿���
#endif
	kDismissChairID.clear();
	kNotAgreeChairID.clear();
	ZeroMemory(lPlayerWinLose, sizeof(lPlayerWinLose));
	ZeroMemory(lPlayerAction, sizeof(lPlayerAction));
	//ZeroMemory(cbLastOfflineReadyState,sizeof(cbLastOfflineReadyState));
#if defined(ALLOWED_KICK_UNREADY_USER)
	ZeroMemory(m_TimerEventIDs, sizeof(m_TimerEventIDs));
#endif
	kTotalRecord = tagPrivateRandTotalRecord();

	if (pITableFrame)
	{
		pITableFrame->SetMasterUser(nullptr);
		pITableFrame->SetCreateUser(nullptr);

		if (nCharCount < 0 || nCharCount > pITableFrame->GetMaxChairCount()) {
			nCharCount = pITableFrame->GetMaxChairCount();
		}
		pITableFrame->SetChairCount(nCharCount);

		kTotalRecord.kScore.resize(nCharCount);
		kTotalRecord.kUserID.resize(nCharCount);
		kTotalRecord.kNickName.resize(nCharCount);
		kTotalRecord.dwKindID = pITableFrame->GetGameServiceAttrib()->wKindID;
		kTotalRecord.dwVersion = pITableFrame->GetGameServiceAttrib()->dwClientVersion;

		for (int i = 0; i < nCharCount; i++)
		{
			kTotalRecord.kScore[i] = 0;
#if defined(ALLOWED_KICK_UNREADY_USER)
			m_TimerEventIDs[i] = 0;
#endif
		}
	}
}
void PrivateTableInfo::newRandChild()
{
	tagPrivateRandRecordChild kRecordChild;
	kRecordChild.dwKindID = pITableFrame->GetGameServiceAttrib()->wKindID;
	kRecordChild.dwVersion = pITableFrame->GetGameServiceAttrib()->dwClientVersion;
	kRecordChild.kScore.resize(getChairCout());
	kTotalRecord.kNickName.resize(getChairCout());
	kTotalRecord.kUserID.resize(getChairCout());
	for (int i = 0;i<(int)getChairCout();i++)
	{
		IServerUserItem * pServerItem = pITableFrame->GetTableUserItem(i);
		if (pServerItem == NULL)
		{
			continue;
		}
		kTotalRecord.kNickName[i] = pServerItem->GetNickName();
		kTotalRecord.kUserID[i] = pServerItem->GetUserID();

		kRecordChild.kScore[i] = 0;
	}
	GetLocalTime(&kRecordChild.kPlayTime);
	kTotalRecord.kRecordChild.push_back(kRecordChild);
}
WORD PrivateTableInfo::getChairCout()
{
	return pITableFrame->GetChairCount();
}
bool PrivateTableInfo::IsAllOffline()
{
	if (pITableFrame->GetSitUserCount() == 0)
	{
		return false;
	}
	return pITableFrame->GetSitUserCount() == pITableFrame->GetOffLineUserCount();
}
void PrivateTableInfo::setRoomNum(DWORD RoomNum)
{
	kTotalRecord.iRoomNum = (int)RoomNum;
	dwRoomNum = RoomNum;
}
void PrivateTableInfo::writeSocre(tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& daUserDefine)
{
	if (kTotalRecord.kRecordChild.size() == 0)
	{
		ASSERT(false);
		return;
	}
	tagPrivateRandRecordChild& kRecord = kTotalRecord.kRecordChild.back();
	if (kRecord.kScore.size() < wScoreCount)
	{
		ASSERT(false);
		return;
	}
	for(WORD i = 0;i < wScoreCount;i++)
	{

		kRecord.kScore[i] += (int)ScoreInfoArray[i].lScore;
		kTotalRecord.kScore[i] += (int)ScoreInfoArray[i].lScore;
		lPlayerWinLose[i] += ScoreInfoArray[i].lScore;
	}

	GetLocalTime(&kRecord.kPlayTime);
	kRecord.kRecordGame = daUserDefine;
}