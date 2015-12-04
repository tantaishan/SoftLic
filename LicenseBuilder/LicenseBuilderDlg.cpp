
// LicenseBuilderDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CLicenseBuilderDlg �Ի���



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


// CLicenseBuilderDlg ��Ϣ�������

BOOL CLicenseBuilderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLicenseBuilderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLicenseBuilderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLicenseBuilderDlg::OnBnClickedBuild()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
