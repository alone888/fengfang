// IOFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "DIOFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIOFrm

IMPLEMENT_DYNCREATE(CDIOFrm, CMDIChildWnd)

CDIOFrm::CDIOFrm()
{
	theApp.m_pDIOfrm = this;
}

CDIOFrm::~CDIOFrm()
{
	theApp.m_pDIOfrm = NULL;
}


BEGIN_MESSAGE_MAP(CDIOFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDIOFrm)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIOFrm message handlers

void CDIOFrm::OnDestroy() 
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
    pApp->m_bCreateDIOFrm = FALSE;
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDIOFrm::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
