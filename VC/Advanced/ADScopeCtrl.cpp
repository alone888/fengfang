//CADScopeCtrl.cpp : implementation file //

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERID 8
#define TIMERID2 9
///////////////////////////////////////////////////////////////////////////
//CADScopeCtrl

CTime g_InitTime;  
CADScopeCtrl::CADScopeCtrl()
{
	m_dPreviousPosition =   0.0;	
	m_nYDecimals = 3;
	for (int Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++)
	{
		m_dLowerLimit[Channel] = -1000.0;
		m_dUpperLimit[Channel] =  1000.0;
	}
	
	m_dRange      =  (double)(m_dUpperLimit - m_dLowerLimit);    
	m_nShiftPixels     = 4;
	m_nHalfShiftPixels = m_nShiftPixels/2;                      
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels;   
	m_nChannelNum = 0;
	
	m_crBackColor  = RGB(0,  0,   0);  
	m_crGridColor  = RGB(255, 255, 255);
	m_crGridGreyColor  = RGB(192, 192, 192);
	m_crGridGreyColor2 = RGB(100, 100, 100);

	m_crPlotColor  = RGB(255, 128, 0); 
	m_clPen[0] = RGB(255, 0, 0);
	m_clPen[1] = RGB(255, 255, 0);
	m_clPen[2] = RGB(0, 255, 0);
	m_clPen[3] = RGB(255, 0, 255);
	m_clPen[4] = RGB(255, 0, 0);
	m_clPen[5] = RGB(255, 255, 0);
	m_clPen[6] = RGB(0, 255, 0);
	m_clPen[7] = RGB(255, 0, 255);
	m_clPen[8] = RGB(255, 0, 0);
	m_clPen[9] = RGB(255, 255, 0);
	m_clPen[10] = RGB(0, 255, 0);
	m_clPen[11] = RGB(255, 0, 255);
	m_clPen[12] = RGB(255, 0, 0);
	m_clPen[13] = RGB(255, 255, 0);
	m_clPen[14] = RGB(0, 255, 0);
	m_clPen[15] = RGB(255, 0, 255);
	m_clPen[16] = RGB(255, 0, 0);
	m_clPen[17] = RGB(255, 255, 0);
	m_clPen[18] = RGB(0, 255, 0);
	m_clPen[19] = RGB(255, 0, 255);
	m_clPen[20] = RGB(255, 0, 0);
	m_clPen[21] = RGB(255, 255, 0);
	m_clPen[22] = RGB(0, 255, 0);
	m_clPen[23] = RGB(255, 0, 255);
	m_clPen[24] = RGB(255, 0, 0);
	m_clPen[25] = RGB(255, 255, 0);
	m_clPen[26] = RGB(0, 255, 0);
	m_clPen[27] = RGB(255, 0, 255);
	m_clPen[28] = RGB(255, 0, 0);
	m_clPen[29] = RGB(255, 255, 0);
	m_clPen[30] = RGB(0, 255, 0);
	m_clPen[31] = RGB(255, 0, 255);
	
	m_crLineAColor = RGB(0, 0, 0);  // A线的颜色
    m_crLineBColor = RGB(0, 0, 0);  // B线的颜色
    m_crLineVColor = RGB(0, 0, 0);  // C线的颜色
	
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor);
	m_brushBack.CreateSolidBrush(m_crBackColor);

	m_brushWhite.CreateSolidBrush(RGB(255,255,255));
	
	m_strXUnitsString.Format(_T("Samples"));  
	m_strYUnitsString.Format(_T("Y units"));  
	
	m_pbitmapOldGrid = NULL;
	m_pbitmapOldPlot = NULL;
 	m_nChannelCount = gl_nChannelCount;
	m_bDrawPoly = FALSE; 
	m_bAllChannel = TRUE;
	m_bInitialed = FALSE;
	m_bLBtnDown  = FALSE;
	m_bOnlyLine  = FALSE;
	m_nPtCount = 0; 
	m_OldMoveX1 = m_CurMoveX1 = 0;
	m_OldMoveX2 = m_CurMoveX2 = 0;
	m_CurMoveY = 0;
	m_CurVoltageY = 0;
	m_OldVoltageY = 0;
	m_nLineIndex = 0xFFFF;
	m_bRBtnDown = FALSE;
	m_BufferID = 0;
	m_Offset = 0;
	m_hCursorHSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorVSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
	m_hCursorArrow = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorAllSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	m_nShowCursor = 0; // 显示光标
	

} // CADScopeCtrl

///////////////////////////////////////////////////////////////////////////

CADScopeCtrl::~CADScopeCtrl()
{
	if (m_pbitmapOldGrid != NULL)
		m_dcGrid.SelectObject(m_pbitmapOldGrid);  
	if (m_pbitmapOldPlot != NULL)
		m_dcPlot.SelectObject(m_pbitmapOldPlot);	
}

BEGIN_MESSAGE_MAP(CADScopeCtrl, CWnd)
//{{AFX_MSG_MAP(CADScopeCtrl)
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_SETCURSOR()
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////
//ADScopeCtrl message handlers
///////////////////////////////////////////////////////////////////////////
BOOL CADScopeCtrl::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, HMENU nID) 
{
	BOOL result;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
	
	result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
		className, NULL, dwStyle, 
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, 
		pParentWnd->GetSafeHwnd(), (HMENU)nID);
	StartTimer();

	//CButton *m_btnadd = new CButton;
	//m_btnadd->Create(_T("Add"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(20,20,40,80),
	//	this,1);
	//m_btnadd->ShowWindow(SW_SHOW);

	return result;
	
} // Create

///////////////////////////////////////////////////////////////////////////
//设置垂直方向上下量程的大小
void CADScopeCtrl::SetRange(double dLower, double dUpper, int nChannel)
{
	ASSERT(dUpper > dLower);
	m_dLowerLimit[nChannel]  = dLower;
	m_dUpperLimit[nChannel]  = dUpper;
	m_nYDecimals = 2;    //  小数点位数
	m_dRange = (double)(m_dUpperLimit - m_dLowerLimit);
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange;
	InvalidateCtrl();
} // SetRange

///////////////////////////////////////////////////////////////////////////
//启动timer
void CADScopeCtrl::StartTimer()
{
	SetTimer(TIMERID,50,0);  //这里就相当于设定了timer,如果要停掉timer就是KillTimer(TIMERID)
	SetTimer(TIMERID2,100,0);//模拟假数据用
}  

static int Drow_text_find_id(int cur_id)
{
	int find_cnt = 0;
	int i;
	for(i = 0; i < 8; i++)
	{
		if(gl_signal_enable[i] == 1)
			find_cnt++;
		if(find_cnt ==cur_id)
			break;
	}
	return i+1;
}

void test_data()
{
	static WORD data[8][30000] = {0};
	static int last_data_id = 0;
	static unsigned int time=0;
	
	if(data[0][0] == 0)
	{
		for(int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 30000; j++)
			{
				data[i][j] = j%8192;
			}
		}
	}
	data[0][0] = 1;

	for (int ch = 0; ch < 8; ch++)
	{	
		for (int i = 0; i < 100 ; i++)
		{
			gt_AD_OrgData[i].data[ch] = data[ch][last_data_id+i];
		}
	}
	last_data_id+=100;
	if(last_data_id >= 30000 - 1)
		last_data_id = 0;
	

	unsigned int tmp = 8000000/ADPara.Frequency;
	for (int i = 0; i < 100 ; i++)
	{
		if(time > g_nTimeAxisRange) time = 0;
		gt_AD_OrgData[i].time = time;
		time += tmp;
	}
}

void CADScopeCtrl::OnTimer(UINT_PTR nIDEvent)

{
	CString str;
	static int i=0;
	if(nIDEvent == TIMERID)
	{
		DrawBkGnd();  // 画背景
		TransitionData();
		ProcessData();
		DrawPoly(); // 画线
	}
	else if(nIDEvent == TIMERID2)
	{
		//test_data();
		//ProcessOrgAdData();
		//i++;

	}

	CTime m_time,time2;  
	m_time=CTime::GetCurrentTime();             //获取当前时间日期  
	CTimeSpan timeSpan = m_time - g_InitTime;
	CString m_strDate=m_time.Format(_T("%x"));          //格式化日期  
	CString m_strTime=timeSpan.Format(_T("%H:%M:%S"));          //格式化时间 
	CString m_strDateTime=m_time.Format(_T("%Y-%m-%d   %H:%M:%S   %A"));   //格式化日期时间  
	
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CADFrm* pADFrm = (CADFrm*)pApp->m_ADFrm;
	CEdit *pEditFre = (CEdit *)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
	pEditFre->SetWindowText(m_strDateTime+"   "+ m_strTime);
}
///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetXUnits(CString string)
{
	m_strXUnitsString = string;
	
	InvalidateCtrl();
	
}  // SetXUnits

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetYUnits(CString string)
{
	m_strYUnitsString = string;
	
	InvalidateCtrl();
	
}  // SetYUnits

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetGridColor(COLORREF color)
{
	m_crGridColor = color;
	
	InvalidateCtrl();
	
}  // SetGridColor


///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color;
	
	m_penPlot.DeleteObject();
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor);
	
	InvalidateCtrl();
	
}  // SetPlotColor


///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetBackgroundColor(COLORREF color)
{
	m_crBackColor = color;
	
	m_brushBack.DeleteObject();
	m_brushBack.CreateSolidBrush(m_crBackColor);

	m_brushWhite.DeleteObject();
	m_brushWhite.CreateSolidBrush(RGB(255,255,255));
	
	InvalidateCtrl();
	
}  // SetBackgroundColor

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::InvalidateCtrl()
{
	CClientDC dc(this);
	if (m_dcGrid.GetSafeHdc() == NULL)
	{
		m_dcGrid.CreateCompatibleDC(&dc);	
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
		m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid);
	}

	if (m_dcPlot.GetSafeHdc() == NULL)
	{
		m_dcPlot.CreateCompatibleDC(&dc);
		m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
		m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot);
	}

	if (m_PenLineA.m_hObject == NULL)
		m_PenLineA.CreatePen(PS_DASHDOT, 1, m_crLineAColor); // 创建A线的画笔
	if (m_PenLineB.m_hObject == NULL)
		m_PenLineB.CreatePen(PS_DASHDOT, 1, m_crLineBColor); // 创建B线的画笔
	if (m_PenLineV.m_hObject == NULL)
		m_PenLineV.CreatePen(PS_DASHDOT, 1, m_crLineVColor); // 创建V线的画笔

	int Channel = 0;
	for (Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++)
	{
		if (m_penChannel[Channel].m_hObject == NULL)
			m_penChannel[Channel].CreatePen(PS_SOLID, 1, RGB(0,0,255));
	}
	

		//gl_nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1;
		m_nChannelCount = gl_nChannelCount;

	//ProcessData(); // 处理数据
	//DrawBkGnd();  // 画背景
	//TransitionData();	// 将原码转化为屏幕绘图Y坐标
	//DrawPoly();
	Invalidate();
	m_bInitialed = TRUE;
} // InvalidateCtrl

///////////////////////////////////////////////////////////////////////////
double CADScopeCtrl::AppendPoint(double dNewPoint)
{
	
	double dPrevious;
	
	dPrevious = m_dCurrentPosition;
	m_dCurrentPosition = dNewPoint;
	DrawPoint();
	
	Invalidate();
	
	return dPrevious;
	
} // AppendPoint

//////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::OnPaint() 
{
	// 这里的m_nClientWidth, m_ClientHeight也是取决于m_rectClient的大小
	CPaintDC dc(this);  // device context for painting
	CDC memDC;
	CBitmap memBitmap;
	CBitmap* oldBitmap; // bitmap originally found in CMemDC
	memDC.CreateCompatibleDC(&dc);
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);
	CString str;
	
	if (memDC.GetSafeHdc() != NULL)
	{
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcGrid, 0, 0, SRCCOPY);
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcPlot, 0, 0, SRCPAINT);  //SRCPAINT
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcMove, 0, 0, SRCPAINT);  //SRCPAINT		
		dc.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&memDC, 0, 0, SRCCOPY);       //SRCCOPY);
		
	}
	this->UpdateChannelCount();

	memDC.SelectObject(oldBitmap);
	
	int	nDrawMode;
	dc.SetBkMode(TRANSPARENT);		                // 设置背景为透明
	nDrawMode = dc.SetROP2(R2_XORPEN);              // 设置绘制模式
	
	dc.SelectObject(&m_PenLineA);                   // 选入线A的画笔
	dc.MoveTo(m_OldMoveX1, m_rectPlot.top);
	dc.LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // 画线A	
	
	dc.SelectObject(&m_PenLineB);				    // 选区入线B的画笔
	dc.MoveTo(m_OldMoveX2, m_rectPlot.top);
	dc.LineTo(m_OldMoveX2, m_rectPlot.Height()+10); // 画线B	
	if (!m_bAllChannel)
	{
		dc.SelectObject(&m_PenLineV);				// 选区入线V的画笔
		dc.MoveTo(m_rectPlot.left, m_OldVoltageY);
		dc.LineTo(m_rectPlot.right, m_OldVoltageY);	// 画线V
	}
	dc.SetROP2(nDrawMode);


} // OnPaint

//#####################################################################
//画滚动的点(位图的移动产生动态)
void CADScopeCtrl::DrawPoint()
{
	int currX, prevX, currY, prevY;
	
	CPen* oldPen;
	CRect rectCleanUp;
	
	if (m_dcPlot.GetSafeHdc() != NULL)
	{
		m_dcPlot.BitBlt(m_rectPlot.left, m_rectPlot.top+1, 
			m_nPlotWidth, m_nPlotHeight, &m_dcPlot, 
			m_rectPlot.left+1/*2/*m_nShiftPixels*/, m_rectPlot.top+1, 
			SRCCOPY);		
		
		rectCleanUp = m_rectPlot;
		rectCleanUp.left  = rectCleanUp.right - 1;
		
		m_dcPlot.FillRect(rectCleanUp, &m_brushBack);
		
		oldPen = m_dcPlot.SelectObject(&m_penPlot);
		
		prevX = m_rectPlot.right-2; // m_nPlotShiftPixels;
		prevY = m_rectPlot.bottom - (long)m_dPreviousPosition-10;
		m_dcPlot.MoveTo (prevX, prevY);
		
		currX = m_rectPlot.right-1; // m_nHalfShiftPixels;
		currY = m_rectPlot.bottom -(long)m_dCurrentPosition-10;
		m_dcPlot.LineTo (currX, currY);
		
		m_dcPlot.SelectObject(oldPen);
		
		m_dPreviousPosition = m_dCurrentPosition;		
	}
	
} // end DrawPoint

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bInitialed) 
	{ 
		GetClientRect(m_rectClient); // 获取当前的客户区大小
		
		m_nClientHeight = m_rectClient.Height(); 
		m_nClientWidth  = m_rectClient.Width();
		
		m_rectPlot.left   = 60;  
		m_rectPlot.top    = 10;
		m_rectPlot.right  = m_rectClient.right -10;
		m_rectPlot.bottom = m_rectClient.bottom - 30; //-25;
		
		m_nPlotHeight = m_rectPlot.Height();
		m_nPlotWidth  = m_rectPlot.Width();	
		
		m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 	
		InvalidateCtrl(); // 重新绘制
		
		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;
		m_OldMoveX1   = m_CurMoveX1 = m_rectPlot.left + 20;
		m_OldMoveX2   = m_CurMoveX2 = m_rectPlot.right - 20;
		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;		
	}
} // OnSize

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::Reset()
{
	InvalidateCtrl();
}

///////////////////////////////////////////////////////////////////////////
int CADScopeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{ 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rectClient = CRect(0, 0, 1920, 1080);   
	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth  = m_rectClient.Width();
	
	m_rectPlot.left   = 20;  
	m_rectPlot.top    = 10;
	m_rectPlot.right  = m_rectClient.right-10;
	m_rectPlot.bottom = m_rectClient.bottom-25;
	
	m_nPlotHeight = m_rectPlot.Height();
	m_nPlotWidth  = m_rectPlot.Width();
	
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 
	for (int Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++)
	{
		switch(gl_InputRange[Channel].nRangeIndex)
		{		
		case USB2831_INPUT_N10000_P10000mV: // ±10000mV
			m_dUpperLimit[Channel] = 10000.00; //??* 上下交换
			m_dLowerLimit[Channel] = -10000.00;
			break;
		case USB2831_INPUT_N5000_P5000mV: // ±5000mV
			m_dUpperLimit[Channel] = 5000.00;
			m_dLowerLimit[Channel] = -5000.00;
			break;
		case USB2831_INPUT_N2500_P2500mV: // ±2500mV
			m_dUpperLimit[Channel] = 2500.00;
			m_dLowerLimit[Channel] = -2500.00;
			break;
		case USB2831_INPUT_0_P10000mV: // 0～10000mV
			m_dUpperLimit[Channel] = 10000.00;
			m_dLowerLimit[Channel] = 0.00;
			break;
		default:
			ASSERT(FALSE);
		}	
	}
	
	g_InitTime=CTime::GetCurrentTime();             //获取当前时间日期  
	//SetBackgroundColor(RGB(255,  255,   255));
	SetGridColor(RGB(0,  0,   0));//设置框架颜色
	//SetPlotColor(RGB(0,  255,   255));
	return 0;
}

//########################################################################
//画背景（网格和外框和竖线）
void CADScopeCtrl::DrawBkGnd()
{
	int nCharacters = 0;
	CPen* oldPen;
	CPen solidPen(PS_SOLID, 0, m_crGridColor);
	CFont axisFont, yUnitFont,xUnitFont;
	CFont* oldFont;
	CString strTemp;
	//---------------------------------------------------------------------------
	m_dcGrid.SetBkColor (RGB(255,255,255)/*m_crBackColor*/);	
	// fill the grid background
	m_dcGrid.FillRect(m_rectClient, &m_brushWhite/*m_brushBack*/);	
	// 计算量程字串所占用的字符宽度
	nCharacters = abs((int)log10(fabs(m_dUpperLimit[0])));
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit[0]))));
	nCharacters = nCharacters + 4 + m_nYDecimals;  
	
	PerY = ((float)m_nPlotHeight / m_nChannelCount); // 每通道的Y宽度 113
	//-----------------	-----------------------------------------------------------------
	// 画四周的框架	
	oldPen = m_dcGrid.SelectObject (&solidPen);  // 画四周的方框
	m_dcGrid.MoveTo (m_rectPlot.left-80, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-80, m_rectPlot.bottom+1);

	m_dcGrid.MoveTo (m_rectPlot.left-1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.SelectObject (oldPen);
	//COLORREF m_Grid = RGB(200, 200, 200);

	
COLORREF m_Grid = RGB(192, 192, 192);//
CPen solidPen2(PS_SOLID, 0, m_Grid);
oldPen = m_dcGrid.SelectObject (&solidPen2);  //垂直的线

	int HLine = 0, VLine = 0;
	int VGridNum = 10; 
	for (int i=1; i<VGridNum; i++) // 画20条垂直的线
	{
		VLine = m_rectPlot.Width()*i/VGridNum;//横坐标

		m_dcGrid.MoveTo (m_rectPlot.left+VLine, m_rectPlot.top);
		m_dcGrid.LineTo(m_rectPlot.left+VLine,m_rectPlot.top + m_rectPlot.Height());
	}
m_dcGrid.SelectObject (oldPen);


//画20个刻度，很短的一条竖线
m_Grid = RGB(0, 0, 0);//
CPen solidPen3(PS_SOLID, 0, m_Grid);
oldPen = m_dcGrid.SelectObject (&solidPen3);  //垂直的线

	for (int i=1; i<VGridNum; i++) // 相隔50个像素画一条垂直的线
	{
		VLine = m_rectPlot.Width()*i/VGridNum;

		m_dcGrid.MoveTo (m_rectPlot.left+VLine, m_rectPlot.top + m_rectPlot.Height()+1);
		m_dcGrid.LineTo(m_rectPlot.left+VLine,m_rectPlot.top + m_rectPlot.Height()+5);

		if (i<=2)
		{
			m_dcGrid.MoveTo (m_rectPlot.left+VLine, m_rectPlot.top + m_rectPlot.Height()+6);
			m_dcGrid.LineTo(m_rectPlot.left+VLine,m_rectPlot.top + m_rectPlot.Height()+15);
		}
	}

//画下边表示一横格的刻度的刻度线和刻度文字
	
	m_dcGrid.SetTextAlign (TA_CENTER|TA_TOP); //刻度的文字要设置成居中对齐 对齐方式

	VLine = m_rectPlot.Width()*1/VGridNum;
	m_dcGrid.MoveTo (m_rectPlot.left+VLine, m_rectPlot.top + m_rectPlot.Height()+10);
	m_dcGrid.LineTo(m_rectPlot.left+2*VLine,m_rectPlot.top + m_rectPlot.Height()+10);


	VLine = m_rectPlot.Width()*1.5/VGridNum;

	int tempRange = g_nTimeAxisRange/1000;// 转换成ms
	CString str;
	if (tempRange>=10000)
	{
		str.Format (_T("%d s"),tempRange/10000); //时间量程s
		m_dcGrid.TextOutW(m_rectPlot.left+VLine,m_rectPlot.top + m_rectPlot.Height()+4,str);
	} 
	else
	{
		str.Format (_T("%d ms"),tempRange/10); // 时间量程ms	
		m_dcGrid.TextOutW(m_rectPlot.left+VLine,m_rectPlot.top + m_rectPlot.Height()+4,str);
	}
	
	


m_dcGrid.SelectObject (oldPen);

	//----------------------------------------------------------------------------
	// 画当前状态每个刻度格表示的时间字符

	//VLine = m_rectPlot.Width()*1/VGridNum;
	//CString str;
	//str.Format(_T("CH %d ms"), g_nTimeAxisRange/10);
	//m_dcGrid.TextOut(m_rectPlot.left + VLine, (int)(m_rectPlot.top+m_rectPlot.Height()+12), str); 

	//----------------------------------------------------------------------------
	// 画每个通道的分界线(水平线)

	if (m_bAllChannel) // 显示所有通道
	{
		if (gl_bTileWave) // 如果是平铺显示
		{
			DrawAllChannelGrid(&m_dcGrid); // 画所有通道的横向网格
		}
		else
		{
			DrawSingleCHGrid(&m_dcGrid);
		}
	}
	else // 画单通道的横向网格线
	{
		DrawSingleCHGrid(&m_dcGrid);
	} 
	//--------------------------------------------------------------------------------
	axisFont.CreateFont (14, 0, 0, 0, 300, 
		FALSE, FALSE, 0, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY,  
		DEFAULT_PITCH|FF_SWISS, _T("Arial"));

	yUnitFont.CreateFont (14, 0, 900, 0, 300,  
		FALSE, FALSE, 0, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, _T("Arial"));
	oldFont = m_dcGrid.SelectObject(&axisFont);
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP); // 对齐方式
	
	//----------------------------------------------------------------------
	if (m_bAllChannel)
	{
		if (gl_bTileWave) // 平铺显示
		{
			DrawAllChannelText(&m_dcGrid);
		}
		else // 叠加显示
		{
			DrawSingleCHText(&m_dcGrid, 0);
		}
	}
	else
	{
		DrawSingleCHText(&m_dcGrid, m_nChannelNum);
	}
// 	InvalidateRect(m_rectClient); // 刷新区域
}

//***********************************
//画所有通道的横向网格
//***********************************
void CADScopeCtrl::DrawAllChannelGrid(CDC* pDC)
{
	int X = 0, Channel = 0;
	int nGridPix = 0; // Y方向的网格线的刻度
	float hight = ((float)m_rectPlot.Height() / m_nChannelCount); // 每通道的Y宽度

COLORREF GridColor = RGB(192, 192, 192);//
CPen* oldPen;
GridColor = RGB(0, 0, 0);//
CPen solidPen4(PS_SOLID, 0, GridColor);
oldPen = m_dcGrid.SelectObject (&solidPen4);  //垂直的线


	for (Channel=1; Channel<m_nChannelCount; Channel++) // 画m_nChannelCount-1条通道间分隔线
	{
		nGridPix = m_rectPlot.top + (int)(m_rectPlot.Height() * Channel) / m_nChannelCount;

		m_dcGrid.MoveTo (m_rectPlot.left-80, nGridPix);//分隔线向左延伸到最左边
		m_dcGrid.LineTo(m_rectPlot.right,nGridPix);

	}
m_dcGrid.SelectObject (oldPen);

GridColor = RGB(192, 192, 192);//
CPen solidPen5(PS_SOLID, 0, GridColor);
oldPen = m_dcGrid.SelectObject (&solidPen5);  //垂直的线
	for (Channel=0; Channel<m_nChannelCount; Channel++) // 画每通道的中线
	{
		m_dcGrid.MoveTo (m_rectPlot.left+1, (int)(m_rectPlot.top + (hight * Channel) + hight/2.0));
		m_dcGrid.LineTo(m_rectPlot.right,(int)(m_rectPlot.top + (hight * Channel) + hight/2.0));

		m_dcGrid.MoveTo (m_rectPlot.left+1, (int)(m_rectPlot.top + (hight * Channel) + hight/4.0));
		m_dcGrid.LineTo(m_rectPlot.right,(int)(m_rectPlot.top + (hight * Channel) + hight/4.0));

		m_dcGrid.MoveTo (m_rectPlot.left+1, (int)(m_rectPlot.top + (hight * Channel) + hight*3/4.0));
		m_dcGrid.LineTo(m_rectPlot.right,(int)(m_rectPlot.top + (hight * Channel) + hight*3/4.0));

	}
m_dcGrid.SelectObject (oldPen);

}

void CADScopeCtrl::DrawSingleCHGrid(CDC* pDC)
{
	//for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=2) // 每隔4个像素画1点
	//{
	//	pDC->SetPixel(X, m_rectPlot.top+(int)(m_rectPlot.Height()/2), RGB(0, 0, 0));	
	//}
}

//////////////////////////////////////////////////////////////////////////
//画PolyLine线
void CADScopeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	if (m_rectPlot.PtInRect(point))
	{
		if (!m_bRBtnDown) m_nShowCursor = 0;
		// 通过鼠标的位置判断是哪一条线获得焦点
		if (abs(point.x - m_OldMoveX1) < 5) 
		{
			m_nShowCursor = 1;
		}
		if (abs(point.x - m_OldMoveX2) < 5) 
		{
			m_nShowCursor = 2;
		}	
		if (!m_bAllChannel && abs(point.y - m_OldVoltageY) < 5) 
		{
			m_nShowCursor = 3;
		}
		if (m_bLBtnDown || m_bRBtnDown) 
		{
			DrawMoveLine(&dc, point);
		}
	}
	m_MouseOffset = point.x;	
	SetStatusBar(); // 设置状态栏的频率等
	CWnd::OnMouseMove(nFlags, point);
}

void CADScopeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CString str;
	if (gl_bTileWave && m_rectPlot.PtInRect(point)) // 当鼠标点在边框内时
	{
		// 通过判断鼠标双击时的位置，来决定显示哪一路单通道
		if (m_bAllChannel) 
		{
			//int TChannel = 0;
			Center = (PerY/2.0)+m_rectPlot.top;
			for (int Channel=0; Channel<m_nChannelCount; Channel++) //判断鼠标双击位置
			{
				HeightMid[Channel] = Center; // 保存通道中间位置坐标
				if (abs(point.y - HeightMid[Channel]) < m_rectPlot.Height()/(2*m_nChannelCount))
				{
					m_nChannelNum = Channel+ADPara.FirstChannel; // 确定双击后的单通道号
					str.Format(_T("%d"), Channel);
					SetYUnits(_T("单通道-")+str);
					break;
				}
				Center += PerY;
			}
			m_dLowerLimit[m_nChannelNum] = -gl_ScreenVolume/2.0;
			m_dUpperLimit[m_nChannelNum] = gl_ScreenVolume/2.0;
		}
		m_bAllChannel = !m_bAllChannel; 
		InvalidateCtrl();
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CADScopeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	if (m_rectPlot.PtInRect(point))
	{
		// 通过鼠标的位置判断是哪一条线获得焦点
		if (abs(point.x - m_OldMoveX1) < 5) 
		{
			m_nLineIndex = 1;
			m_bOnlyLine = TRUE;
		}

		if (abs(point.x - m_OldMoveX2) < 5) 
		{
			m_nLineIndex = 2;
			m_bOnlyLine = TRUE;
		}	

		if (!m_bAllChannel && abs(point.y - m_OldVoltageY) < 5) 
		{
			m_nLineIndex = 3;
			m_bOnlyLine = TRUE;
		}
	}

	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

//#####################################################################################
//用原码值来做(buffer里面存放原码), 再处理成可以显示的点数组
void CADScopeCtrl::AppendPoly(int BufferID, int  Offset)
{
	//m_BufferID = BufferID; // 段缓冲ID
	//m_Offset = Offset;     // 段内偏移
	//TransitionData();	// 将原码转化为屏幕绘图Y坐标
	//ProcessData(); // 处理数据
	//DrawBkGnd();  // 画背景
	//DrawPoly(); // 画线


	TransitionData();

	DrawBkGnd();  // 画背景
	ProcessData();
	DrawPoly(); // 画线
}

//###############################################################################
//处理数据, 把缓存中的数据转换成可以显示的点坐标
void CADScopeCtrl::TransitionData()
{
	//float LsbOfPixel, LsbOfPixelOne;
	//float fScreenVolume = AD_VOLT_RANGE; // 多通道显示时，屏幕是量程是和最大电压值一致的
	//LsbOfPixel = (float)(((gl_voltVolume/gl_voltVolume)*AD_LSB_COUNT)/(PerY )); // 每像素对应的码值    
	//LsbOfPixelOne = (float)(((gl_ScreenVolume*1.0/(gl_voltVolume))*AD_LSB_COUNT)/(m_rectPlot.Height())); // 每像素对应的码值
	//for (int Index=0; Index < (AD_LSB_HALF * 2); Index++) // 将原码转化为屏幕绘图Y坐标
	//{
	//	m_nCoordinateY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixel);
	//	m_nCoordinateOneY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixelOne);
	//}
	float LsbOfPixel, LsbOfPixelOne;
	float fScreenVolume = AD_VOLT_RANGE; // 多通道显示时，屏幕是量程是和最大电压值一致的
	LsbOfPixel = (float)(((AD_LSB_COUNT*g_nVAxisRange/AD_VOLT_RANGE)/(PerY))); // 每像素对应的码值    
	LsbOfPixelOne = (float)(((gl_ScreenVolume*(g_nVAxisRange/AD_VOLT_RANGE)/(AD_VOLT_RANGE*2))*AD_LSB_COUNT)/(m_rectPlot.Height())); // 每像素对应的码值

	for (int Index=0; Index < (AD_LSB_HALF * 2); Index++) // 将原码转化为屏幕绘图Y坐标
	{
		if(Index < AD_LSB_HALF+AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE &&
			Index > AD_LSB_HALF-AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE)
		{
			m_nCoordinateY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixel);
			m_nCoordinateOneY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixelOne);
		}
		else if (Index >= AD_LSB_HALF+AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE)
		{
			m_nCoordinateY[Index] =(int)((AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE)/ LsbOfPixel);
			m_nCoordinateOneY[Index] = (int)((AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE) / LsbOfPixelOne);
		}
		else if(Index <= AD_LSB_HALF-AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE)
		{
			m_nCoordinateY[Index] =0-(int)((AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE)/ LsbOfPixel);
			m_nCoordinateOneY[Index] = 0-(int)((AD_LSB_HALF*g_nVAxisRange/AD_VOLT_RANGE) / LsbOfPixelOne);
		}
	}
}

//处理数据, 把缓存中的数据转换成可以显示的点坐标
void CADScopeCtrl::ProcessData()
{
	UpdateChannelCount();
	Center = (PerY/2.0)+m_rectPlot.top;
	int Channel, Index, StartX;
	PWORD  ptOffset; // 缓存指针
	int Offset = 0, DataY = 0;
	int channe_id_enable = 0;
	StartX = m_rectPlot.left+1; // X方向的起始位置
	
	if (gl_bTileWave) // 多通道平铺显示
	{
		for (Channel=0; Channel<m_nChannelCount; Channel++)
		{
			//每个像素对应的showdata中的点数
			//double point_cnt_per_pix = 10000/(m_rectPlot.Width()-2);
			channe_id_enable = Drow_text_find_id(Channel+1);//判断当前通道是否需要显示该通道
			for (Index=0; Index<=m_nPlotWidth; Index++) // 初始化屏幕上画图宽度的点(创建时，位图的大小) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				pointxy[Channel][Index].y = Center - m_nCoordinateY[showData[channe_id_enable-1][Index*SHOW_DATA_CNT/m_nPlotWidth]&MASK_MSB];
				//pointxy[Channel][Index].y = (int)(Center) - m_nCoordinateY[ptOffset[(Offset+Index) * m_nChannelCount + Channel]&MASK_MSB];
			}
			HeightMid[Channel] = Center; // 保存通道中间位置坐标
			Center += PerY;
		}
	}
	else // 多通道叠加显示 先不考虑这种方式
	{
		//ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
		float LsbOfPixel = (float)(AD_LSB_COUNT/m_rectPlot.Height()); // 每像素对应的码值
		int Center = (int)(m_rectPlot.Height() / 2) + m_rectPlot.top;
		for (int Channel=0; Channel<m_nChannelCount; Channel++)
		{
			for (int Index=0; Index<=m_nPlotWidth; Index++)
			{
				pointxy[Channel][Index].x = StartX + Index;
				DataY = (int)(((ptOffset[Index*gl_nChannelCount + Channel]&MASK_MSB) - AD_LSB_HALF ) / LsbOfPixel);
				pointxy[Channel][Index].y = (int)(Center) - DataY;
			}
		}
	}
	m_bDrawPoly = TRUE; 
}


//##############################################################################
//画处理完的点
void CADScopeCtrl::DrawPoly()
{
	int DataY = 0;
	int nDrawCount = 0;
	gl_bDataProcessing = TRUE;
	CPen* oldPen;
	int channe_id_enable = 0;

	nDrawCount = m_rectPlot.Width() - 1;
	if (m_bAllChannel || !gl_bTileWave) // 所有通道显示或叠加显示时
	{
		for (int Channel = 0; Channel<m_nChannelCount; Channel++) // 画所有通道的点
		{
			//channe_id_enable = Drow_text_find_id(Channel+1);			
			oldPen = m_dcGrid.SelectObject(&m_penChannel[Channel]);
			m_dcGrid.Polyline(pointxy[Channel], nDrawCount);
		}
	}
	else // 单通道显示
	{
		m_dcGrid.SelectObject(&m_penChannel[m_nChannelNum]);
		int StartX = m_rectPlot.left;
		Center = (m_nPlotHeight / 2) + m_rectPlot.top;
		WORD* ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
		for (int Index=0; Index<nDrawCount; Index++)	
		{
			pointTemp[Index].x = StartX  + Index;
			pointTemp[Index].y = Center - m_nCoordinateOneY[(showData[Drow_text_find_id(m_nChannelNum+1)-1][Index*SHOW_DATA_CNT/m_nPlotWidth]&MASK_MSB)-gl_MiddleLsb[Drow_text_find_id(m_nChannelNum+1)-1]];
		}
		m_dcGrid.Polyline(pointTemp, nDrawCount);
	}
	gl_bDataProcessing = FALSE;
	Invalidate(FALSE);
	UpdateWindow();
}

void CADScopeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_nLineIndex = 0xFFFF;
	m_bOnlyLine = FALSE;
	m_bLBtnDown = FALSE; 
	::ReleaseCapture(); 

	CWnd::OnLButtonUp(nFlags, point);
}

//#######################################################################
void CADScopeCtrl::SetStatusBar()
{
	float fTimePixel;
	float fFrequency;
	CString strTimeDiv;
	CString strCountTimer;
	CString strFrequency;
	CString strVol;
	int SubstValue = abs(m_OldMoveX1 - m_OldMoveX2);
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CADFrm* pADFrm = (CADFrm*)pApp->m_ADFrm;
	CEdit* pEditDiv = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv); 
// 	fTimePixel = (float)(1000000.0/ADPara.Frequency); // uS
	if (ADPara.ADMode==USB2831_ADMODE_SEQUENCE)// 连续采集
	{
		fTimePixel = (float)(1000000.0/ADPara.Frequency*gl_nChannelCount); // uS
		// 计算两线之间的时间差
		if (fTimePixel < 1000)
		{
			strCountTimer.Format(_T("%7.2f uS"), fTimePixel * SubstValue);
		}
		if (fTimePixel >= 1000.0 && fTimePixel < 1000000.0)
		{
			strCountTimer.Format(_T("%7.2f mS"), fTimePixel * SubstValue/1000.0);
		}
		CEdit* pEditTime = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_CountTime);
		pEditTime->SetWindowText(strCountTimer);
		
		// 计算频率的大小
		fFrequency = (float)(1000000.0 / (fTimePixel * SubstValue));
		if (fFrequency < 1000.0)
		{
			strFrequency.Format(_T("%7.2f Hz"), fFrequency);
		}
		if (fFrequency > 1000.0 && fFrequency < 1000000.0)
		{
			strFrequency.Format(_T("%7.2f KHz"), fFrequency/1000.0);
		}
		if (fFrequency > 1000000.0)
		{
			strFrequency.Format(_T("%7.2f MHz"), fFrequency/1000000.0);
		}	
		/*		CEdit* pEditFre = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
		pEditFre->SetWindowText(strFrequency);	*/	
	}
	else
	{
		fTimePixel = float((1000000.0/ADPara.Frequency) * gl_nChannelCount * ADPara.LoopsOfGroup + 
			4 + ADPara.GroupInterval) / ADPara.LoopsOfGroup; // uS
		
		// 计算两线之间的时间差
		if (fTimePixel < 1000)
		{
			strCountTimer.Format(_T("%7.2f uS"), fTimePixel * SubstValue);
		}
		
		if (fTimePixel >= 1000.0 && fTimePixel < 1000000.0)
		{
			strCountTimer.Format(_T("%7.2f mS"), fTimePixel * SubstValue / 1000.0);
		}
		CEdit *pEditTime = (CEdit *)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_CountTime);
		pEditTime->SetWindowText(strCountTimer);
		
		// 计算频率的大小
		fFrequency = (float)(1000000.0 / (fTimePixel * SubstValue));
		if (fFrequency < 1000.0)
		{
			strFrequency.Format(_T("%7.2f Hz"), fFrequency);
		}
		
		if (fFrequency > 1000.0 && fFrequency < 1000000.0)
		{
			strFrequency.Format(_T("%7.2f KHz"), fFrequency / 1000.0);
		}
		
		if (fFrequency > 1000000.0)
		{
			strFrequency.Format(_T("%7.2f MHz"), fFrequency / 1000000.0);
		}


	}


	g_InitTime=CTime::GetCurrentTime();             //获取当前时间日期  
	//CString m_strDate=g_InitTime.Format(_T("%x"));          //格式化日期  
	//CString m_strTime=g_InitTime.Format(_T("%X"));          //格式化时间  
	//CString m_strDateTime=g_InitTime.Format(_T("%Y-%m-%d %H:%M:%S %A"));   //格式化日期时间  

	//CEdit *pEditFre = (CEdit *)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
	//pEditFre->SetWindowText(m_strDateTime);
// 	// 计算两线之间的时间差
// 	if (fTimePixel < 1000)
// 	{
// 		strCountTimer.Format("%7.2f uS", fTimePixel * SubstValue);
// 	}
// 
// 	if (fTimePixel >= 1000.0 && fTimePixel < 1000000.0)
// 	{
// 		strCountTimer.Format("%7.2f mS", fTimePixel * SubstValue/1000.0);
// 	}
// 
// 	CEdit* pEditTime = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_CountTime);
// 	pEditTime->SetWindowText(strCountTimer);
// 	
// 	// 计算频率的大小
//     fFrequency = (float)(1000000.0 / (fTimePixel * SubstValue * m_nChannelCount));
// 	if(fFrequency < 1000.0)
// 	{
// 		strFrequency.Format("%7.2f Hz", fFrequency);
// 	}
// 	if(fFrequency >= 1000.0 && fFrequency < 1000000.0)
// 	{
// 		strFrequency.Format("%7.2f KHz", fFrequency/1000.0);
// 	}
// 	if(fFrequency >= 1000000.0)
// 	{
// 		strFrequency.Format("%7.2f MHz", fFrequency/1000000.0);
// 	}
// 
// 	CEdit *pEditFre = (CEdit *)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
// 	pEditFre->SetWindowText(strFrequency);

	// 测量电压值
	float LsbOfPixel = (float)((gl_ScreenVolume*1.0)/(m_rectPlot.Height())); // 每像素对应的码值
	int Center = m_rectPlot.Height()/2 + m_rectPlot.top;
	int nOffset = Center - m_OldVoltageY;
	m_VolOffset = nOffset * LsbOfPixel;
	strVol.Format(_T("%7.2f"), m_VolOffset);
	pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Volt)->SetWindowText(strVol);

	if (m_MouseOffset >= m_rectPlot.left && m_MouseOffset <= m_rectPlot.right)
	{
		CEdit* pEditOffset = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Offset);
		CString strOffset;
		strOffset.Format(_T("%d"), m_MouseOffset - m_rectPlot.left + gl_Offset);
		pEditOffset->SetWindowText(strOffset);
	}


}

int CADScopeCtrl::FindTrigger(int nChannel, int nValue) // 防止波形抖动
{
	return 0;
}



void CADScopeCtrl::DrawAllChannelText(CDC* pDC)
{
	CString str;
	int signe_id = 0;
	float hight = ((float)m_rectPlot.Height() / m_nChannelCount); // 每通道的Y宽度

	int nGridPixS = 0; // Y方向的网格线的刻度
	int nGridPixE = 0; // Y方向的网格线的刻度


	for (int Channel = 0; Channel <gl_nChannelCount; Channel++)
	{
		COLORREF tempPen =  RGB(0, 0, 0);
		//m_dcGrid.SetTextColor(m_clPen[Channel+ADPara.FirstChannel]); // 设置文字的颜色
		m_dcGrid.SetTextColor(tempPen); // 设置文字的颜色
		if(m_nChannelCount<=10)
		{
			
			nGridPixS = m_rectPlot.top + (int)(m_rectPlot.Height() * Channel) / gl_nChannelCount;
			nGridPixE = m_rectPlot.top + (int)(m_rectPlot.Height() * (Channel+1)) / gl_nChannelCount;
			
			//电压上限
			//str.Format (_T("%.*lf V"), m_nYDecimals, m_dUpperLimit[Channel]/1000.0); // 正电压值
			if (g_nVAxisRange>=1000)
			{
				str.Format (_T("%.f V"),g_nVAxisRange/1000.0); // 正电压值
			}
			else
			{
				str.Format (_T("%.f mV"),g_nVAxisRange); // 正电压值	
			}


			m_dcGrid.TextOut (m_rectPlot.left-4, (int)(nGridPixS+1), str); 
			

			//电压下限
			//m_dcGrid.SetTextAlign (TA_RIGHT|TA_BASELINE);
			//str.Format (_T("%.*lf V"), m_nYDecimals, m_dLowerLimit[Channel]/1000.0); // 负电压值
			if (g_nVAxisRange>=1000)
			{
				str.Format (_T("%.f V"),-1*g_nVAxisRange/1000.0); // 负电压值
			}
			else
			{
				str.Format (_T("%.f mV"),-1*g_nVAxisRange); // 负电压值
			}

			m_dcGrid.TextOut (m_rectPlot.left-4, (int)(nGridPixE-13-1), str);
			
			CFont font;
			CFont *oldfont;

			font.CreatePointFont(90,_T("宋体")); //参数含义1，字体大小，2字体类型，3字
			oldfont = m_dcGrid.SelectObject(&font);
			m_dcGrid.SetTextColor(RGB(0,0,255));//字体颜色RGB   

			signe_id = Drow_text_find_id(Channel+1);
			if(signe_id > 4)
				str.Format(_T("Input %d"), signe_id-4);
			else
				str.Format(_T("Singal %d"), signe_id);
			m_dcGrid.TextOut(m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+hight/2-5), str);
			m_dcGrid.SetTextColor(RGB(0,0,0));//字体颜色RGB  
			m_dcGrid.SelectObject(oldfont);
			DeleteObject(font);//释放资源
		}
		else
		{
			str.Format(_T("CH %d"), Channel+ADPara.FirstChannel);
			m_dcGrid.TextOut(m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+5), str); 
		}
	}
}

void CADScopeCtrl::DrawSingleCHText(CDC* pDC, int nChannelNum)
{
	CString strInfo;
	if (nChannelNum != 0xFFFF)
		pDC->SetTextColor(m_clPen[nChannelNum]);
	else
		pDC->SetTextColor(m_crGridColor);


	CString str;
	if (g_nVAxisRange>=1000)
	{
		str.Format (_T("%.f V"),g_nVAxisRange/1000.0); // 正电压值
	}
	else
	{
		str.Format (_T("%.f mV"),g_nVAxisRange); // 正电压值	
	}


	pDC->TextOut (m_rectPlot.left-4,  m_rectPlot.top, str); 

	if (g_nVAxisRange>=1000)
	{
		str.Format (_T("%.f V"),-1*g_nVAxisRange/1000.0); // 负电压值
	}
	else
	{
		str.Format (_T("%.f mV"),-1*g_nVAxisRange); // 负电压值
	}

	pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight, str);





	//strInfo.Format (_T("%.*lf mV"), 0, m_dUpperLimit[nChannelNum]); // 正电压值
	//pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top, strInfo);

	//pDC->SetTextAlign (TA_RIGHT|TA_BASELINE);
	//strInfo.Format (_T("%.*lf mV"), 0, m_dLowerLimit[nChannelNum]); // 负电压值
	//pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight, strInfo);
	//
	if (nChannelNum == 0xFFFF) // 如果是叠加显示时
		strInfo = "CH";
	else
		strInfo.Format(_T("CH %d"), nChannelNum);
	// 写通道号
	//pDC->TextOut(m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight/2+5, strInfo);      
}

void CADScopeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR hCursor;
	m_bRBtnDown = TRUE;
	int DistanceSub = abs(point.x - m_OldMoveX1) + abs(point.x - m_OldMoveX2);
	int DistanceAB = abs(m_OldMoveX1 - m_OldMoveX2);
	if (DistanceSub == DistanceAB) // 如果按下时鼠标在两个线中间
	{
		m_nLineIndex = 0; // 同时选择两条线时的标志值
		m_nShowCursor = 4; // ???
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		::SetCursor(hCursor);
		
		m_RBtnDownPosX = point.x;
		m_StartOldMoveX1 = m_OldMoveX1; // 保存两线移动时，线条1的起始位置
		m_StartOldMoveX2 = m_OldMoveX2; // 保存两线移动时，线条2的起始位置
		
		CRect ClipRect = m_rectPlot;
		ClientToScreen(&ClipRect);
		if (m_OldMoveX1 < m_OldMoveX2) // 如果线1在线2的左边
		{
			ClipRect.left += point.x - m_OldMoveX1;
			ClipRect.right -= m_OldMoveX2 - point.x;
		}
		else // 如果线1在线2的右边
		{
			ClipRect.left += point.x - m_OldMoveX2;
			ClipRect.right -= m_OldMoveX1 - point.x;
		}
		::ClipCursor(&ClipRect);  
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CADScopeCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR hCursor;
	
	hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	::SetCursor(hCursor);
	
	m_bRBtnDown = FALSE;
	m_nLineIndex = 0xFFFF;
	ClipCursor(NULL);
	CWnd::OnRButtonUp(nFlags, point);
	
}

void CADScopeCtrl::DrawMoveLine(CDC* pDC, CPoint point)
{
	CPen* pOldPen;
	int nOldPositionX = point.x;
	int nOldPositionY = point.y;
	int nMode;
	pDC->SetBkMode(TRANSPARENT);         // 透明背景
	nMode = pDC->SetROP2(R2_XORPEN);	 // 异或绘制模式	
	switch(m_nLineIndex)
	{
	case 0: // 同时选择线1线2
		pOldPen = pDC->SelectObject(&m_PenLineA); 
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // 消除上一次画的线A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);	// 画线A
		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // 消除上一次画的线B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// 画线B		
		m_OldMoveX1 = m_StartOldMoveX1 + (point.x - m_RBtnDownPosX); 
		m_OldMoveX2 = m_StartOldMoveX2 + (point.x - m_RBtnDownPosX); 		
		pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top);
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);
		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top);
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // 恢复旧的画笔
		break;

	case 1: // 选择的线条1
		pOldPen = pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // 消除上一次画的线A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // 画线A
		m_OldMoveX1 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // 恢复旧的画笔
		break;

	case 2: // 选择的线条2
		pOldPen = pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // 消除上一次画的线B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// 画线B
		m_OldMoveX2 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // 恢复旧的画笔
		break;
	case 3: // 选择的线条3
		pOldPen = pDC->SelectObject(&m_PenLineV);
		pDC->MoveTo(m_rectPlot.left,  m_OldVoltageY); // 消除上一次画的线V
		pDC->LineTo(m_rectPlot.right, m_OldVoltageY); // 画线V
		m_OldVoltageY = point.y;
		pDC->MoveTo(m_rectPlot.left, point.y);
		pDC->LineTo(m_rectPlot.right, point.y);
		pDC->SelectObject(pOldPen); // 恢复旧的画笔
		break;

	default:
		break;
	}
	pDC->SetROP2(nMode);
	
}

BOOL CADScopeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nShowCursor)
	{
	case 1: // jump to 2
	case 2:
		::SetCursor(m_hCursorHSize); 
		break;
	case 3:
		::SetCursor(m_hCursorVSize);
		break;
	case 4:
		::SetCursor(m_hCursorAllSize);
		break;
	default:
		::SetCursor(m_hCursorArrow);
		break;
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CADScopeCtrl::UpdateChannelCount()
{
// 	if(!gl_bTileWave)  // 如果是叠加显示
// 	{
// 		m_nChannelCount = 1;
// 	}
// 	else // 如果是平铺显示
// 	{
		m_nChannelCount = gl_nChannelCount;		
// 	}

}
void ProcessOrgAdDataEx(int size_buf)
{
	//fill the showData with gt_AD_OrgData
	
	
	int orgDatCnt=size_buf;
	int startID;//根据传进来的新数据的第一个点的时间，来确定从showdata的哪个点开始填起。
	int orgDatID=0;
	int i,j;
	double fTimePerPoint;//这SHOW_DATA_CNT个点中每个点对应的时间

	//g_nTimeAxisRange = 100*1000*1000;//us为单位 也就是100s
	fTimePerPoint = g_nTimeAxisRange/SHOW_DATA_CNT;  //showData 每个点代表的时间 根据时间轴量程计算
	startID = gt_AD_OrgData[orgDatID].time/fTimePerPoint;
	if (!startID)//第0个数据不填 防止下边变成负数
	{
		startID +=1;	
	}
	for (j=startID;j<SHOW_DATA_CNT;j++)
	{	
		if (gt_AD_OrgData[orgDatID].time == 0)
		{
			startID +=1;	
			startID -=1;	
		}
		if (gt_AD_OrgData[orgDatID+1].time == 0 && j*fTimePerPoint>=gt_AD_OrgData[orgDatID].time)
		{
			showData[0][j] = gt_AD_OrgData[orgDatID].data[0];
			showData[1][j] = gt_AD_OrgData[orgDatID].data[1];
			showData[2][j] = gt_AD_OrgData[orgDatID].data[2];
			showData[3][j] = gt_AD_OrgData[orgDatID].data[3];
			showData[4][j] = gt_AD_OrgData[orgDatID].data[4];
			showData[5][j] = gt_AD_OrgData[orgDatID].data[5];
			showData[6][j] = gt_AD_OrgData[orgDatID].data[6];
			showData[7][j] = gt_AD_OrgData[orgDatID].data[7];
		}
		else if (j*fTimePerPoint<gt_AD_OrgData[orgDatID+1].time &&
			j*fTimePerPoint>=gt_AD_OrgData[orgDatID].time)
		{
			showData[0][j] = gt_AD_OrgData[orgDatID].data[0];
			showData[1][j] = gt_AD_OrgData[orgDatID].data[1];
			showData[2][j] = gt_AD_OrgData[orgDatID].data[2];
			showData[3][j] = gt_AD_OrgData[orgDatID].data[3];
			showData[4][j] = gt_AD_OrgData[orgDatID].data[4];
			showData[5][j] = gt_AD_OrgData[orgDatID].data[5];
			showData[6][j] = gt_AD_OrgData[orgDatID].data[6];
			showData[7][j] = gt_AD_OrgData[orgDatID].data[7];
		}
		//showdata的这个点的数据时间已经比gt_AD_OrgData这个的点的时间大了，那么gt_AD_OrgData跳到下一个点，showdate的id减1让下一次仍然赋值给这个点
		else if (j*fTimePerPoint>=gt_AD_OrgData[orgDatID+1].time)
		{
			if (orgDatID + 1==orgDatCnt-1)
			{
				break;//退出循环，新的数据已经全部被填入showdata中
			}
			orgDatID++;
			j--;
		}
		else if (j*fTimePerPoint<gt_AD_OrgData[orgDatID].time)
		{
			//AfxMessageBox("异常情况");

			//break;
			showData[0][j] = gt_AD_OrgData[orgDatID].data[0];
			showData[1][j] = gt_AD_OrgData[orgDatID].data[1];
			showData[2][j] = gt_AD_OrgData[orgDatID].data[2];
			showData[3][j] = gt_AD_OrgData[orgDatID].data[3];
			showData[4][j] = gt_AD_OrgData[orgDatID].data[4];
			showData[5][j] = gt_AD_OrgData[orgDatID].data[5];
			showData[6][j] = gt_AD_OrgData[orgDatID].data[6];
			showData[7][j] = gt_AD_OrgData[orgDatID].data[7];
		}
		
		else
		{
			//AfxMessageBox("异常情况");
			break;
		}
	}

	if (orgDatID<orgDatCnt-2) //新来的数据超过的屏幕右边，从左边开始
	{
		for (i=orgDatID;i<orgDatCnt-1;i++)
		{
			if (gt_AD_OrgData[i].time == 0)
			{
				orgDatID = i;
				break;
			}
		}
			
		//从头赋值
		for (j=0;j<SHOW_DATA_CNT;j++)
		{
			if (gt_AD_OrgData[orgDatID+1].time == 0 && j*fTimePerPoint>=gt_AD_OrgData[orgDatID].time)
			{
				showData[0][j] = gt_AD_OrgData[orgDatID].data[0];
				showData[1][j] = gt_AD_OrgData[orgDatID].data[1];
				showData[2][j] = gt_AD_OrgData[orgDatID].data[2];
				showData[3][j] = gt_AD_OrgData[orgDatID].data[3];
				showData[4][j] = gt_AD_OrgData[orgDatID].data[4];
				showData[5][j] = gt_AD_OrgData[orgDatID].data[5];
				showData[6][j] = gt_AD_OrgData[orgDatID].data[6];
				showData[7][j] = gt_AD_OrgData[orgDatID].data[7];
			}			
			else if (j*fTimePerPoint<gt_AD_OrgData[orgDatID+1].time &&
				j*fTimePerPoint>=gt_AD_OrgData[orgDatID].time)
			{
				showData[0][j] = gt_AD_OrgData[orgDatID].data[0];
				showData[1][j] = gt_AD_OrgData[orgDatID].data[1];
				showData[2][j] = gt_AD_OrgData[orgDatID].data[2];
				showData[3][j] = gt_AD_OrgData[orgDatID].data[3];
				showData[4][j] = gt_AD_OrgData[orgDatID].data[4];
				showData[5][j] = gt_AD_OrgData[orgDatID].data[5];
				showData[6][j] = gt_AD_OrgData[orgDatID].data[6];
				showData[7][j] = gt_AD_OrgData[orgDatID].data[7];
			}
			//showdata的这个点的数据时间已经比gt_AD_OrgData这个的点的时间大了，那么gt_AD_OrgData跳到下一个点，showdate的id减1让下一次仍然赋值给这个点
			else if (j*fTimePerPoint>=gt_AD_OrgData[orgDatID+1].time)
			{
				if (orgDatID+1==orgDatCnt-1)
				{
					break;//退出循环，新的数据已经全部被填入showdata中
				}
				orgDatID++;
				j--;
			}
			else if (j*fTimePerPoint<gt_AD_OrgData[orgDatID].time)
			{
				//AfxMessageBox("异常情况");
				break;
			}
			
			else
			{
				//AfxMessageBox("异常情况");
				break;
			}
		}		
	}		
}

void CADScopeCtrl::ProcessOrgAdData(int size_buf)
{
	if (gl_bDeviceADRun == FALSE)
	{
		return;
	}
	
	
	int startID,endID = 0;//根据传进来的新数据的第一个点的时间，来确定从showdata的哪个点开始填起。
	int i,j;
	double fTimePerPoint;//这SHOW_DATA_CNT个点中每个点对应的时间
	
	fTimePerPoint = g_nTimeAxisRange/SHOW_DATA_CNT;  //showData 每个点代表的时间 根据时间轴量程计算

	startID = gt_AD_OrgData[0].time/fTimePerPoint;
	if(gl_last_end_id+1 < startID)
	{
		for (j=gl_last_end_id;j<=startID;j++)
		{
			showData[0][j] = gt_AD_OrgData[0].data[0];
			showData[1][j] = gt_AD_OrgData[0].data[1];
			showData[2][j] = gt_AD_OrgData[0].data[2];
			showData[3][j] = gt_AD_OrgData[0].data[3];
			showData[4][j] = gt_AD_OrgData[0].data[4];
			showData[5][j] = gt_AD_OrgData[0].data[5];
			showData[6][j] = gt_AD_OrgData[0].data[6];
			showData[7][j] = gt_AD_OrgData[0].data[7];			
		}
	}


	for (i=0;i<size_buf-1;i++)
	{	
		if (gt_AD_OrgData[i].time<gt_AD_OrgData[i+1].time)
		{
			startID = gt_AD_OrgData[i].time/fTimePerPoint;
			endID = gt_AD_OrgData[i+1].time/fTimePerPoint;

		} 
		else
		{
			startID = gt_AD_OrgData[i].time/fTimePerPoint;
			endID = SHOW_DATA_CNT-1;
		}

		startID = (startID >= SHOW_DATA_CNT) ? SHOW_DATA_CNT-1 : startID;
		endID = (endID >= SHOW_DATA_CNT) ? SHOW_DATA_CNT-1 : endID;



		for (j=startID;j<=endID;j++)
		{
			showData[0][j] = gt_AD_OrgData[i].data[0];
			showData[1][j] = gt_AD_OrgData[i].data[1];
			showData[2][j] = gt_AD_OrgData[i].data[2];
			showData[3][j] = gt_AD_OrgData[i].data[3];
			showData[4][j] = gt_AD_OrgData[i].data[4];
			showData[5][j] = gt_AD_OrgData[i].data[5];
			showData[6][j] = gt_AD_OrgData[i].data[6];
			showData[7][j] = gt_AD_OrgData[i].data[7];				
		}
	}
	gl_last_end_id = endID;
}




