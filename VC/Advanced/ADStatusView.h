#if !defined(AFX_ADStatusView_H__E4B4B7A2_D13C_11D6_B1D2_AB386BFF0020__INCLUDED_)
#define AFX_ADStatusView_H__E4B4B7A2_D13C_11D6_B1D2_AB386BFF0020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CADStatusView form view
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CADStatusView : public CFormView
{
protected:
	CADStatusView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADStatusView)

// Form Data
public:
	//{{AFX_DATA(CADStatusView)
	enum { IDD = CG_IDD_Capability };
	CComboBox	m_Combo_ScreenVolume;
	CComboBox	m_Combo_MoveVolt;
	CEdit	    m_Edit_AnalyzeCount;
	CEdit	    m_Edit_OverRange;
	CEdit	    m_Edit_OverRatio;
	CEdit	    m_Edit_TringgerVolt;
	
	CEdit       m_signel[8][2];

	//}}AFX_DATA

// Attributes
public:
	void SetDlgCheck();

// Operations
public:
	void EnableControlWindows(BOOL bFlag = TRUE);
	void AnalyzeAccurate(); // ¾«¶È·ÖÎö

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADStatusView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CADStatusView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CADStatusView)
	afx_msg void OnChangeEDITTriggerVolt();
	afx_msg void OnGraphicShow();
	afx_msg void OnDigitalShow();
	afx_msg void OnDataSave();
	afx_msg void OnSelchangeComboScreenVolume();
	afx_msg void OnCHECKStopFirstDigitalShow();

	afx_msg void OnChangeEDIT2();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADStatusView_H__E4B4B7A2_D13C_11D6_B1D2_AB386BFF0020__INCLUDED_)
