// ADStatusView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADStatusView

IMPLEMENT_DYNCREATE(CADStatusView, CFormView)

CADStatusView::CADStatusView()
	: CFormView(CADStatusView::IDD)
{
	//{{AFX_DATA_INIT(CADStatusView)
	//}}AFX_DATA_INIT

}

CADStatusView::~CADStatusView()
{
}

void CADStatusView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADStatusView)
	DDX_Control(pDX, IDC_Combo_ScreenVolume, m_Combo_ScreenVolume);
	DDX_Control(pDX, IDC_Combo_MoveVolt, m_Combo_MoveVolt);
	DDX_Control(pDX, IDC_EDIT_AnalyzeCount, m_Edit_AnalyzeCount);
	DDX_Control(pDX, IDC_EDIT_OverRange, m_Edit_OverRange);
	DDX_Control(pDX, IDC_EDIT_OverRatio, m_Edit_OverRatio);
	DDX_Control(pDX, IDC_EDIT_TriggerVolt, m_Edit_TringgerVolt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CADStatusView, CFormView)
	//{{AFX_MSG_MAP(CADStatusView)
	ON_EN_CHANGE(IDC_EDIT_TriggerVolt, OnChangeEDITTriggerVolt)
	ON_BN_CLICKED(IDM_GraphicShow, OnGraphicShow)
	ON_BN_CLICKED(IDM_DigitalShow, OnDigitalShow)
	ON_BN_CLICKED(IDM_DataSave, OnDataSave)
	ON_CBN_SELCHANGE(IDC_Combo_ScreenVolume, OnSelchangeComboScreenVolume)
	ON_BN_CLICKED(IDC_CHECK_StopFirstDigitalShow, OnCHECKStopFirstDigitalShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADStatusView diagnostics

#ifdef _DEBUG
void CADStatusView::AssertValid() const
{
	CFormView::AssertValid();
}

void CADStatusView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADStatusView message handlers

void CADStatusView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CString str;
	str.Format(_T("%d"), gl_TriggerVolt);
	m_Edit_TringgerVolt.SetWindowText(str);
	m_Combo_ScreenVolume.SetCurSel(12);
	m_Combo_MoveVolt.SetWindowText(_T("0"));
	m_Edit_AnalyzeCount.SetWindowText(_T("0"));
	m_Edit_OverRange.SetWindowText(_T("0")); // 要返回的
	m_Edit_OverRatio.SetWindowText(_T("0")); // 要返回的
	CButton* pButtonTile = (CButton*)GetDlgItem(IDM_TileWave);
	CButton* pButtonPose = (CButton*)GetDlgItem(IDM_SuperPoseWave);
	if(gl_bTileWave) // 如果是平铺显示
	{
		pButtonTile->SetCheck(1);
	}
	else
	{
		pButtonPose->SetCheck(1);
	}
// 	pButtonTile->EnableWindow(FALSE);
// 	pButtonPose->EnableWindow(FALSE);
	SetDlgCheck();
}

void CADStatusView::OnChangeEDITTriggerVolt() // 改变触发电压的值
{
	// TODO: Add your control notification handler code here
	CString strTringgerVolt;
	m_Edit_TringgerVolt.GetWindowText(strTringgerVolt);
	gl_TriggerVolt = wcstol(strTringgerVolt, NULL, 10);
	// 当重新设置触发电平时，重置超限数和统计总数
	gl_OverLimitCount = 0; 
	gl_AnalyzeAllCount = 0;	
}

void CADStatusView::SetDlgCheck()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDM_DigitalShow);
	pCheck->SetCheck(gl_nProcMode == PROC_MODE_DIGIT ? BST_CHECKED:BST_UNCHECKED);
	pCheck->EnableWindow(gl_bCreateFile);

	pCheck = (CButton*)GetDlgItem(IDM_GraphicShow);
	pCheck->SetCheck(gl_nProcMode == PROC_MODE_WAVE ? BST_CHECKED:BST_UNCHECKED);
	pCheck->EnableWindow(gl_bCreateFile);

	pCheck = (CButton*)GetDlgItem(IDM_DataSave);
	pCheck->SetCheck(gl_nProcMode == PROC_MODE_SAVE ? BST_CHECKED:BST_UNCHECKED);
	pCheck->EnableWindow(!gl_bCreateFile);
} 

void CADStatusView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(lHint==1 || lHint==2) return;
	CView::OnUpdate(pSender, lHint, pHint);
}

void CADStatusView::OnDigitalShow() 
{
	// TODO: Add your control notification handler code here
	gl_nProcMode = PROC_MODE_DIGIT;
	SetDlgCheck();
}

void CADStatusView::OnGraphicShow() 
{	
	// TODO: Add your control notification handler code here
	gl_nProcMode = PROC_MODE_WAVE;
	SetDlgCheck();
}

void CADStatusView::OnDataSave() 
{	
	gl_nProcMode = PROC_MODE_SAVE;
	SetDlgCheck();
}

void CADStatusView::OnSelchangeComboScreenVolume() 
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CADDoc* pDoc = pApp->m_pADDoc;

	CString strScreenVolume;
	m_Combo_ScreenVolume.GetWindowText(strScreenVolume);
	gl_ScreenVolume = wcstol(strScreenVolume, NULL, 10);

	//CADWaveView* pWaveView = (CADWaveView*)CView::FromHandle(pApp->m_pADDoc->m_hWndWave);
	
	
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	if (pWaveView->m_ADScopeCtrl.m_bAllChannel)
	{
		return;
	}
	for (int channal=0; channal<USB2831_MAX_AD_CHANNELS; channal++)
	{
		pWaveView->m_ADScopeCtrl.SetRange(-gl_ScreenVolume/2.0, gl_ScreenVolume/2.0, channal);
	}
}

void CADStatusView::EnableControlWindows(BOOL bFlag)
{
	CButton* pButton;
	m_Edit_TringgerVolt.EnableWindow(bFlag);
	if(gl_nProcMode == PROC_MODE_SAVE) // 如果是存盘操作
	{
		m_Combo_MoveVolt.EnableWindow(bFlag);	
		m_Combo_ScreenVolume.EnableWindow(bFlag);
		pButton = (CButton*)GetDlgItem(IDM_TileWave);
		pButton->EnableWindow(bFlag);
		pButton = (CButton*)GetDlgItem(IDM_SuperPoseWave);
		pButton->EnableWindow(bFlag);
	}
}

void CADStatusView::AnalyzeAccurate() // 精度分析
{
	CString StrAllCount;
	CString StrOverCount;
	CString StrOverRatio;
	int m_TriggerLsb; // 触发电平的码值
	int m_TemporyLsb; // 电压跳动的码值
	m_TriggerLsb = (int)(gl_TriggerVolt*(AD_LSB_COUNT/AD_VOLT_RANGE)); // 上下跳动的值(电压值--->码值)
	for(ULONG Index=0; Index<gl_ReadSizeWords; Index++)
	{
		m_TemporyLsb = (int)(((ADBuffer[gl_nDrawIndex][Index])&MASK_MSB - (gl_MiddleLsb[0] + AD_LSB_HALF)));
		if(abs(m_TemporyLsb>m_TriggerLsb))
		{
			gl_OverLimitCount++;
		}
	}

	gl_AnalyzeAllCount += gl_ReadSizeWords;
	StrAllCount.Format(_T("%3.2e"), gl_AnalyzeAllCount);
	StrOverCount.Format(_T("%d"), gl_OverLimitCount);
	StrOverRatio.Format(_T("%4.4f%%"), (float)(gl_OverLimitCount*100.0/gl_AnalyzeAllCount));
	m_Edit_AnalyzeCount.SetWindowText(StrAllCount); // 分析总数
	m_Edit_OverRange.SetWindowText(StrOverCount);   // 超限次数
	m_Edit_OverRatio.SetWindowText(StrOverRatio);   // 超限比
}

void CADStatusView::OnCHECKStopFirstDigitalShow() 
{
	// TODO: Add your control notification handler code here
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHECK_StopFirstDigitalShow);
	gl_FirstScreenStop = pCheck->GetCheck();
}
