//CADHistScope.cpp : implementation file //

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
//CADHistScope
CADHistScope::CADHistScope()
{
	m_dPreviousPosition =   0.0;	
	m_nYDecimals = 3;
	// 	for (int Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++)
	// 	{
	// 		m_dLowerLimit[Channel] = -10000.0;
	// 		m_dUpperLimit[Channel] =  10000.0;
	// 	}

	m_dRange		   =  (double)(m_dUpperLimit - m_dLowerLimit);    
	m_nShiftPixels     = 4;
	m_nHalfShiftPixels = m_nShiftPixels/2;                      
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels;   

	m_crBackColor  = RGB(0,   0,   0);  
	m_crGridColor  = RGB(255, 0, 0); 
	m_crGridGreyColor  = RGB(192, 192, 192);

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

	m_crLineAColor = RGB(255, 192, 255);  // A�ߵ���ɫ
	m_crLineBColor = RGB(192, 255, 255);  // B�ߵ���ɫ
	m_crLineVColor = RGB(255, 255, 128);  // C�ߵ���ɫ

	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor);
	m_brushBack.CreateSolidBrush(m_crBackColor);

	m_strXUnitsString.Format(_T("Samples"));  
	m_strYUnitsString.Format(_T("Y units"));  

	m_pbitmapOldGrid = NULL;
	m_pbitmapOldPlot = NULL;
	m_nChannelCount = 8;
	m_nChannelNum = 0;
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
	m_nShowCursor = 0; // ��ʾ���
} // CADHistScope

///////////////////////////////////////////////////////////////////////////

CADHistScope::~CADHistScope()
{
	if (m_pbitmapOldGrid != NULL)
		m_dcGrid.SelectObject(m_pbitmapOldGrid);  
	if (m_pbitmapOldPlot != NULL)
		m_dcPlot.SelectObject(m_pbitmapOldPlot);	
}

BEGIN_MESSAGE_MAP(CADHistScope, CWnd)
	//{{AFX_MSG_MAP(CADHistScope)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////
//ADScopeCtrl message handlers
///////////////////////////////////////////////////////////////////////////
BOOL CADHistScope::Create(DWORD dwStyle, const RECT& rect, 
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

static int Drow_text_find_id(int cur_id)
{
	int find_cnt = 0;
	int i;
	for(i = 0; i < 8; i++)
	{
		if(m_channel_enable[i] == 1)
			find_cnt++;
		if(find_cnt ==cur_id)
			break;
	}
	return i+1;
}


///////////////////////////////////////////////////////////////////////////
//���ô�ֱ�����������̵Ĵ�С
void CADHistScope::SetRange(double dLower, double dUpper, int nChannel)
{
	ASSERT(dUpper > dLower);
	m_dLowerLimit[nChannel]  = dLower;
	m_dUpperLimit[nChannel]  = dUpper;
	m_nYDecimals      = 2;    //  С����λ��
	m_dRange          = (double)(m_dUpperLimit - m_dLowerLimit);
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 
	InvalidateCtrl();
} // SetRange

///////////////////////////////////////////////////////////////////////////
void CADHistScope::SetXUnits(CString string)
{
	m_strXUnitsString = string;

	InvalidateCtrl();

}  // SetXUnits

///////////////////////////////////////////////////////////////////////////
void CADHistScope::SetYUnits(CString string)
{
	m_strYUnitsString = string;

	InvalidateCtrl();

}  // SetYUnits

///////////////////////////////////////////////////////////////////////////
void CADHistScope::SetGridColor(COLORREF color)
{
	m_crGridColor = color;

	InvalidateCtrl();

}  // SetGridColor


///////////////////////////////////////////////////////////////////////////
void CADHistScope::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color;

	m_penPlot.DeleteObject();
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor);

	InvalidateCtrl();

}  // SetPlotColor


///////////////////////////////////////////////////////////////////////////
void CADHistScope::SetBackgroundColor(COLORREF color)
{
	m_crBackColor = color;

	m_brushBack.DeleteObject();
	m_brushBack.CreateSolidBrush(m_crBackColor);

	InvalidateCtrl();

}  // SetBackgroundColor

///////////////////////////////////////////////////////////////////////////
double CADHistScope::AppendPoint(double dNewPoint)
{	
	double dPrevious;

	dPrevious = m_dCurrentPosition;
	m_dCurrentPosition = dNewPoint;

	DrawPoint();
	Invalidate();

	return dPrevious;

} // AppendPoint

///////////////////////////////////////////////////////////////////////////
void CADHistScope::InvalidateCtrl()
{
	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
	CADHistDoc* pDoc =(CADHistDoc*)pView->GetDocument();


	for (int index=0; index<USB2831_MAX_AD_CHANNELS; index++)
	{
		m_dLowerLimit[index] = pDoc->m_Header.VoltBottomRange;
		m_dUpperLimit[index] = pDoc->m_Header.VoltTopRange;
	}

	m_HistFirstChannel = pDoc->m_Header.ADPara.FirstChannel;
	m_HistLastChannel = pDoc->m_Header.ADPara.LastChannel;

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
		m_PenLineA.CreatePen(PS_DASHDOT, 1, m_crLineAColor); // ����A�ߵĻ���
	if (m_PenLineB.m_hObject == NULL)
		m_PenLineB.CreatePen(PS_DASHDOT, 1, m_crLineBColor); // ����B�ߵĻ���
	if (m_PenLineV.m_hObject == NULL)
		m_PenLineV.CreatePen(PS_DASHDOT, 1, m_crLineVColor); // ����V�ߵĻ���

	int Channel = 0;
	for (Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++)
	{
		if (m_penChannel[Channel].m_hObject == NULL)
			m_penChannel[Channel].CreatePen(PS_SOLID, 1, m_clPen[Channel]);
	}

	//if (!gl_bTileWave) m_channel_cnt = 1; // ����ǵ�����ʾ���õ�ͨ����ʾ�ķ�ʽ

	// 	if (gl_bTileWave)  // �����ƽ����ʾ
	// 	{
	//m_nChannelCount = m_HistLastChannel - m_HistFirstChannel + 1;

	//m_channel_cnt = m_channel_cnt;
	// 	}
	// 	if (!gl_bTileWave) m_channel_cnt = 1; // ����ǵ�����ʾ���õ�ͨ����ʾ�ķ�ʽ
	DrawBkGnd();	// ������
	DrawPoly();
	TransitionData();
	m_bInitialed = TRUE;

} // InvalidateCtrl

//////////////////////////////////////////////////////////////////////////
void CADHistScope::OnPaint() 
{
	// �����m_nClientWidth, m_ClientHeightҲ��ȡ����m_rectClient�Ĵ�С
	CPaintDC dc(this);  // device context for painting
	CDC memDC;
	CBitmap memBitmap;
	CBitmap* oldBitmap; // bitmap originally found in CMemDC
	memDC.CreateCompatibleDC(&dc);
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);

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

	memDC.SelectObject(oldBitmap);

	int	nDrawMode;
	dc.SetBkMode(TRANSPARENT);		                // ���ñ���Ϊ͸��
	nDrawMode = dc.SetROP2(R2_XORPEN);              // ���û���ģʽ

	dc.SelectObject(&m_PenLineA);                   // ѡ����A�Ļ���
	dc.MoveTo(m_OldMoveX1, m_rectPlot.top);
	dc.LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // ����A

	WCHAR str[100];
	swprintf_s(str, _T("%s"), _T("A��"));
	dc.SetTextColor(RGB(255, 0, 0));
	dc.TextOut(m_OldMoveX1, m_rectPlot.top, str);

	dc.SelectObject(&m_PenLineB);				    // ѡ������B�Ļ���
	dc.MoveTo(m_OldMoveX2, m_rectPlot.top);
	dc.LineTo(m_OldMoveX2, m_rectPlot.Height()+10); // ����B
	swprintf_s(str, _T("%s"), _T("B��"));
	dc.SetTextColor(RGB(255, 0, 0));
	dc.TextOut(m_OldMoveX2, m_rectPlot.top, str);
	if (!m_bAllChannel)
	{
		dc.SelectObject(&m_PenLineV);				// ѡ������V�Ļ���
		dc.MoveTo(m_rectPlot.left, m_OldVoltageY);
		dc.LineTo(m_rectPlot.right, m_OldVoltageY);	// ����V
	}

	dc.SetROP2(nDrawMode);
} // OnPaint

//#####################################################################
//�������ĵ�(λͼ���ƶ�������̬)
void CADHistScope::DrawPoint()
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
void CADHistScope::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (m_bInitialed) 
	{ 
		GetClientRect(m_rectClient); // ��ȡ��ǰ�Ŀͻ�����С

		m_nClientHeight = m_rectClient.Height(); 
		m_nClientWidth  = m_rectClient.Width();

		m_rectPlot.left   = 60;  
		m_rectPlot.top    = 10;
		m_rectPlot.right  = m_rectClient.right -10;
		m_rectPlot.bottom = m_rectClient.bottom - 10; //-25;

		m_nPlotHeight = m_rectPlot.Height();
		m_nPlotWidth  = m_rectPlot.Width();	

		m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 	
		InvalidateCtrl(); // ���»���

		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;
		m_OldMoveX1   = m_CurMoveX1 = m_rectPlot.left + 20;
		m_OldMoveX2   = m_CurMoveX2 = m_rectPlot.right - 20;
		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;		
	}
} // OnSize

///////////////////////////////////////////////////////////////////////////
void CADHistScope::Reset()
{
	InvalidateCtrl();
}

///////////////////////////////////////////////////////////////////////////
int CADHistScope::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{ 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rectClient = CRect(0, 0, 1400, 1000);   
	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth  = m_rectClient.Width();

	m_rectPlot.left   = 60;  
	m_rectPlot.top    = 10;
	m_rectPlot.right  = m_rectClient.right-10;
	m_rectPlot.bottom = m_rectClient.bottom-25;

	m_nPlotHeight = m_rectPlot.Height();
	m_nPlotWidth  = m_rectPlot.Width();

	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 

	return 0;

}

//########################################################################
//����������������
void CADHistScope::DrawBkGnd()  
{ 
	int nCharacters = 0;
	CPen* oldPen;
	CPen solidPen(PS_SOLID, 0, m_crGridColor);
	CFont axisFont, yUnitFont;
	CFont* oldFont;
	CString strTemp;
	//---------------------------------------------------------------------------
	m_dcGrid.SetBkColor (m_crBackColor);	
	// fill the grid background
	m_dcGrid.FillRect(m_rectClient, &m_brushBack);	
	// ���������ִ���ռ�õ��ַ����
	nCharacters = abs((int)log10(fabs(m_dUpperLimit[0])));
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit[0]))));
	nCharacters = nCharacters + 4 + m_nYDecimals;  

	PerY = (int)(m_nPlotHeight / m_channel_cnt); // ÿͨ����Y���
	//----------------------------------------------------------------------------------
	// �����ܵĿ��	
	oldPen = m_dcGrid.SelectObject (&solidPen);  // �����ܵķ���
	m_dcGrid.MoveTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.SelectObject (oldPen);  
	COLORREF m_Grid = RGB(200, 200, 200);
	int HLine = 0, VLine = 0;
	for (VLine=50; VLine<m_rectPlot.Width(); VLine+= 50) // ���50�����ػ�һ����ֱ����
	{
		for (int HLine=5; HLine<m_rectPlot.Height(); HLine+=5) // ����ֱ��������
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + VLine, m_rectPlot.top + HLine), m_Grid);
		}
	}

	for (HLine=30; HLine<m_rectPlot.Height(); HLine+=30) // ���30�����ػ�һ��ˮƽ�������
	{
		for (VLine=0; VLine<m_rectPlot.Width(); VLine+=10) // ��ˮƽ��������
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + VLine, m_rectPlot.top + HLine), m_Grid);
		}
	}	
	//----------------------------------------------------------------------------
	// ��ÿ��ͨ���ķֽ���(ˮƽ��)

	if (m_bAllChannel) // ��ʾ����ͨ��
	{
		if (gl_bTileWave) // �����ƽ����ʾ
		{
			DrawAllChannelGrid(&m_dcGrid); // ������ͨ���ĺ�������
		}
		else
		{
			DrawSingleCHGrid(&m_dcGrid);
		}
	}
	else // ����ͨ���ĺ���������
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
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP); // ���뷽ʽ	

	//----------------------------------------------------------------------

	if (m_bAllChannel)
	{
		if (gl_bTileWave) // ƽ����ʾ
		{
			DrawAllChannelText(&m_dcGrid);
		}
		else // ������ʾ
		{
			DrawSingleCHText(&m_dcGrid, 0);
		}

	}
	else
	{
		DrawSingleCHText(&m_dcGrid, m_nChannelNum);
	}

	InvalidateRect(m_rectClient); // ˢ������

}

//########################################################################
//��PolyLine��
void CADHistScope::OnMouseMove(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);
	if (m_rectPlot.PtInRect(point))
	{
		if (!m_bRBtnDown) m_nShowCursor = 0;
		// ͨ������λ���ж�����һ���߻�ý���
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

	m_MouseOffset = point.x - 60;

	CString str;
	CADHistFrm* pADHistFrm = (CADHistFrm*)GetParentFrame();	
	CEdit* pOffsetEdit = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));	
	str.Format(_T("%d"), m_MouseOffset + m_Offset);
	pOffsetEdit->SetWindowText(str);  // ��ʾ�ļ�ƫ��

	SetStatusBar(); // ����״̬����Ƶ�ʵ�

	CWnd::OnMouseMove(nFlags, point);
}

void CADHistScope::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	m_bLBtnDown = TRUE;
	if (m_rectPlot.PtInRect(point))
	{
		// ͨ������λ���ж�����һ���߻�ý���
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
//��ԭ��ֵ����(buffer������ԭ��), �ٴ���ɿ�����ʾ�ĵ�����
void CADHistScope::AppendPoly(int BufferID, int  Offset)
{
	m_BufferID = BufferID; // �λ���ID
	m_Offset = Offset;     // ����ƫ��

	TransitionData();
	DrawBkGnd();	// ������
	ProcessData(); // ��������
	DrawPoly(); // ����
}

//###############################################################################
//��������, �ѻ����е�����ת���ɿ�����ʾ�ĵ�����
void CADHistScope::TransitionData()
{
	float LsbOfPixel, LsbOfPixelOne;
	float fScreenVolume = AD_VOLT_RANGE; // ��ͨ����ʾʱ����Ļ�������Ǻ�����ѹֵһ�µ�
	LsbOfPixel = (float)(((AD_LSB_COUNT*g_nVAxisRange/AD_VOLT_RANGE)/(PerY))); // ÿ���ض�Ӧ����ֵ    
	LsbOfPixelOne = (float)(((gl_ScreenVolume/(AD_VOLT_RANGE*2))*AD_LSB_COUNT)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ

	for (int Index=0; Index <= (AD_LSB_HALF * 2); Index++) // ��ԭ��ת��Ϊ��Ļ��ͼY����
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

void CADHistScope::ProcessData()  
{	
	Center = (int)(PerY/2.0)+m_rectPlot.top;
	int Channel, Index, StartX;
	PWORD  ptOffset; // ����ָ��
	int Offset = 0, DataY = 0;
	CADHistDoc* pHistDoc = theApp.m_pADHistDoc;
	int channe_id_enable = 0;

	StartX = m_rectPlot.left+1; // X�������ʼλ��

	if (gl_bTileWave) // ��ͨ��ƽ����ʾ
	{
		for (Channel=0; Channel<m_channel_cnt; Channel++)
		{
			ptOffset = &(pHistDoc->m_ADBuffer[m_Offset]); // ָ���ƫ����
			//pointxy[Channel][0].x = StartX;
			//pointxy[Channel][0].y = m_nCoordinateY[ptOffset[Offset]&MASK_MSB];
			channe_id_enable = Drow_text_find_id(Channel+1);
			//for (Index=0; Index<=m_nPlotWidth; Index++) // ��ʼ��1024����(����ʱ��λͼ�Ĵ�С) 
			for (Index=0; Index<m_nPlotWidth; Index++) // ��ʼ��1024����(����ʱ��λͼ�Ĵ�С) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				pointxy[Channel][Index].y = (int)(Center) - m_nCoordinateY[showData[channe_id_enable-1][Index*SHOW_DATA_CNT/m_nPlotWidth]&MASK_MSB];
				//pointxy[Channel][Index].y = (int)(Center) - m_nCoordinateY[ptOffset[(Offset + Index) * m_channel_cnt + Channel]&MASK_MSB];
			}
			HeightMid[Channel] = Center; // ����ͨ���м�λ������
			Center += PerY;
		} 
	}
	else // ��ͨ��������ʾ
	{
		for (Channel=0; Channel<m_channel_cnt; Channel++)
		{
			ptOffset = &(pHistDoc->m_ADBuffer[m_Offset]); // ָ���ƫ����
			int StartX = m_rectPlot.left;
			Center = (int)(m_rectPlot.Height() / 2) + m_rectPlot.top;
			for (int Index=0; Index<=m_nPlotWidth; Index++)	
			{
				pointxy[Channel][Index].x = StartX + Index;	
				pointxy[Channel][Index].y = m_nCoordinateY[ptOffset[(Offset + Index) * m_nChannelCount + Channel]&MASK_MSB];
			}
		}
	}


	m_bDrawPoly = TRUE; 

}

void CADHistScope::ProcessDataEx()  
{	
	float m_ScreenVolume = AD_VOLT_RANGE; // ��ͨ����ʾʱ����Ļ�������Ǻ�����ѹֵһ�µ�
	float LsbOfPixel;
	LsbOfPixel = (float)(((m_ScreenVolume/AD_VOLT_RANGE)*AD_LSB_COUNT)/(PerY)); // ÿ���ض�Ӧ����ֵ    
	Center = (int)(PerY/2.0)+m_rectPlot.top;
	int Channel, Index, StartX;
	PWORD  ptOffset; // ����ָ��
	int Offset = 0, DataY = 0;
	CADHistDoc* pHistDoc = theApp.m_pADHistDoc;

	StartX = m_rectPlot.left+1; // X�������ʼλ��

	if (gl_bTileWave) // ��ͨ��ƽ����ʾ
	{
		for (Channel=0; Channel<m_nChannelCount; Channel++)
		{
			ptOffset = &(pHistDoc->m_ADBuffer[m_Offset]); // ָ���ƫ����
			pointxy[Channel][0].x = StartX;
			DataY = (int)((ptOffset[Offset])/LsbOfPixel);
			pointxy[Channel][0].y = (int)(Center) - DataY;

			for (Index=0; Index<=m_nPlotWidth; Index++) // ��ʼ��1024����(����ʱ��λͼ�Ĵ�С) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				DataY = (int)(((((ptOffset[(Offset + Index) * m_nChannelCount + Channel])) - AD_LSB_HALF) / LsbOfPixel));
				pointxy[Channel][Index].y = (int)(Center) - DataY;
			}
			HeightMid[Channel] = Center; // ����ͨ���м�λ������
			Center += PerY;
		} 
	}
	else // ��ͨ��������ʾ
	{
		for (Channel=0; Channel<m_channel_cnt; Channel++)
		{
			ptOffset = &(pHistDoc->m_ADBuffer[m_Offset]); // ָ���ƫ����
			LsbOfPixel = (float)(((gl_ScreenVolume/AD_VOLT_RANGE)*AD_LSB_COUNT)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ    
			int StartX = m_rectPlot.left;
			Center = (int)(m_rectPlot.Height() / 2) + m_rectPlot.top;
			for (int Index=0; Index<=m_nPlotWidth; Index++)	
			{
				pointxy[Channel][Index].x = StartX + Index;	
				DataY = (int)(((((ptOffset[(Offset + Index) * m_channel_cnt + Channel])) - AD_LSB_HALF) / LsbOfPixel));
				pointxy[Channel][Index].y = (int)(Center) - DataY;
			}
		}
	}


	m_bDrawPoly = TRUE; 

}

//##############################################################################
//��������ĵ�
void CADHistScope::DrawPoly()
{	
	int DataY = 0;
	gl_bDataProcessing = TRUE;
	CPen* oldPen;
	int nDrawCount = m_rectPlot.Width() - 1;
	m_dcPlot.SetBkColor (m_crBackColor);
	m_dcPlot.FillRect(m_rectClient, &m_brushBack);
	m_dcPlot.SetTextColor(RGB(255, 158, 0));


	//---------------------------------------------------------------------------------
	if (m_bAllChannel || !gl_bTileWave) // ����ͨ����ʾ�������ʾʱ
	{
		for (int Channel = 0; Channel<m_channel_cnt; Channel++) // ������ͨ���ĵ�
		{
			oldPen = m_dcPlot.SelectObject(&m_penChannel[Channel]);
			//m_dcPlot.Polyline(pointxy[Channel], m_rectPlot.Width());
			m_dcPlot.Polyline(pointxy[Channel], nDrawCount);
		}
	}
	else // ��ͨ����ʾ
	{	
		WORD* ptOffset;

		m_dcPlot.SelectObject(&m_penChannel[m_nChannelNum]);
		int StartX = m_rectPlot.left;
		Center = (int)(m_nPlotHeight / 2) + m_rectPlot.top;
		ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
		for (int Index=0; Index<=m_nPlotWidth; Index++)	
		{
			pointTemp[Index].x = StartX + Index;
			pointTemp[Index].y = (int)(Center) - m_nCoordinateOneY[(showData[Drow_text_find_id(m_nChannelNum+1)-1][Index*SHOW_DATA_CNT/m_nPlotWidth]&MASK_MSB)-gl_MiddleLsb[Drow_text_find_id(m_nChannelNum+1)-1]];
		}
		m_dcPlot.Polyline(pointTemp, nDrawCount);
	}

	gl_bDataProcessing = FALSE;
	Invalidate();
}

void CADHistScope::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// 	m_nLineIndex = 0xFFFF;
	m_bOnlyLine = FALSE;
	m_bLBtnDown = FALSE; 
	::ReleaseCapture();

	static BOOL bFirstPosition = TRUE;  // ��ʼ��ı�־
	CADHistFrm* pFrame = (CADHistFrm*)GetParentFrame();
	CADHistDoc* pDoc = theApp.m_pADHistDoc;
	CButton* pRadioA = (CButton*)pFrame->m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionA);
	CButton* pRadioB = (CButton*)pFrame->m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionB);
	CStatic* pStaticA = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionA);
	CStatic* pStaticB = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionB);
	CStatic* pDistanceAB = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_DistanceAB);
	CStatic* pStaticFre = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_Frequency);
	CString str;
	long nFrequency;
	float fCalFrequency = 0;
	int nPositionA, nPositionB;
	int nCurPosition = pDoc->m_Offset + point.x - m_rectPlot.left + m_Offset;
	str.Format(_T("%d"), nCurPosition);
	pRadioA->SetCheck(m_nLineIndex == 1 ? 1:0);
	pRadioB->SetCheck(m_nLineIndex == 2 ? 1:0);

	if (pRadioA->GetCheck())
	{
		pStaticA->SetWindowText(str);
	}

	if (pRadioB->GetCheck())
	{
		pStaticB->SetWindowText(str);
	}

	int nDistanceAB = 0;
	nFrequency = pDoc->m_Header.ADPara.Frequency; // ���ļ�ͷ��ȡ�ò���Ƶ��

	pStaticA->GetWindowText(str);
	nPositionA = wcstol(str, NULL, 10);
	pStaticB->GetWindowText(str);
	nPositionB = wcstol(str, NULL, 10);
	nDistanceAB = abs(nPositionA - nPositionB);
	str.Format(_T("%d"), nDistanceAB);
	pDistanceAB->SetWindowText(str);
	if (nDistanceAB)
	{
		fCalFrequency = (float)((nFrequency)/nDistanceAB/pDoc->m_ChannelCount);
		if (fCalFrequency < 1000)
			str.Format(_T("%7.2f Hz"), fCalFrequency);
		if (fCalFrequency > 1000 && fCalFrequency < 1000000)
			str.Format(_T("%7.2f KHz"), fCalFrequency / 1000.00);
		if (fCalFrequency > 1000000)
			str.Format(_T("%7.2f MHz"), fCalFrequency / 1000000.00);		
		pStaticFre->SetWindowText(str);
	}
	bFirstPosition = !bFirstPosition;

	CWnd::OnLButtonUp(nFlags, point);
}

//#######################################################################
void CADHistScope::SetStatusBar()
{
	float fTimePixel;
	CString strTimeDiv;
	CString strCountTimer;
	CString strFrequency;
	CString strVol;
	int SubstValue = abs(m_OldMoveX1 - m_OldMoveX2);
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CADFrm* pADFrm = (CADFrm*)pApp->m_ADFrm;
	CEdit* pEditDiv = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv); 
	fTimePixel = (float)(1000000.0/ADPara.Frequency); // uS

	// ��������֮���ʱ���
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

	// ������ѹֵ��ѹֵ
	float LsbOfPixel = (float)((gl_ScreenVolume*1.0)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ    

	int Center = m_rectPlot.Height()/2 + m_rectPlot.top;
	int nOffset = Center - m_OldVoltageY;
	m_VolOffset = nOffset * LsbOfPixel;
	strVol.Format(_T("%7.2f"), m_VolOffset);
	CEdit* pEditVol = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Volt);
	pEditVol->SetWindowText(strVol);

	CEdit* pEditOffset = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Offset);
	CString strOffset;
	strOffset.Format(_T("%d"), m_MouseOffset - m_rectPlot.left + gl_Offset);
	if (m_MouseOffset >= m_rectPlot.left && m_MouseOffset <= m_rectPlot.right)
	{
		pEditOffset->SetWindowText(strOffset);
	}	
}

int CADHistScope::FindTrigger(int nChannel, int nValue) // ��ֹ���ζ���
{
	return 0;
}

//������ͨ����������
void CADHistScope::DrawAllChannelGrid(CDC* pDC)
{
	int X = 0, Channel = 0;
	int nGridPix = 0; // Y����������ߵĿ̶�
	float hight = (float)(m_rectPlot.Height() / m_channel_cnt); // ÿͨ����Y���

	for (Channel=1; Channel<m_channel_cnt; Channel++) // ��m_channel_cnt-1��������
	{
		nGridPix = m_rectPlot.top + (int)(m_rectPlot.Height() * Channel) / m_channel_cnt;
		for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=2) // ÿ��2�����ػ�1��
		{
			pDC->SetPixel(X, nGridPix, m_crGridColor); 
		}	
	}

	for (Channel=0; Channel<m_channel_cnt; Channel++) // ��ÿͨ��������
	{
		for (X=m_rectPlot.left; X<m_rectPlot.right; X+=3) // ÿ��3�����ػ�1��
		{
			pDC->SetPixel(X, (int)(m_rectPlot.top + (hight * Channel) + hight/2.0), RGB(255, 255, 255)); 
		}

	}
}

void CADHistScope::DrawSingleCHGrid(CDC* pDC)
{
	for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=2) // ÿ��4�����ػ�1��
	{
		pDC->SetPixel(X, m_rectPlot.top+(int)(m_rectPlot.Height()/2), RGB(255, 255, 255));	
	}
}


void CADHistScope::DrawAllChannelText(CDC* pDC)
{	
	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
	CADHistDoc* pDoc =(CADHistDoc*)pView->GetDocument();
	CString str;
	float hight = (float)(m_rectPlot.Height() / m_channel_cnt); // ÿͨ����Y���
	int Tchannel = 0;
	int signe_id;
	for (int Channel=0; Channel<m_channel_cnt; Channel++)
	{		
		pDC->SetTextColor(m_clPen[Channel]); // �������ֵ���ɫ

		//str.Format ("%.*lf V", m_nYDecimals, m_dUpperLimit[Channel]/1000.0); // ����ѹֵ
		str.Format (_T("%.*lf V"), m_nYDecimals, m_dUpperLimit[Channel]/1000.0); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+8), str); 

		pDC->SetTextAlign (TA_RIGHT|TA_BASELINE);
		str.Format (_T("%.*lf V"), m_nYDecimals, m_dLowerLimit[Channel]/1000.0); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*(Channel+1)-5), str);

		signe_id = Drow_text_find_id(Channel+1);
		if(signe_id > 4)
			str.Format(_T("Input %d"), signe_id-4);
		else
			str.Format(_T("Singal %d"), signe_id);
		//str.Format(_T("CH %d"), Channel + m_HistFirstChannel);
		pDC->TextOut(m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+hight/2+5), str);

	}	

}

void CADHistScope::DrawSingleCHText(CDC* pDC, int nChannelNum)
{
	CString strInfo;
	if (nChannelNum != 0xFFFF)
		pDC->SetTextColor(m_clPen[nChannelNum]);
	else
		pDC->SetTextColor(m_crGridColor);

	strInfo.Format (_T("%.*lf mV"), 0, m_dUpperLimit[nChannelNum]); // ����ѹֵ
	pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top, strInfo); 
	pDC->SetTextAlign (TA_RIGHT|TA_BASELINE);
	strInfo.Format (_T("%.*lf mV"), 0, m_dLowerLimit[nChannelNum]); // ����ѹֵ

	pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight, strInfo);

	if (nChannelNum == 0xFFFF) // ����ǵ�����ʾʱ
		strInfo = "CH";
	else
		strInfo.Format(_T("CH %d"), nChannelNum); 

	// дͨ����
	pDC->TextOut(m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight/2+5, strInfo);      

}

void CADHistScope::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR hCursor;
	m_bRBtnDown = TRUE;
	int DistanceSub = abs(point.x - m_OldMoveX1) + abs(point.x - m_OldMoveX2);
	int DistanceAB = abs(m_OldMoveX1 - m_OldMoveX2);
	if (DistanceSub == DistanceAB) // �������ʱ������������м�
	{
		m_nLineIndex = 0; // ͬʱѡ��������ʱ�ı�־ֵ
		m_nShowCursor = 4;
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		::SetCursor(hCursor);

		m_RBtnDownPosX = point.x;
		m_StartOldMoveX1 = m_OldMoveX1; // ���������ƶ�ʱ������1����ʼλ��
		m_StartOldMoveX2 = m_OldMoveX2; // ���������ƶ�ʱ������2����ʼλ��

		CRect ClipRect = m_rectPlot;
		ClientToScreen(&ClipRect);
		if (m_OldMoveX1 < m_OldMoveX2) // �����1����2�����
		{
			ClipRect.left += point.x - m_OldMoveX1;
			ClipRect.right -= m_OldMoveX2 - point.x;
		}
		else // �����1����2���ұ�
		{
			ClipRect.left += point.x - m_OldMoveX2;
			ClipRect.right -= m_OldMoveX1 - point.x;
		}
		::ClipCursor(&ClipRect);  
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CADHistScope::OnRButtonUp(UINT nFlags, CPoint point) 
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

void CADHistScope::DrawMoveLine(CDC* pDC, CPoint point)
{
	CPen* pOldPen;
	int nOldPositionX = point.x;
	int nOldPositionY = point.y;
	int nMode;
	pDC->SetBkMode(TRANSPARENT);         // ͸������
	nMode = pDC->SetROP2(R2_XORPEN);	 // ������ģʽ	
	switch(m_nLineIndex)
	{
	case 0: // ͬʱѡ����1��2
		pOldPen = pDC->SelectObject(&m_PenLineA); 
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // ������һ�λ�����A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);	// ����A

		WCHAR str[100];
		swprintf_s(str, _T("%s"), _T("A��"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(m_OldMoveX1, m_rectPlot.top, str);

		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B

		swprintf_s(str, _T("%s"), _T("A��"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(m_OldMoveX1, m_rectPlot.top, str);

		m_OldMoveX1 = m_StartOldMoveX1 + (point.x - m_RBtnDownPosX); 
		m_OldMoveX2 = m_StartOldMoveX2 + (point.x - m_RBtnDownPosX); 		
		pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top);
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);
		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top);
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 1: // ѡ�������1
		pOldPen = pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // ������һ�λ�����A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // ����A

		swprintf_s(str, _T("%s"), _T("A��"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(m_OldMoveX1, m_rectPlot.top, str);

		m_OldMoveX1 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 2: // ѡ�������2
		pOldPen = pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B

		swprintf_s(str, _T("%s"), _T("B��"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(m_OldMoveX2, m_rectPlot.top, str);

		m_OldMoveX2 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 3: // ѡ�������3
		pOldPen = pDC->SelectObject(&m_PenLineV);
		pDC->MoveTo(m_rectPlot.left,  m_OldVoltageY); // ������һ�λ�����V
		pDC->LineTo(m_rectPlot.right, m_OldVoltageY); // ����V
		m_OldVoltageY = point.y;
		pDC->MoveTo(m_rectPlot.left, point.y);
		pDC->LineTo(m_rectPlot.right, point.y);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;
	default:
		ASSERT(TRUE);
	}

	Invalidate();
	pDC->SetROP2(nMode);
}

BOOL CADHistScope::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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
