#ifndef _TCP_EVENT_SENDER_THREAD_H_
#define _TCP_EVENT_SENDER_THREAD_H_

#include "stdafx.h"


#include "packet.h"
#include "security.hpp"
#include "tcp_event_sender.hpp"
#include "Tools/data_deque.hpp"

using namespace tool;

namespace tcp_con {

using namespace boost::interprocess;

static void sender_push(Data_queue<TCP_Buffer>* que, size_t size) {
	if (que)
		que->end().finalSize = static_cast<WORD>(size);
}

class TcpEventSendThread {

public:

    TcpEventSendThread()
    {}

    ~TcpEventSendThread()
    {
        stop();
    }

    void start(Security* _sert, Data_queue<TCP_Buffer>* _dataQue)
    {

        assert(_sert);
        assert(_dataQue);

        this->_dataQue = _dataQue;
        this->_security = _sert;

    }

    void stop()
    {

        _dataQue = nullptr;
        _security = nullptr;

    }

    void sendData(TCP_Command& comander, void* pdata, size_t sizet)
    {

        pushData(comander, pdata, sizet);

    }

protected:

    void pushData(TCP_Command& comander, void* pdata, size_t sizet)
    {
        scoped_lock<interprocess_mutex> lock(_wr_mtx);

        assert(!_dataQue->isFull());
        if (_dataQue->isFull())
            return;

        //拷贝头
        TCP_Buffer& tcp_buffer =  _dataQue->end();
		TCP_Head* pHead = reinterpret_cast<TCP_Head*>(tcp_buffer.cbBuffer);
		pHead->CommandInfo.wMainCmdID = comander.wMainCmdID;
		pHead->CommandInfo.wSubCmdID = comander.wSubCmdID;

        //拷贝数据
        if (sizet > 0) {
            assert(pdata);
            memcpy(tcp_buffer.cbBuffer + sizeof(TCP_Head), pdata , sizet);
        }

        //加密数据
        WORD wSendSize = _security->EncryptBuffer(tcp_buffer.cbBuffer, sizeof(TCP_Head) + static_cast<WORD>(sizet), SOCKET_TCP_BUFFER);

        try
        {
            _dataQue->push_back(wSendSize, sender_push);
        } catch (std::exception e) {
            printf("Send data que error: %s .\n",  e.what());
        }

    }

protected:

    Security*  _security = nullptr;
    Data_queue<TCP_Buffer>* _dataQue = nullptr;

    interprocess_mutex      _wr_mtx; //写入锁

};
}


#endif