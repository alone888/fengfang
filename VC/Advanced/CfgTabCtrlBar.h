#ifndef AFX_SIZINGTABCTRLBAR_H__3804CCE3_C325_11D1_91F5_B4375BB20A38__INCLUDED_
#define AFX_SIZINGTABCTRLBAR_H__3804CCE3_C325_11D1_91F5_B4375BB20A38__INCLUDED_

// SizingTabCtrlBar.h : Header-Datei
//
#include <afxtempl.h>
#include "CfgCtrlBar.h"

typedef struct
{
	CWnd* pWnd;
	WCHAR szLabel[32];
} TCB_ITEM;

/////////////////////////////////////////////////////////////////////////////
// Fenster CCfgTabCtrlBar 

class CCfgTabCtrlBar : public CCfgCtrlBar
{
// Konstruktion
public:
	CCfgTabCtrlBar();

// Attribute
public:
	CButton m_Button_StartDevice;
	CButton m_Button_StopDevice;

// Operationen
public:

	void SetActiveView(int nNewTab);
	void SetActiveView(CRuntimeClass* pViewClass);
	//{{AFX_VIRTUAL(CCfgTabCtrlBar)
	//}}AFX_VIRTUAL

// Implementierung
public:
	CView* GetActiveView();
	CView* GetView(int nView);
	CView* GetView(CRuntimeClass* pViewClass);
	CWnd* AddView(LPCTSTR lpszLabel, CRuntimeClass* pViewClass, CCreateContext* pContext = NULL);
	void RemoveView(int nView);
	CTabCtrl m_tabctrl;
	CButton	m_test;
	virtual ~CCfgTabCtrlBar();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	int m_nActiveTab;
	CView* m_pActiveView;

	CList <TCB_ITEM *, TCB_ITEM *> m_views;
	CImageList m_images;
	//{{AFX_MSG(CCfgTabCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // AFX_SIZINGTABCTRLBAR_H__3804CCE3_C325_11D1_91F5_B4375BB20A38__INCLUDED_
