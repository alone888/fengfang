// IODoc.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "DIODoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIODoc

IMPLEMENT_DYNCREATE(CDIODoc, CDocument)

CDIODoc::CDIODoc()
{
}

BOOL CDIODoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDIODoc::~CDIODoc()
{
}


BEGIN_MESSAGE_MAP(CDIODoc, CDocument)
	//{{AFX_MSG_MAP(CDIODoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIODoc diagnostics

#ifdef _DEBUG
void CDIODoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIODoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIODoc serialization

void CDIODoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDIODoc commands
