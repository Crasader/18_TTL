#ifndef _TEST_MAIN_HPP_
#define _TEST_MAIN_HPP_

#if defined(UNIT_TEST)

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////

#define TCP_TEST
//#define HTTP_TEST
//#define SQL_SERVER_TEST
//#define WEIXIN_TEST

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

#ifdef SQL_SERVER_TEST

#include "DataBase/SqlServerConector.h"
#include "Configration/PayConfigReader.hpp"

#elif defined(TCP_TEST)

#include "Tcp/tcp_server.hpp"
#include "Tcp/data_deque.hpp"

using namespace tcp_con;
#endif

using namespace payment;

//////////////////////////////////////////////////////////////////////////






namespace unit_test {

int testmain(int argc, char* argv[])
{

    for (char a = 'A'; a < 'z'; a++) {
        printf("%d%c", a, a);
    }
    printf("\n%s\n", tool::randString(32).c_str());

#if defined(SQL_SERVER_TEST)

    using namespace osql;

    PayConfigReader::readConfig(std::string("payment.ini"));
    auto& config = PayConfigReader::getConfig();

    SQLServerConector SqlSevCon(config.strDBAddr,
        config.strDBName,
        config.strDBPort,
        config.strDBUserName,
        config.strDBPassWord
        );

    SqlSevCon.Open();

    SqlSevCon.Excute("select * from dbo.GameScoreInfo where UserID=3", DBOperatType::DBO_READ);

    double OriginationDate;
    SqlSevCon.GetFiledValue("LastLogonDate", DT_DATE, &OriginationDate);

#elif defined(TCP_TEST)

    Data_queue que;

    bool flag = true;
    for (;;)
    {
        printf("first isEmpty = %d, isFull = %d\n", que.isEmpty(), que.isFull());
        //printf("first end = %d\n", que.end_idx());

        if (flag) {
            try
            {
                que.push_back(100);
                que.push_back(100);

            }
            catch (std::exception e)
            {
                printf("error %s.\n", e.what()); ;
            }
            
        }
        else {
            que.pop_front();
        }

        if (que.isEmpty() || que.isFull()) {
            flag = !flag;
        }

        printf("secned isEmpty = %d, isFull = %d\n", que.isEmpty(), que.isFull());
        //printf("secned end = %d\n", que.end_idx());

        memset(&que.end().Head, 1, sizeof(que.end().Head));
    }

#elif defined(WEIXIN_TEST)

    std::string strPayResult = "\
        <xml>\
        <appid><![CDATA[wx464901e0c4443f38]]></appid>\
            <attach><![CDATA[abcd]]></attach>\
            <bank_type><![CDATA[CFT]]></bank_type>\
            <cash_fee><![CDATA[1]]></cash_fee>\
            <fee_type><![CDATA[CNY]]></fee_type>\
            <is_subscribe><![CDATA[N]]></is_subscribe>\
            <mch_id><![CDATA[1491170682]]></mch_id>\
            <nonce_str><![CDATA[EkTNyxdyoYvGXIh9]]></nonce_str>\
            <openid><![CDATA[oP8580g4KRWv_qGLTGgK-k3ho-Yk]]></openid>\
            <out_trade_no><![CDATA[1511762507_3194]]></out_trade_no>\
            <result_code><![CDATA[SUCCESS]]></result_code>\
            <return_code><![CDATA[SUCCESS]]></return_code>\
            <sign><![CDATA[AC3277613EA7DB41E07D6ABF4362E97C]]></sign>\
            <time_end><![CDATA[20171127140206]]></time_end>\
            <total_fee>1</total_fee >\
            <trade_type><![CDATA[APP]]></trade_type>\
            <transaction_id><![CDATA[4200000001201711277389009810]]></transaction_id>\
        </xml>";

    TiXmlDocument pay_result;
    TiXmlPrinter xmlPrinter;

    bool sucess = false;
    WeiXinPayResult wxRet;

    try {
        pay_result.Parse(strPayResult.c_str());
        auto* ch = pay_result.RootElement()->FirstChild();
        while (ch != nullptr) {
            auto* ele = ch->ToElement();
            if (strcmp(ele->Value(), "return_code") == 0) {//返回状态码
                wxRet.return_code = ele->GetText();
            }
            else if (strcmp(ele->Value(), "return_msg") == 0) {//返回信息
                wxRet.return_msg = ele->GetText();
            }
            else if (strcmp(ele->Value(), "appid") == 0) {//应用APPID
                wxRet.appid = ele->GetText();
            }
            else if (strcmp(ele->Value(), "attach") == 0) {//应用APPID
                wxRet.attach = ele->GetText();
            }
            else if (strcmp(ele->Value(), "mch_id") == 0) {//商户号
                wxRet.mch_id = ele->GetText();
            }
            else if (strcmp(ele->Value(), "nonce_str") == 0) {//随机字符串
                wxRet.nonce_str = ele->GetText();
            }
            else if (strcmp(ele->Value(), "sign") == 0) {//签名
                wxRet.sign = ele->GetText();
            }
            else if (strcmp(ele->Value(), "result_code") == 0) {//业务结果
                wxRet.result_code = ele->GetText();
            }
            else if (strcmp(ele->Value(), "err_code") == 0) {//错误代码
                wxRet.err_code = ele->GetText();
            }
            else if (strcmp(ele->Value(), "trade_type") == 0) {//交易类型
                wxRet.trade_type = ele->GetText();
            }
            else if (strcmp(ele->Value(), "openid") == 0) {//用户标识
                wxRet.openid = ele->GetText();
            }
            else if (strcmp(ele->Value(), "bank_type") == 0) {//付款银行
                wxRet.bank_type = ele->GetText();
            }
            else if (strcmp(ele->Value(), "total_fee") == 0) {//总金额
                wxRet.total_fee = ele->GetText();
            }
            else if (strcmp(ele->Value(), "cash_fee") == 0) {//现金支付金额
                wxRet.cash_fee = ele->GetText();
            }
            else if (strcmp(ele->Value(), "fee_type") == 0) {//现金支付金额
                wxRet.fee_type = ele->GetText();
            }
            else if (strcmp(ele->Value(), "is_subscribe") == 0) {//现金支付金额
                wxRet.is_subscribe = ele->GetText();
            }
            else if (strcmp(ele->Value(), "transaction_id") == 0) {//微信支付订单号
                wxRet.transaction_id = ele->GetText();
            }
            else if (strcmp(ele->Value(), "out_trade_no") == 0) {//商户订单号
                wxRet.out_trade_no = ele->GetText();
            }
            else if (strcmp(ele->Value(), "time_end") == 0) {//支付完成时间
                wxRet.time_end = ele->GetText();
            }
            ch = ch->NextSiblingElement();
        }
    }
    catch (...) {
        printf("onVerifyRequest : bad request with bad xml data.\n");
        return false;
    }

    //2.验证签名////////////////////////////////////////////////////////////////////////

    //签名通过
    bool signSucess = false;

    payment::PayConfigReader::readConfig("payment.ini");
    auto& config = payment::PayConfigReader::getConfig();

    if (wxRet.return_code != "SUCCESS") {//支付成功
        printf("onVerifyRequest : pay failed.\n");
        return false;
    }
    if (wxRet.appid != config.strAppID
        || wxRet.mch_id != config.strMchID) {
        printf("onVerifyRequest : weixin id can't match.\n");
        return false;
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

    printf("weixin genOrder sign str = %s\n", sign_str.c_str());

    MD5 md5 = MD5(sign_str);
    std::string strMd5 = boost::algorithm::to_upper_copy(md5.hexdigest());
    printf("weixin genOrder sign md5 = %s\n", strMd5.c_str());

    if (strMd5 == wxRet.sign) {
        signSucess = true;
    }

#endif

    return 0;

}

}

#endif //UNIT_TEST

#endif //_TEST_MAIN_HPP_