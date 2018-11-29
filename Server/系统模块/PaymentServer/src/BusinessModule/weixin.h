#ifndef _WEIXIN_H_
#define _WEIXIN_H_

#include "../PaymentSystem/PaymentWorker.h"
#include "Tools/data_deque.hpp"

using namespace payment;
using namespace http::server;

namespace business {

enum ORDER_STATE
{

    OS_PAY_NULL = 0,//未支付
    OS_PAY_FAILED,//支付失败
    OS_PAY_SUCESS,//支付成功
    //OS_PAY_,//

};

enum WX_PAY_ER_CODE
{

    WPRC_OK = 0,
    WPRC_FAILED,
    WPRC_KEY_MISS_MATCH,
    WPRC_SIGN_MISS_MATCH,
    WPRC_USER_DATA_ERROR,
	WPRC_PRODUCT_DATA_ERROR,

};

struct UrlBuffer
{
    size_t size;
    char data[UrlBufferLength];
};

struct RequestData
{
    int idx;
    tool::Data_queue<UrlBuffer>* que;
};

//////////////////////////////////////////////////////////////////////////

struct WeiXinOrderRequest
{
    std::string appid;
    std::string attach;
    std::string body;
    std::string mch_id;
    std::string nonce_str;
    std::string sign;
    std::string notify_url;
    std::string out_trade_no;
    std::string spbill_create_ip;
    std::string total_fee;
    std::string trade_type;
};

struct WeiXinOrderResult 
{
    std::string return_code;
    std::string appid;
    std::string mch_id;
    std::string ret_nonce_str;
    std::string sign_str;
    std::string result_code;
    std::string return_msg;
    std::string err_code;
    std::string trade_type;
    std::string prepay_id;
};

struct WeiXinPayResult
{

    std::string appid;
    std::string attach;//optional
    std::string bank_type;
    std::string cash_fee;
    std::string fee_type;//optional
    std::string is_subscribe;//optional
    std::string mch_id;
    std::string nonce_str;
    std::string openid;
    std::string out_trade_no;
    std::string result_code;
    std::string return_code;
    std::string sign;
    std::string time_end;
    std::string total_fee;
    std::string trade_type;
    std::string transaction_id;

    std::string err_code;//optional
    std::string return_msg;//optional

};

//////////////////////////////////////////////////////////////////////////

class WeiXin : public PaymentWorker
{
public:
    explicit WeiXin();
    ~WeiXin();

public:

    bool onMessage(pConect con, TCP_Command& tcp_cmd, void * pBuffer, WORD wDataSize) override;
    //处理请求入口
    bool onHttpRequest(std::string req_path, const request& req, reply& rep) override;
    //失败的默认处理
    bool onHandFailed(const request& req, reply& rep) override;
    //成功的默认处理
    bool onHandSuccess(const request& req, reply& rep) override;
    //处理验证
    bool onVerifyRequest(std::string req_path, const request& req) override;

public:
    
    bool onRequestWeiXinOrder(const Product* product, 
        unsigned long nServerSocketID,
        DWORD tableID,
        DWORD dwUserSocketID,
        DWORD userID,
        std::string IP,
        std::string attach);

    tool::Data_queue<UrlBuffer> _genOrderQue;

};

}


#endif