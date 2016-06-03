#if !defined(AFX_DAVIEW_H__CAF338D1_D883_420B_91B1_95594D6F7007__INCLUDED_)
#define AFX_DAVIEW_H__CAF338D1_D883_420B_91B1_95594D6F7007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DAView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDAView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

typedef struct _THREAD_PARA
{
	HANDLE m_hDevice;
	HANDLE m_hEventEnd;
	WORD m_pWaveValue[16384];
	WORD m_DigitValue;
	BOOL m_bDeviceRun;
	BOOL m_bConstant;
	int DeviceID;
	int DAChannelNum;
	int m_nWriteDADataPointCount;
	LONG m_nOutputRange;
} THREAD_PARA, *PTHREAD_PARA;

class CDAView : public CFormView
{
protected:
	CDAView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDAView);
// static	UINT IDC_SLIDER[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_EDIT_Cycle[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_CHECK_WaveDA[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_EDIT_Value[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_PICTURE_WAVE[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_BUTTON_StartDA[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_BUTTON_StopDA[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_SPIN_Cycle[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_SPIN_Value[USB2831_MAX_DA_CHANNELS];
// 	UINT IDC_EDIT_VoltValue[USB2831_MAX_DA_CHANNELS];

// Form Data
public:
	//{{AFX_DATA(CDAView)
	enum { IDD = IDC_DA_View };
	//}}AFX_DATA

// Attributes
public:
	LRESULT OnDrawPicture(WPARAM wParam,LPARAM lParam);
	CWinThread* m_WriteDAThread0;  // 第0通道子线程句柄
	CWinThread* m_WriteDAThread1;  // 第1通道子线程句柄
	CWinThread* m_WriteDAThread2;  // 第2通道子线程句柄
	CWinThread* m_WriteDAThread3;  // 第3通道子线程句柄
	THREAD_PARA m_Para[4];			   // 第0通道子线程参数
	int m_Cycle;					//波形周期
	WORD m_Value;
	HANDLE hDevice;
	int DeviceID;
	BOOL m_bCustomSlider;
	BOOL m_IntialUpdate;
	BOOL m_bEditCycle;
	BOOL bStart;
	BOOL m_bStartThread[4];
	WORD m_WaveValue0[4096];   // 存放波形数据
	float m_PerLsbVolt;
	float m_VoltCenter;
	UINT m_iChangeId;	// 保存最近修改的控件id
// Operations
public:
	void OnCheckWave(int nNum);
	void OnEditValue(int nNum);
	void OnEditCycle(int nNum);
	void OnCustomSlider(int nChannelNum);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDAView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDAView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CDAView)
	afx_msg void OnBUTTONStartWriteDA0();
	afx_msg void OnBUTTONStartWriteDA1();
	afx_msg void OnBUTTONStopWriteDA0();
	afx_msg void OnBUTTONStopWriteDA1();
	afx_msg void OnChangeEDITCycle0();
	afx_msg void OnCHECKWave0();
	afx_msg void OnChangeEDITCycle1();
	afx_msg void OnCHECKWave1();
	afx_msg void OnPaint();
	afx_msg void OnChangeEDITCycle2();
	afx_msg void OnBUTTONStartWriteDA2();
	afx_msg void OnCHECKWave2();
	afx_msg void OnBUTTONStopWriteDA2();
	afx_msg void OnChangeEDITCycle3();
	afx_msg void OnBUTTONStartWriteDA3();
	afx_msg void OnBUTTONStopWriteDA3();
	afx_msg void OnCHECKWave3();
	afx_msg void OnDestroy();
	afx_msg void OnCustomdrawSlider0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCOMBOVoltRange0();
	afx_msg void OnChangeEDITValue0();
	afx_msg void OnChangeEDITValue1();
	afx_msg void OnChangeEDITValue2();
	afx_msg void OnChangeEDITValue3();
	afx_msg void OnKillfocusEDITValue0();
	afx_msg void OnKillfocusEDITValue1();
	afx_msg void OnKillfocusEDITValue2();
	afx_msg void OnKillfocusEDITValue3();
	afx_msg void OnSetfocusEDITValue0();
	afx_msg void OnSetfocusEDITValue1();
	afx_msg void OnSetfocusEDITValue2();
	afx_msg void OnSetfocusEDITValue3();
	afx_msg void OnSetfocusEDITCycle();
	afx_msg void OnKillfocusEDITCycle();
	afx_msg void OnChangeEDITVolt0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DAVIEW_H__CAF338D1_D883_420B_91B1_95594D6F7007__INCLUDED_)
