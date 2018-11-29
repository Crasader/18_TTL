#include "stdafx.h"
#include "PaymentSystem.h"

#include "../boost_http/reply.hpp"
#include "../boost_http/request.hpp"

#include "../BusinessModule/WorkerFactory.hpp"
#include "../Configration/PayConfigReader.h"
#include "Products.hpp"

#include "Tcp/tcp_conection.hpp"

using namespace payment;
using namespace http::server;
using namespace business;

PaymentSystem* PaymentSystem::_instance = NULL;

PaymentSystem* PaymentSystem::Instance()
{
    if (_instance == nullptr)
        _instance = new PaymentSystem();
    return _instance;
}

PaymentSystem::~PaymentSystem()
{
    shutdown();
    if (_instance)
        delete _instance;
}

bool PaymentSystem::startup()
{
    PayConfig config = PayConfigReader::getConfig();
    bUseWeiXin = config.bUseWeixinPay;

    if (bUseWeiXin) {
        _weixin = WorkerFactory::Instance()->Create(PD_WeiXin);
        _workers.insert(WKPR(PD_WeiXin, _weixin));
    }

    if (config.strPlatformID == "congcong"){
        _currentPlatformID = PD_CongCong;
        _workers.insert(WKPR(PD_CongCong, WorkerFactory::Instance()->Create(PD_CongCong)));
    }else if (config.strPlatformID == "gansu") {
        _currentPlatformID = PD_GanSu;
        _workers.insert(WKPR(PD_GanSu, WorkerFactory::Instance()->Create(PD_GanSu)));
    }

    return false;
}

void PaymentSystem::shutdown()
{
    for (WKMP::iterator it = _workers.begin(); it != _workers.end(); it++) {
        delete it->second;
    }
    
    if(_weixin)
        delete _weixin;

    _workers.clear();
}

void PaymentSystem::genOrderSerial(int serverAccountID, const char *account, const char *name, const char *productID)
{
    //TODO
    //gen order
}

bool PaymentSystem::onHttpRequest(std::string req_path, const request& req, reply& rep)
{
    auto wr = _workers.find(_currentPlatformID);
    assert(wr != _workers.end());

    if (bUseWeiXin) {

        wr = _workers.find(PD_WeiXin);
        assert(wr != _workers.end());

        if (wr != _workers.end()) {

            if (wr->second->onHttpRequest(req_path, req, rep)) {
                return wr->second->onHandSuccess(req, rep);
            }

        }
    }

    if (wr != _workers.end()){

        if (wr->second->onHttpRequest(req_path, req, rep)) {
            return wr->second->onHandSuccess(req, rep);
        } 

    }

    assert(false);

    return false;
}

bool PaymentSystem::onHandFailedDefualt(const request& req, reply& rep)
{
    rep.content.clear();
    rep.headers.clear();
    rep.status = rep.internal_server_error;

    return false;
}

PaymentWorker* PaymentSystem::getCurPaymentWorker()
{
    auto it_wk = _workers.find(_currentPlatformID);
    assert(it_wk != _workers.end());

    if (it_wk != _workers.end())
        return it_wk->second;
    return nullptr;
}

bool payment::PaymentSystem::onMessage(unsigned long nSocketID, TCP_Command& Command, void * pBuffer, WORD wDataSize)
{
    auto it_wk = _workers.find(_currentPlatformID);
    assert(it_wk != _workers.end());
    if (it_wk == _workers.end()) {
        printf("Can't find a suitable worker for %d\n", _currentPlatformID);
        return false;
    }

	auto con = tcp_connection::getConection(nSocketID);
    assert(con);
    if (!con) {
        printf("Can't find a suitable server for %d\n", nSocketID);
        return false;
    }

    auto woker = it_wk->second;

    switch (Command.wMainCmdID) {
        case MDM_PM_REGIST://服务器注册
        {
            switch (Command.wSubCmdID) {
                case SUB_PM_REGIST:
                {
                    assert(wDataSize == sizeof(CMD_PM_Regist));
                    if (wDataSize != sizeof(CMD_PM_Regist)) return false;

                    CMD_PM_Regist* reg_data = static_cast<CMD_PM_Regist*>(pBuffer);

                    TCP_Command cmd;
                    cmd.wMainCmdID = MDM_PM_REGIST;
                    cmd.wSubCmdID = SUB_PM_REGIST_RET;

                    CMD_PM_Regist_Result reg_result;
                    reg_result.wRegistResult = 0;//成功

                    PayConfig config = PayConfigReader::getConfig();
                    //平台ID不匹配
                    if (strcmp(reg_data->csPlatformID, config.strPlatformID.c_str()) != 0) {
                        reg_result.wRegistResult = RegistResult::RR_Mismatch;
					} else {
						if (reg_data->wServerID != 0) {
							con->setServerID(reg_data->wServerID);
							con->setKindID(reg_data->wServerKindID);
						} else {
							reg_result.wRegistResult = RegistResult::RR_ServerIDErr;
						}
					}

                    con->sendData(cmd, &reg_result, sizeof(reg_result));

                    break;
                }
            }
        }
        case MDM_PM_BESINESS://业务逻辑
        {
            switch (Command.wSubCmdID) {

                case SUB_PM_PAY_SUCCESS:
                {
                    return woker->onMessage(con, Command, pBuffer, wDataSize);
                }

                case SUB_PM_PAY_GENORDER:
                {
                    //TODO: 生成自己的订单
                    break;
                }

                case SUB_PM_PAY_GEN_WEIXINORDER:
                {
                    assert(wDataSize == SUB_PM_GEN_WEIXIN_OD);
                    if (wDataSize != sizeof(SUB_PM_GEN_WEIXIN_OD)) return false;

                    auto wr = _workers.find(_currentPlatformID);
                    assert(wr != _workers.end());

                    if (wr != _workers.end()) {
                        SUB_PM_GEN_WEIXIN_OD* genOd = static_cast<SUB_PM_GEN_WEIXIN_OD*>(pBuffer);
                        auto* prd = wr->second->getProducts().findProduct(genOd->wPoductID);
                        assert(prd); if(prd == nullptr) return false;
                        new boost::thread(boost::bind(&WeiXin::onRequestWeiXinOrder, 
                            dynamic_cast<WeiXin*>(_weixin), 
                            prd, 
                            nSocketID, 
                            genOd->dwTableID, 
                            genOd->dwSocketID, 
                            genOd->dwUserID, 
                            genOd->cstrIP, 
                            genOd->cstrAttach));
                    }

                    break;
                }
            }
            break;
        }

    }

    assert(false);

    return false;

}