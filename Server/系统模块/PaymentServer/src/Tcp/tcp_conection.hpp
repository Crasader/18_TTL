#ifndef _TCP_CONECTION_HPP_
#define _TCP_CONECTION_HPP_

#include "stdafx.h"

#include "packet.h"
#include "security.hpp"
#include "tcp_event_hander_thread.hpp"
#include "tcp_event_sender_thread.hpp"
#include "Tools/data_deque.hpp"

using namespace boost::asio::ip;
using namespace boost::system::errc;


namespace tcp_con {

struct ServerBindInfo 
{
    //平台发送者当前游戏的ID
    unsigned long nServerID;
    //游戏类型ID
    unsigned long nServerKindID;
};

static void conection_push(Data_queue<TCP_Buffer>* que, size_t size) {
	if(que)
		que->end().finalSize = static_cast<WORD>(size);
}

class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{

public:

    ~tcp_connection()
    {
    }

    static pConect create(boost::asio::io_service& io_service)
    {
        return pConect(new tcp_connection(io_service));
    }

public:

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        nConectCount++;

        nSocketID = nConectCount;
        ConectionMap.insert(std::pair<int, boost::shared_ptr<tcp_connection>>(nSocketID,shared_from_this()));

        _evt_hd_thr.setSocketID(nSocketID);

        _evt_hd_thr.start(&_security, &_queue_recive, _pEvtHander);
        _evt_sd_thr.start(&_security, &_queue_send);

        start_read();
        start_write();

        //_heart_tick_thr = new boost::thread(boost::bind(&tcp_connection::sendHeartTick, this));

    }

    void stop()
    {

		if(_heart_tick_thr)
			_heart_tick_thr->interrupt();

        _evt_hd_thr.stop();
        _evt_sd_thr.stop();

        socket_.close();

        auto it = ConectionMap.find(nSocketID);
        if (it != ConectionMap.end())
            ConectionMap.erase(it);

    }

//////////////////////////////////////////////////////////////////////////

public:

    void setServerID(unsigned long serverID)
    {

        _serverInfo.nServerID = serverID;

    }

    unsigned long getServerID()
    {

        return _serverInfo.nServerID;

    }

    void setKindID(unsigned long kindID)
    {

        _serverInfo.nServerKindID = kindID;

    }

    unsigned long getKindID()
    {

        return _serverInfo.nServerKindID;

    }

    void sendData(TCP_Command& comander, void* pdata, size_t sizet)
    {

        _evt_sd_thr.sendData(comander, pdata, sizet);
		notifyWrite();

    }

    void registEventhander(TcpEventHander* pEvtHander)
    {

        _pEvtHander = pEvtHander;

    }

    static pConect getConection(unsigned long socketID)
    {

        auto it = ConectionMap.find(socketID);
        if (it != ConectionMap.end()) {
            return it->second;
        }

        return nullptr;

    }

    static pConect findConectionByServerID(unsigned long serverID)
    {

        for (auto it = ConectionMap.begin(); it != ConectionMap.end(); it++) {

            if (it->second->getServerID() == serverID) {
                return it->second;
            }

        }

        return nullptr;

    }

    static pConect findConectionByKindID(unsigned long kindID)
    {

        for (auto it = ConectionMap.begin(); it != ConectionMap.end(); it++) {

            if (it->second->getKindID() == kindID) {
                return it->second;
            }

        }

        return nullptr;

    }

private:

    tcp_connection(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
    }

	void notifyWrite()
	{

		scoped_lock<interprocess_mutex> lock(_wr_mtx);
		start_write();

	}

    void start_write()
    {
		if (_queue_send.size()) {
			TCP_Buffer& tcp_buffer = _queue_send.front();

			boost::asio::async_write(socket_,
				boost::asio::buffer(&_queue_send.front(), tcp_buffer.finalSize),
				boost::bind(&tcp_connection::handle_write,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)
			);
		}
    }

    void handle_write(const boost::system::error_code& err_code/*error*/,
        size_t  size/*bytes_transferred*/)
    {

        if (!err_code){

            try {
                _queue_send.pop_front();
            } catch (std::exception e) {
                printf("Send buffer to %s failed : %s.\n", socket_.local_endpoint().address().to_string().c_str(), e.what());
            }

            if (_queue_send.size()) {

                boost::asio::async_write(socket_,
                    boost::asio::buffer(&_queue_send.front(), _queue_send.front().finalSize),
                    boost::bind(&tcp_connection::handle_write,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)
                );

            }

        }else{

            printf("Send buffer to %s failed, close the socket\n", socket_.local_endpoint().address().to_string().c_str());
            socket_.close();

        }

    }

    void start_read()
    {

        socket_.async_read_some(boost::asio::buffer(&(_queue_recive.end()), SOCKET_TCP_BUFFER),
            boost::bind(&tcp_connection::handle_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred)
        );

    }

    void handle_read(const boost::system::error_code& err_code/*error*/,
        size_t  size/*bytes_transferred*/)
    {

        if (!err_code){

            if (!_evt_hd_thr.isRunning()) {
                printf("From %s read hande event error, close conection.\n", socket_.local_endpoint().address().to_string().c_str());
                stop();
                return;
            }

            if (!_queue_recive.isFull()) {

                try {
                    _queue_recive.push_back(size, &conection_push);
                } catch (std::exception e) {
                    printf("From %s read buffer error : %s. \n", socket_.local_endpoint().address().to_string().c_str(), e.what());
                }

            }

            socket_.async_read_some(boost::asio::buffer(&(_queue_recive.end()), SOCKET_TCP_BUFFER),
                boost::bind(&tcp_connection::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred )
            );

        }else{

            switch (err_code.value()){

                case connection_aborted:
                default:
                {
                    printf("From %s read buffer error err_code = %d, close the socket\n", socket_.local_endpoint().address().to_string().c_str(), err_code.value());
                    stop();
                    break;
                }

            }

        }

    }

    void sendHeartTick()
    {

		//boost::this_thread::sleep(boost::posix_time::milliseconds(HEART_TICK_TIME * 2));
        while (true) {

            boost::this_thread::sleep(boost::posix_time::milliseconds(HEART_TICK_TIME));

            TCP_Command tcp_cmd;

            memset(&tcp_cmd, 0, sizeof tcp_cmd);
            tcp_cmd.wMainCmdID = MDM_KN_COMMAND;
            tcp_cmd.wSubCmdID = SUB_KN_DETECT_SOCKET;

            CMD_KN_DetectSocket DetectSocket;
            DetectSocket.dwSendTickCount = GetTickCount();

            _evt_sd_thr.sendData(tcp_cmd, &DetectSocket, sizeof(DetectSocket));

        }

    }

protected:

    tcp::socket socket_;

    //////////////////////////////////////////////////////////////////////////

    //加解密
    Security _security;

    //////////////////////////////////////////////////////////////////////////

    //接收队列
	Data_queue<TCP_Buffer> _queue_recive;

    //////////////////////////////////////////////////////////////////////////

    //发送队列
	Data_queue<TCP_Buffer> _queue_send;

    //////////////////////////////////////////////////////////////////////////

    //消息处理线程
    TcpEventHandThread _evt_hd_thr;
    //消息发送线程
    TcpEventSendThread _evt_sd_thr;
    //心跳包线程
    boost::thread* _heart_tick_thr = nullptr;

    //////////////////////////////////////////////////////////////////////////

    //注册当前的平台接收者
    TcpEventHander* _pEvtHander = nullptr;
    ////注册当前的平台发送者
    //TcpEventSender* _pEvtSender = nullptr;

    //////////////////////////////////////////////////////////////////////////

    //网络ID标识, 没有实际的意义
    unsigned long nSocketID;
    //绑定游戏服务器信息
    ServerBindInfo _serverInfo;
    //////////////////////////////////////////////////////////////////////////

	//发送消息队列同步锁
	interprocess_mutex      _wr_mtx;

	//////////////////////////////////////////////////////////////////////////

private:

    //全局变量
    //////////////////////////////////////////////////////////////////////////
    //标识当前的连接id
    static unsigned long  nConectCount;
    //保存当前所有连接
    static ConectMap ConectionMap;

};


}

#endif