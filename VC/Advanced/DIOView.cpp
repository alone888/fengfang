// IOView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "DIOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int OldIn=-1;

const UINT IDC_CHECK_DI[] = 
{
	IDC_CHECK_DI0, IDC_CHECK_DI1,
	IDC_CHECK_DI2, IDC_CHECK_DI3,
	IDC_CHECK_DI4, IDC_CHECK_DI5,
	IDC_CHECK_DI6, IDC_CHECK_DI7,
	IDC_CHECK_DI8, IDC_CHECK_DI9,
	IDC_CHECK_DI10, IDC_CHECK_DI11,
	IDC_CHECK_DI12, IDC_CHECK_DI13,
	IDC_CHECK_DI14, IDC_CHECK_DI15
};

const UINT IDC_CHECK_DO[] = 
{
	IDC_CHECK_DO0, IDC_CHECK_DO1,
	IDC_CHECK_DO2, IDC_CHECK_DO3,
	IDC_CHECK_DO4, IDC_CHECK_DO5,
	IDC_CHECK_DO6, IDC_CHECK_DO7,
	IDC_CHECK_DO8, IDC_CHECK_DO9,
	IDC_CHECK_DO10, IDC_CHECK_DO11,
	IDC_CHECK_DO12, IDC_CHECK_DO13,
	IDC_CHECK_DO14, IDC_CHECK_DO15
};

#define GetButtonDI(nNum)				((CButton*)GetDlgItem(IDC_CHECK_DI[nNum]))
#define GetButtonDO(nNum)				((CButton*)GetDlgItem(IDC_CHECK_DO[nNum]))

/////////////////////////////////////////////////////////////////////////////
// CDIOView

IMPLEMENT_DYNCREATE(CDIOView, CFormView)

CDIOView::CDIOView()
	: CFormView(CDIOView::IDD)
{
	//{{AFX_DATA_INIT(CDIOView)
	//}}AFX_DATA_INIT
	m_bCreateDevice = FALSE;
}

CDIOView::~CDIOView()
{
}

void CDIOView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDIOView)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDIOView, CFormView)
	//{{AFX_MSG_MAP(CDIOView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_DO0, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO1, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO2, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO3, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO4, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO5, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO6, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO7, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO8, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO9, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO10, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO11, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO12, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO13, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO14, OnCheckDo)
	ON_BN_CLICKED(IDC_CHECK_DO15, OnCheckDo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIOView diagnostics

#ifdef _DEBUG
void CDIOView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDIOView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIOView message handlers

void CDIOView::SetSwitchStatus(void)
{

	if(!m_bCreateDevice) return;
	// 读入各路开关量状态
	if(!USB2831_GetDeviceDI(hDevice, m_byDI))
	{
		KillTimer(1);
		CRect rect;
		GetButtonDI(0)->GetWindowRect(&rect);
		AfxMessageBox(_T("读开关量数据失败..."));
		
		return;
	} 

	CString str;

	for (int nDI=0; nDI<16; nDI++)
	{
		if (m_byDI[nDI] == 1)
		{
			str.Format(_T("开关%d (开)"), nDI);
			GetButtonDI(nDI)->SetState(TRUE);
			GetButtonDI(nDI)->SetWindowText(str);
			GetButtonDI(nDI)->SetCheck(1);
		}
		else
		{
			str.Format(_T("开关%d(关)"), nDI);
			GetButtonDI(nDI)->SetState(FALSE);
			GetButtonDI(nDI)->SetWindowText(str);
			GetButtonDI(nDI)->SetCheck(0);
		}
	}
}

void CDIOView::SetSwitchOut(void)
{

	if(!m_bCreateDevice) return;

	for (int nDO=0; nDO<16; nDO++)
	{
		m_byDO[nDO] = GetButtonDO(nDO)->GetCheck();
	}

	// 输出各路开关量状态
	USB2831_SetDeviceDO(hDevice, m_byDO);

}

void CDIOView::OnCheckDo() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	CButton* pTempButton = NULL;
	CString str;
	for(int nDO=0; nDO<16; nDO++)
	{
		pTempButton = (CButton*)GetDlgItem(IDC_CHECK_DO[nDO]);
		int nDOSts = pTempButton->GetCheck();
		if(pMsg->lParam == (LPARAM)pTempButton->m_hWnd)
		{
			UpdateData(TRUE);
			if (nDOSts == 1)
			{
				str.Format(_T("开关%d (开)"), nDO);
				pTempButton->SetWindowText(str);
			}
			else
			{
				str.Format(_T("开关%d (关)"), nDO);
				pTempButton->SetWindowText(str);
			}

			SetSwitchOut();
		}
	} 
}

void CDIOView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
		// TODO: Add your specialized code here and/or call the base class
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	DeviceID = pApp->m_CurrentDeviceID;  // 取得当前应用程序使用的设备ID号

	hDevice = pApp->m_hDevice;
	if(hDevice==INVALID_HANDLE_VALUE)
	{
		CRect rect;
		GetButtonDO(0)->GetWindowRect(&rect);
		AfxMessageBox(_T("创建设备失败..."));	
		m_bCreateDevice = FALSE;
		return;
	}
	m_bCreateDevice = TRUE;

 	SetTimer(1, 100, NULL); // 启动定时器，查询开关量输入状态
}



void CDIOView::OnDestroy() 
{
	CFormView::OnDestroy();
}

void CDIOView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		SetSwitchStatus();
		break;
// 	default:
// 		ASSERT(FALSE);
	}

	CFormView::OnTimer(nIDEvent);
}

void CDIOView::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	CFormView::OnClose();
}
