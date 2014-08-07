#ifndef CTPSTRUCT_H
#define CTPSTRUCT_H
#include "traderapi\ThostFtdcUserApiStruct.h"
#include<string>
using namespace std;

//连接状态
enum ConnectionStatus
{
    E_uninit,		//未初始化
    E_inited,		//已经初始化
    E_unconnected,	//连接已经断开
    E_connecting,	//连接中
    E_connected,	//连接成功
    E_authing,		//授权中
    E_authed,		//授权成功
    E_logining,		//登录中
    E_logined,		//登录成功
    E_confirming,	//结算单确认中
    E_confirmed,	//已经确认 
    E_conn_max		//最大值
};

//函数回调定义
typedef void(__stdcall *fnOnConnect)(void* pApi, CThostFtdcRspUserLoginField *pRspUserLogin, ConnectionStatus result);//连接后的结果状态
typedef void(__stdcall *fnOnDisconnect)(void* pApi, CThostFtdcRspInfoField *pRspInfo, ConnectionStatus step);//出错时所处的状态
typedef void(__stdcall *fnOnErrRtnOrderAction)(void* pTraderApi, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
typedef void(__stdcall *fnOnErrRtnOrderInsert)(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
typedef void(__stdcall *fnOnRspError)(void* pApi, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspOrderAction)(void* pTraderApi, CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspOrderInsert)(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryDepthMarketData)(void* pTraderApi, CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryInstrument)(void* pTraderApi, CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryInstrumentCommissionRate)(void* pTraderApi, CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryInstrumentMarginRate)(void* pTraderApi, CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryInvestorPosition)(void* pTraderApi, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryInvestorPositionDetail)(void* pTraderApi, CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryOrder)(void* pTraderApi, CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryTrade)(void* pTraderApi, CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRspQryTradingAccount)(void* pTraderApi, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void(__stdcall *fnOnRtnDepthMarketData)(void* pMdUserApi, CThostFtdcDepthMarketDataField *pDepthMarketData);
typedef void(__stdcall *fnOnRtnInstrumentStatus)(void* pTraderApi, CThostFtdcInstrumentStatusField *pInstrumentStatus);
typedef void(__stdcall *fnOnRtnOrder)(void* pTraderApi, CThostFtdcOrderField *pOrder);
typedef void(__stdcall *fnOnRtnTrade)(void* pTraderApi, CThostFtdcTradeField *pTrade);

#endif

