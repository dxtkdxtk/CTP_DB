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
    void Connect(const string& strPath,//数据流地址
        const string& strAddresses,//服务器地址
        const string& strBrokerId,//期商ID
        const string& strInvestorId,//投资者ID
        const string& strPassword);//投资者密码
    void Disconnect();
    void RegisterMsgQueue(CTPMsgQueue* pMsgQueue);//注册回调信息队列
    void Subscribe(const string &strInstrumentIDs);//订阅合约
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

    //检查是否出错
    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//将出错消息送到消息队列
    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);//不送出错消息

private:

    CRITICAL_SECTION m_csMapInstrumentIDs;
    int m_iRequestID;//请求ID
    set<string> m_setInstrumentIDs;//订阅合约
    set<string> m_setServerAddress;//服务器地址群
    CThostFtdcMdApi *m_pMdUserApi;//行情API
    string m_strDataPath;//数据流路径
    string m_strBrokerId; //期商ID
    string m_strInvestorId;//投资者ID
    string m_strPassword;//密码

    CTPMsgQueue* m_msgQueue;// 消息队列
    ConnectionStatus m_status;//连接状态


}; 
#endif

