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
    //处理请求入口
    bool onHttpRequest(std::string req_path, const request& req, reply& rep) override;
    //失败的默认处理
    bool onHandFailed(const request& req, reply& rep) override;
    //成功的默认处理
    bool onHandSuccess(const request& req, reply& rep) override;
    //消息处理
    bool onMessage(pConect con, TCP_Command& Command, void * pBuffer, WORD wDataSize) override;
    //处理验证
    bool onVerifyRequest(std::string req_path, const request& req) override;
};
}


#endif