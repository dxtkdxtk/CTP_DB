
#include <iostream>
#include "Connection.h"
#include "mongo/client/dbclient.h"
#include "traderapi\ThostFtdcMdApi.h"

using namespace mongo;

Connection *con;
mongo::DBClientConnection mCon; 


bool CheckIsConnect()
{
    return !(NULL == con);
}

//connect mongo db
void connectMongo(char *addr)
{
    try
    {
        mCon.connect(addr);
        cout << "连接数据库成功" << endl;
    }
    catch (const mongo::DBException &e)
    {
        cout << "caught " << e.what() << endl;
        assert(false);
    }
}

void connectCTP(char *server)
{
    con = new Connection();
    if (CheckIsConnect())
    {
        con->readInifile(".\\server.ini", server);
        do
        {
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
                con = new Connection();
            }
        } while (con->callbackSet->bIsTdConnected == false);
            cout << "交易端连接成功\n" << endl;
            do
            {
                con->md->Connect(con->streamPath,
                    con->mdServer,
                    con->brokerId,
                    con->investorId,
                    con->password);
                WaitForSingleObject(con->callbackSet->h_connected, 3000);
                if(con->callbackSet->bIsMdConnected == false)
                {
                    cout << "行情端连接失败" << endl;
                }
            }while(con->callbackSet->bIsMdConnected == false);
            
                cout << "行情端连接成功\n" << endl;

                con->td->ReqQryInstrument("");
                Sleep(3000);
                cout << "获取合约成功" << endl;
                con->md->Subscribe(con->callbackSet->strAllIns);
                cout << "订阅合约成功" << endl;
            
        }
    
}

