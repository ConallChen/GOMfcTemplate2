
// GOMfcTemplate2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGOMfcTemplate2App:
// �йش����ʵ�֣������ GOMfcTemplate2.cpp
//

class CGOMfcTemplate2App : public CWinApp
{
public:
	CGOMfcTemplate2App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGOMfcTemplate2App theApp;