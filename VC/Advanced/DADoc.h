#if !defined(AFX_DADOC_H__F0D1AF99_BB70_4404_8941_1C4C11EDF0A5__INCLUDED_)
#define AFX_DADOC_H__F0D1AF99_BB70_4404_8941_1C4C11EDF0A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DADoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDADoc document

class CDADoc : public CDocument
{
protected:
	CDADoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDADoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDADoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDADoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDADoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DADOC_H__F0D1AF99_BB70_4404_8941_1C4C11EDF0A5__INCLUDED_)
