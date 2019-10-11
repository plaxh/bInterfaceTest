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
	mysql_free_result(result); //�ͷŽ���� 
	mysql_close(sock); //�ر����� 
}


bool Service::sqlRequest(char* query)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	
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
	HWND Hwnd = ::FindWindow(NULL, _T("B�ӿڲ��Թ���"));
	SendMessageW(Hwnd, MY_MSG_UPDATEDISPLAY, 0, 0);
}

void Service::print(CString & resultData)
{
	HWND Hwnd = ::FindWindow(NULL, _T("B�ӿڲ��Թ���"));
	EnterCriticalSection(&g_logSection);
	m_Log.AppendFormat(_T("%s\r\n__________________________\r\n"),resultData);
	LeaveCriticalSection(&g_logSection);
	SendMessageW(Hwnd, MY_MSG_UPDATEDISPLAY, 0, 0);
}


bool Service::resolv(const std::string &xmlData, CString& result_XML, std::string &_invokeReturn)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	TiXmlDocument doc;

	doc.Parse(xmlData.c_str());
	int code = this->checkRoot(doc, result_XML);
	if (!code)
	{
		_invokeReturn = "failure";
		return false;
	}
	switch (code)
	{
	case 101:
		if (check101(doc, result_XML))
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
		if (check203(doc, result_XML))
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
		if (check205(doc, result_XML))
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
		if (check303(doc, result_XML))
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
		if (check305(doc, result_XML))
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
		if (check603(doc, result_XML))
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
		if (check605(doc, result_XML))
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
	
	


int Service::checkRoot(TiXmlDocument& doc,CString & result_XML)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	TiXmlElement* root_node = doc.RootElement();
	if (!root_node) {
		result_XML = _T("���ڵ����ʧ�ܣ���ȷ��xml���ĸ�ʽ��ȷ��");
		return 0;
	}
	TiXmlElement* child_node = root_node->FirstChildElement("PK_Type");
	if (!child_node) {
		result_XML = _T("PK_Type�ڵ��������");
		return 0;
	}

	child_node = child_node->FirstChildElement("Name");
	if (!child_node) {
		result_XML = _T("Name�ڵ��������");
		return 0;
	}
	std::string name(child_node->GetText());

	child_node = child_node->NextSiblingElement("Code");
	if (!child_node) {
		result_XML = _T("Code�ڵ��������");
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
		result_XML = _T("ָ��������ָ���Ų�ƥ�䣡");
		return 0;
	}
	return code;
}


bool Service::check101(TiXmlDocument& doc, CString& checkResult)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	
	TiXmlElement *root_node = doc.RootElement();
	TiXmlElement *Info_node = root_node->FirstChildElement("Info");
	if (!Info_node)
	{
		checkResult = _T("Info�ڵ��������");
		return false;
	}
	
	TiXmlElement *child_node = NULL;
	
	if (pSU) //�Ƿ�����SU��¼��Ϣ��������ж�SUconfigtime�Ƿ�仯����������������
	{
		child_node = Info_node->FirstChildElement("SUId");
		if (!child_node)
		{
			delete pSU; pSU = NULL;
			checkResult = _T("SUId�ڵ��������");
			return false;
		}
		if ((pSU->SUId) == CString(child_node->GetText()))
		{
			child_node = Info_node->FirstChildElement("SUConfigTime");
			if (!child_node)
			{
				checkResult = _T("SUConfigTime�ڵ��������");
				delete pSU; pSU = NULL;
				return false;
			}
			if ((pSU->SUConfigTime) == CString(child_node->GetText()))
			{
				checkResult += _T("SU�ظ���¼��");
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
				checkResult += _T("SU���µ�¼�����ݸ��¡�");
				delete pSU;
				pSU = new SU();
			}
			
		}
		else
		{
			checkResult = _T("���󣡸ù���ͬʱ���ܲ���һ̨SU�豸��");
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
		checkResult = _T("DeviceList�ظ�����");
		delete pSU; pSU = NULL;
		return false;
	}else
		pSU->pDeviceMap = new std::map<CString, Device*>;
	child_node = child_node->FirstChildElement("Device");
	if (!child_node)
	{
		checkResult = _T("Device�ڵ��������"); 
		delete pSU;pSU = NULL;
		return false;
	}
	while (child_node)
	{
		CString Id(child_node->Attribute("Id"));
		CString RId(child_node->Attribute("RId"));
		if(Id=="")
		{
			checkResult = _T("Device�ڵ�Id���Խ�������");
			delete pSU; pSU = NULL;
			return false;
			
		}
		
		if (RId == "")
		{
			checkResult = _T("Device�ڵ�RId���Խ�������");
			delete pSU; pSU = NULL;
			return false;

		}
		if ((*pSU->pDeviceMap)[Id])
		{
			checkResult.Format(_T("Device����:%s�ظ���"),Id);
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
				checkResult.Format(_T("Device����:%s�����ݿ��в����ڣ�"), Id);
				delete pSU; pSU = NULL;
				return false;
			}
			
		}
			
		child_node = child_node->NextSiblingElement("Device");
	}


	checkResult.Format(_T("��½�ɹ�\r\n"));
	checkResult.AppendFormat(_T("SUId:\t\t%s\r\nSURId��\t\t%s\r\n�û�����\t\t%s\r\n���룺\t\t%s\r\nSUIP��\t\t%s\r\nSU�˿ںţ�\t%d\r\nSU�������ң�\t%s\r\nSU�ͺţ�\t\t%s\r\nSUӲ���汾�ţ�\t%s\r\n����ʱ�䣺\t%s\r\nSU�汾�ţ�\t%.1f\r\n")
		, pSU->SUId, pSU->SURId, pSU->UserName, pSU->PassWord, pSU->SUIP
		, pSU->SUPort, pSU->SUVendor, pSU->SUModel, pSU->SUHardVer
		, pSU->SUConfigTime, pSU->SUVer);
	return true;
}

bool Service::check203(TiXmlDocument& doc, CString& checkResult)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	return true;
}
bool Service::check205(TiXmlDocument& doc, CString& checkResult)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	return true;
}
bool Service::check303(TiXmlDocument& doc, CString& checkResult)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	return true;
}
bool Service::check305(TiXmlDocument& doc, CString& checkResult)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	return true;
}
bool Service::check603(TiXmlDocument& doc, CString& checkResult)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	return true;
}
bool Service::check605(TiXmlDocument& doc, CString& checkResult)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	return true;
}
