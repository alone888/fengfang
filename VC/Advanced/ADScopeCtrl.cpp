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
	
	m_crLineAColor = RGB(255, 192, 255);  // A�ߵ���ɫ
    m_crLineBColor = RGB(192, 255, 255);  // B�ߵ���ɫ
    m_crLineVColor = RGB(255, 255, 128);  // C�ߵ���ɫ
	
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
	m_nShowCursor = 0; // ��ʾ���
	

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
//���ô�ֱ�����������̵Ĵ�С
void CADScopeCtrl::SetRange(double dLower, double dUpper, int nChannel)
{
	ASSERT(dUpper > dLower);
	m_dLowerLimit[nChannel]  = dLower;
	m_dUpperLimit[nChannel]  = dUpper;
	m_nYDecimals = 2;    //  С����λ��
	m_dRange = (double)(m_dUpperLimit - m_dLowerLimit);
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange;
	InvalidateCtrl();
} // SetRange

///////////////////////////////////////////////////////////////////////////
//����timer
void CADScopeCtrl::StartTimer()
{
	SetTimer(TIMERID,50,0);  //������൱���趨��timer,���Ҫͣ��timer����KillTimer(TIMERID)
	
}  

void CADScopeCtrl::OnTimer(UINT_PTR nIDEvent)

{
	DrawBkGnd();  // ������
	DrawPoly(); // ����
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
		m_PenLineA.CreatePen(PS_DASHDOT, 1, m_crLineAColor); // ����A�ߵĻ���
	if (m_PenLineB.m_hObject == NULL)
		m_PenLineB.CreatePen(PS_DASHDOT, 1, m_crLineBColor); // ����B�ߵĻ���
	if (m_PenLineV.m_hObject == NULL)
		m_PenLineV.CreatePen(PS_DASHDOT, 1, m_crLineVColor); // ����V�ߵĻ���

	int Channel = 0;
	for (Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++)
	{
		if (m_penChannel[Channel].m_hObject == NULL)
			m_penChannel[Channel].CreatePen(PS_SOLID, 1, RGB(0,0,255));
	}
	

		//gl_nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1;
		m_nChannelCount = gl_nChannelCount;

	//ProcessData(); // ��������
	//DrawBkGnd();  // ������
	//TransitionData();	// ��ԭ��ת��Ϊ��Ļ��ͼY����
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
	// �����m_nClientWidth, m_ClientHeightҲ��ȡ����m_rectClient�Ĵ�С
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
	dc.SetBkMode(TRANSPARENT);		                // ���ñ���Ϊ͸��
	nDrawMode = dc.SetROP2(R2_XORPEN);              // ���û���ģʽ
	
	dc.SelectObject(&m_PenLineA);                   // ѡ����A�Ļ���
	dc.MoveTo(m_OldMoveX1, m_rectPlot.top);
	dc.LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // ����A	
	
	dc.SelectObject(&m_PenLineB);				    // ѡ������B�Ļ���
	dc.MoveTo(m_OldMoveX2, m_rectPlot.top);
	dc.LineTo(m_OldMoveX2, m_rectPlot.Height()+10); // ����B	
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
		case USB2831_INPUT_N10000_P10000mV: // ��10000mV
			m_dUpperLimit[Channel] = 10000.00; //??* ���½���
			m_dLowerLimit[Channel] = -10000.00;
			break;
		case USB2831_INPUT_N5000_P5000mV: // ��5000mV
			m_dUpperLimit[Channel] = 5000.00;
			m_dLowerLimit[Channel] = -5000.00;
			break;
		case USB2831_INPUT_N2500_P2500mV: // ��2500mV
			m_dUpperLimit[Channel] = 2500.00;
			m_dLowerLimit[Channel] = -2500.00;
			break;
		case USB2831_INPUT_0_P10000mV: // 0��10000mV
			m_dUpperLimit[Channel] = 10000.00;
			m_dLowerLimit[Channel] = 0.00;
			break;
		default:
			ASSERT(FALSE);
		}	
	}
	
	//SetBackgroundColor(RGB(255,  255,   255));
	SetGridColor(RGB(0,  0,   0));//���ÿ����ɫ
	//SetPlotColor(RGB(0,  255,   255));
	return 0;
}

//########################################################################
//����������������
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
	// ���������ִ���ռ�õ��ַ����
	nCharacters = abs((int)log10(fabs(m_dUpperLimit[0])));
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit[0]))));
	nCharacters = nCharacters + 4 + m_nYDecimals;  
	
	PerY = (int)(m_nPlotHeight / m_nChannelCount); // ÿͨ����Y���
	//-----------------	-----------------------------------------------------------------
	// �����ܵĿ��	
	oldPen = m_dcGrid.SelectObject (&solidPen);  // �����ܵķ���
	m_dcGrid.MoveTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.SelectObject (oldPen); //WYL ����������ɫ 
	//COLORREF m_Grid = RGB(200, 200, 200);

	COLORREF m_Grid = RGB(255, 0, 0);//
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
// 	InvalidateRect(m_rectClient); // ˢ������
}

//////////////////////////////////////////////////////////////////////////
//��PolyLine��
void CADScopeCtrl::OnMouseMove(UINT nFlags, CPoint point)
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
	m_MouseOffset = point.x;	
	SetStatusBar(); // ����״̬����Ƶ�ʵ�
	CWnd::OnMouseMove(nFlags, point);
}

void CADScopeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CString str;
	if (gl_bTileWave && m_rectPlot.PtInRect(point)) // �������ڱ߿���ʱ
	{
		// ͨ���ж����˫��ʱ��λ�ã���������ʾ��һ·��ͨ��
		if (m_bAllChannel) 
		{
			//int TChannel = 0;
			Center = (int)(PerY/2.0)+m_rectPlot.top;
			for (int Channel=0; Channel<m_nChannelCount; Channel++) //�ж����˫��λ��
			{
				HeightMid[Channel] = Center; // ����ͨ���м�λ������
				if (abs(point.y - HeightMid[Channel]) < m_rectPlot.Height()/(2*m_nChannelCount))
				{
					m_nChannelNum = Channel+ADPara.FirstChannel; // ȷ��˫����ĵ�ͨ����
					str.Format(_T("%d"), Channel);
					SetYUnits(_T("��ͨ��-")+str);
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
void CADScopeCtrl::AppendPoly(int BufferID, int  Offset)
{
	m_BufferID = BufferID; // �λ���ID
	m_Offset = Offset;     // ����ƫ��
	TransitionData();	// ��ԭ��ת��Ϊ��Ļ��ͼY����
	ProcessData(); // ��������
	DrawBkGnd();  // ������
	DrawPoly(); // ����
}

//###############################################################################
//��������, �ѻ����е�����ת���ɿ�����ʾ�ĵ�����
void CADScopeCtrl::TransitionData()
{
	float LsbOfPixel, LsbOfPixelOne;
	float fScreenVolume = AD_VOLT_RANGE; // ��ͨ����ʾʱ����Ļ�������Ǻ�����ѹֵһ�µ�
	LsbOfPixel = (float)(((gl_voltVolume/gl_voltVolume)*AD_LSB_COUNT)/(PerY )); // ÿ���ض�Ӧ����ֵ    
	LsbOfPixelOne = (float)(((gl_ScreenVolume*1.0/(gl_voltVolume))*AD_LSB_COUNT)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ
	for (int Index=0; Index < (AD_LSB_HALF * 2); Index++) // ��ԭ��ת��Ϊ��Ļ��ͼY����
	{
		m_nCoordinateY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixel);
		m_nCoordinateOneY[Index] = (int)((Index - AD_LSB_HALF) / LsbOfPixelOne);
	}
}

//��������, �ѻ����е�����ת���ɿ�����ʾ�ĵ�����
void CADScopeCtrl::ProcessData()
{
	UpdateChannelCount();
	Center = (int)(PerY/2.0)+m_rectPlot.top;
	int Channel, Index, StartX;
	PWORD  ptOffset; // ����ָ��
	int Offset = 0, DataY = 0;
	StartX = m_rectPlot.left+1; // X�������ʼλ��
	
	if (gl_bTileWave) // ��ͨ��ƽ����ʾ
	{
		for (Channel=0; Channel<m_nChannelCount; Channel++)
		{
			ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
			pointxy[Channel][0].x = StartX;
			pointxy[Channel][0].y = m_nCoordinateY[ptOffset[Offset]&MASK_MSB];
			
			for (Index=0; Index<=4096/m_nChannelCount; Index++) // ��ʼ��1024����(����ʱ��λͼ�Ĵ�С) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				pointxy[Channel][Index].y = (int)(Center) - m_nCoordinateY[ptOffset[(Offset+Index) * m_nChannelCount + Channel]&MASK_MSB];
			}
			HeightMid[Channel] = Center; // ����ͨ���м�λ������
			Center += PerY;
		}
	}
	else // ��ͨ��������ʾ
	{
		ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
		float LsbOfPixel = (float)(AD_LSB_COUNT/m_rectPlot.Height()); // ÿ���ض�Ӧ����ֵ
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
//��������, �ѻ����е�����ת���ɿ�����ʾ�ĵ�����
void CADScopeCtrl::ProcessData2()
{
	int screenWidth;
	
	UpdateChannelCount();
	Center = (int)(PerY/2.0)+m_rectPlot.top;

	//screenWidth = m_rectPlot.Width


	int Channel, Index, StartX;
	PWORD  ptOffset; // ����ָ��
	int Offset = 0, DataY = 0;
	StartX = m_rectPlot.left+1; // X�������ʼλ��
	
	if (gl_bTileWave) // ��ͨ��ƽ����ʾ
	{
		for (Channel=0; Channel<m_nChannelCount; Channel++)
		{
			ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
			pointxy[Channel][0].x = StartX;
			pointxy[Channel][0].y = m_nCoordinateY[ptOffset[Offset]&MASK_MSB];
			
			for (Index=0; Index<=screenWidth; Index++) // ��ʼ��1024����(����ʱ��λͼ�Ĵ�С) 
			{
				pointxy[Channel][Index].x = StartX + Index;
				pointxy[Channel][Index].y = (int)(Center) - m_nCoordinateY[ptOffset[(Offset+Index) * m_nChannelCount + Channel]&MASK_MSB];
			}
			HeightMid[Channel] = Center; // ����ͨ���м�λ������
			Center += PerY;
		}
	}
	else // ��ͨ��������ʾ
	{
		ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
		float LsbOfPixel = (float)(AD_LSB_COUNT/m_rectPlot.Height()); // ÿ���ض�Ӧ����ֵ
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
//��������ĵ�
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
	if (m_bAllChannel || !gl_bTileWave) // ����ͨ����ʾ�������ʾʱ
	{
		for (int Channel = 0; Channel<m_nChannelCount; Channel++) // ������ͨ���ĵ�
		{
			oldPen = m_dcGrid.SelectObject(&m_penChannel[Channel]);
			m_dcGrid.Polyline(pointxy[Channel], nDrawCount);
		}
	}
	else // ��ͨ����ʾ
	{
		m_dcGrid.SelectObject(&m_penChannel[m_nChannelNum]);
		int StartX = m_rectPlot.left;
		Center = (int)(m_nPlotHeight / 2) + m_rectPlot.top;
		WORD* ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
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
//��������ĵ�
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

	for (int Channel = 0; Channel<m_nChannelCount; Channel++) // ������ͨ���ĵ�
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
	if (m_bAllChannel || !gl_bTileWave) // ����ͨ����ʾ�������ʾʱ
	{
		for (int Channel = 0; Channel<m_nChannelCount; Channel++) // ������ͨ���ĵ�
		{
			oldPen = m_dcGrid.SelectObject(&m_penChannel[Channel]);
			m_dcGrid.Polyline(pointxy[Channel], nDrawCount);
		}
	}
	else // ��ͨ����ʾ
	{
		m_dcGrid.SelectObject(&m_penChannel[m_nChannelNum]);
		int StartX = m_rectPlot.left;
		Center = (int)(m_nPlotHeight / 2) + m_rectPlot.top;
		WORD* ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
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
	if (ADPara.ADMode==USB2831_ADMODE_SEQUENCE)// �����ɼ�
	{
		fTimePixel = (float)(1000000.0/ADPara.Frequency*gl_nChannelCount); // uS
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
		
		// ����Ƶ�ʵĴ�С
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
		
		// ��������֮���ʱ���
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
		
		// ����Ƶ�ʵĴ�С
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
// 	// ��������֮���ʱ���
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
// 	// ����Ƶ�ʵĴ�С
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

	// ������ѹֵ
	float LsbOfPixel = (float)((gl_ScreenVolume*1.0)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ
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

int CADScopeCtrl::FindTrigger(int nChannel, int nValue) // ��ֹ���ζ���
{
	return 0;
}

//������ͨ����������
void CADScopeCtrl::DrawAllChannelGrid(CDC* pDC)
{
	int X = 0, Channel = 0;
	int nGridPix = 0; // Y����������ߵĿ̶�
	float hight = (float)(m_rectPlot.Height() / m_nChannelCount); // ÿͨ����Y���
	
	for (Channel=1; Channel<m_nChannelCount; Channel++) // ��m_nChannelCount-1��������
	{
		nGridPix = m_rectPlot.top + (int)(m_rectPlot.Height() * Channel) / m_nChannelCount;
		for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=1) // ÿ��2�����ػ�1��
		{
			pDC->SetPixel(X, nGridPix, m_crGridColor);
			//pDC->SetPixel(X, nGridPix+1, m_crGridColor);
		}	
	}

	for (Channel=0; Channel<m_nChannelCount; Channel++) // ��ÿͨ��������
	{
		for (X=m_rectPlot.left; X<m_rectPlot.right; X+=3) // ÿ��3�����ػ�1��
		{
			pDC->SetPixel(X, (int)(m_rectPlot.top + (hight * Channel) + hight/2.0), RGB(0, 0, 0)); 
		}
		
	}
}

void CADScopeCtrl::DrawSingleCHGrid(CDC* pDC)
{
	//for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=2) // ÿ��4�����ػ�1��
	//{
	//	pDC->SetPixel(X, m_rectPlot.top+(int)(m_rectPlot.Height()/2), RGB(0, 0, 0));	
	//}
}


void CADScopeCtrl::DrawAllChannelText(CDC* pDC)
{
	CString str;
	float hight = (float)(m_rectPlot.Height() / m_nChannelCount); // ÿͨ����Y���
	for (int Channel = 0; Channel <gl_nChannelCount; Channel++)
	{
		COLORREF tempPen =  RGB(0, 0, 0);
		//m_dcGrid.SetTextColor(m_clPen[Channel+ADPara.FirstChannel]); // �������ֵ���ɫ
		m_dcGrid.SetTextColor(tempPen); // �������ֵ���ɫ
		if(m_nChannelCount<=10)
		{
			str.Format (_T("%.*lf V"), m_nYDecimals, m_dUpperLimit[Channel]/1000.0); // ����ѹֵ
			m_dcGrid.TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+8), str); 
			
			m_dcGrid.SetTextAlign (TA_RIGHT|TA_BASELINE);
			str.Format (_T("%.*lf V"), m_nYDecimals, m_dLowerLimit[Channel]/1000.0); // ����ѹֵ
			m_dcGrid.TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*(Channel+1)-5), str);
			
			CFont font;
			CFont *oldfont;

			font.CreatePointFont(100,_T("����")); //��������1�������С��2�������ͣ�3��
			oldfont = m_dcGrid.SelectObject(&font);
			m_dcGrid.SetTextColor(RGB(0,0,255));//������ɫRGB   
				str.Format(_T("CH %d"), Channel+ADPara.FirstChannel);
				m_dcGrid.TextOut(m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Channel+hight/2+5), str);
			m_dcGrid.SetTextColor(RGB(0,0,0));//������ɫRGB  
			m_dcGrid.SelectObject(oldfont);
			DeleteObject(font);//�ͷ���Դ
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

void CADScopeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR hCursor;
	m_bRBtnDown = TRUE;
	int DistanceSub = abs(point.x - m_OldMoveX1) + abs(point.x - m_OldMoveX2);
	int DistanceAB = abs(m_OldMoveX1 - m_OldMoveX2);
	if (DistanceSub == DistanceAB) // �������ʱ������������м�
	{
		m_nLineIndex = 0; // ͬʱѡ��������ʱ�ı�־ֵ
		m_nShowCursor = 4; // ???
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
	pDC->SetBkMode(TRANSPARENT);         // ͸������
	nMode = pDC->SetROP2(R2_XORPEN);	 // ������ģʽ	
	switch(m_nLineIndex)
	{
	case 0: // ͬʱѡ����1��2
		pOldPen = pDC->SelectObject(&m_PenLineA); 
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // ������һ�λ�����A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);	// ����A
		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B		
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
		m_OldMoveX1 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 2: // ѡ�������2
		pOldPen = pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B
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
// 	if(!gl_bTileWave)  // ����ǵ�����ʾ
// 	{
// 		m_nChannelCount = 1;
// 	}
// 	else // �����ƽ����ʾ
// 	{
		m_nChannelCount = gl_nChannelCount;		
// 	}

}
void CADScopeCtrl::ProcessOrgAdData()
{
	double fTimePoint1,fTimePoint2;//
	int rang = 100*1000;
	int i,j;

	fTimePoint1 =  rang/10000;  //showData ÿ��������ʱ�� ��������ʱ��ֵ�ڼ���
	fTimePoint2= 8/ADPara.Frequency;//�¶���������100��

	static int lastShowDateID;//�����ϴδ浽��λ��



	if(fTimePoint1<fTimePoint2)
	{

		for(i=lastShowDateID;i<10000;i++)
		{
			if( i* rang/10000<j* 8/ADPara.Frequency)
			{
				for (int Channel = 0; Channel<8; Channel++) // ������ͨ���ĵ�
				{
					showData[Channel][i]= AD_OrgData[j*8 + Channel]&MASK_MSB; //�ȴ�ԭʼ���ݣ��Ȳ�������ֵ��
				}			
			}
			else
			{
				j++;
				if(j>=100)
				{
					break;
				}
				i--;//j���� ����i��β��㣬������
			}

		}
	
	
		//�������10000�� j��������100����û���꣬�����ٴ�i=0��ʼ
		//�ж���λ᲻�ᵽ��10000����Ľ�β
		if(j<100)
		{
			for(i=0;i<10000;i++)
			{
				if(i* rang/10000<j* 8/ADPara.Frequency)
				{
					for(int Channel = 0; Channel<8; Channel++) // ������ͨ���ĵ�
					{
						showData[Channel][i]= AD_OrgData[j*8 + Channel]&MASK_MSB; //�ȴ�ԭʼ���ݣ��Ȳ�������ֵ��
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
					i--;//i��β��㣬������
				}

			}
		
		}
	}
}

