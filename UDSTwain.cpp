#include "StdAfx.h"
#include "UDSTwain.h"

bool    m_dsopen;
extern std::vector<CString>  m_FileName;
extern std::vector<CString>  m_WholeName;
extern std::vector<CString>  m_BussName;
extern std::vector<CString>  g_vcCurName;
extern std::vector<CString>  g_vcCurPath;
extern CString m_strBufferPath;

CUDSTwain::CUDSTwain(void)
{
	m_hWnd         = NULL;
	m_hTwain32Dll  = NULL;
	m_pDSMEntry    = NULL;
	m_AppID.Id     = 0;  m_AppID.ProductName[0]      = '\0';
	m_ScannerID1.Id = 0;  m_ScannerID1.ProductName[0] = '\0';
	m_ScannerID2.Id = 0;  m_ScannerID2.ProductName[0] = '\0';
	m_pScannerID = &m_ScannerID1;

	m_ScanInfo.bDSMOpened = m_ScanInfo.bDSOpened=m_ScanInfo.bIsShowUI=false;
	m_ScanInfo.nScanDPI      = 200;
	m_ScanInfo.nScanMode     = 1;
	m_ScanInfo.nBrightness=m_ScanInfo.nContrast=0;
	m_ScanInfo.nThreshold    = 128;
	m_ScanInfo.nQuality      = 50;
	m_ScanInfo.nImageToScan  = -1;
	m_ScanInfo.nImageScanned = 0;
	m_ScanInfo.strSN.Empty();  
	m_ScanInfo.strLicenseCode.Empty();
	m_ScanInfo.strImagePath.Empty();  
	m_ScanInfo.strBarcodeType.Empty();
	m_ScanInfo.bActive = m_ScanInfo.bActiveLocker = false;
	m_ScanInfo.bIsAuthorizedFlatbed = m_ScanInfo.bAutoCrop = m_ScanInfo.bInvert = false;
	m_ScanInfo.nScanMode     = 1;
	m_ScanInfo.nScanSource   = 0; 
	m_ScanInfo.BlackEdge     = 0;    
	m_ScanInfo.edgeThreshold = 32;
	m_ScanInfo.BlankFilter   = 0;  
	m_ScanInfo.grayThreshold = 32;  
	m_ScanInfo.edgeRatio     = 0;
	m_ScanInfo.ColorFilter   = 0;  
	m_ScanInfo.colorThreshold= 32;  
	m_ScanInfo.colorRatio    = 0;
	m_ScanInfo.BackgroundUnify = 0;

	m_totalimage  = 0;
	m_nImageCount = 0;
	m_dsopen = false;
	m_nMutiTifName = 0;
	m_strSlcScanner = _T("");
//	m_format_index = 3;
//	m_transformation = 0;  //默认本地传输


	m_nNameMode     = 0;
	m_nNameDate     = 0;
	m_nNameTime     = 0;
	m_nNameBegin    = 0;

	m_strNameDivide = _T("");
	m_strNamePre1   = _T("");
	m_strNamePre2   = _T("");
	m_strNamePre3   = _T("");
	m_strIniPath    = _T("");

	m_nPageSite  = 2;   //0-左下角，1-中间，2-右下角
	m_nPageReset = 1;   //0-每次扫描重置，1-每次启动软件重置， 2-保持页码手动重置
	m_nPageSize  = 60;
	m_nPageCount = 1;   //页码起始
	m_nGrayBit   = 0;   //灰度图位数
	m_BPageOpen  = TRUE;

	m_BAutoCrop  = FALSE;
}


CUDSTwain::~CUDSTwain(void)
{
}


bool CUDSTwain::OpenDSM(void)
{
	if(m_ScanInfo.bDSMOpened)
	{
		return true;
	}
	m_ScanInfo.bDSMOpened=false;
	m_hWnd=AfxGetMainWnd()->m_hWnd;

	if(m_hWnd==NULL)
	{
		return false;
	}
	char      TwainDir[_MAX_PATH];        
	TW_UINT16 rc;

	GetWindowsDirectory(TwainDir,_MAX_PATH);
	lstrcat(TwainDir,"\\TWAIN_32.DLL");     
	m_hTwain32Dll = AfxLoadLibrary(TwainDir);
	if(m_hTwain32Dll==NULL)
	{
		return false;
	}
	m_pDSMEntry=(DSMENTRYPROC)GetProcAddress(m_hTwain32Dll,MAKEINTRESOURCE(1));
	if(m_pDSMEntry==NULL)
	{
		AfxFreeLibrary(m_hTwain32Dll);
		m_hTwain32Dll=NULL;
		return false;
	}
	m_AppID.Id               = 0;
	m_AppID.Version.MajorNum = 2;
	m_AppID.Version.MinorNum = 1;
	m_AppID.Version.Language = TWLG_CHINESE_PRC;   
	m_AppID.Version.Country  = TWCY_CHINA;         
	lstrcpy(m_AppID.Version.Info, "UDS Scanner Tools");
	m_AppID.ProtocolMajor    = TWON_PROTOCOLMAJOR;
	m_AppID.ProtocolMinor    = TWON_PROTOCOLMINOR;
	m_AppID.SupportedGroups  = DG_IMAGE | DG_CONTROL;
	lstrcpy(m_AppID.Manufacturer, "UDS");        
	lstrcpy(m_AppID.ProductFamily, "Serials"); 
	lstrcpy(m_AppID.ProductName, "UDS Scanner Tools");  

	rc=(*m_pDSMEntry)(&m_AppID,
		               NULL,
		               DG_CONTROL,
		               DAT_PARENT,
		               MSG_OPENDSM,
		              (TW_MEMREF)&m_hWnd);
	if(rc==TWRC_SUCCESS)
	{
		rc = (*m_pDSMEntry)(&m_AppID, 
			                 NULL, 
			                 DG_CONTROL, 
			                 DAT_IDENTITY, 
			                 MSG_GETDEFAULT, 
			                (TW_MEMREF)&m_ScannerID1);  
		if (rc == TWRC_SUCCESS)
		{
			m_ScanInfo.bDSMOpened=true;
		}
		else
		{
			m_ScanInfo.bDSMOpened=false;
			AfxFreeLibrary(m_hTwain32Dll);
			m_hTwain32Dll=NULL;
			m_pDSMEntry=NULL;
		}
	}
	return m_ScanInfo.bDSMOpened;
}


bool CUDSTwain::GetInstalledDS()
{	
	if(!m_ScanInfo.bDSMOpened)
	{
		if(!OpenDSM())
		{
			return false;
		}
	}
	int         t=0;
	TW_UINT16   rc;
	TW_IDENTITY ScannerID;
	CString     strName, strDefault;

	rc=(*m_pDSMEntry)(&m_AppID,
		               NULL,
		               DG_CONTROL,
		               DAT_IDENTITY,
		               MSG_GETDEFAULT,
		              (TW_MEMREF)&ScannerID);
	if(rc!=TWRC_SUCCESS)
	{
		strDefault.Empty();
	}
	else 
	{
		strDefault=ScannerID.ProductName;
	}
	rc=(*m_pDSMEntry)(&m_AppID,
		               NULL,
		               DG_CONTROL,
		               DAT_IDENTITY,
		               MSG_GETFIRST,
		              (TW_MEMREF)&ScannerID);
	if(rc!=TWRC_SUCCESS)
	{
		return false;
	}
	while(rc == TWRC_SUCCESS)
	{
		strName = ScannerID.ProductName;   
		pDSNames[t]=strName;
		t++;
		rc=(*m_pDSMEntry)(&m_AppID,NULL,
			               DG_CONTROL,
			               DAT_IDENTITY,
			               MSG_GETNEXT,
			              (TW_MEMREF)&ScannerID);
	}
	m_scannernumbers = t;
	return true;
}


bool CUDSTwain::SelectScanner(CString ScannerName)
{
	if(m_ScanInfo.bDSOpened)
		CloseDS();
	if(!m_ScanInfo.bDSMOpened)
	{
		if(!OpenDSM())
			return false;
	}
	TW_UINT16     rc;
	int           sNo=0;
	TW_IDENTITY  *pScannerID;
	pScannerID = &m_ScannerID1;
	pScannerID->Id=0;
	pScannerID->ProductName[0]='\0';

	rc=(*m_pDSMEntry)(&m_AppID,
		               NULL,
		               DG_CONTROL,
		               DAT_IDENTITY,
		               MSG_GETDEFAULT,
		              (TW_MEMREF)pScannerID);
	ScannerName.MakeUpper();
	if(ScannerName=="DEFAULT")
	{
		rc=(*m_pDSMEntry)(&m_AppID,
			               NULL,
			               DG_CONTROL,
			               DAT_IDENTITY,
			               MSG_USERSELECT,
			              (TW_MEMREF)pScannerID);
	}
	else
	{
		rc=(*m_pDSMEntry)(&m_AppID,
			               NULL,
			               DG_CONTROL,
			               DAT_IDENTITY,
			               MSG_GETFIRST,
			              (TW_MEMREF)pScannerID);
		if(rc!=TWRC_SUCCESS)  
			return false;
		CString str;
//		while (m_scannernumbers>=0)
		int m_count2=10;
		while(m_count2!=0)
		{
			str=pScannerID->ProductName;
			str.MakeUpper();
			if(str.Find(ScannerName)>=0)
			{
				sNo=1;
				m_SaveScannerID=m_ScannerID1;
				break;
			}
			rc=(*m_pDSMEntry)(&m_AppID,
				               NULL,
				               DG_CONTROL,
				               DAT_IDENTITY,
				               MSG_GETNEXT,
				              (TW_MEMREF)pScannerID);
		//	m_scannernumbers--;
			m_count2--;
		}		
	}
	m_strSlcScanner = ScannerName;
	m_strSlcScanner.MakeUpper();
	return true;
}


bool CUDSTwain::OpenDS(void)
{
	if(m_ScanInfo.bDSOpened)
	{
		return false;
	}
	m_ScanInfo.bDSOpened=false;
	m_dsopen = false;
	if(!m_ScanInfo.bDSMOpened)
	{
		if(!OpenDSM())
			return m_ScanInfo.bDSMOpened;
	}
	m_ScannerID1 = m_SaveScannerID;
	m_pScannerID = &m_ScannerID1;
	
	TW_UINT16  rc;
	CString    str,strName;
	strName.Format("%s",m_pScannerID->ProductName);
	strName.MakeUpper();
	rc=(*m_pDSMEntry)(&m_AppID,
		               NULL,
		               DG_CONTROL,
		               DAT_IDENTITY,
		               MSG_OPENDS,
		               m_pScannerID);
	if(rc==TWRC_SUCCESS)
	{
		m_ScanInfo.bDSOpened=true;
		m_dsopen = true;		
	}
	else
	{
		m_pScannerID=&m_ScannerID1;
//		return m_ScanInfo.bDSOpened;		
	}	

	//设置传输模式
/*	TW_CAPABILITY  twCap;
	pTW_ONEVALUE   pval;
	twCap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE));
	twCap.Cap        = ICAP_XFERMECH;
	twCap.ConType    = TWON_ONEVALUE;

	pval = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
	pval->ItemType = TWTY_UINT16;
	pval->Item = TWSX_NATIVE;

	
	switch(m_transformation)
	{
	case 0:
		pval->Item = TWSX_NATIVE;
		break;
	case 1:
		pval->Item = TWSX_FILE;
		break;
	default:
		pval->Item = TWSX_NATIVE;
		break;
	}
	
	
	rc = (*m_pDSMEntry)(&m_AppID,
		                  m_pScannerID,
						  DG_CONTROL,
						  DAT_CAPABILITY,
						  MSG_SET,
						  (TW_MEMREF)&twCap);
	if (rc != TWRC_SUCCESS)
	{
		GlobalUnlock(twCap.hContainer);
		GlobalFree(twCap.hContainer);
		rc = (*m_pDSMEntry)(&m_AppID,
			                 NULL,
							 DG_CONTROL,
							 DAT_CAPABILITY,
							 MSG_CLOSEDS,
							 m_pScannerID);
		m_pScannerID = &m_ScannerID1;
		return m_ScanInfo.bDSOpened;
	}

	GlobalUnlock(twCap.hContainer);
	GlobalFree(twCap.hContainer);
	*/
	return m_ScanInfo.bDSOpened;
}


long CUDSTwain::EnableDS(void)
{
	if(!m_ScanInfo.bDSOpened)
		return UDS_SCANNER;
	
	TW_UINT16  rc;
	//ShowUI: true-showUI;false-providing;hParent:indicates the application’s window handle
	//ModalUI:To be set by the Application to TRUE or FALSE.	
	TW_USERINTERFACE ui;
	ui.ShowUI  = m_ScanInfo.bIsShowUI;
	
	ui.ModalUI = true;
	ui.hParent = m_hWnd;
	rc=(*m_pDSMEntry)(&m_AppID,
		               m_pScannerID,
		               DG_CONTROL,
		               DAT_USERINTERFACE,
		               MSG_ENABLEDS,
		              (TW_MEMREF)&ui);//P225
	
	if(rc!=TWRC_SUCCESS)
	{
		return UDS_SCANNER;
	}
	
	rc = TWRC_NOTDSEVENT;//meaning it is not a Source event
	bool bRet, bIsEnd=false;
	long nRe;
	TW_EVENT twEvent;
	MSG      msg;
	while((bRet=GetMessage(&msg,NULL,0,0))!=0)
	{
		twEvent.pEvent=(TW_MEMREF)&msg;
		twEvent.TWMessage=MSG_NULL;
		rc=(*m_pDSMEntry)(&m_AppID,
			               m_pScannerID,
			               DG_CONTROL,
			               DAT_EVENT,
			               MSG_PROCESSEVENT,
			              (TW_MEMREF)&twEvent);
		switch(twEvent.TWMessage)
		{
		case MSG_XFERREADY:
//			AfxMessageBox("1");
			if (m_transformation == 1)
			{
				m_nMutiTifName = m_totalimage;
				nRe = GetImageFileMode();
			
			}
			else
			{
				nRe =GetImage();
			}
			
			bIsEnd=true;			
			break;
		case MSG_CLOSEDSOK:
		case MSG_CLOSEDSREQ:
//			AfxMessageBox("2");
			nRe=UDS_SCANNER;
			bIsEnd=true;
			break;
		case MSG_DEVICEEVENT:
//			AfxMessageBox("3");
			nRe=UDS_SCANNER;
			bIsEnd=true;
			break;
		default:
			break;			
		}
		if(bIsEnd==true)
			break;
		if(rc!=TWRC_DSEVENT)
		{
			TranslateMessage(&msg);			
			DispatchMessage(&msg);		
		}
	}
	DisableDS();
	return nRe;
}


long CUDSTwain::EnableDSOnly(void)
{
	if(!m_ScanInfo.bDSOpened)
		return UDS_SCANNER;
	TW_UINT16   rc;
	TW_USERINTERFACE ui;
	ui.ShowUI  = m_ScanInfo.bIsShowUI;
	ui.ModalUI = true;
	ui.hParent = m_hWnd;
	rc=(*m_pDSMEntry)(&m_AppID,
		m_pScannerID,
		DG_CONTROL,
		DAT_USERINTERFACE,
		MSG_ENABLEDSUIONLY,
		(TW_MEMREF)&ui);//P225
	if(rc!=TWRC_SUCCESS)
	{
		return UDS_SCANNER;
	}
	rc = TWRC_NOTDSEVENT;//meaning it is not a Source event
	bool   bRet,bIsEnd=false;
	long   nRe;
	TW_EVENT twEvent;
	MSG      msg;
	while((bRet=GetMessage(&msg,NULL,0,0))!=0)
	{
		twEvent.pEvent=(TW_MEMREF)&msg;
		twEvent.TWMessage=MSG_NULL;
		rc=(*m_pDSMEntry)(&m_AppID,
			m_pScannerID,
			DG_CONTROL,
			DAT_EVENT,
			MSG_PROCESSEVENT,
			(TW_MEMREF)&twEvent);
		switch(twEvent.TWMessage)
		{
		case MSG_XFERREADY:
			bIsEnd=true;
			break;
		case MSG_CLOSEDSOK:
		case MSG_CLOSEDSREQ:
			nRe=UDS_SCANNER;
			bIsEnd=true;
			break;
		case MSG_DEVICEEVENT:
			nRe=UDS_SCANNER;
			bIsEnd=true;
			break;
		default:
			break;			
		}
		if(bIsEnd==true)
			break;
		if(rc!=TWRC_DSEVENT)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	DisableDS();
	return UDS_SUCCESS;
}


bool CUDSTwain::DisableDS(void)
{
	if(!m_ScanInfo.bDSOpened)
		return true;
	TW_UINT16        rc;
	TW_USERINTERFACE ui;
	ui.ShowUI=m_ScanInfo.bIsShowUI;
	ui.ModalUI=true;
	ui.hParent=m_hWnd;
	rc=(*m_pDSMEntry)(&m_AppID,
		               m_pScannerID,
		               DG_CONTROL,
		               DAT_USERINTERFACE,
		               MSG_DISABLEDS,
		              (TW_MEMREF)&ui);
	if (rc != TWRC_SUCCESS)
	{
		return false;
	}
	return true;
}


long CUDSTwain::GetImage(void)
{
	//本地传输
	int    cx = 0;
	int    cy = 0;   //缩略图宽高
	TW_UINT16        rc;
	TW_STATUS        m_Status;
	TW_PENDINGXFERS  m_End;
	TW_UINT32        hBitMap = NULL;               
	bool             retval;  
	m_ScanInfo.nImageScanned = 0;
	CString  msg, FileName, thFN, SumName, WholeName; 
	CString  tem_strThumbPath;        //缩略图路径
	CString  tem_strThumbName;        //缩略图名称，采用时间命名，避免条码分割中计数置零，命名重复
	CString  tem_strBarcodePath = _T("");
	m_End.Count = 1;
//	TW_IMAGEINFO        iInfo;        //用于文件传输
//	TW_SETUPFILEXFER    iSetupFile;   //设置文件传输
	do {  
		rc = (*m_pDSMEntry)(&m_AppID, 
			                 m_pScannerID, 
			                 DG_IMAGE, 
			                 DAT_IMAGEINFO, 
			                 MSG_GET, 
			                (TW_MEMREF)&m_ImageInfo);
		if (rc != TWRC_SUCCESS)
		{
			rc = (*m_pDSMEntry)(&m_AppID, 
				                 m_pScannerID, 
				                 DG_CONTROL,	
				                 DAT_PENDINGXFERS, 
				                 MSG_RESET, 
				                (TW_MEMREF)&m_End);
			return UDS_SUCCESS;
		}
		switch (m_ImageInfo.PixelType)
		{
		case TWPT_BW:
			break;
		case TWPT_GRAY:
			break;
		case TWPT_RGB:
			break;
		default:    
			rc = (*m_pDSMEntry)(&m_AppID, 
				                 m_pScannerID, 
				                 DG_CONTROL,	
				                 DAT_PENDINGXFERS, 
				                 MSG_RESET, 
				                (TW_MEMREF)&m_End);
			return UDS_APPLICATION;
		}

		hBitMap = NULL;
		rc = (*m_pDSMEntry)(&m_AppID, 
			                 m_pScannerID, 
			                 DG_IMAGE, 
			                 DAT_IMAGENATIVEXFER, 
			                 MSG_GET, 
			                (TW_MEMREF)&hBitMap);
		if (rc == TWRC_XFERDONE)
		{
			
//			SumName.Format(TEXT("%04d"),m_totalimage);       //1101修改为自定义命名
//			SumName = "IMG_" + SumName;
			
			m_totalimage++;   
			m_nImageCount++;
			//判断已经扫描数量，进行叠加累计

			SumName = NamingFile(Self_ReadCountInfo(m_totalimage, 2));             
			tem_strThumbName = Self_GetCurTime();
			CxImage       *pImage;
			pImage = new CxImage();  
			pImage->CreateFromHANDLE((HANDLE)hBitMap);
			GlobalFree( (HGLOBAL)hBitMap );  
			if ( pImage == NULL || !pImage->IsValid() )  
			{
				::delete pImage;  pImage = NULL;
				rc = (*m_pDSMEntry)(&m_AppID, 
					m_pScannerID, 
					DG_CONTROL,	
					DAT_PENDINGXFERS, 
					MSG_ENDXFER, 
					(TW_MEMREF)&m_End);
				rc = (*m_pDSMEntry)(&m_AppID, 
					m_pScannerID, 
					DG_CONTROL,	
					DAT_PENDINGXFERS, 
					MSG_RESET, 
					(TW_MEMREF)&m_End);
				return UDS_MEMORY;
			}
			retval = false;
			float    tem_fXDPI = 0;    //用于后面的自动裁切、旋转
			float    tem_fYDPI = 0;
			switch(m_format_index)
			{
			case 1:
				WholeName=SumName+".bmp";

				tem_strThumbPath  = m_strBufferPath;
				tem_strThumbPath += _T("~");
				tem_strThumbPath += tem_strThumbName;
				tem_strThumbPath += _T(".jpg");

//				FileName=m_result_path+WholeName;	
				FileName = m_strBarDocument + WholeName;
				pImage = AddWatermark(pImage, m_strSlcScanner);
				pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
				retval = pImage->Save(FileName, CXIMAGE_FORMAT_BMP);
// 				if (pImage->GetBpp()==8 && m_nGrayBit==1)
// 				{
// 					pImage->IncreaseBpp(10);
// 				}
//				retval = pImage->Save(FileName, CXIMAGE_FORMAT_BMP);

				m_strSlcScanner.MakeUpper();
//				if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
				if (m_BAutoCrop)
				{
					tem_fXDPI = pImage->GetXDPI();
					tem_fYDPI = pImage->GetYDPI();
					Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
					pImage->Load(FileName,CXIMAGE_FORMAT_BMP);
				}

				cx = 160;
				cy = (int)160*pImage->GetHeight()/pImage->GetWidth();
				if (pImage->GetBpp()==1)
				{
					pImage->IncreaseBpp(24);
				}
				pImage->Resample(cx, cy, 1, NULL);
				retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);

				tem_strBarcodePath = Self_BarCodeDivide(tem_strThumbPath, FileName, m_format_index);
				if (tem_strBarcodePath!=_T(""))
				{
					//更新SumName和FileName
					FileName  = tem_strBarcodePath;
					SumName   = PathFindFileName(tem_strBarcodePath);
				}

				g_vcCurName.push_back(SumName);
				g_vcCurPath.push_back(tem_strThumbPath);
				m_FileName.push_back(SumName);
				m_WholeName.push_back(tem_strThumbPath);
				m_BussName.push_back(FileName);
				break;
			case 3:
				if (pImage->GetBpp()==1)
				{
					WholeName=SumName+".jpg";

					tem_strThumbPath  = m_strBufferPath;
					tem_strThumbPath += _T("~");
					tem_strThumbPath += tem_strThumbName;
					tem_strThumbPath += _T(".jpg");

//					FileName=m_result_path+WholeName;
					FileName = m_strBarDocument + WholeName;
					pImage->IncreaseBpp(24);
					pImage->SetJpegQuality((BYTE)m_ScanInfo.nQuality);  
					pImage = AddWatermark(pImage, m_strSlcScanner);
					pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
					retval = pImage->Save(FileName, CXIMAGE_FORMAT_JPG);

					m_strSlcScanner.MakeUpper();
//					if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
					if (m_BAutoCrop)
					{
						tem_fXDPI = pImage->GetXDPI();
						tem_fYDPI = pImage->GetYDPI();
						Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
						pImage->Load(FileName,CXIMAGE_FORMAT_JPG);
					}
					

					cx = 160;
					cy = (int)160*pImage->GetHeight()/pImage->GetWidth();

					pImage->Resample(cx, cy, 1, NULL);
					retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);

					tem_strBarcodePath = Self_BarCodeDivide(tem_strThumbPath, FileName, m_format_index);
					if (tem_strBarcodePath!=_T(""))
					{
						//更新SumName和FileName
						FileName  = tem_strBarcodePath;
						SumName   = PathFindFileName(tem_strBarcodePath);
					}

					g_vcCurName.push_back(SumName);
					g_vcCurPath.push_back(tem_strThumbPath);
					m_FileName.push_back(SumName);
					m_WholeName.push_back(tem_strThumbPath);
					m_BussName.push_back(FileName);
					break;
				} 
				else
				{
					WholeName=SumName+".jpg";

					tem_strThumbPath  = m_strBufferPath;
					tem_strThumbPath += _T("~");
					tem_strThumbPath += tem_strThumbName;
					tem_strThumbPath += _T(".jpg");

//					FileName=m_result_path+WholeName;
					FileName = m_strBarDocument + WholeName;
					pImage->SetJpegQuality((BYTE)m_ScanInfo.nQuality);  
					pImage = AddWatermark(pImage, m_strSlcScanner);	
					pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
// 					if (pImage->GetBpp()==8 && m_nGrayBit==1)
// 					{
// 						pImage->IncreaseBpp(10);
// 					}
					pImage->SetXDPI(300);
					pImage->SetYDPI(300);
					retval = pImage->Save(FileName, CXIMAGE_FORMAT_JPG);

					m_strSlcScanner.MakeUpper();
//					if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
					if (m_BAutoCrop)
					{
						tem_fXDPI = pImage->GetXDPI();
						tem_fYDPI = pImage->GetYDPI();
						Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
//						Self_CropImg(_T("C:\\Users\\Administrator\\Desktop\\自动裁切\\IMG_0042.jpg"), 96, 96);
						pImage->Load(FileName,CXIMAGE_FORMAT_JPG);
					}
					
					
					
					cx = 160;
					cy = (int)160*pImage->GetHeight()/pImage->GetWidth();

					pImage->Resample(cx, cy, 1, NULL);
					retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);

					tem_strBarcodePath = Self_BarCodeDivide(tem_strThumbPath, FileName, m_format_index);
					if (tem_strBarcodePath!=_T(""))
					{
						//更新SumName和FileName
						FileName  = tem_strBarcodePath;
						SumName   = PathFindFileName(tem_strBarcodePath);
					}

					g_vcCurName.push_back(SumName);
					g_vcCurPath.push_back(tem_strThumbPath);
					m_FileName.push_back(SumName);
					m_WholeName.push_back(tem_strThumbPath);
					m_BussName.push_back(FileName);
					break;
				}		
			case 50:
				if (pImage->GetBpp()==1)
				{
					WholeName=SumName+".jpg";
//					FileName  = m_result_path;
					FileName = m_strBufferPath;
					FileName += _T("~");
					FileName += SumName;
					FileName += _T(".jpg");

					pImage->IncreaseBpp(24);
					pImage->SetJpegQuality((BYTE)m_ScanInfo.nQuality);  
					pImage = AddWatermark(pImage, m_strSlcScanner);
					pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
					retval = pImage->Save(FileName, CXIMAGE_FORMAT_JPG);

					m_strSlcScanner.MakeUpper();
//					if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
					if (m_BAutoCrop)
					{
						tem_fXDPI = pImage->GetXDPI();
						tem_fYDPI = pImage->GetYDPI();
						Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
						pImage->Load(FileName,CXIMAGE_FORMAT_JPG);
					}

// 					cx = 160;
// 					cy = (int)160*pImage->GetHeight()/pImage->GetWidth();
// 
// 					pImage->Resample(cx, cy, 1, NULL);
// 					retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);


					//1113-PDF扫描不做分割
// 					tem_strBarcodePath = Self_BarCodeDivide(FileName, m_format_index);
// 					if (tem_strBarcodePath!=_T(""))
// 					{
// 						//更新SumName和FileName
// 						FileName  = tem_strBarcodePath;
// 						SumName   = PathFindFileName(tem_strBarcodePath);
// 					}

					g_vcCurName.push_back(SumName);
					g_vcCurPath.push_back(FileName);
					m_FileName.push_back(SumName);
					m_WholeName.push_back(FileName);
					break;
				} 
				else
				{
					WholeName=SumName+".jpg";

//					FileName  = m_result_path;
					FileName = m_strBufferPath;
					FileName += _T("~");
					FileName += SumName;
					FileName += _T(".jpg");

					pImage->SetJpegQuality((BYTE)m_ScanInfo.nQuality);  
					pImage = AddWatermark(pImage, m_strSlcScanner);
					pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
					retval = pImage->Save(FileName, CXIMAGE_FORMAT_JPG);

					m_strSlcScanner.MakeUpper();
//					if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
					if (m_BAutoCrop)
					{
						tem_fXDPI = pImage->GetXDPI();
						tem_fYDPI = pImage->GetYDPI();
						Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
						pImage->Load(FileName,CXIMAGE_FORMAT_JPG);
					}

// 					cx = 160;
// 					cy = (int)160*pImage->GetHeight()/pImage->GetWidth();
// 
// 					pImage->Resample(cx, cy, 1, NULL);
// 					retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);

					//1113pdf扫描不做分割
//					tem_strBarcodePath = Self_BarCodeDivide(FileName, m_format_index);
// 					if (tem_strBarcodePath!=_T(""))
// 					{
// 						//更新SumName和FileName
// 						FileName  = tem_strBarcodePath;
// 						SumName   = PathFindFileName(tem_strBarcodePath);
// 					}

					g_vcCurName.push_back(SumName);
					g_vcCurPath.push_back(FileName);
					m_FileName.push_back(SumName);
					m_WholeName.push_back(FileName);
					break;
				}		
			case 4:
				WholeName=SumName+".png";

				tem_strThumbPath  = m_strBufferPath;
				tem_strThumbPath += _T("~");
				tem_strThumbPath += tem_strThumbName;
				tem_strThumbPath += _T(".jpg");

//				FileName=m_result_path+WholeName;
				FileName = m_strBarDocument + WholeName;
				pImage = AddWatermark(pImage, m_strSlcScanner);
				pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
// 				if (pImage->GetBpp()==8 && m_nGrayBit==1)
// 				{
// 					pImage->IncreaseBpp(10);
// 				}
				retval = pImage->Save(FileName, CXIMAGE_FORMAT_PNG);

				m_strSlcScanner.MakeUpper();
//				if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
				if (m_BAutoCrop)
				{
					tem_fXDPI = pImage->GetXDPI();
					tem_fYDPI = pImage->GetYDPI();
					Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
					pImage->Load(FileName, CXIMAGE_FORMAT_PNG);
				}
				

				cx = 160;
				cy = (int)160*pImage->GetHeight()/pImage->GetWidth();
				if (pImage->GetBpp()==1)
				{
					pImage->IncreaseBpp(24);
				}
				pImage->Resample(cx, cy, 1, NULL);
				retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);

				tem_strBarcodePath = Self_BarCodeDivide(tem_strThumbPath, FileName, m_format_index);
				if (tem_strBarcodePath!=_T(""))
				{
					//更新SumName和FileName
					FileName  = tem_strBarcodePath;
					SumName   = PathFindFileName(tem_strBarcodePath);
				}

				g_vcCurName.push_back(SumName);
				g_vcCurPath.push_back(tem_strThumbPath);
				m_FileName.push_back(SumName);
				m_WholeName.push_back(tem_strThumbPath);
				m_BussName.push_back(FileName);
				break;
			case 6:
				WholeName=SumName+".tif";

				tem_strThumbPath  = m_strBufferPath;
				tem_strThumbPath += _T("~");
				tem_strThumbPath += tem_strThumbName;
				tem_strThumbPath += _T(".jpg");

//				FileName=m_result_path+WholeName;
				FileName = m_strBarDocument + WholeName;
				pImage->SetCodecOption(5, CXIMAGE_FORMAT_TIF);  
//				pImage->SetCodecOption(7, CXIMAGE_FORMAT_TIF);
				pImage = AddWatermark(pImage, m_strSlcScanner);
// 				if (pImage->GetBpp()==8 && m_nGrayBit==1)
// 				{
// 					pImage->IncreaseBpp(10);
// 				}
				pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
				retval = pImage->Save(FileName, CXIMAGE_FORMAT_TIF);

				m_strSlcScanner.MakeUpper();
//				if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
				if (m_BAutoCrop)
				{
					tem_fXDPI = pImage->GetXDPI();
					tem_fYDPI = pImage->GetYDPI();
					Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
					pImage->Load(FileName, CXIMAGE_FORMAT_TIF);
				}	

				cx = 160;
				cy = (int)160*pImage->GetHeight()/pImage->GetWidth();
				if (pImage->GetBpp()==1)
				{
					pImage->IncreaseBpp(24);
				}
				pImage->Resample(cx, cy, 1, NULL);
				retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);

				tem_strBarcodePath = Self_BarCodeDivide(tem_strThumbPath, FileName, m_format_index);
				if (tem_strBarcodePath!=_T(""))
				{
					//更新SumName和FileName
					FileName  = tem_strBarcodePath;
					SumName   = PathFindFileName(tem_strBarcodePath);
				}

				g_vcCurName.push_back(SumName);
				g_vcCurPath.push_back(tem_strThumbPath);
				m_FileName.push_back(SumName);
				m_WholeName.push_back(tem_strThumbPath);
				m_BussName.push_back(FileName);
				break;
			default:
				WholeName=SumName+".tif";

				tem_strThumbPath  = m_strBufferPath;
				tem_strThumbPath += _T("~");
				tem_strThumbPath += tem_strThumbName;
				tem_strThumbPath += _T(".jpg");

//				FileName=m_result_path+WholeName;
				FileName = m_strBarDocument + WholeName;
				pImage->SetCodecOption(5, CXIMAGE_FORMAT_TIF);  
				pImage = AddWatermark(pImage, m_strSlcScanner);
				pImage = Self_AddPageNum(pImage, m_nPageSite, m_nPageReset, m_BPageOpen);
// 				if (pImage->GetBpp()==8 && m_nGrayBit==1)
// 				{
// 					pImage->IncreaseBpp(10);
// 				}
				retval = pImage->Save(FileName, CXIMAGE_FORMAT_TIF);

				m_strSlcScanner.MakeUpper();
//				if (m_strSlcScanner.Find("KODAK") >= 0 || m_BAutoCrop) 
				if (m_BAutoCrop)
				{
					tem_fXDPI = pImage->GetXDPI();
					tem_fYDPI = pImage->GetYDPI();
					Self_CropImg(FileName, tem_fXDPI, tem_fYDPI);
					pImage->Load(FileName, CXIMAGE_FORMAT_TIF);
				}

				cx = 160;
				cy = (int)160*pImage->GetHeight()/pImage->GetWidth();
				if (pImage->GetBpp()==1)
				{
					pImage->IncreaseBpp(24);
				}
				pImage->Resample(cx, cy, 1, NULL);
				retval = pImage->Save(tem_strThumbPath, CXIMAGE_FORMAT_JPG);

				tem_strBarcodePath = Self_BarCodeDivide(tem_strThumbPath, FileName, m_format_index);
				if (tem_strBarcodePath!=_T(""))
				{
					//更新SumName和FileName
					FileName  = tem_strBarcodePath;
					SumName   = PathFindFileName(tem_strBarcodePath);
				}

				g_vcCurName.push_back(SumName);
				g_vcCurPath.push_back(tem_strThumbPath);
				m_FileName.push_back(SumName);
				m_WholeName.push_back(tem_strThumbPath);
				m_BussName.push_back(FileName);
				break;
			}

			//			pImage->FreeMemory();
			::delete pImage;
			if ( !retval )                                          
			{
				rc = (*m_pDSMEntry)(&m_AppID, 
					m_pScannerID, 
					DG_CONTROL,	
					DAT_PENDINGXFERS, 
					MSG_ENDXFER, 
					(TW_MEMREF)&m_End);
				rc = (*m_pDSMEntry)(&m_AppID, 
					m_pScannerID, 
					DG_CONTROL,	
					DAT_PENDINGXFERS, 
					MSG_RESET, 
					(TW_MEMREF)&m_End);
				return UDS_DISK;
			}	
			m_ScanInfo.nImageScanned ++;  
			// End Transfer
			rc = (*m_pDSMEntry)(&m_AppID,  
				m_pScannerID, 
				DG_CONTROL, 
				DAT_PENDINGXFERS, 
				MSG_ENDXFER, 
				(TW_MEMREF)&m_End);
// 			if ( m_End.Count > 0  )  // 用户终止扫描
// 			{
// 
// 				rc = (*m_pDSMEntry)(&m_AppID, 
// 					m_pScannerID, 
// 					DG_CONTROL,	
// 					DAT_PENDINGXFERS, 
// 					0x0702, 
// 					(TW_MEMREF)&m_End);
// 			}
		}
		else if (rc == TWRC_CANCEL)
		{
			AfxMessageBox("TWRC_CANCEL");
			if (hBitMap != NULL)  GlobalFree( (HGLOBAL)hBitMap );  // 释放hBitmap
			rc = (*m_pDSMEntry)(&m_AppID, 
				m_pScannerID, 
				DG_CONTROL, 
				DAT_PENDINGXFERS, 
				MSG_ENDXFER, 
				(TW_MEMREF)&m_End);
			rc = (*m_pDSMEntry)(&m_AppID, 
				m_pScannerID, 
				DG_CONTROL,	
				DAT_PENDINGXFERS, 
				MSG_RESET, 
				(TW_MEMREF)&m_End);
			return UDS_NOPAPER;
		}
		else //if (rc == TWRC_FAILURE)
		{
			AfxMessageBox("TWRC_FAILURE");
			if (hBitMap != NULL)  GlobalFree( (HGLOBAL)hBitMap );  
			rc = (*m_pDSMEntry)(&m_AppID, 
				                 m_pScannerID, 
				                 DG_CONTROL, 
				                 DAT_STATUS , 
				                 MSG_GET, 
				                (TW_MEMREF)&m_Status);
			rc = (*m_pDSMEntry)(&m_AppID, 
				m_pScannerID, 
				DG_CONTROL,	
				DAT_PENDINGXFERS, 
				MSG_RESET, 
				(TW_MEMREF)&m_End);
			if (rc == TWRC_SUCCESS)  
			{
				switch (m_Status.ConditionCode)
				{
				case TWCC_PAPERJAM:
					return m_ScanInfo.nImageScanned;
				case TWCC_PAPERDOUBLEFEED:
					return m_ScanInfo.nImageScanned;
				case TWCC_LOWMEMORY:
					return UDS_MEMORY;
				default:
					return UDS_SCANNER;
				}
			}
			return UDS_SCANNER;
		}

	} while (m_End.Count > 0);  // 循环取图像，循环条件m_End.Count > 0

	rc = (*m_pDSMEntry)(&m_AppID, 
		m_pScannerID, 
		DG_CONTROL,	
		DAT_PENDINGXFERS, 
		MSG_RESET, 
		(TW_MEMREF)&m_End);
	return m_ScanInfo.nImageScanned;
}


long CUDSTwain::GetImageFileMode(void)
{
	//文件传输
	TW_UINT16        rc;
	TW_STATUS        m_Status;
	TW_PENDINGXFERS  m_End;
	TW_UINT32        hBitMap = NULL;               
	bool             retval;  
	m_ScanInfo.nImageScanned = 0;
	CString  msg, FileName, thFN, SumName, WholeName;  
	TW_SETUPFILEXFER    iSetupFile;   //设置文件传输
	do {  
		rc = (*m_pDSMEntry)(&m_AppID, 
			m_pScannerID, 
			DG_IMAGE, 
			DAT_IMAGEINFO, 
			MSG_GET, 
			(TW_MEMREF)&m_ImageInfo);
		if (rc != TWRC_SUCCESS)
		{
			rc = (*m_pDSMEntry)(&m_AppID, 
				m_pScannerID, 
				DG_CONTROL,	
				DAT_PENDINGXFERS, 
				MSG_RESET, 
				(TW_MEMREF)&m_End);
			return UDS_SUCCESS;
		}
		long  m_nDPI = (long)(m_ImageInfo.XResolution.Whole);
		switch (m_ImageInfo.PixelType)
		{
		case TWPT_BW:
			iSetupFile.Format = TWFF_TIFF;
			break;
		case TWPT_GRAY:
			iSetupFile.Format = TWFF_JFIF;
			break;
		case TWPT_RGB:
			iSetupFile.Format = TWFF_JFIF;
			break;
		}
		m_totalimage++;  
		SumName.Format("%d", m_totalimage);

		switch(m_nTransType)
		{
			//文件命名
		case 0:
				iSetupFile.Format = TWFF_BMP;
				WholeName = SumName +".bmp";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;
			case 1:
				iSetupFile.Format = TWFF_TIFF;
				WholeName = SumName +".tif";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;
			case 2:
				iSetupFile.Format = TWFF_JFIF;
				WholeName = SumName +".jpg";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;
			case 3:
				iSetupFile.Format = TWFF_PNG;
				WholeName = SumName +".png";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;
			case 4:
				iSetupFile.Format = TWFF_PDF;
				WholeName = SumName +".pdf";
				FileName =m_result_path+WholeName;
				break;
			case 5:
				iSetupFile.Format = TWFF_BASE64;
				WholeName = SumName +".txt";
				FileName =m_result_path+WholeName;
				break;
			case 6:
				iSetupFile.Format = TWFF_GIF;
				WholeName = SumName +".gif";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;
			case 7:
				iSetupFile.Format = TWFF_TIFFMULTI;
				SumName.Format("%d", m_nMutiTifName);
				WholeName = SumName +".tif";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;
			case 8:
				iSetupFile.Format = TWFF_JP2;
				WholeName = SumName +".jp2";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;
			default:
				iSetupFile.Format = TWFF_BMP;
				WholeName = SumName +".bmp";
				FileName =m_result_path+WholeName;
				m_FileName.push_back(WholeName);
				break;

		}
		lstrcpy(iSetupFile.FileName, FileName.GetBuffer(0));
		rc = (*m_pDSMEntry)(&m_AppID, m_pScannerID, DG_CONTROL, DAT_SETUPFILEXFER, MSG_SET, (TW_MEMREF)&iSetupFile);
		rc = (*m_pDSMEntry)(&m_AppID, m_pScannerID, DG_IMAGE, DAT_IMAGEFILEXFER, MSG_GET, NULL);

		

		if (rc == TWRC_XFERDONE)
		{
			
			rc = (*m_pDSMEntry)(&m_AppID, m_pScannerID, DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, (TW_MEMREF)&m_End);
			
			if ( m_End.Count > 0  )  // 用户终止扫描
			{

				rc = (*m_pDSMEntry)(&m_AppID, 
					m_pScannerID, 
					DG_CONTROL,	
					DAT_PENDINGXFERS, 
					0x0702, 
					(TW_MEMREF)&m_End);
			}
		}


		//传输分割线----------------------------------------------------------------------
		else if (rc == TWRC_CANCEL)
		{
			AfxMessageBox("TWRC_CANCEL");
			if (hBitMap != NULL)  GlobalFree( (HGLOBAL)hBitMap );  // 释放hBitmap
			rc = (*m_pDSMEntry)(&m_AppID, 
				                 m_pScannerID, 
				                 DG_CONTROL, 
				                 DAT_PENDINGXFERS, 
				                 MSG_ENDXFER, 
			                 	(TW_MEMREF)&m_End);
			rc = (*m_pDSMEntry)(&m_AppID, 
				                 m_pScannerID, 
				                 DG_CONTROL,	
				                 DAT_PENDINGXFERS, 
				                 MSG_RESET, 
				                (TW_MEMREF)&m_End);
			return UDS_NOPAPER;
		}
		else //if (rc == TWRC_FAILURE)
		{
			AfxMessageBox("TWRC_FAILURE");
			if (hBitMap != NULL)  GlobalFree( (HGLOBAL)hBitMap );  
			rc = (*m_pDSMEntry)(&m_AppID, 
				                 m_pScannerID, 
				DG_CONTROL, 
				DAT_STATUS , 
				MSG_GET, 
				(TW_MEMREF)&m_Status);
			rc = (*m_pDSMEntry)(&m_AppID, 
				m_pScannerID, 
				DG_CONTROL,	
				DAT_PENDINGXFERS, 
				MSG_RESET, 
				(TW_MEMREF)&m_End);
			if (rc == TWRC_SUCCESS)  
			{
				switch (m_Status.ConditionCode)
				{
				case TWCC_PAPERJAM:
					return m_ScanInfo.nImageScanned;
				case TWCC_PAPERDOUBLEFEED:
					return m_ScanInfo.nImageScanned;
				case TWCC_LOWMEMORY:
					return UDS_MEMORY;
				default:
					return UDS_SCANNER;
				}
			}
			return UDS_SCANNER;
		}

	} while (m_End.Count > 0);  // 循环取图像，循环条件m_End.Count > 0

	rc = (*m_pDSMEntry)(&m_AppID, 
		m_pScannerID, 
		DG_CONTROL,	
		DAT_PENDINGXFERS, 
		MSG_RESET, 
		(TW_MEMREF)&m_End);
	return m_ScanInfo.nImageScanned;
}


bool CUDSTwain::CloseDS(void)
{
	if(m_ScanInfo.bDSOpened)
	{
		TW_UINT16 rc;
		rc=(*m_pDSMEntry)(&m_AppID,
			               NULL,
			               DG_CONTROL,
			               DAT_IDENTITY,
			               MSG_CLOSEDS,
			               m_pScannerID);
		m_ScanInfo.bDSOpened=false;
	}
	m_pScannerID=&m_ScannerID1;
	return true;
}


bool CUDSTwain::CloseDSM(void)
{
	if(m_ScanInfo.bDSOpened)
		CloseDS();
	TW_UINT16 rc;
	if(m_ScanInfo.bDSMOpened&&m_pDSMEntry!=NULL)
	{
		rc=(*m_pDSMEntry)(&m_AppID,
			               NULL,
			               DG_CONTROL,
			               DAT_PARENT,
			               MSG_CLOSEDSM,
			              (TW_MEMREF)&m_hWnd);
		if(m_hTwain32Dll!=NULL)
		{
			AfxFreeLibrary(m_hTwain32Dll);
			m_hTwain32Dll=NULL;
		}
		m_pDSMEntry=NULL;
	}
	m_pDSMEntry=NULL;
	m_ScanInfo.bDSMOpened=false;
	return true;
}


CxImage* CUDSTwain::AddWatermark(CxImage* img, CString scanner)
{
	//判断设备是否合法
	if (scanner.IsEmpty())
	{
		return img;
	}
 	if (scanner.Find("G4000") >= 0||scanner.Find("G7100") >= 0||scanner.Find("UDS") >= 0 ||scanner.Find("A SERIES") >= 0 ||scanner.Find("B8100") >= 0 ||scanner.Find("AD250F") >= 0 ||scanner.Find("G6400") >= 0 ||scanner.Find("G6600") >= 0 ||scanner.Find("G8700") >= 0 ||scanner.Find("G8800") >= 0 ||
 		scanner.Find("GL") >= 0 ||scanner.Find("GH") >= 0 ||scanner.Find("S410") >= 0 ||scanner.Find("S310") >= 0 ||scanner.Find("CK410") >= 0 ||scanner.Find("CK310") >= 0 ||scanner.Find("BJ1000") >= 0 ||scanner.Find("SMARTSCAN-JHK") >= 0 ||scanner.Find("UDS B SERIES") >= 0 ||scanner.Find("UDS H SERIES SCANNER") >= 0 || 
		scanner.Find("UDS A SERIES SCANNER") >= 0 || scanner.Find("T SERIES") >= 0 || scanner.Find("UDS N SERIES SCANNER") >= 0 || scanner.Find("UDS H SERIES") >= 0 || scanner.Find("UDS A SERIES") >= 0||scanner.Find("UDS T SERIES") >= 0 || scanner.Find("UDS N SERIES") >= 0 || scanner.Find("T SERIES") >= 0 || scanner.Find("KODAK") >= 0) 
	{
		return img;
	}
	else
	{
		//方法一：水印格式一
		CxImage::CXTEXTINFO WMTxtInfo;  // 授权文字水印
		img->InitTextInfo( &WMTxtInfo );
		_tcscpy( WMTxtInfo.text, _T("UDS Sample") );
		WMTxtInfo.lfont.lfHeight = 100;
		WMTxtInfo.lfont.lfItalic = TRUE;
		WMTxtInfo.fcolor = RGB(0, 0, 0);
		WMTxtInfo.opaque = FALSE;  // No background
		img->DrawStringEx(0, img->GetWidth()/3, img->GetHeight()/3, &WMTxtInfo);
		img->DrawStringEx(0, img->GetWidth()*2/3, img->GetHeight()*2/3, &WMTxtInfo);
	}

	return img;
}


void CUDSTwain::ReadIniInfo()
{
	//1、配置文件路径
	CString        tem_strMyDocument = _T("");
	TCHAR          tem_cMyDocument[MAX_PATH] = {0};
	LPITEMIDLIST   tem_Pid = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &tem_Pid);
	if (tem_Pid && SHGetPathFromIDList(tem_Pid, tem_cMyDocument))
	{
		tem_strMyDocument = tem_cMyDocument;
		
	}
	m_strIniPath = tem_strMyDocument;
	m_strIniPath += _T("\\UDSData\\");
	m_strIniPath += _T("config.ini");

	//2、读取配置文件
	int      tem_nNameInfo   = 0;
	CString  tem_strNameInfo = _T("");

	::GetPrivateProfileString("SaveSet", "NameMode", "没有找到NameMode信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nNameInfo = _ttoi(tem_strNameInfo);
	m_nNameMode   = tem_nNameInfo;
	tem_strNameInfo.ReleaseBuffer();
	if (m_nNameMode == 2)
	{
		m_totalimage = 0;   //自定义命名，修改后命名重置
	}

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

	//页码设置
	CString   tem_strPageInfo = _T("");
	::GetPrivateProfileString("SaveSet", "PageOpen", "没有找到PageOpen信息", tem_strPageInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_nPageOpen = _ttoi(tem_strPageInfo);
	tem_strPageInfo.ReleaseBuffer();
	if (m_nPageOpen==1)
	{
		m_BPageOpen = TRUE;
	} 
	else
	{
		m_BPageOpen = FALSE;
	}

	::GetPrivateProfileString("SaveSet", "PageSite", "没有找到PageSite信息", tem_strPageInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_nPageSite = _ttoi(tem_strPageInfo);
	tem_strPageInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "PageReset", "没有找到PageReset信息", tem_strPageInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_nPageReset = _ttoi(tem_strPageInfo);
	tem_strPageInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "PageSize", "没有找到PageSize信息", tem_strPageInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_nPageSize = _ttoi(tem_strPageInfo);
	tem_strPageInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "GrayBit", "没有找到GrayBit信息", tem_strPageInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_nGrayBit = _ttoi(tem_strPageInfo);
	tem_strPageInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "AutoCropCorrect", "没有找到AutoCropCorrect信息", tem_strPageInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	if (tem_strPageInfo == "1")
	{
		m_BAutoCrop = TRUE;
	} 
	else
	{
		m_BAutoCrop = FALSE;
	}

}


CString CUDSTwain::NamingFile(int count)
{
	int       tem_nOffset      = count;
	
	CString   tem_strCount     = _T("");
	CString   tem_strBegin     = _T("");

	SYSTEMTIME   tem_stDateTime;
	CString      tem_strDate = _T("");
	CString      tem_strTime = _T("");
	CString      tem_strDivide = _T("");


	CString   tem_strOrderName = _T("IMG_");   //次序命名
	CString   tem_strTimeName  = _T("");       //时间命名
	CString   tem_strCustomName= _T("");       //自定义名

	CString   tem_strNewName   = _T("");       //返回名称

	switch(m_nNameMode)
	{
	case 0:
		//次序命名
		tem_strCount.Format(_T("%04d"), count);
		tem_strOrderName += tem_strCount;
		tem_strNewName    = tem_strOrderName;
		break;
	case 1:
		//时间命名
		GetLocalTime(&tem_stDateTime);
		if (m_nNameDate == 0)
		{
			tem_strDate.Format(_T("%d%02d%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay);
		} 
		else if (m_nNameDate == 1)
		{
			tem_strDate.Format(_T("%d.%02d.%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay);
		} 
		else if (m_nNameDate == 2)
		{
			tem_strDate.Format(_T("%d-%02d-%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay);
		} 
		else if (m_nNameDate == 3)
		{
			tem_strDate.Format(_T("%d_%02d_%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay);
		} 
		else
		{
			tem_strDate.Format(_T("%d%02d%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay);
		}
		

		if (m_nNameTime == 0)
		{
			tem_strTime.Format(_T("%02d%02d%02d%02d"), tem_stDateTime.wHour, tem_stDateTime.wMinute, tem_stDateTime.wSecond, tem_stDateTime.wMilliseconds);
		} 
		else if (m_nNameTime == 1)
		{
			tem_strTime.Format(_T("%02d.%02d.%02d.%02d"), tem_stDateTime.wHour, tem_stDateTime.wMinute, tem_stDateTime.wSecond, tem_stDateTime.wMilliseconds);
		} 
		else if (m_nNameTime == 2)
		{
			tem_strTime.Format(_T("%02d-%02d-%02d-%02d"), tem_stDateTime.wHour, tem_stDateTime.wMinute, tem_stDateTime.wSecond, tem_stDateTime.wMilliseconds);
		} 
		else if (m_nNameTime == 3)
		{
			tem_strTime.Format(_T("%02d_%02d_%02d_%02d"), tem_stDateTime.wHour, tem_stDateTime.wMinute, tem_stDateTime.wSecond, tem_stDateTime.wMilliseconds);
		} 
		else
		{
			tem_strTime.Format(_T("%02d%02d%02d%02d"), tem_stDateTime.wHour, tem_stDateTime.wMinute, tem_stDateTime.wSecond, tem_stDateTime.wMilliseconds);
		}
		tem_strTimeName  = tem_strDate;
		tem_strTimeName += m_strNameDivide;
		tem_strTimeName += tem_strTime;
		tem_strNewName   = tem_strTimeName;

		break;
	case 2:
		//自定义命名
		tem_nOffset += m_nNameBegin;
		tem_strBegin.Format(_T("%04d"), tem_nOffset);

		tem_strCustomName  = m_strNamePre1;
		tem_strCustomName += m_strNamePre2;
		tem_strCustomName += m_strNamePre3;
		tem_strCustomName += tem_strBegin;
		tem_strNewName     = tem_strCustomName;
		break;
	default:
		tem_strCount.Format(_T("%04d"), count);
		tem_strOrderName += tem_strCount;
		tem_strNewName    = tem_strOrderName;
		break;
	}


	return tem_strNewName;
}


//1110条码分割
CString CUDSTwain::BarcodeReader(HANDLE hBitmap, CString strFileName , CString strType , CString strFormat , long double nLength, bool bBarcodeRotate)
{
	// TODO: Add your control notification handler code here
	
	CString strBarcode = "";
// 	if (theApp.BarcodeEvent == false)  // 不需要条码识别，直接返回"***"
// 		return strBarcode;
	//AfxMessageBox(strType+"-"+strFormat);

	// Call Softek DLL to read barcode
	HANDLE hBarcode = mtCreateBarcodeInstance() ;
	//mtSetLicenseKey(hBarcode, "8H34A60EC2ZS602B012J7BE7MMSX42F5") ;  // version 7.4.1.6
	mtSetLicenseKey(hBarcode, "8Y34BCJWW6M0DCSC012JKIJ1PLC29DOC") ;  // version 7.5.1.10
	// Turn off all barcode types first
	mtSetReadCode128 (hBarcode, 1) ;
	mtSetReadCode39 (hBarcode, 1) ;
	mtSetReadCode25 (hBarcode, 0) ;
	mtSetReadEAN13 (hBarcode, 0) ;
	mtSetReadEAN8 (hBarcode, 0) ;
	mtSetReadUPCA (hBarcode, 0) ;
	mtSetReadUPCE (hBarcode, 0) ;
	mtSetReadCodabar (hBarcode, 0) ;
	mtSetReadPDF417 (hBarcode, 0) ;
	mtSetReadDataMatrix (hBarcode, 0) ;
	mtSetReadDatabar (hBarcode, 0);
	mtSetReadMicroPDF417 (hBarcode, 0);
	mtSetReadQRCode (hBarcode, 0);

	// Turn on barcode types defined
	if (strType.Find("128") >= 0 || strType.Find("ALL") >= 0)
		mtSetReadCode128 (hBarcode, 1) ;
	if (strType.Find("39") >= 0 || strType.Find("ALL") >= 0)
		mtSetReadCode39 (hBarcode, 1) ;
	if (strType.Find("25") >= 0 || strType.Find("ALL") >= 0)
		mtSetReadCode25 (hBarcode, 1) ;
	if (strType.Find("EAN") >= 0 || strType.Find("ALL") >= 0)
	{
		mtSetReadEAN13 (hBarcode, 1) ;
		mtSetReadEAN8 (hBarcode, 1) ;
	}
	if (strType.Find("PDF417") >= 0 || strType.Find("ALL") >= 0)
		mtSetReadPDF417 (hBarcode, 1) ;
	if (strType.Find("DM") >= 0 || strType.Find("ALL") >= 0)
		mtSetReadDataMatrix (hBarcode, 1) ;
	if (strType.Find("QR") >= 0 || strType.Find("ALL") >= 0)
		mtSetReadQRCode (hBarcode, 1);

    // Databar Options is a mask that controls which type of databar barcodes will be read and whether or not
    // the software will look for a quiet zone around the barcode.
    // 1 = 2D-Linkage flag (handle micro-PDf417 barcodes as supplementary data - requires ReadMicroPDF417 to be true).
    // 2 = Read RSS14
    // 4 = Read RSS14 Stacked
    // 8 = Read Limited
    // 16 = Read Expanded
    // 32 = Read Expanded Stacked
    // 64 = Require quiet zone
    mtSetDatabarOptions (hBarcode, 255);

	// If you want to read more than one barcode then set Multiple Read to 1
	// Setting MutlipleRead to False will make the recognition faster
	mtSetMultipleRead (hBarcode, 1) ;

	// Noise reduction takes longer but can make it possible to read some difficult barcodes
	// When using noise reduction a typical value is 10 - the smaller the value the more effect it has.
	// A zero value turns off noise reduction. 
	// If you use NoiseReduction then the ScanDirection mask must be either only horizontal or only
	// vertical (i.e 1, 2, 4, 8, 5 or 10).
	mtSetNoiseReduction (hBarcode, 0) ;

	// You may need to set a small quiet zone if your barcodes are close to text and pictures in the image.
	// A value of zero uses the default.
	mtSetQuietZoneSize (hBarcode, 0) ;

	// LineJump controls the frequency at which scan lines in an image are sampled.
	// The default is 1 - decrease this for difficult barcodes.
	mtSetLineJump (hBarcode, 1) ;

	// You can restrict your search to a particular area of the image if you wish.
	// stSetSetScanRect(TopLeftX, TopLeftY, BottomRightX, BottomRightY, 0)

	// Set the direction that the barcode reader should scan for barcodes
	// The value is a mask where 1 = Left to Right, 2 = Top to Bottom, 3 = Right To Left, 4 = Bottom to Top
	mtSetScanDirection (hBarcode, 15) ;
	//mtSetScanDirection (hBarcode, 5) ;
	//mtSetScanDirection (hBarcode, 10) ;

	// SkewTolerance controls the angle of skew that the barcode toolkit will tolerate. By default
	// the toolkit checks for barcodes along horizontal and vertical lines in an image. This works 
	// OK for most barcodes because even at an angle it is possible to pass a line through the entire
	// length. SkewTolerance can range from 0 to 5 and allows for barcodes skewed to an angle of 45
	// degrees.
	mtSetSkewTolerance (hBarcode, 0) ;

    // ColorProcessingLevel controls how much time the toolkit will searching a color image for a barcode.
    // The default value is 2 and the range of values is 0 to 5. If ColorThreshold is non-zero then 
    // ColorProcessingLevel is effectively set to 0.
    mtSetColorProcessingLevel (hBarcode, 2);

	// MaxLength and MinLength can be used to specify the number of characters you expect to
	// find in a stSet This can be useful to increase accuracy or if you wish to ignore some
	// barcodes in an image.
	if (nLength > 0)  // 指定条码长度
	{
		mtSetMinLength (hBarcode, nLength) ;
		mtSetMaxLength (hBarcode, nLength) ;
	}
	else  // 未指定: 4-999
	{
		mtSetMinLength (hBarcode, 4) ;
		mtSetMaxLength (hBarcode, 999) ;
	}

	// When the toolkit scans an image it records the number of hits it gets for each barcode that
	// MIGHT be in the image. If the hits recorded for any of the barcodes are >= PrefOccurrence
	// then only these barcodes are returned. Otherwise, any barcode whose hits are >= MinOccurrence
	// are reported. If you have a very poor quality image then try setting MinOccurrence to 1, but you
	// may find that some false positive results are returned.
	//mtSetMinOccurrence (hBarcode, 2) ;
	//mtSetPrefOccurrence (hBarcode, 10) ;

	// Read Code 39 barcodes in extended mode
	// mtSetExtendedCode39(hBarcode, 1) ;

	// Barcode string is numeric
	if (strType.Find("digit") >= 0)
		mtSetReadNumeric(hBarcode, 1) ;
	else
		mtSetReadNumeric(hBarcode, 0) ;

	// Set a regular expression for the barcode
	// mtSetPattern(hBarcode, "^[A-Z]{2}[0-9]{5}$") ;

	// If you are scanning at a high resolution and the spaces between bars are
	// larger than 1 pixel then set MinSpaceBarWidth to 2 and increase your read rate.
	// mtSetMinSpaceBarWidth(hBarcode, 2) ;

	// MedianFilter is a useful way to clean up higher resolution images where the black bars contain white dots
	// and the spaces contain black dots. It does not work if the space between bars is only 1 pixel wide.
	mtSetMedianFilter(hBarcode, 0) ;

	// 条码识别结果保存在BarcodeInfo中：条码、类型、位置、方向等
	BARCODEINFO BarCodeInfo;
	BarCodeInfo.nBarCodes = BarCodeInfo.nBottomRightX = BarCodeInfo.nBottomRightY = 0;
	BarCodeInfo.nDirection = BarCodeInfo.nTopLeftX = BarCodeInfo.nTopLeftY = 0;
	BarCodeInfo.strBarCode.Empty();  BarCodeInfo.strBarCodeType.Empty();

	// Call ScanBarCode to process image
	if (hBitmap != NULL)
		BarCodeInfo.nBarCodes = mtScanBarCodeFromDIB(hBarcode, (long)hBitmap) ;  // 从DIB句柄识别条码
		//BarCodeInfo.nBarCodes = mtScanBarCodeFromBitmap(hBarcode, (long)hBitmap) ;  // 从Bitmap句柄识别条码
	else
		BarCodeInfo.nBarCodes = mtScanBarCode(hBarcode, strFileName) ;  // 从文件识别条码
	/*
	CString msg;
	msg.Format("Barcode No: %d", BarCodeInfo.nBarCodes);
	AfxMessageBox(msg);
	*/


	int i, j;
	//for (i = 1; i <= BarCodeInfo.nBarCodes; i++)
	for (i = BarCodeInfo.nBarCodes; i >= 1; i--)
	{
		// The ConvertUTF8Value function is only needed for barcodes that might contain
		// characters with values > 127. In practise this means Kanji QR-Codes and some
		// PDF-417 barcodes, so for ordinary barcodes it is fine to simply do:
		/*
		BarCodeInfo.strBarCode = mtGetBarString(hBarcode, i) ;
		BarCodeInfo.strBarCodeType = mtGetBarStringType(hBarcode, i) ;
		BarCodeInfo.nDirection = mtGetBarStringDirection(hBarcode, i) ;
		int nPage = mtGetBarStringPos(hBarcode, i, &BarCodeInfo.nTopLeftX, &BarCodeInfo.nTopLeftY, &BarCodeInfo.nBottomRightX, &BarCodeInfo.nBottomRightY) ;
		*/
		//strBarcode = mtGetBarString(hBarcode, i);
		ConvertUTF8Value(mtGetBarString(hBarcode, i), strBarcode) ;
		j = strFormat.GetLength();
		if ( j > 2)  // 可指定头部(2301Af*)、尾部(*2301Af)、或中间位置(*2301Af*)
		{
			if (strFormat.Right(1) == "*" && strFormat.Left(1) == "*")  // 中间
			{
				if (strBarcode.Find(strFormat.Mid(1, j-2), 0) < 0)
					strBarcode = "";
			}
			else if (strFormat.Right(1) == "*")  // 头部
			{
				if (strBarcode.Left(j-1) != strFormat.Left(j-1))
					strBarcode = "";
			}
			else if (strFormat.Left(1) == "*")  // 尾部
			{
				if (strBarcode.Right(j-1) != strFormat.Right(j-1))
					strBarcode = "";
			}
		}
		if (strBarcode != "")
		{
			if ( bBarcodeRotate )  // 根据条码自动旋转：此处添加方向标志 "x:::"
			{
				CString strLabel;
				strLabel.Format("%d|", mtGetBarStringDirection(hBarcode, i));
				strBarcode = strLabel + strBarcode;
				//AfxMessageBox(strBarcode);
			}
			break;
		}
	}
	mtDestroyBarcodeInstance(hBarcode) ;
	//AfxMessageBox( strBarcode );

	return strBarcode;
}


long CUDSTwain::ConvertUTF8Value(LPCSTR in, CString &out)
{
	wchar_t str[1000];
	long r = MultiByteToWideChar(CP_UTF8, 0, in, -1, str, 1000);
	out = str;
	return r ;
}


CString CUDSTwain::Self_BarCodeDivide(CString thumbpath, CString imgpath, int format)
{
	//读取配置文件确定条码分割是否打开 BarCodeRotate
	int       tem_nBarInfo   = 0;   //条码分割开关
	int       tem_nBarRotate = 0;   //条码旋转开关
	int       tem_nRotate    = 1;   //旋转方向
	CString   tem_strBarInfo = _T("");
	::GetPrivateProfileString("SaveSet", "BarCodeDivide ", "没有找到BarCodeDivide信息", tem_strBarInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nBarInfo = _ttoi(tem_strBarInfo);
	tem_strBarInfo.ReleaseBuffer();

	::GetPrivateProfileString("SaveSet", "BarCodeRotate ", "没有找到BarCodeRotate信息", tem_strBarInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_nBarRotate = _ttoi(tem_strBarInfo);
	tem_strBarInfo.ReleaseBuffer();

	if (tem_nBarInfo==0 && tem_nBarRotate==0)
	{
		return _T("");
	}

	//识别条码判断返回值
	CString   tem_strBarCode = _T("");

	if (tem_nBarInfo==0 && tem_nBarRotate==1)
	{
		//有条码：情况1-开启条码旋转，关闭条码分割
		tem_strBarCode = BarcodeReader(NULL, imgpath, NULL, NULL, 0, true);
		if (tem_strBarCode == _T(""))
		{
			return _T("");
		}
		//取出分隔符|前的字符
		int   tem_nDivSite = -1;
		tem_nDivSite = tem_strBarCode.Find('|');
		CxImage   tem_cxImg;
		CxImage   tem_cxThumbImg;
		if (tem_nDivSite != -1)
		{
			CString  tem_strRotate = tem_strBarCode.Mid(0, tem_nDivSite);
			tem_nRotate = _ttoi(tem_strRotate);
			if (tem_nRotate == 2)
			{
				//左旋90度
				tem_cxImg.Load(imgpath);
				tem_cxThumbImg.Load(thumbpath);
				tem_cxImg.Rotate(-90);
				tem_cxThumbImg.Rotate(-90);

				switch(format)
				{
				case 1:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_BMP);
					break;
				case 3:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				case 4:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_PNG);
					break;
				case 6:
					tem_cxImg.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_TIF);
					break;
				case 50:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				default:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				}
				tem_cxThumbImg.Save(thumbpath, CXIMAGE_FORMAT_JPG);

			} 
			else if (tem_nRotate == 8)
			{
				//右旋90度
				tem_cxImg.Load(imgpath);
				tem_cxThumbImg.Load(thumbpath);
				tem_cxImg.Rotate(90);
				tem_cxThumbImg.Rotate(90);
				switch(format)
				{
				case 1:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_BMP);
					break;
				case 3:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				case 4:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_PNG);
					break;
				case 6:
					tem_cxImg.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_TIF);
					break;
				case 50:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				default:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				}
				tem_cxThumbImg.Save(thumbpath, CXIMAGE_FORMAT_JPG);
			}
			else if (tem_nRotate == 4)
			{
				//旋转180度
				tem_cxImg.Load(imgpath);
				tem_cxThumbImg.Load(thumbpath);
				tem_cxImg.Rotate(180);
				tem_cxThumbImg.Rotate(180);
				switch(format)
				{
				case 1:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_BMP);
					break;
				case 3:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				case 4:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_PNG);
					break;
				case 6:
					tem_cxImg.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_TIF);
					break;
				case 50:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				default:
					tem_cxImg.Save(imgpath, CXIMAGE_FORMAT_JPG);
					break;
				}
				tem_cxThumbImg.Save(thumbpath, CXIMAGE_FORMAT_JPG);
			} 
		}

		return _T("");

	} 
	else if (tem_nBarInfo==1 && tem_nBarRotate==0)
	{
		//有条码：情况2-关闭条码旋转，开启条码分割
		tem_strBarCode = BarcodeReader(NULL, imgpath, NULL, NULL, 0, false);
		if (tem_strBarCode == _T(""))
		{
			return _T("");
		}

	}
	else
	{
		//有条码：情况3-开启条码旋转，开启条码分割
		tem_strBarCode = BarcodeReader(NULL, imgpath, NULL, NULL, 0, true);
		if (tem_strBarCode == _T(""))
		{
			return _T("");
		}
		//取出分隔符|前的字符
		int   tem_nDivSite = -1;
		tem_nDivSite = tem_strBarCode.Find('|');
		if (tem_nDivSite != -1)
		{
			CString  tem_strRotate = tem_strBarCode.Mid(0, tem_nDivSite);
			tem_nRotate = _ttoi(tem_strRotate);
			tem_strBarCode = tem_strBarCode.Mid(tem_nDivSite+1);	
		}
	}

	//判断子目录是否存在，不存在则创建子目录
	CString   tem_strNewDoc = m_result_path;
	tem_strNewDoc += tem_strBarCode;
	CFileFind    tem_fFileFind;
	if (!tem_fFileFind.FindFile(tem_strNewDoc))
	{
		CreateDirectory(tem_strNewDoc, NULL);
	}
	tem_strNewDoc += _T("\\");
	m_strBarDocument = tem_strNewDoc;

	//重置命名
	CString  tem_strFileName = _T("");
	m_totalimage = 1;
	tem_strFileName = NamingFile(m_totalimage);
	CString   tem_strFilePath = tem_strNewDoc;
	tem_strFilePath += tem_strFileName;
	switch(format)
	{
	case 1:
		tem_strFilePath += ".bmp";
		break;
	case 3:
		tem_strFilePath += ".jpg";
		break;
	case 4:
		tem_strFilePath += ".png";
		break;
	case 6:
		tem_strFilePath += ".tif";
		break;
	case 50:
		tem_strFilePath += ".jpg";
		break;
	default:
		tem_strFilePath += ".tif";
		break;
	}
	

	//剪切文件
	CopyFile(imgpath,  tem_strFilePath, FALSE);
	DeleteFile(imgpath);

	//旋转移动后的图像
	CxImage   tem_cxImg;
	CxImage   tem_cxThumbImg;
	if (tem_nRotate == 2)
	{
		//左旋90度
		tem_cxImg.Load(tem_strFilePath);
		tem_cxThumbImg.Load(thumbpath);
		tem_cxImg.Rotate(-90);
		tem_cxThumbImg.Rotate(-90);

		switch(format)
		{
		case 1:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_BMP);
			break;
		case 3:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		case 4:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_PNG);
			break;
		case 6:
			tem_cxImg.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_TIF);
			break;
		case 50:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		default:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		}
		tem_cxThumbImg.Save(thumbpath, CXIMAGE_FORMAT_JPG);

	} 
	else if (tem_nRotate == 8)
	{
		//右旋90度
		tem_cxImg.Load(tem_strFilePath);
		tem_cxThumbImg.Load(thumbpath);
		tem_cxImg.Rotate(90);
		tem_cxThumbImg.Rotate(90);
		switch(format)
		{
		case 1:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_BMP);
			break;
		case 3:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		case 4:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_PNG);
			break;
		case 6:
			tem_cxImg.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_TIF);
			break;
		case 50:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		default:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		}
		tem_cxThumbImg.Save(thumbpath, CXIMAGE_FORMAT_JPG);
	}
	else if (tem_nRotate == 4)
	{
		//旋转180度
		tem_cxImg.Load(tem_strFilePath);
		tem_cxThumbImg.Load(thumbpath);
		tem_cxImg.Rotate(180);
		tem_cxThumbImg.Rotate(180);
		switch(format)
		{
		case 1:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_BMP);
			break;
		case 3:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		case 4:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_PNG);
			break;
		case 6:
			tem_cxImg.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_TIF);
			break;
		case 50:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		default:
			tem_cxImg.Save(tem_strFilePath, CXIMAGE_FORMAT_JPG);
			break;
		}
		tem_cxThumbImg.Save(thumbpath, CXIMAGE_FORMAT_JPG);
	}

	return tem_strFilePath;
}


CString CUDSTwain::Self_GetCurTime(void)
{
	SYSTEMTIME   tem_stDateTime;
	CString      tem_strDate      = _T("");
	CString      tem_strTime      = _T("");
	CString      tem_strTimeName  = _T("");       //时间命名

	GetLocalTime(&tem_stDateTime);
	tem_strDate.Format(_T("%d%02d%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay);
	tem_strTime.Format(_T("%02d%02d%02d%02d"), tem_stDateTime.wHour, tem_stDateTime.wMinute, tem_stDateTime.wSecond, tem_stDateTime.wMilliseconds);
	
	tem_strTimeName  = tem_strDate;
	tem_strTimeName += tem_strTime;

	return tem_strTimeName;
}


CxImage* CUDSTwain::Self_AddPageNum(CxImage* img, int site , int reset, BOOL openit)
{
	if (!openit)
	{
		return img;
	}
	CxImage::CXTEXTINFO PageInfo;      //页码
	img->InitTextInfo( &PageInfo );

	CString   tem_strPageNum;
	if (reset == 2)
	{
		//手动重置
		//a、读取ini计数值
		::GetPrivateProfileString("SaveSet", "PageCount", "没有找到PageCount信息", tem_strPageNum.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
		//b、计数值+1
		int       tem_nPageNum;
		tem_nPageNum = _ttoi(tem_strPageNum);
		tem_strPageNum.ReleaseBuffer();
		tem_nPageNum++;
		//c、更新页码
		tem_strPageNum.Format(_T("%d"), tem_nPageNum);
		_tcscpy( PageInfo.text, tem_strPageNum);
		//d、将计数值写入ini
		::WritePrivateProfileString("SaveSet", "PageCount", tem_strPageNum, m_strIniPath);
	}
	else if (reset == 1)
	{
		tem_strPageNum.Format(_T("%d"),m_nPageCount);
		_tcscpy( PageInfo.text, tem_strPageNum);
		m_nPageCount++;
	}
	else if (reset == 0)
	{
		tem_strPageNum.Format(_T("%d"),m_nPageCount);
		_tcscpy( PageInfo.text, tem_strPageNum);
		m_nPageCount++;
	}

	//页码大小
	PageInfo.lfont.lfHeight = m_nPageSize;
	PageInfo.lfont.lfItalic = FALSE;    //倾斜
	PageInfo.fcolor = RGB(0, 0, 0);     //颜色
	PageInfo.opaque = FALSE;            // No background

	//位置
	switch(m_nPageSite)
	{
	case 0:
		//左下角
		img->DrawStringEx(0, 20, img->GetHeight(), &PageInfo);
		break;
	case 2:
		//中间
		img->DrawStringEx(0, img->GetWidth()/2, img->GetHeight(), &PageInfo);
		break;
	case 1:
		//右下角
		img->DrawStringEx(0, img->GetWidth()-20, img->GetHeight(), &PageInfo);
		break;
	default:
		//中间
		img->DrawStringEx(0, img->GetWidth()/2, img->GetHeight(), &PageInfo);
		break;

	}

	return img;
}


int CUDSTwain::Self_ReadCountInfo(int curvalue, int mode)
{
	int      tem_nNameInfo   = 0;
	CString  tem_strNameInfo = _T("");
	if (mode==0)
	{
		//0时写入配置文件,并置零
		m_totalimage = 0;
		::WritePrivateProfileString("SaveSet", "ScanCount", _T("0"), m_strIniPath);
	} 
	else if (mode == 1)
	{
		//1时读取配置文件
		::GetPrivateProfileString("SaveSet", "ScanCount", "没有找到ScanCount信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
		tem_nNameInfo = _ttoi(tem_strNameInfo);
		tem_strNameInfo.ReleaseBuffer();
		tem_nNameInfo += curvalue;

		tem_strNameInfo.Format(_T("%d"), tem_nNameInfo);
		::WritePrivateProfileString("SaveSet", "ScanCount", tem_strNameInfo, m_strIniPath);
	}
	else if(mode == 2)
	{
		//2时读取配置文件
		::GetPrivateProfileString("SaveSet", "ScanCount", "没有找到ScanCount信息", tem_strNameInfo.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
		tem_nNameInfo = _ttoi(tem_strNameInfo);
		tem_strNameInfo.ReleaseBuffer();
		tem_nNameInfo += curvalue;
	}
	
	return tem_nNameInfo;
}


//自动裁切与纠偏***************************************************************************************************************
bool CUDSTwain::GetDecliningAngle(Mat _src, double& _degree, float XRay, float YRay)
{
	//获取图像DPI
	float  tem_fXRay = XRay;
	float  tem_fYRay = YRay;

	Mat srcImage = _src;//copyTo拷贝src_img的数据区到srcImage中，在拷贝数据前会有一步：srcImage.create(this->size , this->type)  
	const int width = srcImage.cols;
	const int height = srcImage.rows;

	Mat midImage;
	if(tem_fXRay > 200.00)//降低分辨率
	{
		long lXDPI = tem_fXRay; //获得图像x轴分辨率
		long lYDPI = tem_fYRay;

		float dFx = 200.00/lXDPI;
		float dFy = 200.00/lYDPI;

		WORD unNewWidth= (WORD)(srcImage.cols * dFx); 
		WORD unNewHeight = (WORD)(srcImage.rows * dFy);

		resize(srcImage, midImage, cv::Size(unNewWidth, unNewHeight), 0, 0);	
	}
	else
	{
		srcImage.copyTo(midImage);
	}

	WORD unNewWidth = midImage.cols; 
	WORD unNewHeight = midImage.rows; 

	AutoCrop(midImage, midImage);//

	if(!getContoursByCplus(midImage, midImage)) //得到的已经是黑白图
	{
		return false;
	}
	//【3】进行概率霍夫线变换
	vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	HoughLinesP(midImage, lines, 1, 5*CV_PI/1800, 20, max(unNewWidth/4,unNewHeight/4), 50);

	int linenum = lines.size();
	//【4】依次在图中绘制出每条线段
	float dx = 0.0;
	float dy = 0.0;
	Vec4i maxline;
	if(linenum != 0)
	{
		maxline = lines[0];
	}
	else
	{
		HoughLinesP(midImage, lines, 1, CV_PI/1800, 10, min(unNewWidth/4,unNewHeight/4), 70);

		linenum = lines.size();
		if(linenum==0)
		{
			return false;
		}
	}

	for( size_t i = 0; i < linenum; i++ )
	{
		Vec4i l = lines[i];
		//每一条线由具有四个元素的矢量(x_1,y_1, x_2, y_2）表示，
		//其中，(x_1, y_1)和(x_2, y_2) 是是每个检测到的线段的起点和结束点。
		float mi = (l[2]-l[0]) * (l[2]-l[0]) + (l[3]-l[1]) * (l[3]-l[1]);

		float maxi = (maxline[2]-maxline[0])*(maxline[2]-maxline[0]) + (maxline[3]-maxline[1]) * (maxline[3]-maxline[1]);
		if(mi > maxi)
		{
			maxline = lines[i];
		}
	}

	dx = (float)(maxline[2]-maxline[0]);
	dy = (float)(maxline[3]-maxline[1]);

	bool mark = false;

	double temp;
	double theta;

	if (fabs(dy) < 1e-6 || fabs(dx) < 1e-6)
	{
		theta = 0.0f;
	}
	else
	{
		if(dy > 0)
		{//负表示顺时针，正表示逆时针
			if(dx > dy)
			{	//正确
				temp = dx/dy;
				theta = CV_PI/2.0 - atan(temp);
			}
			else
			{
				temp = dy/dx; 		
				theta = atan(temp); 
				mark = true;
			}
		}
		else
		{
			if(dx > fabs(dy))
			{
				temp = dx/fabs(dy);

				theta = atan(temp);
				mark = true;
			}
			else
			{
				temp = fabs(dy)/dx;
				theta = CV_PI/2.0 - atan(temp);
			}
		}
	}

	theta = theta/CV_PI*180; //弧度转角度
	_degree = mark ? (theta + 270.0) : theta;

	return true;
}


void CUDSTwain::AutoCrop(Mat _src, Mat& _dst)
{
	const int color_black = 0;   // 黑色
	const int color_white = 255; // 白色

	Mat src;
	if (_src.channels()>1)
	{
		cvtColor(_src, src, CV_RGB2GRAY);
		threshold(src, src, 128, 255, CV_THRESH_OTSU); 
	}
	else
	{
		threshold(_src, src, 128, 255, CV_THRESH_OTSU); 
	}


	int width = src.cols; //列
	int height = src.rows; //行

	int up = 0;
	int down = height; //行
	int left = 0;
	int right = width;
	const int bar_width = 30;  // 白条宽度

	// 一行的白点>bar_width就认为是边界
	//上侧
	int count_white = 0;
	for (int j = 0; j < height-1; j++)  
	{  
		uchar* data = src.ptr<uchar>(j);  
		for (int i = 0; i < width-1; i++)  
		{ 
			if(data[i] == color_white)// 找到白色的点
			{
				count_white ++;
			}
		}

		if (count_white >= bar_width) 
		{
			up = j; 
			break;
		}
		count_white = 0;
	}

	// 下侧
	count_white = 0;
	for (int j = height-1; j >= 0; j--)  
	{  
		uchar* data = src.ptr<uchar>(j);  
		for (int i = width-1; i >= 0; i--)  
		{ 
			if(data[i] == color_white)// 找到白色的点
			{
				count_white ++;
			}	
		}

		if (count_white >= bar_width) 
		{
			down = j; 
			break;
		}	
		count_white = 0;
	}	

	//左侧
	count_white = 0;
	for(int j = 0; j < width; j++)  // 从左向右递增
	{
		for(int i = 0; i < height; i++)  // 从上向下递增
		{
			if ( (int)src.at<uchar>(i,j) == color_white ) // 找到白色的点
			{
				count_white ++;
			}	
		}

		if (count_white >= bar_width)
		{
			left = j; 
			break;
		}
		count_white = 0;
	}

	//右侧
	count_white = 0;
	for(int j = width-1; j >= 0; j--)
	{
		for(int i = height-1; i >= 0; i--)
		{
			if ( (int)src.at<uchar>(i,j) == color_white ) // 找到白色的点
			{		
				count_white ++;
			}	
		}

		if (count_white >= bar_width)
		{
			right = j; 
			break;
		}
		count_white = 0;
	}

	cv::Rect rect(left, up, right-left, down-up);  // 裁切后区域
	_dst = _src(rect);	
}


bool CUDSTwain::getContoursByCplus(Mat src_img, Mat& dst_img)
{
	//2018-03-16原纠偏函数
	Mat canny_output;
	if (!src_img.data)  
	{  
		return false;  
	}

	Canny(src_img, canny_output, 200, 150, 3);  

	if(RemoveExpoint(canny_output, dst_img))
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
	

	/*2018-03-16新纠偏函数*/
	/*
	Mat canny_output;
	if (!src_img.data)  
	{  
		return false;  
	}

	//Canny(src_img, canny_output, 200, 150, 3);  
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat src_gray;
	cvtColor( src_img, src_gray,CV_RGB2GRAY );
	Sobel( src_gray, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );  
	convertScaleAbs( grad_x, abs_grad_x );  
	//y方向梯度计算  
	Sobel( src_gray, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );  
	convertScaleAbs( grad_y, abs_grad_y );  
	//加权和  
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, canny_output );  

	if(RemoveExpoint(canny_output, dst_img))
	{
		return true;
	}
	else
	{
		return false;
	}
	*/

}


bool CUDSTwain::RemoveExpoint(Mat _src, Mat& _dst)
{
	_dst = _src;
	const int width = _src.cols; //列
	const int height = _src.rows; //行

	int templeft = 0, tempright = 0;
	//彩色 
	for (int j=0; j<height-1; j++)  
	{  
		uchar* data = _src.ptr<uchar>(j);  
		for (int i=0; i<width-1; i++) 
		{
			if(255==data[i])// 找到白色的点
			{
				templeft = i;

				for(int m = width-1; m > templeft; m--)
				{
					if( 255==data[m]) // 找到白色的点
					{
						tempright = m;

						for(int n = templeft+10; n < tempright-10; n++)
						{
							data[n]=0;//将中间白点全部去除
						}
						break;
					}	
				}
				break;
			}
		}
	}
	return true;
}


void CUDSTwain::RoteteImage2(Mat _src, Mat& _dst, double _degree, bool _front)
{
	if (fabs(_degree) < 1e-6)
	{
		return;
	}

	Mat srcImage = _src;//copyTo拷贝src_img的数据区到srcImage中，在拷贝数据前会有一步：srcImage.create(this->size , this->type)  
	const int width = srcImage.cols;
	const int height = srcImage.rows;

	double   degree = 0.0f;
	degree = _front ?  _degree : (360.0 - _degree); // 多流背面倾斜角 = 360 - 正面倾斜角

	double  angle  = degree/180*CV_PI; //角度转弧度
	double  a = sin(angle), b = cos(angle);
	int     width_rotate = int(height *fabs(a) + width *fabs(b));
	int     height_rotate = int(width *fabs(a) + height *fabs(b));
	float   map[6];
	Mat     m_map_matrix(2,3,CV_32F, map);
	CvPoint2D32f center = cvPoint2D32f(width/2, height/2);  
	CvMat map_matrix2 = m_map_matrix;  

	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);  //degree为角度，不是弧度		

	map[2] += (width_rotate - width)/2;
	map[5] += (height_rotate - height)/2;

	Mat m_image_out;	
	warpAffine(srcImage, m_image_out, m_map_matrix, cv::Size(width_rotate, height_rotate), 1,0,0);
	_dst = m_image_out;
}


CString CUDSTwain::Self_CropImg(CString imgpath, float xres, float yres)
{
	std::string strTempData = (CStringA)imgpath; 
	Mat        tem_cvSrcImg = imread(strTempData);

	imgpath.ReleaseBuffer();
	if (!tem_cvSrcImg.data)
	{
		return _T("");
	}

	double     tem_dAngle = 0;
	GetDecliningAngle(tem_cvSrcImg, tem_dAngle, xres, yres);
	RoteteImage2(tem_cvSrcImg, tem_cvSrcImg, tem_dAngle, true);	
	AutoCrop(tem_cvSrcImg, tem_cvSrcImg);

	imwrite(strTempData, tem_cvSrcImg);


	return imgpath;
}