#ifndef PRIVATE_TABLE_INFO_FILE
#define PRIVATE_TABLE_INFO_FILE

//�����ļ�
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"

///////////////////////////////////////////////////////////////////////////////////////////

enum RoomType
{
	Type_Private,
	Type_Public,
};

//��ʱ��
class PrivateTableInfo 
{
public:
    PrivateTableInfo();
    ~PrivateTableInfo();

    void restValue(int nCharCount = -1);
    void restAgainValue(int nCharCount = -1);
    void newRandChild();
    WORD getChairCout();
    bool IsAllOffline();
    void setRoomNum(DWORD RoomNum);
    void writeSocre(tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& daUserDefine);

    ITableFrame* pITableFrame;
    DWORD dwGameKindID;
    //DWORD dwCreaterUserID;
    DWORD dwRoomNum;
    DWORD dwPlayCout;
    DWORD dwPlayCost;
    bool bStart;
    bool bInEnd;
    float fAgainPastTime;
    std::string kHttpChannel;

    BYTE cbRoomType;

    DWORD dwStartPlayCout;
    DWORD dwFinishPlayCout;

    BYTE bPlayCoutIdex; //��Ҿ���
    BYTE bGameTypeIdex; //��Ϸ����
    DWORD bGameRuleIdex; //��Ϸ����

    //BYTE cbLastOfflineReadyState[MAX_CHAIR]; //�ϴζ���״̬
    SCORE lPlayerWinLose[MAX_CHAIR];
    BYTE lPlayerAction[MAX_CHAIR][MAX_PRIVATE_ACTION];

    float fDismissPastTime;
    std::vector<DWORD> kDismissChairID;
    std::vector<DWORD> kNotAgreeChairID;

    float fOfflinePastTime;

#if defined(ALLOWED_KICK_UNREADY_USER)
    DWORD m_TimerEventIDs[MAX_CHAIR]; //��ʱ��ID
#endif

    tagPrivateRandTotalRecord kTotalRecord;

#if defined(PLATFORM_CONGCONG)
    bool bPassionationMode;    //����ģʽ����
    bool bBloodFightMode;    //Ѫս���׿���
#endif

    bool bAllowedStrangerJoin;    //�Ƿ�����İ���˼���
    DWORD dwBaseScore; //��ע
    DWORD dwEnterMatchNum; //�볡����
    DWORD dwOutMatchNum; //�볡����
};

#endif