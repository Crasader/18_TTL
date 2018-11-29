#ifndef _COM_PAYMENT_SERVER_H_
#define _COM_PAYMENT_SERVER_H_


#pragma pack(push, 1)



//////////////////////////////////////////////////////////////////////////

#define  MDM_PM_REGIST                  1         //注册游戏

#define  SUB_PM_REGIST                  100       //注册游戏服务器
#define  SUB_PM_REGIST_RET           101      //注册游戏服务器结果

struct CMD_PM_Regist
{
    WORD wServerID;//游戏服务器ID
    WORD wServerKindID;//游戏类型ID
    char csPlatformID[32];//平台标识
};

enum RegistResult {

    RR_Mismatch = 101,//
    RR_ServerIDErr = 102,

};

struct CMD_PM_Regist_Result
{
    WORD wRegistResult;
};

//////////////////////////////////////////////////////////////////////////

#define MDM_PM_BESINESS                                  2           //业务消息

#define SUB_PM_PAY_GENORDER                         100        //请求订单
#define SUB_PM_PAY_GENORDER_RET                  101        //请求订单结果
#define SUB_PM_PAY_GEN_WEIXINORDER            102        //请求微信后台订单
#define SUB_PM_PAY_WEIXINORDER_RET             103        //请求微信后台订单结果
#define SUB_PM_PAY_SUCCESS                              104        //支付成功

struct SUB_PM_PAY_SUC
{
	DWORD dwUserID;//玩家ID
    DWORD dwUserSocketID;//玩家ID
    WORD wMoneyType;//金额的类型
    DWORD wMoney;//充值的金额
    DWORD wServerID;//游戏服务器ID
    DWORD wKindID;//游戏类型ID
    DWORD wTableID;//桌子ID
};

#define IP_LEN                     20
#define ATTACH_LEN          128
struct SUB_PM_GEN_WEIXIN_OD
{
    DWORD dwUserID;//玩家ID
    DWORD dwSocketID;//玩家ID
    DWORD dwTableID;//桌子号
    WORD wPoductID;//商品ID
    char cstrIP[IP_LEN];//IP
    char cstrAttach[ATTACH_LEN];//自定义数据
};

#define NONCE_LEN           64
#define PREPARE_LEN         64
#define ORDER_LEN            64
struct SUB_PM_GEN_WEIXIN_OD_RET
{
    DWORD dwUserID;//玩家ID
    DWORD dwSocketID;//玩家ID
    DWORD dwTableID;//桌子号
    WORD wPoductID;
    WORD nonce_str_len;
    WORD prepare_id_len;
    WORD order_len;
    char nonce_str[NONCE_LEN];
    char prepare_id[PREPARE_LEN];
    char order[ORDER_LEN];
};

//////////////////////////////////////////////////////////////////////////


#pragma pack(pop)

#endif
