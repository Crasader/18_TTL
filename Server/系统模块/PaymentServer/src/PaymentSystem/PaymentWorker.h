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

    //�����������
    virtual bool onHttpRequest(std::string req_path, const request& req, reply& rep) = 0;
    //ʧ�ܵ�Ĭ�ϴ���
    virtual bool onHandFailed(const request& req, reply& rep) = 0;
    //�ɹ���Ĭ�ϴ���
    virtual bool onHandSuccess(const request& req, reply& rep) = 0;
    //������֤
    virtual bool onVerifyRequest(std::string req_path, const request& req) = 0;
    //����http����
    virtual bool onMessage(pConect con, TCP_Command& tcp_cmd, void * pBuffer, WORD wDataSize) = 0;
    //��ȡ��Ʒ��Ϣ
    Products& getProducts() {  return _products;  }

protected:

    PlatformID _id = PD_NULL;

    Products _products;

    osql::SQLServerConector* SqlSevCon = nullptr;

};

}

#endif