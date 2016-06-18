#if !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
#define AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaCfgView.h : header file

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ADFrm.h"
#include "afxwin.h"

class CADParaCfgView : public CFormView
{
protected:
	CADParaCfgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADParaCfgView)
public:
	CADFrm* m_pADFrm;
// 	void SetInputRange(int nInputRange, int nChannel);
	void SetInputRangeEx(int nInputRange, int nChannel);
	BOOL SaveFrequency(LONG Frequency);
	LONG LoadFrequency(LONG Frequency[256]);

// Form Data
public:
	//{{AFX_DATA(CADParaCfgView)
	enum { IDD = CG_IDD_AD_CFG };
	CComboBox	m_Combo_GroundingMode;
	CEdit	m_Edit_TrigWindow;
	CEdit	m_Edit_ChannelSum;
	CComboBox	m_Combo_TriggerSource;
	CEdit	m_Edit_GetDevTrigPosAD;
	CStatic	m_Static_Art;
	CComboBox	m_Combo_Range;
	CButton	m_Check_bClockOutput;
	CEdit	m_Edit_LoopsOfGroup;
	CEdit	m_Edit_GroupInterval;
	CComboBox	m_Combo_TriggerDir;
	CComboBox	m_Combo_LastChannel;
	CComboBox	m_Combo_Gains;
	CComboBox	m_Combo_FirstChannel;
	CComboBox	m_Combo_CheckStsMode;
	CComboBox	m_Combo_ADMode;
	CComboBox	m_Combo_Frequency;
	CComboBox	m_Combo_TriggerMode;
	CComboBox	m_Combo_TriggerType;
	CComboBox	m_Combo_ClockSource;
	CStatic	m_Static_Offset;
	CSpinButtonCtrl	m_Spin_BufferID;
	CSliderCtrl	m_Slider_Offset;
	CEdit		m_Edit_BufferID;
	//}}AFX_DATA
protected:
	int m_nSyncSignalSrc[USB2831_MAX_AD_CHANNELS]; // 同(异)步切换前, 同步时信号源的选择
	int m_nSyncInputRange[USB2831_MAX_AD_CHANNELS]; // 同(异)步切换前, 同步时电压量程的选择
	int m_nAsynInputRange[USB2831_MAX_AD_CHANNELS]; // 同(异)步切换前, 异步时电压量程的选择
	int m_nSyncFrequency;    // 切换采集模式时保存同步时的频率值
	int m_nAsynFrequency;    // 切换采集模式时保存异步时的频率值

// Attributes
public:
	BOOL m_bInitialUpdate;
// Operations
public:
	void SetSignalSrcComboSel(int* pSignalSrc);
	void EnableControlWindows(BOOL Flag = TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADParaCfgView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetStatusBar();
	void SetFreqComboSel(int nFrequecy);
	virtual ~CADParaCfgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CADParaCfgView)
	afx_msg void OnSelchangeCOMBOTriggerSource();
	afx_msg void OnSelchangeCOMBOTriggerDir();
	afx_msg void OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEDITBufferID();
	afx_msg void OnSelchangeCOMBOClockSource();
	afx_msg void OnSelchangeCOMBOTriggerType();
	afx_msg void OnCHECKClockOutput();
	afx_msg void OnSelchangeCOMBOTriggerMode();
	afx_msg void OnKillfocusCOMBOFrequency();
	afx_msg void OnChangeEDITSensitive();
	afx_msg void OnSelchangeCOMBOFirstChannel();
	afx_msg void OnSelchangeCOMBOLastChannel();
	afx_msg void OnSelchangeCOMBORange();
	afx_msg void OnSelchangeCOMBOGains();
	afx_msg void OnSelchangeCOMBOADMode();
	afx_msg void OnChangeEDITGroupInterval();
	afx_msg void OnChangeEDITLoopsOfGroup();
	afx_msg void OnSelchangeCOMBOCheckStsMode();
	afx_msg void OnChangeEDITTrigWindow();
	afx_msg void OnSelchangeCOMBOGroundingMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckInput1();
	afx_msg void OnBnClickedCheckInput9();
	CButton m_singal_1;
	CButton m_singal_2;
	CButton m_singal_3;
	CButton m_singal_4;
	CButton m_input_1;
	CButton m_input_2;
	CButton m_input_3;
	CButton m_input_4;
	afx_msg void OnBnClickedCheckInput10();
	afx_msg void OnBnClickedCheckInput11();
	afx_msg void OnBnClickedCheckInput12();
	afx_msg void OnBnClickedCheckInput13();
	afx_msg void OnBnClickedCheckInput14();
	afx_msg void OnBnClickedCheckInput15();
	afx_msg void OnBnClickedCheckInput16();
	CEdit m_TimerCnt;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
