#ifndef _GANSU_H_
#define _GANSU_H_

#include "../PaymentSystem/PaymentWorker.h"

using namespace payment;
using namespace http::server;

namespace business {
class GanSu : public PaymentWorker
{
public:
    explicit GanSu();
    ~GanSu();
public:
    //�����������
    bool onHttpRequest(std::string req_path, const request& req, reply& rep) override;
    //ʧ�ܵ�Ĭ�ϴ���
    bool onHandFailed(const request& req, reply& rep) override;
    //�ɹ���Ĭ�ϴ���
    bool onHandSuccess(const request& req, reply& rep) override;
    //��Ϣ����
    bool onMessage(pConect con, TCP_Command& Command, void * pBuffer, WORD wDataSize) override;
    //������֤
    bool onVerifyRequest(std::string req_path, const request& req) override;
};
}


#endif