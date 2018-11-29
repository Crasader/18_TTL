#ifndef _PAY_CONFIG_H_
#define _PAY_CONFIG_H_

#include "stdafx.h"

namespace payment {

struct PayConfig {
std:: string strSocektAddr;
std::string  strHttpPort;
std::string  strSocketPort;
std::string strHttpRoot;
std::string strDBAddr;
std::string strDBName;
std::string strDBPort;
std::string strDBUserName;
std::string strDBPassWord;
std::string strPlatformID;
std::string strAppID;
std::string strAppKey;
};
}

#endif