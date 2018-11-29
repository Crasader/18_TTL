#ifndef _PAY_CONFIG_READER_H_
#define _PAY_CONFIG_READER_H_

#include "stdafx.h"
#include "PayConfig.hpp"

namespace payment {

class PayConfigReader
{

private:

    PayConfigReader() {};
    virtual ~PayConfigReader() {};

public:

    static void readConfig(std::string path);
    static PayConfig& getConfig();

private:

    static PayConfig _config;
    static bool _bReadConfig;

};


}

#endif