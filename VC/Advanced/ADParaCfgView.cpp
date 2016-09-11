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
		m_nSyncInputRange[iChannel] = 0; // ��ʼ��ͬ���ĵ�ѹ����
		m_nAsynInputRange[iChannel] = 0; // ��ʼ���첽�ĵ�ѹ����
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
// ��dataָ��ָ��Ļ��������±�Ϊleft��right��Ԫ�ؽ��� ��������
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
		if(i <= j) { //����i, j ��ֵ
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


	// ��ʼ���û������Ӿ�����		
	CString str;
//	OldADPara = ADPara; // ����ɵ�Ӳ������
	//ADPara.CheckStsMode = 1; // Ĭ�����ݲ�ѯ��ʽΪ�ǿ�
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

	// ��ʼ����������
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
	m_Combo_TriggerType.SetCurSel(ADPara.TriggerType); // ������ʽ(��ƽ|����)
	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir); // ��ʼ���ⴥ������
	m_Combo_TriggerSource.SetCurSel(ADPara.TriggerSource);

	m_Static_Offset.SetWindowText(_T("0"));
	m_Spin_BufferID.SetRange(0, 63);	   // ��ʼ������η�Χ
	m_Spin_BufferID.SetBuddy(&m_Edit_BufferID);
	m_Spin_BufferID.SetPos(0);
	m_Slider_Offset.SetRange(0, 8192 / gl_nChannelCount); // ��ʼ���������ƫ�Ʒ�Χ

	m_Combo_ClockSource.SetCurSel(ADPara.ClockSource); // ��ʼ��ʱ��Դ
	m_Check_bClockOutput.SetCheck(ADPara.bClockOutput);

	SetStatusBar();

	m_Combo_Range.SetCurSel(ADPara.InputRange);
	int Channel = 0;
	for (Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++) // ��ʼ����ѹ����
	{
		gl_InputRange[Channel].nRangeIndex = USB2831_INPUT_N10000_P10000mV;
		SetInputRangeEx(gl_InputRange[Channel].nRangeIndex, Channel);
	}

	m_bInitialUpdate = TRUE;
	theApp.m_pADDoc->m_hADParaCfgView = this;


	// <<--------------------------------------------------------->>
	// ����������ʾ��
	//HICON hIcon = static_cast<HICON>(::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16,16, LR_DEFAULTCOLOR));
	//theApp.m_BalloonHelp.SetIcon(hIcon);
	//::DestroyIcon(hIcon);
	
	//CFont* pFont = new CFont;
	//pFont->CreatePointFont(120, _T("��Բ(TT)"));
	//theApp.m_BalloonHelp.SetTitleFont(pFont);   // deleted by balloon.
	
	//CRect rect;
	//GetClientRect(&rect);
	//m_Edit_GetDevTrigPosAD.GetWindowRect(&rect);
	
	//theApp.m_BalloonHelp.DestroyBalloon();

	//theApp.m_BalloonHelp.Create("����!", "��ӭ���밢��̩�����ʾϵͳ!", rect.CenterPoint(),	// rect.BottomRight()
	//	CBalloonHelp::unSHOW_INNER_SHADOW|CBalloonHelp::unSHOW_CLOSE_BUTTON|
	//	CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN, 
	//	this);
	
	//pDoc->m_RectBalloon = rect;	

	OnSelchangeCOMBORange();
	// <<--------------------------------------------------------->>
	
}

//###########################################################################
// ���ô���Դ (�ڴ������ⴥ��)
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
	m_Combo_TriggerDir.EnableWindow(Flag);    // �ⴥ����	
	m_Combo_ClockSource.EnableWindow(Flag);
	m_Combo_Range.EnableWindow(Flag);
	m_Combo_GroundingMode.EnableWindow(Flag);
	
	m_Edit_TrigWindow.EnableWindow(Flag);
	m_Edit_GroupInterval.EnableWindow(Flag);
	m_Edit_LoopsOfGroup.EnableWindow(Flag);

	if (gl_nProcMode == PROC_MODE_SAVE) // ����Ǵ��̲���
	{
		m_Edit_BufferID.EnableWindow(Flag);
		m_Spin_BufferID.EnableWindow(Flag);
	}
	m_Combo_TriggerSource.EnableWindow(Flag);
	m_Combo_TriggerType.EnableWindow(Flag);
}

//######################################################################################
// �ⴥ����������
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
	if (m_bInitialUpdate) // �����������OnInitialUpdate�����ѵ��ú�
	{ 
		//CSliderCtrl* pSliderOffset = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_Offset));	
		//int Pos = pSliderOffset->GetPos();  // ȡ�û�����λ��
		int Pos = m_Slider_Offset.GetPos();  // ȡ�û�����λ��
 		gl_Offset = Pos;	// gl offset ��־�ڻ������е�ƫ��
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
		
		pWaveView->OnDrawPolyLine(); // ���²�����ͼ
		pDigitView->SetScrollPos(SB_VERT, gl_Offset * 16);  // ����������ͼ
		pDigitView->Invalidate();
	//	pDigitView->RedrawWindow();  // ����������ͼ
		
		CADFrm* pADFrm = (CADFrm*)pWaveView->GetParentFrame();
		CEdit* pEditOffset = (CEdit*)(pADFrm->m_wndShowStatus).GetDlgItem(IDC_EDIT_Offset); // ȡ��IDC_STATIC_ShowXY���
// 		int Count = gl_Offset+gl_WavePointX;  // ƫ���ܵ���
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
	gl_BufferID = wcstol(string, NULL, 10); // �ı�ADBuffer�Ļ���κ�
	if (gl_BufferID<0 || gl_BufferID>63)
	{	
		gl_BufferID = 0;
		pEditBufferID->SetWindowText(_T("0"));
	}
	/*
	CADDoc* pADDoc = (CADDoc*)theApp.m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pADDoc->m_hWndWave));
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pADDoc->m_hWndDigit));
	int ReadSizeWords = gl_BufferID * 8192; // RAMƫ��λ�ã���8KΪ��λ
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
// �����ⴥ������ (��ƽ�������ش���)
void CADParaCfgView::OnSelchangeCOMBOTriggerType() 
{
	ADPara.TriggerType = m_Combo_TriggerType.GetCurSel();
}

//#########################################################################
// ʱ��Դѡ�� (��ʱ�ӻ���ʱ��)
void CADParaCfgView::OnSelchangeCOMBOClockSource() 
{
	ADPara.ClockSource = m_Combo_ClockSource.GetCurSel();
	if (ADPara.ClockSource == USB2831_CLOCKSRC_IN)
		m_Combo_Frequency.EnableWindow(TRUE);
	else
		m_Combo_Frequency.EnableWindow(FALSE);
}
 
// ͨ��Ƶ��ֵ������Ƶ���������ѡ��
void CADParaCfgView::SetFreqComboSel(int nFrequecy)
{
	CString strFrequecy;
	strFrequecy.Format(_T("%d "), nFrequecy); // %d����Ŀո�����!
	m_Combo_Frequency.SelectString(-1, strFrequecy);
	ADPara.Frequency = m_nAsynFrequency;    // �����첽ʱ��Ƶ��ֵ
}

// ���õ��µ�״̬��
void CADParaCfgView::SetStatusBar()
{
	CString strTimeDiv, strFrequency;
	m_Combo_Frequency.GetWindowText(strFrequency);
	long nFrequency = (long)wcstol(strFrequency, NULL, 10);
	ADPara.Frequency = nFrequency;
	float fTimePixel = (float)(1000000.0/nFrequency); // ��ÿ����������(uS)
	float fTimeDiv = fTimePixel * 50; // ��ÿ��������(ÿ���Ϊ50������)
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
	ADPara.bClockOutput = m_Check_bClockOutput.GetCheck(); // ������ʱ�����
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
		strMsg.Format(_T("%s"),_T("����Ƶ��Ӧ����30HZ\nС��260000Hz"));
		theApp.MsgWarning("����", strMsg, rect.CenterPoint(), 10000);
		strFrequency.Format(_T("%d"), ADPara.Frequency);
		m_Combo_Frequency.SetWindowText(strFrequency);
		return;
	}
	ADPara.Frequency = nFrequency*8; // ����Ƶ��ֵ
	SaveFrequency(nFrequency);
	SetStatusBar(); // ���õ��µ�״̬��
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
	while(TRUE) // ��Ѱ��������ֵͬ��
	{
		strFreqKey.Format(_T("Frequency%d"), KeyIndex);
		//TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFrequency,0x00);
		TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFreqKey,0x00);
		if(TFrequency == Frequency) // ��Ҫ�����Ƶ����Ϣ��ע������Ѵ���
		{
			return TRUE; // �����ڣ���ֱ�ӷ���
		}
		KeyIndex++;
		if(KeyIndex >= 256)
			break;  // ��Ѱ��256���δ�У�����Ϊ�������κ��ظ���,�˳���Ѱ
	}
	KeyIndex = 0;
	while(TRUE) // ��û����ֵͬ�������£�Ѱ�ҿ�ȱ�ļ���
	{
		strFreqKey.Format(_T("Frequency%d"), KeyIndex);
		TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFrequency,0x00);
		if(TFrequency == 0x00) // ������Ĭ��ֵ������Ϊ�޴˼�����ô���Դ˼�������ֵ
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
			// ����������Ʒ��Ϊ��Сֵ
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
	while(TRUE) // ��û����ֵͬ�������£�Ѱ�ҿ�ȱ�ļ���
	{
		strFrequency.Format(_T("Frequency%d"), KeyIndex);
		TFrequency = pApp->GetProfileInt(_T("strSectionFre"),strFrequency,0x00);
		if(TFrequency != 0x00) // ������Ĭ��ֵ������Ϊ�޴˼�����ô���Դ˼�������ֵ
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
		strMsg.Format(_T("%s"),_T("��ͨ���ű���С�ڵ���ĩͨ����"));
		theApp.MsgWarning("���󾯸�", strMsg, rect.CenterPoint(), 10000);
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
		strMsg.Format(_T("%s"),_T("������ѡ����ͨ����βͨ����ȷ����ͨ��С��βͨ��"));
		theApp.MsgWarning("���󾯸�", strMsg, rect.CenterPoint(), 10000);	
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
	for (int Channel=0; Channel<USB2831_MAX_AD_CHANNELS; Channel++) // ��ʼ����ѹ����
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
		theApp.MsgWarning("���󾯸�", "1~255", rect.CenterPoint(), 10000);
	}
	if (Temp>255)
	{
		Temp = 255;
		theApp.MsgWarning("���󾯸�", "1~255", rect.CenterPoint(), 10000);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CADParaCfgView::OnBnClickedCheckInput9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

static int TimeAxisRangeVal[]={10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000};//��msΪ��λ
static int TimeAxisRangeID=6;//��ʼΪ1s��


//ʱ�����̼�С
void CADParaCfgView::OnBnClickedButton1()
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
	

	g_nTimeAxisRange = TimeAxisRangeVal[TimeAxisRangeID]*1000;
	CString tmp;
	tmp.Format(_T("%d"),TimeAxisRangeVal[TimeAxisRangeID]);
	m_TimeAxisRange.SetWindowTextW(tmp);

	//�¾����̵ı���
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

//ʱ����������
void CADParaCfgView::OnBnClickedButton3()
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
	m_TimeAxisRange.SetWindowTextW(tmp);

	//�¾����̵ı���
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





static int VAxisRangeVal[]={10,20,50,100,200,500,1000,2000,5000,10000};//��mvΪ��λ
static int VAxisRangeID=9;//��ʼΪ1s��
void CADParaCfgView::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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