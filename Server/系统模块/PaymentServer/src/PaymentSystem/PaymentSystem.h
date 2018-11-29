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
    //��ʼ��
    bool startup();
    //�رշ���
    void shutdown();
    //���ɶ���
    void genOrderSerial(int serverAccountID,
        const char *account,
        const char *name,
        const char *productID);
    //��ȡ��ǰ�๤
    PaymentWorker* getCurPaymentWorker();

public:

    //�����������
    bool onHttpRequest(std::string req_path, const http::server::request& req, http::server::reply& rep);
    //��Ϣ����
    bool onMessage(unsigned long nSocketID, TCP_Command& Command, void * pBuffer, WORD wDataSize) override;

protected:

    //ʧ�ܵ�Ĭ�ϴ���
    bool onHandFailedDefualt(const http::server::request& req, http::server::reply& rep);

private:

    WKMP _workers;
    PaymentWorker* _weixin;

    PlatformID _currentPlatformID;
    bool bUseWeiXin;

};

}

#endif