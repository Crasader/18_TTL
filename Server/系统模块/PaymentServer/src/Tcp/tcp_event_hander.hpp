#ifndef _TCP_EVENT_HANDER_H_
#define _TCP_EVENT_HANDER_H_

#include "stdafx.h"
#include "packet.h"

namespace tcp_con {

class TcpEventHander
{
public:

    //��ȡ��Ϣ
    virtual bool onMessage(unsigned long nSocketID, TCP_Command& Command, void * pBuffer, WORD wDataSize) = 0;

};
}

#endif