#include "mainConnection.h"

string database = "MarketData.tick";

int main()
{
    while (!connectMongo("localhost"));
    while (!connectCTP("simServer"));
    
    cout << "���ݿ�д����" << endl;
    string s;
    while (1)
    {
        cin >> s;
        if (s == "exit")
        {
            cout << "���ݿ�д�����" << endl;
            break;
        }
    }
    
    return 0;
}
