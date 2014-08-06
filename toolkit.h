#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#include "ThostFtdcUserApiStruct.h"

#include <vector>
#include <set>
#include <string>
#include <ctime>
#include <Windows.h>
using namespace std;

//����·�������ɶ༶Ŀ¼
void makedirs(const char* dir);

//���ַ�����ָ���ַ��ָ�
void GetSetFromString(const char* szString,const char* seps,vector<string>& vct,set<string>& st);

//����OnFrontDisconnected(int nReason)��ֵ���ϴ�����Ϣ
void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo);

//ת��������ʱ�䵽unix epoch time
time_t GetEpochTime(string TradingDay, string UpdateTime, int milisecond);

//ת��GBK��UTF8
string GBKToUTF8(const char* strGBK);

#endif
