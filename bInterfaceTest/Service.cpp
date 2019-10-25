#include "stdafx.h"
#include "Service.h"

extern CString m_Log;
extern CRITICAL_SECTION g_logSection;

Service::Service()
{
	
	mysql = MYSQL();
	mysql_init(&mysql);
	sock = NULL;
	while (!sock){
		sock = mysql_real_connect(&mysql, host,
			user, passwd, db, port, unix_socket, client_flag);
	}
	pSU = NULL;
}


Service::~Service()
{
	if(pSU) delete pSU;
	mysql_free_result(result); //释放结果集 
	mysql_close(sock); //关闭连接 
}


bool Service::sqlRequest(char* query)
{
	// TODO: 在此处添加实现代码.
	
	if (mysql_query(&mysql, query) != 0) return false;
	if ((result = mysql_store_result(&mysql)) == NULL) 
		return false;
	if ((row = mysql_fetch_row(result)) == NULL)
		return false;
	return true;
}

void Service::print(const std::string & xmlData)
{
	int dwNum = MultiByteToWideChar(CP_UTF8, 0, xmlData.c_str(), -1, NULL, 0);
	wchar_t* pwText = NULL;
	pwText = new wchar_t[dwNum + 1];
	memset(pwText, 0, dwNum * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, xmlData.c_str(), -1, pwText, dwNum);
	
	EnterCriticalSection(&g_logSection);
	m_Log.AppendFormat(_T("%s\r\n"),pwText);
	LeaveCriticalSection(&g_logSection);
	HWND Hwnd = ::FindWindow(NULL, _T("B接口测试工具"));
	SendMessageW(Hwnd, MY_MSG_UPDATEDISPLAY, 0, 0);
}

void Service::print(CString & resultData)
{
	HWND Hwnd = ::FindWindow(NULL, _T("B接口测试工具"));
	EnterCriticalSection(&g_logSection);
	m_Log.AppendFormat(_T("%s\r\n__________________________\r\n"),resultData);
	LeaveCriticalSection(&g_logSection);
	SendMessageW(Hwnd, MY_MSG_UPDATEDISPLAY, 0, 0);
}

//处理接收到的报文，xmldata接受到的报文，_invokeReturn用于soap返回
bool Service::dispatch(const std::string &xmlData, std::string &_invokeReturn)
{
	// TODO: 在此处添加实现代码.
	TiXmlDocument doc;
	receiveLog = "";
	doc.Parse(xmlData.c_str());
	int code = this->checkRoot(doc);
	if (!code)
	{
		_invokeReturn = "failure";
		return false;
	}
	switch (code)
	{
	case 101:
		if (check101(doc))
		{
			_invokeReturn = "sucess";
			return true;
		}
		else
		{
			_invokeReturn = "failure";
			return false;
		}
		break;
	case 203:
		if (check203(doc))
		{
			_invokeReturn = "sucess";
			return true;
		}
		else
		{
			_invokeReturn = "failure";
			return false;
		}
		break;
	case 205:
		if (check205(doc))
		{
			_invokeReturn = "sucess";
			return true;
		}
		else
		{
			_invokeReturn = "failure";
			return false;
		}
		break;
	case 303:
		if (check303(doc))
		{
			_invokeReturn = "sucess";
			return true;
		}
		else
		{
			_invokeReturn = "failure";
			return false;
		}
		break;
	case 305:
		if (check305(doc))
		{
			_invokeReturn = "sucess";
			return true;
		}
		else
		{
			_invokeReturn = "failure";
			return false;
		}
		break;
	case 603:
		if (check603(doc))
		{
			_invokeReturn = "sucess";
			return true;
		}
		else
		{
			_invokeReturn = "failure";
			return false;
		}
		break;
	case 605:
		if (check605(doc))
		{
			_invokeReturn = "sucess";
			return true;
		}
		else
		{
			_invokeReturn = "failure";
			return false;
		}
		break;
	default:
		break;
		
	}
	return false;
}
	
	


int Service::checkRoot(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.
	TiXmlElement* root_node = doc.RootElement();
	if (!root_node) {
		receiveLog = _T("根节点解析失败，请确定xml报文格式正确！");
		return 0;
	}
	TiXmlElement* child_node = root_node->FirstChildElement("PK_Type");
	if (!child_node) {
		receiveLog = _T("PK_Type节点解析错误！");
		return 0;
	}

	child_node = child_node->FirstChildElement("Name");
	if (!child_node) {
		receiveLog = _T("Name节点解析错误！");
		return 0;
	}
	std::string name(child_node->GetText());

	child_node = child_node->NextSiblingElement("Code");
	if (!child_node) {
		receiveLog = _T("Code节点解析错误！");
		return 0;
	}
	int code = atoi(child_node->GetText());
	size_t index = 0;
	for (index = 0; index <= 6; ++index)
	{
		if (codeArray[index] == code)
			break;
	}
	if (nameArray[index] != name)
	{
		receiveLog = _T("指令名称与指令编号不匹配！");
		return 0;
	}
	return code;
}


bool Service::check101(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.
	
	TiXmlElement *root_node = doc.RootElement();
	TiXmlElement *Info_node = root_node->FirstChildElement("Info");
	if (!Info_node)
	{
		receiveLog = _T("Info节点解析错误！");
		return false;
	}
	
	TiXmlElement *child_node = NULL;
	
	if (pSU) //是否已有SU登录信息，如果有判断SUconfigtime是否变化，如果有则更新数据
	{
		child_node = Info_node->FirstChildElement("SUId");
		if (!child_node)
		{
			delete pSU; pSU = NULL;
			receiveLog = _T("SUId节点解析错误！");
			return false;
		}
		if ((pSU->SUId) == CString(child_node->GetText()))
		{
			child_node = Info_node->FirstChildElement("SUConfigTime");
			if (!child_node)
			{
				receiveLog = _T("SUConfigTime节点解析错误！");
				delete pSU; pSU = NULL;
				return false;
			}
			if ((pSU->SUConfigTime) == CString(child_node->GetText()))
			{
				receiveLog += _T("SU重复登录。");
				nodeExist(UserName);
				nodeExist(PassWord);
				nodeExist(SURId);
				nodeExist(SUIP);
				nodeExist(SUPort);
				nodeExist(SUVendor);
				nodeExist(SUModel);
				nodeExist(SUHardVer);
				nodeExist(SUVer);
				nodeExist(DeviceList);
				return true;
			}
			else
			{
				receiveLog += _T("SU更新登录，数据更新。");
				delete pSU;
				pSU = new SU();
			}
			
		}
		else
		{
			receiveLog = _T("错误！该工具同时仅能测试一台SU设备！");
			return false;
		}

	}else
		pSU = new SU();
	nodeExist(UserName);
	pSU->UserName = child_node->GetText();
	nodeExist(PassWord);
	pSU->PassWord = child_node->GetText();
	nodeExist(SUId);
	pSU->SUId = child_node->GetText();
	nodeExist(SURId);
	pSU->SURId = child_node->GetText();
	nodeExist(SUIP);
	pSU->SUIP = child_node->GetText();
	nodeExist(SUPort);
	pSU->SUPort = atoi(child_node->GetText());
	nodeExist(SUVendor);
	
	int dwNum = MultiByteToWideChar(CP_UTF8, 0, child_node->GetText(), -1, NULL, 0);
	wchar_t* pwText = NULL;
	pwText = new wchar_t[dwNum + 1];
	memset(pwText, 0, dwNum * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, child_node->GetText(), -1, pwText, dwNum);
	pSU->SUVendor = pwText;
	nodeExist(SUModel);
	pSU->SUModel = child_node->GetText();
	nodeExist(SUHardVer);
	pSU->SUHardVer = child_node->GetText();
	nodeExist(SUConfigTime);
	pSU->SUConfigTime = child_node->GetText();
	nodeExist(SUVer);
	pSU->SUVer = atof(child_node->GetText());

	nodeExist(DeviceList);
	if(pSU->pDeviceMap)
	{
		receiveLog = _T("DeviceList重复错误！");
		delete pSU; pSU = NULL;
		return false;
	}else
		pSU->pDeviceMap = new std::map<CString, Device*>;
	child_node = child_node->FirstChildElement("Device");
	if (!child_node)
	{
		receiveLog = _T("Device节点解析错误！");
		delete pSU;pSU = NULL;
		return false;
	}
	while (child_node)
	{
		CString Id(child_node->Attribute("Id"));
		CString RId(child_node->Attribute("RId"));
		if(Id=="")
		{
			receiveLog = _T("Device节点Id属性解析错误！");
			delete pSU; pSU = NULL;
			return false;
			
		}
		
		if (RId == "")
		{
			receiveLog = _T("Device节点RId属性解析错误！");
			delete pSU; pSU = NULL;
			return false;

		}
		if ((*pSU->pDeviceMap)[Id])
		{
			receiveLog.Format(_T("Device编码:%s重复！"),Id);
			delete pSU; pSU = NULL;
			return false;
		}
		else
		{
			CString query;
			query.Format(_T("select DeviceCategory from idlist where DeviceId = '%s' limit 1;"), Id.Left(3));
			USES_CONVERSION;
			char* charQuery = T2A(query.GetBuffer(0));
			query.ReleaseBuffer();

			
			if (sqlRequest(charQuery))
			{
				(*pSU->pDeviceMap)[Id] = new Device(Id, RId);
			}
			else
			{
				receiveLog.Format(_T("Device编码:%s在数据库中不存在！"), Id);
				delete pSU; pSU = NULL;
				return false;
			}
			
		}
			
		child_node = child_node->NextSiblingElement("Device");
	}


	receiveLog.Format(_T("登陆成功\r\n"));
	receiveLog.AppendFormat(_T("SUId:\t\t%s\r\nSURId：\t\t%s\r\n用户名：\t\t%s\r\n密码：\t\t%s\r\nSUIP：\t\t%s\r\nSU端口号：\t%d\r\nSU生产厂家：\t%s\r\nSU型号：\t\t%s\r\nSU硬件版本号：\t%s\r\n配置时间：\t%s\r\nSU版本号：\t%.1f\r\n")
		, pSU->SUId, pSU->SURId, pSU->UserName, pSU->PassWord, pSU->SUIP
		, pSU->SUPort, pSU->SUVendor, pSU->SUModel, pSU->SUHardVer
		, pSU->SUConfigTime, pSU->SUVer);
	return true;
}

bool Service::check203(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.

	return true;
}
bool Service::check205(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.

	return true;
}
bool Service::check303(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.

	return true;
}
bool Service::check305(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.

	return true;
}
bool Service::check603(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.

	return true;
}
bool Service::check605(TiXmlDocument& doc)
{
	// TODO: 在此处添加实现代码.

	return true;
}
