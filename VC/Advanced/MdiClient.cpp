// MdiClient.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
// This class does subclass the MDI-CLIENT window.
// Subclassing means that all messages are first routed to this class, then 
// to the original window (in this case the MDI-CLIENT).
// We need this to get notifications of the creation and deletion of the 
// MDI child frames (contain views).
/////////////////////////////////////////////////////////////////////////////
//
// Copyright ?1998 Written by Dieter Fauth 
//		mailto:fauthd@zvw.de 
//  
// This code may be used in compiled form in any way you desire. This    
// file may be redistributed unmodified by any means PROVIDING it is     
// not sold for profit without the authors written consent, and     
// providing that this notice and the authors name and all copyright     
// notices remains intact. If the source code in this file is used in     
// any  commercial application then a statement along the lines of     
// "Portions Copyright ?1999 Dieter Fauth" must be included in     
// the startup banner, _T("About" box or printed documentation. An email     
// letting me know that you are using it would be nice as well. That's     
// not much to ask considering the amount of work that went into this.    
//    
// This file is provided "as is" with no expressed or implied warranty.    
// The author accepts no liability for any damage/loss of business that    
// this product may cause.    
//  
// ==========================================================================  
// HISTORY:	  
// ==========================================================================  
//			1.00	08 May 1999	- Initial release.  
// ==========================================================================  
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabCtrlBarDoc.h"
#include "MdiClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiClient

CMdiClient::CMdiClient(): m_sizeClient(0, 0)
{
	m_crBkColor = GetSysColor(COLOR_DESKTOP);
	if (m_crBkColor == 0)
	{
		m_crBkColor = 0x00984e00;
	}
	m_pWndTabs = NULL;
}


CMdiClient::~CMdiClient()
{
}


BEGIN_MESSAGE_MAP(CMdiClient, CWnd)
	//{{AFX_MSG_MAP(CMdiClient)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MDICREATE,OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY,OnMDIDestroy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CMdiClient::AddHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	// 添加列表
	m_pWndTabs->AddHandle(hWnd);
}

void CMdiClient::RemoveHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	m_pWndTabs->RemoveHandle(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CMdiClient message handlers

LRESULT CMdiClient::OnMDICreate(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = (HWND) DefWindowProc(WM_MDICREATE,  wParam, lParam);
	AddHandle(hWnd);
	return (LRESULT) hWnd;
}

LRESULT CMdiClient::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
{
	RemoveHandle((HWND) wParam);
	return DefWindowProc(WM_MDIDESTROY,  wParam, lParam);
}



BOOL CMdiClient::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	// 当改变窗口大小或被移动或首次创建时，此函数被调用，它创建窗口的背景
	CRect rect;
	// 取得主窗口客户区矩形
	GetClientRect(&rect);	
	
	// 如果背景前色不为0, m_crBkColor在构造函数中初始化
	if (m_crBkColor != 0)
	{

		CBrush NewBrush(m_crBkColor); 
		// 设置刷子的起始原点
		pDC->SetBrushOrg(0, 0);
		// 选择新刷子，且将原刷子转换成CBrush类
		CBrush* pOldBrush = static_cast<CBrush*>(pDC->SelectObject(&NewBrush));  
		// 以拷贝方式绘制窗口背景
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY); 		
		pDC->SelectObject(pOldBrush);
		// 删除新刷子对象
		NewBrush.DeleteObject();
	}

    CRect rcDataBox;
	CString strLogo = "阿尔泰科技";//szLogoString;
	CFont fontLogo;
	TEXTMETRIC tm;
	
	// 将第一个32位参数与第二个32位参数相乘，再除以第三个32位数，最后取商
	int fontSize = -MulDiv(18, pDC->GetDeviceCaps(LOGPIXELSY), 72);
	
	// 创建字体
	fontLogo.CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_ROMAN, _T("Times New Roman"));
	// 设置背景模式为不透明方式（OPAQUE）
	pDC->SetBkMode(OPAQUE);		
	
	// 选择新字体
	CFont* oldFont = pDC->SelectObject(&fontLogo);
	CRect st(0, 0, 0, 0);
	
	// 取得给定字符串strLogo的矩形区域
	CSize sz = pDC->GetTextExtent(strLogo, strLogo.GetLength());
	// 根据当前字体，获得所选字体的度量
	pDC->GetTextMetrics(&tm);

	// Calculate the box size by subtracting the text width and height from the
	// window size.  Also subtract 20% of the average character size to keep the
	// logo from printing into the borders...

	// 取得客户区
	GetClientRect(&rcDataBox);
	
	rcDataBox.left = rcDataBox.right  - sz.cx - tm.tmAveCharWidth ;
	rcDataBox.top  = rcDataBox.bottom - sz.cy - st.bottom - tm.tmHeight ;
	
	CRect rcSave = rcDataBox;		
	// 设置背景模式为透明方式（TRANSPARENT）
	pDC->SetBkMode(TRANSPARENT); 
	rcSave = rcDataBox;
	
	// shift logo box right, and print black...
	rcDataBox.left   += tm.tmAveCharWidth / 5;
	// 设置文字色（黑色）
	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 0));
	// 绘制立体字的底层文字
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	rcDataBox = rcSave;
	
	//向左偏移文体位置为当前字体宽度的25%
	rcDataBox.left -= tm.tmAveCharWidth /4;
	// 设置立体字的中层字体色为白色
	pDC->SetTextColor(RGB(255, 255, 255));
	// 绘制中层字
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	
	rcDataBox = rcSave;
	// 设置文字为系统色
	pDC->SetTextColor(GetSysColor(COLOR_BTNFACE));
	// 在原位置绘制上层文字
	pDC->DrawText(strLogo, strLogo.GetLength(), &rcDataBox, 
		DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	
	// restore the original properties and release resources...
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);   
	pDC->SetBkMode(OPAQUE);	
	fontLogo.DeleteObject();
    // 释放公共DC和窗口DC，使其他应用程序可以使用，对类DC和私有DC无影响	
	ReleaseDC(pDC);
	
    return TRUE;	
	//return CMDIFrameWnd::OnEraseBkgnd(pDC);
}
void CMdiClient::OnSize(UINT nType, int cx, int cy) 
{
	// 当客户区域变化时，被调用。即当子帧窗口被最大化时，也会产生此消息
	CWnd::OnSize(nType, cx, cy);
	
	// 如果应用程序当启动，则保存这个大小参数，即可返回
    if ((m_sizeClient.cx == 0) && (m_sizeClient.cy == 0))
	{
        m_sizeClient.cx = cx;
        m_sizeClient.cy = cy;
		
        return ;
	}	
    // 如果客户窗口大小未发生变化，则返回
    if ((m_sizeClient.cx == cx) && ( m_sizeClient.cy == cy))
    { 
        return;
    }	
	// 保存新值
    m_sizeClient.cx = cx;
    m_sizeClient.cy = cy;
	// 强制重绘
    RedrawWindow(NULL, NULL,
        RDW_INVALIDATE | RDW_ERASE | RDW_ERASENOW | RDW_ALLCHILDREN);    
	
    return;                
}