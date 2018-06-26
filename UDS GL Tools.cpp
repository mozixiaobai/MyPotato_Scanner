
// UDS GL Tools.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UDS GL Tools.h"
#include "UDS GL ToolsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDSGLToolsApp

BEGIN_MESSAGE_MAP(CUDSGLToolsApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUDSGLToolsApp ����

CUDSGLToolsApp::CUDSGLToolsApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUDSGLToolsApp ����

CUDSGLToolsApp theApp;


// CUDSGLToolsApp ��ʼ��

BOOL CUDSGLToolsApp::InitInstance()
{
	//����ʶ�𿪹�
	BarcodeEvent = true;
	//��������������ֻ֤��һ��ʵ������
	m_hMutex = CreateMutex(NULL,TRUE,_T("UDSScannerToolMutualExclusion"));
	if (m_hMutex == NULL)
	{
		return FALSE;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return FALSE;
	}
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CUDSGLToolsDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CUDSGLToolsApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);	//�رվ��
	}

	return CWinApp::ExitInstance();
}
