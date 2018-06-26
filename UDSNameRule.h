#pragma once
#define  WM_READINI  WM_USER+1001
#include "resource.h"
#include "UDSHelpInfo.h"


// CUDSNameRule 对话框

class CUDSNameRule : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSNameRule)

public:
	CUDSNameRule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSNameRule();

// 对话框数据
	enum { IDD = IDD_DLG_NAMERULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int       m_nNmaeMode;
	int       m_nNameDate;
	int       m_nNameTime;
	int       m_nNameBegin;
	int       m_nBarCode;         //条码分割
	int       m_nBarCodeRotate;   //条码旋转
	int       m_nPageOpen;        //页码开关
	int       m_nPageSite;        //页码位置
	int       m_nPageSize;        //页码大小
	int       m_nPageReset;       //页码重置
	int       m_nOldPageReset;    //原页码重置
	int       m_nGrayBit;         //灰度位数
	int       m_nAdvancedSet;     //高级设置
	int       m_nAutoCrop;        //自动纠偏裁切
	
	CString   m_strNameDivide;
	CString   m_strNamePre1;
	CString   m_strNamePre2;
	CString   m_strNamePre3;
	CString   m_strIniPath;

	HWND      m_hMainWnd;

	CUDSHelpInfo    m_dlgHelpInfo;

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);



	afx_msg void OnClickedChkOrder();
	afx_msg void OnClickedChkTime();
	afx_msg void OnClickedChkCustom();
	BOOL Self_SetCtrl(int chkindex);
	CComboBox m_conComDate;
	CComboBox m_conComTime;
	CString SubGetMyDocument(void);
	void ReadIniInfo(CString inipath);
	CString m_EditDivide;
	CString m_EditPre1;
	CString m_EditPre2;
	CString m_EditPre3;
	CString m_EditBegin;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL SubVerityRule(CString input);
	afx_msg void OnChangeEditDivition();
	afx_msg void OnClickedChkBarcode();
	int m_SizeValue;
	afx_msg void OnClickedChkSite1();
	afx_msg void OnClickedChkSite2();
	afx_msg void OnClickedChkSite3();
	afx_msg void OnClickedChkReset1();
	afx_msg void OnClickedChkReset2();
	afx_msg void OnClickedChkReset3();
	afx_msg void OnBnClickedBtnResetpage();
	afx_msg void OnClickedChkPageopen();
	afx_msg void OnBnClickedBtnHelp();
	afx_msg void OnClickedChkBarrotate();
	afx_msg void OnClickedChkGray8();
	afx_msg void OnClickedChkGray10();
	afx_msg void OnBnClickedBtnResetcount();
	afx_msg void OnClickedChkAdvanced();
	afx_msg void OnClickedChkBautocrop();
};
