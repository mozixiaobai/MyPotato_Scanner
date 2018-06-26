// UDSHelpInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSHelpInfo.h"
#include "afxdialogex.h"


// CUDSHelpInfo 对话框

IMPLEMENT_DYNAMIC(CUDSHelpInfo, CDialogEx)

CUDSHelpInfo::CUDSHelpInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSHelpInfo::IDD, pParent)
{

}

CUDSHelpInfo::~CUDSHelpInfo()
{
}

void CUDSHelpInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUDSHelpInfo, CDialogEx)
END_MESSAGE_MAP()


// CUDSHelpInfo 消息处理程序


BOOL CUDSHelpInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_strHelpInfo = _T("一：扫描重置页码：启动扫描，页码起始位重置为1；\n二：重启重置页码：每次重新打开软件，页码起始位重置为1；\n三：手动重置：页码会持续累计，直至手动重置页码为1；\n四：切换页码重置方式后，扫描重置和重启重置页码计数重置。");


	GetDlgItem(IDC_STA_HELPINFO)->SetWindowText(m_strHelpInfo);

	m_CurFont.CreatePointFont(150, _T("楷体"));
	GetDlgItem(IDC_STA_HELPINFO)->SetFont(&m_CurFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
