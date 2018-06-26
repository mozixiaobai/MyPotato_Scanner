#pragma once
#include "resource.h"


// CUDSHelpInfo 对话框

class CUDSHelpInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSHelpInfo)

public:
	CUDSHelpInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSHelpInfo();

// 对话框数据
	enum { IDD = IDD_DLG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString  m_strHelpInfo;
	CFont    m_CurFont;
	virtual BOOL OnInitDialog();
};
