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
	m_CurrentDeviceID = 0;   // 指定当前设备的ID标示符
	
	int DeviceID = 0;
	while (TRUE)
	{
		swprintf_s(szMutex, _T("USB2831-%d"), DeviceID);
		
		// 创建互斥对象
		m_hMutex=::CreateMutex(NULL, NULL, szMutex);  // m_pszExeName为本程序的执行名
		if (GetLastError() == ERROR_ALREADY_EXISTS)      // 第二次创建应用程序
		{
			DeviceID++;
			continue;  // 如果已经创建，则继续下一个设备的应用程序创建
		}
		else
		{ 	
			m_CurrentDeviceID = DeviceID;
			m_hDevice = USB2831_CreateDevice(m_CurrentDeviceID);
			if (m_hDevice == INVALID_HANDLE_VALUE)
			{			
				//AfxMessageBox(_T("对不起，没有设备或您的所有设备已被相应程序管理，不能再创建新实例..."), MB_ICONWARNING, 0);
			}
			break;
		}
	}

	///////////////////////////////////////////////
	// 判断用户的显示器模式是否为1024*768
	int Len = GetSystemMetrics(SM_CXSCREEN);  // 取得屏幕宽度
	if (Len<1024) // 如果屏幕宽度大小1024，则
	{
		if (AfxMessageBox(_T("请最好使用1024*768或以上的显示器分辨率，继续吗？"),MB_ICONWARNING|MB_YESNO,0)==IDNO)	
		{
			ExitInstance();
			return FALSE;
		}
	}
	
	// 应用程序正常创建代码：
	AfxEnableControlContainer();
/*	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif*/
	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
	
	// 添加AD采集模板
	pADTemplate = new CMultiDocTemplate(
		IDR_AD,
		RUNTIME_CLASS(CADDoc),
		RUNTIME_CLASS(CADFrm), // custom MDI child frame
		RUNTIME_CLASS(CADDigitView));
	AddDocTemplate(pADTemplate);
	
	// 添加历史数据回放模板
	pHistDataTemplate = new CMultiDocTemplate(
		IDR_ADHist,
		RUNTIME_CLASS(CADHistDoc),
		RUNTIME_CLASS(CADHistFrm), // custom MDI child frame
		RUNTIME_CLASS(CADHistDigitView));
	AddDocTemplate(pHistDataTemplate);
	
	// DIO模板
	pDIOTemplate= new CMultiDocTemplate(
		IDR_DIO,
		RUNTIME_CLASS(CDIODoc),
		RUNTIME_CLASS(CDIOFrm), // custom MDI child frame
		RUNTIME_CLASS(CDIOView));
	AddDocTemplate(pDIOTemplate);
		
	// DA模板
	pDATemplate= new CMultiDocTemplate(
		IDR_DA,
		RUNTIME_CLASS(CDADoc),
		RUNTIME_CLASS(CDAFrm), // custom MDI child frame
		RUNTIME_CLASS(CDAView));
	AddDocTemplate(pDATemplate);
	
	
	// Enable DDE Execute open
	// 让WINDOWS登记该程序的缺省数据文件名扩展名
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
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED); // 使主窗口最大化
	pMainFrame->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles(); // 支持拖放功能
	::SetProp(m_pMainWnd->GetSafeHwnd(), szMutex, (HANDLE)1);	
	
	CString MainFrmName;
	WCHAR str[256];
	LONG DeviceLgcID, DevicePhysID;
	USB2831_GetDeviceCurrentID(m_hDevice, &DeviceLgcID, &DevicePhysID);
	swprintf_s(str, _T("USB2831-%d-%d "), m_CurrentDeviceID,DevicePhysID);
	//MainFrmName = pMainFrame->GetTitle();
	MainFrmName = "汉恒生物"+MainFrmName;
	pMainFrame->SetTitle(MainFrmName);
	OnOpenAD();		// 打开AD采集模板
	m_ADFrm->BringWindowToTop();	// 将AD模板置于屏幕顶端
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
	// 取得操作系统版本
	DWORD	dwVersion = GetVersion();
	// Get the Windows version.
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	// Get the build number.

	BeginWaitCursor();  // 开始漏斗鼠标
	m_pADDoc = (CADDoc*)pADTemplate->CreateNewDocument();  // 创建新文档
	// 创建该文档对象的子帧窗口
	m_ADFrm = (CADFrm*)pADTemplate->CreateNewFrame(m_pADDoc, NULL);
	
	// 初始化FRAME子帧窗口，并创建文档实例, 且触发FRAME的OnUpdate函数
  	pADTemplate->InitialUpdateFrame(m_ADFrm, m_pADDoc);
	// 设置默认的文档标题
	m_pADDoc->SetTitle(_T("生物电检测"));
	if (gl_pADStatusView != NULL)
	{
		// 将参数配置视图类加入文档列表，以便跟正常视图一样被文档类管理
		m_pADDoc->AddView((CView*)gl_pADStatusView); 
	}
	if (gl_pParaCfgView != NULL)
	{// 将参数配置视图类加入文档列表，以便跟正常视图一样被文档类管理
		m_pADDoc->AddView((CView*)gl_pParaCfgView);
	}
	EndWaitCursor(); // 停止漏斗鼠标
}

// 打开历史文档
void CSysApp::OnOpenADHist() 
{
	BeginWaitCursor();  // 开始漏斗鼠标
	CFile m_File;
	CString strFileName;
	// 弹出文件对话框
	if (!(DoPromptFileName(strFileName, IDS_ADHist,	OFN_HIDEREADONLY, TRUE, NULL))) 
		return;	
	WCHAR* pFileName = strFileName.GetBuffer(sizeof(strFileName)); // 类型转换
	if (!m_File.Open(pFileName, USB2831_modeRead)) // 打开文件
	{
		MsgWarning("警告", "文件打开错误或无此文件!", CPoint(1000, 200), 10000);
		return;
	}
	m_File.Close();	
	m_strFilePath = strFileName;
	m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(strFileName);
	EndWaitCursor(); // 停止漏斗鼠标
}

void CSysApp::OnOpenIo() 
{
	if (m_pDIOfrm == NULL)
	{
		BeginWaitCursor();
		m_pDIODoc = (CDIODoc*)pDIOTemplate->OpenDocumentFile(NULL);
		m_pDIODoc->SetTitle(_T("开关量测试"));
		EndWaitCursor();
	}
	else
	{
		m_pDIOfrm->BringWindowToTop();	// 恢复重叠窗
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
		m_pDADoc->SetTitle(_T("DA输出测试"));
		EndWaitCursor();
	}
	else
	{
		m_pDAFrm->BringWindowToTop();	// 恢复重叠窗
	}
}

void CSysApp::OnUpdateOpenDA(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(this->m_bCreateDAFrm==FALSE?1:0);

}


// 从最近打开的文件中打开文件
CDocument* CSysApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	
	BeginWaitCursor();  // 开始漏斗鼠标
	CFile File;
	int nRet = 0;
	BOOL bFileAvailable = TRUE;
	FILE_HEADER FileHeader;
	if (File.Open(lpszFileName, USB2831_modeRead)) // 如果文件打开成功
	{
		File.Seek(0, CFile::begin);
		File.Read((WORD*)&FileHeader, sizeof(FileHeader)); // 读取文件头
		File.Close();
		if ((FileHeader.DeviceNum) != DEFAULT_DEVICE_NUM)
		{
			memset(&(FileHeader.ADPara), 0x00, sizeof(FileHeader.ADPara));
			if (AfxMessageBox(_T("此文件并非USB2831板卡配套文件, 是否继续?"), MB_OKCANCEL) == IDCANCEL)
				return NULL;
			bFileAvailable = FALSE; // 文件无效
		}
		m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(lpszFileName); // 为NULL时不引发CHistDataFrm::OpenDocumentFile
		return m_pADHistDoc; // 返回文档指针
	}
	else
	{
		MsgWarning("警告", "文件打开错误或无此文件!", CPoint(1000, 200), 10000);
		return NULL;
	}

	EndWaitCursor(); // 停止漏斗鼠标
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
    // 在线程中取得视图指针
    CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	if (gl_nProcMode == PROC_MODE_SAVE) // 如果为存盘处理方式
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
	
    // 在线程中取得视图指针	
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	
	if (gl_nProcMode == PROC_MODE_SAVE) // 如果为存盘处理方式
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
	USB2831_ListDeviceDlg(); // 列表该类设备
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
