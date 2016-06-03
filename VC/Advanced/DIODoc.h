#if !defined(AFX_IODOC_H__B8A8D100_1478_11D5_B883_D6511767BB0F__INCLUDED_)
#define AFX_IODOC_H__B8A8D100_1478_11D5_B883_D6511767BB0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IODoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDIODoc document

class CDIODoc : public CDocument
{
protected:
	CDIODoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDIODoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIODoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDIODoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDIODoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IODOC_H__B8A8D100_1478_11D5_B883_D6511767BB0F__INCLUDED_)
