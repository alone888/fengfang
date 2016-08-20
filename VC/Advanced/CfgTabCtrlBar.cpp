// SizingTabCtrlBar.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Sys.h"

#include "CfgTabCtrlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgTabCtrlBar

CCfgTabCtrlBar::CCfgTabCtrlBar()
{
	m_nActiveTab = 0;
}

CCfgTabCtrlBar::~CCfgTabCtrlBar()
{
	while (!m_views.IsEmpty())
	{
		TCB_ITEM* pMember = m_views.RemoveHead();
		delete pMember;
	}
}

BEGIN_MESSAGE_MAP(CCfgTabCtrlBar, CCfgCtrlBar)
	//{{AFX_MSG_MAP(CCfgTabCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDI_TABCTRLBAR, OnTabSelChange)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f黵 Nachrichten CCfgTabCtrlBar 

int CCfgTabCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCfgCtrlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//Create the Tab Control
	CRect rect;
	m_tabctrl.Create(WS_VISIBLE|WS_CHILD,  
		rect, this, IDI_TABCTRLBAR);
	
	m_images.Create(IDB_TABIMAGES, 16, 1, RGB(255,0,255));  // 创建位图阵列
	m_tabctrl.SetImageList(&m_images); // 将位图阵列置入控件
	
	// set "normal" GUI-font
	CFont* font = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	m_tabctrl.SetFont(font);

	CRect StartButtonRect(10, 6, 180, 36);
	CRect StopButtonRect(10, 40, 180, 70);
	

	/*m_Button_StartDevice.Create(_T("开始数据采集"), 
	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON, 
	StartButtonRect, this, IDM_StartDeviceAD);

	m_Button_StopDevice.Create(_T("停止数据采集"), 
	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  
	StopButtonRect, this, IDM_StopDeviceAD);*/



	return 0;
}

void CCfgTabCtrlBar::OnSize(UINT nType, int cx, int cy) 
{
	CCfgCtrlBar::OnSize(nType, cx, cy);
	int bottom = (IsHorz() || IsFloating()) ? cy - 82 : cy - 84;
	m_tabctrl.MoveWindow(7, 77, cx , bottom); // 在CControlBar在移动Tab控件位置
	CWnd* pWnd;
	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		pWnd = m_views.GetAt(pos)->pWnd;
		pWnd->MoveWindow(9, 99, cx-14, bottom-25); // 在CControlBar在移动视图类的控件位置
	}
}

CWnd* CCfgTabCtrlBar::AddView(LPCTSTR lpszLabel, CRuntimeClass* pViewClass, CCreateContext* pContext)
{	

#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	CCreateContext context;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
		CView* pOldView = NULL;
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(context.m_pCurrentFrame == NULL);
			context.m_pLastView = pOldView;
			context.m_pCurrentDoc = pOldView->GetDocument();
			if (context.m_pCurrentDoc != NULL)
				context.m_pNewDocTemplate =
				context.m_pCurrentDoc->GetDocTemplate();
		}
		pContext = &context;
	}
	
	CWnd* pWnd;

	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
	
    ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CRect rect, rect1;
	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	m_pActiveView = (CView*)pWnd;


	TCB_ITEM* pMember = new TCB_ITEM;
	pMember->pWnd = pWnd;
	wcscpy_s(pMember->szLabel, lpszLabel);
	m_views.AddTail(pMember);

	INT_PTR nViews = m_views.GetCount();
	if (nViews != 1)
	{
		pWnd->EnableWindow(FALSE);
		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		((CFrameWnd*)GetParent())->SetActiveView((CView*)m_pActiveView);
	}

	TC_ITEM tci;
	tci.mask = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText = (LPTSTR)(LPCTSTR)lpszLabel;
	tci.iImage = (int)nViews-1;
	m_tabctrl.InsertItem((int)nViews, &tci);

	return pWnd;
}

void CCfgTabCtrlBar::RemoveView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	// remove the page from internal list
	m_views.RemoveAt(m_views.FindIndex(nView));
}

void CCfgTabCtrlBar::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetActiveView(m_tabctrl.GetCurSel());
}

void CCfgTabCtrlBar::SetActiveView(int nNewTab)
{
	ASSERT_VALID(this);
	ASSERT(nNewTab >= 0);

	if ((nNewTab != -1) && (nNewTab != m_nActiveTab))
	{
        TCB_ITEM* newMember = m_views.GetAt(m_views.FindIndex(nNewTab));
        TCB_ITEM* oldMember = NULL;
		
        if (m_nActiveTab != -1)
        {
            oldMember = m_views.GetAt(m_views.FindIndex(m_nActiveTab));
            oldMember->pWnd->EnableWindow(FALSE);
            oldMember->pWnd->ShowWindow(SW_HIDE);
        }
        newMember->pWnd->EnableWindow(TRUE);
        newMember->pWnd->ShowWindow(SW_SHOW);
        newMember->pWnd->SetFocus();

        m_pActiveView = (CView*)newMember->pWnd;
		((CFrameWnd*)GetParent())->SetActiveView(m_pActiveView);

        m_nActiveTab = nNewTab;
		// select the tab (if tab programmatically changed)
		m_tabctrl.SetCurSel(m_nActiveTab);
    }
}

void CCfgTabCtrlBar::SetActiveView(CRuntimeClass* pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	int nNewTab = 0;
	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM* pMember = m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			//first hide old first view
            m_pActiveView->EnableWindow(FALSE);
            m_pActiveView->ShowWindow(SW_HIDE);
			
			// set new active view
			m_pActiveView = (CView*)pMember->pWnd;
			// enable, show, set focus to new view
			m_pActiveView->EnableWindow(TRUE);
			m_pActiveView->ShowWindow(SW_SHOW);
			m_pActiveView->SetFocus();
			
			((CFrameWnd*)GetParent())->SetActiveView(m_pActiveView);

	        m_nActiveTab = nNewTab;
			// select the tab
			m_tabctrl.SetCurSel(m_nActiveTab);
			break;
		}
		nNewTab++;
    }
}

CView* CCfgTabCtrlBar::GetActiveView()
{
	return m_pActiveView;
}

CView* CCfgTabCtrlBar::GetView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	if (nView != -1)
	{
        TCB_ITEM* pMember = m_views.GetAt(m_views.FindIndex(nView));
		return (CView*)pMember->pWnd;
	}
	else
		return NULL;
}

CView* CCfgTabCtrlBar::GetView(CRuntimeClass* pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM* pMember = m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			return (CView*)pMember->pWnd;
		}
    }
	return NULL;
}

