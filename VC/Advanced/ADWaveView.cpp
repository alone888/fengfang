// ADWaveView.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CADWaveView, CScrollView)

CADWaveView::CADWaveView()
{
	m_CurrentProgress = 0;  // 置采集进度条起值为0
}

CADWaveView::~CADWaveView()
{
}


BEGIN_MESSAGE_MAP(CADWaveView, CScrollView)
//{{AFX_MSG_MAP(CADWaveView)
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_SHOW_WAVE, ShowWave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADWaveView drawing
//BOOL bCreate = FALSE;
void CADWaveView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CADDoc* pDoc = (CADDoc*)GetDocument();
	pDoc->m_hWndWave = m_hWnd;   // 取得视图句柄，以便在DOC中的线程使用
	CSize sizeTotal;
	const SIZE conPSize = {200, 1};
	const SIZE conLSize = {20, 1};
	sizeTotal.cx = 0; 
	sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));

	// 创建绘图窗体
	CRect rect;
	GetClientRect(rect);
 	m_ADScopeCtrl.Create(WS_CHILD | WS_VISIBLE, CRect(10, 0, 400, 600), this);
	m_ADScopeCtrl.SetXUnits("X");
	m_ADScopeCtrl.SetYUnits("Y");
	m_ADScopeCtrl.SetRange(-5, 5, 1);
	m_ADScopeCtrl.InvalidateCtrl();
}

void CADWaveView::OnDraw(CDC* pDC)
{
	
}

/////////////////////////////////////////////////////////////////////////////
// CADWaveView diagnostics

#ifdef _DEBUG
void CADWaveView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CADWaveView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CADDoc* CADWaveView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADDoc)));
	return (CADDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CADWaveView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(0, 0, 0));
	return TRUE;
	// 	return CScrollView::OnEraseBkgnd(pDC);
}

void CADWaveView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_ADScopeCtrl.m_hWnd != NULL)
	{
		m_ADScopeCtrl.SetWindowPos(NULL, 0, 0, cx, cy, SWP_DRAWFRAME);
		this->OnDrawPolyLine();	
	}
}

LRESULT CADWaveView::ShowWave(WPARAM wParam, LPARAM lParam)
{
	if (gl_bProgress && gl_bDeviceADRun) // 刷新ADDigitView视图时，更新进度条的状态
	{
// 		if (!gl_bDeviceADRun) // 检查是否设备
// 		{
// 			return 0;
// 		}    
		CADFrm* pADFrm = theApp.m_ADFrm; // 取得子帧窗口句柄
		CProgressCtrl* pProgress;
		pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
		pProgress->SetPos(m_CurrentProgress);
		m_CurrentProgress++;
		if (m_CurrentProgress > 50) m_CurrentProgress = 0;
	}

	gl_bProgress = FALSE;
	OnDrawPolyLine();
	return 1;
}

void CADWaveView::OnDrawPolyLine() // 显示波形
{
	//m_ADScopeCtrl.AppendPoly(gl_BufferID, gl_Offset);
	//if (gl_ProcessMoveVolt == 0)
	//{
	//	gl_pADStatusView->AnalyzeAccurate(); // 精度分析
	//}
	//m_ADScopeCtrl.StartTimer();
}

void CADWaveView::StartScopeCtrlTimer() // 启动timer
{
	m_ADScopeCtrl.StartTimer();
}
