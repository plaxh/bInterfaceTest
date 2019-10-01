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
	checkResult = _T(#nodename##"节点解析错误！");\
	delete pSU;pSU = NULL;\
	return false;\
}\


class Service
{
private:
	const char * host = "127.0.0.1"; //因为是作为本机测试，所以填写的是本地IP 
	const char * user = "root"; //这里改为你的用户名，即连接MySQL的用户名 
	const char * passwd = "123456"; //这里改为你的用户密码 
	const char * db = "student"; //这里改为你要连接的数据库的名字,一个数据可能有几张表
	const unsigned int port = 3306; //这是MySQL的服务器的端口，如果你没有修改过的话就是3306。 
	const char * unix_socket = NULL; //unix_socket这是unix下的，我在Windows下，所以就把它设置为NULL 
	const unsigned int codeArray[7] = { 101,203,205,303,305,603,605 };
	const std::string nameArray[7] = { "LOGIN","SEND_AIDATA",
		"SEND_HISAIDATA" ,"SEND_DIDATA",
		"SEND_HISDIDATA","SEND_ALARM",
		"SEND_HISALARM" };
	unsigned long client_flag = 0; //这个参数一般为0 
private:
	MYSQL mysql, *sock;
	
public:
	MYSQL_RES *result;
	MYSQL_ROW row;
	SU* pSU;

public:
	Service();
	~Service();
	bool sqlRequest(char*);
	void print(const std::string &);
	void print(CString &);
	bool resolv(const std::string &,CString &, std::string &);
private:
	int checkRoot(TiXmlDocument&,CString&);
	bool check101(TiXmlDocument&, CString&);
};
