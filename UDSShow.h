#pragma once
#include "ximage.h"

// CUDSShow �Ի���

class CUDSShow : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSShow)

public:
	CUDSShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSShow();

// �Ի�������
	enum { IDD = IDD_DIALOG_WPICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
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

	void ShowImage(int x, int y);
	
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
};
