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
		strcpy(pRspInfo->ErrorMsg,"0x1001 网络读失败");
		break;
	case 0x1002:
		strcpy(pRspInfo->ErrorMsg,"0x1002 网络写失败");
		break;
	case 0x2001:
		strcpy(pRspInfo->ErrorMsg,"0x2001 接收心跳超时");
		break;
	case 0x2002:
		strcpy(pRspInfo->ErrorMsg,"0x2002 发送心跳失败");
		break;
	case 0x2003:
		strcpy(pRspInfo->ErrorMsg,"0x2003 收到错误报文");
		break;
	default:
		sprintf(pRspInfo->ErrorMsg,"%x 未知错误");
		break;
	}
}

time_t GetEpochTime(SYSTEMTIME st, string UpdateTime, int milisecond)
{
    struct tm t;
    time_t res;
    t.tm_year = st.wYear - 1900;
    t.tm_mon = st.wMonth - 1;
    t.tm_mday = st.wDay;

    sscanf(UpdateTime.c_str(), "%d:%d:%d", &t.tm_hour, &t.tm_min, &t.tm_sec);
    t.tm_isdst = -1;
    res = mktime(&t);
    //如果是半夜则加一天时间
    if (t.tm_hour >= 0 && t.tm_hour <= 3)
    {
        res += 24 * 60 * 60;
    }
    return res*1000 + milisecond;
}

string GBKToUTF8(const char* strGBK)
{
    int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if (wstr) delete[] wstr;
    if (str) delete[] str;
    return strTemp;
}