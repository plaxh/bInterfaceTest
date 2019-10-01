
// bInterfaceTestDlg.h: 头文件
//

#pragma once

#define MY_MSG_UPDATEDISPLAY WM_USER+1

// CbInterfaceTestDlg 对话框
class CbInterfaceTestDlg : public CDialogEx
{
// 构造
public:
	CbInterfaceTestDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CWinThread * m_ListenThread;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINTERFACETEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT DisplayTest(WPARAM wParam, LPARAM lParam);
public:
	

	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnClose();
	bool m_InService;
	afx_msg void OnBnClickedButtonSuspend();
};
