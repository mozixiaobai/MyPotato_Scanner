
// UDS GL ToolsDlg.h : 头文件
//

#pragma once
#define  WM_READINI  WM_USER+1001
#define THUMB_WIDTH  160
#define THUMB_HEIGHT 180
#include "UDSTwain.h"
#include "hpdf.h"
#include "hpdf_types.h"
#include "hpdf_consts.h"
#include "UDSShow.h"
#include <iostream>
#include <direct.h>
#include "afxwin.h"
#include "PngButton.h"
#include "UDSTips.h"
#include "HSMenu.h"   //菜单自绘
#include "HSMenuPro.h"
#include "HSMenuX.h"
#include <algorithm>
#include "UDSNameRule.h"

typedef void (__stdcall *HPDF_Error_Handler)  (HPDF_STATUS   error_no,
	                                           HPDF_STATUS   detail_no,
	                                           void         *user_data);

// CUDSGLToolsDlg 对话框
class CUDSGLToolsDlg : public CDialogEx
{
// 构造
public:
	CUDSGLToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UDSGLTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();

public:
	CUDSTips        m_iTips;        //删除提示框
	int             index;
	int             nIndex;
	int             m_listindex;
	int             m_nNameMode;     //设置命名规则
	CString         index_value;
	CString         index_value2;
	CString         m_ini_path;
//	CString        *AllPath;
//	CString        *AllFilename;

	CEdit           m_path_edit;
	CComboBox       m_format;
	CComboBox       m_scannername;
	CListCtrl       m_imagelist;
	CImageList      imagelist;
		
	CUDSTwain      m_dlg_twain;
	CUDSNameRule   m_dlg_name;
	
	afx_msg void OnSelchangeComboScaners();
	
	
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnDropdownComboScaners();
	afx_msg void OnDropdownComboFormat();
	afx_msg void OnSelchangeComboFormat();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonClear();

	CString DefaultPath(void);	
	CString GetIniPath(void);	
	bool GerneratePdf(int count);
	bool ClearImage(void);
	bool ThumbailList(void);
	int GetTypeFromFileName(LPCTSTR pstr);	
	CPngButton m_button_path;
	CPngButton m_button_save;
	CPngButton m_button_clear;
	CPngButton m_button_start;
	CPngButton m_button_set;

	CStatic m_pic2;
	void ShowWpicture(CString m_path);
	CStatic    m_pic;
	CScrollBar m_scrollh;
	CScrollBar m_scrollv;
	int        cx, cy;
	int        xpos, ypos;
	int        offsetx, offsety;
	CDC*       pDc;
	CDC        imageDC;
	CxImage*   pImage;
	CRect      pic_rect;
	CScrollBar m_scrollhs;
	CScrollBar m_scrollvs;

	bool      m_click_mark;
	bool      m_scan_mark;
	CPoint    m_pt;
	CPoint    n_pt;
	CPoint    m_move;
	CPoint    m_save;
	int       m_pdfcount;
	BOOL      m_BDownBk;   //加载界面背景图

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClickListPicture(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedCheckPicture();	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDblclkStaPath();
	CPngButton m_btnExit;
	afx_msg void OnBnClickedBtnExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CToolTipCtrl   m_tipInfo;       //用于按钮文字提示
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedBtnWeb();
	CPngButton m_btnMin;
	CPngButton m_btnWeb;
	afx_msg LRESULT OnNcHitTest(CPoint point);
//	std::vector<CString>  m_vcPDFPicPath;    //用于保存生成PDF时，显示的缓存图像，退出时删除缓存图像   0829
	afx_msg void OnCopyIt();
	afx_msg void OnDeleteIt();
	afx_msg void OnRename();
	afx_msg void OnProperty();
	CHSMenu     m_mnuRightClick;  //右键菜单
	BOOL CreateHSMenu(void);
	afx_msg void OnRclickListPicture(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHoverListPicture(NMHDR *pNMHDR, LRESULT *pResult);
	std::vector<int>  m_vcCtrlIndex;   //Ctrl多选索引
	BOOL CopyFiles2Clipboard(std::vector<CString> const& vecFiles);
	int     m_nLastSlcFile;     //用于重命名多个文件时保存最后一个索引
	CEdit*  m_conRename;        //获取重命名时编辑框
	BOOL    m_BMaxDlg;          //最大化标识位
	CRect    m_rcRectSrc;        //最初的客户端区域大小
	CRect    m_rcBT, m_rcMax, m_rcMin, m_rcExit, m_rcSlcScanner, m_rcSlcFmt, m_rcComScanner, m_rcComFmt, m_rcChkPic, m_rcPic;
	CRect    m_rcScan, m_rcSet, m_rcRename, m_rcWPic, m_rcList, m_rcHScroll, m_rcVScroll;
	CRect    m_rcSlcPath, m_rcNowPath, m_rcShowPath, m_rcAutoCropS, m_rcAutoCropC;
	int      m_nSlctItem;

	CString GetMyDocument(void);
	void CheckIniFile(void);
	CString Self_GetMyDocument(void);
	bool     m_bClosed;    //关闭TWAIN
	
protected:
	afx_msg LRESULT OnReadini(WPARAM wParam, LPARAM lParam);
public:
	void Self_DisableCtrls(bool mode);

	BOOL    m_BAdvancedSet;     //扫描时显示高级设置界面
	bool ThumbailList(std::vector<CString> curname, std::vector<CString> curpath);

	int     m_nNameCounts;     //显示图像名计数
	int     m_nImgCounts;      //显示图像计数
	BOOL    m_BAutoCrop;       //自动裁切开关
	afx_msg void OnClickedChkAautocrop();

	COLORREF   m_bkColor;
};
