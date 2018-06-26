// UDSShow.cpp : 实现文件
//

#include "stdafx.h"
#include "UDS GL Tools.h"
#include "UDSShow.h"
#include "afxdialogex.h"


extern CString m_path_image;

// CUDSShow 对话框

IMPLEMENT_DYNAMIC(CUDSShow, CDialogEx)

CUDSShow::CUDSShow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSShow::IDD, pParent)
{
	cx = 0;
	cy = 0;
	offsetx = 0;
	offsety = 0;
	xpos = 0;
	ypos = 0;
}

CUDSShow::~CUDSShow()
{
}

void CUDSShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	DDX_Control(pDX, IDC_SCROLLBARH, m_scrollh);
	DDX_Control(pDX, IDC_SCROLLBARV, m_scrollv);
}


BEGIN_MESSAGE_MAP(CUDSShow, CDialogEx)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUDSShow 消息处理程序


void CUDSShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	m_scrollh.SetScrollPos(1);
	m_scrollv.SetScrollPos(1);
	
	pImage = new CxImage;
	pImage->Load(m_path_image);
	cx = pImage->GetWidth();
	cy = pImage->GetHeight();

	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&pic_rect);
	ScreenToClient(&pic_rect);

	m_scrollh.SetScrollRange(1,cx-pic_rect.right);
	m_scrollv.SetScrollRange(1,cy-pic_rect.bottom);

	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC_PIC);
	pDc=pWnd->GetDC();
	imageDC.CreateCompatibleDC(pDc);
	HBITMAP   m_hBitmap;
	m_hBitmap = pImage->MakeBitmap(imageDC);
	CBitmap m_bmp;
	m_bmp.Attach(m_hBitmap);
	imageDC.SelectObject(&m_bmp);
	offsetx = pic_rect.left;
	offsety = pic_rect.top;
	ShowImage(0,0);

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CUDSShow::ShowImage(int x, int y)
{
	pDc->BitBlt(0,0,pic_rect.right-offsetx,pic_rect.bottom-offsety,&imageDC,x,y,SRCCOPY);	
}


void CUDSShow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	xpos = m_scrollh.GetScrollPos();
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
	m_scrollh.SetScrollPos(xpos);
	ShowImage(xpos,ypos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CUDSShow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ypos = m_scrollv.GetScrollPos();
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
	m_scrollv.SetScrollPos(ypos);

	ShowImage(xpos,ypos);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CUDSShow::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::delete pImage;
	ReleaseDC(pDc);
	CDialogEx::OnClose();
}
