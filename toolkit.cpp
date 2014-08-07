#include "toolkit.h"

#include <direct.h>

extern CRITICAL_SECTION cs_fileWriting;
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
    //����ǰ�ҹ���һ��ʱ��
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

void PrintLog(fstream &file, const char *info)
{
    CLock cl(&cs_fileWriting);
    SYSTEMTIME ST;
    GetLocalTime(&ST);
    string time = "[";
    string tmp;
    time += to_string(ST.wYear) + '-';
    time += (tmp = to_string(ST.wMonth), tmp.size() == 1 ? string("0") + tmp : tmp) + '-';
    time += (tmp = to_string(ST.wDay), tmp.size() == 1 ? string("0") + tmp : tmp) + 'T';
    time += (tmp = to_string(ST.wHour), tmp.size() == 1 ? string("0") + tmp : tmp) + ':';
    time += (tmp = to_string(ST.wMinute), tmp.size() == 1 ? string("0") + tmp : tmp) + ':';
    time += (tmp = to_string(ST.wSecond), tmp.size() == 1 ? string("0") + tmp : tmp) + '.';
    tmp = to_string(ST.wMilliseconds);
    if (tmp.size() == 2)
        tmp = string("0") + tmp;
    else if (tmp.size() == 1)
        tmp = string("00") + tmp;
    time += tmp + "Z]: ";
    file << time << info << endl;
    cout << time << info << endl;
}


string ConnectionStatusMsg(ConnectionStatus status)
{
    string res;
    switch (status)
    {

    case E_inited:
            res = "�ѳ�ʼ��";
            break;
    case E_connecting:
            res = "������";
            break;
    case E_connected:
            res = "���ӳɹ�";
            break;
    case E_authing:
            res = "��Ȩ��";
            break;
    case E_authed:
            res = "��Ȩ�ɹ�";
            break;
    case E_logining:
            res = "��¼��";
            break;
    case E_logined:
            res = "��½�ɹ�";
            break;
    case E_confirming:
            res = "���㵥ȷ����";
            break;
    case E_confirmed:
            res = "���㵥�Ѿ�ȷ��";
            break;
    case E_conn_max:
            res = "���Ӵﵽ���ֵ";
            break;
    }
    return res;
}
