#ifndef _TINY_HTTP_SERVER_H_
#define _TINY_HTTP_SERVER_H_

#include "stdafx.h"

#include "commads.hpp"
#include "boost_http/server.hpp"
#include "PaymentSystem/PaymentSystem.h"
#include "Configration/PayConfigReader.h"
#include "Tcp/tcp_server.hpp"

#pragma pack(push, 1)

namespace payment {

namespace TinyServerThread
{
    http::server::server* _httpServer;
    tcp_con::tcp_server* _tcpServer;

    void http_run(int argc, char* argv[])
    {
        try {

            auto& config = PayConfigReader::getConfig();
            _httpServer = new http::server::server(config.strSocektAddr, config.strHttpPort, config.strHttpRoot);

            printf("Http service on address %s port %s, resource root %s\n",config.strSocektAddr.c_str(), config.strHttpPort.c_str(), config.strHttpRoot.c_str());

            _httpServer->run();

        }catch (std::exception& e){

            std::cerr << "exception: " << e.what() << "\n";

        }
    }

    void tcp_run(tcp_con::TcpEventHander* pHander)
    {
        try {

            auto& config = PayConfigReader::getConfig();
            _tcpServer = new tcp_con::tcp_server(config.strSocektAddr, config.strSocketPort);

            printf("Tcp service on address %s port %s.\n", config.strSocektAddr.c_str(), config.strSocketPort.c_str());
            // Run the server until stopped.
            _tcpServer->run(pHander);

        } catch (std::exception& e) {

            std::cerr << "exception: " << e.what() << "\n";

        }
    }

    void stop()
    {
        PaymentSystem::Instance()->shutdown();
        _httpServer->stop();
        _tcpServer->stop();
    }

    void wait_cmd()
    {
        CMDS cmd = MD_NULL;
        while ( true ) {

            cmd = Commands::cmd_service();
            switch (cmd) {
            case payment::MD_ERROR:
                break;
            case payment::MD_EXIT:
                //TODO: stop service
                printf("Http server stoped normally.\n");
                printf("TCP server stoped normally.\n");
                return;
            default:
                break;
            }

        }
    }

    void thread_run(int argc, char* argv[])
    {

        PayConfigReader::readConfig(std::string("payment.ini"));
        PaymentSystem::Instance()->startup();

        boost::thread http_thr(boost::bind(&http_run, argc, argv));
        boost::thread tcp_thr(boost::bind(&tcp_run, PaymentSystem::Instance()));

        // Run the server until stopped.
        wait_cmd();

    }
};

}

#pragma pack(pop)

#endif