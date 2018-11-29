#ifndef _CONGCONG_H_
#define _CONGCONG_H_

#include "../PaymentSystem/PaymentWorker.h"

using namespace payment;
using namespace http::server;


namespace business {
class CongCong : public PaymentWorker
{
public:
    explicit CongCong();
    ~CongCong();

public:

    bool onMessage(pConect con, TCP_Command& tcp_cmd, void * pBuffer, WORD wDataSize) override;
    //�����������
    bool onHttpRequest(std::string req_path, const request& req, reply& rep) override;
    //ʧ�ܵ�Ĭ�ϴ���
    bool onHandFailed(const request& req, reply& rep) override;
    //�ɹ���Ĭ�ϴ���
    bool onHandSuccess(const request& req, reply& rep) override;
    //������֤
    bool onVerifyRequest(std::string req_path, const request& req) override;

};
}


#endif