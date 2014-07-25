#include "toolkit.h"
#include <direct.h>

void makedirs(const char* dir)
{
	if (NULL == dir)
		return;

	int len = strlen(dir);

	char * p = new char[len+1];
	strcpy(p,dir);
	for (int i=0;i<len;++i)
	{
		char ch = p[i];
		if ('\\' == ch ||'/' == ch)
		{ 
			p[i] = '\0';
			_mkdir(p);
			p[i] = ch;
		}
	}
	delete[] p;
}

void GetSetFromString(const char* szString,const char* seps,vector<string>& vct,set<string>& st)
{
	vct.clear();
	st.clear();

	if(NULL == szString
		||NULL == seps)
		return;

	if(strlen(szString)==0
		||strlen(seps)==0)
		return;
	
	size_t len = strlen(szString)+1;
	char* buf = new char[len];
	strncpy(buf,szString,len);

	char* token = strtok(buf, seps);
	while(token)
	{
		if (strlen(token)>0)
		{
			vct.push_back(token);
			st.insert(token);
		}
		token = strtok( NULL, seps);
	}

	delete[] buf;
}

void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo)
{
	if(NULL == pRspInfo)
		return;

	switch(pRspInfo->ErrorID)
	{
	case 0x1001:
		strcpy(pRspInfo->ErrorMsg,"0x1001 �����ʧ��");
		break;
	case 0x1002:
		strcpy(pRspInfo->ErrorMsg,"0x1002 ����дʧ��");
		break;
	case 0x2001:
		strcpy(pRspInfo->ErrorMsg,"0x2001 ����������ʱ");
		break;
	case 0x2002:
		strcpy(pRspInfo->ErrorMsg,"0x2002 ��������ʧ��");
		break;
	case 0x2003:
		strcpy(pRspInfo->ErrorMsg,"0x2003 �յ�������");
		break;
	default:
		sprintf(pRspInfo->ErrorMsg,"%x δ֪����");
		break;
	}
}