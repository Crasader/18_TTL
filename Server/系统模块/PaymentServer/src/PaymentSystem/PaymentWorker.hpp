#ifndef _PAYMENT_WORKER_H_
#define _PAYMENT_WORKER_H_

#include "ConstTypes.h"

#include "../boost_http/reply.hpp"
#include "../boost_http/request.hpp"

#include "../Tcp/tcp_event_hander.hpp"
#include "../Tcp/tcp_event_sender.hpp"

namespace payment {

using namespace http::server;
using namespace tcp_con;

class PaymentWorker: public TcpEventHander, public  TcpEventSender
{
protected:
    PaymentWorker() {};
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
protected:
    PlatformID _id = PD_NULL;
};

}
#endif