#ifndef CTPSTRUCT_H
#define CTPSTRUCT_H
#include "traderapi\ThostFtdcUserApiStruct.h"
#include<string>
using namespace std;

//����״̬
enum ConnectionStatus
{
    E_uninit,		//δ��ʼ��
    E_inited,		//�Ѿ���ʼ��
    E_unconnected,	//�����Ѿ��Ͽ�
    E_connecting,	//������
    E_connected,	//���ӳɹ�
    E_authing,		//��Ȩ��
    E_authed,		//��Ȩ�ɹ�
    E_logining,		//��¼��
    E_logined,		//��¼�ɹ�
    E_confirming,	//���㵥ȷ����
    E_confirmed,	//�Ѿ�ȷ�� 
    E_conn_max		//���ֵ
};

//�����ص�����
typedef void(__stdcall *fnOnConnect)(void* pApi, CThostFtdcRspUserLoginField *pRspUserLogin, ConnectionStatus result);//���Ӻ�Ľ��״̬
typedef void(__stdcall *fnOnDisconnect)(void* pApi, CThostFtdcRspInfoField *pRspInfo, ConnectionStatus step);//����ʱ������״̬
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

