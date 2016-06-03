// ADFrm.cpp : implementation of the CADFrm class
//

#include "stdafx.h"
#include "Sys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// CADFrm

IMPLEMENT_DYNCREATE(CADFrm, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CADFrm, CMDIChildWnd)
ON_COMMAND_EX(CG_ID_VIEW_DLGBAR, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DLGBAR, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS1, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS1, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS, OnUpdateControlBarMenu)
ON_WM_CREATE()
ON_COMMAND_EX(CG_ID_VIEW_SETUPDLG, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SETUPDLG, OnUpdateControlBarMenu)
//{{AFX_MSG_MAP(CADFrm)
ON_WM_CLOSE()
ON_WM_CANCELMODE()
ON_WM_LBUTTONUP()
ON_COMMAND(IDM_MoveVolt, OnMoveVolt)
ON_WM_DESTROY()
ON_WM_CREATE()
ON_UPDATE_COMMAND_UI(IDM_MoveVolt, OnUpdateMoveVolt)
ON_COMMAND(IDM_ShowCfgAnalBar, OnShowCfgAnalBar)
ON_UPDATE_COMMAND_UI(IDM_ShowCfgAnalBar, OnUpdateShowCfgAnalBar)
ON_COMMAND(IDM_ShowStatusBar, OnShowStatusBar)
ON_UPDATE_COMMAND_UI(IDM_ShowStatusBar, OnUpdateShowStatusBar)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADFrm construction/destruction

CADFrm::CADFrm()
{
	m_bShowCfgAnalBar = true;
	m_bShowStatusBar = true;
	gl_ProcessMoveVolt = 1;
	gl_ReadSizeWords = 0;
	theApp.m_ADFrm = this;
}

CADFrm::~CADFrm()
{
	theApp.m_ADFrm = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CADFrm diagnostics

#ifdef _DEBUG
void CADFrm::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CADFrm::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADFrm message handlers

BOOL CADFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2))  // 创建1行2列分割
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	// 创建第一个格子，在0列中使用默认的视图（由文档模板决定）
	
	if (!m_wndSplitter.CreateView(0, 0, 
		RUNTIME_CLASS(CADDigitView), CSize(365, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 
	
	if (!m_wndSplitter.CreateView(0, 1, 
		RUNTIME_CLASS(CADWaveView), CSize(0, 470), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	return TRUE;	
}


void CADFrm::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED; // 使子帧窗口最大化
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

int CADFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 创建右侧CControlBar窗体
	m_wndSTCBar.Create(this, CSize(200, 300), IDW_PROP_BAR);
	m_wndSTCBar.SetBarStyle(CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	m_wndSTCBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndSTCBar, AFX_IDW_DOCKBAR_RIGHT);
	
	gl_pParaCfgView = (CADParaCfgView*)m_wndSTCBar.AddView(_T("参数配置"), RUNTIME_CLASS(CADParaCfgView)); // CParaCfgView
	gl_pParaCfgView->m_pADFrm = this;
	
	gl_pADStatusView = (CADStatusView*)m_wndSTCBar.AddView(_T("性能分析"), RUNTIME_CLASS(CADStatusView)); // CADStatusView
	
	USB2831_LoadParaAD(NULL, &ADPara);  // 读入系统参数
	gl_nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1;
	
	
	// Initialize dialog bar m_wndShowStatus
	if (!m_wndShowStatus.Create(this, CG_IDD_Status,  
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE, 
		CG_IDD_Status))
	{
		TRACE0("Failed to create dialog bar m_wndShowStatus\n");
		return -1;		// fail to create
	}
	
	m_wndShowStatus.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndShowStatus);
	
	CProgressCtrl* pProgress = (CProgressCtrl*)m_wndShowStatus.GetDlgItem(IDC_PROGRESS);
	pProgress->SetRange(0, 50);
	
	if (!m_wndDlgSave.Create(this, CG_IDD_DIALOG_Save, 
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE, 
		CG_IDD_DIALOG_Save))
	{
		TRACE0("Failed to create dialog bar m_wndShowStatus\n");
		return -1;		// fail to create
	}
	m_wndDlgSave.ShowWindow(SW_HIDE);
	m_wndDlgSave.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndDlgSave);
	
 	CEdit* pFreqEdit = (CEdit*)(this->m_wndShowStatus).GetDlgItem(IDC_EDIT_MeasureFreq); 
 	pFreqEdit->SetWindowText(_T("拖鼠标测频"));
	
	//m_ToolTip.Create(this, TTS_ALWAYSTIP|WS_CHILD|WS_VISIBLE);
	CComboBox* pFrequency = (CComboBox *)gl_pParaCfgView->GetDlgItem(IDC_COMBO_Frequency);
	//m_ToolTip.AddTool(pFrequency, _T("内部触发或外部触发"));

	CComboBox* pTriggerDir = (CComboBox *)gl_pParaCfgView->GetDlgItem(IDC_COMBO_OutTriggerDir);
	//m_ToolTip.AddTool(pTriggerDir, _T("触发方向"));

	CEdit* pBufferID =(CEdit *)gl_pParaCfgView->GetDlgItem(IDC_EDIT_BufferID);
	//m_ToolTip.AddTool(pBufferID, _T("在64个8K缓冲区中选择缓冲区"));

	CSliderCtrl* pSLIDER_Offset = (CSliderCtrl*)gl_pParaCfgView->GetDlgItem(IDC_SLIDER_Offset);
	//m_ToolTip.AddTool(pSLIDER_Offset, _T("在8K缓冲区中选择偏移"));
	
	return 0;
}


void CADFrm::OnClose() 
{
	CMDIChildWnd::OnClose();
}


void CADFrm::OnCancelMode() 
{
	CMDIChildWnd::OnCancelMode();	
}

void CADFrm::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}

void CADFrm::OnMoveVolt() // 平移电压的函数
{
	SHORT m_DataMiddle; // 中间数原码
	ULONG nArrayCount = -1;
	ULONG m = 0;
	WCHAR str[256];
	int Channel = 0;
	CSysApp* pApp = (CSysApp *)AfxGetApp();
	CADDoc* pDoc = pApp->m_pADDoc; 
	CString strn;
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	if (gl_ProcessMoveVolt == 1) // 开始求中间值
	{
		int iChIndex = 0;
		for(Channel=0; Channel<gl_nChannelCount; Channel++)
		{
			iChIndex = ADPara.FirstChannel + Channel;
			for(ULONG Index=0; Index<gl_ReadSizeWords/gl_nChannelCount; Index++)
			{
				DataAnalyze[iChIndex][Index] = ((ADBuffer[gl_nDrawIndex][Index*gl_nChannelCount + Channel])&MASK_MSB); 
			}
		}
		
		gl_pADStatusView->m_Combo_MoveVolt.DeleteString(0);
		/////////////////////////////////////////////			
		for(Channel=0; Channel<gl_nChannelCount; Channel++) // 每通道的数据各自进行排序
		{
			iChIndex = ADPara.FirstChannel + Channel;
			m_QSortData.Sort(DataAnalyze[iChIndex], 0, gl_ReadSizeWords/gl_nChannelCount-1);
			m_DataMiddle = m_QSortData.GetMidData(); // 取得中间数		
			// m_DataMiddle是非偏移码，即原码，即0-LSB_COUNT中的任意值
			// gl_MiddleLsb 是偏移码值，即原码减去LSB_HALF后的值, 即在原点上下浮动的值
			// gl_MiddleLsb[k]=(int)((m_DataMiddle)-LSB_HALF);
			gl_MiddleLsb[Channel]=(int)(m_DataMiddle - AD_LSB_HALF);

			switch(gl_DigitShowMode)
			{
			case SHOW_MODE_DEC: // 十进制原码
				swprintf_s(str, _T("CH%d %d Dec"), Channel, (int)m_DataMiddle);
				break;
			case SHOW_MODE_HEX: // 十六进制原码
				swprintf_s(str, _T("CH%d %x Hec"), Channel, m_DataMiddle);
				break;
			case SHOW_MODE_VOLT: // 电压值
				if ( ADPara.InputRange==0
					|| ADPara.InputRange==1
					|| ADPara.InputRange==2 )
				{
					swprintf_s(str, _T("CH%d %5.2fmV"), Channel, (m_DataMiddle - AD_LSB_HALF)*gl_PerLsbVolt[iChIndex]);
				}
				else
				{
					swprintf_s(str, _T("CH%d %5.2fmV"), Channel, m_DataMiddle*gl_PerLsbVolt[iChIndex]);
				}
				break;
			}

			gl_pADStatusView->m_Combo_MoveVolt.AddString(str); // 向组合框中添加平移电压值
			gl_AnalyzeAllCount = 0;
			gl_OverLimitCount = 0;
		}
		gl_pADStatusView->m_Combo_MoveVolt.SetCurSel(0);
		gl_ProcessMoveVolt = 0;
	}	
	else
	{
		for(Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++)
		{
			gl_MiddleLsb[Channel] = 0; // 如果不偏移, 置偏移值为0
		}
		
		while (gl_pADStatusView->m_Combo_MoveVolt.GetCount()>0)
			gl_pADStatusView->m_Combo_MoveVolt.DeleteString(0);
		
		CString StrMoveVolt;
		StrMoveVolt = "没有平移";
		gl_pADStatusView->m_Combo_MoveVolt.AddString (StrMoveVolt);
		gl_pADStatusView->m_Combo_MoveVolt.SetCurSel(0);
		gl_ProcessMoveVolt = 1;
	}
	
	if (gl_bDeviceADRun == FALSE) // 如果设备没有运行，即静态分析
	{
		pDigitView->TransitionData();
		pDigitView->RedrawWindow();
		pWaveView->OnDrawPolyLine();
		return;
	}
	
	if (gl_nProcMode == PROC_MODE_DIGIT)  // 如果是数字处理方式，则只更新数字窗口
	{
		pDigitView->TransitionData();
		pDigitView->RedrawWindow();
	}
	
	if (gl_nProcMode == PROC_MODE_WAVE)  // 如果是波形处理方式，则只更新波形窗口
	{
		//pWaveView->OnDrawPolyLine();	
		pWaveView->Invalidate();
	}	
}

void CADFrm::OnUpdateMoveVolt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!gl_ProcessMoveVolt);	
}

void CADFrm::OnDestroy() 
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	if (gl_bDeviceADRun)
	{
		pApp->m_pADDoc->StopDeviceAD();
	}
	USB2831_SaveParaAD(NULL, &ADPara);
	pApp->WriteProfileInt(_T("strSectionFre"),_T("DefaultFreq"),ADPara.Frequency);
	if (gl_bDeviceADRun) // 检查是否设备
	{
		CADDoc* pDoc = (CADDoc*)GetActiveDocument();
		pDoc->Public_ReleaseDevice();
	}    
	pApp->m_ADFrm = NULL;
//	theApp.m_BalloonHelp.DestroyBalloon();
//	theApp.m_BalloonHelp.~CBalloonHelp();
	CMDIChildWnd::OnDestroy();	
}

void CADFrm::OnShowCfgAnalBar() 
{
	// TODO: Add your command handler code here
	m_bShowCfgAnalBar =  !m_bShowCfgAnalBar;
	ShowControlBar(&m_wndSTCBar, m_bShowCfgAnalBar, false);
}

void CADFrm::OnUpdateShowCfgAnalBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowCfgAnalBar);
	
}

void CADFrm::OnShowStatusBar() 
{
	// TODO: Add your command handler code here
	m_bShowStatusBar = !m_bShowStatusBar;
	ShowControlBar(&m_wndShowStatus, m_bShowStatusBar, false);	
}

void CADFrm::OnUpdateShowStatusBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowStatusBar);
}

BOOL CADFrm::PreTranslateMessage(MSG* pMsg) 
{
	//m_ToolTip.RelayEvent(pMsg);		
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

