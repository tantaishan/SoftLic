
// SoftLicTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSoftLicTestApp:
// �йش����ʵ�֣������ SoftLicTest.cpp
//

#define SOFT_ID _T("{5806DD87-DCF4-4812-961D-C4E9CFDBE0AC}")

class CSoftLicTestApp : public CWinApp
{
public:
	CSoftLicTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSoftLicTestApp theApp;