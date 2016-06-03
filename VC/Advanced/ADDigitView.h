// ADDigitView.h : interface of the CADDigitView class

#if !defined(AFX_ADDIGITVIEW_H__D485DF07_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_ADDIGITVIEW_H__D485DF07_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CADDigitView : public CScrollView
{
protected:
	CADDigitView();
	DECLARE_DYNCREATE(CADDigitView)
// Attributes 
public:
	int m_CurrentProgress;
	int m_iDataTxtWidth;
	int m_iDataTxtHeight;
	float m_fADVolt[USB2831_MAX_AD_CHANNELS][AD_LSB_HALF * 2];
// Operations
public:
	void ADDigit(CDC* pDC);
	CADDoc* GetDocument();
	CADDoc* m_pADDoc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADDigitView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	void TransitionData();
	LRESULT ShowDigit(WPARAM wParam, LPARAM lParam);
	virtual ~CADDigitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CADDigitView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnShowHex();
	afx_msg void OnUpdateShowHex(CCmdUI* pCmdUI);
	afx_msg void OnShowDec();
	afx_msg void OnUpdateShowDec(CCmdUI* pCmdUI);
	afx_msg void OnShowVolt();
	afx_msg void OnUpdateShowVolt(CCmdUI* pCmdUI);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CADDoc* GetDocument();
#ifndef _DEBUG  // debug version in ADDigitView.cpp
inline CADDoc* CADDigitView::GetDocument()
   { return (CADDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDigitVIEW_H__D485DF07_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
