// UDSHelpInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDSHelpInfo.h"
#include "afxdialogex.h"


// CUDSHelpInfo �Ի���

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


// CUDSHelpInfo ��Ϣ�������


BOOL CUDSHelpInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_strHelpInfo = _T("һ��ɨ������ҳ�룺����ɨ�裬ҳ����ʼλ����Ϊ1��\n������������ҳ�룺ÿ�����´������ҳ����ʼλ����Ϊ1��\n�����ֶ����ã�ҳ�������ۼƣ�ֱ���ֶ�����ҳ��Ϊ1��\n�ģ��л�ҳ�����÷�ʽ��ɨ�����ú���������ҳ��������á�");


	GetDlgItem(IDC_STA_HELPINFO)->SetWindowText(m_strHelpInfo);

	m_CurFont.CreatePointFont(150, _T("����"));
	GetDlgItem(IDC_STA_HELPINFO)->SetFont(&m_CurFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
