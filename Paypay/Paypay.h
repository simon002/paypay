
// Paypay.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPaypayApp:
// �йش����ʵ�֣������ Paypay.cpp
//

class CPaypayApp : public CWinAppEx
{
public:
	CPaypayApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPaypayApp theApp;