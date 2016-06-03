// DADoc.cpp : implementation file
//

#include "stdafx.h"
#include "sys.h"
#include "DADoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDADoc

IMPLEMENT_DYNCREATE(CDADoc, CDocument)

CDADoc::CDADoc()
{
}

BOOL CDADoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDADoc::~CDADoc()
{
}


BEGIN_MESSAGE_MAP(CDADoc, CDocument)
	//{{AFX_MSG_MAP(CDADoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDADoc diagnostics

#ifdef _DEBUG
void CDADoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDADoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDADoc serialization

void CDADoc::Serialize(CArchive& ar)
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
// CDADoc commands
