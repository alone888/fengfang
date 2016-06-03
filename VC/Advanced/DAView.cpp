// DAView.cpp : implementation file
#include "stdafx.h"
#include "sys.h"
#include "DAView.h"
#include "DADoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDAView
WORD DigitValue[8192];
DWORD FrequenceDA=50000;
UINT WriteDAThread(PVOID pPara); // 读数据线程
CRect m_PictureRect;
POINT pointxy[4096];
BOOL DisplayFlag;

#define WM_DRAWPICTURE WM_USER+1

UINT IDC_SLIDER[]=
{//四个通道的滑动条ID
	IDC_SLIDER_0, IDC_SLIDER_1, IDC_SLIDER_2, IDC_SLIDER_3
};

UINT IDC_EDIT_Cycle111[]=
{//正弦波周期点
	IDC_EDIT_Cycle0,IDC_EDIT_Cycle1,IDC_EDIT_Cycle2,IDC_EDIT_Cycle3
};

UINT IDC_CHECK_WaveDA[]=
{//只输出恒定值
	IDC_CHECK_Wave0,IDC_CHECK_Wave1,IDC_CHECK_Wave2,IDC_CHECK_Wave3
};

UINT IDC_EDIT_Value111[]=
{//恒定值
IDC_EDIT_Value0,IDC_EDIT_Value1,IDC_EDIT_Value2,IDC_EDIT_Value3
};

UINT IDC_PICTURE_WAVE[]=
{//波形显示图
IDC_PICTURE0,IDC_PICTURE1,IDC_PICTURE2,IDC_PICTURE3
};

UINT IDC_BUTTON_StartDA[]=
{//开始输出
IDC_BUTTON_StartWriteDA0,IDC_BUTTON_StartWriteDA1,IDC_BUTTON_StartWriteDA2,IDC_BUTTON_StartWriteDA3
};

UINT IDC_BUTTON_StopDA[]=
{//停止输出
IDC_BUTTON_StopWriteDA0,IDC_BUTTON_StopWriteDA1,IDC_BUTTON_StopWriteDA2,IDC_BUTTON_StopWriteDA3
};

UINT IDC_SPIN_Cycle[]=
{
IDC_SPIN_CycleDA0,IDC_SPIN_CycleDA1,IDC_SPIN_CycleDA2,IDC_SPIN_CycleDA3
};

UINT IDC_SPIN_Value111[]=
{
IDC_SPIN_Value0,IDC_SPIN_Value1,IDC_SPIN_Value2,IDC_SPIN_Value3
};

// 电压值
UINT IDC_EDIT_VoltValue[]=
{
	IDC_EDIT_Volt0, IDC_EDIT_Volt1, IDC_EDIT_Volt2, IDC_EDIT_Volt3
};

#define m_pEdit_Cycle(nNum)		((CEdit*)GetDlgItem(IDC_EDIT_Cycle111[nNum])) //正弦波周期点

#define m_pCheck_Wave(nNum)		((CButton*)GetDlgItem(IDC_CHECK_WaveDA[(nNum)]))//只输出恒定值

#define m_pEdit_Value(nNum)		((CEdit*)GetDlgItem(IDC_EDIT_Value111[(nNum)]))//恒定值

#define m_pButton_Start(nNum)	((CButton*)GetDlgItem(IDC_BUTTON_StartDA[(nNum)]))//开始输出按钮

#define m_pButton_Stop(nNum)	((CButton*)GetDlgItem(IDC_BUTTON_StopDA[(nNum)]))//停止输出按钮

#define m_pPictrue(nNum)		((CStatic*)GetDlgItem(IDC_PICTURE_WAVE[(nNum)]))//波形显示图

#define m_pSpinValue(nNum)		((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_Value111[(nNum)]))

#define m_pSpinCycle(nNum)		((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_Cycle[(nNum)]))

#define m_pSlider(nNum)			((CSliderCtrl*)GetDlgItem(IDC_SLIDER[(nNum)]))

#define m_pVolt(nNum)			((CEdit*)GetDlgItem(IDC_EDIT_VoltValue[(nNum)]))	// 电压值

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDAView, CFormView)
CDAView::CDAView()
	: CFormView(CDAView::IDD)
{
	//{{AFX_DATA_INIT(CDAView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bStartThread[0] = FALSE;
	m_bStartThread[1] = FALSE;
	m_bStartThread[2] = FALSE;
	m_bStartThread[3] = FALSE;
	m_PerLsbVolt = 20000.0/4096.0;
	m_VoltCenter = 10000.0;
	m_IntialUpdate = FALSE;
	m_bEditCycle = FALSE;
}

CDAView::~CDAView()
{
}

void CDAView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDAView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDAView, CFormView)
	//{{AFX_MSG_MAP(CDAView)
	ON_BN_CLICKED(IDC_BUTTON_StartWriteDA0, OnBUTTONStartWriteDA0)
	ON_BN_CLICKED(IDC_BUTTON_StartWriteDA1, OnBUTTONStartWriteDA1)
	ON_BN_CLICKED(IDC_BUTTON_StopWriteDA0, OnBUTTONStopWriteDA0)
	ON_BN_CLICKED(IDC_BUTTON_StopWriteDA1, OnBUTTONStopWriteDA1)
	ON_EN_CHANGE(IDC_EDIT_Cycle0, OnChangeEDITCycle0)
	ON_BN_CLICKED(IDC_CHECK_Wave0, OnCHECKWave0)
	ON_EN_CHANGE(IDC_EDIT_Cycle1, OnChangeEDITCycle1)
	ON_BN_CLICKED(IDC_CHECK_Wave1, OnCHECKWave1)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_Cycle2, OnChangeEDITCycle2)
	ON_BN_CLICKED(IDC_BUTTON_StartWriteDA2, OnBUTTONStartWriteDA2)
	ON_BN_CLICKED(IDC_CHECK_Wave2, OnCHECKWave2)
	ON_BN_CLICKED(IDC_BUTTON_StopWriteDA2, OnBUTTONStopWriteDA2)
	ON_EN_CHANGE(IDC_EDIT_Cycle3, OnChangeEDITCycle3)
	ON_BN_CLICKED(IDC_BUTTON_StartWriteDA3, OnBUTTONStartWriteDA3)
	ON_BN_CLICKED(IDC_BUTTON_StopWriteDA3, OnBUTTONStopWriteDA3)
	ON_BN_CLICKED(IDC_CHECK_Wave3, OnCHECKWave3)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_0, OnCustomdrawSlider0)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_1, OnCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_2, OnCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_3, OnCustomdrawSlider3)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange0, OnSelchangeCOMBOVoltRange0)
	ON_EN_CHANGE(IDC_EDIT_Value0, OnChangeEDITValue0)
	ON_EN_CHANGE(IDC_EDIT_Value1, OnChangeEDITValue1)
	ON_EN_CHANGE(IDC_EDIT_Value2, OnChangeEDITValue2)
	ON_EN_CHANGE(IDC_EDIT_Value3, OnChangeEDITValue3)
	ON_EN_KILLFOCUS(IDC_EDIT_Value0, OnKillfocusEDITValue0)
	ON_EN_KILLFOCUS(IDC_EDIT_Value1, OnKillfocusEDITValue1)
	ON_EN_KILLFOCUS(IDC_EDIT_Value2, OnKillfocusEDITValue2)
	ON_EN_KILLFOCUS(IDC_EDIT_Value3, OnKillfocusEDITValue3)
	ON_EN_SETFOCUS(IDC_EDIT_Value0, OnSetfocusEDITValue0)
	ON_EN_SETFOCUS(IDC_EDIT_Value1, OnSetfocusEDITValue1)
	ON_EN_SETFOCUS(IDC_EDIT_Value2, OnSetfocusEDITValue2)
	ON_EN_SETFOCUS(IDC_EDIT_Value3, OnSetfocusEDITValue3)
	ON_EN_SETFOCUS(IDC_EDIT_Cycle0, OnSetfocusEDITCycle)
	ON_EN_KILLFOCUS(IDC_EDIT_Cycle0, OnKillfocusEDITCycle)
	ON_EN_KILLFOCUS(IDC_EDIT_Cycle1, OnKillfocusEDITCycle)
	ON_EN_SETFOCUS(IDC_EDIT_Cycle1, OnSetfocusEDITCycle)
	ON_EN_KILLFOCUS(IDC_EDIT_Cycle2, OnKillfocusEDITCycle)
	ON_EN_SETFOCUS(IDC_EDIT_Cycle2, OnSetfocusEDITCycle)
	ON_EN_KILLFOCUS(IDC_EDIT_Cycle3, OnKillfocusEDITCycle)
	ON_EN_SETFOCUS(IDC_EDIT_Cycle3, OnSetfocusEDITCycle)
	ON_MESSAGE(WM_DRAWPICTURE,OnDrawPicture)
	ON_EN_CHANGE(IDC_EDIT_Volt0, OnChangeEDITVolt0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDAView diagnostics

#ifdef _DEBUG
void CDAView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDAView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDAView message handlers 
UINT WriteDAThread(PVOID pPara)  // 读数据线程
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	PTHREAD_PARA m_pPara = (PTHREAD_PARA)pPara;  
	int Index = 0, k=0;
	double t=0;
	int WriteSizeWords = 4096 - 4096%m_pPara->m_nWriteDADataPointCount;
	while(m_pPara->m_bDeviceRun)
	{	
		for(Index=0; Index<WriteSizeWords; Index++)
		{		
			if(m_pPara->m_bConstant )	//如果是恒定值
				USB2831_WriteDeviceDA(m_pPara->m_hDevice, m_pPara->m_nOutputRange, m_pPara->m_DigitValue, m_pPara->DAChannelNum);
			else
				USB2831_WriteDeviceDA(m_pPara->m_hDevice, m_pPara->m_nOutputRange, m_pPara->m_pWaveValue[Index], m_pPara->DAChannelNum);
			Sleep(1);
			if (!m_pPara->m_bDeviceRun)
			{
				break;
			}
// 			USB2831_DelayTimeUs(1);	// 延时1微秒
		}
	}
	::SetEvent(m_pPara->m_hEventEnd);
	return 1;
}


void CDAView::OnBUTTONStartWriteDA0()
{
	CDADoc* pDoc = (CDADoc*)GetDocument();
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	
	m_Para[0].m_hDevice= pApp->m_hDevice;
	m_Para[0].m_hDevice = pApp->m_hDevice;
	if(m_Para[0].m_hDevice ==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("创建设备失败!"));
		m_Para[0].m_bDeviceRun = FALSE;
		return;
	}
	m_Para[0].m_bDeviceRun=TRUE;
	m_Para[0].m_hEventEnd = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Para[0].DAChannelNum =0;
	m_Para[0].m_nOutputRange = ((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange0))->GetCurSel();
	DisplayFlag=TRUE;

	m_WriteDAThread0=AfxBeginThread(WriteDAThread,(LPVOID)&m_Para[0], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_WriteDAThread0->m_bAutoDelete=false;
	m_WriteDAThread0->ResumeThread();  // 启动采集线程ReadThread
	m_pButton_Start(0)->EnableWindow(FALSE);
	m_pButton_Stop(0)->EnableWindow(TRUE);
	
	bStart=TRUE;
	m_bStartThread[0] = TRUE;	
}

void CDAView::OnBUTTONStartWriteDA1()
{
	// TODO: Add your control notification handler code here
	CDADoc* pDoc = (CDADoc*)GetDocument();
	CSysApp *pApp=(CSysApp *)AfxGetApp();

	m_Para[1].m_hDevice= pApp->m_hDevice;
	if(m_Para[1].m_hDevice ==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("创建设备失败!"));
		m_Para[1].m_bDeviceRun = FALSE;
		return;
	}
	m_Para[1].m_bDeviceRun=TRUE;
	m_Para[1].m_hEventEnd = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Para[1].DAChannelNum =1;
	m_Para[1].m_nOutputRange = ((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange0))->GetCurSel();
	DisplayFlag=TRUE;

	m_WriteDAThread1=AfxBeginThread(WriteDAThread,(LPVOID)&m_Para[1], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_WriteDAThread1->m_bAutoDelete=false;
	m_WriteDAThread1->ResumeThread();  // 启动采集线程ReadThread
	m_pButton_Start(1)->EnableWindow(FALSE);
	m_pButton_Stop(1)->EnableWindow(TRUE);
	m_bStartThread[1] = TRUE;
}

void CDAView::OnBUTTONStartWriteDA2() 
{
	CDADoc* pDoc = (CDADoc*)GetDocument();
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	m_Para[2].m_hDevice= pApp->m_hDevice;
	if(m_Para[2].m_hDevice ==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("创建设备失败!"));
		m_Para[2].m_bDeviceRun = FALSE;
		return;
	}
	m_Para[2].m_bDeviceRun=TRUE;
	m_Para[2].m_hEventEnd = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Para[2].DAChannelNum =2;
	m_Para[2].m_nOutputRange = ((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange0))->GetCurSel();
	DisplayFlag=TRUE;

	m_WriteDAThread2=AfxBeginThread(WriteDAThread,(LPVOID)&m_Para[2], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_WriteDAThread2->m_bAutoDelete=false;
	m_WriteDAThread2->ResumeThread();  // 启动采集线程ReadThread
	m_pButton_Start(2)->EnableWindow(FALSE);
	m_pButton_Stop(2)->EnableWindow(TRUE);
	m_bStartThread[2] = TRUE;
}

void CDAView::OnBUTTONStartWriteDA3() 
{
	CDADoc* pDoc = (CDADoc*)GetDocument();
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	
	m_Para[3].m_hDevice= pApp->m_hDevice;
	if(m_Para[3].m_hDevice ==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("创建设备失败!"));
		m_Para[3].m_bDeviceRun = FALSE;
		return;
	}
	m_Para[3].m_bDeviceRun=TRUE;
	m_Para[3].m_hEventEnd = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Para[3].DAChannelNum =3;
	m_Para[3].m_nOutputRange = ((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange0))->GetCurSel();
	DisplayFlag=TRUE;

	m_WriteDAThread3=AfxBeginThread(WriteDAThread,(LPVOID)&m_Para[3], THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_WriteDAThread3->m_bAutoDelete=false;
	m_WriteDAThread3->ResumeThread();  // 启动采集线程ReadThread
	m_pButton_Start(3)->EnableWindow(FALSE);
	m_pButton_Stop(3)->EnableWindow(TRUE);
	
	m_bStartThread[3] = TRUE;
}

void CDAView::OnBUTTONStopWriteDA0() 
{
	// TODO: Add your control notification handler code here
	// 终止绘图线程
	m_Para[0].m_bDeviceRun=FALSE;
	if(m_WriteDAThread0)
	{	
		WaitForSingleObject(m_WriteDAThread0->m_hThread, INFINITE);//INFINITE);	// 等待子线程停止
	}
	
	if(m_WriteDAThread0)
	{
		m_WriteDAThread0->ExitInstance();
		m_WriteDAThread0=NULL;
	}	
	m_pButton_Start(0)->EnableWindow(TRUE);
	m_pButton_Stop(0)->EnableWindow(FALSE);	
	bStart=FALSE;
	m_bStartThread[0] = FALSE;

}

void CDAView::OnBUTTONStopWriteDA1() 
{
	// 终止绘图线程
	m_Para[1].m_bDeviceRun=FALSE;
	if(m_WriteDAThread1)
	{	
		WaitForSingleObject(m_WriteDAThread1->m_hThread, 1000);//INFINITE);	// 等待子线程停止
	}
	
	if(m_WriteDAThread1)
	{
		m_WriteDAThread1->ExitInstance();
		m_WriteDAThread1=NULL;
	}
	m_pButton_Start(1)->EnableWindow(TRUE);
	m_pButton_Stop(1)->EnableWindow(FALSE);	
	bStart=FALSE;
	m_bStartThread[1] = FALSE;

}

void CDAView::OnBUTTONStopWriteDA2() 
{	// 终止绘图线程
	m_Para[2].m_bDeviceRun=FALSE;
	if(m_WriteDAThread2)
	{	
		WaitForSingleObject(m_WriteDAThread2->m_hThread, INFINITE);//INFINITE);	// 等待子线程停止
	}
	if(m_WriteDAThread2)
	{
		m_WriteDAThread2->ExitInstance();
		m_WriteDAThread2=NULL;
	}
	m_pButton_Start(2)->EnableWindow(TRUE);
	m_pButton_Stop(2)->EnableWindow(FALSE);	
//	bStart=FALSE;
	m_bStartThread[2] = FALSE;
}

void CDAView::OnBUTTONStopWriteDA3() 
{	// 终止绘图线程
	m_Para[3].m_bDeviceRun=FALSE;
	if(m_WriteDAThread3)
	{	
		WaitForSingleObject(m_WriteDAThread3->m_hThread, INFINITE);//INFINITE);	// 等待子线程停止
	}
	if(m_WriteDAThread3)
	{
		m_WriteDAThread3->ExitInstance();
		m_WriteDAThread3=NULL;
	}	
	m_pButton_Start(3)->EnableWindow(TRUE);
	m_pButton_Stop(3)->EnableWindow(FALSE);
//	bStart=FALSE;
	m_bStartThread[3] = FALSE;
}

void CDAView::OnChangeEDITCycle0() 
{
	OnEditCycle(0);
}



void CDAView::OnCHECKWave0() 
{
	OnCheckWave(0);
}

void CDAView::OnChangeEDITCycle1() 
{
	OnEditCycle(1);
}


void CDAView::OnCHECKWave1() 
{
	OnCheckWave(1);
}

void CDAView::OnCheckWave(int nNum)
{
	if (!m_IntialUpdate) return;

	CString str;
	m_Para[nNum].m_bConstant = m_pCheck_Wave(nNum)->GetCheck();
	if(m_Para[nNum].m_bConstant)
	{
		m_pEdit_Value(nNum)->GetWindowText(str);//获得恒定值
		m_pEdit_Cycle(nNum)->EnableWindow(FALSE);
		m_pSpinCycle(nNum)->EnableWindow(FALSE);
		m_pEdit_Value(nNum)->EnableWindow(TRUE);
		m_pSpinValue(nNum)->EnableWindow(TRUE);
	}
	else
	{
		m_pEdit_Cycle(nNum)->GetWindowText(str);//获得正弦波周期点
		m_pEdit_Cycle(nNum)->EnableWindow(TRUE);
		m_pSpinCycle(nNum)->EnableWindow(TRUE);
		m_pEdit_Value(nNum)->EnableWindow(FALSE);
		m_pSpinValue(nNum)->EnableWindow(FALSE);
	}
	PostMessage(WM_DRAWPICTURE,nNum,0);
}

void CDAView::OnEditValue(int nNum)
{
	if (!m_IntialUpdate)
		return;

	CString str;
	m_pEdit_Value(nNum)->GetWindowText(str);
	int nValue = wcstol(str,NULL,10);
	if(nValue<0)
	{
		m_pEdit_Value(nNum)->SetWindowText(_T("0"));
		return;
	}
	if(nValue>4095)
	{
		m_pEdit_Value(nNum)->SetWindowText(_T("4095"));
		return;
	}
	float fDAVolt = (float)(nValue * m_PerLsbVolt - m_VoltCenter);
	str.Format(_T("%5.2f"), fDAVolt);
	m_pVolt(nNum)->SetWindowText(str);
	m_Para[nNum].m_DigitValue = nValue;
	m_pSlider(nNum)->SetPos(4095-m_Para[nNum].m_DigitValue);
	PostMessage(WM_DRAWPICTURE,nNum,0);
}

void CDAView::OnEditCycle(int nNum)
{
	if (!m_IntialUpdate) return;
	int i, k;
	CString str;
	m_pEdit_Cycle(nNum)->GetWindowText(str);
	k = (int)wcstol(str,NULL,10);
	if(k<1)
	{
		m_pEdit_Cycle(nNum)->SetWindowText(_T("0"));
		return;
	}
	if(k>4096)
	{
		m_pEdit_Cycle(nNum)->SetWindowText(_T("4096"));
		return;
	}
	m_Cycle=k;
	m_pSlider(nNum)->SetPos(4096-k);
	for(i=0; i<4096; i++)
	{
		m_Para[nNum].m_pWaveValue[i] =(WORD)(sin(6.28*i/m_Cycle)*2048+2047);
	}
	m_Para[nNum].m_nWriteDADataPointCount =4096 - 4096%m_Cycle;
	PostMessage(WM_DRAWPICTURE,nNum,0);
}

LRESULT CDAView::OnDrawPicture(WPARAM wParam,LPARAM lParam)
{
	CRect rect;

	int nNum=(int)wParam;
	m_pPictrue(nNum)->GetClientRect(&rect);
	rect.top++;
	rect.left ++;
	rect.bottom --;
	rect.right--;


	CDC* pDC=m_pPictrue(nNum)->GetDC();
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,255,255));
	pDC->FillRect(&rect,&brush);
	double middle=4096.0/(rect.Height());
	if(!m_Para[nNum].m_bConstant ) // 正弦波
	{
		POINT PointXY[1000];
		for(int i=0;i<rect.Width();i++)
		{
			PointXY[i].x =i;
			PointXY[i].y=(int)( m_Para[nNum].m_pWaveValue[i]/middle)+1;
		} // 画波形
		pDC->Polyline(PointXY,rect.Width());
	}
	else
	{	// 恒定值
		int y=(int)((4095-m_Para[nNum].m_DigitValue) /middle+1);
		pDC->MoveTo(rect.left ,y);
		pDC->LineTo(rect.right ,y);
	}
	return 1;
}

void CDAView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
//	PostMessage(WM_DRAWPICTURE,0,0);
// 	PostMessage(WM_DRAWPICTURE,1,0);
// 	PostMessage(WM_DRAWPICTURE,2,0);
// 	PostMessage(WM_DRAWPICTURE,3,0);
}

void CDAView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	m_bCustomSlider = FALSE;
	for(int nNum=0;nNum<4;nNum++)
	{
		m_pSlider(nNum)->SetRange(0,4095);
		m_pSlider(nNum)->SetPos(4095);
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange0))->SetCurSel(0);
	for(int i=0;i<4;i++)
	{
		m_pEdit_Cycle(i)->SetWindowText(_T("1"));
		m_pSpinCycle(i)->SetRange32(1,65535);
		m_pSpinCycle(i)->SetBuddy(m_pEdit_Cycle(i));
		m_pSpinCycle(i)->SetPos(m_Cycle);
		m_pEdit_Value(i)->SetWindowText(_T("0"));
		m_pSpinValue(i)->SetRange32(0,4096);
		m_pSpinValue(i)->SetBuddy(m_pEdit_Value(i));
		m_pSpinValue(i)->SetPos(m_Value);
		m_pButton_Start(i)->EnableWindow(TRUE);
 		m_pButton_Stop(i)->EnableWindow(FALSE);
		m_pSpinValue(i)->EnableWindow(FALSE);
		m_pEdit_Value(i)->EnableWindow(FALSE);
		m_Para[i].m_bConstant = FALSE; 
 		m_pVolt(i)->SetWindowText(_T("0"));
	}

	m_IntialUpdate = TRUE;
	for (int j=0; j<4; j++)
	{
		m_pEdit_Cycle(j)->SetWindowText(_T("128"));  
		OnEditCycle(j);
	}
	OnSelchangeCOMBOVoltRange0();
}

void CDAView::OnChangeEDITCycle2() 
{
	OnEditCycle(2);
}



void CDAView::OnCHECKWave2() 
{
	OnCheckWave(2);
}

void CDAView::OnChangeEDITCycle3() 
{
	OnEditCycle(3);
}

void CDAView::OnCHECKWave3() 
{
	OnCheckWave(3);
}

void CDAView::OnDestroy()
{
	CFormView::OnDestroy();
	if (m_bStartThread[0])
	{
		m_Para[0].m_bDeviceRun = FALSE;
		if(m_WriteDAThread0)
		{	
			WaitForSingleObject(m_WriteDAThread0->m_hThread, INFINITE);	// 等待子线程停止
		}
		m_WriteDAThread0=NULL;
	}

	if (m_bStartThread[1])
	{
		m_Para[1].m_bDeviceRun = FALSE;
		if(m_WriteDAThread1)
		{	
			WaitForSingleObject(m_WriteDAThread1->m_hThread, INFINITE);	// 等待子线程停止
		}
		m_WriteDAThread1=NULL;
	}

	if (m_bStartThread[2])
	{
		m_Para[2].m_bDeviceRun = FALSE;
		if(m_WriteDAThread2)
		{	
			WaitForSingleObject(m_WriteDAThread2->m_hThread, INFINITE);	// 等待子线程停止
		}
		m_WriteDAThread2=NULL;
	}

	if (m_bStartThread[3])
	{
		m_Para[3].m_bDeviceRun = FALSE;
		if(m_WriteDAThread3)
		{	
			WaitForSingleObject(m_WriteDAThread3->m_hThread, INFINITE);	// 等待子线程停止
		}
		m_WriteDAThread3=NULL;
	}
}

void CDAView::OnCustomdrawSlider0(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCustomSlider(0);
	*pResult = 0;
}

void CDAView::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCustomSlider(1);
	*pResult = 0;
}

void CDAView::OnCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCustomSlider(2);
	*pResult = 0;
}

void CDAView::OnCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCustomSlider(3);
	*pResult = 0;
}

void CDAView::OnCustomSlider(int nChannelNum)
{	// 响应滑动条滑动
	if (m_bCustomSlider)
		return;
	if (m_bEditCycle)
		return;		
	int nPos = m_pSlider(nChannelNum)->GetPos();
	m_Para[nChannelNum].m_DigitValue = nPos;
	CString str;
	if(m_pCheck_Wave(nChannelNum)->GetCheck())
	{
		str.Format(_T("%d"),4095-nPos); // 恒定值不能是4096,故用0<=4096-nPos<=4095
		m_pEdit_Value(nChannelNum)->SetWindowText(str);
	}
	else
	{
		str.Format(_T("%d"),4096-nPos); // 周期不能为0，故用,1<=4096-nPos<=4096
		m_pEdit_Cycle(nChannelNum)->SetWindowText(str);
	}
}

void CDAView::OnSelchangeCOMBOVoltRange0()
{

	
	for (int i=0; i<4; i++)
	{
		m_Para[i].m_nOutputRange = ((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange0))->GetCurSel();
	}
	switch(((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange0))->GetCurSel())
	{
	case USB2831_OUTPUT_0_P5000mV:	// 0～5000mV
		m_PerLsbVolt = (5000.0/DA_LSB_COUNT);
		m_VoltCenter = 0.0;
	    break;
	case USB2831_OUTPUT_0_P10000mV:	// 0～10000mV
		m_PerLsbVolt = (10000.0/DA_LSB_COUNT);
		m_VoltCenter = 0.0;
		break;
	case USB2831_OUTPUT_0_P10800mV:	// 0～10800mV
		m_PerLsbVolt = (10800.0/DA_LSB_COUNT);
		m_VoltCenter = 0.0;
		break;
	case USB2831_OUTPUT_N5000_P5000mV:	// ±5000mV
		m_PerLsbVolt = (10000.0/DA_LSB_COUNT);
		m_VoltCenter = 5000.0;
		break;
	case USB2831_OUTPUT_N10000_P10000mV:	// ±10000mV
		m_PerLsbVolt = (20000.0/DA_LSB_COUNT);
		m_VoltCenter = 10000.0;
		break;
	case USB2831_OUTPUT_N10800_P10800mV:	// ±10800mV
		m_PerLsbVolt = (21600.0/DA_LSB_COUNT);
		m_VoltCenter = 10800.0;
		break;
	default:
	    break;
	}
}

void CDAView::OnChangeEDITValue0() 
{
	OnEditValue(0);
}

void CDAView::OnChangeEDITValue1()
{
	OnEditValue(1);		
}

void CDAView::OnChangeEDITValue2()
{
	OnEditValue(2);
}

void CDAView::OnChangeEDITValue3() 
{
	OnEditValue(3);
}

void CDAView::OnKillfocusEDITValue0() 
{
	m_bCustomSlider = FALSE;
}

void CDAView::OnKillfocusEDITValue1()
{
	m_bCustomSlider = FALSE;
}

void CDAView::OnKillfocusEDITValue2() 
{
	m_bCustomSlider = FALSE;
}

void CDAView::OnKillfocusEDITValue3() 
{
	m_bCustomSlider = FALSE;
}

void CDAView::OnSetfocusEDITValue0() 
{
	m_bCustomSlider = TRUE;
}

void CDAView::OnSetfocusEDITValue1() 
{
	m_bCustomSlider = TRUE;	
}

void CDAView::OnSetfocusEDITValue2() 
{
	m_bCustomSlider = TRUE;	
}

void CDAView::OnSetfocusEDITValue3() 
{
	m_bCustomSlider = TRUE;	
}

//////////////////////////////////////////////////////////////////////////
void CDAView::OnSetfocusEDITCycle() 
{
	m_bEditCycle = 	TRUE;
}

void CDAView::OnKillfocusEDITCycle() 
{
	m_bEditCycle = 	FALSE;
}

void CDAView::OnChangeEDITVolt0() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
// 	const MSG* pMsg = GetCurrentMessage();
// 	for (int index=0; index<4; index++)
// 	{
// 		if (pMsg->lParam == (LPARAM)m_pVolt(index)->m_hWnd)
// 		{
// 			AfxMessageBox("dd");
// 		}
// 	}	
}


