#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#include "ThostFtdcUserApiStruct.h"

#include <vector>
#include <set>
#include <string>

using namespace std;

//����·�������ɶ༶Ŀ¼
void makedirs(const char* dir);

//���ַ�����ָ���ַ��ָ�
void GetSetFromString(const char* szString,const char* seps,vector<string>& vct,set<string>& st);

//����OnFrontDisconnected(int nReason)��ֵ���ϴ�����Ϣ
void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo);

#endif