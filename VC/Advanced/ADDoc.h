// ADDoc.h : interface of the CADDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWDATADOC_H__D485DF05_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_SHOWDATADOC_H__D485DF05_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ADParaCfgView.h"

class CADDoc : public CDocument
{
protected: // create from serialization only
	CADDoc();
	DECLARE_DYNCREATE(CADDoc)

// Attributes
public:
	HANDLE	m_hDevice;			// �豸���
	HWND	m_hWndDigit;		// ���ִ��ھ��
	HWND	m_hWndWave;			// ���δ��ھ��
	CADParaCfgView* m_hADParaCfgView;
	HWND	m_hWndADSave;		// ���δ��ھ��
	HWND	m_hWndADCfg;		// �ұߵ����������
	FILE_HEADER m_header;		// �ļ�ͷ��Ϣ
	ULONG	m_RemainMB;
	ULONG	m_WroteMB;
	CRect	m_RectBalloon;
	CFile	m_File;

public:
	void OnDataFileNew();
	void OnCloseFile();
	void Public_ReleaseDevice(void);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	CWinThread* m_hReadThread;  // ���߳̾��
	CWinThread* m_hProcThread;  // ���߳̾��
	int m_nADDigitWidth;

	virtual ~CADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	void StartDeviceAD();
	void StopDeviceAD();

// Generated message map functions
public:
	//{{AFX_MSG(CADDoc)
	afx_msg void OnTileWave();
	afx_msg void OnUpdateTileWave(CCmdUI* pCmdUI);
	afx_msg void OnSuperPoseWave();
	afx_msg void OnUpdateSuperPoseWave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWDATADOC_H__D485DF05_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
