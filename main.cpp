#include "mainConnection.h"
int main()
{
    connectMongo("localhost");
    connectCTP();
    string s;
    while (1)
    {
        cin >> s;
        if (s == "exit")
            break;
    }

    return 0;
}
