#if !defined(AFX_IOVIEW_H__B8A8D101_1478_11D5_B883_D6511767BB0F__INCLUDED_)
#define AFX_IOVIEW_H__B8A8D101_1478_11D5_B883_D6511767BB0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IOView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDIOView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDIOView : public CFormView
{
protected:
	CDIOView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDIOView)
public:
	int DeviceID;
	HANDLE hDevice;
	BOOL m_bCreateDevice;
	BOOL Status[16];
	ULONG LinearAddr, PhysAddr;
	BYTE m_byDI[16];
	BYTE m_byDO[16];

	void SetSwitchStatus(void);
	void SetSwitchOut(void);

// Form Data
public:
	//{{AFX_DATA(CDIOView)
	enum { IDD = IDD_DIALOG_DIO_View };
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIOView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDIOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDIOView)
	afx_msg void OnCheckDo();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IOVIEW_H__B8A8D101_1478_11D5_B883_D6511767BB0F__INCLUDED_)
