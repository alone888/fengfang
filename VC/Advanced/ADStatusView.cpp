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
	DDX_Control(pDX, IDC_EDIT3, m_signel[0][0]);
	DDX_Control(pDX, IDC_EDIT2, m_signel[0][1]);
	DDX_Control(pDX, IDC_EDIT5, m_signel[1][0]);
	DDX_Control(pDX, IDC_EDIT4, m_signel[1][1]);
	DDX_Control(pDX, IDC_EDIT7, m_signel[2][0]);
	DDX_Control(pDX, IDC_EDIT6, m_signel[2][1]);
	DDX_Control(pDX, IDC_EDIT9, m_signel[3][0]);
	DDX_Control(pDX, IDC_EDIT8, m_signel[3][1]);
	DDX_Control(pDX, IDC_EDIT11, m_signel[4][0]);
	DDX_Control(pDX, IDC_EDIT10, m_signel[4][1]);
	DDX_Control(pDX, IDC_EDIT13, m_signel[5][0]);
	DDX_Control(pDX, IDC_EDIT12, m_signel[5][1]);
	DDX_Control(pDX, IDC_EDIT15, m_signel[6][0]);
	DDX_Control(pDX, IDC_EDIT14, m_signel[6][1]);
	DDX_Control(pDX, IDC_EDIT17, m_signel[7][0]);
	DDX_Control(pDX, IDC_EDIT16, m_signel[7][1]);
	DDX_Control(pDX, IDC_EDIT_TriggerVolt, m_Edit_TringgerVolt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CADStatusView, CFormView)
	//{{AFX_MSG_MAP(CADStatusView)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT6, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT7, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT8, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT9, OnChangeEDIT2)

	ON_EN_KILLFOCUS(IDC_EDIT10, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT11, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT12, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT13, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT14, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT15, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT16, OnChangeEDIT2)
	ON_EN_KILLFOCUS(IDC_EDIT17, OnChangeEDIT2)
	
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


	for (int i = 0; i < 8; i++)
	{
		g_filer[i][0] = 0;
		g_filer[i][1] = 0;
		m_signel[i][0].SetWindowText(_T("0"));
		m_signel[i][1].SetWindowText(_T("0"));
	}
// 	pButtonTile->EnableWindow(FALSE);
// 	pButtonPose->EnableWindow(FALSE);
	SetDlgCheck();
	OnGraphicShow();
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


void read_filter(char *filpath,int dis_frq,int index_s,double *filter)
{
	FILE *fd = NULL;
	unsigned char read_one_byte = 0;
	unsigned char read_feq[10] = {0};
	unsigned char read_buf[2048] = {0};

	unsigned int start_id = 0;
	unsigned int end_id = 0;
	unsigned int index = 0;

	if(dis_frq == 0)
	{
		memset(filter,0,sizeof(double)*FILTER_DEEP);
	}

	fd = fopen(filpath,"r");
	if (fd == NULL)
		return ;

	for (; ;index++)
	{
		if(fread(&read_one_byte,1,1,fd) != 1)
			return;
		if (read_one_byte == '=')
		{
			if(fread(read_feq,1,5,fd) != 5)
				return;
			if(atoi((char*)read_feq) == dis_frq)
			{
				index+=8;
				start_id = index;
				break;
			}
			index+=5;
		}
	}
	if(start_id == 0)
		return;
	fseek(fd,start_id,0);
	for (; ;index++)
	{
		if(fread(&read_one_byte,1,1,fd) != 1)
			return;

		if (read_one_byte == '!')
		{
			end_id = index;
			break;
		}
	}
	if(end_id == 0) return;

	fseek(fd,start_id+1,0);
	
	index = fread(read_buf,1,end_id-start_id-2,fd);
	if(index != end_id-start_id-2)
		return;
	CString str;
	CString resToken;
	int curPos= 0;
	char *stopstr;
	char str_tmp[100];

	str = read_buf;
	index = 0;
	resToken= str.Tokenize(_T("$"),curPos);
	//str_tmp = resToken;
	strcpy((char*)str_tmp,CT2CA(resToken));
	filter[index++] = strtod(str_tmp,&stopstr);
	
	while (resToken != "" && index < FILTER_DEEP)
	{
		resToken= str.Tokenize(_T("$"),curPos);
		strcpy((char*)str_tmp,CT2CA(resToken));
		filter[index++] = strtod(str_tmp,&stopstr);
	}
}

void CADStatusView::OnChangeEDIT2()
{
	CString tmp;

	static int filer[8][2] = {0};
	for (int i = 0; i < 8; i++)
	{
		m_signel[i][0].GetWindowText(tmp);
		g_filer[i][0] = wcstol(tmp, NULL, 10);
		m_signel[i][1].GetWindowText(tmp);
		g_filer[i][1] = wcstol(tmp, NULL, 10);

		if(g_filer[i][0] >= ADPara.Frequency/8/2)
		{
			m_signel[i][0].SetWindowText(_T("0"));
		}
		if(g_filer[i][1] >= ADPara.Frequency/8/2)
		{
			m_signel[i][1].SetWindowText(_T("0"));
		}
		if (m_signel[i][0].GetWindowTextLengthW() > 6)
		{
			m_signel[i][0].SetWindowText(_T("0"));
		}
		if (m_signel[i][1].GetWindowTextLengthW() > 6)
		{
			m_signel[i][1].SetWindowText(_T("0"));
		}
	}
	char path[100];
	for(int  i = 0; i < 8 ; i++)
	{
		if(filer[i][0] != g_filer[i][0])// 高通
		{
			sprintf(path,"filter_h_%d.txt",ADPara.Frequency);
			read_filter(path,g_filer[i][0],i,g_filter_data_h[i]);
			filer[i][0] = g_filer[i][0];
			if(g_filter_data_h[i][0] == 0)
			{
				g_filer[i][0] = 0;
				m_signel[i][0].SetWindowText(_T("0"));
			}
		}
		if(filer[i][1] != g_filer[i][1])// 低通
		{
			sprintf(path,"filter_l_%d.txt",ADPara.Frequency);
			read_filter(path,g_filer[i][1],i,g_filter_data_l[i]);
			filer[i][1] = g_filer[i][1];
			if(g_filter_data_l[i][1] == 0)
			{
				g_filer[i][1] = 0;
				m_signel[i][1].SetWindowText(_T("0"));
			}
		}
	}
}
