
// LicenseBuilderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LicenseBuilder.h"
#include "LicenseBuilderDlg.h"
#include "afxdialogex.h"
#include "../SoftLicSDK/SoftLicSDK.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLicenseBuilderDlg 对话框



CLicenseBuilderDlg::CLicenseBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLicenseBuilderDlg::IDD, pParent)
	, m_strSoftId(_T(""))
	, m_strPassword(_T(""))
	, m_strExpiry(COleDateTime::GetCurrentTime())
	, m_strPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLicenseBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOFTID, m_strSoftId);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPassword);
	DDX_DateTimeCtrl(pDX, IDC_DTP_EXPIRY, m_strExpiry);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
}

BEGIN_MESSAGE_MAP(CLicenseBuilderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUILD, &CLicenseBuilderDlg::OnBnClickedBuild)
END_MESSAGE_MAP()


// CLicenseBuilderDlg 消息处理程序

BOOL CLicenseBuilderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLicenseBuilderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLicenseBuilderDlg::OnPaint()
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
HCURSOR CLicenseBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLicenseBuilderDlg::OnBnClickedBuild()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	UpdateData(TRUE);
	LicInfo licInfo;
	licInfo.softid = T2A(m_strSoftId);
	licInfo.pasword = T2A(m_strPassword);
	time_t now;
	time(&now);
	licInfo.start_time  = now;

	SYSTEMTIME st;
	m_strExpiry.GetAsSystemTime(st);

	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	LONGLONG nLL;
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;
	nLL = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;
	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);

	licInfo.end_time = pt;

	if (SoftLicSDK::Build(licInfo, T2A(m_strPath)))
	{
		AfxMessageBox(_T("Build successful."));
	}
	else
	{
		AfxMessageBox(_T("Build failed."));
	}

}
