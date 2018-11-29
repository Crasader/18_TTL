#include "stdafx.h"
#include "congcong.h"

#include "Tcp/tcp_conection.hpp"
#include "Configration/PayConfigReader.h"
#include "DataBase/SqlServerConector.h"

using namespace payment;
using namespace http::server;
using namespace business;
using namespace tcp_con;
using namespace osql;

CongCong::CongCong()
{

}

CongCong::~CongCong()
{

}

bool business::CongCong::onVerifyRequest(std::string req_path, const request& req)
{

    return false;

}

bool CongCong::onHttpRequest(std::string req_path, const request& req, reply& rep)
{

    int paySt ;
    int amount;
    std::string mchntOrderNo ;
    std::string orderNo;

    std::string userID;
    std::string md5Orig;
    int serverKindID;

    bool bOldVersion = false;
    bool bRequestAgain = false;

    //////////////////////////////////////////////////////////////////////////
    //验证回调是否正确
    header* hd = nullptr;
    if (req.findHeader("content-Length", &hd)) {

        //解析json
        Json::Value responseData;
        Json::Reader jsonReader;

        jsonReader.parse(req.content, responseData);

        paySt = responseData["paySt"].asInt();
        amount = responseData["amount"].asInt();
        mchntOrderNo = responseData["mchntOrderNo"].asString();
        orderNo = responseData["orderNo"].asString();
        std::string extra = responseData["extra"].asString();

        //printf("paySt = %d, amount = %d, mchntOrderNo = %s, orderNo = %s, extra = %s",paySt, amount, mchntOrderNo.c_str(), orderNo.c_str(), extra.c_str());

        std::vector<std::string> exts;
        tool::splitString(extra, "_", exts);

        if (exts.size() == 2) {
            bOldVersion = true;
            userID = exts[0];
            md5Orig = exts[1];
        } else if (exts.size() == 3) {
            userID = exts[0];
            md5Orig = exts[1];
            serverKindID = atoi(exts[2].c_str());
        } else {
            printf("extra data size  =  %d error.\n", exts.size());
            return false;
        }

        auto& config = PayConfigReader::getConfig();
        std::string md5_str = config.strAppID + "*" + config.strMchID + "*" + mchntOrderNo;
        //std::string md5_str = config.strAppID + "*" + config.strAppKey + "*" + exts[0] + mchntOrderNo;

        MD5 md5 = MD5(md5_str);

        std::string strMd5 = md5.hexdigest();
        if (strcmp(md5Orig.c_str(), strMd5.c_str()) != 0) {
            printf("Md5 miss match..\n");
            return false;
        }

    }

    //////////////////////////////////////////////////////////////////////////
    //执行数据库操作

    SqlSevCon->Open();

    char sql_cmd[1024] = { 0 };
    int orderID = -1;
    int onlineID = -1;
    int status = -1;
    int money = -1;

    //查询订单
    sprintf( sql_cmd, "select OrderStatus, OnlineID  from dbo.OnlineOrder where UserID = %s and OrderID = %s", userID.c_str(), orderNo.c_str() );
    SqlSevCon->Excute(sql_cmd, DBOperatType::DBO_READ);

    /*    if (SqlSevCon->ResultSize() > 0) {

        } else*/ {

        //支付结果
        switch (paySt) {
            case 2://成功
            {

                if (SqlSevCon->GetFiledValue("OrderStatus", DataType::DT_INT, &status)) {
                    SqlSevCon->GetFiledValue("OnlineID", DataType::DT_INT, &onlineID);
                    switch (status) {

                        case 1://已经成功充值
                            printf("Pay OrderNo is existed, insert falied. OrderStatus=%d\n", status);
                            return true;

                        case 2://充值失败
                            //继续走充值的流程
                            bRequestAgain = true;
                            break;
                    }
                }

                SqlSevCon->CloseRecord();

                //获取当前的金钱
                memset(sql_cmd, 0, sizeof(sql_cmd));
                sprintf(sql_cmd, "select Score from dbo.GameScoreInfo where UserID=%s", userID.c_str());
                SqlSevCon->Excute(sql_cmd, DBOperatType::DBO_READ);
                SqlSevCon->GetFiledValue("Score", DataType::DT_LONGLONG, &money);
                SqlSevCon->CloseRecord();

                if (bRequestAgain) {

                    //更新订单状态
                    memset(sql_cmd, 0, sizeof(sql_cmd));
                    sprintf(sql_cmd, "update dbo.OnlineOrder set OrderStatus=%d where OnlineID=%d", 1, onlineID);
                    SqlSevCon->Excute(sql_cmd, DBOperatType::DBO_WRITE);
                    SqlSevCon->CloseRecord();

                } else {

                    //查询订单ID
                    memset(sql_cmd, 0, sizeof(sql_cmd));
                    sprintf(sql_cmd, "select Count = count(OnlineID) from dbo.OnlineOrder");
                    SqlSevCon->Excute(sql_cmd, DBOperatType::DBO_READ);
                    SqlSevCon->GetFiledValue("Count", DataType::DT_INT, &orderID);
                    SqlSevCon->CloseRecord();

                    //插入订单号
                    orderID++;
                    memset(sql_cmd, 0, sizeof(sql_cmd));
                    sprintf(sql_cmd, "SET IDENTITY_INSERT dbo.OnlineOrder ON \n \
	                                            insert into dbo.OnlineOrder(UserID,OrderID,OrderAmount,OrderStatus,OnlineID,OperUserID,ShareID,GameID,Accounts,CardTypeID,CardPrice,CardGold,CardTotal,DiscountScale,PayAmount,IPAddress,ApplyDate)\
	                                            values(%s, %s, %d, %d, %d, %s, 0, 0, %s, 0, 0 ,0, 0, 0, %d, 0, getdate())\n \
	                                            SET IDENTITY_INSERT dbo.OnlineOrder OFF",
                                                userID.c_str(), orderNo.c_str(), amount, 1, orderID, userID.c_str(), userID.c_str(), amount);
                    SqlSevCon->Excute(sql_cmd, DBOperatType::DBO_WRITE);
                    SqlSevCon->CloseRecord();

                }

                //更新金钱
                memset(sql_cmd, 0, sizeof(sql_cmd));
                sprintf(sql_cmd, "update dbo.GameScoreInfo set Score=%d where UserID=%s", money + amount, userID.c_str());
                SqlSevCon->Excute(sql_cmd, DBOperatType::DBO_WRITE);
                SqlSevCon->CloseRecord();
                printf("Player %s add money %d", userID.c_str(), amount);
                break;
            }
            default:
                break;
        }

    }

    SqlSevCon->Close();

    if (bOldVersion) {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //通知游戏服务器

    pConect con = tcp_connection::findConectionByKindID(serverKindID);
    assert(con);
    if (!con) {
		printf("A http request failed : Can't match game server kind ID: %d.\n", serverKindID);
        return false;
    }

    SUB_PM_PAY_SUC pay_suc;

    memset(&pay_suc, 0, sizeof(pay_suc));
    pay_suc.wServerID = serverKindID;
    pay_suc.dwUserSocketID = atoi(userID.c_str());

    TCP_Command cmd;
    cmd.wMainCmdID = MDM_PM_BESINESS;
    cmd.wSubCmdID = SUB_PM_PAY_SUCCESS;

    con->sendData(cmd, &pay_suc, sizeof(pay_suc));
    return true;
}

bool business::CongCong::onHandSuccess(const request& req, reply& rep)
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

bool CongCong::onHandFailed(const request& req, reply& rep)
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

bool CongCong::onMessage(pConect con, TCP_Command& tcp_cmd, void * pBuffer, WORD wDataSize)
{

    switch (tcp_cmd.wMainCmdID){

        case MDM_PM_REGIST:
        {
            switch (tcp_cmd.wSubCmdID) {

                case SUB_PM_REGIST:
                {
                    assert(wDataSize == sizeof(CMD_PM_Regist));
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
                    }

                    if (reg_data->wServerID != 0) {
                        con->setServerID(reg_data->wServerID);
                        con->setKindID(reg_data->wServerKindID);
                    } else {
                        reg_result.wRegistResult = RegistResult::RR_ServerIDErr;
                    }

                    con->sendData(cmd, &reg_result, sizeof(reg_result));

                    break;
                }

            }

            break;
        }

    }

    return true;

}
