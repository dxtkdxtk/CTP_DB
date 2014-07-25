#include "MdUserApi.h"
#include <iostream>
#include "toolkit.h"
#include "CLock.h"
using namespace std;
#define _CTP_SEPS_ ",;"
MdUserApi::MdUserApi(void)
{
    m_pMdUserApi = NULL;
    m_iRequestID = 0;
    m_msgQueue = NULL;
    m_status = E_uninit;
    InitializeCriticalSection(&m_csMapInstrumentIDs);

}

MdUserApi::~MdUserApi(void)
{
    Disconnect();
    DeleteCriticalSection(&m_csMapInstrumentIDs);
}

void MdUserApi::RegisterMsgQueue(CTPMsgQueue* pMsgQueue)
{
    m_msgQueue = pMsgQueue;
}

bool MdUserApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bRet)
    {
        if (m_msgQueue)
            m_msgQueue->Input_OnRspError(this, pRspInfo, nRequestID, bIsLast);
    }
    return bRet;
}
bool MdUserApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
    bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bRet)
    {
        //cout << "错误ID: " << pRspInfo->ErrorID << "  错误信息： " << pRspInfo->ErrorMsg << endl;
    }
    return bRet;
}

void MdUserApi::Connect(const string& strPath,
    const string& strAddresses,
    const string& strBrokerId,
    const string& strInvestorId,
    const string& strPassword)
{
    m_strBrokerId = strBrokerId;
    m_strInvestorId = strInvestorId;
    m_strPassword = strPassword;
    char *pPath = new char[strPath.size() + 20];
    sprintf(pPath, "%s\\Md\\", strPath.c_str());
    makedirs(pPath);
    m_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi(pPath, (strAddresses.find("udp://") != strAddresses.npos));
    delete[] pPath;

    m_status = E_inited;
    if (m_msgQueue)
        m_msgQueue->Input_OnConnect(this, NULL, m_status);
    if (m_pMdUserApi)
    {
        m_pMdUserApi->RegisterSpi(this);

        size_t len = strAddresses.length() + 1;
        char* buf = new char[len];
        strncpy(buf, strAddresses.c_str(), len);
        

        char *token = strtok(buf, _CTP_SEPS_);

        while (token)
        {
            if (strlen(token) > 0)
            {
                char *pch = strstr(token, "udp://");
                if (pch)
                {
                    strncpy(pch, "tcp://", 6);
                }
               m_pMdUserApi->RegisterFront(token);
            }
            token = strtok(NULL, _CTP_SEPS_);
        }
        delete[] buf;

        m_pMdUserApi->Init();
        m_status = E_connecting;
        if (m_msgQueue)
            m_msgQueue->Input_OnConnect(this, NULL, m_status);
    }//end 
}

void MdUserApi::ReqUserLogin()
{
    if (NULL == m_pMdUserApi)
        return;
    CThostFtdcReqUserLoginField request = { 0 };

    strncpy(request.BrokerID, m_strBrokerId.c_str(), sizeof(TThostFtdcBrokerIDType));
    strncpy(request.UserID, m_strInvestorId.c_str(), sizeof(TThostFtdcInvestorIDType));
    strncpy(request.Password, m_strPassword.c_str(), sizeof(TThostFtdcPasswordType));

    //只有这一处用到了m_iRequestID，没有必要每次重连m_iRequestID都从0开始
    m_pMdUserApi->ReqUserLogin(&request, ++m_iRequestID);
    m_status = E_logining;
    if (m_msgQueue)
        m_msgQueue->Input_OnConnect(this, NULL, m_status);
    
}

void MdUserApi::Disconnect()
{
    m_status = E_unconnected;
    if (m_pMdUserApi)
    {
        m_pMdUserApi->RegisterSpi(NULL);
        m_pMdUserApi->Release();
        m_pMdUserApi = NULL;
        if (m_msgQueue)
            m_msgQueue->Input_OnDisconnect(this, NULL, m_status);
    }
}

void MdUserApi::Subscribe(const string& szInstrumentIDs)
{
    if (NULL == m_pMdUserApi)
        return;

    vector<char*> vct;

    size_t len = szInstrumentIDs.length() + 1;
    char* buf = new char[len];
    strncpy(buf, szInstrumentIDs.c_str(), len);

    CLock cl(&m_csMapInstrumentIDs);

    char* token = strtok(buf, _CTP_SEPS_);
    while (token)
    {
        size_t l = strlen(token);
        if (l>0)
        {
            //合约已经存在，不用再订阅，但多次订阅也没关系
            m_setInstrumentIDs.insert(token);//记录此合约进行订阅
            vct.push_back(token);
        }
        token = strtok(NULL, _CTP_SEPS_);
    }

    if (vct.size()>0)
    {
        //转成字符串数组
        char** pArray = new char*[vct.size()];
        for (size_t j = 0; j<vct.size(); ++j)
        {
            pArray[j] = vct[j];
        }

        //订阅
        m_pMdUserApi->SubscribeMarketData(pArray, (int)vct.size());

        delete[] pArray;
    }

    //释放内存
    delete[] buf;
}

void MdUserApi::Subscribe(const set<string> &setInstrumentIDs)
{
    if (NULL == m_pMdUserApi)
        return;

    string szInstrumentIDs;
    for (set<string>::iterator i = setInstrumentIDs.begin(); i != setInstrumentIDs.end(); ++i)
    {
        szInstrumentIDs.append(*i);
        szInstrumentIDs.append(";");
    }

    if (szInstrumentIDs.length()>1)
    {
        Subscribe(szInstrumentIDs);
    }
}

void MdUserApi::Unsubscribe(const string& strInstrumentIDs)
{
    if (NULL == m_pMdUserApi)
        return;

    vector<char*> vct;
    size_t len = strInstrumentIDs.length() + 1;
    char* buf = new char[len];
    strncpy(buf, strInstrumentIDs.c_str(), len);

    CLock cl(&m_csMapInstrumentIDs);

    char* token = strtok(buf, _CTP_SEPS_);
    while (token)
    {
        size_t l = strlen(token);
        if (l>0)
        {
            //合约已经不存在，不用再取消订阅，但多次取消也没什么关系
            m_setInstrumentIDs.erase(token);
            vct.push_back(token);
        }
        token = strtok(NULL, _CTP_SEPS_);
    }

    if (vct.size()>0)
    {
        //转成字符串数组
        char** pArray = new char*[vct.size()];
        for (size_t j = 0; j<vct.size(); ++j)
        {
            pArray[j] = vct[j];
        }

        //订阅
        m_pMdUserApi->UnSubscribeMarketData(pArray, (int)vct.size());

        delete[] pArray;
    }

    //释放内存
    delete[] buf;
}

void MdUserApi::OnFrontConnected()
{
    m_status = E_connected;
    if (m_msgQueue)
        m_msgQueue->Input_OnConnect(this, NULL, m_status);
    //连接成功后自动请求登录
    ReqUserLogin();
}

void MdUserApi::OnFrontDisconnected(int nReason)
{
    m_status = E_unconnected;
    CThostFtdcRspInfoField RspInfo;
    //连接失败返回的信息是拼接而成，主要是为了统一输出
    RspInfo.ErrorID = nReason;
    GetOnFrontDisconnectedMsg(&RspInfo);
    if (m_msgQueue)
        m_msgQueue->Input_OnDisconnect(this, &RspInfo, m_status);
}

void MdUserApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (!IsErrorRspInfo(pRspInfo)
        && pRspUserLogin)
    {
        m_status = E_logined;
        if (m_msgQueue)
            m_msgQueue->Input_OnConnect(this, pRspUserLogin, m_status);
        //有可能断线了，本处是断线重连后重新订阅
        set<string> mapOld = m_setInstrumentIDs;//记下上次订阅的合约
        //Unsubscribe(mapOld);//由于已经断线了，没有必要再取消订阅
        Subscribe(mapOld);//订阅
    }
    else
    {
        m_status = E_authed;
        if (m_msgQueue)
            m_msgQueue->Input_OnDisconnect(this, pRspInfo, E_logining);
        //cout << "登陆行情端成功" << endl;
    }
}


void MdUserApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    //在模拟平台可能这个函数不会触发，所以要自己维护一张已经订阅的合约列表
    if (!IsErrorRspInfo(pRspInfo)
        && pSpecificInstrument)
    {
        CLock cl(&m_csMapInstrumentIDs);

        m_setInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
    }
}

void MdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    //模拟平台可能这个函数不会触发
    if (!IsErrorRspInfo(pRspInfo)
        && pSpecificInstrument)
    {
        CLock cl(&m_csMapInstrumentIDs);

        m_setInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
    }
}

//行情回调，得保证此函数尽快返回
void MdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    if (m_msgQueue)
        m_msgQueue->Input_OnRtnDepthMarketData(this, pDepthMarketData);
}


void MdUserApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (m_msgQueue)
        m_msgQueue->Input_OnRspError(this, pRspInfo, nRequestID, bIsLast);
}
