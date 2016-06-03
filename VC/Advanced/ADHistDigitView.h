#if !defined(AFX_HistDigitView_H__D442B983_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
#define AFX_HistDigitView_H__D442B983_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistDigitView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView view

class CADHistDigitView : public CListView
{
protected:
	CADHistDigitView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADHistDigitView)

// Attributes
public:
	CADHistDoc* GetDocument();
    void SetListView(void);
// Operations
public:
	void SetHotItem(int Index);
	float m_PerLsbVolt;
	WCHAR str[100];
	int nChooseMode; // 1、十进制显示 2、十六进制显示 3、电压显示

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistDigitView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CADHistDigitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CADHistDigitView)
	afx_msg void OnShowHex();
	afx_msg void OnUpdateShowHex(CCmdUI* pCmdUI);
	afx_msg void OnShowVolt();
	afx_msg void OnUpdateShowVolt(CCmdUI* pCmdUI);
	afx_msg void OnShowDec();
	afx_msg void OnUpdateShowDec(CCmdUI* pCmdUI);
	afx_msg void OnRefurbish();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ShowDigitView.cpp
inline CADHistDoc* CADHistDigitView::GetDocument()
   { return (CADHistDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HistDigitView_H__D442B983_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
