
// UDS GL Tools.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CUDSGLToolsApp:
// 有关此类的实现，请参阅 UDS GL Tools.cpp
//

class CUDSGLToolsApp : public CWinApp
{
public:
	CUDSGLToolsApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	HANDLE    m_hMutex;  //创建、注销互斥量
	bool      BarcodeEvent;
};

extern CUDSGLToolsApp theApp;