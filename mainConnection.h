
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
        PrintLog(filestream, "连接数据库成功");
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
            PrintLog(filestream, "连接交易端失败，正在重连");
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
            PrintLog(filestream, "连接行情端失败，正在重连");
            return false;
        }
        con->td->ReqQryInstrument("");
        Sleep(3000);
        PrintLog(filestream, "获取合约信息成功");
        con->md->Subscribe(con->callbackSet->strAllIns);
        PrintLog(filestream, "订阅合约成功");
        return true;
    }
    else
    {
        PrintLog(filestream, "数据库连接空间开辟失败");
        return false;
    }
}

//显示帮助文件
void showhelp()
{
    cout << "帮助: " << endl;
    cout << "    --help: 显示帮助" << endl;
    cout << "    --real: 连接realServer字段，即实盘账户" << endl;
    cout << "    --ip [ip address]: 连接指定ip" << endl;
    cout << "    --logpath [log path]: 指定日志路径，只需要指定文件夹，文件名系统自动设置" << endl;
    cout << "    --inipath [inifile path]: 指定账户信息ini文件路径，需要指定具体ini文件" << endl;
}

//主行情接收线程
DWORD WINAPI MainThread(LPVOID pM)
{
    while (!connectMongo(ip));
    while (!connectCTP(inipath, server.c_str()));
    PrintLog(filestream, "数据接收开始运行");
    return 0;
}

//程序心跳线程
DWORD WINAPI HeartBeatThread(LPVOID pM)
{
    PrintLog(filestream, "程序心跳线程已启动");
    while (1)
    {
        Sleep(1000 * 900 - 3);
        PrintLog(filestream, "发送程序心跳，正常运行中");
        SYSTEMTIME hbt;
        GetLocalTime(&hbt);
        if (hbt.wHour == 15 && hbt.wMinute > 20)
        {
            PrintLog(filestream, "日盘行情已经结束，启动退出程序");
            break;
        }
        else if (hbt.wHour == 2 && hbt.wMinute > 35)
        {
            PrintLog(filestream, "夜盘行情已经结束，启动退出程序");
            break;
        }

    }
    PrintLog(filestream, "程序退出成功");
    return 0;
}