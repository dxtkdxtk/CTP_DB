#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#include "ThostFtdcUserApiStruct.h"
#include "CLock.h"
#include "CTPStruct.h"

#include <vector>
#include <set>
#include <string>
#include <ctime>
#include <Windows.h>
#include <fstream>
#include <iostream>

using namespace std;

//输入路径，生成多级目录
void makedirs(const char* dir);

//将字符串按指定字符分割
void GetSetFromString(const char* szString,const char* seps,vector<string>& vct,set<string>& st);

//根据OnFrontDisconnected(int nReason)的值填上错误消息
void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo);

//转换交易所时间到unix epoch time
time_t GetEpochTime(SYSTEMTIME st, string UpdateTime, int milisecond);

//转换GBK到UTF8
string GBKToUTF8(const char* strGBK);

//日志显示
void PrintLog(fstream &file, const char *info);

//转换连接信息
string ConnectionStatusMsg(ConnectionStatus status);


#endif
