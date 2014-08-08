#include "mainConnection.h"

int main(int argc, char *argv[])
{
    //调用参数配置
    bool ishelp = false;
    InitializeCriticalSection(&cs_fileWriting);
    if (argc > 1)
    {
        for (int i = 1; i < argc;)
        {
            if (strcmp(argv[i], "--real") == 0)
            {
                server = "realServer";
                database = "MarketData.tick";
                instdatabase = "MarketData.instrument";
                ++i;
            }
            else if (strcmp(argv[i], "--ip") == 0)
            {
                if (argc < i + 2)
                {
                    cout << "参数错误" << endl;
                    return -1;
                }
                ip = argv[i + 1];
                i += 2;
            }
            else if (strcmp(argv[i], "--help") == 0)
            {
                if (i == 1)
                {
                    ishelp = true;
                    showhelp();
                    ++i;
                }
                else
                {
                    cout << "错误的help调用" << endl;
                    return -1;
                }
            }
            else if (strcmp(argv[i], "--inipath") == 0)
            {
                if (argc < i + 2)
                {
                    cout << "参数错误" << endl;
                    return -1;
                }
                inipath = argv[i + 1];
                i += 2;
            }
            else if (strcmp(argv[i], "--logpath") == 0)
            {
                if (argc < i + 2)
                {
                    cout << "参数错误" << endl;
                    return -1;
                }
                logpath = argv[i + 1];
                i += 2;
            }
            else
            {
                cout << "没有找到指定操作" << endl;
                return -1;
            }
        }

    }
    
    if (!ishelp)
    {
        GetLocalTime(&st);
        stringstream ss, sss;
        ss << st.wYear * 10000 + st.wMonth * 100 + st.wDay;
        logpath = logpath + string(ss.str()) + ".log"; 
        filestream.open(logpath, ios::app);
        sss<< "获取当前交易日: " << st.wYear*10000 + st.wMonth*100 + st.wDay;
        string tradingdaytxt = string(sss.str());
        PrintLog(filestream, tradingdaytxt.c_str());
        
        while (!connectMongo(ip));
        while (!connectCTP(inipath, server.c_str()));
        PrintLog(filestream, "数据接收开始运行");

        string s;
        while (1)
        {
            PrintLog(filestream, "发送程序心跳，正常运行中");
            Sleep(1000 * 900 - 5);
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
        filestream.close();
        PrintLog(filestream, "程序退出成功");
    }
    DeleteCriticalSection(&cs_fileWriting);
    return 0;
}
