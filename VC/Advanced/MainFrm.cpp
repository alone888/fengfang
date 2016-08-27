// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR, 		// status line indicator
	ID_SHOWMOUSEVOLT,   //显示鼠标的指针的位置
	ID_SHOWMOUSEVOLT, 	// status line indicator
	ID_INDICATOR_CAPS, 
	ID_INDICATOR_NUM, 
	ID_INDICATOR_SCRL, 
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here	
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	


	//if (CreateTabs()==-1)
	//	return -1;
	
	// 调用CreateEx函数创建工具栏，并调用LoadToolBar函数加载工具栏资源   
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | /*CBRS_FLOATING|*/CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||   
        !m_wndToolBar.LoadToolBar(IDR_TABVIEW_MENU))   
    {   
        TRACE0("Failed to create toolbar\n");   
        return -1;      // fail to create   
    }
	int i = m_wndToolBar.CommandToIndex( ID_TOOLSBAR_START);
	m_wndToolBar.SetButtonInfo(i, ID_TOOLSBAR_START, TBBS_GROUP|TBBS_CHECKBOX    , i);
	m_wndToolBar.SetButtonInfo(i+1, ID_TOOLSBAR_STOP, TBBS_GROUP |TBBS_CHECKBOX  , i+1);
	m_wndToolBar.IsDlgButtonChecked(ID_TOOLSBAR_STOP);

         // 为m_wndToolBar启用停靠   
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);   
         // 为框架窗口启用停靠   
    EnableDocking(CBRS_ALIGN_ANY);  
		DockControlBar((CToolBar*)&m_wndToolBar);
	//DockPane(&m_wndToolBar);

	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_TOOLSBAR_START);
	lstBasicCommands.AddTail(ID_TOOLSBAR_STOP);
	lstBasicCommands.AddTail(ID_TOOLSBAR_NEW_FILE);
	lstBasicCommands.AddTail(ID_TOOLSBAR_OPEN_FILE);
	lstBasicCommands.AddTail(ID_TOOLSBAR_SAVE_FILE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



