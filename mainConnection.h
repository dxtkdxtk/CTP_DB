
#include "Connection.h"
#include "mongo/client/dbclient.h"
#include "traderapi\ThostFtdcMdApi.h"

#include <iostream>
#include <ctime>

using namespace mongo;

Connection *con;
mongo::DBClientConnection *mCon;
double INF = 1e+100;

bool CheckIsConnect()
{
    return !(NULL == con);
}

//connect mongo db
bool connectMongo(char *addr)
{
    try
    {
        Sleep(5000);
        mCon = new mongo::DBClientConnection();
        mCon->connect(addr);
        cout << "连接数据库成功" << endl;
    }
    catch (const mongo::DBException &e)
    {
        cout << "caught " << e.what() << endl;
        delete mCon;
        mCon = NULL;
        return false;
    }
    return true;
}

bool connectCTP(char *server)
{
    con = new Connection();
    if (CheckIsConnect())
    {
        con->readInifile(".\\server.ini", server);
        con->td->Connect(con->streamPath,
            con->tdServer,
            con->brokerId,
            con->investorId,
            con->password,
            THOST_TERT_RESTART, "", "");
        WaitForSingleObject(con->callbackSet->h_connected, 3000);
        if (con->callbackSet->bIsTdConnected == false)
        {
            delete con;
            con = NULL;
            cout << "交易端连接失败\n" << endl;
            return false;
        }
        cout << "交易端连接成功\n" << endl;
        con->md->Connect(con->streamPath,
            con->mdServer,
            con->brokerId,
            con->investorId,
            con->password);
        WaitForSingleObject(con->callbackSet->h_connected, 3000);
        if (con->callbackSet->bIsMdConnected == false)
        {
            delete con;
            con = NULL;
            cout << "行情端连接失败" << endl;
            return false;
        }
        cout << "行情端连接成功\n" << endl;
        con->td->ReqQryInstrument("");
        Sleep(3000);
        cout << "获取合约成功" << endl;
        con->md->Subscribe(con->callbackSet->strAllIns);
        cout << "订阅合约成功" << endl;
        return true;
    }
    else
    {
        cout << "连接分配失败" << endl;
        return false;
    }
}

