
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
        cout << "�������ݿ�ɹ�" << endl;
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
            cout << "���׶�����ʧ��\n" << endl;
            return false;
        }
        cout << "���׶����ӳɹ�\n" << endl;
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
            cout << "���������ʧ��" << endl;
            return false;
        }
        cout << "��������ӳɹ�\n" << endl;
        con->td->ReqQryInstrument("");
        Sleep(3000);
        cout << "��ȡ��Լ�ɹ�" << endl;
        con->md->Subscribe(con->callbackSet->strAllIns);
        cout << "���ĺ�Լ�ɹ�" << endl;
        return true;
    }
    else
    {
        cout << "���ӷ���ʧ��" << endl;
        return false;
    }
}

