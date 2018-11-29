#include "stdafx.h"
#include "weixin.h"

#include "Tcp/tcp_conection.hpp"
#include "Configration/PayConfigReader.h"
#include "DataBase/SqlServerConector.h"
#include "tinyxml2/tinyxml.h"

using namespace payment;
using namespace http::server;
using namespace business;
using namespace tcp_con;
using namespace osql;

void payment_push(Data_queue<UrlBuffer>* que, size_t size)
{
    if (que)
        que->end().size = static_cast<WORD>(size);
}

size_t genWeiXinOrderResult(void *buffer, size_t size, size_t nmemb, void *customPtr)
{
    RequestData* rd = static_cast<RequestData*>(customPtr);
    Data_queue<UrlBuffer>* orderQue = rd->que;
    if (orderQue)
    {
        size_t dataSize = size * nmemb;
        rd->idx = orderQue->push_back(dataSize, payment_push);

        assert(dataSize < UrlBufferLength);
        if (dataSize >= UrlBufferLength) {
            rd->idx = -1;
            return 0;
        }

        char* data = orderQue->at(rd->idx).data;
        memcpy(data, buffer, dataSize);
        data[dataSize] = 0;
        return dataSize;
    }
    return 0;
}

WeiXin::WeiXin()
{

}

WeiXin::~WeiXin()
{

}

bool WeiXin::onVerifyRequest(std::string req_path, const request& req)
{
    return false;
}

bool business::WeiXin::onRequestWeiXinOrder(const Product* product,
    unsigned long nServerSocketID,
    DWORD tableID,
    DWORD dwUserSocketID,
    DWORD userID,
    std::string IP,
    std::string attach)
{
    assert(product != nullptr); if(product== nullptr) return false;

    auto& config = PayConfigReader::getConfig();
    auto& wx_order_xml = config.strWeiXinOrderXml;

    TiXmlDocument genOrderXml;
    TiXmlPrinter xmlPrinter;
    TiXmlNode* ele_sign = nullptr;

    WeiXinOrderRequest wxOdReq;

    try {

        char server_attch[128] = {0};
        sprintf_s(server_attch, "%ld@%ld@%s", nServerSocketID, tableID, product->getPoductValue("product_type").cStr);

		//printf("server_attch = %s\n", server_attch);

        wxOdReq.attach = server_attch;
        wxOdReq.body = product->getPoductValue("body").cStr;
        char fee[32] = {0};
        sprintf_s(fee, "%d", product->getPoductValue("total_fee").nValue);
        wxOdReq.total_fee = fee;
        wxOdReq.trade_type = product->getPoductValue("trade_type").cStr;
        wxOdReq.nonce_str = tool::randString(16);
        wxOdReq.appid = config.strAppID;
        wxOdReq.mch_id = config.strMchID;
        wxOdReq.notify_url = config.strNotifyUrl;

        //TODO:生成订单号并且保存到数据库

        char out_order[128] = { 0 };
        auto tt = time(nullptr);
        sprintf_s(out_order, "%ld_%ld_%ld", static_cast<unsigned long>(tt), userID, dwUserSocketID);
        wxOdReq.out_trade_no = out_order;
        wxOdReq.spbill_create_ip = IP;

        genOrderXml.Parse(wx_order_xml.c_str());

        auto* ele = genOrderXml.RootElement()->FirstChild();
        while (ele != nullptr) {
            auto* data = ele->Value();
            if (data == nullptr) continue;
            if (strcmp(data, "appid") == 0) {
                ele->LinkEndChild(new TiXmlText(wxOdReq.appid.c_str()));
            } else if (strcmp(data, "attach") == 0) {//附加数据
                ele->LinkEndChild(new TiXmlText(wxOdReq.attach.c_str()));
            } else if (strcmp(data, "body") == 0) {//商品描述
                ele->LinkEndChild(new TiXmlText(wxOdReq.body.c_str()));
            } else if (strcmp(data, "mch_id") == 0) {//商户号
                ele->LinkEndChild(new TiXmlText(wxOdReq.mch_id.c_str()));
            } else if (strcmp(data, "nonce_str") == 0) {//随机字符串
                ele->LinkEndChild(new TiXmlText(wxOdReq.nonce_str.c_str()));
            } else if (strcmp(data, "sign") == 0) {//签名
                ele_sign = ele;
            } else if (strcmp(data, "notify_url") == 0) {//通知地址
                ele->LinkEndChild(new TiXmlText(wxOdReq.notify_url.c_str()));
            } else if (strcmp(data, "out_trade_no") == 0) {//商户订单号
                ele->LinkEndChild(new TiXmlText(wxOdReq.out_trade_no.c_str()));
            } else if (strcmp(data, "spbill_create_ip") == 0) {//用户IP
                ele->LinkEndChild(new TiXmlText(wxOdReq.spbill_create_ip.c_str()));
            } else if (strcmp(data, "total_fee") == 0) {//金额
                ele->LinkEndChild(new TiXmlText(wxOdReq.total_fee.c_str()));
            } else if (strcmp(data, "trade_type") == 0) {//交易类型
                ele->LinkEndChild(new TiXmlText(wxOdReq.trade_type.c_str()));
            }
            ele = ele->NextSiblingElement();
        }
    } catch (...) {
        printf("A request failed when make request xml.\n");
        return false;
    }

    //按照字母key顺序打签名
    std::string key_str;
    key_str += "appid=" + wxOdReq.appid;
    key_str += "&attach=" + wxOdReq.attach;
    key_str += "&body=" + wxOdReq.body;
    key_str += "&mch_id=" + wxOdReq.mch_id;
    key_str += "&nonce_str=" + wxOdReq.nonce_str;
    key_str += "&notify_url=" + wxOdReq.notify_url;
    key_str += "&out_trade_no=" + wxOdReq.out_trade_no;
    key_str += "&spbill_create_ip=" + wxOdReq.spbill_create_ip;
    key_str += "&total_fee=" + wxOdReq.total_fee;
    key_str += "&trade_type=" + wxOdReq.trade_type;
    key_str += "&key=" + config.strSecretKey;

    //printf("weixin genOrder sign str = %s\n", key_str.c_str());

    MD5 md5 = MD5(key_str);
    std::string strMd5 = boost::algorithm::to_upper_copy(md5.hexdigest());
    //printf("weixin genOrder sign md5 = %s\n", strMd5.c_str());

    ele_sign->LinkEndChild(new TiXmlText(strMd5.c_str()));

    genOrderXml.Accept(&xmlPrinter);
    //printf("weixin order url: %s\ngen weixin order: \n%s\n", config.strWeiXinOrder.c_str(), xmlPrinter.CStr());

    //3.发送url请求//////////////////////////////////////////////////////////////////////////

    CURLcode code = CURLE_OK;
    RequestData data = { -1 , &_genOrderQue };
    try {
        CURL *curl = nullptr;
        curl = curl_easy_init();
        if (curl) {
            //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // 跳过证书检查
            //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // 从证书中检查SSL加密算法是否存在
            curl_easy_setopt(curl, CURLOPT_URL, config.strWeiXinOrder.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, true);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xmlPrinter.CStr());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &genWeiXinOrderResult);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&data);
            code = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
    } catch (...) {
        printf("Wei Xin order request failed.\n");
        return false;
    }

    //4.解析返回数据//////////////////////////////////////////////////////////////////////////

    TiXmlDocument resultXml;
    //TiXmlPrinter xmlpt;

    WeiXinOrderResult wxOdRet;

    try {

        if (code == CURLE_OK) {
            auto* xml_str = resultXml.Parse(_genOrderQue.at(data.idx).data);
            auto* ch = resultXml.RootElement()->FirstChild();

            while (ch != nullptr) {
                auto* ele = ch->ToElement();
                if (strcmp(ele->Value(), "return_code") == 0) {//返回状态码
                    wxOdRet.return_code = ele->GetText();
                } else if (strcmp(ele->Value(), "return_msg") == 0) {//返回信息
                    wxOdRet.return_msg = ele->GetText();
                } else if (strcmp(ele->Value(), "appid") == 0) {//应用APPID
                    wxOdRet.appid = ele->GetText();
                } else if (strcmp(ele->Value(), "mch_id") == 0) {//商户号
                    wxOdRet.mch_id = ele->GetText();
                } else if (strcmp(ele->Value(), "nonce_str") == 0) {//随机字符串
                    wxOdRet.ret_nonce_str = ele->GetText();
                } else if (strcmp(ele->Value(), "sign") == 0) {//签名
                    wxOdRet.sign_str = ele->GetText();
                } else if (strcmp(ele->Value(), "result_code") == 0) {//业务结果
                    wxOdRet.result_code = ele->GetText();
                } else if (strcmp(ele->Value(), "err_code") == 0) {//错误代码
                    wxOdRet.err_code = ele->GetText();
                } else if (strcmp(ele->Value(), "trade_type") == 0) {//交易类型
                    wxOdRet.trade_type = ele->GetText();
                } else if (strcmp(ele->Value(), "prepay_id") == 0) {//预支付交易会话标识
                    wxOdRet.prepay_id = ele->GetText();
                }
                ch = ch->NextSiblingElement();
            }

            //返回请求结果
            if (wxOdRet.result_code == "SUCCESS") {
                pConect con = tcp_connection::getConection(nServerSocketID);
                assert(con);
                if (!con) {
					printf("A http request failed : Can't match game server dwSocketID: %d.\n", nServerSocketID);
                    return false;
                }

                SUB_PM_GEN_WEIXIN_OD_RET orderRet;
                memset(&orderRet, 0, sizeof(orderRet));
                orderRet.dwUserID = userID;
                orderRet.dwTableID = tableID;
                orderRet.wPoductID = product->getPoductValue("id").nValue;
                sprintf(orderRet.nonce_str, "%s", wxOdRet.ret_nonce_str.c_str());
                orderRet.nonce_str_len = static_cast<WORD>(wxOdRet.ret_nonce_str.length());
                sprintf(orderRet.prepare_id, "%s", wxOdRet.prepay_id.c_str());
                orderRet.prepare_id_len = static_cast<WORD>(wxOdRet.prepay_id.length());
                sprintf(orderRet.order, "%s", wxOdReq.out_trade_no.c_str());
                orderRet.order_len = static_cast<WORD>(wxOdReq.out_trade_no.length());
                orderRet.dwSocketID = dwUserSocketID;

                TCP_Command cmd;
                cmd.wMainCmdID = MDM_PM_BESINESS;
                cmd.wSubCmdID = SUB_PM_PAY_WEIXINORDER_RET;
                con->sendData(cmd, &orderRet, sizeof(orderRet));

				printf("Player %ld gen weixin order %s sucess.\n", userID, orderRet.order);

            }else {
                printf("WeiXin pay return_msg: %s, error_code = %s\n", wxOdRet.return_msg.c_str(), wxOdRet.err_code.c_str());
                return false;
            }
            _genOrderQue.pop_front();
        }
        //resultXml.Accept(&xmlpt);
        //printf("weixin order result: %s\n", xmlpt.CStr());
    } catch (...) {

        printf("Parse weixin order result error.\n");
        _genOrderQue.pop_front();
        return false;

    }

    return true;

}

bool WeiXin::onHttpRequest(std::string req_path, const request& req, reply& rep)
{

    //1.解析返回结果//////////////////////////////////////////////////////////////////////////
    header* payRet = nullptr;
    if (!req.findHeader("content-Length", &payRet)) { //没有数据
        printf("onVerifyRequest : bad request with no content.\n");
        return false;
    }

    TiXmlDocument pay_result;
    //TiXmlPrinter xmlPrinter;

    bool sucess = false;
    WeiXinPayResult wxRet;

    try {
        pay_result.Parse(req.content.c_str());
        auto* ch = pay_result.RootElement()->FirstChild();
        while (ch != nullptr) {
            auto* ele = ch->ToElement();
            if (strcmp(ele->Value(), "return_code") == 0) {//返回状态码
                wxRet.return_code = ele->GetText();
            } else if (strcmp(ele->Value(), "return_msg") == 0) {//返回信息
                wxRet.return_msg = ele->GetText();
            } else if (strcmp(ele->Value(), "appid") == 0) {//应用APPID
                wxRet.appid = ele->GetText();
            } else if (strcmp(ele->Value(), "attach") == 0) {//应用APPID
                wxRet.attach = ele->GetText();
            } else if (strcmp(ele->Value(), "mch_id") == 0) {//商户号
                wxRet.mch_id = ele->GetText();
            } else if (strcmp(ele->Value(), "nonce_str") == 0) {//随机字符串
                wxRet.nonce_str = ele->GetText();
            } else if (strcmp(ele->Value(), "sign") == 0) {//签名
                wxRet.sign = ele->GetText();
            } else if (strcmp(ele->Value(), "result_code") == 0) {//业务结果
                wxRet.result_code = ele->GetText();
            } else if (strcmp(ele->Value(), "err_code") == 0) {//错误代码
                wxRet.err_code = ele->GetText();
            } else if (strcmp(ele->Value(), "trade_type") == 0) {//交易类型
                wxRet.trade_type = ele->GetText();
            } else if (strcmp(ele->Value(), "openid") == 0) {//用户标识
                wxRet.openid = ele->GetText();
            } else if (strcmp(ele->Value(), "bank_type") == 0) {//付款银行
                wxRet.bank_type = ele->GetText();
            } else if (strcmp(ele->Value(), "total_fee") == 0) {//总金额
                wxRet.total_fee = ele->GetText();
            } else if (strcmp(ele->Value(), "cash_fee") == 0) {//现金支付金额
                wxRet.cash_fee = ele->GetText();
            } else if (strcmp(ele->Value(), "fee_type") == 0) {//现金支付金额
                wxRet.fee_type = ele->GetText();
            } else if (strcmp(ele->Value(), "is_subscribe") == 0) {//现金支付金额
                wxRet.is_subscribe = ele->GetText();
            } else if (strcmp(ele->Value(), "transaction_id") == 0) {//微信支付订单号
                wxRet.transaction_id = ele->GetText();
            } else if (strcmp(ele->Value(), "out_trade_no") == 0) {//商户订单号
                wxRet.out_trade_no = ele->GetText();
            } else if (strcmp(ele->Value(), "time_end") == 0) {//支付完成时间
                wxRet.time_end = ele->GetText();
            }
            ch = ch->NextSiblingElement();
        }
    } catch (...) {
        printf("onVerifyRequest : bad request with bad xml data.\n");
        return false;
    }

    //pay_result.Accept(&xmlPrinter);
    //printf("pay resut:\n%s\n", xmlPrinter.CStr());

    //2.验证签名////////////////////////////////////////////////////////////////////////
    WX_PAY_ER_CODE errorCode = WPRC_OK;
    ORDER_STATE status = OS_PAY_NULL;

    std::string Order = "";
    std::string userID = "";
	std::string userIDSocketID = "";
    std::string servSocketID = "";
    std::string tableID = "";
	std::string owntype = "";
    {

        auto& config = PayConfigReader::getConfig();

        if ( wxRet.result_code != "SUCCESS") {//微信后台支付失败
            printf("onVerifyRequest : pay failed %s, err_code: %s.\n", wxRet.return_msg.c_str(), wxRet.err_code.c_str());
            errorCode = WPRC_FAILED;
            //return false;
        }

        if( wxRet.appid != config.strAppID
            || wxRet.mch_id != config.strMchID)  {
            printf("onVerifyRequest : weixin id can't match.\n");
            errorCode = WPRC_KEY_MISS_MATCH;
            goto STEP_WX_UPDATE_ORDER_STATE;
        }

        //按照字母key顺序打签名
        std::string sign_str;
        sign_str += "appid=" + wxRet.appid;
        sign_str += "&attach=" + wxRet.attach;
        sign_str += "&bank_type=" + wxRet.bank_type;
        sign_str += "&cash_fee=" + wxRet.cash_fee;
        sign_str += "&fee_type=" + wxRet.fee_type;
        sign_str += "&is_subscribe=" + wxRet.is_subscribe;
        sign_str += "&mch_id=" + wxRet.mch_id;
        sign_str += "&nonce_str=" + wxRet.nonce_str;
        sign_str += "&openid=" + wxRet.openid;
        sign_str += "&out_trade_no=" + wxRet.out_trade_no;
        sign_str += "&result_code=" + wxRet.result_code;
        sign_str += "&return_code=" + wxRet.return_code;
        sign_str += "&time_end=" + wxRet.time_end;
        sign_str += "&total_fee=" + wxRet.total_fee;
        sign_str += "&trade_type=" + wxRet.trade_type;
        sign_str += "&transaction_id=" + wxRet.transaction_id;

        sign_str += "&key=" + config.strSecretKey;

        //printf("weixin pay sign str = %s\n", sign_str.c_str());

        MD5 md5 = MD5(sign_str);
        std::string strMd5 = boost::algorithm::to_upper_copy(md5.hexdigest());
        //printf("weixin pay sign md5 = %s\n", strMd5.c_str());

        if (strMd5 != wxRet.sign) {
            printf("Weixin pay sign verify failed md5 don't match.\n");
            errorCode = WPRC_SIGN_MISS_MATCH;
            goto STEP_WX_UPDATE_ORDER_STATE;
        }

        std::vector<std::string> strsOrder;
        tool::splitString(wxRet.out_trade_no, "_", strsOrder);
        if (strsOrder.size() != 3) {
            printf("Weixin pay can't find user data.\n");
            errorCode = WPRC_USER_DATA_ERROR;
            return false;
        }
        Order = strsOrder[0];
        userID = strsOrder[1];
		userIDSocketID = strsOrder[2];

        std::vector<std::string> strsAttach;
        tool::splitString(wxRet.attach, "@", strsAttach);
        if (strsAttach.size() != 3) {
            printf("Weixin pay can't find server data.\n");
			errorCode = WPRC_USER_DATA_ERROR;
			return false;
        } else {
            servSocketID = strsAttach[0];
            tableID = strsAttach[1];
			owntype = strsAttach[2];
        }
    }

    //3.更新订单状态////////////////////////////////////////////////////////////////////////

STEP_WX_UPDATE_ORDER_STATE:

    int amount = atoi(wxRet.total_fee.c_str());
    int onlineID = -1;

    {
        SqlSevCon->Open();
        SqlSevCon->RequestOrder(userID, wxRet.transaction_id);
        if (SqlSevCon->GetFiledValue("OrderStatus", DataType::DT_INT, &status)) {
            SqlSevCon->GetFiledValue("OnlineID", DataType::DT_INT, &onlineID);
            switch (status) {
                case OS_PAY_SUCESS: //已经成功充值
                    printf("Pay OrderNo is existed, insert falied. OrderStatus=%d\n", status);
                    return true;
                case OS_PAY_FAILED://充值失败
                    //更新订单状态
                    SqlSevCon->CloseRecord();
                    SqlSevCon->UpdateOrderState(onlineID, OS_PAY_SUCESS);
                    SqlSevCon->CloseRecord();
                    goto STEP_WX_OSQL_ADD_MONEY;
                    break;
            }
        }
        SqlSevCon->CloseRecord();

        int amount = atoi(wxRet.total_fee.c_str());
        SqlSevCon->InsertOrder(userID, Order, wxRet.transaction_id, amount, OS_PAY_SUCESS);
        SqlSevCon->CloseRecord();
    }

    //4.加钱////////////////////////////////////////////////////////////////////////

STEP_WX_OSQL_ADD_MONEY:

    {
		if (strcmp(owntype.c_str(), "room_card") == 0) {
			SqlSevCon->UpdateUserMoney(userID, amount, PT_RoomCard);
		} else if (strcmp(owntype.c_str(), "gold") == 0) {
			SqlSevCon->UpdateUserMoney(userID, amount, PT_Gold);
		}

        SqlSevCon->CloseRecord();

        printf("Player %s add money %d sucess.\n", userID.c_str(), amount);

        SqlSevCon->Close();

        //////////////////////////////////////////////////////////////////////////
        //通知游戏服务器

        DWORD dwSocketID = atoi(servSocketID.c_str());
        pConect con = tcp_connection::getConection(dwSocketID);
        assert(con);
        if (!con) {
            printf("A http request failed : Can't match game server dwSocketID: %s.\n", servSocketID.c_str());
            return true;
        }

        SUB_PM_PAY_SUC pay_suc;

        memset(&pay_suc, 0, sizeof(pay_suc));
        pay_suc.wServerID = dwSocketID;
		pay_suc.dwUserID = atoi(userID.c_str());
        pay_suc.dwUserSocketID = atoi(userIDSocketID.c_str());
        pay_suc.wTableID = atoi(tableID.c_str());

        TCP_Command cmd;
        cmd.wMainCmdID = MDM_PM_BESINESS;
        cmd.wSubCmdID = SUB_PM_PAY_SUCCESS;

        con->sendData(cmd, &pay_suc, sizeof(pay_suc));
    }

    return true;
}

bool business::WeiXin::onHandSuccess(const request& req, reply& rep)
{
    std::string content_type = "text/html";
	std::string content_value = "<xml>\
												<return_code><![CDATA[SUCCESS]]></return_code>\
												<return_msg><![CDATA[OK]]></return_msg>\
												</xml>";

    rep.content.append(content_value.c_str());

    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = content_type;

    rep.status = rep.ok;

    return  true;
}

bool WeiXin::onHandFailed(const request& req, reply& rep)
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

bool WeiXin::onMessage(pConect con, TCP_Command& tcp_cmd, void * pBuffer, WORD wDataSize)
{

    assert(false);
    return false;

}
