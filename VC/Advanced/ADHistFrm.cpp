// HistDataFrm.cpp : implementation file

#include "stdafx.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm

IMPLEMENT_DYNCREATE(CADHistFrm, CMDIChildWnd)

CADHistFrm::CADHistFrm()
{
	m_bStart = FALSE;
	m_ShowMode = 2;  // ���λطţ�1Ϊ���ֻطţ�
}

CADHistFrm::~CADHistFrm()
{
}

BEGIN_MESSAGE_MAP(CADHistFrm, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND_EX(CG_ID_VIEW_DIALOGBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DIALOGBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CADHistFrm)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(IDM_History_Start, OnHistoryStart)
	ON_UPDATE_COMMAND_UI(IDM_History_Start, OnUpdateHistoryStart)
	ON_COMMAND(IDM_History_Stop, OnHistoryStop)
	ON_UPDATE_COMMAND_UI(IDM_History_Stop, OnUpdateHistoryStop)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PositionB, OnPositionB)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ScreenCur, OnCustomdrawSLIDERScreenCur)
	ON_CBN_SELCHANGE(IDC_COMBO_Rate, OnSelchangeCOMBORate)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_RefrushData, OnBUTTONRefrushData)
	ON_BN_CLICKED(IDC_BUTTON_PositionA, OnBUTTONPositionA)
	
	ON_BN_CLICKED(IDC_BUTTON_PositionB, OnBUTTONPositionB)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_CHECK_INPUT9_P, OnBnClickedCheckInput9)
	ON_BN_CLICKED(IDC_CHECK_INPUT10_P, OnBnClickedCheckInput10)
	ON_BN_CLICKED(IDC_CHECK_INPUT11_P, OnBnClickedCheckInput11)
	ON_BN_CLICKED(IDC_CHECK_INPUT12_P, OnBnClickedCheckInput12)
	ON_BN_CLICKED(IDC_CHECK_INPUT13_P, OnBnClickedCheckInput13)
	ON_BN_CLICKED(IDC_CHECK_INPUT14_P, OnBnClickedCheckInput14)
	ON_BN_CLICKED(IDC_CHECK_INPUT15_P, OnBnClickedCheckInput15)
	ON_BN_CLICKED(IDC_CHECK_INPUT16_P, OnBnClickedCheckInput16)
	ON_BN_CLICKED(IDC_BUTTON_TIME_L, OnBnClickedButton1TimeL)
	ON_BN_CLICKED(IDC_BUTTON_TIME_R, OnBnClickedButton1TimeR)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedCheckInput6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedCheckInput7)
	ON_BN_CLICKED(IDC_EXPORT_EXCEL,OnBnClickedCheckexportexcel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm message handlers

BOOL CADHistFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2))  // ����1��2�зָ�
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	// ������һ�����ӣ���0����ʹ��Ĭ�ϵ���ͼ�����ĵ�ģ�����, CADHistDigitView��
	if (!m_wndSplitter.CreateView(0, 0, 
		pContext->m_pNewViewClass, CSize(/*450*/0, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 
	
	if (!m_wndSplitter.CreateView(0, 1,  
			RUNTIME_CLASS(CADHistWaveView), CSize(350, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 	
    m_wndSplitter.EnableWindow(FALSE); // ��ֹ���ڱ仯
	return TRUE;
	// return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

int CADHistFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize dialog bar m_wndSetupBar
	if (!m_wndSetupBar.Create(this, CG_IDD_AD_HIST_DLG, 
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE, 
		CG_ID_VIEW_DIALOGBAR))
	{
		TRACE0("Failed to create dialog bar m_wndSetupBar\n");
		return -1;		// fail to create
	}
	
	m_wndSetupBar.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndSetupBar);
	
	CButton* pRadioA = (CButton*)m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionA);
	CButton* pRadioB = (CButton*)m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionB);
	pRadioA->SetCheck(1);
	pRadioB->SetCheck(0);

	if(hist_m_channel_cnt <=0 || hist_m_channel_cnt > 8)
		m_channel_cnt = 8;
	else
		m_channel_cnt = hist_m_channel_cnt;
	for (int i = 0; i < 8 ; i++)
	{
		if(m_channel_cnt == 8)
			m_channel_enable[i] = 1;
		else
			m_channel_enable[i] = hist_m_channel_enable[i];
	}

	CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT9_P);
	if(m_channel_enable[0])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT10_P);
	if(m_channel_enable[1])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT11_P);
	if(m_channel_enable[2])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT12_P);
	if(m_channel_enable[3])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT13_P);
	if(m_channel_enable[4])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT14_P);
	if(m_channel_enable[5])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT15_P);
	if(m_channel_enable[6])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT16_P);
	if(m_channel_enable[7])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);

	CString tmp;
	tmp.Format(_T("%d"),1000);
	CEdit* pEditPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_STATIC_TIME));
	m_wndSetupBar.GetDlgItem(IDC_STATIC_TIME)->SetWindowText(tmp);

	OnBnClickedCheckInput7();
	return 0;
}

void CADHistFrm::flashwindows_check(void)
{
	m_channel_cnt = hist_m_channel_cnt;
	for (int i = 0; i < 8 ; i++)
	{
		m_channel_enable[i] = 1;
	}

	CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT9_P);
	if(m_channel_enable[0])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT10_P);
	if(m_channel_enable[1])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT11_P);
	if(m_channel_enable[2])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT12_P);
	if(m_channel_enable[3])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT13_P);
	if(m_channel_enable[4])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT14_P);
	if(m_channel_enable[5])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT15_P);
	if(m_channel_enable[6])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
	pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT16_P);
	if(m_channel_enable[7])
		pRadio->SetCheck(1);
	else
		pRadio->SetCheck(0);
}

void CADHistFrm::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}

void CADHistFrm::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));
	pDoc->m_Offset += pDoc->m_Rate;
	pSliderOffset->SetPos(pDoc->m_Offset);
	if (pDoc->m_Offset>=(pDoc->m_FileLength-sizeof(::_FILE_HEADER))/pDoc->m_ChannelCount-pDoc->m_nCount) // m_FileLength WORD
	//if(pDoc->m_Offset+pDoc->m_nCount >= pDoc->m_FileLength-sizeof(::_FILE_HEADER))
	{
		KillTimer(1);
	}

	RedrawDataWindow();
	CMDIChildWnd::OnTimer(nIDEvent);
}

void CADHistFrm::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
}

void CADHistFrm::OnHistoryStart() 
{
	// TODO: Add your command handler code here
	m_bStart = TRUE;
	SetTimer(1, 200, NULL);

}

void CADHistFrm::OnUpdateHistoryStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart == FALSE ? 1:0);
}

void CADHistFrm::OnHistoryStop() 
{
	// TODO: Add your command handler code here
	m_bStart = FALSE;
	KillTimer(1);
}

void CADHistFrm::OnUpdateHistoryStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart == TRUE ? 1:0);
}

void CADHistFrm::OnPositionA(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
} 
void CADHistFrm::OnPositionB(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
} 

void CADHistFrm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CMDIChildWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
    CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 

	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	pDoc->m_Offset=pSliderOffset->GetPos();  // ȡ�û�����λ�ã����ļ�ƫ�ƣ��֣���
	pSliderOffset->SetPos(pDoc->m_Offset);


// 	CString str;	
// 	CEdit* pEditOffset = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
// 	CEdit* pEditScreenPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
// 	str.Format(_T("%d"), Pos);
// 	pEditOffset->SetWindowText(str);  // ��ʾ�ļ�ƫ��
// 	str.Format(_T("%d"), Pos);
// 	pEditScreenPos->SetWindowText(str);   
	RedrawDataWindow();  // �ػ�������ͼ
	//pWaveView->m_ADHistScope.AppendPoly(gl_BufferID, gl_Offset);

	*pResult = 0;

}


void CADHistFrm::OnCustomdrawSLIDERScreenCur(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
    CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	int nChannelCount = pDoc->m_Header.ADPara.LastChannel - pDoc->m_Header.ADPara.FirstChannel + 1;

	CSliderCtrl* pSliderScreenCur = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_ScreenCur));	
	int Pos=pSliderScreenCur->GetPos();  // ȡ�û�����λ�ã����ļ�ƫ�ƣ��֣���
	pSliderScreenCur->SetPos(Pos);
	pDoc->m_ScreenOffset = Pos - Pos%nChannelCount;		// �ĵ��е���Ļƫ�Ƶ��ڻ��ߵ���Ļλ��ƫ�� 

	RedrawDataWindow();  // �ػ�������ͼ
	pWaveView->m_ADHistScope.AppendPoly(gl_BufferID, pDoc->m_ScreenOffset);

	*pResult = 0;
}

void CADHistFrm::RedrawDataWindow(void)
{
	CString str;
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	CADHistDigitView* pDigitView = (CADHistDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
    CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
//	CEdit* pOffset = (CEdit*)m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset);

 	CEdit* pEditPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
    str.Format(_T("%d"), pDoc->m_Offset);
	m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset)->SetWindowText(str);
   // pEditPos->SetWindowText(str);	
	pDoc->ReadData();
	pWaveView->m_ADHistScope.AppendPoly(gl_BufferID, gl_Offset);
}

void CADHistFrm::OnSelchangeCOMBORate() 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	CComboBox* pRate = (CComboBox*)(m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	CString strRate;
	int Index = pRate->GetCurSel();  // ȡ�õ�ǰѡ��������
	pRate->GetLBText(Index, strRate);   // ����ѡ��ȡ����ѡ�����ı�	
	pDoc->m_Rate = wcstol(strRate, NULL, 10);  // ���ַ�ת��������
}

void CADHistFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}

void CADHistFrm::OnBUTTONRefrushData() 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	CADHistDigitView* pDigitView = (CADHistDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	pDigitView->SetListView();
}

void CADHistFrm::OnBUTTONPositionA() 
{
	CADHistDoc* pDoc = theApp.m_pADHistDoc;
 	CADHistWaveView* pWaveView = (CADHistWaveView*)CWnd::FromHandle(pDoc->m_hWndWave);
  	CADHistDigitView* pView = (CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);

	CString strPositionA;
	CStatic* pStaticA = (CStatic*)m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionA);
 	pStaticA->GetWindowText(strPositionA);
	pView->SetListView();

	CListCtrl& List = pView->GetListCtrl();
	
 	LPCTSTR lpszmyString = strPositionA;
 	LVFINDINFO info;
 	int nIndex;
 	info.flags = LVFI_STRING ;
 	info.psz = lpszmyString;
 	while ((nIndex = List.FindItem(&info)) != -1)
 	{
		if (nIndex >= 0)
		{		
			List.EnsureVisible(nIndex, FALSE);							//ʹlistctrl��һ��ɼ���������������
			List.SetItemState(nIndex, 2 , LVIS_SELECTED|LVIS_FOCUSED);  //ѡ��ѡ��һ��(0,Ϊȡ��)
			List.SetFocus();
			break;
		}		
	}	

	Invalidate();	
}

void CADHistFrm::OnBUTTONPositionB() 
{
	CADHistDoc* pDoc = theApp.m_pADHistDoc;
 	CADHistWaveView* pWaveView = (CADHistWaveView*)CWnd::FromHandle(pDoc->m_hWndWave);
  	CADHistDigitView* pView = (CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);

	CString strPositionB;;
	CStatic* pStaticB = (CStatic*)m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionB);
 	pStaticB->GetWindowText(strPositionB);
	pView->SetListView();

	CListCtrl& List = pView->GetListCtrl();
	
 	LPCTSTR lpszmyString = strPositionB;
 	LVFINDINFO info;
 	int nIndex;
 	info.flags = LVFI_STRING ;
 	info.psz = lpszmyString;
 	while ((nIndex = List.FindItem(&info)) != -1)
 	{
		if (nIndex >= 0)
		{		
			List.EnsureVisible(nIndex, FALSE);							//ʹlistctrl��һ��ɼ���������������
			List.SetItemState(nIndex, 2 , LVIS_SELECTED|LVIS_FOCUSED);  //ѡ��ѡ��һ��(0,Ϊȡ��)
			List.SetFocus();
			break;
		}		
	}	

	Invalidate();	
	
}


void CADHistFrm::OnBnClickedCheckInput9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(m_channel_enable[0])
	{
		m_channel_cnt -= 1;
		m_channel_enable[0] = 0;
	}
	else
	{
		m_channel_enable[0] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT9_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[0] = 1;
	}
	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}


void CADHistFrm::OnBnClickedCheckInput10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_channel_enable[1])
	{
		m_channel_cnt -= 1;
		m_channel_enable[1] = 0;
	}
	else
	{
		m_channel_enable[1] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT10_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[1] = 1;
	}
	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}


void CADHistFrm::OnBnClickedCheckInput11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_channel_enable[2])
	{
		m_channel_cnt -= 1;
		m_channel_enable[2] = 0;
	}
	else
	{
		m_channel_enable[2] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT11_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[2] = 1;
	}

	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}


void CADHistFrm::OnBnClickedCheckInput12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_channel_enable[3])
	{
		m_channel_cnt -= 1;
		m_channel_enable[3] = 0;
	}
	else
	{
		m_channel_enable[3] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT12_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[3] = 1;
	}
	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}


void CADHistFrm::OnBnClickedCheckInput13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_channel_enable[4])
	{
		m_channel_cnt -= 1;
		m_channel_enable[4] = 0;
	}
	else
	{
		m_channel_enable[4] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT13_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[4] = 1;
	}
	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}


void CADHistFrm::OnBnClickedCheckInput14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_channel_enable[5])
	{
		m_channel_cnt -= 1;
		m_channel_enable[5] = 0;
	}
	else
	{
		m_channel_enable[5] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT14_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[5] = 1;
	}
	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}


void CADHistFrm::OnBnClickedCheckInput15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_channel_enable[6])
	{
		m_channel_cnt -= 1;
		m_channel_enable[6] = 0;
	}
	else
	{
		m_channel_enable[6] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT15_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[6] = 1;
	}
	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}


void CADHistFrm::OnBnClickedCheckInput16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_channel_enable[7])
	{
		m_channel_cnt -= 1;
		m_channel_enable[7] = 0;
	}
	else
	{
		m_channel_enable[7] = 1;
		m_channel_cnt += 1;
	}
	if(m_channel_cnt == 0)
	{
		CButton* pRadio = (CButton*)m_wndSetupBar.GetDlgItem(IDC_CHECK_INPUT16_P);
		pRadio->SetCheck(1);
		m_channel_cnt = 1;
		m_channel_enable[7] = 1;
	}
	LRESULT sta;
	OnCustomdrawSLIDERScreenCur(NULL,&sta);
}

static int TimeAxisRangeVal[]={10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000};//��msΪ��λ
static int TimeAxisRangeID=6;//��ʼΪ1s��
//ʱ�����̼�С
void CADHistFrm::OnBnClickedButton1TimeL()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (TimeAxisRangeID==0)
	{
		//TimeAxisRangeID=sizeof(TimeAxisRangeVal)/4-1;
		return;
	}
	else
	{
		TimeAxisRangeID--;
	}

	//RedrawDataWindow();
	g_nTimeAxisRange = TimeAxisRangeVal[TimeAxisRangeID]*1000;
	CString tmp;
	tmp.Format(_T("%d"),TimeAxisRangeVal[TimeAxisRangeID]);
	CEdit* pEditPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_STATIC_TIME));
	m_wndSetupBar.GetDlgItem(IDC_STATIC_TIME)->SetWindowText(tmp);

	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();
	ULONG tmp1;
	if (g_nTimeAxisRange >= 1000000)
	{
		tmp1 = g_nTimeAxisRange/1000000;
		int tmp3 = pDoc->m_Header.ADPara.Frequency/1000;
		for (ULONG i = 0; i < SHOW_DATA_CNT; i++)
		{
			ULONG tmp2 = i*tmp1*tmp3;
			read_point_offset[i]=tmp2/(10*8);
		}
	}
	else
	{
		tmp1 = g_nTimeAxisRange/10000;
		int tmp3 = pDoc->m_Header.ADPara.Frequency/1000;
		for (ULONG i = 0; i < SHOW_DATA_CNT; i++)
		{
			ULONG tmp2 = i*tmp1*tmp3;
			read_point_offset[i]=tmp2/(1000*8);
		}
	}
	RedrawDataWindow();
}

//ʱ����������
void CADHistFrm::OnBnClickedButton1TimeR()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (TimeAxisRangeID==sizeof(TimeAxisRangeVal)/4-1)
	{
		//TimeAxisRangeID=0;
		return;
	}
	else
	{
		TimeAxisRangeID++;
	}

	g_nTimeAxisRange = TimeAxisRangeVal[TimeAxisRangeID]*1000;
	CString tmp;
	tmp.Format(_T("%d"),TimeAxisRangeVal[TimeAxisRangeID]);
	CEdit* pEditPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_STATIC_TIME));
	m_wndSetupBar.GetDlgItem(IDC_STATIC_TIME)->SetWindowText(tmp);

	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();
	ULONG tmp1;
	if (g_nTimeAxisRange >= 1000000)
	{
		tmp1 = g_nTimeAxisRange/1000000;
		int tmp3 = pDoc->m_Header.ADPara.Frequency/1000;
		for (ULONG i = 0; i < SHOW_DATA_CNT; i++)
		{
			ULONG tmp2 = i*tmp1*tmp3;
			read_point_offset[i]=tmp2/(10*8);
		}
	}
	else
	{
		tmp1 = g_nTimeAxisRange/10000;
		int tmp3 = pDoc->m_Header.ADPara.Frequency/1000;
		for (ULONG i = 0; i < SHOW_DATA_CNT; i++)
		{
			ULONG tmp2 = i*tmp1*tmp3;
			read_point_offset[i]=tmp2/(1000*8);
		}
	}
	RedrawDataWindow();
}


static int VAxisRangeVal[]={10,20,50,100,200,500,1000,2000,5000,10000};//��mvΪ��λ
static int VAxisRangeID=9;//��ʼΪ1s��
void CADHistFrm::OnBnClickedCheckInput6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(VAxisRangeID > 0)
		VAxisRangeID--;
	//else
	//	return;

	CString tmp;
	tmp.Format(_T("%d"),VAxisRangeVal[VAxisRangeID]);
	CEdit* pEditPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_VAxisRange2));
	m_wndSetupBar.GetDlgItem(IDC_EDIT_VAxisRange2)->SetWindowText(tmp);
	g_nVAxisRange = VAxisRangeVal[VAxisRangeID];
	RedrawDataWindow();
}

void CADHistFrm::OnBnClickedCheckInput7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(VAxisRangeID < 9 )
		VAxisRangeID++;
	/*else
		return;
*/
	CString tmp;
	tmp.Format(_T("%d"),VAxisRangeVal[VAxisRangeID]);
	CEdit* pEditPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_VAxisRange2));
	m_wndSetupBar.GetDlgItem(IDC_EDIT_VAxisRange2)->SetWindowText(tmp);

	g_nVAxisRange = VAxisRangeVal[VAxisRangeID];
	RedrawDataWindow();
}

void CADHistFrm::OnBnClickedCheckexportexcel()
{
	WORD read_buf[80000] = {0}; // 1M
	ULONG offset = 0;
	ULONG read_len = 0;
	ULONG last_len = 0;
	ULONG Mb_len = 1024*1024;
	int file_cnt = 1;
	FILE *df = NULL;
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	unsigned char m_path[512];
	unsigned char path_use[512];
	unsigned char write_buf[1024];
	
	pDoc->Retern_FilePath(m_path);

	CString strNewFileName;
	CString TempFileName;

	if (!(theApp.DoPromptFileName(strNewFileName, IDS_NEW_FILE, 
		OFN_HIDEREADONLY , TRUE, NULL)))
		return;

	int len =WideCharToMultiByte(CP_ACP,0,strNewFileName,-1,NULL,0,NULL,NULL);  
	WideCharToMultiByte(CP_ACP,0,strNewFileName,-1,(LPSTR)path_use,len,NULL,NULL );  
	sprintf((char*)path_use,"%s_%d.csv",path_use,file_cnt);
	df = fopen((char*)path_use,"w+");
	if(df == NULL) return;
	sprintf((char*)write_buf,"Time,Signal1,Signal2,Signal3,Signal4,Input1,Input2,Input3,Input4\n");
	fwrite(write_buf,1,strlen((char*)write_buf),df);

	while (1)
	{
		read_len =pDoc->ReadDataForExcel(read_buf,80000*2,offset);
		offset += read_len;
		for ( int i = 0; i < read_len/2; )
		{
			sprintf((char*)write_buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				i,//��ΪHH:MM:SS-ms
				read_buf[i],read_buf[i+1],read_buf[i+2],
				read_buf[i+3],read_buf[i+4],read_buf[i+5],
				read_buf[i+6],read_buf[i+7]);
			i+=8;
			fwrite(write_buf,1,strlen((char*)write_buf),df);
		}
		
		if (offset - last_len >= Mb_len*10) // 26m���ԣ�   50M�����޷�����ȫ
		{
			last_len = offset;
			file_cnt++;
			fclose(df);
			sprintf((char*)path_use,"%s_%d.csv",m_path,file_cnt);
			df = fopen((char*)path_use,"w+");
			if(df == NULL) break;
			sprintf((char*)write_buf,"Signel1,Signel2,Signel3,Signel4,Input1,Input2,Input3,Input4\n");
			fwrite(write_buf,1,strlen((char*)write_buf),df);
		}
		if(read_len < 80000)
		{
			fclose(df);
			break;
		}
	}
	sprintf((char*)write_buf,"����excel�ɹ���������%d���ļ���\n",file_cnt);
	LPCTSTR  lpb = (LPCTSTR)(LPTSTR)write_buf;
	MessageBox(_T("����excel�ɹ�!"),MB_OK);
	return;
}
