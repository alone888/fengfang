#if !defined(AFX_HistWaveView_H__D442B984_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
#define AFX_HistWaveView_H__D442B984_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistWaveView.h : header file
//
#include "ADHistScope.h"

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView view

class CADHistWaveView : public CScrollView
{
protected:
	CADHistWaveView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADHistWaveView)
	
// Attributes
public:

	float m_PerPixelLsb;	// ÿ�����ط�������
	int center;	
	int PerY;
	long Offset;  // ��¼��8192���е�ǰƫ��λ��
	int Index;
	int  m_ScreenVolume; // ������Ļ��ʾ������
	int  m_TriggerVolt; // ������ѹ
	int m_MiddleLsb;
	int m_nOldDataPos; // save the cursor positon of data buffer
	BOOL m_bFirTriPos;
	CADHistScope m_ADHistScope;

	// Operations
public:
	CRect ClientRect;
	BOOL m_Draw;
	CPoint oldPoint;
	CPoint m_TriPospoint;
	CPoint m_PointA;
	BOOL m_bCapture;
// 	int m_nWidth;
	POINT	 pointxy[32][8192]; // ����ͨ����ʾ�ĵ�����
	CPoint	 pointTemp[8192];  // ������ŵ�ͨ����ʾ�ĵ�����
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistWaveView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CADHistWaveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CADHistWaveView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HistWaveView_H__D442B984_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
