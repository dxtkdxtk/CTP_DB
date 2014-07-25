/*****************************************************************************
File name: Connection.cpp
Description: Defined connection struct for operate CTP
Author: jebin
Date: 2014/07/10
History: see git log
*****************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "MdUserApi.h"
#include "TraderApi.h"

using namespace std;

//�������ӽṹ
class Connection
{

public:
    Connection()
    {

        msgQueue = new CTPMsgQueue();
        callbackSet = new FunctionCallBackSet();

        msgQueue->RegisterCallback(callbackSet);
        md = new MdUserApi();
        md->RegisterMsgQueue(msgQueue);
        td = new TraderApi();
        td->RegisterMsgQueue(msgQueue);
        msgQueue->StartThread();

    }
    ~Connection()
    {
        msgQueue->StopThread();
        msgQueue->Clear();
        td->Disconnect();
        md->Disconnect();

        delete td;
        delete md;
        delete callbackSet;
        delete msgQueue;
    }
    //��ȡ������Ϣ
    void readInifile(const char* file, const char *servername)
    {
        GetPrivateProfileStringA(servername, "path", "", tmp, sizeof(tmp), file);
        streamPath = tmp;
        GetPrivateProfileStringA(servername, "mdserver", "", tmp, sizeof(tmp), file);
        mdServer = tmp;
        GetPrivateProfileStringA(servername, "tdserver", "", tmp, sizeof(tmp), file);
        tdServer = tmp;
        GetPrivateProfileStringA(servername, "brokerid", "", tmp, sizeof(tmp), file);
        brokerId = tmp;
        GetPrivateProfileStringA(servername, "investorid", "", tmp, sizeof(tmp), file);
        investorId = tmp;
        GetPrivateProfileStringA(servername, "password", "", tmp, sizeof(tmp), file);
        password = tmp;
        GetPrivateProfileStringA(servername, "instrument", "", tmp, sizeof(tmp), file);
        instrumentId = tmp;

    }
    char tmp[105];
    CTPMsgQueue *msgQueue;//�ر���Ϣ����
    FunctionCallBackSet *callbackSet;//ʵʱ�ص���Ϣ
    MdUserApi *md;//�������ӿ�
    TraderApi *td;//�������ӿ�

    string streamPath;//������·��
    string mdServer;//���������
    string tdServer;//���׷�����
    string brokerId;//����ID
    string investorId;//Ͷ����ID
    string password;//Ͷ��������
    string instrumentId;//��Լ��


};

#endif

