
// LicenseBuilder.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLicenseBuilderApp:
// �йش����ʵ�֣������ LicenseBuilder.cpp
//

class CLicenseBuilderApp : public CWinApp
{
public:
	CLicenseBuilderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLicenseBuilderApp theApp;