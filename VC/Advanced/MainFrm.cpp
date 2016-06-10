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
	ID_SHOWMOUSEVOLT,   //��ʾ����ָ���λ��
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
	
	// ����CreateEx����������������������LoadToolBar�������ع�������Դ   
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | /*CBRS_FLOATING|*/CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||   
        !m_wndToolBar.LoadToolBar(IDR_TABVIEW_MENU))   
    {   
        TRACE0("Failed to create toolbar\n");   
        return -1;      // fail to create   
    }

         // Ϊm_wndToolBar����ͣ��   
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);   
         // Ϊ��ܴ�������ͣ��   
    EnableDocking(CBRS_ALIGN_ANY);  
		DockControlBar((CToolBar*)&m_wndToolBar);
	//DockPane(&m_wndToolBar);

	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW_TEST);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);


	CMFCToolBar::SetBasicCommands(lstBasicCommands);

// 	if (CMFCToolBar::GetUserImages() == NULL)
// 	{
// 		// �����û�����Ĺ�����ͼ��
// 		if (m_UserImages.Load(_T(".\\Res\\toolbar_32_32.bmp")))
// 		{
// 			m_UserImages.SetImageSize(CSize(32, 32), FALSE);   s
// 			CMFCToolBar::SetSizes(CSize (34, 40), CSize (32, 32));   //���ô�ͼ��
// 			CMFCToolBar::SetUserImages(&m_UserImages); 
// 
// 			//CMFCToolBar::SetUserImages(&m_UserImages);
// 		}
// 	}
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



