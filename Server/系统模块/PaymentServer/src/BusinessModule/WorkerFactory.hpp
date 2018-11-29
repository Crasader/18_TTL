#ifndef _WORKER_FACTORY_H_
#define _WORKER_FACTORY_H_


#include "congcong.h"
#include "gansu.h"
#include "weixin.h"


namespace business {

class WorkerFactory
{

public:

    static WorkerFactory* Instance()
    {
        if (_instance == nullptr)
            _instance = new WorkerFactory();
        return _instance;
    }

private:

    WorkerFactory() {};

public:

    inline PaymentWorker* Create(PlatformID id)
    {

        PaymentWorker* worker = nullptr;

        switch (id) {

        case PlatformID::PD_CongCong:
            worker =  new CongCong();
            break;
        case PlatformID::PD_GanSu:
            worker = new GanSu();
            break;
        case PlatformID::PD_WeiXin:
            worker = new WeiXin();
            break;
        }

        return worker;

    }

private:

    static WorkerFactory* _instance;

};


}

#endif