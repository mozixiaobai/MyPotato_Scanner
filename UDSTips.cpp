// UDSTips.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSTips.h"
#include "afxdialogex.h"


// CUDSTips 对话框

IMPLEMENT_DYNAMIC(CUDSTips, CDialogEx)

CUDSTips::CUDSTips(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSTips::IDD, pParent)
{
	m_BDeleteAll = FALSE;
}

CUDSTips::~CUDSTips()
{
}

void CUDSTips::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CUDSTips, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUDSTips::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDSTips::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CUDSTips::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDialogEx::SetBackgroundColor(RGB(134, 130, 167));
	m_btnOK.Init(IDB_PNG_CANCEL, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnCancel.Init(IDB_PNG_OK, BTN_IMG_1, BTN_TYPE_NORMAL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CUDSTips::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID()==IDC_STA_TIPS)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(245, 250, 6));
		CFont   tem_Font;
		tem_Font.CreatePointFont(200, _T("楷体"));
		pDC->SelectObject(&tem_Font);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
// CUDSTips 消息处理程序


void CUDSTips::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BDeleteAll = TRUE;
	CDialogEx::OnOK();
}


void CUDSTips::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_BDeleteAll = FALSE;
	CDialogEx::OnCancel();
}

