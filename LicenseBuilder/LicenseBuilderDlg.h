
// LicenseBuilderDlg.h : ͷ�ļ�
//

#pragma once
#include "atlcomtime.h"


// CLicenseBuilderDlg �Ի���
class CLicenseBuilderDlg : public CDialogEx
{
// ����
public:
	CLicenseBuilderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LICENSEBUILDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBuild();
	CString m_strSoftId;
	CString m_strPassword;
	COleDateTime m_strExpiry;
	CString m_strPath;
};
