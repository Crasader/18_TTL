#ifndef _PAYMENT_SYSTEM_H_
#define _PAYMENT_SYSTEM_H_

#include <map>
#include "ConstTypes.h"

#include "tinyxml2/tinyxml.h"
#include "../Tcp/tcp_event_hander.hpp"
#include "../Tcp/tcp_event_sender.hpp"
#include "Tools/data_deque.hpp"

using namespace tool;

namespace http {
	namespace server {
		struct reply; struct request;
	}
}

namespace tcp_con {
	class TcpEventHander;
	class TcpEventSender;
}

using namespace tcp_con;

namespace payment{

class PaymentWorker;

class PaymentSystem : public TcpEventHander, public  TcpEventSender
{

private:
    PaymentSystem() {};
    static PaymentSystem* _instance;

public:

    static PaymentSystem* Instance();
    virtual ~PaymentSystem();

public:
    //初始化
    bool startup();
    //关闭服务
    void shutdown();
    //生成订单
    void genOrderSerial(int serverAccountID,
        const char *account,
        const char *name,
        const char *productID);
    //获取当前苦工
    PaymentWorker* getCurPaymentWorker();

public:

    //处理请求入口
    bool onHttpRequest(std::string req_path, const http::server::request& req, http::server::reply& rep);
    //消息处理
    bool onMessage(unsigned long nSocketID, TCP_Command& Command, void * pBuffer, WORD wDataSize) override;

protected:

    //失败的默认处理
    bool onHandFailedDefualt(const http::server::request& req, http::server::reply& rep);

private:

    WKMP _workers;
    PaymentWorker* _weixin;

    PlatformID _currentPlatformID;
    bool bUseWeiXin;

};

}

#endif