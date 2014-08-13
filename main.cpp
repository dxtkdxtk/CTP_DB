#include "mainConnection.h"

int main(int argc, char *argv[])
{
    //���ò�������
    bool ishelp = false;
    InitializeCriticalSection(&cs_fileWriting);
    InitializeCriticalSection(&cs_market);
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
        v_marketObj.clear();
        stringstream ss, sss;
        ss << st.wYear * 10000 + st.wMonth * 100 + st.wDay;
        logpath = logpath + string(ss.str()) + ".log"; 
        filestream.open(logpath, ios::app);
        sss<< "��ȡ��ǰ������: " << st.wYear*10000 + st.wMonth*100 + st.wDay;
        string tradingdaytxt = string(sss.str());
        PrintLog(filestream, tradingdaytxt.c_str());
        
        void* h_main = CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
        void* h_heart = CreateThread(NULL, 0, HeartBeatThread, NULL, 0, NULL);
        WaitForSingleObject(h_heart, INFINITE);
        mCon->insert(database, v_marketObj);
        v_marketObj.clear();
        filestream.close();
        
    }
    DeleteCriticalSection(&cs_fileWriting);
    DeleteCriticalSection(&cs_market);
    return 0;
}
