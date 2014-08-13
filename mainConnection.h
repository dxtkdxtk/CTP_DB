
#include "Connection.h"
#include "mongo/client/dbclient.h"
#include "traderapi\ThostFtdcMdApi.h"

#include <iostream>
#include <ctime>
#include <Windows.h>
#include <process.h>

using namespace mongo;

Connection *con;
mongo::DBClientConnection *mCon;
double INF = 1e+100;
SYSTEMTIME st;
string database = "MarketData.testtick";
string instdatabase = "MarketData.testinstrument";
string ip = "localhost";
string server = "simServer";
string inipath = "F:\\mongodb\\data\\server.ini";
string logpath = "F:\\mongodb\\data\\dblog\\";
CRITICAL_SECTION cs_fileWriting;
fstream filestream;
vector<BSONObj> v_marketObj;
CRITICAL_SECTION cs_market;
bool CheckIsConnect()
{
    return !(NULL == con);
}

//connect mongo db
bool connectMongo(const string &addr)
{
    try
    {
        mCon = new mongo::DBClientConnection();
        mCon->connect(addr);
        PrintLog(filestream, "�������ݿ�ɹ�");
    }
    catch (const mongo::DBException &e)
    {
        PrintLog(filestream, e.what());
        delete mCon;
        mCon = NULL;
        return false;
    }
    return true;
}

bool connectCTP(const string &inipath, const char *server)
{
    con = new Connection();
    if (CheckIsConnect())
    {
        con->readInifile(inipath.c_str(), server);
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
            PrintLog(filestream, "���ӽ��׶�ʧ�ܣ���������");
            return false;
        }
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
            PrintLog(filestream, "���������ʧ�ܣ���������");
            return false;
        }
        con->td->ReqQryInstrument("");
        Sleep(3000);
        PrintLog(filestream, "��ȡ��Լ��Ϣ�ɹ�");
        con->md->Subscribe(con->callbackSet->strAllIns);
        PrintLog(filestream, "���ĺ�Լ�ɹ�");
        return true;
    }
    else
    {
        PrintLog(filestream, "���ݿ����ӿռ俪��ʧ��");
        return false;
    }
}

//��ʾ�����ļ�
void showhelp()
{
    cout << "����: " << endl;
    cout << "    --help: ��ʾ����" << endl;
    cout << "    --real: ����realServer�ֶΣ���ʵ���˻�" << endl;
    cout << "    --ip [ip address]: ����ָ��ip" << endl;
    cout << "    --logpath [log path]: ָ����־·����ֻ��Ҫָ���ļ��У��ļ���ϵͳ�Զ�����" << endl;
    cout << "    --inipath [inifile path]: ָ���˻���Ϣini�ļ�·������Ҫָ������ini�ļ�" << endl;
}

//����������߳�
DWORD WINAPI MainThread(LPVOID pM)
{
    while (!connectMongo(ip));
    while (!connectCTP(inipath, server.c_str()));
    PrintLog(filestream, "���ݽ��տ�ʼ����");
    return 0;
}

//���������߳�
DWORD WINAPI HeartBeatThread(LPVOID pM)
{
    PrintLog(filestream, "���������߳�������");
    while (1)
    {
        Sleep(1000 * 900 - 3);
        PrintLog(filestream, "���ͳ�������������������");
        SYSTEMTIME hbt;
        GetLocalTime(&hbt);
        if (hbt.wHour == 15 && hbt.wMinute > 20)
        {
            PrintLog(filestream, "���������Ѿ������������˳�����");
            break;
        }
        else if (hbt.wHour == 2 && hbt.wMinute > 35)
        {
            PrintLog(filestream, "ҹ�������Ѿ������������˳�����");
            break;
        }

    }
    PrintLog(filestream, "�����˳��ɹ�");
    return 0;
}