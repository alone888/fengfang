#if !defined(AFX_IOFRAME_H__B8A8D102_1478_11D5_B883_D6511767BB0F__INCLUDED_)
#define AFX_IOFRAME_H__B8A8D102_1478_11D5_B883_D6511767BB0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IOFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDIOFrm frame

class CDIOFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDIOFrm)
protected:
	CDIOFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIOFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDIOFrm();

	// Generated message map functions
	//{{AFX_MSG(CDIOFrm)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IOFRAME_H__B8A8D102_1478_11D5_B883_D6511767BB0F__INCLUDED_)
