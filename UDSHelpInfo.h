#pragma once
#include "resource.h"


// CUDSHelpInfo �Ի���

class CUDSHelpInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSHelpInfo)

public:
	CUDSHelpInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSHelpInfo();

// �Ի�������
	enum { IDD = IDD_DLG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString  m_strHelpInfo;
	CFont    m_CurFont;
	virtual BOOL OnInitDialog();
};
