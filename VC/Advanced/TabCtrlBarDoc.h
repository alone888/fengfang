// titlebar.h : header file
//
#ifndef _TABBAR_H_58A62F03_FEFE_11D2_BC0D_006008CCD137
#define _TABBAR_H_58A62F03_FEFE_11D2_BC0D_006008CCD137

/////////////////////////////////////////////////////////////////////////////
#include "TabCtrlDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CTabBar window
//#define CTabBar_parent CControlBar 
class CTabCtrlBarDoc : public CControlBar
{
	DECLARE_DYNAMIC(CTabCtrlBarDoc)
// Construction
public:
// Construction
public:
	CTabCtrlBarDoc();

// Attributes
public:
	void RemoveHandle(HWND hWnd);
	void AddHandle(HWND hWnd);
	void SetTitles(void);
	
// Operations
public:
	BOOL Create(CWnd* pParentWnd,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		UINT nID = AFX_IDW_STATUS_BAR);

// Implementation
public:
//	virtual ~CTabBar();
  	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	CTabCtrlDoc m_tabctrl;
	
// Overrides
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCtrlBarDoc)
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CTabCtrlBarDoc)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif

