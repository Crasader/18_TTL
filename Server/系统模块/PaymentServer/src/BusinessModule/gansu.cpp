#include "stdafx.h"
#include "gansu.h"

#include "Tcp/tcp_conection.hpp"
#include "Configration/PayConfigReader.h"
#include "DataBase/SqlServerConector.h"

using namespace payment;
using namespace http::server;
using namespace business;
using namespace tcp_con;
using namespace osql;

GanSu::GanSu()
{

}

GanSu::~GanSu()
{

}

bool GanSu::onVerifyRequest(std::string req_path, const request& req)
{

    return false;

}

bool GanSu::onHttpRequest(std::string req_path, const request& req, reply& rep)
{

    return false;

}

bool GanSu::onHandSuccess(const request& req, reply& rep)
{
    std::string content_type = "text/html";
    std::string content_value = "{success:true}";

    rep.content.append(content_value.c_str());

    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = content_type;

    rep.status = rep.ok;

    return  true;
}

bool GanSu::onHandFailed(const request& req, reply& rep)
{
    std::string content_type = "text/html";
    std::string content_value = "{success:false}";

    rep.content.append(content_value.c_str());

    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = content_type;

    rep.status = rep.ok;

    return  true;
}

bool GanSu::onMessage(pConect con, TCP_Command& tcp_cmd, void * pBuffer, WORD wDataSize)
{

    switch (tcp_cmd.wMainCmdID){

        case MDM_PM_BESINESS:
            break;

    }
    return true;
}
