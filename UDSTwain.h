#pragma once

#include "twain.h"
#include <vector>
#include "ximage.h"
#include "uds_cap.h"
#include "SoftekBarcodeDLL.h"
#include "cv.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"

using namespace cv;
using namespace std;

#define UDS_SUCCESS			 0
#define UDS_TWAIN			-1
#define UDS_TWAINDRIVER		-2
#define UDS_SCANNER			-3
#define UDS_LICENSE			-4
#define UDS_LICENSE_QUIT	-5
#define UDS_NOPAPER			-6
#define UDS_PAPERJAM		-7
#define UDS_PAPERMULTIFEED	-8
#define UDS_MEMORY			-9
#define UDS_DISK			-10
#define UDS_APPLICATION		-11

typedef TW_UINT16 (FAR PASCAL *DSMENTRYPROC)(pTW_IDENTITY, pTW_IDENTITY, TW_UINT32, TW_UINT16, TW_UINT16, TW_MEMREF);

typedef struct tagTWAINSCANINFO
{
	long       nScanDPI;
	long       nScanMode;
	long	   nScanSource;  // 0-Flatbed, 1-Single ADF, 2-Double ADF
	long	   nBarcodeLength;
	long	   BackgroundUnify, Tg, Gg; 
	long       nImageToScan,nImageScanned;
	long       BlackEdge, edgeThreshold;  
	long	   BlankFilter, grayThreshold, edgeRatio;  
	long	   ColorFilter, colorThreshold, colorRatio;  
	long       nBrightness,nContrast,nThreshold,nQuality;
	bool       bIsShowUI;
	bool       bDSMOpened, bDSOpened;
	bool	   bActive, bActiveLocker;
	bool	   bMultiBarcode, bBarcodeRotate;
	bool	   bIsAuthorizedFlatbed, bAutoCrop, bInvert;
	CString	   strSN, strLicenseCode;	
	CString    strImagePath, strBarcodeType, strBarcodeFormat;  	
}TWAINSCANINFO;

typedef struct tagBARCODEINFO
{
	int	nBarCodes;
	int nDirection;
	long nTopLeftX;
	long nTopLeftY;
	long nBottomRightX;
	long nBottomRightY;
	CString strBarCode;
	CString strBarCodeType;
} BARCODEINFO;

class CUDSTwain
{
public:
	HWND           m_hWnd;
	HINSTANCE      m_hTwain32Dll;
	DSMENTRYPROC   m_pDSMEntry;

	TW_IMAGEINFO   m_ImageInfo;
	TWAINSCANINFO  m_ScanInfo;
	TW_IDENTITY    m_AppID, *m_pScannerID;
	TW_IDENTITY    m_ScannerID1, m_ScannerID2, m_SaveScannerID;
	CString        m_result_path;
	CString        pDSNames[30];
	long           m_scannernumbers;
	int            m_totalimage;     //用于命名计数，命名计数中间可能被重置，如识别条码后
	int            m_nImageCount;    //用于统计一次的总计数
	int            m_format_index;
//	std::vector<CString>  m_FileName;
	int            m_transformation;
	int            m_nTransType;
	int            m_nMutiTifName;

	int            m_nNameMode;
	int            m_nNameDate;
	int            m_nNameTime;
	int            m_nNameBegin;

	CString        m_strNameDivide;
	CString        m_strNamePre1;
	CString        m_strNamePre2;
	CString        m_strNamePre3;
	CString        m_strIniPath;

	BOOL           m_BAutoCrop;
	
public:
	CUDSTwain(void);
	virtual ~CUDSTwain(void);
	bool OpenDSM(void);
	bool GetInstalledDS();
	bool SelectScanner(CString ScannerName);
	bool OpenDS(void);
	long EnableDS(void);
	long EnableDSOnly(void);
	bool DisableDS(void);
	long GetImage(void);
	bool CloseDS(void);
	bool CloseDSM(void);
	long GetImageFileMode(void);

	int  m_nPageOpen;
	int  m_nPageSite;
	int  m_nPageReset;
	int  m_nPageSize;
	int  m_nPageCount;
	int  m_nGrayBit; 
	BOOL m_BPageOpen;

	CxImage* AddWatermark(CxImage* img, CString scanner);
	CString   m_strSlcScanner;   //选中的扫描设备，用于判断是否合法
	void ReadIniInfo();
	CString NamingFile(int count);


	CString BarcodeReader(HANDLE hBitmap, CString strFileName , CString strType , CString strFormat , long double nLength, bool bBarcodeRotate);
	long ConvertUTF8Value(LPCSTR in, CString &out);
	CString Self_BarCodeDivide(CString thumbpath, CString imgpath, int format);
	CString  m_strBarDocument;
	CString Self_GetCurTime(void);
	CxImage* Self_AddPageNum(CxImage* img, int site , int reset, BOOL openit);
	int Self_ReadCountInfo(int curvalue, int mode);
	bool GetDecliningAngle(Mat _src, double& _degree, float XRay, float YRay);
	void AutoCrop(Mat _src, Mat& _dst);
	bool getContoursByCplus(Mat src_img, Mat& dst_img);
	bool RemoveExpoint(Mat _src, Mat& _dst);
	CString Self_CropImg(CString imgpath, float xres, float yres);
	void RoteteImage2(Mat _src, Mat& _dst, double _degree, bool _front);
};

