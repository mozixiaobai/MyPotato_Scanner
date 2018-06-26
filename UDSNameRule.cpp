// UDSNameRule.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSNameRule.h"
#include "afxdialogex.h"


// CUDSNameRule 对话框

IMPLEMENT_DYNAMIC(CUDSNameRule, CDialogEx)

CUDSNameRule::CUDSNameRule(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSNameRule::IDD, pParent)
{

	m_EditDivide = _T("");
	m_EditPre1 = _T("");
	m_EditPre2 = _T("");
	m_EditPre3 = _T("");
	m_EditBegin = _T("");
	m_SizeValue = 20;
}

CUDSNameRule::~CUDSNameRule()
{
}

void CUDSNameRule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_DATE, m_conComDate);
	DDX_Control(pDX, IDC_CMB_TIME, m_conComTime);
	DDX_Text(pDX, IDC_EDIT_DIVITION, m_EditDivide);
	DDX_Text(pDX, IDC_EDIT_PRE1, m_EditPre1);
	DDX_Text(pDX, IDC_EDIT_PRE2, m_EditPre2);
	DDX_Text(pDX, IDC_EDIT_PRE3, m_EditPre3);
	DDX_Text(pDX, IDC_EDIT_PRE4, m_EditBegin);
	DDX_Text(pDX, IDC_EDIT_PAGESIZE, m_SizeValue);
	DDV_MinMaxInt(pDX, m_SizeValue, 1, 2000);
}


BEGIN_MESSAGE_MAP(CUDSNameRule, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHK_ORDER, &CUDSNameRule::OnClickedChkOrder)
	ON_BN_CLICKED(IDC_CHK_TIME, &CUDSNameRule::OnClickedChkTime)
	ON_BN_CLICKED(IDC_CHK_CUSTOM, &CUDSNameRule::OnClickedChkCustom)
	ON_BN_CLICKED(IDOK, &CUDSNameRule::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDSNameRule::OnBnClickedCancel)
//	ON_NOTIFY(NM_THEMECHANGED, IDC_EDIT_DIVITION, &CUDSNameRule::OnThemechangedEditDivition)
ON_EN_CHANGE(IDC_EDIT_DIVITION, &CUDSNameRule::OnChangeEditDivition)
ON_BN_CLICKED(IDC_CHK_BARCODE, &CUDSNameRule::OnClickedChkBarcode)
ON_BN_CLICKED(IDC_CHK_SITE1, &CUDSNameRule::OnClickedChkSite1)
ON_BN_CLICKED(IDC_CHK_SITE2, &CUDSNameRule::OnClickedChkSite2)
ON_BN_CLICKED(IDC_CHK_SITE3, &CUDSNameRule::OnClickedChkSite3)
ON_BN_CLICKED(IDC_CHK_RESET1, &CUDSNameRule::OnClickedChkReset1)
ON_BN_CLICKED(IDC_CHK_RESET2, &CUDSNameRule::OnClickedChkReset2)
ON_BN_CLICKED(IDC_CHK_RESET3, &CUDSNameRule::OnClickedChkReset3)
ON_BN_CLICKED(IDC_BTN_RESETPAGE, &CUDSNameRule::OnBnClickedBtnResetpage)
ON_BN_CLICKED(IDC_CHK_PAGEOPEN, &CUDSNameRule::OnClickedChkPageopen)
ON_BN_CLICKED(IDC_BTN_HELP, &CUDSNameRule::OnBnClickedBtnHelp)
ON_BN_CLICKED(IDC_CHK_BARROTATE, &CUDSNameRule::OnClickedChkBarrotate)
ON_BN_CLICKED(IDC_CHK_GRAY8, &CUDSNameRule::OnClickedChkGray8)
ON_BN_CLICKED(IDC_CHK_GRAY10, &CUDSNameRule::OnClickedChkGray10)
ON_BN_CLICKED(IDC_BTN_RESETCOUNT, &CUDSNameRule::OnBnClickedBtnResetcount)
ON_BN_CLICKED(IDC_CHK_ADVANCED, &CUDSNameRule::OnClickedChkAdvanced)
ON_BN_CLICKED(IDC_CHK_BAUTOCROP, &CUDSNameRule::OnClickedChkBautocrop)
END_MESSAGE_MAP()


// CUDSNameRule 消息处理程序


BOOL CUDSNameRule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/**********************************
	*
	* 0、变量初始化
	* 1、CheckBox初始化
	* 2、Combox和Edit初始化
	* 3、Static初始化
	*
	*
	*
	*
	*
	**********************************/

	//0、变量初始化-------------------------------------------
	m_nNameDate     = 0;
	m_nNameTime     = 0;
	m_nNameBegin    = 1;
	m_nBarCode      = 1;
	m_nBarCodeRotate= 0;
	m_nPageOpen     = 0;
	m_nPageSite     = 1;
	m_nPageSize     = 60;
	m_nPageReset    = 2;
	m_nGrayBit      = 0;

	m_strNameDivide = _T("");
	m_strNamePre1   = _T("");
	m_strNamePre2   = _T("");
	m_strNamePre3   = _T("");
	m_strIniPath    = _T("");

	m_hMainWnd = AfxGetMainWnd()->m_hWnd;



	m_strIniPath  = SubGetMyDocument();
	m_strIniPath += _T("\\UDSData\\");
	m_strIniPath += _T("config.ini");

	ReadIniInfo(m_strIniPath);

	//1、CheckBox初始化---------------------------------------
	//条码分割初始化
	if (m_nBarCode==1)
	{
		((CButton*)GetDlgItem(IDC_CHK_BARCODE))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_BARCODE))->SetCheck(FALSE);
	}

	//条码旋转开关
	if (m_nBarCodeRotate==1)
	{
		((CButton*)GetDlgItem(IDC_CHK_BARROTATE))->SetCheck(TRUE);
	} 
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_BARROTATE))->SetCheck(FALSE);
	}


	if(m_nAdvancedSet==1)
	{
		((CButton*)GetDlgItem(IDC_CHK_ADVANCED))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_ADVANCED))->SetCheck(FALSE);
	}


	switch(m_nNmaeMode)
	{
	case 0:
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(FALSE);
		Self_SetCtrl(0);
		break;
	case 1:
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(FALSE);
		Self_SetCtrl(1);
		break;
	case 2:
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(TRUE);
		Self_SetCtrl(2);
		break;
	default:
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(FALSE);
		Self_SetCtrl(0);
		break;
	}

	switch(m_nPageSite)
	{
	case 0:
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(FALSE);
		break;
	case 1:
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(FALSE);
		break;
	case 2:
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(TRUE);
		break;
	default:
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(FALSE);
		break;
	}

	switch(m_nPageReset)
	{
	case 0:
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(FALSE);
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
		break;
	case 1:
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(FALSE);
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
		break;
	case 2:
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(TRUE);
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(TRUE);
		break;
	default:
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(TRUE);
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(TRUE);
		break;
	}

	if (m_nPageOpen == 1)
	{
		//开启
		((CButton*)GetDlgItem(IDC_CHK_PAGEOPEN))->SetCheck(TRUE);
		GetDlgItem(IDC_CHK_SITE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_SITE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_SITE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RESET1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RESET2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RESET3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PAGESIZE)->EnableWindow(TRUE);
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_RESET3))
		{
			GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
		}
	} 
	else
	{
		//关闭
		((CButton*)GetDlgItem(IDC_CHK_PAGEOPEN))->SetCheck(FALSE);
		GetDlgItem(IDC_CHK_SITE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_SITE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_SITE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RESET1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RESET2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RESET3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAGESIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
	}

	if (m_nGrayBit==0)
	{
		((CButton*)GetDlgItem(IDC_CHK_GRAY8))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_GRAY10))->SetCheck(FALSE);
	} 
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_GRAY8))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_GRAY10))->SetCheck(TRUE);
	}

	if (m_nAutoCrop == 1)
	{
		((CButton*)GetDlgItem(IDC_CHK_BAUTOCROP))->SetCheck(TRUE);
	} 
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_BAUTOCROP))->SetCheck(FALSE);
	}


	

	//2、Combox和Edit初始化-------------------------------------
	//1)------------------------
	m_conComDate.ResetContent();
	m_conComDate.InsertString(0, _T("年月日"));
	m_conComDate.InsertString(1, _T("年.月.日"));
	m_conComDate.InsertString(2, _T("年-月-日"));
	m_conComDate.InsertString(3, _T("年_月_日"));

	m_conComTime.ResetContent();
	m_conComTime.InsertString(0, _T("时分秒毫秒"));
	m_conComTime.InsertString(1, _T("时.分.秒.毫秒"));
	m_conComTime.InsertString(2, _T("时-分-秒-毫秒"));
	m_conComTime.InsertString(3, _T("时_分_秒_毫秒"));

	//2)------------------------
	if (m_nNameDate>=0 && m_nNameDate<=3)
	{
		m_conComDate.SetCurSel(m_nNameDate);
	}
	else
	{
		m_conComDate.SetCurSel(0);
	}

	if (m_nNameTime>=0 && m_nNameTime<=3)
	{
		m_conComTime.SetCurSel(m_nNameTime);
	}
	else
	{
		m_conComTime.SetCurSel(0);
	}

	m_EditDivide = m_strNameDivide;
	m_EditPre1   = m_strNamePre1;
	m_EditPre2   = m_strNamePre2;
	m_EditPre3   = m_strNamePre3;
	m_EditBegin.Format(_T("%d"), m_nNameBegin);
	UpdateData(FALSE);


	//3、-------------------------
	m_SizeValue = m_nPageSize;
	UpdateData(FALSE);

	CString   tem_strNameRule = _T("注：以下字符不能表示文件名 < > / \\ | : \" * ?");
	GetDlgItem(IDC_STA_NAMERULE)->SetWindowText(tem_strNameRule);




	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CUDSNameRule::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STA_NAMERULE)
	{
		CFont    tem_ftFont;
		tem_ftFont.CreatePointFont(120, _T("楷体"));
		pDC->SelectObject(&tem_ftFont);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


CString CUDSNameRule::SubGetMyDocument(void)
{
	CString        tem_strMyDocument = _T("");
	TCHAR          tem_cMyDocument[MAX_PATH] = {0};
	LPITEMIDLIST   tem_Pid = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &tem_Pid);
	if (tem_Pid && SHGetPathFromIDList(tem_Pid, tem_cMyDocument))
	{
		tem_strMyDocument = tem_cMyDocument;
		return tem_strMyDocument;
	}
	return tem_strMyDocument;
}


void CUDSNameRule::ReadIniInfo(CString inipath)
{
	int      tem_nNameInfo   = 0;
	CString  tem_strNameInfo = _T("");

	::GetPrivateProfileString("SaveSet", "NameMode", "没有找到NameMode信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nNmaeMode   = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "NameDate", "没有找到NameDate信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nNameDate   = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();
	

	::GetPrivateProfileString("SaveSet", "NameTime", "没有找到NameTime信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nNameTime   = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "NameDivide", "没有找到NameDivide信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_strNameDivide = tem_strNameInfo;
	tem_strNameInfo.ReleaseBuffer();


	::GetPrivateProfileString("SaveSet", "NamePre1", "没有找到NamePre1信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_strNamePre1 = tem_strNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "NamePre2", "没有找到NamePre2信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_strNamePre2 = tem_strNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "NamePre3", "没有找到NamePre3信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_strNamePre3 = tem_strNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "NameBegin", "没有找到NameBegin信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nNameBegin  = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "BarCodeDivide", "没有找到BarCodeDivide信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nBarCode    = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "BarCodeRotate", "没有找到BarCodeRotate信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo    = _ttoi(tem_strNameInfo);
	m_nBarCodeRotate = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "PageSite", "没有找到PageSite信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nPageSite  = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "PageSize", "没有找到PageSize信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nPageSize  = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "PageReset", "没有找到PageReset信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nPageReset  = tem_nNameInfo;
	m_nOldPageReset = m_nPageReset;   //用于最后对比，是否需要重置计数
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "PageOpen", "没有找到PageOpen信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nPageOpen  = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "GrayBit", "没有找到GrayBit信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nGrayBit    = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "AdvancedSet", "没有找到AdvancedSet信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo  = _ttoi(tem_strNameInfo);
	m_nAdvancedSet = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "AutoCropCorrect", "没有找到AutoCropCorrect信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nAutoCrop   = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();
}

//命名设置***************************************************************
void CUDSNameRule::OnClickedChkOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_ORDER))
	{
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(FALSE);
		Self_SetCtrl(0);
	}
	else
	{
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(FALSE);
		Self_SetCtrl(1);
	}
}


void CUDSNameRule::OnClickedChkTime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_TIME))
	{
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(FALSE);
		Self_SetCtrl(1);
	}
	else
	{
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_CUSTOM))->SetCheck(TRUE);
		Self_SetCtrl(2);
	}
}


void CUDSNameRule::OnClickedChkCustom()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_CUSTOM))
	{
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(FALSE);
		Self_SetCtrl(2);
	}
	else
	{
		GetDlgItem(IDC_BTN_RESETCOUNT)->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_ORDER))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_TIME))->SetCheck(FALSE);
		Self_SetCtrl(0);
	}
}


BOOL CUDSNameRule::Self_SetCtrl(int chkindex)
{
	GetDlgItem(IDC_CMB_DATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DIVITION)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMB_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PRE1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PRE2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PRE3)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PRE4)->EnableWindow(FALSE);
	switch(chkindex)
	{
	case 0:
		GetDlgItem(IDC_CMB_DATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DIVITION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE4)->EnableWindow(FALSE);
		break;
	case 1:
		GetDlgItem(IDC_CMB_DATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DIVITION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PRE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE4)->EnableWindow(FALSE);
		break;
	case 2:
		GetDlgItem(IDC_CMB_DATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DIVITION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PRE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PRE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PRE4)->EnableWindow(TRUE);
		break;
	default:
		GetDlgItem(IDC_CMB_DATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DIVITION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PRE4)->EnableWindow(FALSE);
		break;
	}
	return TRUE;
}


//命名设置***************************************************************
BOOL CUDSNameRule::SubVerityRule(CString input)
{
	/*
	int  tem_nDivide = -1;
	int  tem_nPre1   = -1;
	int  tem_nPre2   = -1;
	int  tem_nPre3   = -1;

	tem_nDivide = input.Find('\\' || '/' || ':' || '*' ||'?' ||'\"' ||'<' ||'>');
	tem_nPre1   = input.Find('\\' || '/' || ':' || '*' ||'?' ||'\"' ||'<' ||'>');
	tem_nPre2   = input.Find('\\' || '/' || ':' || '*' ||'?' ||'\"' ||'<' ||'>');
	tem_nPre3   = input.Find('\\' || '/' || ':' || '*' ||'?' ||'\"' ||'<' ||'>');
	if (tem_nDivide==-1 && tem_nPre1==-1 && tem_nPre2 && tem_nPre3==-1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	*/
	return 0;
}


void CUDSNameRule::OnChangeEditDivition()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//UpdateData(FALSE);
	//int  tem_nLength = m_EditDivide.GetLength();
	//if (tem_nLength>0)
	//{
	//	if(m_EditDivide.Right(1)==_T("\\"));
	//	{
	//		m_EditDivide.Left(tem_nLength-1);
	//		UpdateData(TRUE);
	//	}
	//}

}


//条码设置***************************************************************
void CUDSNameRule::OnClickedChkBarcode()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_BARCODE))
	{
		m_nBarCode = 1;
	}
	else
	{
		m_nBarCode = 0;
	}
}


//条码旋转
void CUDSNameRule::OnClickedChkBarrotate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_BARROTATE))
	{
		m_nBarCodeRotate = 1;
	}
	else
	{
		m_nBarCodeRotate = 0;
	}
}


//页码设置***************************************************************
void CUDSNameRule::OnClickedChkPageopen()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_PAGEOPEN))
	{
		//开启
		GetDlgItem(IDC_CHK_SITE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_SITE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_SITE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RESET1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RESET2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RESET3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PAGESIZE)->EnableWindow(TRUE);
		//获取reset状态
		if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_RESET3))
		{
			GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
		}
		m_nPageOpen = 1;
	}
	else
	{
		GetDlgItem(IDC_CHK_SITE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_SITE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_SITE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RESET1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RESET2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RESET3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAGESIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
		m_nPageOpen = 0;
	}
}


void CUDSNameRule::OnClickedChkSite1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_SITE1))
	{
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(FALSE);
		m_nPageSite = 0;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(FALSE);
		m_nPageSite = 1;
	}
}


void CUDSNameRule::OnClickedChkSite2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_SITE2))
	{
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(FALSE);
		m_nPageSite = 1;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE3))->SetCheck(TRUE);
		m_nPageSite = 2;
	}
}


void CUDSNameRule::OnClickedChkSite3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_SITE3))
	{
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(FALSE);
		m_nPageSite = 2;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_SITE1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_SITE2))->SetCheck(FALSE);
		m_nPageSite = 0;
	}
}


void CUDSNameRule::OnClickedChkReset1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_RESET1))
	{
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(FALSE);
		m_nPageReset = 0;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(FALSE);
		m_nPageReset = 1;
	}
	GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
}


void CUDSNameRule::OnClickedChkReset2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_RESET2))
	{
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(FALSE);
		m_nPageReset = 1;
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET3))->SetCheck(TRUE);
		m_nPageReset = 2;
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(TRUE);
	}
}


void CUDSNameRule::OnClickedChkReset3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_RESET3))
	{
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(FALSE);
		m_nPageReset = 2;
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHK_RESET1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_RESET2))->SetCheck(FALSE);
		m_nPageReset = 0;
		GetDlgItem(IDC_BTN_RESETPAGE)->EnableWindow(FALSE);
	}
}


void CUDSNameRule::OnBnClickedBtnResetpage()
{
	// TODO: 在此添加控件通知处理程序代码
	//写入配置文件重新计数，并发送消息，更新相应变量
	::WritePrivateProfileString("SaveSet", "PageCount", _T("0"), m_strIniPath);

	::SendMessage(m_hMainWnd, WM_READINI, 0, 0);

}


//保存设置
void CUDSNameRule::OnClickedChkGray8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED==IsDlgButtonChecked(IDC_CHK_GRAY8))
	{
		m_nGrayBit = 0;
		((CButton*)GetDlgItem(IDC_CHK_GRAY10))->SetCheck(FALSE);
	}
	else
	{
		m_nGrayBit = 1;
		((CButton*)GetDlgItem(IDC_CHK_GRAY10))->SetCheck(TRUE);
	}
}


void CUDSNameRule::OnClickedChkGray10()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED==IsDlgButtonChecked(IDC_CHK_GRAY10))
	{
		m_nGrayBit = 1;
		((CButton*)GetDlgItem(IDC_CHK_GRAY8))->SetCheck(FALSE);
	}
	else
	{
		m_nGrayBit = 0;
		((CButton*)GetDlgItem(IDC_CHK_GRAY8))->SetCheck(TRUE);
	}
}


//关闭窗口***************************************************************
void CUDSNameRule::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存配置项，写入配置文件-----------
	CString tem_strIniInfo = _T("");

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_ORDER))
	{
		tem_strIniInfo = _T("0");
	} 
	else if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_TIME))
	{
		tem_strIniInfo = _T("1");
	} 
	else if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_CUSTOM))
	{
		tem_strIniInfo = _T("2");
	} 
	else
	{
		tem_strIniInfo = _T("0");
	}
	::WritePrivateProfileString("SaveSet", "NameMode", tem_strIniInfo, m_strIniPath);

	m_nNameDate = m_conComDate.GetCurSel();
	tem_strIniInfo.Format(_T("%d"), m_nNameDate);
	::WritePrivateProfileString("SaveSet", "NameDate", tem_strIniInfo, m_strIniPath);

	m_nNameTime = m_conComTime.GetCurSel();
	tem_strIniInfo.Format(_T("%d"), m_nNameTime);
	::WritePrivateProfileString("SaveSet", "NameTime", tem_strIniInfo, m_strIniPath);

	UpdateData(TRUE);

	::WritePrivateProfileString("SaveSet", "NameDivide", m_EditDivide, m_strIniPath);
	::WritePrivateProfileString("SaveSet", "NamePre1", m_EditPre1, m_strIniPath);
	::WritePrivateProfileString("SaveSet", "NamePre2", m_EditPre2, m_strIniPath);
	::WritePrivateProfileString("SaveSet", "NamePre3", m_EditPre3, m_strIniPath);

	m_nNameBegin = _ttoi(m_EditBegin);
	if (m_nNameBegin>=0 && m_nNameBegin<=2147400000)
	{
		::WritePrivateProfileString("SaveSet", "NameBegin", m_EditBegin, m_strIniPath);
	}
	else
	{
		m_nNameBegin = 0;
		m_EditBegin  = _T("0");
		::WritePrivateProfileString("SaveSet", "NameBegin", m_EditBegin, m_strIniPath);
	}

	//条码分割
	CString  tem_strBarCode = "";
	tem_strBarCode.Format("%d", m_nBarCode);
	::WritePrivateProfileString("SaveSet", "BarCodeDivide", tem_strBarCode, m_strIniPath);

	//条码旋转
	tem_strBarCode.Format("%d", m_nBarCodeRotate);
	::WritePrivateProfileString("SaveSet", "BarCodeRotate", tem_strBarCode, m_strIniPath);

	//自动裁切
	if (m_nAutoCrop == 1)
	{
		::WritePrivateProfileString("SaveSet", "AutoCropCorrect", _T("1"), m_strIniPath);
	} 
	else
	{
		::WritePrivateProfileString("SaveSet", "AutoCropCorrect", _T("0"), m_strIniPath);
	}

	//页码设置
	CString  tem_strPage = _T("");
	tem_strPage.Format(_T("%d"), m_nPageSite);
	::WritePrivateProfileString("SaveSet", "PageSite", tem_strPage, m_strIniPath);

	tem_strPage.Format(_T("%d"), m_nPageReset);
	::WritePrivateProfileString("SaveSet", "PageReset", tem_strPage, m_strIniPath);

	UpdateData(TRUE);
	tem_strPage.Format(_T("%d"), m_SizeValue);
	::WritePrivateProfileString("SaveSet", "PageSize", tem_strPage, m_strIniPath);

	tem_strPage.Format(_T("%d"), m_nPageOpen);
	::WritePrivateProfileString("SaveSet", "PageOpen", tem_strPage, m_strIniPath);


	if (m_nOldPageReset!=1 && m_nPageReset==1)
	{
		::SendMessage(m_hMainWnd, WM_READINI, 0, 0);
	} 
	else
	{
		::SendMessage(m_hMainWnd, WM_READINI, 1, 0);
	}

	tem_strPage.Format(_T("%d"), m_nGrayBit);
	::WritePrivateProfileString("SaveSet", "GrayBit", tem_strPage, m_strIniPath);
	::SendMessage(m_hMainWnd, WM_READINI, 0, 1);

	

	CDialogEx::OnOK();
}


void CUDSNameRule::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//
	CDialogEx::OnCancel();
}


void CUDSNameRule::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlgHelpInfo.DoModal();
}



void CUDSNameRule::OnBnClickedBtnResetcount()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_hMainWnd, WM_READINI, 0, 2);
}


void CUDSNameRule::OnClickedChkAdvanced()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_ADVANCED))
	{
		::WritePrivateProfileString("SaveSet", "AdvancedSet", _T("1"), m_strIniPath);
	}
	else
	{
		::WritePrivateProfileString("SaveSet", "AdvancedSet", _T("0"), m_strIniPath);
	}
	::SendMessage(m_hMainWnd, WM_READINI, 0, 3);
}


void CUDSNameRule::OnClickedChkBautocrop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_BAUTOCROP))
	{
		m_nAutoCrop = 1;
	}
	else
	{
		m_nAutoCrop = 0;
	}
}
