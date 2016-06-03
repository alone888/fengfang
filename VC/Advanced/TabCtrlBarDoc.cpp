// tabbar.cpp : implementation file
//
// This class implements a tab control for MDI windows.
// The tab labels are filled automatically with the titles of the MDI frames.
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
// the startup banner, "About" box or printed documentation. An email     
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
#include <afxpriv.h>
//#include <..\src\afximpl.h>

#if _MFC_VER <= 0x0600 
#include <..\src\afximpl.h> // °üº¬ VC 6.0 AFX_DATA AUX_DATA afxData 
#else 
#include <..\src\mfc\afximpl.h> // VC 7.0 °üº¬AFX_DATA AUX_DATA afxData 
#endif // _MFC_VER > 1000

#include "resource.h"
#include "TabCtrlBarDoc.h"

// RCS info
static char rcsid[] = "$Id: $";

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CTabCtrlBarDoc

CTabCtrlBarDoc::CTabCtrlBarDoc()
{
}

IMPLEMENT_DYNAMIC(CTabCtrlBarDoc, CControlBar)

BEGIN_MESSAGE_MAP(CTabCtrlBarDoc, CControlBar)
	//{{AFX_MSG_MAP(CTabCtrlBarDoc)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_MDI_TAB_CTRL, OnSelchange)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////

BOOL CTabCtrlBarDoc::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// save the style (some of these style bits are MFC specific)
	m_dwStyle = CBRS_ALL;
	// translate MFC style bits to windows style bits
	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE;
	if (pParentWnd->GetStyle() & WS_THICKFRAME)
		dwStyle |= SBARS_SIZEGRIP;
	
	// initialize common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));

	// create the HWND
	CRect rect; rect.SetRectEmpty();

	return CWnd::Create(STATUSCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlBarDoc message handlers

int CTabCtrlBarDoc::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect; rect.SetRectEmpty();
	//Create the Tab Control
	if (!m_tabctrl.Create(WS_VISIBLE|WS_CHILD, rect, this, IDC_MDI_TAB_CTRL))
	{
		TRACE0("Unable to create tab control bar\n");
		return -1;
	}
 
	// set "regular" GUI-font
	// Use a smaller font for smaller tabs
	CFont* font = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	m_tabctrl.SetFont(font);

	return 0;
}

void CTabCtrlBarDoc::OnSize(UINT nType, int cx, int cy) 
{
	CControlBar::OnSize(nType, cx, cy);
	CRect rect;
	if (m_tabctrl.GetSafeHwnd())
	{
		GetWindowRect(rect);
		m_tabctrl.AdjustRect(TRUE, &rect);
		m_tabctrl.SetWindowPos(&wndTop, 0, 0, rect.Width()-7, rect.Height(), SWP_SHOWWINDOW);
	}
}

void CTabCtrlBarDoc::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	SetTitles();
}

// This is called in idle time.
// Here we update the tab labels and selection
void CTabCtrlBarDoc::SetTitles(void)
{
	CString csName;
	TC_ITEM tci;
	WCHAR buf[MAX_PATH+1];
	CMDIFrameWnd* pMainframe = ((CMDIFrameWnd *)AfxGetMainWnd());
	if (pMainframe == NULL)
		return;	// no official mainframe window yet

	CMDIChildWnd* pActive = pMainframe->MDIGetActive(NULL);
	
	// update all tab labels if neccessary
	int numitems = m_tabctrl.GetItemCount();
	for (int item = 0; item < numitems; item++)
	{
		csName.Empty();
		tci.mask = TCIF_PARAM|TCIF_TEXT;
		tci.pszText = buf;
		tci.cchTextMax = MAX_PATH;
		if (!m_tabctrl.GetItem(item, &tci))
			continue;	// skip bad ones (never saw one yet)

		CWnd *pFrame = FromHandle((HWND) tci.lParam);
		ASSERT(pFrame != NULL);
		if (pFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		{
			pFrame->GetWindowText(csName);
			if (buf != csName)	// avoid flicker
			{
				tci.mask = TCIF_TEXT;
				tci.pszText = csName.LockBuffer();
				m_tabctrl.SetItem(item, &tci);
				csName.UnlockBuffer();
			}

			if (pActive == pFrame)	// mark the active one 
				m_tabctrl.SetCurSel(item);
		}
	}
}

// Add a new window handle to the tab control
// Also creates a new tab
void CTabCtrlBarDoc::AddHandle(HWND hWnd)
{
	CWnd* pFrame = FromHandle(hWnd);
	ASSERT(pFrame != NULL);
	if (pFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
	{
		TC_ITEM tci;
		tci.mask = TCIF_PARAM|TCIF_TEXT;
		tci.pszText = _T("");
		tci.lParam = (LPARAM) hWnd;
		m_tabctrl.InsertItem(m_tabctrl.GetItemCount(), &tci);
	}
}

// Remove a new window handle from the tab control
// Also removes the tab
void CTabCtrlBarDoc::RemoveHandle(HWND hWnd)
{
	TC_ITEM tci;
	int numitems = m_tabctrl.GetItemCount();
	for (int item = 0; item < numitems; item++)
	{
		tci.mask = TCIF_PARAM;
		m_tabctrl.GetItem(item, &tci);
		if (tci.lParam == (LPARAM) hWnd)
		{
			m_tabctrl.DeleteItem(item);
			break;
		}
	}
}

// The user clicked onto a tab.
// Now select a new MDI child frame
void CTabCtrlBarDoc::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int idx = m_tabctrl.GetCurSel();

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	if (m_tabctrl.GetItem(idx, &tci))
	{
		CWnd* pFrame = FromHandle((HWND) tci.lParam);
		ASSERT(pFrame != NULL);
		ASSERT_KINDOF(CMDIChildWnd, pFrame);
		((CMDIFrameWnd*)AfxGetMainWnd())->MDIActivate(pFrame);
	}

	*pResult = 0;
}

// Almost a copy from CStatusbar:
// The framework calls this function to get an idea of the size.
CSize CTabCtrlBarDoc::CalcFixedLayout(BOOL, BOOL bHorz)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// determinme size of font being used by the status bar
	TEXTMETRIC tm;
	{
		CClientDC dc(NULL);
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
		HGDIOBJ hOldFont = NULL;
		if (hFont != NULL)
			hOldFont = dc.SelectObject(hFont);
		VERIFY(dc.GetTextMetrics(&tm));
		if (hOldFont != NULL)
			dc.SelectObject(hOldFont);
	}

	// get border information
	CRect rect; rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);
	int rgBorders[3];
	DefWindowProc(SB_GETBORDERS, 0, (LPARAM)&rgBorders);

	// determine size, including borders
	CSize size;
	size.cx = 32767;
	size.cy = tm.tmHeight - tm.tmInternalLeading - 1
		+ rgBorders[1] * 2 + ::GetSystemMetrics(SM_CYBORDER) * (2+3)
		- rect.Height();

	return size;
}

