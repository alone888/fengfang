// DAFrm.cpp : implementation file
//

#include "stdafx.h"
#include "sys.h"
#include "DAFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDAFrm

IMPLEMENT_DYNCREATE(CDAFrm, CMDIChildWnd)

CDAFrm::CDAFrm()
{
	theApp.m_pDAFrm = this;
}

CDAFrm::~CDAFrm()
{
	theApp.m_pDAFrm = NULL;
}


BEGIN_MESSAGE_MAP(CDAFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDAFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDAFrm message handlers
