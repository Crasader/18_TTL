#include "stdafx.h"

#include "PaymentWorker.h"
#include "Configration/PayConfigReader.h"
#include "DataBase/SqlServerConector.h"

using namespace payment;
using namespace osql;

PaymentWorker::PaymentWorker()
{

    auto& config = PayConfigReader::getConfig();

    if (!SqlSevCon) {

        SqlSevCon = new SQLServerConector(config.strDBAddr,
            config.strDBName,
            config.strDBPort,
            config.strDBUserName,
            config.strDBPassWord
        );
    }

    _products.readProducts(config.strProductFile);

}

PaymentWorker::~PaymentWorker()
{
    if (SqlSevCon) {
        delete SqlSevCon;
    }
}