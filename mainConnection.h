
#include <iostream>
#include "Connection.h"
#include "mongo/client/dbclient.h"
#include "traderapi\ThostFtdcMdApi.h"

using namespace std;

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
        cout << "�������ݿ�ɹ�" << endl;
    }
    catch (const mongo::DBException &e)
    {
        cout << "caught " << e.what() << endl;
    }
}

void connectCTP()
{
    con = new Connection();
    if (CheckIsConnect())
    {
        con->readInifile(".\\server.ini", "simServer");
        con->td->Connect(con->streamPath,
            con->tdServer,
            con->brokerId,
            con->investorId,
            con->password,
            THOST_TERT_RESTART, "", "");
        WaitForSingleObject(con->callbackSet->h_connected, 5000);
        if (con->callbackSet->bIsTdConnected == false)
        {
            delete con;
            con = NULL;
            cout << "���׶�����ʧ��\n" << endl;
        }
        else
        {
            cout << "���׶����ӳɹ�\n" << endl;
            con->md->Connect(con->streamPath,
                con->mdServer,
                con->brokerId,
                con->investorId,
                con->password);
            WaitForSingleObject(con->callbackSet->h_connected, 5000);
            if (con->callbackSet->bIsMdConnected == false)
            {
                delete con;
                con = NULL;
                cout << "���������ʧ��" << endl;
            }
            else
            {
                cout << "��������ӳɹ�\n" << endl;

                con->td->ReqQryInstrument("");
                Sleep(3000);
                cout << "��ȡ��Լ�ɹ�" << endl;
                con->md->Subscribe(con->callbackSet->strAllIns);
                cout << "���ĺ�Լ�ɹ�" << endl;
            }
        }
    }
    else
    {
        cerr << "�������Ӵ���" << endl;
    }
}

