//CADScopeCtrl.cpp : implementation file //

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERID 8

///////////////////////////////////////////////////////////////////////////
//CADScopeCtrl
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
//	m_crBackColor  = RGB(0,  0,   0);  
	m_crGridColor  = RGB(255, 255, 255);  
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
	
	m_crLineAColor = RGB(255, 192, 255);  // A线的颜色
    m_crLineBColor = RGB(192, 255, 255);  // B线的颜色
    m_crLineVColor = RGB(255, 255, 128);  // C线的颜色
	
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
	
}  

void CADScopeCtrl::OnTimer(UINT_PTR nIDEvent)

{
	DrawBkGnd();  // 画背景
	DrawPoly(); // 画线
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
		m_rectPlot.bottom = m_rectClient.bottom - 10; //-25;
		
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

	m_rectClient = CRect(0, 0, 1440, 1000);   
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
	
	//SetBackgroundColor(RGB(255,  255,   255));
	SetGridColor(RGB(0,  0,   0));//设置框架颜色
	//SetPlotColor(RGB(0,  255,   255));
	return 0;
}

//########################################################################
//画背景（网格和外框）
void CADScopeCtrl::DrawBkGnd()
{
	int nCharacters = 0;
	CPen* oldPen;
	CPen solidPen(PS_SOLID, 0, m_crGridColor);
	CFont axisFont, yUnitFont;
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
	
	PerY = (int)(m_nPlotHeight / m_nChannelCount); // 每通道的Y宽度
	//-----------------	-----------------------------------------------------------------
	// 画四周的框架	
	oldPen = m_dcGrid.SelectObject (&solidPen);  // 画四周的方框
	m_dcGrid.MoveTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.SelectObject (oldPen); //WYL 设置虚线颜色 
	//COLORREF m_Grid = RGB(200, 200, 200);

	COLORREF m_Grid = RGB(255, 0, 0);//
	int HLine = 0, VLine = 0;
	for (VLine=50; VLine<m_rectPlot.Width(); VLine+= 50) // 相隔50个像素画一条垂直的线
	{
		for (int HLine=5; HLine<m_rectPlot.Height(); HLine+=5) // 画垂直方向虚线
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + VLine, m_rectPlot.top + HLine), m_Grid);
		}
	}
	for (HLine=30; HLine<m_rectPlot.Height(); HLine+=30) // 相隔30个像素画一条水平方向的线
	{
		for (VLine=0; VLine<m_rectPlot.Width(); VLine+=10) // 画水平方向虚线
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + VLine, m_rectPlot.top + HLine), m_Grid);
		}
	}
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
			Center = (int)(PerY/2.0)+m_rectPlot.top;
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
	m_BufferID = BufferID; // 段缓冲ID
	m_Offset = Offset;     // 段内偏移
	TransitionData();	// 将原码转化为屏幕绘图Y坐标
	ProcessData(); // 处理数据
	DrawBkGnd();  // 画背景
	DrawPoly(); // 画线
}

//###############################################################################
//处理数据, 把缓存中的数据转换成可以显示的点坐标
void CADScopeCtrl::TransitionData()
{
	float LsbOfPixel, LsbOfPixelOne;
	float fScreenVolume = AD_VOLT_RANGE; // 多通道显示时，屏幕是量程是和最大电压值一致的
	LsbOfPixel = (float)(((gl_voltVolume/gl_voltVolume)*AD_LSB_COUNT)/(PerY )); // 每像素对应的码值    
	LsbOfPixelOne = (float)(((gl_ScreenVolume*1.0/(gl_voltVolume))*AD_LSB_COUNT)/(m_rectPlot.Height())); // 每像素对应的码值
	for (int Index=0; Index < (AD_LSB_HALF * 2); Index++) // 将原码转化为屏幕绘图Y坐标
	{
		m_nCoordinateY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixel);
		m_nCoordinateOneY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixelOne);
	}
}

//处理数据, 把缓存中的数据转换成可以显示的点坐标
void CADScopeCtrl::ProcessData()
{
	UpdateChannelCount();
	Center = (int)(PerY/2.0)+m_rectPlot.top;
	int Channel, Index, StartX;
	PWORD  ptOffset; // 缓存指针
	int Offset = 0, DataY = 0;
	StartX = m_rectPlot.left+1; // X方向的起始位置
	
	if (gl_bTileWave) // 多通道平铺显示
	{
		for (Channel=0; Channel<m_nChannelCount; Channel++)
		{
			ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
			pointxy[Channel][0].x = StartX;
			pointxy[Channel][0].y = m_nCoordinateY[ptOffset[Offset]&MASK_MSB];
			
			for (Index=0; Index<=4096/m_nChannelCount; Index++) // 初始化1024个点(创建时，位图的大小) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				pointxy[Channel][Index].y = (int)(Center) - m_nCoordinateY[ptOffset[(Offset+Index) * m_nChannelCount + Channel]&MASK_MSB];
			}
			HeightMid[Channel] = Center; // 保存通道中间位置坐标
			Center += PerY;
		}
	}
	else // 多通道叠加显示
	{
		ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
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
//处理数据, 把缓存中的数据转换成可以显示的点坐标
void CADScopeCtrl::ProcessData2()
{
	int screenWidth;
	
	UpdateChannelCount();
	Center = (int)(PerY/2.0)+m_rectPlot.top;

	//screenWidth = m_rectPlot.Width


	int Channel, Index, StartX;
	PWORD  ptOffset; // 缓存指针
	int Offset = 0, DataY = 0;
	StartX = m_rectPlot.left+1; // X方向的起始位置
	
	if (gl_bTileWave) // 多通道平铺显示
	{
		for (Channel=0; Channel<m_nChannelCount; Channel++)
		{
			ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
			pointxy[Channel][0].x = StartX;
			pointxy[Channel][0].y = m_nCoordinateY[ptOffset[Offset]&MASK_MSB];
			
			for (Index=0; Index<=screenWidth; Index++) // 初始化1024个点(创建时，位图的大小) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				pointxy[Channel][Index].y = (int)(Center) - m_nCoordinateY[ptOffset[(Offset+Index) * m_nChannelCount + Channel]&MASK_MSB];
			}
			HeightMid[Channel] = Center; // 保存通道中间位置坐标
			Center += PerY;
		}
	}
	else // 多通道叠加显示
	{
		ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
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
void CADScopeCtrl::DrawPoly2()
{
	int DataY = 0;
	int nDrawCount = 0;
	gl_bDataProcessing = TRUE;
	CPen* oldPen;
	//m_dcPlot.SetBkColor (RGB(255,255,255)/*m_crBackColor*/);
	//m_dcPlot.FillRect(m_rectClient, &m_brushBack);
	//m_dcPlot.SetTextColor(RGB(255, 158, 0));
	//---------------------------------------------------------------------------------

	/*if (m_nChannelCount < 6)
	{
		nDrawCount = m_rectPlot.Width() - 1;
	}
	else
	{
		nDrawCount = 4096/m_nChannelCount - 1;
	}*/
	nDrawCount = (4096-4096%m_nChannelCount)/m_nChannelCount - 1;
	if (nDrawCount>m_rectPlot.Width() - 1)
	{
		nDrawCount = m_rectPlot.Width() - 1;
	}
	if (m_bAllChannel || !gl_bTileWave) // 所有通道显示或叠加显示时
	{
		for (int Channel = 0; Channel<m_nChannelCount; Channel++) // 画所有通道的点
		{
			oldPen = m_dcGrid.SelectObject(&m_penChannel[Channel]);
			m_dcGrid.Polyline(pointxy[Channel], nDrawCount);
		}
	}
	else // 单通道显示
	{
		m_dcGrid.SelectObject(&m_penChannel[m_nChannelNum]);
		int StartX = m_rectPlot.left;
		Center = (int)(m_nPlotHeight / 2) + m_rectPlot.top;
		WORD* ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
		for (int Index=0; Index<nDrawCount; Index++)	
		{
			pointTemp[Index].x = StartX  + Index;
			pointTemp[Index].y = (int)(Center) - m_nCoordinateOneY[(ptOffset[Index * m_nChannelCount + m_nChannelNum-ADPara.FirstChannel]&MASK_MSB)-gl_MiddleLsb[m_nChannelNum]];
		}
		m_dcGrid.Polyline(pointTemp, nDrawCount);
	}
	gl_bDataProcessing = FALSE;
	Invalidate(FALSE);
	UpdateWindow();
}
//##############################################################################
//画处理完的点
void CADScopeCtrl::DrawPoly()
{
	int DataY = 0;
	int nDrawCount = 0;
	gl_bDataProcessing = TRUE;
	CPen* oldPen;
	//m_dcPlot.SetBkColor (RGB(255,255,255)/*m_crBackColor*/);
	//m_dcPlot.FillRect(m_rectClient, &m_brushBack);
	//m_dcPlot.SetTextColor(RGB(255, 158, 0));
	//---------------------------------------------------------------------------------

	/*if (m_nChannelCount < 6)
	{
		nDrawCount = m_rectPlot.Width() - 1;
	}
	else
	{
		nDrawCount = 4096/m_nChannelCount - 1;
	}*/
	nDrawCount = (4096-4096%m_nChannelCount)/m_nChannelCount - 1;

	for (int Channel = 0; Channel<m_nChannelCount; Channel++) // 画所有通道的点
	{
		for(int i= 0;i<nDrawCount;i++)
		{
			POINT p;
			p.x = i;
			p.y = i%100;
			pointxy[Channel][i] = p;
		}
	}

	if (nDrawCount>m_rectPlot.Width() - 1)
	{
		nDrawCount = m_rectPlot.Width() - 1;
	}
	if (m_bAllChannel || !gl_bTileWave) // 所有通道显示或叠加显示时
	{
		for (int Channel = 0; Channel<m_nChannelCount; Channel++) // 画所有通道的点
		{
			oldPen = m_dcGrid.SelectObject(&m_penChannel[Channel]);
			m_dcGrid.Polyline(pointxy[Channel], nDrawCount);
		}
	}
	else // 单通道显示
	{
		m_dcGrid.SelectObject(&m_penChannel[m_nChannelNum]);
		int StartX = m_rectPlot.left;
		Center = (int)(m_nPlotHeight / 2) + m_rectPlot.top;
		WORD* ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // 指针的偏移量
		for (int Index=0; Index<nDrawCount; Index++)	
		{
			pointTemp[Index].x = StartX  + Index;
			pointTemp[Index].y = (int)(Center) - m_nCoordinateOneY[(ptOffset[Index * m_nChannelCount + m_nChannelNum-ADPara.FirstChannel]&MASK_MSB)-gl_MiddleLsb[m_nChannelNum]];
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
		CEdit* pEditFre = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
		pEditFre->SetWindowText(strFrequency);		
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
		
		CEdit *pEditFre = (CEdit *)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
		pEditFre->SetWindowText(strFrequency);
	}
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

//画所有通道的网格线
void CADScopeCtrl::DrawAllChannelGrid(CDC* pDC)
{
	int X = 0, Channel = 0;
	int nGridPix = 0; // Y方向的网格线的刻度
	float hight = (float)(m_rectPlot.Height() / m_nChannelCount); // 每通道的Y宽度
	
	for (Channel=1; Channel<m_nChannelCount; Channel++) // 画m_nChannelCount-1条网格线
	{
		nGridPix = m_rectPlot.top + (int)(m_rectPlot.Height() * Channel) / m_nChannelCount;
		for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=1) // 每隔2个像素画1点
		{
			pDC->SetPixel(X, nGridPix, m_crGridColor);
			//pDC->SetPixel(X, nGridPix+1, m_crGridColor);
		}	
	}

	for (Channel=0; Channel<m_nChannelCount; Channel++) // 画每通道的中线
	{
		for (X=m_rectPlot.left; X<m_rectPlot.right; X+=3) // 每隔3个像素画1点
		{
			pDC->SetPixel(X, (int)(m_rectPlot.top + (hight * Channel) + hight/2.0), RGB(0, 0, 0)); 
		}
		
	}
}

void CADScopeCtrl::DrawSingleCHGrid(CDC* pDC)
{
	//for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=2) // 每隔4个像素画1点
	//{
	//	pDC->SetPixel(X, m_rectPlot.top+(int)(m_rectPlot.Height()/2), RGB(0, 0, 0));	
	//}
}


void CADScopeCtrl::DrawAllChannelText(CDC* pDC)
{
	CString str;
	float hight = (float)(m_rectPlot.Height() / m_nChannelCount); // 每通道的Y宽度
	for (int Channel = 0; Channel <gl_nChannelCount; Channel++)
	{
		COLORREF tempPen =  RGB(0, 0, 0);
		//m_dcGrid.SetTextColor(m_clPen[Channel+ADPara.FirstChannel]); // 设置文字的颜色
		m_dcGrid.SetTextColor(tempPen); // 设置文字的颜色
		if(m_nChannelCount<=10)
		{
			str.Format (_T("%.*lf V"), m_nYDecimals, m_dUpperLimit[Channel]/1000.0); // 正电压值
			m_dcGrid.TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+8), str); 
			
			m_dcGrid.SetTextAlign (TA_RIGHT|TA_BASELINE);
			str.Format (_T("%.*lf V"), m_nYDecimals, m_dLowerLimit[Channel]/1000.0); // 负电压值
			m_dcGrid.TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*(Channel+1)-5), str);
			
			CFont font;
			CFont *oldfont;

			font.CreatePointFont(100,_T("黑体")); //参数含义1，字体大小，2字体类型，3字
			oldfont = m_dcGrid.SelectObject(&font);
			m_dcGrid.SetTextColor(RGB(0,0,255));//字体颜色RGB   
				str.Format(_T("CH %d"), Channel+ADPara.FirstChannel);
				m_dcGrid.TextOut(m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+hight/2+5), str);
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

	strInfo.Format (_T("%.*lf mV"), 0, m_dUpperLimit[nChannelNum]); // 正电压值
	pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top, strInfo);

	pDC->SetTextAlign (TA_RIGHT|TA_BASELINE);
	strInfo.Format (_T("%.*lf mV"), 0, m_dLowerLimit[nChannelNum]); // 负电压值
	pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight, strInfo);
	
	if (nChannelNum == 0xFFFF) // 如果是叠加显示时
		strInfo = "CH";
	else
		strInfo.Format(_T("CH %d"), nChannelNum);
	// 写通道号
	pDC->TextOut(m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight/2+5, strInfo);      
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
void CADScopeCtrl::ProcessOrgAdData()
{
	double fTimePoint1,fTimePoint2;//
	int rang = 100*1000;
	int i,j;

	fTimePoint1 =  rang/10000;  //showData 每个点代表的时间 根据满屏时间值在计算
	fTimePoint2= 8/ADPara.Frequency;//新读出来的这100点

	static int lastShowDateID;//保存上次存到的位置



	if(fTimePoint1<fTimePoint2)
	{

		for(i=lastShowDateID;i<10000;i++)
		{
			if( i* rang/10000<j* 8/ADPara.Frequency)
			{
				for (int Channel = 0; Channel<8; Channel++) // 画所有通道的点
				{
					showData[Channel][i]= AD_OrgData[j*8 + Channel]&MASK_MSB; //先存原始数据，先不管坐标值。
				}			
			}
			else
			{
				j++;
				if(j>=100)
				{
					break;
				}
				i--;//j变了 所以i这次不算，再来。
			}

		}
	
	
		//如果到了10000了 j还不等于100，还没用完，我们再从i=0开始
		//判断这次会不会到达10000个点的结尾
		if(j<100)
		{
			for(i=0;i<10000;i++)
			{
				if(i* rang/10000<j* 8/ADPara.Frequency)
				{
					for(int Channel = 0; Channel<8; Channel++) // 画所有通道的点
					{
						showData[Channel][i]= AD_OrgData[j*8 + Channel]&MASK_MSB; //先存原始数据，先不管坐标值。
					}			
				}
				else
				{
					j++;
					if(j>=100)
					{
						lastShowDateID = i;
						break;
					}
					i--;//i这次不算，再来。
				}

			}
		
		}
	}
}

