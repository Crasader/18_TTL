#ifndef _PAYMENT_WORKER_H_
#define _PAYMENT_WORKER_H_

#include "ConstTypes.h"
#include "../Tcp/packet.h"

#include "../boost_http/reply.hpp"
#include "../boost_http/request.hpp"

#include "Products.hpp"

using namespace http::server;
namespace osql { class SQLServerConector; }
namespace tcp_con { class tcp_connection; }

namespace payment {

class PaymentWorker
{

public:

    virtual ~PaymentWorker();

protected:

    PaymentWorker();

    PaymentWorker(const PaymentWorker&) = delete;
    PaymentWorker& operator = (const PaymentWorker&) = delete;

public:

    //处理请求入口
    virtual bool onHttpRequest(std::string req_path, const request& req, reply& rep) = 0;
    //失败的默认处理
    virtual bool onHandFailed(const request& req, reply& rep) = 0;
    //成功的默认处理
    virtual bool onHandSuccess(const request& req, reply& rep) = 0;
    //处理验证
    virtual bool onVerifyRequest(std::string req_path, const request& req) = 0;
    //处理http请求
    virtual bool onMessage(pConect con, TCP_Command& tcp_cmd, void * pBuffer, WORD wDataSize) = 0;
    //获取商品信息
    Products& getProducts() {  return _products;  }

protected:

    PlatformID _id = PD_NULL;

    Products _products;

    osql::SQLServerConector* SqlSevCon = nullptr;

};

}

#endif