// MTabWnd.cpp:implementation file
//
/////////////////////////////////////////////////////////////////////////////
// Derive your mainframe window from this class to get support for the
// MDI tabs. Also adds support for next/previous MDI window selection.
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
#include "Sys.h"
#include "MTabWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDITabFrameWnd

IMPLEMENT_DYNCREATE(CMDITabFrameWnd, CMDIFrameWnd)

CMDITabFrameWnd::CMDITabFrameWnd()
{
}

CMDITabFrameWnd::~CMDITabFrameWnd()
{
}

BEGIN_MESSAGE_MAP(CMDITabFrameWnd, CMDIFrameWnd)
	ON_WM_INITMENU()
	//{{AFX_MSG_MAP(CMDITabFrameWnd)
	ON_COMMAND(ID_WINDOW_NEXT_PANE, OnWindowNextPane)
	ON_COMMAND(ID_WINDOW_PREV_PANE, OnWindowPrevPane)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMDITabFrameWnd::CreateTabs(void) 
{
	// m_wndTabs是CControlBar派生类
	if (!m_wndTabs.Create(this, 
		WS_VISIBLE | WS_CHILD | CBRS_TOP | WS_EX_WINDOWEDGE, 
		IDC_MDI_TAB_CTRL_BAR))
	{
		TRACE("Failed to create test status bar\n");
		return -1;      // fail to create
	}

	m_wndMdiClient.m_pWndTabs = &m_wndTabs;
	
	ASSERT(m_hWndMDIClient != NULL);


	// 通过注册m_hWndMDIClient窗口类，便可以使它得到添加和移去TAB控件的消息
	if (!m_wndMdiClient.SubclassWindow(m_hWndMDIClient))
	{
		TRACE("Failed to subclass MDI client\n");
		return -1;      // fail to create
	}	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMDITabFrameWnd message handlers
void CMDITabFrameWnd::OnWindowNextPane() 
{
	MDINext();	
}

void CMDITabFrameWnd::OnWindowPrevPane() 
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, 1); 
}

void CMDITabFrameWnd::OnInitMenu(CMenu* pMenu)
{
	CMDIFrameWnd::OnInitMenu(pMenu);
	
	
	// Add Tip of the Day menu item on the fly!
	static CMenu* pSubMenu = NULL;
	
	CString strHelp; strHelp.LoadString(CG_IDS_TIPOFTHEDAYHELP);
	CString strMenu;
	int nMenuCount = pMenu->GetMenuItemCount();
	BOOL bFound = FALSE;
	for (int MenuItem=0; MenuItem<nMenuCount; MenuItem++) 
	{
		pMenu->GetMenuString(MenuItem, strMenu, MF_BYPOSITION);
		if (strMenu == strHelp)
		{ 
			pSubMenu = pMenu->GetSubMenu(MenuItem);
			bFound = TRUE;
			ASSERT(pSubMenu != NULL);
		}
	}
	
	CString strTipMenu;
	strTipMenu.LoadString(CG_IDS_TIPOFTHEDAYMENU);
	if (!bFound)
	{
		// Help menu is not available. Please add it!
		if (pSubMenu == NULL) 
		{
			// The same pop-up menu is shared between mainfrm and frame 
			// with the doc.
			static CMenu popUpMenu;
			pSubMenu = &popUpMenu;
			pSubMenu->CreatePopupMenu();
			pSubMenu->InsertMenu(0, MF_STRING | MF_BYPOSITION, 
				CG_IDS_TIPOFTHEDAY, strTipMenu);
		}
		
		pMenu->AppendMenu(MF_STRING | MF_BYPOSITION | MF_ENABLED | MF_POPUP, 
			(UINT_PTR)pSubMenu->m_hMenu, strHelp);

		DrawMenuBar();
	} 
	else
	{      
		// Check to see if the Tip of the Day menu has already been added.
		pSubMenu->GetMenuString(0, strMenu, MF_BYPOSITION);
		
		if (strMenu != strTipMenu) 
		{
			// Tip of the Day submenu has not been added to the 
			// first position, so add it.
			pSubMenu->InsertMenu(0, MF_BYPOSITION);  // Separator
			pSubMenu->InsertMenu(0, MF_STRING|MF_BYPOSITION, 
				CG_IDS_TIPOFTHEDAY, strTipMenu);
		}
	}	
}
