/*****************************************************************************
File name: FunctionCallBackSet.h
Description: Defined a set of callback functions for the CTP callback &
*defined some struct to store the message of callback.
Author: jebin
Date: 2014/07/10
History: see git log
*****************************************************************************/

#ifndef FUNCTION_CALL_BACK_H
#define FUNCTION_CALL_BACK_H

#include "CTPStruct.h"
#include "CLock.h"

#include "traderapi\ThostFtdcUserApiStruct.h"

#include <WinSock2.h>
#include <list>
#include <Windows.h>
#include <map>
#include <vector>
#include <set>
#include <utility>
using namespace std;
class FunctionCallBackSet
{


public:
    //是否已获取合约
    static bool bIsGetInst;
    static bool bIsTdConnected;
    static bool bIsMdConnected;


    //是否连接事件
    static HANDLE h_connected;
    static HANDLE h_hasInst;


    //当日交易所有合约基本信息
    static CRITICAL_SECTION v_csInstInfo;
    static vector<CThostFtdcInstrumentField> v_instInfo;
    static string strAllIns;

    FunctionCallBackSet()
    {
        bIsGetInst = false;
        bIsTdConnected = false;
        bIsMdConnected = false;
        h_connected = CreateEvent(NULL, FALSE, FALSE, NULL);
        h_hasInst = CreateEvent(NULL, FALSE, FALSE, NULL);
        strAllIns = "";
        v_instInfo.clear();
        InitializeCriticalSection(&v_csInstInfo);
    }
    ~FunctionCallBackSet()
    {
        CloseHandle(h_connected);
        CloseHandle(h_hasInst);
        DeleteCriticalSection(&v_csInstInfo);
    }

    //获取当日交易合约信息
    vector<CThostFtdcInstrumentField> &GetInstInfo()
    {
        CLock cl(&v_csInstInfo);
        return v_instInfo;
    }

    static void __stdcall OnConnect(void* pApi, CThostFtdcRspUserLoginField *pRspUserLogin, ConnectionStatus result);//连接后的结果状态
    static void __stdcall OnDisconnect(void* pApi, CThostFtdcRspInfoField *pRspInfo, ConnectionStatus step);//出错时所处的状态
    static void __stdcall OnErrRtnOrderAction(void* pTraderApi, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
    static void __stdcall OnErrRtnOrderInsert(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
    static void __stdcall OnRspError(void* pApi, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspOrderAction(void* pTraderApi, CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspOrderInsert(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryDepthMarketData(void* pTraderApi, CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryInstrument(void* pTraderApi, CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryInstrumentCommissionRate(void* pTraderApi, CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryInstrumentMarginRate(void* pTraderApi, CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryInvestorPosition(void* pTraderApi, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryInvestorPositionDetail(void* pTraderApi, CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryOrder(void* pTraderApi, CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryTrade(void* pTraderApi, CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRspQryTradingAccount(void* pTraderApi, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    static void __stdcall OnRtnDepthMarketData(void* pMdUserApi, CThostFtdcDepthMarketDataField *pDepthMarketData);
    static void __stdcall OnRtnInstrumentStatus(void* pTraderApi, CThostFtdcInstrumentStatusField *pInstrumentStatus);
    static void __stdcall OnRtnOrder(void* pTraderApi, CThostFtdcOrderField *pOrder);
    static void __stdcall OnRtnTrade(void* pTraderApi, CThostFtdcTradeField *pTrade);

};

#endif