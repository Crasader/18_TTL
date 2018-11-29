#ifndef _CONST_TYPES_H_
#define _CONST_TYPES_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef char TCHAR, *PTCHAR;
typedef unsigned long DWORD;
typedef double DATE;

//////////////////////////////////////////////////////////////////////////

#define  pConect boost::shared_ptr<tcp_con::tcp_connection>
#define  ConectMap std::map<unsigned long, boost::shared_ptr<tcp_con::tcp_connection>>
#define  ConectPair std::pair<unsigned long, boost::shared_ptr<tcp_con::tcp_connection>>
#define  ConItrator ConectMap::iterator 

#define ProductPair std::pair<int, Product>
#define ProductsMap std::map<int, Product>

#define WKPR std::pair<PlatformID, payment::PaymentWorker*>
#define WKMP std::map <PlatformID, payment::PaymentWorker*>

//////////////////////////////////////////////////////////////////////////

//每一个socket消息队列最大数量
#define MAX_PACKAGE_DEQUE 16
//每个消息的最大长度
//#define MAX_PACKAGE_BUFFER_SIZE 65536
//心跳包间隔
#define HEART_TICK_TIME 5000
//请求curlURL的buffer长度
#define UrlBufferLength 1024*1024

enum PlatformID
{
    PD_NULL,
    PD_WeiXin,//微信支付
    PD_CongCong, //聪聪平台支付
    PD_GanSu, //嘎嘎平台支付



    PD_Count,
};

enum IAPPurchaseResultType
{
    IAPPRT_Unknown = 0,
    IAPPRT_Ok,                      // 支付成功
    IAPPRT_DatabaseFailed,          // 写入数据库失败
    IAPPRT_NotPurchased,            // 支付失败（多半是外挂乱发数据
    IAPPRT_AlreadyPurchased,        // 已经支付过了（在支付数据库中已经有TransactionID存在了
    IAPPRT_Processing,              // 处理中（多半是服务器那边还没交易完毕
    IAPPRT_NoOrderSerial,           // 没有订单
};

enum Produt_Type
{
	PT_Gold = 0,//充值金额
	PT_RoomCard,//房卡
};

#endif // 