#ifndef PRIVATE_TABLE_INFO_FILE
#define PRIVATE_TABLE_INFO_FILE

//引入文件
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"

///////////////////////////////////////////////////////////////////////////////////////////

enum RoomType
{
	Type_Private,
	Type_Public,
};

//定时赛
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

    BYTE bPlayCoutIdex; //玩家局数
    BYTE bGameTypeIdex; //游戏类型
    DWORD bGameRuleIdex; //游戏规则

    //BYTE cbLastOfflineReadyState[MAX_CHAIR]; //上次断线状态
    SCORE lPlayerWinLose[MAX_CHAIR];
    BYTE lPlayerAction[MAX_CHAIR][MAX_PRIVATE_ACTION];

    float fDismissPastTime;
    std::vector<DWORD> kDismissChairID;
    std::vector<DWORD> kNotAgreeChairID;

    float fOfflinePastTime;

#if defined(ALLOWED_KICK_UNREADY_USER)
    DWORD m_TimerEventIDs[MAX_CHAIR]; //定时器ID
#endif

    tagPrivateRandTotalRecord kTotalRecord;

#if defined(PLATFORM_CONGCONG)
    bool bPassionationMode;    //激情模式开关
    bool bBloodFightMode;    //血战到底开关
#endif

    bool bAllowedStrangerJoin;    //是否允许陌生人加入
    DWORD dwBaseScore; //底注
    DWORD dwEnterMatchNum; //入场限制
    DWORD dwOutMatchNum; //离场限制
};

#endif