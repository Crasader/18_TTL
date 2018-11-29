#ifndef _PAY_CONFIG_H_
#define _PAY_CONFIG_H_

#include <string.h>

namespace payment {

struct PayConfig {

//////////////////////////////////////////////////////////////////////////

std:: string strSocektAddr;
std::string strHttpPort;
std::string strSocketPort;
std::string strHttpRoot;
std::string strDBAddr;
std::string strDBName;
std::string strDBPort;
std::string strDBUserName;
std::string strDBPassWord;
std::string strPlatformID;
std::string strProductFile;
std::string strNotifyUrl;
bool bUseWeixinPay;

//////////////////////////////////////////////////////////////////////////

std::string strAppID;
std::string strMchID;
std::string strSecretKey;

//////////////////////////////////////////////////////////////////////////

std::string strWeiXinOrder;
std::string strWeiXinOrderXml;

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

};

}

#endif