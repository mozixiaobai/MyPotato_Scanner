// UDSTips.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDSTips.h"
#include "afxdialogex.h"


// CUDSTips �Ի���

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CDialogEx::SetBackgroundColor(RGB(134, 130, 167));
	m_btnOK.Init(IDB_PNG_CANCEL, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnCancel.Init(IDB_PNG_OK, BTN_IMG_1, BTN_TYPE_NORMAL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CUDSTips::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID()==IDC_STA_TIPS)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(245, 250, 6));
		CFont   tem_Font;
		tem_Font.CreatePointFont(200, _T("����"));
		pDC->SelectObject(&tem_Font);
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
// CUDSTips ��Ϣ�������


void CUDSTips::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_BDeleteAll = TRUE;
	CDialogEx::OnOK();
}


void CUDSTips::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_BDeleteAll = FALSE;
	CDialogEx::OnCancel();
}

