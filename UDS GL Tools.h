
// UDS GL Tools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUDSGLToolsApp:
// �йش����ʵ�֣������ UDS GL Tools.cpp
//

class CUDSGLToolsApp : public CWinApp
{
public:
	CUDSGLToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	HANDLE    m_hMutex;  //������ע��������
	bool      BarcodeEvent;
};

extern CUDSGLToolsApp theApp;