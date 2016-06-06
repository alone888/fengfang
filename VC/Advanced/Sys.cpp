// Sys.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CSysApp
BEGIN_MESSAGE_MAP(CSysApp, CWinApp)
//{{AFX_MSG_MAP(CSysApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
ON_UPDATE_COMMAND_UI(IDM_CloseFile, OnUpdateCloseFile)
ON_UPDATE_COMMAND_UI(IDM_DataFileNew, OnUpdateDataFileNew)
ON_COMMAND(IDM_CloseFile, OnCloseFile)
ON_COMMAND(IDM_DataFileNew, OnDataFileNew)
ON_COMMAND(IDM_StartDeviceAD, OnStartDeviceAD)
ON_COMMAND(IDM_StopDeviceAD, OnStopDeviceAD)
ON_UPDATE_COMMAND_UI(IDM_StopDeviceAD, OnUpdateStopDeviceAD)
ON_COMMAND(IDM_Open_AD, OnOpenAD)
ON_UPDATE_COMMAND_UI(IDM_Open_AD, OnUpdateOpenAD)
ON_COMMAND(IDM_OpenADHist, OnOpenADHist)
ON_UPDATE_COMMAND_UI(IDM_StartDeviceAD, OnUpdateStartDeviceAD)
ON_UPDATE_COMMAND_UI(IDM_MDataSave, OnUpdateDataSave)
ON_UPDATE_COMMAND_UI(IDM_MDigitalShow, OnUpdateDigitalShow)
ON_UPDATE_COMMAND_UI(IDM_MGraphicShow, OnUpdateGraphicShow)
ON_COMMAND(IDM_MDataSave, OnMDataSave)
ON_COMMAND(IDM_MDigitalShow, OnMDigitalShow)
ON_COMMAND(IDM_MGraphicShow, OnMGraphicShow)
ON_COMMAND(IDM_ListDeviceDlg, OnListDeviceDlg)
ON_COMMAND(IDM_OPEN_IO, OnOpenIo)
ON_UPDATE_COMMAND_UI(IDM_OPEN_IO, OnUpdateOpenIo)
ON_COMMAND(IDM_Open_DA, OnOpenDA)
ON_UPDATE_COMMAND_UI(IDM_Open_DA, OnUpdateOpenDA)
	ON_COMMAND(IDM_GetDevVersion, OnGetDevVersion)
	//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
// Standard print setup command
ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysApp construction
CSysApp::CSysApp()
{
 	m_bCreateDIOFrm = FALSE;
	m_bCreateCNTFrm = FALSE;
	m_bCreateDAFrm = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSysApp object

CSysApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CSysApp initialization

BOOL CSysApp::InitInstance()
{
	WCHAR szMutex[256];
	m_CurrentDeviceID = 0;   // ָ����ǰ�豸��ID��ʾ��
	
	int DeviceID = 0;
	while (TRUE)
	{
		swprintf_s(szMutex, _T("USB2831-%d"), DeviceID);
		
		// �����������
		m_hMutex=::CreateMutex(NULL, NULL, szMutex);  // m_pszExeNameΪ�������ִ����
		if (GetLastError() == ERROR_ALREADY_EXISTS)      // �ڶ��δ���Ӧ�ó���
		{
			DeviceID++;
			continue;  // ����Ѿ��������������һ���豸��Ӧ�ó��򴴽�
		}
		else
		{ 	
			m_CurrentDeviceID = DeviceID;
			m_hDevice = USB2831_CreateDevice(m_CurrentDeviceID);
			if (m_hDevice == INVALID_HANDLE_VALUE)
			{			
				//AfxMessageBox(_T("�Բ���û���豸�����������豸�ѱ���Ӧ������������ٴ�����ʵ��..."), MB_ICONWARNING, 0);
			}
			break;
		}
	}

	///////////////////////////////////////////////
	// �ж��û�����ʾ��ģʽ�Ƿ�Ϊ1024*768
	int Len = GetSystemMetrics(SM_CXSCREEN);  // ȡ����Ļ���
	if (Len<1024) // �����Ļ��ȴ�С1024����
	{
		if (AfxMessageBox(_T("�����ʹ��1024*768�����ϵ���ʾ���ֱ��ʣ�������"),MB_ICONWARNING|MB_YESNO,0)==IDNO)	
		{
			ExitInstance();
			return FALSE;
		}
	}
	
	// Ӧ�ó��������������룺
	AfxEnableControlContainer();
/*	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif*/
	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
	
	// ���AD�ɼ�ģ��
	pADTemplate = new CMultiDocTemplate(
		IDR_AD,
		RUNTIME_CLASS(CADDoc),
		RUNTIME_CLASS(CADFrm), // custom MDI child frame
		RUNTIME_CLASS(CADDigitView));
	AddDocTemplate(pADTemplate);
	
	// �����ʷ���ݻط�ģ��
	pHistDataTemplate = new CMultiDocTemplate(
		IDR_ADHist,
		RUNTIME_CLASS(CADHistDoc),
		RUNTIME_CLASS(CADHistFrm), // custom MDI child frame
		RUNTIME_CLASS(CADHistDigitView));
	AddDocTemplate(pHistDataTemplate);
	
	// DIOģ��
	pDIOTemplate= new CMultiDocTemplate(
		IDR_DIO,
		RUNTIME_CLASS(CDIODoc),
		RUNTIME_CLASS(CDIOFrm), // custom MDI child frame
		RUNTIME_CLASS(CDIOView));
	AddDocTemplate(pDIOTemplate);
		
	// DAģ��
	pDATemplate= new CMultiDocTemplate(
		IDR_DA,
		RUNTIME_CLASS(CDADoc),
		RUNTIME_CLASS(CDAFrm), // custom MDI child frame
		RUNTIME_CLASS(CDAView));
	AddDocTemplate(pDATemplate);
	
	
	// Enable DDE Execute open
	// ��WINDOWS�ǼǸó����ȱʡ�����ļ�����չ��
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	// The main window has been initialized, so show and update it.
	// pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED); // ʹ���������
	pMainFrame->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles(); // ֧���ϷŹ���
	::SetProp(m_pMainWnd->GetSafeHwnd(), szMutex, (HANDLE)1);	
	
	CString MainFrmName;
	WCHAR str[256];
	LONG DeviceLgcID, DevicePhysID;
	USB2831_GetDeviceCurrentID(m_hDevice, &DeviceLgcID, &DevicePhysID);
	swprintf_s(str, _T("USB2831-%d-%d "), m_CurrentDeviceID,DevicePhysID);
	//MainFrmName = pMainFrame->GetTitle();
	MainFrmName = "��������"+MainFrmName;
	pMainFrame->SetTitle(MainFrmName);
	OnOpenAD();		// ��AD�ɼ�ģ��
	m_ADFrm->BringWindowToTop();	// ��ADģ��������Ļ����
	return TRUE;
}

int CSysApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	ReleaseMutex(m_hMutex);
	if (m_hDevice != INVALID_HANDLE_VALUE)
		USB2831_ReleaseDevice(m_hDevice);

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSysApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSysApp message handlers
void CSysApp::OnUpdateOpenAD(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CSysApp::OnOpenAD(void)
{
	if (m_ADFrm != NULL)
	{
		m_ADFrm->BringWindowToTop();
		return;
	}
	// ȡ�ò���ϵͳ�汾
	DWORD	dwVersion = GetVersion();
	// Get the Windows version.
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	// Get the build number.

	BeginWaitCursor();  // ��ʼ©�����
	m_pADDoc = (CADDoc*)pADTemplate->CreateNewDocument();  // �������ĵ�
	// �������ĵ��������֡����
	m_ADFrm = (CADFrm*)pADTemplate->CreateNewFrame(m_pADDoc, NULL);
	
	// ��ʼ��FRAME��֡���ڣ��������ĵ�ʵ��, �Ҵ���FRAME��OnUpdate����
  	pADTemplate->InitialUpdateFrame(m_ADFrm, m_pADDoc);
	// ����Ĭ�ϵ��ĵ�����
	m_pADDoc->SetTitle(_T("�������"));
	if (gl_pADStatusView != NULL)
	{
		// ������������ͼ������ĵ��б��Ա��������ͼһ�����ĵ������
		m_pADDoc->AddView((CView*)gl_pADStatusView); 
	}
	if (gl_pParaCfgView != NULL)
	{// ������������ͼ������ĵ��б��Ա��������ͼһ�����ĵ������
		m_pADDoc->AddView((CView*)gl_pParaCfgView);
	}
	EndWaitCursor(); // ֹͣ©�����
}

// ����ʷ�ĵ�
void CSysApp::OnOpenADHist() 
{
	BeginWaitCursor();  // ��ʼ©�����
	CFile m_File;
	CString strFileName;
	// �����ļ��Ի���
	if (!(DoPromptFileName(strFileName, IDS_ADHist,	OFN_HIDEREADONLY, TRUE, NULL))) 
		return;	
	WCHAR* pFileName = strFileName.GetBuffer(sizeof(strFileName)); // ����ת��
	if (!m_File.Open(pFileName, USB2831_modeRead)) // ���ļ�
	{
		MsgWarning("����", "�ļ��򿪴�����޴��ļ�!", CPoint(1000, 200), 10000);
		return;
	}
	m_File.Close();	
	m_strFilePath = strFileName;
	m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(strFileName);
	EndWaitCursor(); // ֹͣ©�����
}

void CSysApp::OnOpenIo() 
{
	if (m_pDIOfrm == NULL)
	{
		BeginWaitCursor();
		m_pDIODoc = (CDIODoc*)pDIOTemplate->OpenDocumentFile(NULL);
		m_pDIODoc->SetTitle(_T("����������"));
		EndWaitCursor();
	}
	else
	{
		m_pDIOfrm->BringWindowToTop();	// �ָ��ص���
	}
}

void CSysApp::OnUpdateOpenIo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(this->m_bCreateDIOFrm==FALSE?1:0);
}



void CSysApp::OnOpenDA() 
{
	if (m_pDAFrm == NULL)
	{
		BeginWaitCursor();
		m_pDADoc = (CDADoc*)pDATemplate->OpenDocumentFile(NULL);
		m_pDADoc->SetTitle(_T("DA�������"));
		EndWaitCursor();
	}
	else
	{
		m_pDAFrm->BringWindowToTop();	// �ָ��ص���
	}
}

void CSysApp::OnUpdateOpenDA(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(this->m_bCreateDAFrm==FALSE?1:0);

}


// ������򿪵��ļ��д��ļ�
CDocument* CSysApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	
	BeginWaitCursor();  // ��ʼ©�����
	CFile File;
	int nRet = 0;
	BOOL bFileAvailable = TRUE;
	FILE_HEADER FileHeader;
	if (File.Open(lpszFileName, USB2831_modeRead)) // ����ļ��򿪳ɹ�
	{
		File.Seek(0, CFile::begin);
		File.Read((WORD*)&FileHeader, sizeof(FileHeader)); // ��ȡ�ļ�ͷ
		File.Close();
		if ((FileHeader.DeviceNum) != DEFAULT_DEVICE_NUM)
		{
			memset(&(FileHeader.ADPara), 0x00, sizeof(FileHeader.ADPara));
			if (AfxMessageBox(_T("���ļ�����USB2831�忨�����ļ�, �Ƿ����?"), MB_OKCANCEL) == IDCANCEL)
				return NULL;
			bFileAvailable = FALSE; // �ļ���Ч
		}
		m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(lpszFileName); // ΪNULLʱ������CHistDataFrm::OpenDocumentFile
		return m_pADHistDoc; // �����ĵ�ָ��
	}
	else
	{
		MsgWarning("����", "�ļ��򿪴�����޴��ļ�!", CPoint(1000, 200), 10000);
		return NULL;
	}

	EndWaitCursor(); // ֹͣ©�����
	return NULL;
}

void CSysApp::OnUpdateDataFileNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((!gl_bDeviceADRun)&&gl_bCreateFile);
}

void CSysApp::OnCloseFile() 
{
	m_pADDoc->OnCloseFile();
}

void CSysApp::OnDataFileNew() 
{
	m_pADDoc->OnDataFileNew();
}

void CSysApp::OnStartDeviceAD() 
{
	m_pADDoc->StartDeviceAD();
}

void CSysApp::OnUpdateStartDeviceAD(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gl_bDeviceADRun==FALSE?1:0);	
}

void CSysApp::OnStopDeviceAD() 
{
	m_pADDoc->StopDeviceAD();
}

void CSysApp::OnUpdateStopDeviceAD(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gl_bDeviceADRun == TRUE ? 1:0);		
}

void CSysApp::OnUpdateCloseFile(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (FALSE); 
	if ((gl_nProcMode == PROC_MODE_SAVE) && (gl_bCreateDevice))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!gl_bCloseFile);
}

void CSysApp::OnUpdateDataSave(CCmdUI* pCmdUI) 
{
	if (gl_pADStatusView != NULL)
		gl_pADStatusView->SetDlgCheck();

	if (gl_bCreateFile)
		pCmdUI->Enable(FALSE);
	
	pCmdUI->SetCheck(gl_nProcMode == PROC_MODE_SAVE ? 1:0);	
}

void CSysApp::OnUpdateDigitalShow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gl_nProcMode == PROC_MODE_DIGIT ? 1:0);
	gl_pADStatusView->SetDlgCheck();
	
    CADDoc* pDoc = theApp.m_pADDoc; 
    // ���߳���ȡ����ͼָ��
    CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	if (gl_nProcMode == PROC_MODE_SAVE) // ���Ϊ���̴���ʽ
	{
		pDigitView->EnableWindow(FALSE);
	}
	else
	{
		pDigitView->EnableWindow(TRUE);
	}		
}

void CSysApp::OnUpdateGraphicShow(CCmdUI* pCmdUI) 
{
	gl_pADStatusView->SetDlgCheck();
	pCmdUI->SetCheck(gl_nProcMode == PROC_MODE_WAVE ? 1:0);
    CADDoc* pDoc = theApp.m_pADDoc;
	
    // ���߳���ȡ����ͼָ��	
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	
	if (gl_nProcMode == PROC_MODE_SAVE) // ���Ϊ���̴���ʽ
	{
		pWaveView->EnableWindow(FALSE);
	}
	else
	{
		pWaveView->EnableWindow(TRUE);
	}	
	
}

void CSysApp::OnMDataSave() 
{
	gl_nProcMode = PROC_MODE_SAVE;
	gl_pADStatusView->SetDlgCheck();
}

void CSysApp::OnMDigitalShow() 
{
	gl_nProcMode = PROC_MODE_DIGIT;
	gl_pADStatusView->SetDlgCheck();
}

void CSysApp::OnMGraphicShow() 
{
	gl_nProcMode = PROC_MODE_WAVE;
	gl_pADStatusView->SetDlgCheck();
}

void CSysApp::OnListDeviceDlg() 
{
	USB2831_ListDeviceDlg(); // �б�����豸
}

void CSysApp::MsgWarning(CString strTitle, CString strMsg, CPoint point, UINT lTime)
{
	/*
	m_BalloonHelp.LaunchBalloon(strTitle,
		strMsg, 
		point, IDI_WARNING, 
		CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN|CBalloonHelp::unDISABLE_FADE, 
		NULL, 
		"", 
		lTime);
		*/
}

void CSysApp::OnGetDevVersion() 
{
	// TODO: Add your command handler code here
	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		CString str;
		CString strVer;
		ULONG ulFmwVersion, ulDriverVersion;
		if (USB2831_GetDevVersion(theApp.m_hDevice, &ulFmwVersion, &ulDriverVersion))
		{
			strVer.Format(_T("Sys V%x.%02x.%02x\n"), 
				(ulFmwVersion>>16) & 0xFF,
				(ulFmwVersion>>8) & 0xFF,
				ulFmwVersion & 0xFF);
			
			str.Format(_T("Sys V%x.%02x.%02x.%02x"), (ulDriverVersion>>24) & 0xFF,
				(ulDriverVersion>>16) & 0xFF,
				(ulDriverVersion>>8) & 0xFF,
				ulDriverVersion & 0xFF);
			
			
			AfxMessageBox(strVer + str);
		}
		
	}
}
