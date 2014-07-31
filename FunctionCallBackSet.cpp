/*****************************************************************************
File name: FunctionCallBackSet.cpp
Description: Implement of FunctionCallBackSet.h
Author: jebin
Date: 2014/07/10
History: see git log
*****************************************************************************/

#include "FunctionCallBackSet.h"
#include "Connection.h"
#include "mongo/client/dbclient.h"

#include <iostream>
#include <string>

using namespace mongo;

extern mongo::DBClientConnection *mCon;
extern Connection *con;
extern string database;
extern double INF;

//是否已获取合约
bool FunctionCallBackSet::bIsGetInst;
bool FunctionCallBackSet::bIsTdConnected;
bool FunctionCallBackSet::bIsMdConnected;

//是否连接CTP
void* FunctionCallBackSet::h_connected;
void* FunctionCallBackSet::h_hasInst;


//当日参与交易合约信息
CRITICAL_SECTION FunctionCallBackSet::v_csInstInfo;
vector<CThostFtdcInstrumentField> FunctionCallBackSet::v_instInfo;
string FunctionCallBackSet::strAllIns;

//合约行情信息
CRITICAL_SECTION FunctionCallBackSet::m_csMarketData;
map<string, CThostFtdcDepthMarketDataField> FunctionCallBackSet::m_marketData;

//有效报单信息
CRITICAL_SECTION FunctionCallBackSet::m_csOrders;
map<pair<int, pair<int, string> >, CThostFtdcOrderField> FunctionCallBackSet::m_orders;

//持仓信息
CRITICAL_SECTION FunctionCallBackSet::m_csPosition;
map<pair<string, char>, CThostFtdcInvestorPositionField> FunctionCallBackSet::m_position;

//错误信息
CRITICAL_SECTION FunctionCallBackSet::v_csErrorInfo;
vector<string> FunctionCallBackSet::v_errorInfo;

void __stdcall FunctionCallBackSet::OnRtnDepthMarketData(void* pMdUserApi, CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    BSONObjBuilder b;
    b.appendDate("UpdateTime", Date_t(GetEpochTime(con->td->m_RspUserLogin.TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec)));
    b.append("InstrumentID", pDepthMarketData->InstrumentID);
    b.append("OpenPrice", pDepthMarketData->OpenPrice > INF ? -1 : pDepthMarketData->OpenPrice);
    b.append("HighestPrice", pDepthMarketData->HighestPrice > INF ? -1 : pDepthMarketData->HighestPrice);
    b.append("LowestPrice", pDepthMarketData->LowestPrice > INF ? -1 : pDepthMarketData->LowestPrice);
    b.append("ClosePrice", pDepthMarketData->ClosePrice > INF ? -1 : pDepthMarketData->ClosePrice);
    b.append("LastPrice", pDepthMarketData->LastPrice > INF ? -1 : pDepthMarketData->LastPrice);
    b.append("Volume", pDepthMarketData->Volume);
    b.append("AskPrice1", pDepthMarketData->AskPrice1 > INF ? -1 : pDepthMarketData->AskPrice1);
    b.append("BidPrice1", pDepthMarketData->BidPrice1 > INF ? -1 : pDepthMarketData->BidPrice1);
    b.append("AskVolume1", pDepthMarketData->AskVolume1);
    b.append("BidVolume1", pDepthMarketData->BidVolume1);
    b.append("Turnover", pDepthMarketData->Turnover > INF ? -1 : pDepthMarketData->Turnover);
    b.append("UpperLimitPrice", pDepthMarketData->UpperLimitPrice > INF ? -1 : pDepthMarketData->UpperLimitPrice);
    b.append("LowerLimitPrice", pDepthMarketData->LowerLimitPrice > INF ? -1 : pDepthMarketData->LowerLimitPrice);
    b.append("AveragePrice", pDepthMarketData->AveragePrice > INF ? -1 : pDepthMarketData->AveragePrice);
    b.append("PreSettlementPrice", pDepthMarketData->PreSettlementPrice > INF ? -1 : pDepthMarketData->PreSettlementPrice);
    b.append("PreClosePrice", pDepthMarketData->PreClosePrice > INF ? -1 : pDepthMarketData->PreClosePrice);
    b.append("PreOpenInterest", pDepthMarketData->PreOpenInterest > INF ? -1 : pDepthMarketData->PreOpenInterest);
    b.append("OpenInterest", pDepthMarketData->OpenInterest > INF ? -1 : pDepthMarketData->OpenInterest);
    b.append("SettlementPrice", pDepthMarketData->SettlementPrice > INF ? -1 : pDepthMarketData->SettlementPrice);
    b.append("ExchangeID", pDepthMarketData->ExchangeID);
    b.append("ExchangeInstID", pDepthMarketData->ExchangeInstID);
    mCon->insert(database, b.obj());
    cout << pDepthMarketData->InstrumentID << "-----" << pDepthMarketData->UpdateTime;
    cout << "-----" << pDepthMarketData->TradingDay << endl;
}

void __stdcall FunctionCallBackSet::OnConnect(void* pApi, CThostFtdcRspUserLoginField *pRspUserLogin, ConnectionStatus result)
{
    static bool isTd = false;
    if (result == E_confirmed)
    {
        bIsTdConnected = true;
        SetEvent(h_connected);
        isTd = true;
    }
    else if (isTd && result == E_logined)
    {
        bIsMdConnected = true;
        SetEvent(h_connected);
    }
}

void __stdcall FunctionCallBackSet::OnDisconnect(void* pApi, CThostFtdcRspInfoField *pRspInfo, ConnectionStatus step)
{

}

void __stdcall FunctionCallBackSet::OnErrRtnOrderAction(void* pTraderApi, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{

}

void __stdcall FunctionCallBackSet::OnErrRtnOrderInsert(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{

}

void __stdcall FunctionCallBackSet::OnRspError(void* pApi, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspOrderAction(void* pTraderApi, CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspOrderInsert(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryDepthMarketData(void* pTraderApi, CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    
}

void __stdcall FunctionCallBackSet::OnRspQryInstrument(void* pTraderApi, CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CLock cl(&v_csInstInfo);


    strAllIns += pInstrument->InstrumentID;
    strAllIns += ';';
    v_instInfo.push_back(*pInstrument);
    if (v_instInfo.size() >= 100)
    {
        SetEvent(h_hasInst);
        bIsGetInst = true;
    }

}

void __stdcall FunctionCallBackSet::OnRspQryInstrumentCommissionRate(void* pTraderApi, CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryInstrumentMarginRate(void* pTraderApi, CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryInvestorPosition(void* pTraderApi, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    /*CLock cl(&m_csPosition);
    if (pInvestorPosition->Position == 0)
    {
        m_position.erase(make_pair(pInvestorPosition->InstrumentID, pInvestorPosition->PosiDirection));
    }
    else
    {
        m_position[make_pair(pInvestorPosition->InstrumentID, pInvestorPosition->PosiDirection)] = *pInvestorPosition;
    }*/
}

void __stdcall FunctionCallBackSet::OnRspQryInvestorPositionDetail(void* pTraderApi, CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryOrder(void* pTraderApi, CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    /*CLock cl(&v_csErrorInfo);
    if (pRspInfo->ErrorID == 0)
    {
        v_errorInfo.push_back(string("[OnRspQryOrder]: ") + string("查询委托单成功!"));
    }
    else
    {
        v_errorInfo.push_back(string("[OnRspQryOrder]: ") + string(pRspInfo->ErrorMsg));
    }*/
}

void __stdcall FunctionCallBackSet::OnRspQryTrade(void* pTraderApi, CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    /*CLock cl(&v_csErrorInfo);
    if (pRspInfo->ErrorID == 0)
    {
        v_errorInfo.push_back(string("[OnRspQryTrade]: ") + string("查询成交成功!"));
    }
    else
    {
        v_errorInfo.push_back(string("[OnRspQryTrade]: ") + string(pRspInfo->ErrorMsg));
    }*/
}

void __stdcall FunctionCallBackSet::OnRspQryTradingAccount(void* pTraderApi, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    /*CLock cl(&v_csErrorInfo);
    if (pRspInfo->ErrorID == 0)
    {
        v_errorInfo.push_back(string("[OnRspQryTradingAccount]: ") + string("查询交易账户成功!"));
    }
    else
    {
        v_errorInfo.push_back(string("[OnRspQryTradingAccount]: ") + string(pRspInfo->ErrorMsg));
    }*/
}

void __stdcall FunctionCallBackSet::OnRtnInstrumentStatus(void* pTraderApi, CThostFtdcInstrumentStatusField *pInstrumentStatus)
{

}

void __stdcall FunctionCallBackSet::OnRtnOrder(void* pTraderApi, CThostFtdcOrderField *pOrder)
{
    /*CLock cl(&m_csOrders);
    pair<int, pair<int, string> > ref = make_pair(pOrder->FrontID, make_pair(pOrder->SessionID, pOrder->OrderRef));
    m_orders[ref] = *pOrder;*/
}

void __stdcall FunctionCallBackSet::OnRtnTrade(void* pTraderApi, CThostFtdcTradeField *pTrade)
{

}
