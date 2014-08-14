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

//是否为订阅第一根tick
#define ISFIRST 5

using namespace mongo;
using namespace bson;
using namespace std;

extern mongo::DBClientConnection *mCon;
extern Connection *con;
extern string database;
extern string instdatabase;
extern double INF;
extern SYSTEMTIME st;
extern string logpath;
extern fstream filestream;
extern CRITICAL_SECTION cs_market;
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


void __stdcall FunctionCallBackSet::OnRtnDepthMarketData(void* pMdUserApi, CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    static map<string, int> ok;
    if (ok[pDepthMarketData->InstrumentID] != ISFIRST)
    {
        ok[pDepthMarketData->InstrumentID] = ISFIRST;
        return;
    }
    BSONObjBuilder b;
    b.appendDate("UpdateTime", Date_t(GetEpochTime(st, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec)));
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
    mCon->insert(database, b.done());

}

void __stdcall FunctionCallBackSet::OnRspQryInstrument(void* pTraderApi, CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CLock cl(&v_csInstInfo);

    auto_ptr<DBClientCursor> cursor =
        mCon->query(instdatabase, QUERY("InstrumentID" << pInstrument->InstrumentID));
    if (cursor->itcount() == 0)
    {
        BSONObjBuilder b;
        b.append("InstrumentID", pInstrument->InstrumentID);
        b.append("ExchangeID", pInstrument->ExchangeID);
        b.append("InstrumentName", GBKToUTF8(pInstrument->InstrumentName));
        b.append("ExchangeInstID", pInstrument->ExchangeInstID);
        b.append("ProductID", pInstrument->ProductID);
        b.append("ProductClass", pInstrument->ProductClass);
        b.append("DeliveryYear", pInstrument->DeliveryYear);
        b.append("DeliveryMonth", pInstrument->DeliveryMonth);
        b.append("MaxMarketOrderVolume", pInstrument->MaxMarketOrderVolume);
        b.append("MinMarketOrderVolume", pInstrument->MinMarketOrderVolume);
        b.append("MaxLimitOrderVolume", pInstrument->MaxLimitOrderVolume);
        b.append("MinLimitOrderVolume", pInstrument->MinLimitOrderVolume);
        b.append("VolumeMultiple", pInstrument->VolumeMultiple);
        b.append("PriceTick", pInstrument->PriceTick);
        b.append("CreateDate", pInstrument->CreateDate);
        b.append("OpenDate", pInstrument->OpenDate);
        b.append("ExpireDate", pInstrument->ExpireDate);
        b.append("StartDelivDate", pInstrument->StartDelivDate);
        b.append("EndDelivDate", pInstrument->EndDelivDate);
        b.append("InstLifePhase", pInstrument->InstLifePhase);
        b.append("IsTrading", pInstrument->IsTrading);
        b.append("PositionType", pInstrument->PositionType);
        b.append("PositionDateType", pInstrument->PositionDateType);
        b.append("LongMarginRatio", pInstrument->LongMarginRatio);
        b.append("ShortMarginRatio", pInstrument->ShortMarginRatio);
        b.append("MaxMarginSideAlgorithm", pInstrument->MaxMarginSideAlgorithm);
        mCon->insert(instdatabase, b.obj());
    }



    strAllIns += pInstrument->InstrumentID;
    strAllIns += ';';
    v_instInfo.push_back(*pInstrument);
    if (v_instInfo.size() >= 100)
    {
        SetEvent(h_hasInst);
        bIsGetInst = true;
    }


}

void __stdcall FunctionCallBackSet::OnConnect(void* pApi, CThostFtdcRspUserLoginField *pRspUserLogin, ConnectionStatus result)
{
    string msg;
    if (pApi == con->td)
    {
        msg = "交易端状态->";
        PrintLog(filestream, (msg + ConnectionStatusMsg(result)).c_str());
        if (result == E_confirmed)
        {
            bIsTdConnected = true;
            SetEvent(h_connected);
            PrintLog(filestream, "交易端连接成功");
        }
    }
    else if (pApi == con->md)
    {
        msg = "行情端状态->";
        PrintLog(filestream, (msg + ConnectionStatusMsg(result)).c_str());
        if (result == E_logined)
        {
            bIsMdConnected = true;
            SetEvent(h_connected);
            PrintLog(filestream, "行情端连接成功");
        }
    }
}

void __stdcall FunctionCallBackSet::OnDisconnect(void* pApi, CThostFtdcRspInfoField *pRspInfo, ConnectionStatus step)
{
    string msg;
    if (pApi == con->md)
    {
        msg = "行情端失去连接，原因：";
        msg += pRspInfo->ErrorMsg;
        PrintLog(filestream, msg.c_str());
    }
        
    else if (pApi == con->td)
    {
        msg = "交易端失去连接，原因：";
        msg += pRspInfo->ErrorMsg;
        PrintLog(filestream, msg.c_str());
    }
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

void __stdcall FunctionCallBackSet::OnRspQryInstrumentCommissionRate(void* pTraderApi, CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryInstrumentMarginRate(void* pTraderApi, CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryInvestorPosition(void* pTraderApi, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryInvestorPositionDetail(void* pTraderApi, CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryOrder(void* pTraderApi, CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryTrade(void* pTraderApi, CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRspQryTradingAccount(void* pTraderApi, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void __stdcall FunctionCallBackSet::OnRtnInstrumentStatus(void* pTraderApi, CThostFtdcInstrumentStatusField *pInstrumentStatus)
{

}

void __stdcall FunctionCallBackSet::OnRtnOrder(void* pTraderApi, CThostFtdcOrderField *pOrder)
{

}

void __stdcall FunctionCallBackSet::OnRtnTrade(void* pTraderApi, CThostFtdcTradeField *pTrade)
{

}
