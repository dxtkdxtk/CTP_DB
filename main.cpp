#include "mainConnection.h"

int main(int argc, char *argv[])
{
    //���ò�������
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
                    cout << "��������" << endl;
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
                    cout << "�����help����" << endl;
                    return -1;
                }
            }
            else if (strcmp(argv[i], "--inipath") == 0)
            {
                if (argc < i + 2)
                {
                    cout << "��������" << endl;
                    return -1;
                }
                inipath = argv[i + 1];
                i += 2;
            }
            else if (strcmp(argv[i], "--logpath") == 0)
            {
                if (argc < i + 2)
                {
                    cout << "��������" << endl;
                    return -1;
                }
                logpath = argv[i + 1];
                i += 2;
            }
            else
            {
                cout << "û���ҵ�ָ������" << endl;
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
        sss<< "��ȡ��ǰ������: " << st.wYear*10000 + st.wMonth*100 + st.wDay;
        string tradingdaytxt = string(sss.str());
        PrintLog(filestream, tradingdaytxt.c_str());
        
        while (!connectMongo(ip));
        while (!connectCTP(inipath, server.c_str()));
        PrintLog(filestream, "���ݽ��տ�ʼ����");

        string s;
        while (1)
        {
            PrintLog(filestream, "���ͳ�������������������");
            Sleep(1000 * 900 - 5);
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
        filestream.close();
        PrintLog(filestream, "�����˳��ɹ�");
    }
    DeleteCriticalSection(&cs_fileWriting);
    return 0;
}
