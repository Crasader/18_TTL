#include "stdafx.h"
#include "SqlServerConector.h"

using namespace osql;

osql::SQLServerConector::SQLServerConector(const std::string& strDataBaseAddr,
    const std::string& strDataBaseName,
    const std::string& strDataBasePort,
    const std::string& strUserName,
    const std::string& strPassword)
{

    _strDBAdress = strDataBaseAddr;
    _strDBName = strDataBaseName;
    _strDBPort = strDataBasePort;
    _strUserName = strUserName;
    _strPassword = strPassword;

    char conection[256] = { 0 };

    sprintf(conection,
        //"Driver={sql server};server=%s,%s;uid=%s;pwd=%s;database=%s;", 
        //"Provider=SQLOLEDB;uid=%s;DataBase=%s;Server=%s,%s;psd=%s;",
        "Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%s;Password=%s;",
        _strUserName.c_str(),
        _strDBName.c_str(),
        _strDBAdress.c_str(),
        _strDBPort.c_str(),
        _strPassword.c_str());

    _strConection = conection;

    memset(sql_cmd, 0, sizeof(sql_cmd));

}

osql::SQLServerConector::~SQLServerConector()
{

    if (_pRecordset)
        _pRecordset->Release();
    if(_sqlCon)
        _sqlCon->Release();

}

bool SQLServerConector::Open()
{

    try {

        CoInitialize(nullptr);

        HRESULT hr = _sqlCon.CreateInstance(_uuidof(Connection));

        if (FAILED(hr)) {

            throw std::logic_error("数据库对象初始化失败");
            std::cout << "数据库对象初始化失败" << std::endl;

        }

        hr = _pRecordset.CreateInstance(_uuidof(Recordset));
        if (FAILED(hr)) {

            std::cout << "记录集对象指针实例化失败！" << std::endl;

        }
        _sqlCon->Open(_strConection.c_str(), "", "", adConnectUnspecified);

    } catch (_com_error &e) {

        std::cout << e.Description() << std::endl;
        return false;

    }

    return true;

}

void osql::SQLServerConector::Close()
{

    try {

        if (_pRecordset->GetState() != adStateClosed)
            _pRecordset->Close();
        if(_sqlCon->GetState() != adStateClosed)
            _sqlCon->Close();

    }catch (_com_error &e) {

        std::cout << e.Description() << std::endl;

    }

}

void osql::SQLServerConector::CloseRecord()
{

    try {

        if(_pRecordset->GetState() != adStateClosed)
            _pRecordset->Close();

    } catch (_com_error &e) {

        std::cout << e.Description() << std::endl;

    }

}

bool osql::SQLServerConector::RequestOrder(std::string userID, std::string orderNo)
{

    memset(sql_cmd, 0, sizeof(sql_cmd));
    sprintf(sql_cmd, "select OrderStatus, OnlineID  from dbo.OnlineOrder where UserID=%s and OrderID=%s",
    userID.c_str(),
    orderNo.c_str());

    return Excute(sql_cmd, DBOperatType::DBO_READ);

}

bool osql::SQLServerConector::GetUserMoney(std::string userID)
{

    memset(sql_cmd, 0, sizeof(sql_cmd));
    sprintf(sql_cmd, "select Score from dbo.GameScoreInfo where UserID=%s", userID.c_str());

    return Excute(sql_cmd, DBOperatType::DBO_READ);

}

bool osql::SQLServerConector::UpdateOrderState(size_t onlineID, int state)
{

    memset(sql_cmd, 0, sizeof(sql_cmd));
    sprintf(sql_cmd, "update dbo.OnlineOrder set OrderStatus=%d where OnlineID=%d", state, onlineID);

    return Excute(sql_cmd, DBOperatType::DBO_WRITE);

}

bool osql::SQLServerConector::InsertOrder(std::string userID, std::string orderNo,std::string wxOrder, int amount, int state)
{

    int count = 0;
    memset(sql_cmd, 0, sizeof(sql_cmd));
    sprintf(sql_cmd, "select Count = count(OnlineID) from dbo.OnlineOrder");
    Excute(sql_cmd, DBOperatType::DBO_READ);
    GetFiledValue("Count", DataType::DT_INT, &count);
    CloseRecord();

    memset(sql_cmd, 0, sizeof(sql_cmd));
    sprintf(sql_cmd, "SET IDENTITY_INSERT dbo.OnlineOrder ON\n\
	                            insert into dbo.OnlineOrder(UserID,OrderID,OrderAmount,OrderStatus,OnlineID,OperUserID,ShareID,GameID,Accounts,CardTypeID,CardPrice,CardGold,CardTotal,DiscountScale,PayAmount,IPAddress,ApplyDate)\
	                            values(%s, %s, %d, %d, %d, %s, 0, 0, %s, 0, 0 ,0, 0, 0, %d, 0, getdate())\
	                            SET IDENTITY_INSERT dbo.OnlineOrder OFF",
        userID.c_str(), wxOrder.c_str(), amount, state, count + 1, userID.c_str(), orderNo.c_str(), amount);

    return Excute(sql_cmd, DBOperatType::DBO_WRITE);

}

bool osql::SQLServerConector::UpdateUserMoney(std::string userID, int amount, int type)
{

    memset(sql_cmd, 0, sizeof(sql_cmd));
    LONGLONG total = 0;
	switch (type)
	{
	case PT_Gold:
		sprintf(sql_cmd, "select Score from dbo.GameScoreInfo where UserID=%s", userID.c_str());
		Excute(sql_cmd, DBOperatType::DBO_READ);
		GetFiledValue("Score", DataType::DT_LONGLONG, &total);
		break;
	case PT_RoomCard:
		sprintf(sql_cmd, "select InsureScore from dbo.GameScoreInfo where UserID=%s", userID.c_str());
		Excute(sql_cmd, DBOperatType::DBO_READ);
		GetFiledValue("InsureScore", DataType::DT_LONGLONG, &total);
		break;
	}
    CloseRecord();

	switch (type)
	{
	case PT_Gold:
		sprintf(sql_cmd, "update dbo.GameScoreInfo set Score=%lld where UserID=%s", total + amount, userID.c_str());
		break;
	case PT_RoomCard:
		sprintf(sql_cmd, "update dbo.GameScoreInfo set InsureScore=%lld where UserID=%s", total + amount, userID.c_str());
		break;
	}
    
    return Excute(sql_cmd, DBOperatType::DBO_WRITE);

}

bool osql::SQLServerConector::Excute(std::string strSqlCmd, DBOperatType opType)
{

    try {
        
        if ( (_pRecordset->GetState() & (adStateFetching | adStateExecuting ) )!= 0) {
            std::cout << "DBRequest failed : DBRecord don't close." << std::endl;
            return false;
        }

    }catch(_com_error &e) {

        std::cout << e.Description() << std::endl;
        return false;

    }

    try {
        
        switch (opType) {

        case DBO_READ:
            _pRecordset->Open(strSqlCmd.c_str(), (IDispatch*)_sqlCon, adOpenForwardOnly, adLockReadOnly, adCmdText);
            break;

        case DBO_WRITE:
        default:
            _pRecordset->Open(strSqlCmd.c_str(), (IDispatch*)_sqlCon, adOpenForwardOnly, adLockPessimistic, adCmdText);
            break;

        }

    } catch (_com_error &e) {

        std::cout << e.Description() << std::endl;
        return false;

    }

    return true;

}

long osql::SQLServerConector::ResultSize()
{

    try
    {
        if(_pRecordset->ADO_EOF)
            return 0;
        return _pRecordset->Fields->Count;

    } catch (_com_error &e) {

        std::cout << e.Description() << std::endl;
        return 0;

    }

}

bool osql::SQLServerConector::GetFiledValue(std::string fieldName, DataType dtType, void* _out_data)
{

    assert(_out_data != nullptr);

    if (ResultSize() <= 0) {
        return false;
    }

    if (_out_data == NULL) {
        throw std::logic_error("Get database field error, when null pointer error.");
        return false;
    }

    try {

        _variant_t vtFld = _pRecordset->Fields->GetItem(fieldName.c_str())->Value;

        switch (vtFld.vt) {

            case VT_NULL:
            case VT_EMPTY:
            {
                return false;
            }

            default:
            {

                switch (dtType) {
                case DT_BOOL:
                    *(static_cast<bool*>(_out_data)) = vtFld.boolVal;
                    break;
                case DT_INT:
                    *(static_cast<int*>(_out_data)) = (int)vtFld.iVal;
                    break;
                case DT_WORD:
                    *(static_cast<WORD*>(_out_data)) = (WORD)vtFld.ulVal;
                    break;
                case DT_LONG:
                    *(static_cast<WORD*>(_out_data)) = (WORD)vtFld.ulVal;
                    break;
                case DT_LONGLONG:
                    *(static_cast<ULONGLONG*>(_out_data)) = vtFld.ulVal;
                    break;
                case DT_DATE:
                    //windows typedef Date as double
                    *(static_cast<DATE*>(_out_data)) = vtFld.date;
                    break;
                case DT_STR:
                    lstrcpy(static_cast<char*>(_out_data), (char*)(_bstr_t(vtFld)));
                    break;
                }

            }

        }

    } catch (_com_error &e) {

        std::cout << e.Description() << std::endl;
        return false;

    }

    return true;
}
