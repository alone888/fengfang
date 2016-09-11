// ParaCfgView.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "ADParaCfgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView
///////////////////////////////

///////////////////////////////
IMPLEMENT_DYNCREATE(CADParaCfgView, CFormView)
CADParaCfgView::CADParaCfgView()
: CFormView(CADParaCfgView::IDD)
{
	//{{AFX_DATA_INIT(CADParaCfgView)
	//}}AFX_DATA_INIT

	m_bInitialUpdate = FALSE;
	for (int iChannel=0; iChannel<USB2831_MAX_AD_CHANNELS; iChannel++)
	{
		m_nSyncInputRange[iChannel] = 0; // 初始化同步的电压量程
		m_nAsynInputRange[iChannel] = 0; // 初始化异步的电压量程
	}
}

CADParaCfgView::~CADParaCfgView()
{
}

void CADParaCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADParaCfgView)
	DDX_Control(pDX, IDC_COMBO_GroundingMode, m_Combo_GroundingMode);
	DDX_Control(pDX, IDC_EDIT_TrigWindow, m_Edit_TrigWindow);
	DDX_Control(pDX, IDC_EDIT_ChannelSum, m_Edit_ChannelSum);
	DDX_Control(pDX, IDC_COMBO_TriggerSource, m_Combo_TriggerSource);
	DDX_Control(pDX, IDC_EDIT_GetDevTrigPosAD, m_Edit_GetDevTrigPosAD);
	DDX_Control(pDX, IDC_COMBO_Range, m_Combo_Range);
	DDX_Control(pDX, IDC_CHECK_ClockOutput, m_Check_bClockOutput);
	DDX_Control(pDX, IDC_EDIT_LoopsOfGroup, m_Edit_LoopsOfGroup);
	DDX_Control(pDX, IDC_EDIT_GroupInterval, m_Edit_GroupInterval);
	DDX_Control(pDX, IDC_COMBO_TriggerDir, m_Combo_TriggerDir);
	DDX_Control(pDX, IDC_COMBO_LastChannel, m_Combo_LastChannel);
	DDX_Control(pDX, IDC_COMBO_Gains, m_Combo_Gains);
	DDX_Control(pDX, IDC_COMBO_FirstChannel, m_Combo_FirstChannel);
	DDX_Control(pDX, IDC_COMBO_CheckStsMode, m_Combo_CheckStsMode);
	DDX_Control(pDX, IDC_COMBO_ADMode, m_Combo_ADMode);
	DDX_Control(pDX, IDC_COMBO_Frequency, m_Combo_Frequency);
	DDX_Control(pDX, IDC_COMBO_TriggerMode, m_Combo_TriggerMode);
	DDX_Control(pDX, IDC_COMBO_TriggerType, m_Combo_TriggerType);
	DDX_Control(pDX, IDC_COMBO_ClockSource, m_Combo_ClockSource);
	DDX_Control(pDX, IDC_STATIC_Offset, m_Static_Offset);
	DDX_Control(pDX, IDC_SPIN_BufferID, m_Spin_BufferID);
	DDX_Control(pDX, IDC_SLIDER_Offset, m_Slider_Offset);
	DDX_Control(pDX, IDC_EDIT_BufferID, m_Edit_BufferID);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_INPUT9, m_singal_1);
	DDX_Control(pDX, IDC_CHECK_INPUT10, m_singal_2);
	DDX_Control(pDX, IDC_CHECK_INPUT11, m_singal_3);
	DDX_Control(pDX, IDC_CHECK_INPUT12, m_singal_4);
	DDX_Control(pDX, IDC_CHECK_INPUT13, m_input_1);
	DDX_Control(pDX, IDC_CHECK_INPUT14, m_input_2);
	DDX_Control(pDX, IDC_CHECK_INPUT15, m_input_3);
	DDX_Control(pDX, IDC_CHECK_INPUT16, m_input_4);
	DDX_Control(pDX, IDC_EDIT_ChannelSum2, m_TimerCnt);
	DDX_Control(pDX, IDC_EDIT_TimeAxisRange, m_TimeAxisRange);
	DDX_Control(pDX, IDC_EDIT_TimeAxisRange2, m_VAxisRange);
	DDX_Control(pDX, IDC_EDIT25, m_singal_offset[0]);
	DDX_Control(pDX, IDC_EDIT26, m_singal_offset[1]);
	DDX_Control(pDX, IDC_EDIT27, m_singal_offset[2]);
	DDX_Control(pDX, IDC_EDIT28, m_singal_offset[3]);
	DDX_Control(pDX, IDC_EDIT29, m_singal_offset[4]);
	DDX_Control(pDX, IDC_EDIT30, m_singal_offset[5]);
	DDX_Control(pDX, IDC_EDIT31, m_singal_offset[6]);
	DDX_Control(pDX, IDC_EDIT32, m_singal_offset[7]);
}


BEGIN_MESSAGE_MAP(CADParaCfgView, CFormView)
//{{AFX_MSG_MAP(CADParaCfgView)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerSource, OnSelchangeCOMBOTriggerSource)
	ON_CBN_SELCHANGE(IDC_COMBO_OutTriggerDir, OnSelchangeCOMBOTriggerDir)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_EN_CHANGE(IDC_EDIT_BufferID, OnChangeEDITBufferID)
	ON_CBN_SELCHANGE(IDC_COMBO_ClockSource, OnSelchangeCOMBOClockSource)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerType, OnSelchangeCOMBOTriggerType)
	ON_BN_CLICKED(IDC_CHECK_ClockOutput, OnCHECKClockOutput)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerMode, OnSelchangeCOMBOTriggerMode)
	ON_CBN_KILLFOCUS(IDC_COMBO_Frequency, OnKillfocusCOMBOFrequency)
	ON_CBN_SELCHANGE(IDC_COMBO_FirstChannel, OnSelchangeCOMBOFirstChannel)
	ON_CBN_SELCHANGE(IDC_COMBO_LastChannel, OnSelchangeCOMBOLastChannel)
	ON_CBN_SELCHANGE(IDC_COMBO_Range, OnSelchangeCOMBORange)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_ADMode, OnSelchangeCOMBOADMode)
	ON_EN_CHANGE(IDC_EDIT_GroupInterval, OnChangeEDITGroupInterval)
	ON_EN_CHANGE(IDC_EDIT_LoopsOfGroup, OnChangeEDITLoopsOfGroup)
	ON_CBN_SELCHANGE(IDC_COMBO_CheckStsMode, OnSelchangeCOMBOCheckStsMode)
	ON_EN_CHANGE(IDC_EDIT_TrigWindow, OnChangeEDITTrigWindow)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerDir, OnSelchangeCOMBOTriggerDir)
	ON_CBN_SELCHANGE(IDC_COMBO_GroundingMode, OnSelchangeCOMBOGroundingMode)
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_CHECK_INPUT1, &CADParaCfgView::OnBnClickedCheckInput1)
	ON_BN_CLICKED(IDC_CHECK_INPUT9, &CADParaCfgView::OnBnClickedCheckInput9)
	ON_BN_CLICKED(IDC_CHECK_INPUT10, &CADParaCfgView::OnBnClickedCheckInput10)
	ON_BN_CLICKED(IDC_CHECK_INPUT11, &CADParaCfgView::OnBnClickedCheckInput11)
	ON_BN_CLICKED(IDC_CHECK_INPUT12, &CADParaCfgView::OnBnClickedCheckInput12)
	ON_BN_CLICKED(IDC_CHECK_INPUT13, &CADParaCfgView::OnBnClickedCheckInput13)
	ON_BN_CLICKED(IDC_CHECK_INPUT14, &CADParaCfgView::OnBnClickedCheckInput14)
	ON_BN_CLICKED(IDC_CHECK_INPUT15, &CADParaCfgView::OnBnClickedCheckInput15)
	ON_BN_CLICKED(IDC_CHECK_INPUT16, &CADParaCfgView::OnBnClickedCheckInput16)
	ON_BN_CLICKED(IDC_BUTTON1, &CADParaCfgView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CADParaCfgView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CADParaCfgView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CADParaCfgView::OnBnClickedButton5)

	ON_EN_CHANGE(IDC_EDIT25, Change_Offset)
	ON_EN_CHANGE(IDC_EDIT26, Change_Offset)
	ON_EN_CHANGE(IDC_EDIT27, Change_Offset)
	ON_EN_CHANGE(IDC_EDIT28, Change_Offset)
	ON_EN_CHANGE(IDC_EDIT29, Change_Offset)
	ON_EN_CHANGE(IDC_EDIT30, Change_Offset)
	ON_EN_CHANGE(IDC_EDIT31, Change_Offset)
	ON_EN_CHANGE(IDC_EDIT32, Change_Offset)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView diagnostics

#ifdef _DEBUG
void CADParaCfgView::AssertValid() const
{
	CFormView::AssertValid();
}

void CADParaCfgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView message handlers
// 对data指针指向的缓冲区从下标为left到right的元素进行 降序排序
void SortDes(LONG *data, int left, int right) 
{
	register int i,j;  
	LONG m,t;     
	i=left;j=right;
	m=data[(int)(left+right)/2];  
	
	do{
		while(data[i]>m && i<right)
			i++;
		while(m>data[j] && j>left)
			j--;
		if(i <= j) { //交换i, j 的值
			t = data[i];
			data[i] = data[j];
			data[j] = t;
			i++;j--;
		}
	}while(i<=j);
	if(left<j)  SortDes(data,left,j);   
	if(i<right) SortDes(data,i,right);
}

void save_channel()
{
	FILE *pd = NULL;

	pd = fopen("channel_valid.txt","w");
	if(pd == NULL) return ;
	fwrite(gl_signal_enable,sizeof(gl_signal_enable),1,pd);
	fclose(pd);
}

void read_channel()
{
	FILE *pd = NULL;

	pd = fopen("channel_valid.txt","r");
	if(pd == NULL) return ;
	fread(gl_signal_enable,sizeof(gl_signal_enable),1,pd);
	fclose(pd);
	gl_nChannelCount = 0;
	for (int i = 0;i < 8;i++)
	{
		if (gl_signal_enable[i] == 1)
		{
			gl_nChannelCount++;
		}
	}
}

void CADParaCfgView::OnInitialUpdate()
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CFormView::OnInitialUpdate();
	CADDoc* pDoc = (CADDoc*)theApp.m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 

	pDoc->m_hWndADCfg = this->m_hWnd;
	theApp.m_hParaCfgView = this->m_hWnd;
	
	gl_signal_enable[0] = 1;
	gl_signal_enable[1] = 1;
	gl_signal_enable[2] = 1;
	gl_signal_enable[3] = 1;
	gl_signal_enable[4] = 1;
	gl_signal_enable[5] = 1;
	gl_signal_enable[6] = 1;
	gl_signal_enable[7] = 1;
	gl_nChannelCount = 8;

	read_channel();

	if(gl_signal_enable[0])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT9))->SetCheck(1);
	}
	if(gl_signal_enable[1])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT10))->SetCheck(1);
	}
	if(gl_signal_enable[2])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT11))->SetCheck(1);
	}
	if(gl_signal_enable[3])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT12))->SetCheck(1);
	}
	if(gl_signal_enable[4])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT13))->SetCheck(1);
	}
	if(gl_signal_enable[5])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT14))->SetCheck(1);
	}
	if(gl_signal_enable[6])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT15))->SetCheck(1);
	}
	if(gl_signal_enable[7])
	{
		((CButton *)GetDlgItem(IDC_CHECK_INPUT16))->SetCheck(1);
	}


	OnBnClickedButton1();
	OnBnClickedButton5();


	// 初始化用户界面视觉参数		
	CString str;
//	OldADPara = ADPara; // 保存旧的硬件参数
	//ADPara.CheckStsMode = 1; // 默认数据查询方式为非空
	m_Combo_CheckStsMode.SetCurSel(ADPara.CheckStsMode);
	
	m_Combo_ADMode.SetCurSel(ADPara.ADMode);
	m_Combo_FirstChannel.SetCurSel(ADPara.FirstChannel);
	m_Combo_LastChannel.SetCurSel(ADPara.LastChannel);	
	
	m_Combo_GroundingMode.SetCurSel(USB2831_GNDMODE_DI);//ADPara.GroundingMode
	OnSelchangeCOMBOGroundingMode();
	
	
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	
	
	str.Format((_T("%d")), ADPara.TrigWindow);
	m_Edit_TrigWindow.SetWindowText(str);
	CString strMsg;
	UpdateData(FALSE);
	
	pApp->WriteProfileInt(_T("strSectionFre"),_T("Frequency0"),250000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency1"), 200000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency2"), 150000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency3"), 100000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency4"), 80000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency5"), 50000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency6"), 40000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency7"), 25000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency8"), 20000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency9"), 10000/8);
	pApp->WriteProfileInt(_T("strSectionFre"), _T("Frequency10"), 1000/8);


	LONG FreqCount, arFrequency[256];
	FreqCount = LoadFrequency(arFrequency);
	str.Format(_T("%d"), FreqCount);
	SortDes(arFrequency, 0, FreqCount-1);
	for(int FreqIndex=0; FreqIndex<FreqCount; FreqIndex++)
	{
		strMsg.Format(_T("%d"), arFrequency[FreqIndex]);		
		m_Combo_Frequency.AddString(strMsg);
	}

	LONG Frequency = 0;
	Frequency = pApp->GetProfileInt(_T("strSectionFre"),_T("Frequency0"), 0x00);
	if(Frequency == 0) Frequency = 25000/8;
	strMsg.Format(_T("%d"), Frequency);
	m_Combo_Frequency.SetWindowText(strMsg);

	// 初始化增益设置
	m_Combo_Gains.AddString(_T("1"));
	m_Combo_Gains.AddString(_T("2"));
	m_Combo_Gains.AddString(_T("4"));
	m_Combo_Gains.AddString(_T("8"));
	m_Combo_Gains.SetCurSel(ADPara.Gains);
	strMsg.Format(_T("%d"), ADPara.GroupInterval);
	m_Edit_GroupInterval.SetWindowText(strMsg);
	strMsg.Format(_T("%d"), ADPara.LoopsOfGroup);
	m_Edit_LoopsOfGroup.SetWindowText(strMsg);

	m_Combo_TriggerMode.SetCurSel(ADPara.TriggerMode);
	m_Combo_TriggerType.SetCurSel(ADPara.TriggerType); // 触发方式(电平|边沿)
	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir); // 初始化外触发方向
	m_Combo_TriggerSource.SetCurSel(ADPara.TriggerSource);

	m_Static_Offset.SetWindowText(_T("0"));
	m_Spin_BufferID.SetRange(0, 63);	   // 初始化缓冲段范围
	m_Spin_BufferID.SetBuddy(&m_Edit_BufferID);
	m_Spin_BufferID.SetPos(0);
	m_Slider_Offset.SetRange(0, 8192 / gl_nChannelCount); // 初始化缓冲段内偏移范围

	m_Combo_ClockSource.SetCurSel(ADPara.ClockSource); // 初始化时钟源
	m_Check_bClockOutput.SetCheck(ADPara.bClockOutput);

	SetStatusBar();

	m_Combo_Range.SetCurSel(ADPara.InputRange);
	int Channel = 0;
	for (Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++) // 初始化电压量程
	{
		gl_InputRange[Channel].nRangeIndex = USB2831_INPUT_N10000_P10000mV;
		SetInputRangeEx(gl_InputRange[Channel].nRangeIndex, Channel);
	}

	m_bInitialUpdate = TRUE;
	theApp.m_pADDoc->m_hADParaCfgView = this;


	// <<--------------------------------------------------------->>
	// 创建汽泡提示类
	//HICON hIcon = static_cast<HICON>(::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16,16, LR_DEFAULTCOLOR));
	//theApp.m_BalloonHelp.SetIcon(hIcon);
	//::DestroyIcon(hIcon);
	
	//CFont* pFont = new CFont;
	//pFont->CreatePointFont(120, _T("幼圆(TT)"));
	//theApp.m_BalloonHelp.SetTitleFont(pFont);   // deleted by balloon.
	
	//CRect rect;
	//GetClientRect(&rect);
	//m_Edit_GetDevTrigPosAD.GetWindowRect(&rect);
	
	//theApp.m_BalloonHelp.DestroyBalloon();

	//theApp.m_BalloonHelp.Create("您好!", "欢迎进入阿尔泰测控演示系统!", rect.CenterPoint(),	// rect.BottomRight()
	//	CBalloonHelp::unSHOW_INNER_SHADOW|CBalloonHelp::unSHOW_CLOSE_BUTTON|
	//	CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN, 
	//	this);
	
	//pDoc->m_RectBalloon = rect;	

	OnSelchangeCOMBORange();
	// <<--------------------------------------------------------->>
	
}

//###########################################################################
// 设置触发源 (内触发或外触发)
void CADParaCfgView::OnSelchangeCOMBOTriggerMode() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TriggerMode);
	ADPara.TriggerMode = pCombo->GetCurSel();
}

void CADParaCfgView::EnableControlWindows(BOOL Flag)
{
	m_Combo_ADMode.EnableWindow(Flag);
	m_Combo_CheckStsMode.EnableWindow(Flag);
	m_Combo_FirstChannel.EnableWindow(Flag);
	m_Combo_LastChannel.EnableWindow(Flag);
	m_Combo_Gains.EnableWindow(Flag);
// 	m_Combo_Gains_Chip.EnableWindow(Flag);
	m_Check_bClockOutput.EnableWindow(Flag);
	m_Combo_Frequency.EnableWindow(Flag);
	m_Combo_TriggerMode.EnableWindow(Flag);
	m_Combo_TriggerDir.EnableWindow(Flag);    // 外触发沿	
	m_Combo_ClockSource.EnableWindow(Flag);
	m_Combo_Range.EnableWindow(Flag);
	m_Combo_GroundingMode.EnableWindow(Flag);
	
	m_Edit_TrigWindow.EnableWindow(Flag);
	m_Edit_GroupInterval.EnableWindow(Flag);
	m_Edit_LoopsOfGroup.EnableWindow(Flag);

	if (gl_nProcMode == PROC_MODE_SAVE) // 如果是存盘操作
	{
		m_Edit_BufferID.EnableWindow(Flag);
		m_Spin_BufferID.EnableWindow(Flag);
	}
	m_Combo_TriggerSource.EnableWindow(Flag);
	m_Combo_TriggerType.EnableWindow(Flag);
}

//######################################################################################
// 外触发方向设置
void CADParaCfgView::OnSelchangeCOMBOTriggerDir() 
{
	ADPara.TriggerDir = m_Combo_TriggerDir.GetCurSel();
}

void CADParaCfgView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)  
{
	// TODO: Add your specialized code here and/or call the base class
	if ((lHint==1) || (lHint==2)) return;
	CView::OnUpdate(pSender, lHint, pHint);
}

void CADParaCfgView::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	if (m_bInitialUpdate) // 如果本类对象的OnInitialUpdate函数已调用后
	{ 
		//CSliderCtrl* pSliderOffset = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_Offset));	
		//int Pos = pSliderOffset->GetPos();  // 取得滑动条位置
		int Pos = m_Slider_Offset.GetPos();  // 取得滑动条位置
 		gl_Offset = Pos;	// gl offset 标志在缓冲区中的偏移
// 		gl_Offset = gl_Offset - gl_Offset%gl_nChannelCount;
// 		Pos = gl_Offset;
// 		pSliderOffset->SetPos(Pos);

		CString str;
		str.Format(_T("%d"), gl_Offset);
		CStatic* pStatic=(CStatic*)(GetDlgItem(IDC_STATIC_Offset));
		pStatic->SetWindowText(str);
		//GetDlgItem(IDC_STATIC_Offset)->SetWindowText(str);
		CSysApp* pApp = (CSysApp*)AfxGetApp();	
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
		
		pWaveView->OnDrawPolyLine(); // 更新波形视图
		pDigitView->SetScrollPos(SB_VERT, gl_Offset * 16);  // 更新数字视图
		pDigitView->Invalidate();
	//	pDigitView->RedrawWindow();  // 更新数字视图
		
		CADFrm* pADFrm = (CADFrm*)pWaveView->GetParentFrame();
		CEdit* pEditOffset = (CEdit*)(pADFrm->m_wndShowStatus).GetDlgItem(IDC_EDIT_Offset); // 取得IDC_STATIC_ShowXY句柄
// 		int Count = gl_Offset+gl_WavePointX;  // 偏移总点数
// 		str.Format(_T("%d"), Count);
// 		pEditOffset->SetWindowText(str);
		str.Format(_T("%d"), gl_Offset);
		pEditOffset->SetWindowText(str);
		//*pResult = 0;
	}
}

void CADParaCfgView::OnChangeEDITBufferID() 
{
	CString string;
	CEdit* pEditBufferID = (CEdit*)(GetDlgItem(IDC_EDIT_BufferID));	
	pEditBufferID->GetWindowText(string); 
	gl_BufferID = wcstol(string, NULL, 10); // 改变ADBuffer的缓冲段号
	if (gl_BufferID<0 || gl_BufferID>63)
	{	
		gl_BufferID = 0;
		pEditBufferID->SetWindowText(_T("0"));
	}
	/*
	CADDoc* pADDoc = (CADDoc*)theApp.m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pADDoc->m_hWndWave));
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pADDoc->m_hWndDigit));
	int ReadSizeWords = gl_BufferID * 8192; // RAM偏移位置，以8K为单位
	if (m_bInitialUpdate)
	{
		if (gl_bDeviceADRun == FALSE)
		{
			if (gl_bCollected)
			{
				for (int nADChannel=0; nADChannel<MAX_AD_CHANNELS; nADChannel++) 
				{   
					USB2831_ReadDeviceProAD_Npt(theApp.m_hDevice, 
						ADBuffer[gl_nDrawIndex][nADChannel], 
						ReadSizeWords, (LONG*)ReadSizeWords); // ????
				}
			}
		}
		pDigitView->RedrawWindow();
		pWaveView->OnDrawPolyLine();
	}*/	
}


void CADParaCfgView::SetInputRangeEx(int nInputRange, int nChannel)
{
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndWave)); 
	switch(nInputRange)
	{
	case USB2831_INPUT_N10000_P10000mV:
		gl_InputRange[nChannel].nRangeIndex = USB2831_INPUT_N10000_P10000mV;
		gl_InputRange[nChannel].nVoltRange = 20000;
		gl_PerLsbVolt[nChannel] = (float)(20000.00 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
		gl_ScreenVolume = 20000;
		gl_voltVolume = 20000;
		break;
	case USB2831_INPUT_N5000_P5000mV:
		gl_InputRange[nChannel].nRangeIndex = USB2831_INPUT_N5000_P5000mV;
		gl_InputRange[nChannel].nVoltRange = 10000;
		gl_PerLsbVolt[nChannel] = (float)(10000.00 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
		gl_ScreenVolume = 10000;
		gl_voltVolume = 10000;
		break;
	case USB2831_INPUT_N2500_P2500mV:
		gl_InputRange[nChannel].nRangeIndex = USB2831_INPUT_N2500_P2500mV;
		gl_InputRange[nChannel].nVoltRange = 5000;
		gl_PerLsbVolt[nChannel] = (float)(5000.00 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-2500, 2500, nChannel);
		gl_ScreenVolume = 5000;
		gl_voltVolume = 5000;
		break;
	case USB2831_INPUT_0_P10000mV:
		gl_InputRange[nChannel].nRangeIndex = USB2831_INPUT_0_P10000mV;
		gl_InputRange[nChannel].nVoltRange = 10000;
		gl_PerLsbVolt[nChannel] = (float)(10000.00 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(0, 10000, nChannel);
		gl_ScreenVolume = 10000;
		gl_voltVolume = 10000;
		break;
	default:
		break;
	}
}

// void CADParaCfgView::SetInputRange(int nInputRange, int nChannel)
// {
// 	CADDoc* pDoc = theApp.m_pADDoc; 
// 	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
// 	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
// 	SetInputRangeEx(nInputRange, nChannel);
// 	gl_InputRange[nChannel].nRangeIndex = nInputRange;
// 	pDigitView->Invalidate();
// }

//######################################################################
// 设置外触发类型 (电平触发或沿触发)
void CADParaCfgView::OnSelchangeCOMBOTriggerType() 
{
	ADPara.TriggerType = m_Combo_TriggerType.GetCurSel();
}

//#########################################################################
// 时钟源选择 (内时钟或外时钟)
void CADParaCfgView::OnSelchangeCOMBOClockSource() 
{
	ADPara.ClockSource = m_Combo_ClockSource.GetCurSel();
	if (ADPara.ClockSource == USB2831_CLOCKSRC_IN)
		m_Combo_Frequency.EnableWindow(TRUE);
	else
		m_Combo_Frequency.EnableWindow(FALSE);
}
 
// 通过频率值来设置频率设置项的选择
void CADParaCfgView::SetFreqComboSel(int nFrequecy)
{
	CString strFrequecy;
	strFrequecy.Format(_T("%d "), nFrequecy); // %d后面的空格不能少!
	m_Combo_Frequency.SelectString(-1, strFrequecy);
	ADPara.Frequency = m_nAsynFrequency;    // 设置异步时的频率值
}

// 设置底下的状态栏
void CADParaCfgView::SetStatusBar()
{
	CString strTimeDiv, strFrequency;
	m_Combo_Frequency.GetWindowText(strFrequency);
	long nFrequency = (long)wcstol(strFrequency, NULL, 10);
	ADPara.Frequency = nFrequency;
	float fTimePixel = (float)(1000000.0/nFrequency); // 求每两点间的周期(uS)
	float fTimeDiv = fTimePixel * 50; // 求每大格的周期(每大格为50个象素)
	if (fTimeDiv < 1000.0)
		strTimeDiv.Format(_T("%7.2f uS/Div"), fTimeDiv);
	if ((fTimeDiv >= 1000.0) && (fTimeDiv < 1000000.0))
		strTimeDiv.Format(_T("%7.2f mS/Div"), fTimeDiv/1000);
	if (fTimeDiv >= 1000000.0)
		strTimeDiv.Format(_T("%7.2f S/Div"), fTimeDiv/1000000);
    m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv)->SetWindowText(strTimeDiv);
}

void CADParaCfgView::OnCHECKClockOutput() 
{
	ADPara.bClockOutput = m_Check_bClockOutput.GetCheck(); // 否允许时钟输出
}

void CADParaCfgView::OnKillfocusCOMBOFrequency()
{
	CString strFrequency;
	m_Combo_Frequency.GetWindowText(strFrequency);
	long nFrequency = (long)wcstol(strFrequency, NULL, 10);
	if (nFrequency > 260000 || nFrequency < 30)
	{
		CRect rect;
		m_Combo_Frequency.GetWindowRect(&rect);
		strMsg.Format(_T("%s"),_T("采样频率应大于30HZ\n小于260000Hz"));
		theApp.MsgWarning("警告", strMsg, rect.CenterPoint(), 10000);
		strFrequency.Format(_T("%d"), ADPara.Frequency);
		m_Combo_Frequency.SetWindowText(strFrequency);
		return;
	}
	ADPara.Frequency = nFrequency*8; // 设置频率值
	SaveFrequency(nFrequency);
	SetStatusBar(); // 设置底下的状态栏
}

BOOL CADParaCfgView::SaveFrequency(LONG Frequency)
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	if(Frequency < 1) return FALSE;
	CString strFreqKey, strFrequency, strTmp;
	BOOL bCompare = TRUE;
	LONG TFrequency = 0;
	int KeyIndex = 0;
	int nCount = m_Combo_Frequency.GetCount();
	while(TRUE) // 先寻找有无相同值项
	{
		strFreqKey.Format(_T("Frequency%d"), KeyIndex);
		//TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFrequency,0x00);
		TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFreqKey,0x00);
		if(TFrequency == Frequency) // 若要保存的频率信息在注册表中已存在
		{
			return TRUE; // 若存在，则直接返回
		}
		KeyIndex++;
		if(KeyIndex >= 256)
			break;  // 若寻找256项，都未有，则认为不存在任何重复键,退出搜寻
	}
	KeyIndex = 0;
	while(TRUE) // 在没有相同值项的情况下，寻找空缺的键项
	{
		strFreqKey.Format(_T("Frequency%d"), KeyIndex);
		TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFrequency,0x00);
		if(TFrequency == 0x00) // 若返回默认值，则认为无此键，那么就以此键存入新值
		{
			pApp->WriteProfileInt(_T("strSectionFre"),strFreqKey,Frequency);
			strFrequency.Format(_T("%d"), Frequency);
			for (int nIndex=0; nIndex<nCount; nIndex++)
			{
				m_Combo_Frequency.GetLBText(nIndex, strTmp);
				LONG lTmp = wcstol(strTmp, NULL, 10);
				if (Frequency > lTmp)
				{
					bCompare = FALSE;
					m_Combo_Frequency.InsertString(nIndex, strFrequency);
					break;
				}
			}
			// 如果输入的新品率为最小值
			if (bCompare)
			{
				m_Combo_Frequency.InsertString(nCount, strFrequency);
			}
			break;
		}
		KeyIndex++;
		if(KeyIndex >= 256) break;
	}
	return TRUE;
}


LONG CADParaCfgView::LoadFrequency(LONG Frequency[])
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	CString strFrequency;
	LONG TFrequency = 0;
	BOOL bHas = FALSE;
	int KeyIndex = 0, FreqIndex = 0;

	KeyIndex = 0;
	FreqIndex = 0;
	while(TRUE) // 在没有相同值项的情况下，寻找空缺的键项
	{
		strFrequency.Format(_T("Frequency%d"), KeyIndex);
		TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFrequency,0x00);
		if(TFrequency != 0x00) // 若返回默认值，则认为无此键，那么就以此键存入新值
		{
			Frequency[FreqIndex] = TFrequency;
			FreqIndex++;
		}
		KeyIndex++;
		if(KeyIndex >= 256) break;
	}
	return FreqIndex;
}

void CADParaCfgView::OnSelchangeCOMBOFirstChannel() 
{
	int ntemp = m_Combo_FirstChannel.GetCurSel();
	if (ntemp>ADPara.LastChannel)
	{
		CRect rect;
		m_Combo_FirstChannel.GetWindowRect(&rect);
		strMsg.Format(_T("%s"),_T("首通道号必须小于等于末通道号"));
		theApp.MsgWarning("错误警告", strMsg, rect.CenterPoint(), 10000);
		m_Combo_FirstChannel.SetCurSel(ADPara.FirstChannel);
	}
	else
	{
		ADPara.FirstChannel = ntemp;
		//gl_nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1;
		CString str;
		str.Format(_T("%d"), gl_nChannelCount);
		m_Edit_ChannelSum.SetWindowText(str);
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndWave)); 
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndDigit)); 
		pWaveView->m_ADScopeCtrl.InvalidateCtrl();
		pDigitView->Invalidate();
		m_Slider_Offset.SetRange(0, 8192 / gl_nChannelCount);
		m_Slider_Offset.SetPos(0);
	}
}

void CADParaCfgView::OnSelchangeCOMBOLastChannel() 
{
	int ntemp = m_Combo_LastChannel.GetCurSel();
	if (ntemp<ADPara.FirstChannel)
	{
		CRect rect;
		m_Combo_LastChannel.GetWindowRect(&rect);
		strMsg.Format(_T("%s"),_T("请重新选择首通道和尾通道，确保首通道小于尾通道"));
		theApp.MsgWarning("错误警告", strMsg, rect.CenterPoint(), 10000);	
		m_Combo_LastChannel.SetCurSel(ADPara.LastChannel);
	}
	else
	{
		ADPara.LastChannel = ntemp;
		//gl_nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1;
		CString str;
		str.Format(_T("%d"), gl_nChannelCount);
		m_Edit_ChannelSum.SetWindowText(str);
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndWave)); 
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndDigit)); 
		pWaveView->m_ADScopeCtrl.InvalidateCtrl();
		pDigitView->Invalidate();
		m_Slider_Offset.SetRange(0, 8192 / gl_nChannelCount);
		m_Slider_Offset.SetPos(0);
	}
}


void CADParaCfgView::OnSelchangeCOMBORange()
{
	CRect rect;
	CString str;
	m_Combo_Range.GetWindowText(str);

	int nInputRagne = m_Combo_Range.GetCurSel();
	ADPara.InputRange = nInputRagne;
	for (int Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++) // 初始化电压量程
	{
		gl_InputRange[Channel].nRangeIndex = nInputRagne;
		SetInputRangeEx(gl_InputRange[Channel].nRangeIndex, Channel);
	}
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndDigit)); 
	pDigitView->TransitionData();
	pDigitView->Invalidate();
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
}

void CADParaCfgView::OnSelchangeCOMBOGains() 
{
	ADPara.Gains = m_Combo_Gains.GetCurSel();
}


// void CADParaCfgView::OnSelchangeCOMBOGainsChip() 
// {
// // 	int nGainsChip = m_Combo_Gains_Chip.GetCurSel();
// 	int nGains = m_Combo_Gains.GetCurSel();
// 	for (int nIndex=0; nIndex<4; nIndex++)
// 	{
// 		m_Combo_Gains.DeleteString(0);
// 	}
// 	switch(nGainsChip)
// 	{
// 	case 0:
// 		m_Combo_Gains.AddString("1");
// 		m_Combo_Gains.AddString("10");
// 		m_Combo_Gains.AddString("100");
// 		m_Combo_Gains.AddString("1000");
// 		break;
// 	case 1:
// 		m_Combo_Gains.AddString("1");
// 		m_Combo_Gains.AddString("2");
// 		m_Combo_Gains.AddString("4");
// 		m_Combo_Gains.AddString("8");
// 	    break;
// 	default:
// 	    break;
// 	}
// 	m_Combo_Gains.SetCurSel(nGains);
// }


void CADParaCfgView::OnSelchangeCOMBOCheckStsMode() 
{
	ADPara.CheckStsMode = m_Combo_CheckStsMode.GetCurSel();	
}


void CADParaCfgView::OnSelchangeCOMBOTriggerSource() 
{
	ADPara.TriggerSource = m_Combo_TriggerSource.GetCurSel();	
}


void CADParaCfgView::OnSelchangeCOMBOADMode() 
{
	ADPara.ADMode = m_Combo_ADMode.GetCurSel();
}


void CADParaCfgView::OnChangeEDITGroupInterval() 
{
	CString str;
	m_Edit_GroupInterval.GetWindowText(str);
	ADPara.GroupInterval = wcstol(str, NULL, 10);
}


void CADParaCfgView::OnChangeEDITLoopsOfGroup() 
{
	CString str;
	m_Edit_LoopsOfGroup.GetWindowText(str);
	ADPara.LoopsOfGroup = wcstol(str, NULL, 10);
}

void CADParaCfgView::OnChangeEDITTrigWindow() 
{
	CString str;
	m_Edit_TrigWindow.GetWindowText(str);
	int Temp = wcstol(str, NULL, 10);
	CRect rect;
	m_Edit_TrigWindow.GetWindowRect(&rect);
	if (Temp<1)
	{
		Temp = 1;
		theApp.MsgWarning("错误警告", "1~255", rect.CenterPoint(), 10000);
	}
	if (Temp>255)
	{
		Temp = 255;
		theApp.MsgWarning("错误警告", "1~255", rect.CenterPoint(), 10000);
	}
	ADPara.TrigWindow = Temp;
}

void CADParaCfgView::OnSelchangeCOMBOGroundingMode() 
{
	// TODO: Add your control notification handler code here
	int i;
	ADPara.GroundingMode = m_Combo_GroundingMode.GetCurSel();
	for ( i=0; i<MAX_CHANNEL_COUNT; i++)
	{
		m_Combo_FirstChannel.DeleteString(0);
		m_Combo_LastChannel.DeleteString(0);
	}
	
	CString str;
	if (ADPara.GroundingMode == USB2831_GNDMODE_DI)
	{
		for (i=0; i<MAX_CHANNEL_COUNT/2; i++)
		{
			str.Format(_T("%d"), i);
			m_Combo_FirstChannel.AddString(str);
			m_Combo_LastChannel.AddString(str);
		}
		ADPara.FirstChannel = 0;
		ADPara.LastChannel = 3;
		m_Combo_FirstChannel.SetCurSel(ADPara.FirstChannel);
		m_Combo_LastChannel.SetCurSel(ADPara.LastChannel);
	}
	else
	{
		
		for (i=0; i<MAX_CHANNEL_COUNT; i++)
		{
			str.Format(_T("%d"), i);
			m_Combo_FirstChannel.AddString(str);
			m_Combo_LastChannel.AddString(str);
		}
		m_Combo_FirstChannel.SetCurSel(ADPara.FirstChannel);
		m_Combo_LastChannel.SetCurSel(ADPara.LastChannel);
	}
	
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(theApp.m_pADDoc->m_hWndDigit)); 
	pDigitView->Invalidate();
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();


}


void CADParaCfgView::OnBnClickedCheckInput1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CADParaCfgView::OnBnClickedCheckInput9()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_singal_1.GetCheck())
	{
		m_singal_1.SetCheck(TRUE);
		return;
	}

	if(device_start_ok)
	{
		m_singal_1.SetCheck(gl_signal_enable[0]);
		return;
	}

	if(m_singal_1.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[0] = 1;
	}
	else
	{
		gl_signal_enable[0] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}


void CADParaCfgView::OnBnClickedCheckInput10()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_singal_2.GetCheck())
	{
		m_singal_2.SetCheck(TRUE);
		return;
	}
	if(device_start_ok)
	{
		m_singal_2.SetCheck(gl_signal_enable[1]);
		return;
	}
	if(m_singal_2.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[1] = 1;
	}
	else
	{
		gl_signal_enable[1] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}


void CADParaCfgView::OnBnClickedCheckInput11()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_singal_3.GetCheck())
	{
		m_singal_3.SetCheck(TRUE);
		return;
	}
	if(device_start_ok)
	{
		m_singal_3.SetCheck(gl_signal_enable[2]);
		return;
	}
	if(m_singal_3.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[2] = 1;
	}
	else
	{
		gl_signal_enable[2] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}


void CADParaCfgView::OnBnClickedCheckInput12()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_singal_4.GetCheck())
	{
		m_singal_4.SetCheck(TRUE);
		return;
	}
	if(device_start_ok)
	{
		m_singal_4.SetCheck(gl_signal_enable[3]);
		return;
	}
	if(m_singal_4.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[3] = 1;
	}
	else
	{
		gl_signal_enable[3] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}


void CADParaCfgView::OnBnClickedCheckInput13()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_input_1.GetCheck())
	{
		m_input_1.SetCheck(TRUE);
		return;
	}
	if(device_start_ok)
	{
		m_input_1.SetCheck(gl_signal_enable[4]);
		return;
	}
	if(m_input_1.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[4] = 1;
	}
	else
	{
		gl_signal_enable[4] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}


void CADParaCfgView::OnBnClickedCheckInput14()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_input_2.GetCheck())
	{
		m_input_2.SetCheck(TRUE);
		return;
	}
	if(device_start_ok)
	{
		m_input_2.SetCheck(gl_signal_enable[5]);
		return;
	}
	if(m_input_2.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[5] = 1;
	}
	else
	{
		gl_signal_enable[5] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}


void CADParaCfgView::OnBnClickedCheckInput15()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_input_3.GetCheck())
	{
		m_input_3.SetCheck(TRUE);
		return;
	}
	if(device_start_ok)
	{
		m_input_3.SetCheck(gl_signal_enable[6]);
		return;
	}
	if(m_input_3.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[6] = 1;
	}
	else
	{
		gl_signal_enable[6] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}


void CADParaCfgView::OnBnClickedCheckInput16()
{
	// TODO: 在此添加控件通知处理程序代码
	if(gl_nChannelCount == 1 && !m_input_4.GetCheck()) 
	{
		m_input_4.SetCheck(TRUE);
		return;
	}
	if(device_start_ok)
	{
		m_input_4.SetCheck(gl_signal_enable[7]);
		return;
	}
	if(m_input_4.GetCheck())
	{
		gl_nChannelCount += 1;
		gl_signal_enable[7] = 1;
	}
	else
	{
		gl_signal_enable[7] = 0;
		gl_nChannelCount -= 1;
	}
	CString str;
	str.Format(_T("%d"), gl_nChannelCount);
	m_Edit_ChannelSum.SetWindowText(str);
	save_channel();
}

static int TimeAxisRangeVal[]={10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000};//以ms为单位
static int TimeAxisRangeID=6;//初始为1s钟


//时间量程减小
void CADParaCfgView::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (TimeAxisRangeID==0)
	{
		//TimeAxisRangeID=sizeof(TimeAxisRangeVal)/4-1;
		return;
	}
	else
	{
		TimeAxisRangeID--;
	}
	

	g_nTimeAxisRange = TimeAxisRangeVal[TimeAxisRangeID]*1000;
	CString tmp;
	tmp.Format(_T("%d"),TimeAxisRangeVal[TimeAxisRangeID]);
	m_TimeAxisRange.SetWindowTextW(tmp);

	//新旧量程的比例
	int lastRange = TimeAxisRangeVal[TimeAxisRangeID+1];
	int nowRange =TimeAxisRangeVal[TimeAxisRangeID];
	int i,j,temp;
	

	for (i=SHOW_DATA_CNT-1;i>0;i--)
	{
		for (j=0;j<8;j++)
		{
			temp=i*nowRange/lastRange;
			showData[j][i] = showData[j][temp];
		}
	}
	if(gl_last_end_id < 5000)
		gl_last_end_id = gl_last_end_id*2;
}

//时间量程增大
void CADParaCfgView::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
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
	m_TimeAxisRange.SetWindowTextW(tmp);

	//新旧量程的比例
	int lastRange = TimeAxisRangeVal[TimeAxisRangeID-1];
	int nowRange =TimeAxisRangeVal[TimeAxisRangeID];

	int i,j,temp;
	for (i=0;i<SHOW_DATA_CNT*lastRange/nowRange;i++)
	{
		for (j=0;j<8;j++)
		{
			temp=i*nowRange/lastRange;
			showData[j][i] = showData[j][temp];
		}

	}
	for (i=SHOW_DATA_CNT*lastRange/nowRange;i<SHOW_DATA_CNT;i++)
	{
		for (j=0;j<8;j++)
		{
			showData[j][i] = 0;
		}
	}
}





static int VAxisRangeVal[]={10,20,50,100,200,500,1000,2000,5000,10000};//以mv为单位
static int VAxisRangeID=9;//初始为1s钟
void CADParaCfgView::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(VAxisRangeID > 0)
		VAxisRangeID--;
	//else
	//	return;

	CString tmp;
	tmp.Format(_T("%d"),VAxisRangeVal[VAxisRangeID]);
	m_VAxisRange.SetWindowTextW(tmp);
	g_nVAxisRange = VAxisRangeVal[VAxisRangeID];
}

void CADParaCfgView::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(VAxisRangeID < 9 )
		VAxisRangeID++;
	/*else
		return;
*/
	CString tmp;
	tmp.Format(_T("%d"),VAxisRangeVal[VAxisRangeID]);
	m_VAxisRange.SetWindowTextW(tmp);

	g_nVAxisRange = VAxisRangeVal[VAxisRangeID];
}


void CADParaCfgView::Change_Offset(void)
{
	CString str;
	int offset = 0;
	
	for (int i = 0; i < 8; i++)
	{
		offset = 0;
		m_singal_offset[i].GetWindowText(str);
		offset = wcstol(str, NULL, 10);
		offset =offset*4096/5000;
		g_y_offset[i] = offset;
	}
}