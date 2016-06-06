// OScopeCtrl.h : header file
//

#ifndef __ADScopeCtrl_H__
#define __ADScopeCtrl_H__
/////////////////////////////////////////////////////////////////////////////
class CADScopeCtrl : public CWnd
{
// Construction
public:
	CADScopeCtrl();
// Attributes
public:
	double AppendPoint(double dNewPoint);
	void SetRange(double dLower, double dUpper, int nChannel);
	void SetXUnits(CString string);
	void SetYUnits(CString string);
	void SetGridColor(COLORREF color);
	void SetPlotColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);
	void InvalidateCtrl();
	void Reset();
// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADScopeCtrl)
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, HMENU nID=NULL);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void DrawSingleCHText(CDC* pDC, int nChannelNum);
	void DrawAllChannelText(CDC* pDC);
	void DrawSingleCHGrid(CDC* pDC);
	void DrawAllChannelGrid(CDC* pDC);
	int FindTrigger(int nChannel, int nValue);
	void SetStatusBar();
	void AppendPoly(int BufferID, int  Offset);
	void UpdateChannelCount();
	int		m_nPtCount;
	int		m_nChannelCount;
	int		m_nChannelNum;
	int		PerY;
	int		m_BufferID;
	int		m_Offset;
	float	PixelOfLsb;
	int		Center;
	POINT	 pointxy[32][8192]; // ����ͨ����ʾ�ĵ�����
	int		 m_nCoordinateY[AD_LSB_HALF * 2];	// �ɼ������ݶ�Ӧ�Ļ�ͼ��Y����ֵ
	int		 m_nCoordinateOneY[AD_LSB_HALF * 2];	// ��ͨ����ʾ�ɼ������ݶ�Ӧ�Ļ�ͼ��Y����ֵ
	CPoint	 pointTemp[8192];  // ������ŵ�ͨ����ʾ�ĵ�����
	COLORREF m_clPen[USB2831_MAX_AD_CHANNELS]; 
	int		 HeightMid[16];
	BOOL     m_bRBtnMoveLeft;
	int      m_nRBtnMoveX;
	int		 m_MouseOffset;		 // ����ڻ�ͼ����ƫ��λ��
	int	     m_OldMoveX1;		 // �������ƶ����ߵ�λ��
	int		 m_OldMoveX2;
	int		 m_OldMoveY;
	int		 m_CurMoveX1;
	int		 m_CurMoveX2;
	int		 m_CurMoveY;
	int m_RBtnDownPosX;
	int m_StartOldMoveX1;
	int m_StartOldMoveX2;
    COLORREF m_crLineAColor;  // A�ߵ���ɫ
	COLORREF m_crLineBColor;  // B�ߵ���ɫ
	COLORREF m_crLineVColor;  // C�ߵ���ɫ 
	CPen     m_PenLineA;	  // A�ߵĻ���
	CPen     m_PenLineB;	  // B�ߵĻ���
	CPen     m_PenLineV;      // V�ߵĻ���
	HCURSOR  m_hCursorHSize;
	HCURSOR  m_hCursorVSize;
	HCURSOR  m_hCursorArrow;
	HCURSOR  m_hCursorAllSize;
	int      m_nShowCursor;
	int m_CurVoltageY;
	int m_OldVoltageY;
	float m_VolOffset;
	int m_nLineIndex;
	BOOL m_bAllChannel;
	
	int m_nShiftPixels;           
	int m_nYDecimals;
	
	CString m_strXUnitsString;
	CString m_strYUnitsString;
	
	COLORREF m_crBackColor;        
	COLORREF m_crGridColor;        
	COLORREF m_crPlotColor;        
	
	double m_dCurrentPosition;   
	double m_dPreviousPosition;  
	
	virtual ~CADScopeCtrl();
private:
	UINT m_nLoop;
	void DrawBkGnd(); // ��������
	void DrawPoint();
	void DrawPoly();
// 	void DrawPolyEx();
	void ProcessData();
// 	void ProcessDataEx();
	void TransitionData();

protected:
	void DrawMoveLine(CDC *pDC, CPoint point);
	BOOL m_bRBtnDown;
	CPen m_penChannel[USB2831_MAX_AD_CHANNELS];
	BOOL m_bOnlyLine;
	BOOL m_bLBtnDown;
	BOOL m_bAsynMode;
	//{{AFX_MSG(CADScopeCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
		int m_nHalfShiftPixels;
	int		m_nPlotShiftPixels;
	int		m_nClientHeight;
	int		m_nClientWidth;
	int		m_nPlotHeight;
	int		m_nPlotWidth;  // 
	BOOL	m_bDrawPoly;
	BOOL	m_bInitialed;
	double  m_dLowerLimit[USB2831_MAX_AD_CHANNELS];        // lower bounds
	double  m_dUpperLimit[USB2831_MAX_AD_CHANNELS];        // upper bounds
	double  m_dRange;
	double  m_dVerticalFactor;
	
	CRect   m_rectClient;
	CRect   m_rectPlot;
	CPen    m_penPlot;
	CBrush  m_brushBack;
	CBrush  m_brushWhite;
	
	CDC     m_dcGrid;
	CDC     m_dcPlot;
	CDC	    m_dcMove;
	CBitmap* m_pbitmapOldGrid;
	CBitmap* m_pbitmapOldPlot;
	CBitmap m_bitmapGrid;
	CBitmap m_bitmapPlot;
	
	
};

/////////////////////////////////////////////////////////////////////////////
#endif
