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
    //�����������
    virtual bool onHttpRequest(std::string req_path, const request& req, reply& rep) = 0;
    //ʧ�ܵ�Ĭ�ϴ���
    virtual bool onHandFailed(const request& req, reply& rep) = 0;
    //�ɹ���Ĭ�ϴ���
    virtual bool onHandSuccess(const request& req, reply& rep) = 0;
    //������֤
    virtual bool onVerifyRequest(std::string req_path, const request& req) = 0;
protected:
    PlatformID _id = PD_NULL;
};

}
#endif