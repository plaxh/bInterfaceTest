#pragma once
#include "SU.h"
#include <mysql.h>
#include <string>
#include "tinyxml.h"
#include "bInterfaceTestDlg.h"
#pragma comment(lib, "libmysql.lib")

#define nodeExist(nodename) child_node = Info_node->FirstChildElement(#nodename);\
if (!child_node)\
{\
	receiveLog = _T(#nodename##"�ڵ��������");\
	delete pSU;pSU = NULL;\
	return false;\
}\


class Service
{
private:
	const char * host = "127.0.0.1"; //��Ϊ����Ϊ�������ԣ�������д���Ǳ���IP 
	const char * user = "root"; //�����Ϊ����û�����������MySQL���û��� 
	const char * passwd = "1234"; //�����Ϊ����û����� 
	const char * db = "binterface"; //�����Ϊ��Ҫ���ӵ����ݿ������,һ�����ݿ����м��ű�
	const unsigned int port = 3306; //����MySQL�ķ������Ķ˿ڣ������û���޸Ĺ��Ļ�����3306�� 
	const char * unix_socket = NULL; //unix_socket����unix�µģ�����Windows�£����ԾͰ�������ΪNULL 
	const unsigned int codeArray[7] = { 101,203,205,303,305,603,605 };
	const std::string nameArray[7] = { "LOGIN","SEND_AIDATA",
		"SEND_HISAIDATA" ,"SEND_DIDATA",
		"SEND_HISDIDATA","SEND_ALARM",
		"SEND_HISALARM" };
	unsigned long client_flag = 0; //�������һ��Ϊ0 
private:
	MYSQL mysql, *sock;
	
public:
	MYSQL_RES *result;
	MYSQL_ROW row;
	SU* pSU;
	CString receiveLog; //������ʾ�ڴ����У���ʾ���յ����ĵĽ������
public:
	Service();
	~Service();
	bool sqlRequest(char*);
	void print(const std::string &);
	void print(CString &);
	bool dispatch(const std::string &,std::string &);
private:
	int checkRoot(TiXmlDocument&);
	bool check101(TiXmlDocument&);
	bool check203(TiXmlDocument&);
	bool check205(TiXmlDocument&);
	bool check303(TiXmlDocument&);
	bool check305(TiXmlDocument&);
	bool check603(TiXmlDocument&);
	bool check605(TiXmlDocument&);
};
