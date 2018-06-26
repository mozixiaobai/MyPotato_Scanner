#pragma once
#include "resource.h"
#include "PngButton.h"


// CUDSTips 对话框

class CUDSTips : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSTips)

public:
	CUDSTips(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSTips();

// 对话框数据
	enum { IDD = IDD_DIA_TIPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL     m_BDeleteAll;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CPngButton m_btnOK;
	CPngButton m_btnCancel;
};
