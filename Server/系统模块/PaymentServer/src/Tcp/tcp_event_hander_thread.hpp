#ifndef _TCP_EVENT_HANDER_THREAD_H_
#define _TCP_EVENT_HANDER_THREAD_H_

#include "stdafx.h"

#include "packet.h"
#include "tcp_event_hander.hpp"
#include "Tools/data_deque.hpp"

using namespace tool;

enum EventExitCode 
{
	EEC_Running=-1,//正在运行
	EEC_OK=0,//正常退出
	EEC_Data_error,//数据队列异常退出
	EEC_SEC_error,//安全模块异常退出
	EEC_Package_Over_Length,//数据包长度超长
	EEC_Package_Head_error,//数据包头错误
	EEC_Package_ENCRYPT_error,//数据包加密类型错误
};

namespace tcp_con{

class TcpEventHandThread {

public:

    TcpEventHandThread() 
    {}

    ~TcpEventHandThread() 
    {

		process_running = true;
        stop();
        
    }

    void start(Security* _sert, Data_queue<TCP_Buffer>* _dataQue, TcpEventHander* _evtHander)
    {

        this->_dataQue = _dataQue;
        this->_security = _sert;
        this->_eventHander = _evtHander;
        process_running = true;
		process_end = false;
        _evt_thr = new boost::thread( boost::bind(&TcpEventHandThread::run, this) );

    }

    bool isRunning() 
    {
        return process_running;
    }

    void stop()
    {
		stopHander();

        //等待线程执行结束
        while (!process_end) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }

    }

    void setSocketID(unsigned long socketID)
    {
        _nSocketID = socketID;
    }

protected:

	void stopHander()
	{
		boost::unique_lock<boost::shared_mutex> ul(_wr_mtx);
		process_running = false;
	}

	EventExitCode handEvent()
	{
		boost::unique_lock<boost::shared_mutex> ul(_wr_mtx);
		assert(_dataQue);
		if (!_dataQue) { return EEC_Data_error; }
		if (_dataQue->size()) {
			assert(_security);
			if (!_security) { return EEC_SEC_error; }
			TCP_Buffer& tcp_buffer = _dataQue->front();
			TCP_Head* pHead = reinterpret_cast<TCP_Head*>(tcp_buffer.cbBuffer);
			WORD wOffset = 0;
			//开始解包
			while (tcp_buffer.finalSize >= sizeof(TCP_Head)) {
				//效验数据
				WORD wPacketSize = pHead->TCPInfo.wPacketSize;
				WORD data_size = 0;
				if (wPacketSize > SOCKET_TCP_BUFFER) { return  EEC_Package_Over_Length; }
				if (wPacketSize < sizeof(TCP_Head)) { return  EEC_Package_Head_error; }
				if (pHead->TCPInfo.cbDataKind != DK_ENCRYPT) { return  EEC_Package_ENCRYPT_error; }
				if (tcp_buffer.finalSize < wPacketSize) continue;
				data_size = _security->CrevasseBuffer(tcp_buffer.cbBuffer + wOffset, wPacketSize);
				assert(data_size >= sizeof(TCP_Head));
				if (data_size < sizeof(TCP_Head)) { continue; }
				TCP_Command& Command = pHead->CommandInfo;
				//直接处理心跳包
				if (Command.wMainCmdID == MDM_KN_COMMAND && Command.wSubCmdID == SUB_KN_DETECT_SOCKET) {
					//do nothing
					continue;
				}
				_eventHander->onMessage(_nSocketID, Command, tcp_buffer.cbBuffer + sizeof(TCP_Head) + wOffset, data_size - sizeof(TCP_Head));
				wOffset += wPacketSize;
				tcp_buffer.finalSize -= wPacketSize;
				pHead = reinterpret_cast<TCP_Head*>(tcp_buffer.cbBuffer + wOffset);
			}
			_dataQue->pop_front();
		} else {
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
		return  EEC_OK;
	}

    void run()
    {
		EventExitCode exit_code = EEC_OK;
		try {
			while (exit_code == EEC_OK && process_running) {
				exit_code = handEvent();
			}
		} catch (std::exception& e) {
			printf("Tcp package error: %s\n", e.what());
			process_running = false;
		} catch (...) {
			printf("Unkown hander message error.\n");
			process_running = false;
			//exit_code = EEC_SEC_error;
		}
		switch (exit_code) {
		case EEC_OK:
			printf("消息处理线程安全退出.\n");
			break;
		case EEC_Data_error:
			printf("消息处理线程数据异常退出.\n");
			break;
		case EEC_Package_Over_Length:
			printf("消息处理线程数据超长退出.\n");
			break;
		case EEC_Package_ENCRYPT_error:
			printf("消息处理线程解密数据错误退出.\n");
			break;
		case EEC_Package_Head_error:
			printf("消息处理线程数据头错误退出.\n");
			break;
		case EEC_SEC_error:
			printf("消息处理线程解密模块错误退出.\n");
			break;
		}
		process_end = true;
    }

protected:

    //////////////////////////////////////////////////////////////////////////

    Security*  _security = nullptr;
    Data_queue<TCP_Buffer>* _dataQue = nullptr;

    //////////////////////////////////////////////////////////////////////////

    TcpEventHander* _eventHander = nullptr;
    boost::thread* _evt_thr = nullptr;
    boost::shared_mutex  _wr_mtx; //写入锁

    //////////////////////////////////////////////////////////////////////////

	bool process_end;
    bool process_running;

    unsigned long _nSocketID;

};

}

#endif