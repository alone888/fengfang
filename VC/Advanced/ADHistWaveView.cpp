// HistWaveView.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView
IMPLEMENT_DYNCREATE(CADHistWaveView, CScrollView)

CADHistWaveView::CADHistWaveView()
{
	Index = 0;
	m_ScreenVolume = (int)AD_VOLT_RANGE;  // 定义量程为10V	
	m_MiddleLsb = 0;   // 偏移0个LSB
	oldPoint.x = -1;
	m_bCapture = FALSE;
	m_Draw = FALSE;
	m_nOldDataPos = 0;
	m_bFirTriPos = TRUE;
}

CADHistWaveView::~CADHistWaveView()
{
}

BEGIN_MESSAGE_MAP(CADHistWaveView, CScrollView)
	//{{AFX_MSG_MAP(CADHistWaveView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView drawing

void CADHistWaveView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	int ChannelCount = 1;
	// TODO: calculate the total size of this view
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument(); 
	CADHistFrm* pHistFrm = (CADHistFrm*)GetParentFrame();
	CSize sizeTotal;
// 	m_nWidth = sizeTotal.cx = 600; // (int)(pDoc->m_ReadDataSize/ChannelCount)-1;
	sizeTotal.cx = 10; // (int)(pDoc->m_ReadDataSize/ChannelCount)-1;

	sizeTotal.cy = 10;
	SetScrollSizes(MM_TEXT, sizeTotal);
	CPoint point;
	m_bFirTriPos = TRUE;
	pDoc->m_hWndWave = m_hWnd; 
	point.x = (LONG)pDoc->m_Header.nTriggerPos;
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(pHistFrm->m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	pSliderOffset->SetPos(point.x - 100);	// 屏幕显示在触发点位置处
	oldPoint.x = point.x;


	// 创建绘图窗体
	CRect rect;
 	GetClientRect(rect);
  	m_ADHistScope.Create(WS_CHILD | WS_VISIBLE, rect, this);
 	m_ADHistScope.SetXUnits("X");
 	m_ADHistScope.SetYUnits("Y");
// 	m_ADHistScope.SetRange(-5, 5, 1);
 	m_ADHistScope.SetRange(pDoc->m_Header.VoltBottomRange, pDoc->m_Header.VoltTopRange, 1);
 	m_ADHistScope.InvalidateCtrl();


}

void CADHistWaveView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView diagnostics

#ifdef _DEBUG
void CADHistWaveView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CADHistWaveView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView message handlers

void CADHistWaveView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	GetWindowRect(&ClientRect);

	if (m_ADHistScope.m_hWnd != NULL)
	{
		m_ADHistScope.SetWindowPos(NULL, 0, 0, cx, cy, SWP_DRAWFRAME);
	}
}

void CADHistWaveView::OnDestroy() 
{
	CScrollView::OnDestroy();	
}

