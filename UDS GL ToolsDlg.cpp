
// UDS GL ToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS GL Tools.h"
#include "UDS GL ToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString   m_path_image;
extern bool    m_dsopen;
std::vector<CString>  m_FileName;
std::vector<CString>  m_WholeName;
std::vector<CString>  m_BussName;
std::vector<CString>  g_vcCurName;
std::vector<CString>  g_vcCurPath;
CString   m_strBufferPath;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUDSGLToolsDlg 对话框


CUDSGLToolsDlg::CUDSGLToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSGLToolsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDSGLToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SCANERS, m_scannername);
	DDX_Control(pDX, IDC_EDIT1, m_path_edit);
	DDX_Control(pDX, IDC_LIST_PICTURE, m_imagelist);
	DDX_Control(pDX, IDC_COMBO_FORMAT, m_format);
	DDX_Control(pDX, IDC_BUTTON_PATH, m_button_path);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_button_save);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_button_clear);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_button_start);
	DDX_Control(pDX, IDC_BUTTON_SET, m_button_set);
	DDX_Control(pDX, IDC_STATIC_PICTURE2, m_pic2);
	DDX_Control(pDX, IDC_SCROLLBAR_HS, m_scrollhs);
	DDX_Control(pDX, IDC_SCROLLBAR_VS, m_scrollvs);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_BTN_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BTN_WEB, m_btnWeb);
}

BEGIN_MESSAGE_MAP(CUDSGLToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CUDSGLToolsDlg::OnBnClickedButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO_SCANERS, &CUDSGLToolsDlg::OnSelchangeComboScaners)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CUDSGLToolsDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CUDSGLToolsDlg::OnBnClickedButtonPath)
	ON_CBN_DROPDOWN(IDC_COMBO_SCANERS, &CUDSGLToolsDlg::OnDropdownComboScaners)
	ON_CBN_DROPDOWN(IDC_COMBO_FORMAT, &CUDSGLToolsDlg::OnDropdownComboFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, &CUDSGLToolsDlg::OnSelchangeComboFormat)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CUDSGLToolsDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CUDSGLToolsDlg::OnBnClickedButtonClear)
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PICTURE, &CUDSGLToolsDlg::OnDblclkListPicture)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_CLICK, IDC_LIST_PICTURE, &CUDSGLToolsDlg::OnClickListPicture)
	ON_BN_CLICKED(IDC_CHECK_PICTURE, &CUDSGLToolsDlg::OnClickedCheckPicture)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_STN_DBLCLK(IDC_STA_PATH, &CUDSGLToolsDlg::OnDblclkStaPath)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CUDSGLToolsDlg::OnBnClickedBtnExit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MIN, &CUDSGLToolsDlg::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTN_WEB, &CUDSGLToolsDlg::OnBnClickedBtnWeb)
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_32776, &CUDSGLToolsDlg::OnCopyIt)
	ON_COMMAND(ID_32777, &CUDSGLToolsDlg::OnDeleteIt)
	ON_COMMAND(ID_32778, &CUDSGLToolsDlg::OnRename)
	ON_COMMAND(ID_32779, &CUDSGLToolsDlg::OnProperty)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PICTURE, &CUDSGLToolsDlg::OnRclickListPicture)
	ON_NOTIFY(NM_HOVER, IDC_LIST_PICTURE, &CUDSGLToolsDlg::OnHoverListPicture)
//	ON_WM_DRAWITEM()
//ON_WM_CREATE()
ON_MESSAGE(WM_READINI, &CUDSGLToolsDlg::OnReadini)
ON_BN_CLICKED(IDC_CHK_AAUTOCROP, &CUDSGLToolsDlg::OnClickedChkAautocrop)
END_MESSAGE_MAP()


// CUDSGLToolsDlg 消息处理程序

BOOL CUDSGLToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//变量初始化---------------------------------------------
	m_BDownBk   = TRUE;
	m_BMaxDlg   = TRUE;
	m_nNameMode = 0;
	m_bClosed   = true;

	m_BAdvancedSet = FALSE;

	m_nNameCounts = 0;
	m_nImgCounts  = 0;

	m_bkColor = RGB(237,234,219);

	//配置GDI+----------------------------------------
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	//按钮初始化
	m_button_start.Init(IDB_PNG_SM, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_button_set.Init(IDB_PNG_SET, BTN_IMG_4, BTN_TYPE_NORMAL);
	m_button_save.Init(IDB_PNG_MAX, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_button_clear.Init(IDB_PNG_RENAME, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnExit.Init(IDB_PNG_EXIT, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnMin.Init(IDB_PNG_MIN, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnWeb.Init(IDB_PNG_WEB, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_button_path.Init(IDB_PNG_XZLJ, BTN_IMG_3, BTN_TYPE_NORMAL);

	m_tipInfo.Create(this);
	m_tipInfo.AddTool(GetDlgItem(IDC_BUTTON_SELECT), _T("扫描"));
	m_tipInfo.AddTool(GetDlgItem(IDC_BUTTON_SET), _T("设置"));
	m_tipInfo.AddTool(GetDlgItem(IDC_BUTTON_SAVE), _T("最大化"));
	m_tipInfo.AddTool(GetDlgItem(IDC_BTN_MIN), _T("最小化"));
	m_tipInfo.AddTool(GetDlgItem(IDC_BUTTON_CLEAR), _T("软件设置"));
	m_tipInfo.AddTool(GetDlgItem(IDC_BTN_EXIT), _T("退出"));
	m_tipInfo.AddTool(GetDlgItem(IDC_BUTTON_PATH), _T("选择保存目录"));

	m_tipInfo.SetDelayTime(TTDT_INITIAL, 100);   //设置延迟
	m_tipInfo.SetDelayTime(TTDT_AUTOPOP, 5000);
	m_tipInfo.SetDelayTime(TTDT_RESHOW, 100);


	m_tipInfo.SetTipTextColor(RGB(0, 0, 0));//设置字体颜色
	m_tipInfo.SetTipBkColor(RGB(255, 255, 255));//设置提示框背景颜色

	m_tipInfo.Activate(TRUE); //启用提示功能



	CDialogEx::SetBackgroundColor(RGB(55, 150, 240)); //设置窗口背景色
	m_imagelist.SetBkColor(m_bkColor);      //设置listctrl背景色

	


// 	m_button_path.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_PATH), IMAGE_ICON, 32, 32, LR_SHARED)); 
// 	m_button_save.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SAVE), IMAGE_ICON, 32, 32, LR_SHARED));
// 	m_button_clear.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_CLEAR), IMAGE_ICON, 32, 32, LR_SHARED));
// 	m_button_start.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_START), IMAGE_ICON, 32, 32, LR_SHARED));
// 	m_button_set.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SET), IMAGE_ICON, 32, 32, LR_SHARED));

	//复制配置文件
	CheckIniFile();
	//获取配置文件
//	GetIniPath();
	m_ini_path = GetMyDocument();
	m_ini_path += _T("\\UDSData\\");
	m_strBufferPath  = m_ini_path;
	m_ini_path += _T("config.ini");

	//缩略图缓存目录
	m_strBufferPath += _T("ScanBuffer2");
	CFileFind    tem_fFileFind;
	if (!tem_fFileFind.FindFile(m_strBufferPath))
	{
		CreateDirectory(m_strBufferPath, NULL);
	}
	m_strBufferPath += _T("\\");
//	m_strBufferPath.Replace(_T("\\"), _T("\\\\"));

	CString  tem_strIniInfo = _T("");
	::GetPrivateProfileString("SaveSet","AdvancedSet","没有找到AdvancedSet信息",tem_strIniInfo.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path); 
	if (tem_strIniInfo == _T("1"))
	{
		m_BAdvancedSet = TRUE;
	}
	else
	{
		m_BAdvancedSet = FALSE;
	}
	tem_strIniInfo.ReleaseBuffer();


	//读取缓存目录，并把目录传给TWAIN
	
	CString     tem_strBufferDoc;
	::GetPrivateProfileString("Resource","curdirectory","没有找到curdirectory信息",tem_strBufferDoc.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path); 
	tem_strBufferDoc.ReleaseBuffer();
	
	if (!tem_fFileFind.FindFile(tem_strBufferDoc))
	{
		int  tem_rcCreateDoc;
		tem_rcCreateDoc = CreateDirectory(tem_strBufferDoc, NULL);
// 		if (!tem_rcCreateDoc)
// 		{
// 			return 0;
// 		}
	}
	m_path_edit.SetWindowTextA(tem_strBufferDoc);
	GetDlgItem(IDC_STA_PATH)->SetWindowText(tem_strBufferDoc);
	m_dlg_twain.m_result_path = tem_strBufferDoc+"\\";
	m_dlg_twain.m_strBarDocument = tem_strBufferDoc + "\\";
	

	::GetPrivateProfileString("LastUsedScanner","Name","没有找到Name信息",index_value2.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path); 
	index_value2.ReleaseBuffer();
	index_value2.Format("%s", index_value2);
	index_value2.MakeUpper();

	//TWAIN类读取配置文件

	m_dlg_twain.ReadIniInfo();
	m_dlg_twain.OpenDSM();
	m_dlg_twain.GetInstalledDS();

	
	int t=0;
	int tem_nUDS   = -1;
	int tem_nOther = -1;
	int tem_nFree  = -1;
	CString   tem_strScanner;
	m_scannername.ResetContent(); 	
	while(m_dlg_twain.pDSNames[t]!=_T(""))
	{      
		tem_strScanner = m_dlg_twain.pDSNames[t];
		tem_strScanner.MakeUpper();
		tem_nUDS = tem_strScanner.Find("UDS");
		tem_nOther = tem_strScanner.Find("GL");
		tem_nFree= tem_strScanner.Find("FREEIMAGE");

//		if (tem_nUDS!=-1 || tem_nOther!=-1 ||tem_nFree!=-1)
		{
			m_scannername.AddString(m_dlg_twain.pDSNames[t]);

		}	
		t++;
	}
   t = m_scannername.GetCount();  //获取条目数量用于匹配
   CString   tem_strScannerName;
   for (int i=0; i<t; i++)
   {
	   m_scannername.GetLBText(i, tem_strScannerName);
	   tem_strScannerName.MakeUpper();
	   if (index_value2 == tem_strScannerName)
	   {
		   m_scannername.SelectString(-1,index_value2);
		   index_value2.MakeUpper();
		   m_dlg_twain.SelectScanner(index_value2);
		   break;
	   }
	   
   }
	
	m_format.ResetContent(); 	
	m_format.InsertString(0, ".bmp");
	m_format.InsertString(1, ".jpg");
	m_format.InsertString(2, ".png");
	m_format.InsertString(3, ".tif");
	m_format.InsertString(4, ".pdf");
	CString m_fileformat = NULL;
//	GetIniPath();
	::GetPrivateProfileString("FileFormat","Format","没有找到Format信息",m_fileformat.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path); 
	int m_fileformat_num = _ttoi(m_fileformat);
	m_fileformat.ReleaseBuffer();
	switch(m_fileformat_num)
	{
	case 1:
		m_format.SetCurSel(0);
		m_dlg_twain.m_format_index = 1;
		break;
	case 3:
		m_format.SetCurSel(1);
		m_dlg_twain.m_format_index = 3;
		break;
	case 4:
		m_format.SetCurSel(2);
		m_dlg_twain.m_format_index = 4;
		break;
	case 6:
		m_format.SetCurSel(3);
		m_dlg_twain.m_format_index = 6;
		break;
	case 50:
		m_format.SetCurSel(4);
		m_dlg_twain.m_format_index = 50;
		break;
	default:
		m_format.SetCurSel(1);
		m_dlg_twain.m_format_index = 3;
		break;
	}
	CString m_transmode = NULL;
	CString tem_strTransType = NULL;
	::GetPrivateProfileString("ScanTransMode","TransMode","没有找到TransMode信息",m_transmode.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path);
	int m_trans_num = _ttoi(m_transmode);
	switch(m_trans_num)
	{
	case 0:
		m_dlg_twain.m_transformation = 0;
		break;
	case 1:
		::GetPrivateProfileString("ScanTransMode","TransType","没有找到TransMode信息",tem_strTransType.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path);
		m_trans_num = _ttoi(tem_strTransType);
		tem_strTransType.ReleaseBuffer();
		switch(m_trans_num)
		{
		case 0:
			//0-bmp
			m_dlg_twain.m_nTransType = 0;
			m_format.SetCurSel(0);
			break;
		case 1:
			//1-tif
			m_dlg_twain.m_nTransType = 1;
			m_format.SetCurSel(3);
			break;
		case 2:
			//2-jpg
			m_dlg_twain.m_nTransType = 2;
			m_format.SetCurSel(1);
			break;
		case 3:
			//3-png
			m_dlg_twain.m_nTransType = 3;
			m_format.SetCurSel(2);
			break;
		case 4:
			//4-pdf
			m_dlg_twain.m_nTransType = 4;
			m_format.SetCurSel(4);
			break;
		case 5:
			//base64
			m_dlg_twain.m_nTransType = 5;
			break;
		case 6:
			//gif
			m_dlg_twain.m_nTransType = 6;
			break;
		case 7:
			//muti-tif
			m_dlg_twain.m_nTransType = 7;
			break;
		case 8:
			//JPEG2000
			m_dlg_twain.m_nTransType = 8;
			break;
		default:
			m_dlg_twain.m_nTransType = 0;
			break;

		}
		m_dlg_twain.m_transformation = 1;
		break;
	default:
		m_dlg_twain.m_transformation = 0;
		break;
	}
	
	m_transmode.ReleaseBuffer();

	//自动裁切
	CString tem_strAutoCrop = NULL;
	::GetPrivateProfileString("SaveSet","AutoCropCorrect","没有找到AutoCropCorrect信息",tem_strAutoCrop.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path); 
	if (tem_strAutoCrop == "1")
	{
		m_BAutoCrop = TRUE;
	} 
	else
	{
		m_BAutoCrop = FALSE;
	}
	((CButton*)GetDlgItem(IDC_CHK_AAUTOCROP))->SetCheck(m_BAutoCrop);

    imagelist.Create(THUMB_WIDTH,THUMB_HEIGHT,ILC_COLOR24,0,1);
	m_listindex = -1;

	cx = 0;
	cy = 0;
	offsetx = 0;
	offsety = 0;
	xpos = 0;
	ypos = 0;
	m_click_mark = false;
	m_scan_mark  = false;

	m_pt.x = 0;
	m_pt.y = 0;
	n_pt.x = 0;
	n_pt.y = 0;
	m_move.x = 0;
	m_move.y = 0;
	m_save.x = 0;
	m_save.y = 0;
	m_pdfcount = 1;

	m_scrollhs.SetScrollPos(1);
	m_scrollvs.SetScrollPos(1);
	((CButton*)GetDlgItem(IDC_CHECK_PICTURE))->SetCheck(0);
	CWnd*  pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC_PICTURE2);
	pDc = pWnd->GetDC();

	CreateHSMenu();

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUDSGLToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDSGLToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		if (m_BDownBk)
		{
			/*方法一：
			//加载图像背景
			CImage Image;
			CDC *pDC = GetDC();	
			Image.Load(_T("res\\BK.png"));
			if (Image.IsNull())
			{
				MessageBox(_T("没加载成功"));
				CDialogEx::OnPaint();
				return;
			}
			if (Image.GetBPP() == 32) //确认该图像包含Alpha通道
			{
				int i;
				int j;
				for (i = 0; i < Image.GetWidth(); i++)
				{
					for (j = 0; j < Image.GetHeight(); j++)
					{
						byte *pByte = (byte *)Image.GetPixelAddress(i, j);
						pByte[0] = pByte[0] * pByte[3] / 255;
						pByte[1] = pByte[1] * pByte[3] / 255;
						pByte[2] = pByte[2] * pByte[3] / 255;
					}
				}
			}
			Image.Draw(pDC->m_hDC, 0, 0);
			Image.Destroy();
			ReleaseDC(pDC);
//			m_BDownBk = FALSE;
			*/

			//方法二：
			CImage m_imgBK;
			LoadPicture(m_imgBK, IDB_PNG_BK);
			CPaintDC dc(this);
			CDC* pDC = &dc;

			CDC dcMem;
			dcMem.CreateCompatibleDC(pDC);

			CRect rcClient;
			GetClientRect(rcClient);

			CBitmap memBitmap;
			memBitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
			dcMem.SelectObject(memBitmap);

			dcMem.FillSolidRect(rcClient, RGB(55, 150, 240));	//设置画布颜色

			if (!m_imgBK.IsNull())
			{
				CRect rcImg = CRect(0, 0, m_imgBK.GetWidth(), m_imgBK.GetHeight());
				m_imgBK.Draw(dcMem.m_hDC, rcImg, rcImg);
			}

			pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
			memBitmap.DeleteObject();
//			m_BDownBk = FALSE; //如果置为FALSE按钮状态切换会出问题	
		
		}
		//加个判断，如果vector已经没有图了，不再显示
		int  tem_nVecNum = m_WholeName.size();
		if (tem_nVecNum)
		{
			//非0， 
			CRect   tem_rcPic;
			GetDlgItem(IDC_STATIC_PICTURE2)->GetWindowRect(&tem_rcPic);
			ScreenToClient(&tem_rcPic);
			pDc->FillSolidRect(0, 0, tem_rcPic.right-tem_rcPic.left, tem_rcPic.bottom-tem_rcPic.top,m_bkColor);
			pDc->BitBlt(0,0,tem_rcPic.right,tem_rcPic.bottom,&imageDC,0,0,SRCCOPY);			
		}
		else
		{
			GetDlgItem(IDC_STATIC_PICTURE2)->GetWindowRect(&pic_rect);
			ScreenToClient(&pic_rect);
			pDc->FillSolidRect(0, 0, pic_rect.right-pic_rect.left, pic_rect.bottom-pic_rect.top,m_bkColor);
			m_pic2.SetBitmap(NULL);
		}
 		

		CDialogEx::OnPaint();
	}
// 	int t=0;
// 	m_scannername.ResetContent(); 	
// 	while(m_dlg_twain.pDSNames[t]!=_T(""))
// 	{      
// 		m_scannername.AddString(m_dlg_twain.pDSNames[t]);
// 		t++;		
// 	}
// 	m_scannername.SelectString(-1,index_value2);
// 	m_scrollhs.SetScrollPos(1);
// 	m_scrollvs.SetScrollPos(1);
// 	((CButton*)GetDlgItem(IDC_CHECK_PICTURE))->SetCheck(0);
// 	CWnd*  pWnd = NULL;
// 	pWnd = GetDlgItem(IDC_STATIC_PICTURE2);
// 	pDc = pWnd->GetDC();
}


HBRUSH CUDSGLToolsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//Static控件
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT1 || pWnd->GetDlgCtrlID()==IDC_STATIC_FMT || pWnd->GetDlgCtrlID()==IDC_STA_NOWPATH ||
		pWnd->GetDlgCtrlID() == IDC_STA_PATH || pWnd->GetDlgCtrlID() == IDC_STACHK_PIC || pWnd->GetDlgCtrlID() == IDC_STA_AAUTOCROP)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont   tem_Font;
		tem_Font.CreatePointFont(100, _T("宋体"));
		pDC->SelectObject(&tem_Font);
	}
	else if (pWnd->GetDlgCtrlID()==IDC_STA_BT)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
//		pDC->SetTextColor(RGB(174, 174, 175));
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont   tem_FontSta;
		tem_FontSta.CreatePointFont(250, _T("黑体"));
//		tem_FontSta.CreateFont(40, 30, 0, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, _T("楷体"));
		pDC->SelectObject(&tem_FontSta);
	}
	else if (pWnd->GetDlgCtrlID()==IDC_STA_BT2)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(174, 174, 175));
		CFont   tem_FontSta;
		tem_FontSta.CreateFont(20, 17, 0, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, _T("楷体"));
		pDC->SelectObject(&tem_FontSta);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CUDSGLToolsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
	{
		m_tipInfo.RelayEvent(pMsg);
	}

	CString   tem_strNewName;
	CString   tem_strBussName; 
	CString   tem_strMiddle;
	LPSTR     tem_wstrBussName;
	CString   tem_strFormat; 
	CString   tem_strPath;
	CString   tem_strNewWhole;
	CString   tem_strNewBuss;
	CFile     tem_File;

	std::vector<int>::iterator  tem_nIndex;

	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			if (m_conRename == GetFocus())
			{
				m_conRename->GetWindowText(tem_strNewName);              //显示名

				tem_strBussName = m_BussName[m_nLastSlcFile];	
				tem_strFormat   = PathFindExtension(tem_strBussName);    //后缀
				tem_strFormat.MakeLower();

				tem_strMiddle    = tem_strBussName;
				tem_wstrBussName = tem_strMiddle.GetBuffer();
				tem_strMiddle.ReleaseBuffer();
				PathRemoveFileSpec(tem_wstrBussName);   //路径
				tem_strPath     = tem_wstrBussName;     //路径				

				if (tem_strFormat == _T(".jpg")||tem_strFormat == _T(".bmp")||tem_strFormat == _T(".png")||tem_strFormat == _T(".tif"))
				{
					//显示名， 图像名，文件名

					tem_strNewWhole = tem_strPath + _T("\\") + tem_strNewName + tem_strFormat;
					tem_strNewBuss  = tem_strNewWhole;

//					int tem_rcRename = tem_File.Rename(tem_strBussName, tem_strNewBuss);
					int tem_rcRename = rename(tem_strBussName, tem_strNewBuss);
					if (tem_rcRename == 0)
					{
						//改名成功
						m_FileName[m_nLastSlcFile] = tem_strNewName;
//						m_WholeName[m_nLastSlcFile]= tem_strNewWhole;
						m_BussName[m_nLastSlcFile] = tem_strNewBuss;

						ThumbailList();
					}
					else
					{
						AfxMessageBox("文件名已存在");
					}					

				} 
				//不同于高拍仪，扫描以pdf-图像，存在一对多的情况
//  				else if (tem_strFormat == _T(".pdf"))
//  				{
//  					tem_strNewBuss = tem_strPath + _T("\\") + tem_strNewName + tem_strFormat;
//  
//  					m_FileName[m_nLastSlcFile] = tem_strNewName;
//  					m_BussName[m_nLastSlcFile] = tem_strNewBuss;
//  
//  					tem_File.Rename(tem_strBussName, tem_strNewBuss);
//  					ThumbailList();
//  				} 				
			} 
			return TRUE;
			break;
		case VK_SPACE:
			return TRUE;
			break;
		case VK_DELETE:
			if (m_listindex>=0)
			{
				//删除指定文件
				m_iTips.DoModal();
				if (m_iTips.m_BDeleteAll)
				{
					SHFILEOPSTRUCT FileOp;
					BOOL           m_BRC;    //ReturnCode操作返回码
					CString        strSrcPath;

					strSrcPath  = m_BussName[m_listindex];
					strSrcPath += '\0';

					CString  tem_strDstPath;
					tem_strDstPath = '\0';     //删除操作，直接置为NULL

					CString     tem_strTitle = "正在删除...";
					tem_strTitle += '\0';

					FileOp.fFlags = FOF_ALLOWUNDO|FOF_MULTIDESTFILES|FOF_SIMPLEPROGRESS|FOF_NOCONFIRMATION;

					FileOp.lpszProgressTitle = tem_strTitle;
					FileOp.hwnd              = m_hWnd;
					FileOp.hNameMappings     = NULL;
					FileOp.pFrom             = strSrcPath;
					FileOp.pTo               = tem_strDstPath;    //目标目录
					FileOp.wFunc             = FO_DELETE;

					//弹窗提示--------------------------------------------------
					m_BRC = SHFileOperation(&FileOp);
					if (!FileOp.fAnyOperationsAborted)
					{
						if(m_BRC)   
						{
							//操作出现错误
						}		
					}
					else
					{
						//出现意外中止
					}				
					

					
					//删除vector中信息

					m_FileName.erase(m_FileName.begin()+m_listindex);
					m_WholeName.erase(m_WholeName.begin()+m_listindex);
					m_BussName.erase(m_BussName.begin()+m_listindex);
					ThumbailList();
					//刷新显示
					//判断ListCtrl是否还有图像
					int  tem_nVecNum = m_WholeName.size();
					if (tem_nVecNum)
					{
						//非0， 
						m_nSlctItem = tem_nVecNum-1;
						CString   tem_strLastPic = m_BussName.back();

						//判断是否为PDF图像
						int      tem_nFormat   = 0;
						CString  tem_strFormat = tem_strLastPic;
						tem_nFormat   = tem_strFormat.ReverseFind('.');
						tem_strFormat = tem_strFormat.Mid(tem_nFormat+1);
						tem_strFormat.MakeLower();
						if (tem_strFormat == _T("pdf"))
						{
							tem_strLastPic = m_WholeName[m_nSlctItem];
							ShowWpicture(tem_strLastPic);
						}
						else
						{
							ShowWpicture(tem_strLastPic);  //0829
						}

						
					}
					else
					{
						//0个元素,清空picture控件的显示
						m_nSlctItem = tem_nVecNum-1;
						GetDlgItem(IDC_STATIC_PICTURE2)->GetWindowRect(&pic_rect);
						ScreenToClient(&pic_rect);

						pDc->FillSolidRect(0, 0, pic_rect.right-pic_rect.left, pic_rect.bottom-pic_rect.top,m_bkColor);
						m_pic2.SetBitmap(NULL);

					}
					
				}

			}
			else
			{
				m_vcCtrlIndex.clear();
				POSITION   pos;
				pos = m_imagelist.GetFirstSelectedItemPosition();
				while (pos)
				{
					int  tem_nIndex = (int)pos-1;
					m_vcCtrlIndex.push_back(tem_nIndex);
					m_imagelist.GetNextSelectedItem(pos);		
				}
				if (m_vcCtrlIndex.size()>0)
				{
					//逐项删除
					OnDeleteIt();
					

				}
			}
			m_listindex = -1;
			
			break;
		case VK_ESCAPE:
			OnBnClickedBtnExit();
			break;
		case 0x41:
			//A键=0x41
			if (GetDlgItem(IDC_LIST_PICTURE)->GetSafeHwnd()==::GetFocus()&& (GetKeyState(VK_CONTROL)&0x8000))
			{
				m_vcCtrlIndex.clear();
				m_listindex = -1;
				for (int i=0; i<m_imagelist.GetItemCount(); i++)
				{
					m_vcCtrlIndex.push_back(i);
					//选中并高亮该项
					m_imagelist.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				}

			}
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUDSGLToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUDSGLToolsDlg::OnDropdownComboScaners()
{
//	m_dlg_twain.GetInstalledDS();
/*
	int t=0;
	int tem_nUDS   = -1;
	int tem_nOther = -1;
	int tem_nFree  = -1;
	CString   tem_strScanner;
	m_scannername.ResetContent(); 	
	while(m_dlg_twain.pDSNames[t]!=_T(""))
	{      
		tem_strScanner = m_dlg_twain.pDSNames[t];
		tem_strScanner.MakeUpper();
		tem_nUDS = tem_strScanner.Find("UDS");
		tem_nOther = tem_strScanner.Find("GL");
		tem_nFree= tem_strScanner.Find("FREEIMAGE");

		if (tem_nUDS!=-1 || tem_nOther!=-1 ||tem_nFree!=-1)
		{
			m_scannername.AddString(m_dlg_twain.pDSNames[t]);
				
		}	
		t++;
	}
	*/
}

void CUDSGLToolsDlg::OnSelchangeComboScaners()
{
	index = m_scannername.GetCurSel(); 
	m_scannername.GetLBText(index,index_value);
	index_value.MakeUpper();
	m_dlg_twain.SelectScanner(index_value);
//	m_dlg_twain.OpenDS();
	::WritePrivateProfileString("LastUsedScanner", "Name", index_value, m_ini_path);	
}

void CUDSGLToolsDlg::OnDropdownComboFormat()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CUDSGLToolsDlg::OnSelchangeComboFormat()
{
	int index = m_format.GetCurSel();
	CString   m_fileformat = NULL;
	switch(index) 
	{
	case 0:
		m_dlg_twain.m_format_index = 1;
		m_fileformat.Format("%d", 1);
		break;
	case 1:
		m_dlg_twain.m_format_index = 3;
		m_fileformat.Format("%d", 3);
		break;
	case 2:
		m_dlg_twain.m_format_index = 4;
		m_fileformat.Format("%d", 4);
		break;
	case 3:
		m_dlg_twain.m_format_index = 6;
		m_fileformat.Format("%d", 6);
		break;
	case 4:
		m_dlg_twain.m_format_index = 50;
		m_fileformat.Format("%d", 50);
		break;
	default:
		break;
	}
	::WritePrivateProfileString("FileFormat", "Format", m_fileformat, m_ini_path);
}

void CUDSGLToolsDlg::OnBnClickedButtonStart()
{
	m_dlg_twain.OpenDS();
	if (!m_dsopen)
	{
		AfxMessageBox("当前扫描仪未准备好，请重新选择！");
		return ;
	}
	else
	{
		g_vcCurName.clear();
		g_vcCurPath.clear();
		if (m_dlg_twain.m_nPageReset==0)
		{
			//每次扫描重置页码
			m_dlg_twain.m_nPageCount = 1;
		}

		m_dlg_twain.m_strBarDocument = m_dlg_twain.m_result_path;
		int tem_nPageBegin = m_dlg_twain.m_totalimage;

		if (m_BAdvancedSet)
		{
			m_dlg_twain.m_ScanInfo.bIsShowUI = true;
		}
			
		int tem_nRC        = m_dlg_twain.EnableDS();

		m_dlg_twain.m_ScanInfo.bIsShowUI = false;


		if (tem_nRC>0)
		{
			
//			ThumbailList();
			//----------------------------------------------------------
			ThumbailList(g_vcCurName, g_vcCurPath);
			//----------------------------------------------------------
			int  tem_nVecNum = m_WholeName.size();
			if (tem_nVecNum!=0 && m_dlg_twain.m_format_index != 50)
			{
				//非0， 如果为图像文件加载m_BussName,如果为PDF图像加载
				CString   tem_strLastPic = m_BussName.back();   //0829加载大图

				ShowWpicture(tem_strLastPic);				
			}
			else if (tem_nVecNum!=0 && m_dlg_twain.m_format_index == 50)
			{
				//判断是否为PDF文件，若为PDF文件则加载图像文件
				CString   tem_strLastPic = m_WholeName.back();   //0829加载大图

				ShowWpicture(tem_strLastPic);	
			}
			
			m_listindex = nIndex-1;
			m_scan_mark = true;
			if (m_dlg_twain.m_format_index == 50)
			{
				GerneratePdf(tem_nPageBegin);
			}
		}
		
	}
	m_dlg_twain.CloseDS();
}

void CUDSGLToolsDlg::OnBnClickedButtonSet()
{
	m_button_set.EnableWindow(FALSE);
	m_dlg_twain.OpenDS();
	if (!m_dsopen)
	{
		AfxMessageBox("当前扫描仪未准备好，请重新选择！");
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);
		return ;
	}
	else
	{
		m_dlg_twain.m_ScanInfo.bIsShowUI=true;
		Self_DisableCtrls(true);
		m_dlg_twain.EnableDSOnly();
		Self_DisableCtrls(false);
		m_dlg_twain.m_ScanInfo.bIsShowUI=false;
	}
	m_bClosed = m_dlg_twain.CloseDS();
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);
}

CString CUDSGLToolsDlg::DefaultPath(void)
{
	char pFolderName[MAX_PATH]; 
	int nPos = GetCurrentDirectory( MAX_PATH, pFolderName); 
	CFileFind   m_filefind;
	CString     m_scanresult(pFolderName);
	m_scanresult += "\\ScanTestResult";	
	if(!m_filefind.FindFile(m_scanresult))
	{
		CreateDirectory(m_scanresult,NULL);    //创建缓存目录
	}
	return m_scanresult;
}

void CUDSGLToolsDlg::OnDblclkStaPath()
{
	// TODO: 在此添加控件通知处理程序代码
	char     m_selectpath[MAX_PATH];
	CString  m_goalpath;
	ZeroMemory(m_selectpath, sizeof(m_selectpath));
	BROWSEINFO   m_broseinfo;
	m_broseinfo.hwndOwner = m_hWnd;
	m_broseinfo.pidlRoot  = NULL;
	m_broseinfo.pszDisplayName = m_selectpath;
	m_broseinfo.lpszTitle = "请选择目录"; 
	m_broseinfo.ulFlags   = 0;
	m_broseinfo.lpfn      = NULL;
	m_broseinfo.lParam    = 0;
	m_broseinfo.iImage    = 0;
	LPITEMIDLIST   lp = SHBrowseForFolder(&m_broseinfo);
	if (lp&&SHGetPathFromIDList(lp, m_selectpath))
	{
		m_goalpath.Format("%s", m_selectpath);
//		m_goalpath.Replace("\\", "\\\\");
		GetDlgItem(IDC_STA_PATH)->SetWindowText(m_goalpath);
		CRect rtlbl; 
		GetDlgItem(IDC_STA_PATH)->GetWindowRect(&rtlbl); 
		ScreenToClient(&rtlbl); //转到客户端界面
		InvalidateRect(&rtlbl);//最后刷新对话框背景 
		UpdateWindow();
		//写入配置文件
		::WritePrivateProfileString("Resource", "curdirectory", m_goalpath, m_ini_path);
		m_goalpath =m_goalpath+"\\";
		m_dlg_twain.m_result_path = m_goalpath;	
	} 
	else
	{
		AfxMessageBox("选择目录无效");
	}	
}


void CUDSGLToolsDlg::OnBnClickedButtonPath()
{
	// TODO: 在此添加控件通知处理程序代码
	char     m_selectpath[MAX_PATH];
	CString  m_goalpath;
	ZeroMemory(m_selectpath, sizeof(m_selectpath));
	BROWSEINFO   m_broseinfo;
	m_broseinfo.hwndOwner = m_hWnd;
	m_broseinfo.pidlRoot  = NULL;
	m_broseinfo.pszDisplayName = m_selectpath;
	m_broseinfo.lpszTitle = "请选择目录"; 
	m_broseinfo.ulFlags   = 0;
	m_broseinfo.lpfn      = NULL;
	m_broseinfo.lParam    = 0;
	m_broseinfo.iImage    = 0;
	LPITEMIDLIST   lp = SHBrowseForFolder(&m_broseinfo);
	if (lp&&SHGetPathFromIDList(lp, m_selectpath))
	{
		m_goalpath.Format("%s", m_selectpath);
		
		GetDlgItem(IDC_STA_PATH)->SetWindowText(m_goalpath);
		CRect rtlbl; 
		GetDlgItem(IDC_STA_PATH)->GetWindowRect(&rtlbl); 
		ScreenToClient(&rtlbl); //转到客户端界面
		InvalidateRect(&rtlbl);//最后刷新对话框背景 
		UpdateWindow();
		//写入配置文件
		::WritePrivateProfileString("Resource", "curdirectory", m_goalpath, m_ini_path);
//		m_goalpath.Replace("\\", "\\\\");
//		MessageBox(m_ini_path);
		m_goalpath =m_goalpath+"\\";
		m_dlg_twain.m_result_path = m_goalpath;	
		m_dlg_twain.m_strBarDocument = m_goalpath;
	} 
// 	else
// 	{
// 		AfxMessageBox("选择目录无效");
// 	}	
}


bool CUDSGLToolsDlg::ThumbailList(void)
{
	DWORD dwStyle;
	dwStyle = m_imagelist.GetExtendedStyle();
	dwStyle=dwStyle|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_AUTOARRANGE;
	m_imagelist.SetExtendedStyle(dwStyle);
	m_imagelist.SetImageList(&imagelist,LVSIL_NORMAL);
	for(int i=0;i<imagelist.GetImageCount();i++)
		imagelist.Remove(i);

	m_imagelist.DeleteAllItems();

	imagelist.SetImageCount(m_FileName.size());

	char path[MAX_PATH];
	std::vector<CString>::iterator iter;
	std::vector<CString>::iterator iter2;
	m_imagelist.SetRedraw(false);
	nIndex=0;
	m_imagelist.SetTextBkColor(m_bkColor);
	m_imagelist.SetTextColor(RGB(255, 255, 255));
	for(iter=m_FileName.begin();iter!=m_FileName.end();iter++,nIndex++)
	{
		m_imagelist.InsertItem(nIndex,*iter,nIndex);
	}
	m_imagelist.SetRedraw(true);
	m_imagelist.Invalidate();

	HBRUSH hBrushBorder    =::CreateSolidBrush(m_bkColor);
	HBRUSH hBrushBkground  =::CreateSolidBrush(m_bkColor);

	RECT rcBorder;
	rcBorder.left=rcBorder.top  =0;
	rcBorder.right              =THUMB_WIDTH;
	rcBorder.bottom             =THUMB_HEIGHT;

	float fRatio=(float)THUMB_HEIGHT/THUMB_WIDTH; 

	int XDest,YDest,nDestWidth,nDestHeight;
	nIndex=0;

	for(iter2=m_WholeName.begin();iter2!=m_WholeName.end();iter2++,nIndex++)
	{
		sprintf(path,*iter2);
		int nImageType=GetTypeFromFileName(path);
		if(nImageType==CXIMAGE_FORMAT_UNKNOWN)
			continue;
		CxImage image(path,nImageType);
		if(image.IsValid()==false)
			continue;
		float fImgRatio=(float)image.GetHeight()/image.GetWidth();  
		if(fImgRatio>fRatio)
		{
			nDestWidth=(int)THUMB_HEIGHT/fImgRatio;   
			XDest=(THUMB_WIDTH-nDestWidth)/2;
			YDest=0;
			nDestHeight=THUMB_HEIGHT;
		}
		else
		{
			XDest=0;
			nDestWidth=THUMB_WIDTH;
			nDestHeight=(int)THUMB_WIDTH*fImgRatio;  
			YDest=(THUMB_HEIGHT-nDestHeight)/2;
		}

		CClientDC cdc(this);
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
		HBITMAP bm=CreateCompatibleBitmap(cdc.m_hDC,THUMB_WIDTH,THUMB_HEIGHT);
		HBITMAP pOldBitmapImage=(HBITMAP)SelectObject(hDC,bm);

		::FillRect(hDC,&rcBorder,hBrushBkground);
		image.Stretch(hDC,XDest,YDest,nDestWidth,nDestHeight);
		::FrameRect(hDC,&rcBorder,hBrushBorder);
		SelectObject(hDC,pOldBitmapImage);
		CBitmap bitmap;
		bitmap.Attach(bm);
		imagelist.Replace(nIndex,&bitmap,NULL);
		m_imagelist.RedrawItems(nIndex,nIndex);
		DeleteDC(hDC);
		DeleteObject(bm);
		m_imagelist.Invalidate();
		
	}
	/*
	for(iter=m_FileName.begin();iter!=m_FileName.end();iter++,nIndex++)
	{
		sprintf(path,m_dlg_twain.m_result_path+*iter);

		AllPath[nIndex]     = path;
		AllFilename[nIndex] = *iter;
		int nImageType=GetTypeFromFileName(path);
		if(nImageType==CXIMAGE_FORMAT_UNKNOWN)
			continue;
		CxImage image(path,nImageType);
		if(image.IsValid()==false)
			continue;
		float fImgRatio=(float)image.GetHeight()/image.GetWidth();  
		if(fImgRatio>fRatio)
		{
			nDestWidth=(int)THUMB_HEIGHT/fImgRatio;   
			XDest=(THUMB_WIDTH-nDestWidth)/2;
			YDest=0;
			nDestHeight=THUMB_HEIGHT;
		}
		else
		{
			XDest=0;
			nDestWidth=THUMB_WIDTH;
			nDestHeight=(int)THUMB_WIDTH*fImgRatio;  
			YDest=(THUMB_HEIGHT-nDestHeight)/2;
		}

		CClientDC cdc(this);
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
		HBITMAP bm=CreateCompatibleBitmap(cdc.m_hDC,THUMB_WIDTH,THUMB_HEIGHT);
		HBITMAP pOldBitmapImage=(HBITMAP)SelectObject(hDC,bm);

		::FillRect(hDC,&rcBorder,hBrushBkground);
		image.Stretch(hDC,XDest,YDest,nDestWidth,nDestHeight);
		::FrameRect(hDC,&rcBorder,hBrushBorder);
		SelectObject(hDC,pOldBitmapImage);
		CBitmap bitmap;
		bitmap.Attach(bm);
		imagelist.Replace(nIndex,&bitmap,NULL);
		m_imagelist.RedrawItems(nIndex,nIndex);
		DeleteDC(hDC);
		DeleteObject(bm);
	}
	*/
	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBkground);
	int  tem_nCount = m_imagelist.GetItemCount();
	if (tem_nCount>0)
	{
		m_imagelist.EnsureVisible(tem_nCount-1, FALSE);
	}
	m_imagelist.Invalidate();

	return true;
}


bool CUDSGLToolsDlg::ThumbailList(std::vector<CString> curname, std::vector<CString> curpath)
{
    DWORD dwStyle;
	dwStyle = m_imagelist.GetExtendedStyle();
	dwStyle=dwStyle|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_AUTOARRANGE;
	m_imagelist.SetExtendedStyle(dwStyle);
	m_imagelist.SetImageList(&imagelist,LVSIL_NORMAL);

//	imagelist.SetImageCount(g_vcCurName.size());
	imagelist.SetImageCount(m_FileName.size());

	char path[MAX_PATH];
	std::vector<CString>::iterator iter;
	std::vector<CString>::iterator iter2;
	m_imagelist.SetRedraw(false);
	nIndex=0;
	m_imagelist.SetTextBkColor(m_bkColor);
	m_imagelist.SetTextColor(RGB(0, 0, 0));
	for(iter=g_vcCurName.begin();iter!=g_vcCurName.end();iter++,m_nNameCounts++)
	{
		m_imagelist.InsertItem(m_nNameCounts,*iter,m_nNameCounts);
	}
	m_imagelist.SetRedraw(true);
	m_imagelist.Invalidate();

	
	HBRUSH hBrushBorder    =::CreateSolidBrush(m_bkColor);
	HBRUSH hBrushBkground  =::CreateSolidBrush(m_bkColor);

	RECT rcBorder;
	rcBorder.left=rcBorder.top  =0;
	rcBorder.right              =THUMB_WIDTH;
	rcBorder.bottom             =THUMB_HEIGHT;

	float fRatio=(float)THUMB_HEIGHT/THUMB_WIDTH; 

	int XDest,YDest,nDestWidth,nDestHeight;
	nIndex=0;

	for(iter2=g_vcCurPath.begin();iter2!=g_vcCurPath.end();iter2++,m_nImgCounts++, nIndex++)
	{
		sprintf(path,*iter2);
		int nImageType=GetTypeFromFileName(path);
		if(nImageType==CXIMAGE_FORMAT_UNKNOWN)
			continue;
		CxImage image(path,nImageType);
		if(image.IsValid()==false)
			continue;
		float fImgRatio=(float)image.GetHeight()/image.GetWidth();  
		if(fImgRatio>fRatio)
		{
			nDestWidth=(int)THUMB_HEIGHT/fImgRatio;   
			XDest=(THUMB_WIDTH-nDestWidth)/2;
			YDest=0;
			nDestHeight=THUMB_HEIGHT;
		}
		else
		{
			XDest=0;
			nDestWidth=THUMB_WIDTH;
			nDestHeight=(int)THUMB_WIDTH*fImgRatio;  
			YDest=(THUMB_HEIGHT-nDestHeight)/2;
		}

		CClientDC cdc(this);
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
		HBITMAP bm=CreateCompatibleBitmap(cdc.m_hDC,THUMB_WIDTH,THUMB_HEIGHT);
		HBITMAP pOldBitmapImage=(HBITMAP)SelectObject(hDC,bm);

		::FillRect(hDC,&rcBorder,hBrushBkground);
		image.Stretch(hDC,XDest,YDest,nDestWidth,nDestHeight);
		::FrameRect(hDC,&rcBorder,hBrushBorder);
		SelectObject(hDC,pOldBitmapImage);
		CBitmap bitmap;
		bitmap.Attach(bm);
//		imagelist.Replace(nIndex,&bitmap,NULL);
		imagelist.Replace(m_nImgCounts,&bitmap,NULL);
		m_imagelist.RedrawItems(m_nImgCounts,m_nImgCounts);
		DeleteDC(hDC);
		DeleteObject(bm);
		m_imagelist.Invalidate();
	}
	
	nIndex = m_FileName.size();

	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBkground);
	int  tem_nCount = m_imagelist.GetItemCount();
	if (tem_nCount>0)
	{
		m_imagelist.EnsureVisible(tem_nCount-1, FALSE);
	}
	m_imagelist.Invalidate();
	return true;
}


int CUDSGLToolsDlg::GetTypeFromFileName(LPCTSTR pstr)
{
	CString fileName(pstr);
	CString ext3=fileName.Right(3);
	CString ext4=fileName.Right(4);
#if CXIMAGE_SUPPORT_BMP
	if(ext3.CompareNoCase(_T("bmp"))==0)
		return CXIMAGE_FORMAT_BMP;
#endif

#if CXIMAGE_SUPPORT_GIF
	if(ext3.CompareNoCase(_T("gif"))==0)
		return CXIMAGE_FORMAT_GIF;
#endif

#if CXIMAGE_SUPPORT_JPG
	if(ext3.CompareNoCase(_T("jpg"))==0 || ext4.CompareNoCase(_T("jpeg"))==0)
		return CXIMAGE_FORMAT_JPG;
#endif

#if CXIMAGE_SUPPORT_PNG
	if(ext3.CompareNoCase(_T("png"))==0)
		return CXIMAGE_FORMAT_PNG;
#endif

#if CXIMAGE_SUPPORT_MNG
	if(ext3.CompareNoCase(_T("mng"))==0 || ext3.CompareNoCase(_T("jng"))==0 ||ext3.CompareNoCase(_T("png"))==0)
		return CXIMAGE_FORMAT_MNG;
#endif

#if CXIMAGE_SUPPORT_ICO
	if(ext3.CompareNoCase(_T("ico"))==0)
		return CXIMAGE_FORMAT_ICO;
#endif

#if CXIMAGE_SUPPORT_TIF
	if(ext3.CompareNoCase(_T("tif"))==0 || ext4.CompareNoCase(_T("tiff"))==0)
		return CXIMAGE_FORMAT_TIF;
#endif

#if CXIMAGE_SUPPORT_TGA
	if(ext3.CompareNoCase(_T("tga"))==0)
		return CXIMAGE_FORMAT_TGA;
#endif

#if CXIMAGE_SUPPORT_PCX
	if(ext3.CompareNoCase(_T("pcx"))==0)
		return CXIMAGE_FORMAT_PCX;
#endif

#if CXIMAGE_SUPPORT_WBMP
	if(ext4.CompareNoCase(_T("wbmp"))==0)
		return CXIMAGE_FORMAT_WBMP;
#endif

#if CXIMAGE_SUPPORT_WMF
	if(ext3.CompareNoCase(_T("wmf"))==0 || ext3.CompareNoCase(_T("emf"))==0)
		return CXIMAGE_FORMAT_WMF;
#endif

#if CXIMAGE_SUPPORT_J2K
	if(ext3.CompareNoCase(_T("j2k"))==0 || ext3.CompareNoCase(_T("jp2"))==0)
		return CXIMAGE_FORMAT_J2K;
#endif

#if CXIMAGE_SUPPORT_JBG
	if(ext3.CompareNoCase(_T("jbg"))==0)
		return CXIMAGE_FORMAT_JBG;
#endif

#if CXIMAGE_SUPPORT_JP2
	if(ext3.CompareNoCase(_T("j2k"))==0 || ext3.CompareNoCase(_T("jp2"))==0)
		return CXIMAGE_FORMAT_JP2;
#endif

#if CXIMAGE_SUPPORT_JPC
	if(ext3.CompareNoCase(_T("j2c"))==0 || ext3.CompareNoCase(_T("jpc"))==0)
		return CXIMAGE_FORMAT_JPC;
#endif

#if CXIMAGE_SUPPORT_PGX
	if(ext3.CompareNoCase(_T("pgx"))==0)
		return CXIMAGE_FORMAT_PGX;
#endif

#if CXIMAGE_SUPPORT_PNM
	if(ext3.CompareNoCase(_T("pnm"))==0 || ext3.CompareNoCase(_T("pgm"))==0 || ext3.CompareNoCase(_T("ppm"))==0)
		return CXIMAGE_FORMAT_PNM;
#endif

#if CXIMAGE_SUPPORT_RAS
	if(ext3.CompareNoCase(_T("ras"))==0)
		return CXIMAGE_FORMAT_RAS;
#endif

	return CXIMAGE_FORMAT_UNKNOWN;
}

CString CUDSGLToolsDlg::GetIniPath(void)
{
	/*6-23原来的为GetCurrent与GetModule有一定差别
	char m_path[MAX_PATH]; 
	GetCurrentDirectory( MAX_PATH, m_path); 
	CString m_full_path(m_path);
//	m_full_path+= "\\DriverTest.ini";
	m_full_path += "\\config.ini";    //6-23，统一修改为config配置文件
	m_ini_path = m_full_path;
	return m_ini_path;
	*/

	TCHAR        tem_tcFilepath[MAX_PATH+1] = {0};
	GetModuleFileName(NULL, tem_tcFilepath, MAX_PATH);
	(_tcsrchr(tem_tcFilepath, _T('\\')))[1] = 0;
	m_ini_path    = tem_tcFilepath;
	m_ini_path   += _T("config.ini");
	return m_ini_path;
}

void CUDSGLToolsDlg::OnBnClickedButtonClear()
{
	/*1031修改为软件设置
	//对话框提示
	m_iTips.DoModal();
	if (m_iTips.m_BDeleteAll)
	{
		//删除缩略图像
		for(int i=0;i<imagelist.GetImageCount();i++)
			imagelist.Remove(i);

		ClearImage();
		m_imagelist.DeleteAllItems();
		m_dlg_twain.m_totalimage = 0;
		m_FileName.clear();
		m_WholeName.clear();
		//	ClearImage();
		pDc->FillSolidRect(0, 0, pic_rect.right-pic_rect.left, pic_rect.bottom-pic_rect.top,RGB(72,77,91));
		m_click_mark = false;
		m_scan_mark  = false;
		//	GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(TRUE);
	}
	*/


	//1、命名设置---------------------------------------
	if (IDOK==m_dlg_name.DoModal())
	{
		//同步自动裁切状态
		CString tem_strAutoCrop = NULL;
		::GetPrivateProfileString("SaveSet","AutoCropCorrect","没有找到AutoCropCorrect信息",tem_strAutoCrop.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path); 
		if (tem_strAutoCrop == "1")
		{
			m_BAutoCrop = TRUE;
		} 
		else
		{
			m_BAutoCrop = FALSE;
		}
		((CButton*)GetDlgItem(IDC_CHK_AAUTOCROP))->SetCheck(m_BAutoCrop);


		m_dlg_twain.ReadIniInfo();
	} 	
	
}

//暂时未用
void CUDSGLToolsDlg::OnBnClickedButtonSave()
{	
	/*
	if (m_dlg_twain.m_format_index == 50)
	{
	//	GerneratePdf();
		ClearImage();
	}
	//删除缩略图像
	for(int i=0;i<imagelist.GetImageCount();i++)
		imagelist.Remove(i);

	m_imagelist.DeleteAllItems();
	m_dlg_twain.m_totalimage = 0;
	m_FileName.clear();

	pDc->FillSolidRect(0, 0, pic_rect.right-pic_rect.left, pic_rect.bottom-pic_rect.top,RGB(220,220,220));
	m_click_mark = false;
	m_scan_mark  = false;

	*/
	//IsZoomed()可用来判断窗口是否最大化
	
	if (m_BMaxDlg)
	{
		GetWindowRect(&m_rcRectSrc);
    	GetDlgItem(IDC_STA_BT)->GetWindowRect(&m_rcBT);

		GetDlgItem(IDC_BUTTON_SAVE)->GetWindowRect(&m_rcMax);
		GetDlgItem(IDC_BTN_MIN)->GetWindowRect(&m_rcMin);
		GetDlgItem(IDC_BTN_EXIT)->GetWindowRect(&m_rcExit);

		GetDlgItem(IDC_STATIC_TEXT1)->GetWindowRect(&m_rcSlcScanner);
		GetDlgItem(IDC_STATIC_FMT)->GetWindowRect(&m_rcSlcFmt);
		GetDlgItem(IDC_COMBO_SCANERS)->GetWindowRect(&m_rcComScanner);
		GetDlgItem(IDC_COMBO_FORMAT)->GetWindowRect(&m_rcComFmt);

		GetDlgItem(IDC_CHECK_PICTURE)->GetWindowRect(&m_rcChkPic);
		GetDlgItem(IDC_STACHK_PIC)->GetWindowRect(&m_rcPic);

		GetDlgItem(IDC_BUTTON_SELECT)->GetWindowRect(&m_rcScan);
		GetDlgItem(IDC_BUTTON_SET)->GetWindowRect(&m_rcSet);
		GetDlgItem(IDC_BUTTON_CLEAR)->GetWindowRect(&m_rcRename);
		GetDlgItem(IDC_SCROLLBAR_VS)->GetWindowRect(&m_rcVScroll);
		GetDlgItem(IDC_SCROLLBAR_HS)->GetWindowRect(&m_rcHScroll);
		GetDlgItem(IDC_STATIC_PICTURE2)->GetWindowRect(&m_rcWPic);
		GetDlgItem(IDC_LIST_PICTURE)->GetWindowRect(&m_rcList);

		GetDlgItem(IDC_BUTTON_PATH)->GetWindowRect(&m_rcSlcPath);
		GetDlgItem(IDC_STA_NOWPATH)->GetWindowRect(&m_rcNowPath);
		GetDlgItem(IDC_STA_PATH)->GetWindowRect(&m_rcShowPath);

		GetDlgItem(IDC_CHK_AAUTOCROP)->GetWindowRect(&m_rcAutoCropS);
		GetDlgItem(IDC_STA_AAUTOCROP)->GetWindowRect(&m_rcAutoCropC);
		

		

		CRect rcWorkArea; 
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0); 

		//重新定义各个控件的位置
		//1、标题位置
		GetDlgItem(IDC_STA_BT)->ShowWindow(SW_HIDE);
		CRect  tem_rcControl;
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 20;
		tem_rcControl.left   = 20;
		tem_rcControl.right  = 500;
		tem_rcControl.bottom = 100;
		GetDlgItem(IDC_STA_BT)->MoveWindow(&tem_rcControl);

		//2、三个小图标
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 5;	
		tem_rcControl.right  -= 10;
		tem_rcControl.left   = tem_rcControl.right-16;
		tem_rcControl.bottom = 21;
		GetDlgItem(IDC_BTN_EXIT)->MoveWindow(&tem_rcControl);
		
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 26;	
		tem_rcControl.right  -= 10;
		tem_rcControl.left   = tem_rcControl.right-16;
		tem_rcControl.bottom = 42;
		GetDlgItem(IDC_BUTTON_SAVE)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 47;	
		tem_rcControl.right  -= 10;
		tem_rcControl.left   = tem_rcControl.right-16;
		tem_rcControl.bottom = 63;
		GetDlgItem(IDC_BTN_MIN)->MoveWindow(&tem_rcControl);
		

		//3、两个大按钮
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 5;	
		tem_rcControl.right  -= 40;
		tem_rcControl.left   = tem_rcControl.right-48;
		tem_rcControl.bottom = 53;
		GetDlgItem(IDC_BUTTON_CLEAR)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 5;	
		tem_rcControl.right  -= 100;
		tem_rcControl.left   = tem_rcControl.right-48;
		tem_rcControl.bottom = 53;
		GetDlgItem(IDC_BUTTON_SET)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 5;	
		tem_rcControl.right  -= 160;
		tem_rcControl.left   = tem_rcControl.right-48;
		tem_rcControl.bottom = 53;
		GetDlgItem(IDC_BUTTON_SELECT)->MoveWindow(&tem_rcControl);

		//4、ComboBox控件等
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 10;	
		tem_rcControl.right -= 250;
		tem_rcControl.left   = tem_rcControl.right-250;
		GetDlgItem(IDC_COMBO_SCANERS)->MoveWindow(&tem_rcControl);

		tem_rcControl.top    = 45;			
		tem_rcControl.right  = tem_rcControl.left + 70;
		GetDlgItem(IDC_COMBO_FORMAT)->MoveWindow(&tem_rcControl);

		GetDlgItem(IDC_STACHK_PIC)->ShowWindow(SW_HIDE);   //避免最大化后，static位置出现残影
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 50;	
		tem_rcControl.right -= 340;
		tem_rcControl.left   = tem_rcControl.right-42;
		tem_rcControl.bottom = 67;
		GetDlgItem(IDC_STACHK_PIC)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 50;	
		tem_rcControl.right -= 392;
		tem_rcControl.left   = tem_rcControl.right-20;
		tem_rcControl.bottom = 62;
		GetDlgItem(IDC_CHECK_PICTURE)->MoveWindow(&tem_rcControl);

		tem_rcControl.top    = 50;	
		tem_rcControl.left   = tem_rcControl.right + 60;
		tem_rcControl.right  = tem_rcControl.left + 20;
		tem_rcControl.bottom = 62;
		GetDlgItem(IDC_CHK_AAUTOCROP)->MoveWindow(&tem_rcControl);

		tem_rcControl.top    = 50;	
		tem_rcControl.left   = tem_rcControl.right + 10;
		tem_rcControl.right  = tem_rcControl.left + 90;
		tem_rcControl.bottom = 67;
		GetDlgItem(IDC_STA_AAUTOCROP)->MoveWindow(&tem_rcControl);

		GetDlgItem(IDC_STATIC_TEXT1)->ShowWindow(SW_HIDE);   //避免最大化后，static位置出现残影
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 13;	
		tem_rcControl.right -= 450;
		tem_rcControl.left   = tem_rcControl.right-130;
		tem_rcControl.bottom = 31;
		GetDlgItem(IDC_STATIC_TEXT1)->MoveWindow(&tem_rcControl);

		GetDlgItem(IDC_STATIC_FMT)->ShowWindow(SW_HIDE);     //避免最大化后，static位置出现残影
		tem_rcControl.top    = 48;	
		tem_rcControl.bottom = 66;
		GetDlgItem(IDC_STATIC_FMT)->MoveWindow(&tem_rcControl);

		//5、picture控件和listctrl控件
		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 70;	
		tem_rcControl.left  += 5;
		tem_rcControl.right  = tem_rcControl.left+20;
		tem_rcControl.bottom-= 40;
		GetDlgItem(IDC_SCROLLBAR_VS)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;
		tem_rcControl.bottom-= 40;
		tem_rcControl.top    = tem_rcControl.bottom - 20;	
		tem_rcControl.left  += 25;
		tem_rcControl.right -= 450;
		GetDlgItem(IDC_SCROLLBAR_HS)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 70;	
		tem_rcControl.bottom-= 60;
		tem_rcControl.left  += 25;
		tem_rcControl.right -= 450;
		GetDlgItem(IDC_STATIC_PICTURE2)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;
		tem_rcControl.top    = 70;	
		tem_rcControl.bottom-= 40;
		tem_rcControl.left   = tem_rcControl.right - 447;
		tem_rcControl.right -= 5;
		GetDlgItem(IDC_LIST_PICTURE)->MoveWindow(&tem_rcControl);

		//6、下侧显示控件
		tem_rcControl = rcWorkArea;			
		tem_rcControl.bottom-= 4;
		tem_rcControl.top    = tem_rcControl.bottom - 32;
		tem_rcControl.left   = 5;
		tem_rcControl.right  = 37;
		GetDlgItem(IDC_BUTTON_PATH)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;			
		tem_rcControl.bottom-= 5;
		tem_rcControl.top    = tem_rcControl.bottom - 21;
		tem_rcControl.left   = 40;
		tem_rcControl.right  = 140;
		GetDlgItem(IDC_STA_NOWPATH)->MoveWindow(&tem_rcControl);

		tem_rcControl = rcWorkArea;			
		tem_rcControl.bottom-= 5;
		tem_rcControl.top    = tem_rcControl.bottom - 21;
		tem_rcControl.left   = 145;
		tem_rcControl.right  = 700;
		GetDlgItem(IDC_STA_PATH)->MoveWindow(&tem_rcControl);

		m_tipInfo.AddTool(GetDlgItem(IDC_BUTTON_SAVE), _T("恢复"));

		MoveWindow(&rcWorkArea); 

		GetDlgItem(IDC_STA_BT)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC_TEXT1)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC_FMT)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STACHK_PIC)->ShowWindow(SW_NORMAL);


		
		m_BMaxDlg = FALSE;
		
	}
	else
	{

		//重新定义各个控件的位置
		
		////1、标题位置
		//CRect  tem_rcControl;
		//tem_rcControl = m_rcRectSrc;
		//tem_rcControl.top    = 20;
		//tem_rcControl.left   = 20;
		//tem_rcControl.right  = 400;
		//tem_rcControl.bottom = 100;
		//GetDlgItem(IDC_STA_BT)->MoveWindow(&tem_rcControl);

		////2、三个小图标
		//tem_rcControl = m_rcRectSrc;
		//tem_rcControl.top    = 10;	
		//
		//tem_rcControl.left   = 450;
		//tem_rcControl.right  = 466;
		//tem_rcControl.bottom = 26;

		//tem_rcControl = m_rcRectSrc;
		//tem_rcControl.top    = 10;	
		//tem_rcControl.right  -= 10;
		//tem_rcControl.left   = tem_rcControl.right-16;
		//tem_rcControl.bottom = 26;
//		GetDlgItem(IDC_BUTTON_SAVE)->MoveWindow(&tem_rcControl);
		

		//tem_rcControl = m_rcRectSrc;
		//tem_rcControl.top    = 35;	
		//tem_rcControl.right  -= 10;
		//tem_rcControl.left   = tem_rcControl.right-16;
		//tem_rcControl.bottom = 61;
		//GetDlgItem(IDC_BTN_MIN)->MoveWindow(&tem_rcControl);

		//tem_rcControl = m_rcRectSrc;
		//tem_rcControl.top    = 60;	
		//tem_rcControl.right  -= 10;
		//tem_rcControl.left   = tem_rcControl.right-16;
		//tem_rcControl.bottom = 86;
		//GetDlgItem(IDC_BTN_EXIT)->MoveWindow(&tem_rcControl);

		////3、两个大按钮
		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 10;	
		//tem_rcControl.right  -= 60;
		//tem_rcControl.left   = tem_rcControl.right-64;
		//tem_rcControl.bottom = 74;
		//GetDlgItem(IDC_BUTTON_SET)->MoveWindow(&tem_rcControl);

		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 10;	
		//tem_rcControl.right  -= 154;
		//tem_rcControl.left   = tem_rcControl.right-64;
		//tem_rcControl.bottom = 74;
		//GetDlgItem(IDC_BUTTON_SELECT)->MoveWindow(&tem_rcControl);

		////4、ComboBox控件等
		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 10;	
		//tem_rcControl.right -= 258;
		//tem_rcControl.left   = tem_rcControl.right-200;
		//GetDlgItem(IDC_COMBO_SCANERS)->MoveWindow(&tem_rcControl);

		//tem_rcControl.top    = 45;			
		//tem_rcControl.right  = tem_rcControl.left + 100;
		//GetDlgItem(IDC_COMBO_FORMAT)->MoveWindow(&tem_rcControl);

		//GetDlgItem(IDC_STACHK_PIC)->ShowWindow(SW_HIDE);   //避免最大化后，static位置出现残影
		//tem_rcControl = m_rcRectSrc;
		//tem_rcControl.top    = 53;	
		//tem_rcControl.right -= 258;
		//tem_rcControl.left   = tem_rcControl.right-42;
		//tem_rcControl.bottom = 70;
		//GetDlgItem(IDC_STACHK_PIC)->MoveWindow(&tem_rcControl);

		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 53;	
		//tem_rcControl.right -= 320;
		//tem_rcControl.left   = tem_rcControl.right-15;
		//tem_rcControl.bottom = 65;
		//GetDlgItem(IDC_CHECK_PICTURE)->MoveWindow(&tem_rcControl);

		//GetDlgItem(IDC_STATIC_TEXT1)->ShowWindow(SW_HIDE);   //避免最大化后，static位置出现残影
		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 13;	
		//tem_rcControl.right -= 470;
		//tem_rcControl.left   = tem_rcControl.right-130;
		//tem_rcControl.bottom = 31;
		//GetDlgItem(IDC_STATIC_TEXT1)->MoveWindow(&tem_rcControl);

		//GetDlgItem(IDC_STATIC_FMT)->ShowWindow(SW_HIDE);     //避免最大化后，static位置出现残影
		//tem_rcControl.top    = 48;	
		//tem_rcControl.bottom = 66;
		//GetDlgItem(IDC_STATIC_FMT)->MoveWindow(&tem_rcControl);

		////5、picture控件和listctrl控件
		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 90;	
		//tem_rcControl.left  += 5;
		//tem_rcControl.right  = tem_rcControl.left+20;
		//tem_rcControl.bottom-= 40;
		//GetDlgItem(IDC_SCROLLBAR_VS)->MoveWindow(&tem_rcControl);

		//tem_rcControl = rcWorkArea;
		//tem_rcControl.bottom-= 40;
		//tem_rcControl.top    = tem_rcControl.bottom - 20;	
		//tem_rcControl.left  += 25;
		//tem_rcControl.right -= 425;
		//GetDlgItem(IDC_SCROLLBAR_HS)->MoveWindow(&tem_rcControl);

		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 90;	
		//tem_rcControl.bottom-= 60;
		//tem_rcControl.left  += 25;
		//tem_rcControl.right -= 425;
		//GetDlgItem(IDC_STATIC_PICTURE2)->MoveWindow(&tem_rcControl);

		//tem_rcControl = rcWorkArea;
		//tem_rcControl.top    = 90;	
		//tem_rcControl.bottom-= 40;
		//tem_rcControl.left   = tem_rcControl.right - 420;
		//tem_rcControl.right -= 5;
		//GetDlgItem(IDC_LIST_PICTURE)->MoveWindow(&tem_rcControl);

		////6、下侧显示控件
		//tem_rcControl = rcWorkArea;			
		//tem_rcControl.bottom-= 4;
		//tem_rcControl.top    = tem_rcControl.bottom - 32;
		//tem_rcControl.left   = 5;
		//tem_rcControl.right  = 37;
		//GetDlgItem(IDC_BUTTON_PATH)->MoveWindow(&tem_rcControl);

		//tem_rcControl = rcWorkArea;			
		//tem_rcControl.bottom-= 5;
		//tem_rcControl.top    = tem_rcControl.bottom - 21;
		//tem_rcControl.left   = 40;
		//tem_rcControl.right  = 120;
		//GetDlgItem(IDC_STA_NOWPATH)->MoveWindow(&tem_rcControl);

		//tem_rcControl = rcWorkArea;			
		//tem_rcControl.bottom-= 5;
		//tem_rcControl.top    = tem_rcControl.bottom - 21;
		//tem_rcControl.left   = 125;
		//tem_rcControl.right  = 700;
		//GetDlgItem(IDC_STA_PATH)->MoveWindow(&tem_rcControl);


		MoveWindow(&m_rcRectSrc); 

		GetDlgItem(IDC_STA_BT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FMT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STACHK_PIC)->ShowWindow(SW_HIDE);

		ScreenToClient(&m_rcBT);
		GetDlgItem(IDC_STA_BT)->MoveWindow(&m_rcBT);	

		ScreenToClient(&m_rcMax);
		GetDlgItem(IDC_BUTTON_SAVE)->MoveWindow(&m_rcMax);
		ScreenToClient(&m_rcMin);
		GetDlgItem(IDC_BTN_MIN)->MoveWindow(&m_rcMin);
		ScreenToClient(&m_rcExit);
		GetDlgItem(IDC_BTN_EXIT)->MoveWindow(&m_rcExit);

		ScreenToClient(&m_rcSlcScanner);
		GetDlgItem(IDC_STATIC_TEXT1)->MoveWindow(&m_rcSlcScanner);
		ScreenToClient(&m_rcSlcFmt);
		GetDlgItem(IDC_STATIC_FMT)->MoveWindow(&m_rcSlcFmt);
		ScreenToClient(&m_rcComScanner);
		GetDlgItem(IDC_COMBO_SCANERS)->MoveWindow(&m_rcComScanner);
		ScreenToClient(&m_rcComFmt);
		GetDlgItem(IDC_COMBO_FORMAT)->MoveWindow(&m_rcComFmt);

		ScreenToClient(&m_rcAutoCropS);
		GetDlgItem(IDC_STA_AAUTOCROP)->MoveWindow(&m_rcAutoCropS);
		ScreenToClient(&m_rcAutoCropC);
		GetDlgItem(IDC_CHK_AAUTOCROP)->MoveWindow(&m_rcAutoCropC);

		ScreenToClient(&m_rcChkPic);
		GetDlgItem(IDC_CHECK_PICTURE)->MoveWindow(&m_rcChkPic);
		ScreenToClient(&m_rcPic);
		GetDlgItem(IDC_STACHK_PIC)->MoveWindow(&m_rcPic);

		ScreenToClient(&m_rcScan);
		GetDlgItem(IDC_BUTTON_SELECT)->MoveWindow(&m_rcScan);
		ScreenToClient(&m_rcSet);
		GetDlgItem(IDC_BUTTON_SET)->MoveWindow(&m_rcSet);
		ScreenToClient(&m_rcRename);
		GetDlgItem(IDC_BUTTON_CLEAR)->MoveWindow(&m_rcRename);
		ScreenToClient(&m_rcVScroll);
		GetDlgItem(IDC_SCROLLBAR_VS)->MoveWindow(&m_rcVScroll);
		ScreenToClient(&m_rcHScroll);
		GetDlgItem(IDC_SCROLLBAR_HS)->MoveWindow(&m_rcHScroll);
		ScreenToClient(&m_rcWPic);
		GetDlgItem(IDC_STATIC_PICTURE2)->MoveWindow(&m_rcWPic);
		ScreenToClient(&m_rcList);
		GetDlgItem(IDC_LIST_PICTURE)->MoveWindow(&m_rcList);
		
		ScreenToClient(&m_rcSlcPath);
		GetDlgItem(IDC_BUTTON_PATH)->MoveWindow(&m_rcSlcPath);
		ScreenToClient(&m_rcNowPath);
		GetDlgItem(IDC_STA_NOWPATH)->MoveWindow(&m_rcNowPath);
		ScreenToClient(&m_rcShowPath);
		GetDlgItem(IDC_STA_PATH)->MoveWindow(&m_rcShowPath);
		

		GetDlgItem(IDC_STA_BT)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC_TEXT1)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC_FMT)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STACHK_PIC)->ShowWindow(SW_NORMAL);

		m_tipInfo.AddTool(GetDlgItem(IDC_BUTTON_SAVE), _T("最大化"));

		m_BMaxDlg = TRUE;

		/*该段代码为根据低分屏设置的具体控件位置，屏幕分辨率改变时会产生显示错误问题
		//调节控件位置
		CRect  tem_rcControlSrc;
		tem_rcControlSrc.top    = 9;
		tem_rcControlSrc.left   = 5;
		tem_rcControlSrc.right  = 453;
		tem_rcControlSrc.bottom = 62;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_STA_BT)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 2;
		tem_rcControlSrc.left   = 1013;
		tem_rcControlSrc.right  = 1032;
		tem_rcControlSrc.bottom = 21;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_BUTTON_SAVE)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 28;
		tem_rcControlSrc.left   = 1013;
		tem_rcControlSrc.right  = 1032;
		tem_rcControlSrc.bottom = 46;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_BTN_MIN)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 53;
		tem_rcControlSrc.left   = 1013;
		tem_rcControlSrc.right  = 1032;
		tem_rcControlSrc.bottom = 72;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_BTN_EXIT)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 2;
		tem_rcControlSrc.left   = 933;
		tem_rcControlSrc.right  = 1003;
		tem_rcControlSrc.bottom = 72;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_BUTTON_SET)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 0;
		tem_rcControlSrc.left   = 851;
		tem_rcControlSrc.right  = 919;
		tem_rcControlSrc.bottom = 72;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_BUTTON_SELECT)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 7;
		tem_rcControlSrc.left   = 627;
		tem_rcControlSrc.right  = 825;
		tem_rcControlSrc.bottom = 29;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_COMBO_SCANERS)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 42;
		tem_rcControlSrc.left   = 628;
		tem_rcControlSrc.right  = 723;
		tem_rcControlSrc.bottom = 64;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_COMBO_FORMAT)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 46;
		tem_rcControlSrc.left   = 796;
		tem_rcControlSrc.right  = 822;
		tem_rcControlSrc.bottom = 60;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_STACHK_PIC)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 44;
		tem_rcControlSrc.left   = 774;
		tem_rcControlSrc.right  = 786;
		tem_rcControlSrc.bottom = 58;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_CHECK_PICTURE)->MoveWindow(&tem_rcControlSrc);

		GetDlgItem(IDC_STATIC_TEXT1)->ShowWindow(SW_HIDE);
		tem_rcControlSrc.top    = 12;
		tem_rcControlSrc.left   = 536;
		tem_rcControlSrc.right  = 624;
		tem_rcControlSrc.bottom = 26;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_STATIC_TEXT1)->MoveWindow(&tem_rcControlSrc);

		GetDlgItem(IDC_STATIC_FMT)->ShowWindow(SW_HIDE);
		tem_rcControlSrc.top    = 46;
		tem_rcControlSrc.left   = 536;
		tem_rcControlSrc.right  = 620;
		tem_rcControlSrc.bottom = 60;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_STATIC_FMT)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 75;
		tem_rcControlSrc.left   = 2;
		tem_rcControlSrc.right  = 23;
		tem_rcControlSrc.bottom = 656;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_SCROLLBAR_VS)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 635;
		tem_rcControlSrc.left   = 23;
		tem_rcControlSrc.right  = 802;
		tem_rcControlSrc.bottom = 654;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_SCROLLBAR_HS)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 75;
		tem_rcControlSrc.left   = 23;
		tem_rcControlSrc.right  = 800;
		tem_rcControlSrc.bottom = 635;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_STATIC_PICTURE2)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 75;
		tem_rcControlSrc.left   = 803;
		tem_rcControlSrc.right  = 1032;
		tem_rcControlSrc.bottom = 653;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_LIST_PICTURE)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 658;
		tem_rcControlSrc.left   = 9;
		tem_rcControlSrc.right  = 39;
		tem_rcControlSrc.bottom = 686;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_BUTTON_PATH)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 663;
		tem_rcControlSrc.left   = 44;
		tem_rcControlSrc.right  = 114;
		tem_rcControlSrc.bottom = 677;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_STA_NOWPATH)->MoveWindow(&tem_rcControlSrc);

		tem_rcControlSrc.top    = 663;
		tem_rcControlSrc.left   = 123;
		tem_rcControlSrc.right  = 599;
		tem_rcControlSrc.bottom = 677;
		ClientToScreen(&tem_rcControlSrc);
		GetDlgItem(IDC_STA_PATH)->MoveWindow(&tem_rcControlSrc);




		MoveWindow(&m_rcRectSrc);
		GetDlgItem(IDC_STATIC_TEXT1)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC_FMT)->ShowWindow(SW_NORMAL);
		m_BMaxDlg = TRUE;
		*/
	}
	ThumbailList();
}


bool CUDSGLToolsDlg::GerneratePdf(int count)
{
	std::vector<CString>::iterator  tem_item;
	HPDF_Doc   m_pdf;
	HPDF_Error_Handler   error_handler=NULL;
	m_pdf = HPDF_New(error_handler, NULL);
	if (!m_pdf)
	{
		AfxMessageBox("创建PDF失败！");
		return false;
	}

	//设置文档对象
	HPDF_SetCompressionMode(m_pdf, HPDF_COMP_ALL);
	HPDF_SetPageMode(m_pdf, HPDF_PAGE_MODE_USE_OUTLINE);

	//创建Page对象
	HPDF_Page  m_page;
	/*
	for (tem_item=m_WholeName.begin(); tem_item!=m_WholeName.end(); tem_item++)
	{
		m_page = HPDF_AddPage(m_pdf);
		HPDF_Image m_jpg = HPDF_LoadJpegImageFromFile(m_pdf, *tem_item);
		
		m_vcPDFPicPath.push_back(*tem_item);  //用于退出时删除缓存图像
		
		HPDF_Page_SetWidth(m_page, HPDF_Image_GetWidth(m_jpg));
		HPDF_Page_SetHeight(m_page, HPDF_Image_GetHeight(m_jpg));
		HPDF_Page_DrawImage(m_page, m_jpg, 0, 0, HPDF_Image_GetWidth (m_jpg),HPDF_Image_GetHeight (m_jpg));	
	}
	*/
	
	for (int i=count;i<nIndex;i++)
	{		
		m_page = HPDF_AddPage(m_pdf);
		HPDF_Image m_jpg = HPDF_LoadJpegImageFromFile(m_pdf,m_WholeName[i]);
//		m_vcPDFPicPath.push_back(m_WholeName[i]);  //用于退出时删除缓存图像
		HPDF_Page_SetWidth(m_page, HPDF_Image_GetWidth(m_jpg));
		HPDF_Page_SetHeight(m_page, HPDF_Image_GetHeight(m_jpg));
		HPDF_Page_DrawImage(m_page, m_jpg, 0, 0, HPDF_Image_GetWidth (m_jpg),HPDF_Image_GetHeight (m_jpg));	
	}
	
	CString  pdfpath=NULL;
	CString  m_pdfname;
	m_pdfname.Format(TEXT("%04d"),m_dlg_twain.m_totalimage);
//	m_pdfname.Format(TEXT("%d"), m_pdfcount);
	m_pdfname+=".pdf";
	m_pdfname = "PDF_" + m_pdfname;
	pdfpath = m_dlg_twain.m_result_path + m_pdfname;
	for (int i=count;i<nIndex;i++)
	{
		m_BussName.push_back(pdfpath);
	}
	
	HPDF_SaveToFile(m_pdf,pdfpath);
	m_pdfcount++;
	if (NULL!= m_pdf)
	{
		HPDF_Free(m_pdf);
	}

	return true;
}

bool CUDSGLToolsDlg::ClearImage(void)
{
	/*方法一：永久删除
	if (m_dlg_twain.m_totalimage>0)
	{
		for (int i=0;i<nIndex;i++)
		{
			DeleteFile(AllPath[i]);
		}
	}
	*/	

	//方法二：删除到回收站
	std::vector<CString>::iterator  tem_item;
	SHFILEOPSTRUCT FileOp;
	BOOL           m_BRC;    //ReturnCode操作返回码
	CString        strSrcPath;
	if (m_dlg_twain.m_totalimage>0)
	{
		for (tem_item=m_WholeName.begin(); tem_item!=m_WholeName.end();tem_item++)
		{
			strSrcPath  = *tem_item;
			strSrcPath += '\0';

			CString  tem_strDstPath;
			tem_strDstPath = '\0';     //删除操作，直接置为NULL

			CString     tem_strTitle = "正在删除...";
			tem_strTitle += '\0';

			FileOp.fFlags = FOF_ALLOWUNDO|FOF_MULTIDESTFILES|FOF_SIMPLEPROGRESS|FOF_NOCONFIRMATION;

			FileOp.lpszProgressTitle = tem_strTitle;
			FileOp.hwnd              = m_hWnd;
			FileOp.hNameMappings     = NULL;
			FileOp.pFrom             = strSrcPath;
			FileOp.pTo               = tem_strDstPath;    //目标目录
			FileOp.wFunc             = FO_DELETE;

			m_BRC = SHFileOperation(&FileOp);
			if (!FileOp.fAnyOperationsAborted)
			{
				if(m_BRC)   
				{
					//操作出现错误
					continue;
				}		
			}
			else
			{
				//出现意外中止
				continue;
			}
		}
		/*
		for (int i=0;i<nIndex;i++)
		{
			strSrcPath  = AllPath[i];
			strSrcPath += '\0';

			CString  tem_strDstPath;
			tem_strDstPath = '\0';     //删除操作，直接置为NULL

			CString     tem_strTitle = "正在删除...";
			tem_strTitle += '\0';

			FileOp.fFlags = FOF_ALLOWUNDO|FOF_MULTIDESTFILES|FOF_SIMPLEPROGRESS|FOF_NOCONFIRMATION;

			FileOp.lpszProgressTitle = tem_strTitle;
			FileOp.hwnd              = m_hWnd;
			FileOp.hNameMappings     = NULL;
			FileOp.pFrom             = strSrcPath;
			FileOp.pTo               = tem_strDstPath;    //目标目录
			FileOp.wFunc             = FO_DELETE;

			m_BRC = SHFileOperation(&FileOp);
			if (!FileOp.fAnyOperationsAborted)
			{
				if(m_BRC)   
				{
					//操作出现错误
					continue;
				}		
			}
			else
			{
				//出现意外中止
				continue;
			}
		}
		*/
	}
	

	
	
	return true;
}

void CUDSGLToolsDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	
	ReleaseDC(pDc);
//	m_dlg_twain.CloseDS();
	m_dlg_twain.CloseDSM();	


	CDialogEx::OnClose();
}

void CUDSGLToolsDlg::OnClickedCheckPicture()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_scan_mark)
	{
		return;
	}
	else
	{
		if (!m_click_mark)
		{
			//没有选定图像，点击<原图>加载首张图像
			CString   tem_strFilePath = m_BussName[0];
			if (!tem_strFilePath.IsEmpty())
			{
				//判断tem_strFilePath是图像还是PDF文件
				int      tem_nFormat   = 0;
				CString  tem_strFormat = tem_strFilePath;
				tem_nFormat   = tem_strFormat.ReverseFind('.');
				tem_strFormat = tem_strFormat.Mid(tem_nFormat+1);
				tem_strFormat.MakeLower();
				if (tem_strFormat == _T("pdf"))
				{
					tem_strFilePath = m_WholeName[0];
					ShowWpicture(tem_strFilePath);
				}
				else
				{
					ShowWpicture(tem_strFilePath);  //0829
				}

			}
			
		}
		else
		{
			if (m_nSlctItem>=0)
			{
				CString  tem_SlcItem = m_BussName[m_nSlctItem];   //0829
				//判断tem_strFilePath是图像还是PDF文件
				int      tem_nFormat   = 0;
				CString  tem_strFormat = tem_SlcItem;
				tem_nFormat   = tem_strFormat.ReverseFind('.');
				tem_strFormat = tem_strFormat.Mid(tem_nFormat+1);
				tem_strFormat.MakeLower();
				if (tem_strFormat == _T("pdf"))
				{
					tem_SlcItem = m_WholeName[m_nSlctItem];
					ShowWpicture(tem_SlcItem);
				}
				else
				{
					ShowWpicture(tem_SlcItem);  //0829
				}
				
			}
			
		}
		return;
	}	
}

void CUDSGLToolsDlg::ShowWpicture(CString m_path)
{
	CxImage* pImage;
	pImage = new CxImage; 
	pImage->Load(m_path);
	cx = pImage->GetWidth();
	cy = pImage->GetHeight();

	GetDlgItem(IDC_STATIC_PICTURE2)->GetWindowRect(&pic_rect);
	ScreenToClient(&pic_rect);

	pDc->FillSolidRect(0, 0, pic_rect.right-pic_rect.left, pic_rect.bottom-pic_rect.top,m_bkColor);

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_PICTURE))
	{
		m_scrollhs.SetScrollRange(1, cx-pic_rect.right+pic_rect.left);
		m_scrollvs.SetScrollRange(1, cy-pic_rect.bottom+pic_rect.top);

		m_scrollhs.SetScrollPos(1);
		m_scrollvs.SetScrollPos(1);

		m_pt.x = 0;
		m_pt.y = 0;
		n_pt.x = 0;
		n_pt.y = 0;
		m_move.x = 0;
		m_move.y = 0;
		m_save.x = 0;
		m_save.y = 0;

		imageDC.CreateCompatibleDC(pDc);
		HBITMAP   m_hBitmap;
		m_hBitmap = pImage->MakeBitmap(imageDC);
		CBitmap m_bmp;
		m_bmp.Attach(m_hBitmap);
		imageDC.SelectObject(&m_bmp);
		offsetx = pic_rect.left;
		offsety = pic_rect.top;
		pDc->BitBlt(0,0,pic_rect.right-offsetx,pic_rect.bottom-offsety,&imageDC,0,0,SRCCOPY);		
		
	}
	else
	{
		GetDlgItem(IDC_SCROLLBAR_HS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCROLLBAR_VS)->EnableWindow(FALSE);
		int   m_neww = 0;
		int   m_newh = 0;
		float m_picw = (float)(pic_rect.right-pic_rect.left);
		float m_pich = (float)(pic_rect.bottom-pic_rect.top);
		float m_pic_ratio = m_picw/m_pich;
		float m_imgw = (float)cx;
		float m_imgh = (float)cy;
		float m_img_ratio = m_imgw/m_imgh;
		if (m_img_ratio>m_pic_ratio)
		{
			m_neww = (int)m_picw;
			m_newh = (int)((m_picw/m_imgw)*m_imgh);
			pImage->Resample(m_neww, m_newh);
		}
		else
		{
			m_newh = (int)m_pich;
			m_neww = (int)((m_pich/m_imgh)*m_imgw);
			pImage->Resample(m_neww, m_newh);
		}
		imageDC.CreateCompatibleDC(pDc);
		HBITMAP   m_hBitmap;
		m_hBitmap = pImage->MakeBitmap(imageDC);
		CBitmap m_bmp;
		m_bmp.Attach(m_hBitmap);
		imageDC.SelectObject(&m_bmp);
		offsetx = pic_rect.left;
		offsety = pic_rect.top;
		pDc->BitBlt(0,0,pic_rect.right-offsetx,pic_rect.bottom-offsety,&imageDC,0,0,SRCCOPY);
	}

	::delete pImage;	
}


void CUDSGLToolsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	xpos = m_scrollhs.GetScrollPos();
	switch(nSBCode)
	{
	case SB_LINEUP:    //滚动一行,点击滚动条左两侧箭头
		xpos-=1;
		break;
	case SB_LINEDOWN:
		xpos+=1;
		break;
	case SB_PAGEUP:    //滚动一页，点击滚动条中间空白区域
		xpos-=10;
		break;
	case SB_PAGEDOWN:
		xpos+=10;
		break;
	case SB_TOP:     //最大值
		xpos=100;
		break;
	case SB_BOTTOM:
		xpos=1;
		break;
	case SB_THUMBPOSITION:     //拖动
		xpos=nPos;
		break;
	default:
		return;
	}
	m_scrollhs.SetScrollPos(xpos);
	pDc->BitBlt(0,0,pic_rect.right-offsetx,pic_rect.bottom-offsety,&imageDC,xpos,ypos,SRCCOPY);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CUDSGLToolsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ypos = m_scrollvs.GetScrollPos();
	switch(nSBCode)
	{
	case SB_LINEUP:    //滚动一行,点击滚动条左两侧箭头
		ypos-=1;
		break;
	case SB_LINEDOWN:
		ypos+=1;
		break;
	case SB_PAGEUP:    //滚动一页，点击滚动条中间空白区域
		ypos-=10;
		break;
	case SB_PAGEDOWN:
		ypos+=10;
		break;
	case SB_TOP:     //最大值
		ypos=100;
		break;
	case SB_BOTTOM:
		ypos=1;
		break;
	case SB_THUMBPOSITION:     //拖动
		ypos=nPos;
		break;
	default:
		return;
	}
	m_scrollvs.SetScrollPos(ypos);
	pDc->BitBlt(0,0,pic_rect.right-offsetx,pic_rect.bottom-offsety,&imageDC,xpos,ypos,SRCCOPY);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CUDSGLToolsDlg::OnClickListPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	BOOL    tem_BRepeat = FALSE;
	CString tem_strSlcFile;
	std::vector<int>::iterator       item_nIndex;

	//如果ctrl键被按下，说明要导出PDF或多页TIF，每次存入vector都应应该首先清空
	if (pNMItemActivate->uKeyFlags == LVKF_CONTROL)
	{
		m_listindex = pNMItemActivate->iItem;
		if (m_listindex>=0 && m_listindex<m_imagelist.GetItemCount())
		{
			//判断是否为二次点击，二次点击为不选该文件
			for (item_nIndex=m_vcCtrlIndex.begin(); item_nIndex!=m_vcCtrlIndex.end(); item_nIndex++)
			{
				if (m_listindex == *item_nIndex)
				{
					tem_BRepeat = TRUE;
					m_vcCtrlIndex.erase(item_nIndex);
					break;
				}
			}
			if (tem_BRepeat)
			{
				//是重复的
				tem_BRepeat = FALSE;
			}
			else
			{
				//非重复的
				m_vcCtrlIndex.push_back(m_listindex);
			}
		}
	}
	else
	{
		m_vcCtrlIndex.clear();
		m_listindex = pNMItemActivate->iItem;
		if (m_listindex>=0 && m_listindex<m_imagelist.GetItemCount())
		{
			m_nSlctItem = m_listindex;
			m_vcCtrlIndex.push_back(m_listindex);
			m_path_image = m_BussName[m_listindex];    //0829加载大图
			
			//判断m_path_image的图像类型
			int      tem_nFormat   = 0;
			CString  tem_strFormat = m_path_image;
			tem_nFormat   = m_path_image.ReverseFind('.');
			tem_strFormat = m_path_image.Mid(tem_nFormat+1);
			tem_strFormat.MakeLower();
			if (tem_strFormat == _T("pdf"))
			{
				//如果为PDF文件加载图像
				m_path_image = m_WholeName[m_listindex];
				m_scrollhs.SetScrollPos(1);
				m_scrollvs.SetScrollPos(1);
				ShowWpicture(m_path_image);
			}
			else
			{
				m_scrollhs.SetScrollPos(1);
				m_scrollvs.SetScrollPos(1);
				ShowWpicture(m_path_image);
			}
		
			m_click_mark = true;
		}


	}
	
	*pResult = 0;
}


void CUDSGLToolsDlg::OnRclickListPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_imagelist.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_imagelist.GetNextSelectedItem(pos);		
	}
	

	m_listindex = pNMItemActivate->iItem;
	//缩略图右键处理，添加右键处理菜单
	if (m_listindex>=0 && m_listindex<m_imagelist.GetItemCount())
	{
		CPoint   tem_ptRight;
		GetCursorPos(&tem_ptRight);
		m_mnuRightClick.TrackPopMenu(tem_ptRight.x, tem_ptRight.y);
	}

	*pResult = 0;
}


void CUDSGLToolsDlg::OnHoverListPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}


void CUDSGLToolsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
//	PostMessage(WM_NCLBUTTONDOWN,   HTCAPTION,   MAKELPARAM(point.x,  point.y));   
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_PICTURE))
	{
		if (pic_rect.PtInRect(point))
		{
			SetCapture();
//			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			SetCursor(LoadCursor(NULL,IDC_HAND));
			GetCursorPos(&m_pt);
		}
	}	
	

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CUDSGLToolsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int tem_nPicWidth, tem_nPicHeight;
	tem_nPicWidth  = pic_rect.right - pic_rect.left;
	tem_nPicHeight = pic_rect.bottom - pic_rect.top; 
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_PICTURE))
	{
		if (pic_rect.PtInRect(point))
		{
			if (cx>tem_nPicWidth && cy>tem_nPicHeight)
			{
				GetCursorPos(&n_pt);
				m_move.x = m_pt.x-n_pt.x;
				m_move.y = m_pt.y-n_pt.y;

				m_save.x += m_move.x;
				m_save.y += m_move.y;

				if (m_save.x>0&&m_save.x<cx-pic_rect.right+pic_rect.left)
				{
					OnHScroll(SB_THUMBPOSITION, m_save.x, NULL);
				}
				else if (m_save.x>=cx-pic_rect.right+pic_rect.left)
				{
					m_save.x = cx-pic_rect.right+pic_rect.left;
					OnHScroll(SB_THUMBPOSITION, m_save.x, NULL);
				}
				else
				{
					m_save.x = 1;
					OnHScroll(SB_THUMBPOSITION, 1, NULL);

				}

				if (m_save.y>0&&m_save.y<cy-pic_rect.bottom+pic_rect.top)
				{
					OnVScroll(SB_THUMBPOSITION, m_save.y, NULL);
				}
				else if (m_save.y>=cy-pic_rect.bottom+pic_rect.top)
				{
					m_save.y = cy-pic_rect.bottom+pic_rect.top;
					OnVScroll(SB_THUMBPOSITION, m_save.y, NULL);
				}
				else
				{
					m_save.y = 1;
					OnVScroll(SB_THUMBPOSITION, 1, NULL);

				}

				ReleaseCapture();
				SetCursor(LoadCursor(NULL,IDC_ARROW));

			}
			else if (cx>tem_nPicWidth && cy<=tem_nPicHeight)
			{
				GetCursorPos(&n_pt);
				m_move.x = m_pt.x-n_pt.x;
				m_save.x += m_move.x;

				if (m_save.x>0&&m_save.x<cx-pic_rect.right+pic_rect.left)
				{
					OnHScroll(SB_THUMBPOSITION, m_save.x, NULL);
					OnVScroll(SB_THUMBPOSITION, 0, NULL);
				}
				else if (m_save.x>=cx-pic_rect.right+pic_rect.left)
				{
					m_save.x = cx-pic_rect.right+pic_rect.left;
					OnHScroll(SB_THUMBPOSITION, m_save.x, NULL);
					OnVScroll(SB_THUMBPOSITION, 0, NULL);
				}
				else
				{
					m_save.x = 1;
					OnHScroll(SB_THUMBPOSITION, 1, NULL);
					OnVScroll(SB_THUMBPOSITION, 0, NULL);
				}
				ReleaseCapture();
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
			else if (cx<=tem_nPicWidth && cy>tem_nPicHeight)
			{
				GetCursorPos(&n_pt);
				m_move.y = m_pt.y-n_pt.y;
				m_save.y += m_move.y;

				if (m_save.y>0&&m_save.y<cy-pic_rect.bottom+pic_rect.top)
				{
					OnHScroll(SB_THUMBPOSITION, 0, NULL);
					OnVScroll(SB_THUMBPOSITION, m_save.y, NULL);
				}
				else if (m_save.y>=cy-pic_rect.bottom+pic_rect.top)
				{
					m_save.y = cy-pic_rect.bottom+pic_rect.top;
					OnVScroll(SB_THUMBPOSITION, m_save.y, NULL);
					OnHScroll(SB_THUMBPOSITION, 0, NULL);
				}
				else
				{
					m_save.y = 1;
					OnVScroll(SB_THUMBPOSITION, 1, NULL);
					OnHScroll(SB_THUMBPOSITION, 0, NULL);
				}

				ReleaseCapture();
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
			else
			{
				ReleaseCapture();
				SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
		}	
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CUDSGLToolsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseMove(nFlags, point);
}


void CUDSGLToolsDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString    tem_strScannerMark = "1";
	m_dlg_twain.Self_ReadCountInfo(m_dlg_twain.m_totalimage, 1);
	ReleaseDC(pDc);
	//	m_dlg_twain.CloseDS();
	m_dlg_twain.CloseDSM();	

	std::vector<CString>::iterator  item;
    //原清除PDF缓存图像方法
// 	for (item=m_vcPDFPicPath.begin(); item!=m_vcPDFPicPath.end(); item++)
// 	{
// 		DeleteFile(*item);
// 	}

	for (item=m_WholeName.begin(); item!=m_WholeName.end(); item++)
	{
		DeleteFile(*item);
	}

	

	//向主窗口线程发送消息
	CString   tem_str2   = _T("2");
//	HWND      tem_hWnd = ::FindWindow(_T("MyResourceManagement"), NULL);     //无标题窗口
	HWND      tem_hWnd = ::FindWindow(NULL, _T("紫图SmartScan影像平台系统v1.8"));          //标题窗口
	if (tem_hWnd != NULL)
	{
		::SendMessage(tem_hWnd, WM_SETTEXT, 0, (LPARAM)tem_str2.GetBuffer(tem_str2.GetLength()));

	}


	CDialogEx::OnOK();
}



void CUDSGLToolsDlg::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
	
}


void CUDSGLToolsDlg::OnBnClickedBtnWeb()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"),  _T("http://www.arc-uds.com"), NULL, NULL,SW_SHOWNORMAL);
}


LRESULT CUDSGLToolsDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 取得鼠标所在的窗口区域
	UINT nHitTest = CDialog::OnNcHitTest(point);

	CRect   tem_rcStaBT;
	GetDlgItem(IDC_STA_BT)->GetWindowRect(&tem_rcStaBT);
	CPoint  tem_Pt = point;
	if (tem_rcStaBT.PtInRect(tem_Pt))
	{
		return HTCAPTION;
	}


	// 如果鼠标在窗口客户区，则返回标题条代号给Windows
    // 使Windows按鼠标在标题条上类进行处理，即可单击移动窗口
//	return (nHitTest==HTCLIENT) ? HTCAPTION : nHitTest;


	return CDialogEx::OnNcHitTest(point);
}


void CUDSGLToolsDlg::OnCopyIt()
{
	// TODO: 在此添加命令处理程序代码
	if (m_vcCtrlIndex.size()==0)
	{
		m_vcCtrlIndex.clear();
		return;
	}
	std::vector<CString>            tem_vcFilesList;
	std::vector<int>::iterator      tem_nitem;
	for (tem_nitem=m_vcCtrlIndex.begin(); tem_nitem!=m_vcCtrlIndex.end(); tem_nitem++)
	{
		CString tem_strPath = m_BussName[*tem_nitem];
		tem_vcFilesList.push_back(tem_strPath);
	}
	CopyFiles2Clipboard(tem_vcFilesList);
	m_vcCtrlIndex.clear();
}


void CUDSGLToolsDlg::OnDeleteIt()
{
	// TODO: 在此添加命令处理程序代码
//	AfxMessageBox("delete");
	if (m_vcCtrlIndex.size() == 0)
	{
		return;
	}
	if (IDOK !=m_iTips.DoModal())
	{
		return;
	}
	//删除指定文件
	SHFILEOPSTRUCT FileOp;
	BOOL           m_BRC;    //ReturnCode操作返回码
	CString        strSrcPath;
	//删除至回收站
	std::vector<int>::iterator  tem_nIndex;
	for (tem_nIndex=m_vcCtrlIndex.begin(); tem_nIndex!=m_vcCtrlIndex.end(); tem_nIndex++)
	{
		int tem_nDeleteItem = *tem_nIndex;

		strSrcPath  = m_BussName[tem_nDeleteItem];

		strSrcPath += '\0';

		CString  tem_strDstPath;
		tem_strDstPath = '\0';     //删除操作，直接置为NULL

		CString     tem_strTitle;
		tem_strTitle = _T("正在删除...");
		tem_strTitle += '\0';

		FileOp.fFlags = FOF_ALLOWUNDO|FOF_MULTIDESTFILES|FOF_SIMPLEPROGRESS|FOF_NOCONFIRMATION;

		FileOp.lpszProgressTitle = tem_strTitle;
		FileOp.hwnd              = m_hWnd;
		FileOp.hNameMappings     = NULL;
		FileOp.pFrom             = strSrcPath;
		FileOp.pTo               = tem_strDstPath;    //目标目录
		FileOp.wFunc             = FO_DELETE;

		//弹窗提示--------------------------------------------------


		m_BRC = SHFileOperation(&FileOp);
		if (!FileOp.fAnyOperationsAborted)
		{
			if(m_BRC)   
			{
				//操作出现错误
			}		
		}
		else
		{
			//出现意外中止
		}
	}

	//对m_vcCtrlIndex进行从大到小的排序，删除vector信息逆向删除，避免删除前面的元素影响后面的位置
	//使用sort和反向迭代器对vector排降序
	sort(m_vcCtrlIndex.rbegin(), m_vcCtrlIndex.rend());
	for (tem_nIndex=m_vcCtrlIndex.begin(); tem_nIndex!=m_vcCtrlIndex.end(); tem_nIndex++)
	{
		m_FileName.erase(m_FileName.begin()+*tem_nIndex);
		m_WholeName.erase(m_WholeName.begin()+*tem_nIndex);
		m_BussName.erase(m_BussName.begin()+*tem_nIndex);   //0829添加了原文件的vector删除，不知是否之前忘记添加

		//删除ImageList中对应图像
		m_nNameCounts--;
		m_nImgCounts--;
//		imagelist.Remove(*tem_nIndex);
	}


	ThumbailList();
	//判断ListCtrl是否还有图像
	int  tem_nVecNum = m_WholeName.size();
	if (tem_nVecNum)
	{
		//非0， 
		m_nSlctItem = tem_nVecNum-1;
		CString   tem_strLastPic = m_BussName.back();
		//判断是否为PDF图像
		int      tem_nFormat   = 0;
		CString  tem_strFormat = tem_strLastPic;
		tem_nFormat   = tem_strFormat.ReverseFind('.');
		tem_strFormat = tem_strFormat.Mid(tem_nFormat+1);
		tem_strFormat.MakeLower();
		if (tem_strFormat == _T("pdf"))
		{
			tem_strLastPic = m_WholeName[m_nSlctItem];
			ShowWpicture(tem_strLastPic);
		}
		else
		{
			ShowWpicture(tem_strLastPic);  //0829
		}
	}
	else
	{
		//0个元素,清空picture控件的显示
		m_nSlctItem = tem_nVecNum-1;
		GetDlgItem(IDC_STATIC_PICTURE2)->GetWindowRect(&pic_rect);
		ScreenToClient(&pic_rect);
		
		pDc->FillSolidRect(0, 0, pic_rect.right-pic_rect.left, pic_rect.bottom-pic_rect.top,m_bkColor);
		m_pic2.SetBitmap(NULL);

	}

	m_vcCtrlIndex.clear();
}


void CUDSGLToolsDlg::OnRename()
{
	// TODO: 在此添加命令处理程序代码
	if (m_vcCtrlIndex.size() == 0)
	{
		return;
	}

	m_nLastSlcFile = m_vcCtrlIndex.back();
	m_imagelist.SetFocus();
	m_conRename = m_imagelist.EditLabel(m_nLastSlcFile);


	m_vcCtrlIndex.clear();
}


void CUDSGLToolsDlg::OnProperty()
{
	// TODO: 在此添加命令处理程序代码
	int       tem_nLastFile;
	CString   tem_strLastFile;

	if (m_vcCtrlIndex.size()==0)
	{
		return;
	}
	tem_nLastFile = m_vcCtrlIndex.back();
	tem_strLastFile = m_BussName[tem_nLastFile];  //获取

	SHELLEXECUTEINFO   sei; 
	sei.hwnd   =   this->GetSafeHwnd(); 
	sei.lpVerb   =  TEXT( "properties"); 
	sei.lpFile   =   tem_strLastFile; 
	sei.lpDirectory   =   NULL; 
	sei.lpParameters   =  NULL; 
	sei.nShow   =   SW_SHOWNORMAL; 
	sei.fMask   =   SEE_MASK_INVOKEIDLIST ; 
	sei.lpIDList   =   NULL; 
	sei.cbSize   =   sizeof(SHELLEXECUTEINFO); 
	ShellExecuteEx(&sei);
	m_vcCtrlIndex.clear();
}


BOOL CUDSGLToolsDlg::CreateHSMenu(void)
{
	COLORREF    tem_clrSet0 = RGB(185,205,246);   //底色
	COLORREF    tem_clrSet1 = RGB(222,160,228);   //选中颜色
	COLORREF    tem_clrSet2 = RGB(131,88,136);    //分支夜色

	HWND hwnd = GetSafeHwnd();
	m_mnuRightClick.CreateMenu( hwnd );
	CHSMenu::SetMenuItemHeight(28);  //设置Menu高度
	CHSMenuPro::SetColor(CLR_BKGNDMENU, tem_clrSet0); //设置底色
	CHSMenuPro::SetColor(CLR_BKGNDSEL, tem_clrSet1);	//设置被选中时底色
	CHSMenuPro::SetColor(CLR_BORDERSPR, tem_clrSet2);	//设置分支颜色
	CHSMenuPro::SetMenuStyle(ML_ARROWROUND, true);
	CHSMenuPro::SetMenuStyle(ML_SELGRADIENT_V, true);

	m_mnuRightClick.CreateMenu(hwnd);
	m_mnuRightClick.AppendMenu(_T("复  制"), 32775, 0, 0);
	m_mnuRightClick.AppendMenu(_T("删  除"), 32776, 0, 0);
	m_mnuRightClick.AppendMenu(_T("重命名"), 32777, 0, 0);
	m_mnuRightClick.AppendMenu(_T("属  性"), 32778, 0, 0); //数字为菜单ID
	

	return TRUE;
}



BOOL CUDSGLToolsDlg::CopyFiles2Clipboard(std::vector<CString> const& vecFiles)
{
	UINT  uDropEffect     = RegisterClipboardFormat(_T("Preferred DropEffect"));  
	HGLOBAL hGblEffect    = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE, sizeof(DWORD));  
	DWORD *pDWDropEffect  = (DWORD*)GlobalLock(hGblEffect);  
	*pDWDropEffect        = DROPEFFECT_COPY;  
	GlobalUnlock(hGblEffect);  

	DROPFILES dropFiles;  
	dropFiles.pFiles = sizeof(DROPFILES);  
	dropFiles.pt.x   = 0;  
	dropFiles.pt.y   = 0;  
	dropFiles.fNC    = FALSE;  
	dropFiles.fWide  = FALSE;  

	UINT uFileListLen = 0;  
	for (std::vector<CString>::const_iterator it = vecFiles.begin(); it != vecFiles.end(); ++it)  
	{  
		uFileListLen += (it->GetLength() + 1);//尾部一个\0结束符  
	}  

	UINT uGblLen      = sizeof(DROPFILES) + sizeof(TCHAR)* (uFileListLen + 1);  
	HGLOBAL hGblFiles = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE, uGblLen);  
	BYTE *  pData     = (BYTE*)GlobalLock(hGblFiles);  
	memcpy(pData, (LPVOID)(&dropFiles), sizeof(DROPFILES));  

	BYTE *pFileList = pData + sizeof(DROPFILES);  

	for (std::vector<CString>::const_iterator it = vecFiles.begin(); it != vecFiles.end(); ++it)  
	{  
		UINT uLen = (it->GetLength() + 1) * sizeof(TCHAR);  
		memcpy(pFileList, (BYTE*)(LPCTSTR)(*it),  uLen);  
		pFileList += uLen;  
	}  

	GlobalUnlock(hGblFiles);  

	if( OpenClipboard() )  
	{  
		EmptyClipboard();  
		SetClipboardData( CF_HDROP, hGblFiles );  
		SetClipboardData(uDropEffect,hGblEffect);  
		CloseClipboard();  
		return TRUE;  
	}  
	else  
	{  
		::GlobalFree(hGblEffect);  
		::GlobalFree(hGblFiles);  
	}  
	return TRUE;  
}


CString CUDSGLToolsDlg::GetMyDocument(void)
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


void CUDSGLToolsDlg::CheckIniFile(void)
{
	CString   tem_strResDir = _T("");
	CString   tem_strIniSrc = _T("");
	CString   tem_strIniDst = _T("");
	TCHAR     tem_tcFilepath[MAX_PATH+1] = {0};
	GetModuleFileName(NULL, tem_tcFilepath, MAX_PATH);
	(_tcsrchr(tem_tcFilepath, _T('\\')))[1] = 0;
	tem_strResDir   = tem_tcFilepath;
	tem_strIniSrc  = tem_strResDir;


	tem_strIniSrc  += _T("\\");
	tem_strIniSrc  += _T("config.ini");      //安装目录下的配置文件


	tem_strIniDst    = Self_GetMyDocument();
	tem_strIniDst   += _T("\\UDSData");
	CFileFind  tem_fFileFind;
	if (!tem_fFileFind.FindFile(tem_strIniDst))
	{
		CreateDirectory(tem_strIniDst, NULL);
	}
	tem_strIniDst   += _T("\\config.ini");      //我的文档下的配置文件
	CopyFile(tem_strIniSrc, tem_strIniDst, TRUE);
}


CString CUDSGLToolsDlg::Self_GetMyDocument(void)
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


afx_msg LRESULT CUDSGLToolsDlg::OnReadini(WPARAM wParam, LPARAM lParam)
{
	int tem_nInfo    = (int)wParam;
	int tem_nOperate = (int)lParam;
	CString  tem_strIniInfo = _T("");

	switch(tem_nOperate)
	{
	case 0:
		if (tem_nInfo==0)
		{
			m_dlg_twain.m_nPageCount = 1;
		}
		m_dlg_twain.ReadIniInfo();
		break;
	case 1:
		m_dlg_twain.ReadIniInfo();
		break;
	case 2:
		m_dlg_twain.Self_ReadCountInfo(0, 0);
		break;
	case 3:
		::GetPrivateProfileString("SaveSet","AdvancedSet","没有找到AdvancedSet信息",tem_strIniInfo.GetBuffer(MAX_PATH),MAX_PATH,m_ini_path); 
		if (tem_strIniInfo == _T("1"))
		{
			m_BAdvancedSet = TRUE;
		}
		else
		{
			m_BAdvancedSet = FALSE;
		}
		tem_strIniInfo.ReleaseBuffer();

		break;
	}
	
	return 0;
}


void CUDSGLToolsDlg::Self_DisableCtrls(bool mode)
{
	if (mode)
	{
		GetDlgItem(IDC_COMBO_SCANERS)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PICTURE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_EXIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(FALSE);
		
	}
	else
	{
		GetDlgItem(IDC_COMBO_SCANERS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PICTURE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(TRUE);
	}
}

void CUDSGLToolsDlg::OnClickedChkAautocrop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_AAUTOCROP))
	{
		m_BAutoCrop = TRUE;
		::WritePrivateProfileString("SaveSet", "AutoCropCorrect", "1", m_ini_path);
	} 
	else
	{
		m_BAutoCrop = FALSE;
		::WritePrivateProfileString("SaveSet", "AutoCropCorrect", "0", m_ini_path);
	}

}
