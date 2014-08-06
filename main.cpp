#include "mainConnection.h"

int main(int argc, char *argv[])
{
    //调用参数配置
    bool ishelp = false;
    if (argc > 1)
    {
        for (int i = 1; i < argc;)
        {
            if (strcmp(argv[i], "--real") == 0)
            {
                server = "realServer";
                ++i;
            }
            else if (strcmp(argv[i], "--ip") == 0)
            {
                if (argc < i + 2)
                {
                    cout << "parameter error" << endl;
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
                    cout << "invalid help call!" << endl;
                    return -1;
                }
            }
            else if (strcmp(argv[i], "--inipath") == 0)
            {
                if (argc < i + 2)
                {
                    cout << "parameter error" << endl;
                    return -1;
                }
                inipath = argv[i + 1];
                i += 2;
            }
            else
            {
                cout << "no matched option!" << endl;
                return -1;
            }
        }

    }
    if (!ishelp)
    {
        while (!connectMongo(ip));
        while (!connectCTP(inipath, server.c_str()));
        cout << "数据库写入中" << endl;
        string s;
        while (1)
        {
            cin >> s;
            if (s == "exit")
            {
                cout << "数据库写入结束" << endl;
                break;
            }
        }
    }
    return 0;
}
