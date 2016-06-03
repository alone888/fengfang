// Sys.h : main header file for the SYS application
//
#include "ADDoc.h"

#include "ADHistDoc.h"
#include "ADHistFrm.h"
#include "ADFrm.h"
#include "DADoc.h"
#include "DIODoc.h"

#if !defined(AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSysApp:
// See Sys.cpp for the implementation of this class
//
class CDAFrm;
class CCNTFrm;
class CDIOFrm;
class CSysApp : public CWinApp
{
public:
	CSysApp();

public:
	// 以下三个变量用于控制子窗体的唯一性
	BOOL m_bCreateDIOFrm;
	BOOL m_bCreateCNTFrm;
	BOOL m_bCreateDAFrm;

	BOOL m_bCreateDevice;
	HWND m_hParaCfgView;

public:
	CADFrm* m_ADFrm;	// 保存数据采集子帧窗口
	CDAFrm* m_pDAFrm;
	CCNTFrm* m_pCNTFrm;
	CDIOFrm* m_pDIOfrm;
	CADHistFrm* m_pHistDataFrm;
	CMultiDocTemplate* pADTemplate;
	CMultiDocTemplate* pDIOTemplate;
	CMultiDocTemplate* pHistDataTemplate;
	CMultiDocTemplate* pDATemplate;

	CADDoc*			m_pADDigitListDoc;
	CADDoc*			m_pADDoc;
	CADHistDoc*		m_pADHistDoc;
	CDADoc*			m_pDADoc;
	CDIODoc*		m_pDIODoc;

	//CBalloonHelp	m_BalloonHelp;
	HANDLE m_hDevice;
	HANDLE m_hMutex;
	CString m_strFilePath; // 打开异步采集文件时，保存文件路径
public:
	int m_CurrentDeviceID;  // 记录当前设备ID号
public:
	void MsgWarning(CString strTitle, CString strMsg, CPoint point, UINT lTime);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSysApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateCloseFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataFileNew(CCmdUI* pCmdUI);
	afx_msg void OnCloseFile();
	afx_msg void OnCollectData();
	afx_msg void OnDataFileNew();
	afx_msg void OnReleaseDevice();
	afx_msg void OnStartDeviceAD();
	afx_msg void OnStopDeviceAD();
	afx_msg void OnUpdateStopDeviceAD(CCmdUI* pCmdUI);
	afx_msg void OnOpenAD();
	afx_msg void OnUpdateOpenAD(CCmdUI* pCmdUI);
	afx_msg void OnOpenADHist();
	afx_msg void OnUpdateStartDeviceAD(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDigitalShow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphicShow(CCmdUI* pCmdUI);
	afx_msg void OnMDataSave();
	afx_msg void OnMDigitalShow();
	afx_msg void OnMGraphicShow();
	afx_msg void OnListDeviceDlg();
	afx_msg void OnOpenIo();
	afx_msg void OnUpdateOpenIo(CCmdUI* pCmdUI);
	afx_msg void OnOpenDA();
	afx_msg void OnUpdateOpenDA(CCmdUI* pCmdUI);
	afx_msg void OnGetDevVersion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
private:
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
