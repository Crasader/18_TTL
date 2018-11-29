#ifndef _TCP_SERVER_HPP_
#define _TCP_SERVER_HPP_

//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "stdafx.h"

#include "tcp_conection.hpp"
#include "../Configration/PayConfigReader.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace tcp_con {

class tcp_server
{

protected:

    tcp_server() = delete;
    tcp_server(tcp_server&) = delete;
    tcp_server& operator=(tcp_server&) = delete;

public:

    tcp_server(std::string strAddr, std::string strPort)
    {

        _strServerAddr = strAddr;
        _strServerPort = strPort;

        address adrs;
        adrs.from_string(_strServerAddr);

        tcp::endpoint ed(adrs, atoi(_strServerPort.c_str()));

        _pIo_service = new io_service();
        _pSocket = new tcp::socket(*_pIo_service);
        _pAcceptor = new tcp::acceptor((*_pIo_service), ed);

    }

    virtual ~tcp_server()
    {

        if(_pIo_service != nullptr)
            delete _pIo_service;
        if(_pAcceptor != nullptr)
            delete _pAcceptor;
        if( _pSocket != nullptr)
            delete _pSocket;

    }

public:

    void run(TcpEventHander* pEvtHander)
    {

        start_accept(pEvtHander);
        _pIo_service->run();

    }

    void stop()
    {

        _pIo_service->stop();

    }

private:
    void start_accept(TcpEventHander* pEvtHander)
    {

        pConect new_connection = tcp_connection::create(_pAcceptor->get_io_service());

        _pAcceptor->async_accept(new_connection->socket(),
            boost::bind(&tcp_server::handle_accept,
                this,
                new_connection,
                boost::asio::placeholders::error,
                pEvtHander)
        );

    }

    void handle_accept(pConect new_connection,
        const boost::system::error_code& error,
        TcpEventHander* pEvtHander)
    {

        if (!error){

            printf("new connection at %s.\n", new_connection->socket().local_endpoint().address().to_string().c_str());
            new_connection->registEventhander(pEvtHander);
            new_connection->start();

        }

        start_accept(pEvtHander);

    }

private:

    std ::string _strServerAddr="";
    std::string _strServerPort="";

    io_service* _pIo_service = nullptr;
    tcp::acceptor* _pAcceptor = nullptr;
    tcp::socket* _pSocket = nullptr;


};
}

#endif