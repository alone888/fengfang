// ADDoc.cpp : implementation of the CADDoc class
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CADDoc

IMPLEMENT_DYNCREATE(CADDoc, CDocument)

BEGIN_MESSAGE_MAP(CADDoc, CDocument)
//{{AFX_MSG_MAP(CADDoc)
ON_COMMAND(IDM_TileWave, OnTileWave)
ON_UPDATE_COMMAND_UI(IDM_TileWave, OnUpdateTileWave)
//ON_COMMAND(IDM_SuperPoseWave, OnSuperPoseWave)
//ON_UPDATE_COMMAND_UI(IDM_SuperPoseWave, OnUpdateSuperPoseWave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADDoc construction/destruction

CADDoc::CADDoc()
{
	
	// TODO: add one-time construction code here
	gl_bProgress = FALSE; // 一开始不更新进度条
	gl_nProcMode = PROC_MODE_DIGIT; // 默认方式：数字显示
	gl_TriggerVolt = (int)AD_VOLT_RANGE; // 置触发电平的初值为其最大量程
	gl_bCreateFile = TRUE;  // 一开始，为第一次创建文件对象
	gl_bCloseFile = TRUE; // 没有打开文件时，认为文件被关闭
	
}

CADDoc::~CADDoc()
{

}

BOOL CADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CADDoc serialization

void CADDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CADDoc diagnostics

#ifdef _DEBUG
void CADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

static int device_start_ok = 0;
void CADDoc::StartDeviceAD() 
{
	if (device_start_ok)
	{
		return;
	}	
	device_start_ok = 0;	
	m_hDevice = theApp.m_hDevice;
	////////////////////////////////////////////////////////////////////////
	if (m_hDevice == INVALID_HANDLE_VALUE)
	{
		if (m_hDevice == INVALID_HANDLE_VALUE)
		{
			CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // 取得子帧窗口指针
			//CButton* pStart = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StartDeviceAD));
			//CRect rect;
			//pStart->GetWindowRect(&rect);
			AfxMessageBox(_T("尚未连接采集器或采集器连接出错!"));
			return;
		}
	}

	SYSTEMTIME  time;
	int nRandNum;

	//gl_nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1;
	ADPara.LastChannel = 7;
	ADPara.FirstChannel = 0;
	if (!gl_bCreateFile) // 如果创建了文件对象，则先写头文件
	{
		::GetSystemTime(&time); 
		srand(time.wMilliseconds); // 种子数
		nRandNum =  rand();        // 随机数
		m_header.HeadSizeBytes = sizeof(FILE_HEADER);// 头信息大小
		m_header.FileType= DEFAULT_FILE_TYPE;     // 文件类型
		m_header.BusType = DEFAULT_BUSTYPE;       // 总线类型
		m_header.DeviceNum = DEFAULT_DEVICE_NUM;	// 设备ID号
		m_header.HeadVersion = 0x0600;            // 头版本号: 6.0
		m_header.ADPara = ADPara;                 // 硬件参数		
		m_header.BatCode = nRandNum<<2;           // 批文件识别码
		m_header.HeadEndFlag = HEAD_END_FLAG;     // 文件头结束符
		
		m_header.nTriggerPos = gl_nTriggerPos;
		switch(gl_InputRange[0].nRangeIndex)    // 电压量程
		{		
		case USB2831_INPUT_N10000_P10000mV: // ±10000mV 
			m_header.VoltTopRange = 10000; // ??* 上下交换
			m_header.VoltBottomRange = -10000;
			break;
		case USB2831_INPUT_N5000_P5000mV: // ±5000mV
			m_header.VoltTopRange = 5000;
			m_header.VoltBottomRange = -5000;
			break;
		case USB2831_INPUT_N2500_P2500mV: // ±2500mV
			m_header.VoltTopRange = 2500;
			m_header.VoltBottomRange = -2500;
			break;
		case USB2831_INPUT_0_P10000mV: // 
			m_header.VoltTopRange = 10000;
			m_header.VoltBottomRange = 0;
			break;
		default:
			ASSERT(FALSE);
		}		

		// 写文件头
		m_File.Write((WORD*)&m_header, 	m_header.HeadSizeBytes);
	}
	
	gl_FileLenghtWords = m_header.HeadSizeBytes; // 文件长度初始化
	
	CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // 取得子帧窗口指针
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	
	memset(ADBuffer, 0x00, sizeof(ADBuffer)); // 将用户缓冲区复位清零
	
	if (!MyStartDeviceAD(m_hDevice)) // 启动设备, IDM_DataFileNew数据
	{
		//CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // 取得子帧窗口指针
		//CButton* pStart = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StartDeviceAD));
		//CRect rect;
		//pStart->GetWindowRect(&rect);
		//theApp.MsgWarning("警告", "设备启动失败!", rect.CenterPoint(), 10000);
		AfxMessageBox(_T("设备启动失败!"));
		return;
	}

	// 创建绘制窗口线程
	m_hProcThread = AfxBeginThread(ProcessDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_hProcThread->m_bAutoDelete = TRUE; // 自动删除    
	gl_bDeviceADRun = TRUE; // 设置设备启动标志

	// 创建数据采集与窗口绘制同步事件对象
	gl_hEvent = USB2831_CreateSystemEvent();
	gl_hExitEvent = USB2831_CreateSystemEvent();

	gl_pParaCfgView->EnableControlWindows(FALSE);  // 在设备开始运行时，禁止操作硬件参数
	gl_pADStatusView->EnableControlWindows(FALSE); 

	CADWaveView* pWaveView = (CADWaveView*)CView::FromHandle(m_hWndWave);
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();

	m_hProcThread->ResumeThread(); // 启动绘制窗口线程
	gl_bDataProcessing = FALSE;

	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(m_hWndDigit));
	pDigitView->TransitionData();

	// 开启定时器取得触发点位置
	m_hADParaCfgView->SetTimer(1, 1000, NULL);
	device_start_ok = 1;
}

//###################  处理数据线程函数 #########################
UINT ProcessDataThread(PVOID pThreadPara)
{
	CADFrm* pADFrm = theApp.m_ADFrm;
	CADDoc* pADDoc = theApp.m_pADDoc;
	ULONG WroteMB = 0;  
	pADDoc->m_WroteMB = 0;
	int ShowCount = 0;
	gl_nDrawIndex = 0;
	gl_nCurrentIndex = 0; // 复位当前绘图索引
	while (gl_bDeviceADRun) // 循环处理AD数据
	{ 
		do{
			if (!gl_bDeviceADRun) goto ExitProcDataThread;
		}while (WaitForSingleObject(gl_hEvent, 100) != WAIT_OBJECT_0);
		switch(gl_nProcMode)  // 判断数据处理方式
		{
		case PROC_MODE_DIGIT: // 数字处理
			if (gl_bDataProcessing == FALSE)
			{
				gl_nDrawIndex = gl_nCurrentIndex; // 如果窗口已完成数据刷新，则置新的缓冲区索引号，使之绘制新缓冲区
				::SendMessage(pADDoc->m_hWndDigit, WM_SHOW_DIGIT, NULL, NULL);
			}
			gl_bProgress = TRUE; // 使OnDraw函数能更新进度条		
			break;
		case PROC_MODE_WAVE:  // 波形处理
			//if (gl_bDataProcessing == FALSE)
			{
				gl_nDrawIndex = gl_nCurrentIndex; // 如果窗口已完成数据刷新，则置新的缓冲区索引号，使之绘制新缓冲区
				//::SendMessage(pADDoc->m_hWndDigit, WM_SHOW_DIGIT, NULL, NULL);
				::SendMessage(pADDoc->m_hWndWave, WM_SHOW_WAVE, NULL, NULL);
			}
			gl_bProgress = TRUE; // 使OnDraw函数能更新进度条	
			break;
		case PROC_MODE_SAVE: // 数据存盘处理	
			if (!gl_bDeviceADRun) goto ExitProcDataThread;
			pADDoc->m_File.Write(ADBuffer[gl_nCurrentIndex], gl_ReadSizeWords*2);
			CString strFileLenght;
			WroteMB = (WroteMB+16)%1024;
			gl_FileLenghtWords += gl_ReadSizeWords*2/1024; // 256Kb
			pADFrm->GetProgressCtrl()->SetPos((int)WroteMB);
			strFileLenght.Format(_T("%d"), gl_FileLenghtWords);
			int iDotPos = 0;
			iDotPos = strFileLenght.GetLength()-3;
			while (iDotPos>0)
			{
				strFileLenght.Insert(iDotPos, ',');
				iDotPos -= 3;
			}
			strFileLenght += " Kb";
			pADFrm->GetFileLenghtStatic()->SetWindowText(strFileLenght);
			ShowCount++;

			//存盘的时候同时显示曲线
			gl_nDrawIndex = gl_nCurrentIndex; // 如果窗口已完成数据刷新，则置新的缓冲区索引号，使之绘制新缓冲区
			//::SendMessage(pADDoc->m_hWndDigit, WM_SHOW_DIGIT, NULL, NULL);
			::SendMessage(pADDoc->m_hWndWave, WM_SHOW_WAVE, NULL, NULL);

			break;
// 		default:
// 			ASSERT(FALSE);
		} // end switch  
		gl_nCurrentIndex++;
		if (gl_nCurrentIndex > MAX_SEGMENT_COUNT-1) gl_nCurrentIndex = 0;
	} // end while
ExitProcDataThread:
	return TRUE;
}
//##########################################################################################

void CADDoc::Public_ReleaseDevice(void) // 供MDI窗口调用
{
	if (gl_bDeviceADRun)
	{
		StopDeviceAD();
	}
}

void CADDoc::StopDeviceAD() 
{	


	//DockControlBar(&m_newToolBar); //这行是为了在工具栏被拖役出边框时，工具栏窗口不能被隐藏。需要重靠。

	CSysApp* pApp = (CSysApp *)AfxGetApp();
	gl_bDeviceADRun = FALSE; // 停止子线程进行连续数据采集	
// 	Sleep(20);

	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // 取得子帧窗口句柄

	if(device_start_ok != 1) return;

	device_start_ok = 0;
	// 停止定时器取得触发点位置
	m_hADParaCfgView->KillTimer(1);

	if (!MyStopDeviceAD(m_hDevice)) // 关闭AD设备
	{
		//CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // 取得子帧窗口指针
		//CButton* pStop = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StopDeviceAD));
		//CRect rect;
		/*pStop->GetWindowRect(&rect);*/
		//theApp.MsgWarning("警告", "关闭AD设备失败!", rect.CenterPoint(), 10000);
		AfxMessageBox(_T("关闭AD设备失败!"));
		return;
	}
	// 在设备对象释放后，允许操作硬件参数
	gl_pParaCfgView->EnableControlWindows(TRUE);
	gl_pADStatusView->EnableControlWindows(TRUE);
	gl_bCreateDevice = FALSE;  
	USB2831_ReleaseSystemEvent(gl_hEvent); // 释放消息
	// 终止采集线程
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	


	gl_bCreateDevice = FALSE;     // 释放设备后，视设备未创建
	if (gl_nProcMode == PROC_MODE_SAVE)  // 如果数据处理方式为存盘
	{
		m_File.Seek(0,CFile::begin); // 返回到文件起始位置
		m_File.Write((WORD*)&m_header, m_header.HeadSizeBytes);
		OnCloseFile();              // 释放文件对象
		gl_nProcMode = PROC_MODE_WAVE; // 置成数字波形方式			
		CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(FALSE);  // 使存盘方式单选框有效
	}

	//CADFrm* pADFrm1 = (CADFrm*)(theApp.m_ADFrm); // 取得子帧窗口指针
	//CButton* pStop1 = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StopDeviceAD));
	//CRect rect;
	//pStop1->GetWindowRect(&rect);
	//theApp.MsgWarning("警告", "采集设备异常断开!", rect.CenterPoint(), 10000);
	//AfxMessageBox(_T("采集设备异常断开!"));
	memset(showData,0,sizeof(showData));
	gl_last_end_id = 0;
}
//新建文件 录音按钮
void CADDoc::OnDataFileNew() 
{
	// TODO: Add your command handler code here
	CString strNewFileName;
	CString TempFileName;
	HANDLE hDevice = m_hDevice;

	if (!(theApp.DoPromptFileName(strNewFileName, IDS_NEW_FILE, 
		OFN_HIDEREADONLY , TRUE, NULL)))
		return;

	if (!gl_bCreateFile)
	{  // 如果不是第一次创建文件对象, 则释放老文件对象
		m_File.Close();
	} 
	
	// 创建新文件对象
	int FileNameLength = strNewFileName.GetLength();
	TempFileName = strNewFileName.Left(FileNameLength-4);

	strNewFileName = TempFileName + ".nd";
	BOOL bRet = m_File.Open(strNewFileName,USB2831_modeCreate|USB2831_modeWrite);

    if (bRet == FALSE)		
	{
		AfxMessageBox(_T("释放文件对象失败..."));
		return;
	}	
	
	// 保存打开的文件名称
	g_strFileFullName = strNewFileName;

	CADFrm* pADFrm = theApp.m_ADFrm; // 取得子帧窗口句柄
	pADFrm->ShowHideDlgSave ();	// 显示存盘栏
	pADFrm->ShowHideStatus(FALSE); // 隐藏状态栏
	
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // 使存盘方式单选框有效
	
	ULONGLONG DiskFreeBytes;  // 取得文件所在磁盘可用空间

	ULARGE_INTEGER FreeBytesAvailableToCaller;
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;
	#ifdef _UNICODE
		if(!GetDiskFreeSpaceExW(strNewFileName.Left(3), 
			&FreeBytesAvailableToCaller,
			&TotalNumberOfBytes,
			&TotalNumberOfFreeBytes))
		{
			return ;
		}
	#else
		if(!GetDiskFreeSpaceExA(strNewFileName.Left(3), 
			&FreeBytesAvailableToCaller,
			&TotalNumberOfBytes,
			&TotalNumberOfFreeBytes))
		{
			return ;
		}
	#endif
	DiskFreeBytes = TotalNumberOfFreeBytes.QuadPart;

	// 取得文件所在磁盘可用空间
	m_RemainMB = (ULONG)(DiskFreeBytes/(1024*1024));  // 求出有多少M字	
	CString str;
	
	if (m_RemainMB<2) 
	{
		theApp.MsgWarning("警告", "硬盘剩余空间不足2M，存盘操作不能进行...", CPoint(1000, 200), 10000);
		return;
	}

	str.Format(_T("%d Mb"), m_RemainMB);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str); // 置当前硬盘可用空间MB
	
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024); // 设置进程范围
	pADFrm->GetProgressCtrl()->SetPos(0);
	
	/////////////
	pADFrm->GetFileNameStatic()->SetWindowText(strNewFileName); // 显示存盘文件名
	gl_bCreateFile = FALSE;  // 若成功文件对象，则置标志	
	
	gl_bCloseFile = FALSE; // 有文件没有关闭, 可以操作《关闭当前文件》菜单
	gl_pADStatusView->SetDlgCheck();
	
	gl_nProcMode = PROC_MODE_SAVE; // 当打开新文件时，意味着要进行存盘处理
	gl_pADStatusView->SetDlgCheck();

	//打开文件之后直接开始采集
	StartDeviceAD();
}

void CADDoc::OnCloseFile() 
{
	// TODO: Add your command handler code here
	m_File.Close();

	// 添加到 曾打开的文件列表中
	this->SetPathName(g_strFileFullName);

	SetTitle(_T("AD数采测试"));

	gl_bCloseFile = TRUE;  // 文件被关闭
	gl_nProcMode = PROC_MODE_DIGIT; // 当关闭新文件时，意味着要不能再进行存盘处理，因此置成数字方式
	gl_bCreateFile = TRUE; // 当新文件被关闭后，意味着以后新文件还是第一次被创建 
	///////
	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // 取得子帧窗口句柄
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // 使存盘方式单选框有效
	
	CString str;
	str.Format(_T("%d"), 0);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str);
	pADFrm->ShowHideDlgSave (FALSE); // 隐藏存盘栏
	pADFrm->ShowHideStatus(); // 显示状态栏
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024);  // 设置进程范围
	pADFrm->GetProgressCtrl()->SetPos(0);	
	// 一兆进度
	gl_pADStatusView->SetDlgCheck();
	
}

void CADDoc::OnTileWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = TRUE;
	CButton* pButtonTile = (CButton*)gl_pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(1);
	CButton* pButtonPose = (CButton*)gl_pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(0);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	
}

void CADDoc::OnSuperPoseWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = FALSE;
	CButton* pButtonTile = (CButton*)gl_pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(0);
	CButton* pButtonPose = (CButton*)gl_pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(1);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	pWaveView->OnDrawPolyLine();
	
}

void CADDoc::OnUpdateSuperPoseWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave == TRUE ? 0:1);
}

void CADDoc::OnUpdateTileWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave == TRUE ? 1:0);
	
}

