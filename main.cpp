#include "mainConnection.h"

string database = "MarketData.testtick";

int main()
{
    while (!connectMongo("localhost"));
    while (!connectCTP("realServer"));
    
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
    
    return 0;
}
