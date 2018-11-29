#include "stdafx.h"
#include "TinyServerThread.hpp"

#ifdef UNIT_TEST

#include "TestMain.hpp"

#endif // UNIT_TEST

#include "Configration/PayConfigReader.h"

struct WeiXinPayResult
{

    std::string appid;
    std::string attach;//optional
    std::string bank_type;
    std::string cash_fee;
    std::string fee_type;//optional
    std::string is_subscribe;//optional
    std::string mch_id;
    std::string nonce_str;
    std::string openid;
    std::string out_trade_no;
    std::string result_code;
    std::string return_code;
    std::string sign;
    std::string time_end;
    std::string total_fee;
    std::string trade_type;
    std::string transaction_id;

    std::string err_code;//optional
    std::string return_msg;//optional

};

int main(int argc, char* argv[])
{

#if !defined(UNIT_TEST)

    try
    {

        //ÇÐ»»cmdµ½utf8
        //system("CHCP 65001");

        payment::TinyServerThread::thread_run(argc, argv);

    }catch (std::exception& e){

        std::cerr << e.what() << std::endl;

    }

#else

    return unit_test::testmain(argc, argv);

#endif // UNIT_TEST

    system("pause");

    return 0;

}
