#ifndef _PAY_CONFIG_READER_H_
#define _PAY_CONFIG_READER_H_

#include "PayConfig.h"

namespace payment {

class PayConfigReader
{

private:

    PayConfigReader() {};
    virtual ~PayConfigReader() {};

public:

    static void readConfig(const std::string& path);
    static const PayConfig& getConfig();

private:

    static PayConfig _config;
    static bool _bReadConfig;

};


}

#endif