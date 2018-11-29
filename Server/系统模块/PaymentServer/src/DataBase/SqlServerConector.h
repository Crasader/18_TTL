#ifndef _SQL_SERVER_CONECTOR_H_
#define _SQL_SERVER_CONECTOR_H_

#include <string.h>
#include <vector>

#import "msado15.dll" rename_namespace("adocg") rename("EOF","ADO_EOF")

using namespace adocg;

namespace osql {

enum DBOperatType {

    DBO_READ = 0,
    DBO_WRITE = 1,

};

enum DataType {

    DT_BOOL,
    DT_WORD,
    DT_INT,
    DT_LONG,
    DT_LONGLONG,
    DT_STR,
    DT_DATE,

};

class SQLServerConector{

public:

    SQLServerConector(const std::string& strDataBaseAddr,
        const std::string& strDataBaseName,
        const std::string& strDataBasePort,
        const std::string& strUserName,
        const std::string& strPassword);

    virtual ~SQLServerConector();

protected:

    SQLServerConector() {};
    SQLServerConector(SQLServerConector&) = delete;
    SQLServerConector& operator = (SQLServerConector&) = delete;

public:

    virtual bool Open();
    virtual void Close();

    //////////////////////////////////////////////////////////////////////////

    virtual bool Excute(std::string strSqlCmd, DBOperatType opType);
    virtual long ResultSize();
    virtual bool GetFiledValue(std::string filedName, DataType dtType, void* _out_data);
    virtual void CloseRecord();

    //////////////////////////////////////////////////////////////////////////

    virtual bool RequestOrder(std::string userID, std::string orderNo);
    virtual bool GetUserMoney(std::string userID);
    virtual bool UpdateOrderState(size_t onlineID, int state);
    virtual bool InsertOrder(std::string userID, std::string orderNo, std::string wxOrder, int amount, int state);
    virtual bool UpdateUserMoney(std::string userID, int amount, int type = 0);

private:

    _ConnectionPtr  _sqlCon;
    _RecordsetPtr _pRecordset;

    std::string _strDBAdress;
    std::string _strDBName;
    std::string _strDBPort;
    std::string _strUserName;
    std::string _strPassword;

    std::string _strConection;


    char sql_cmd[1024];

};

}

#endif