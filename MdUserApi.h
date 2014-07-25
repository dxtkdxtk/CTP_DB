#ifndef MDUSERAPI_H
#define MDUSERAPI_H
#include "traderapi\ThostFtdcMdApi.h"
#include "CTPStruct.h"
#include "CTPMsgQueue.h"
#include <set>
#include <string>
#include <WinSock2.h>
#include <Windows.h>
using namespace std;
class MdUserApi : public CThostFtdcMdSpi
{
public:
    MdUserApi(void);
    virtual ~MdUserApi(void);
    void Connect(const string& strPath,//��������ַ
        const string& strAddresses,//��������ַ
        const string& strBrokerId,//����ID
        const string& strInvestorId,//Ͷ����ID
        const string& strPassword);//Ͷ��������
    void Disconnect();
    void RegisterMsgQueue(CTPMsgQueue* pMsgQueue);//ע��ص���Ϣ����
    void Subscribe(const string &strInstrumentIDs);//���ĺ�Լ
    void Unsubscribe(const string &strInstrumentIDs);//

private:
    void Subscribe(const set<string> &setInstrumentIDs);
    void ReqUserLogin();
    
    virtual void OnFrontConnected();
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

    //����Ƿ����
    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//��������Ϣ�͵���Ϣ����
    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);//���ͳ�����Ϣ

private:

    CRITICAL_SECTION m_csMapInstrumentIDs;
    int m_iRequestID;//����ID
    set<string> m_setInstrumentIDs;//���ĺ�Լ
    set<string> m_setServerAddress;//��������ַȺ
    CThostFtdcMdApi *m_pMdUserApi;//����API
    string m_strDataPath;//������·��
    string m_strBrokerId; //����ID
    string m_strInvestorId;//Ͷ����ID
    string m_strPassword;//����

    CTPMsgQueue* m_msgQueue;// ��Ϣ����
    ConnectionStatus m_status;//����״̬


}; 
#endif

