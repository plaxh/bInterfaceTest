
// bInterfaceTestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "bInterfaceTest.h"
#include "bInterfaceTestDlg.h"
#include "afxdialogex.h"
#include "Service.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SCServiceSoapBinding.nsmap"
#include "soapH.h"
#include "soapSCServiceSoapBindingService.h"


CString m_Log;
CRITICAL_SECTION g_logSection;
Service soapResolv;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CbInterfaceTestDlg 对话框



CbInterfaceTestDlg::CbInterfaceTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BINTERFACETEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbInterfaceTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CbInterfaceTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(MY_MSG_UPDATEDISPLAY, &CbInterfaceTestDlg::DisplayTest)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CbInterfaceTestDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CbInterfaceTestDlg::OnBnClickedButtonClear)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND, &CbInterfaceTestDlg::OnBnClickedButtonSuspend)
END_MESSAGE_MAP()

// 新建线程，开启soap服务
UINT ListenTheadFunc(LPVOID pParam)
{
	
	
	int backlog = 0;
	SCServiceSoapBindingService SCService;
	soap_set_mode(SCService.soap, SOAP_C_UTFSTRING);
	while (true) {
		SCService.run(8080);
	}
		
	
	return 0;
}
// CbInterfaceTestDlg 消息处理程序

BOOL CbInterfaceTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_InService = TRUE;
	InitializeCriticalSection(&g_logSection);
	m_ListenThread = AfxBeginThread(ListenTheadFunc, this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CbInterfaceTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbInterfaceTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CbInterfaceTestDlg::OnQueryDragIcon()
{ 
	return static_cast<HCURSOR>(m_hIcon);
}

int SCServiceSoapBindingService::invoke(const std::string& _xmlData, std::string &_invokeReturn)
{
	soapResolv.print(_xmlData);
	CString result("");
	soapResolv.resolv(_xmlData, result,_invokeReturn);
	soapResolv.print(result);
	return SOAP_OK;
}

LRESULT CbInterfaceTestDlg::DisplayTest(WPARAM wParam, LPARAM lParam)
{
	CEdit * m_Edit;
	m_Edit = (CEdit *)GetDlgItem(IDC_EDIT_LOG);
	m_Edit->SetWindowTextW(m_Log);
	m_Edit->LineScroll(m_Edit->GetLineCount() - 1, 0);
	return 0;
}




void CbInterfaceTestDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	
		
		 //     ");
	EnterCriticalSection(&g_logSection);
	m_Log += "King \r\n";
	m_Log += "Athamus of northern \r\n";
	m_Log += "Greece had two children，\r\n";
	m_Log += "Phrixus and Helle．\r\n";
	m_Log += "After he left his first wife\r\n";
	m_Log += "and mar ried Ino，\r\n";
	m_Log += "two children received all\r\n";
	m_Log += "the cruel treatment that\r\n";
	m_Log += "a wicked woman，the\r\n";
	m_Log += "a stepmother coulddevise ，\r\n";
	m_Log += "At one timethe kingdom was\r\n";
	m_Log += "ruined by a famine．\r\n";
	LeaveCriticalSection(&g_logSection);
	DisplayTest(0,0);
}


void CbInterfaceTestDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Log = "";
	DisplayTest(0, 0);
}


void CbInterfaceTestDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//TerminateProcess
	m_InService = FALSE;
	
	///WaitForSingleObject(m_ListenThread->m_hThread, INFINITE);
	DeleteCriticalSection(&g_logSection);
	CDialogEx::OnClose();
}


void CbInterfaceTestDlg::OnBnClickedButtonSuspend()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
