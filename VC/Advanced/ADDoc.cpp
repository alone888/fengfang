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
	gl_bProgress = FALSE; // һ��ʼ�����½�����
	gl_nProcMode = PROC_MODE_DIGIT; // Ĭ�Ϸ�ʽ��������ʾ
	gl_TriggerVolt = (int)AD_VOLT_RANGE; // �ô�����ƽ�ĳ�ֵΪ���������
	gl_bCreateFile = TRUE;  // һ��ʼ��Ϊ��һ�δ����ļ�����
	gl_bCloseFile = TRUE; // û�д��ļ�ʱ����Ϊ�ļ����ر�
	
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
			CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // ȡ����֡����ָ��
			//CButton* pStart = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StartDeviceAD));
			//CRect rect;
			//pStart->GetWindowRect(&rect);
			AfxMessageBox(_T("��δ���Ӳɼ�����ɼ������ӳ���!"));
			return;
		}
	}

	SYSTEMTIME  time;
	int nRandNum;

	//gl_nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1;
	ADPara.LastChannel = 7;
	ADPara.FirstChannel = 0;
	if (!gl_bCreateFile) // ����������ļ���������дͷ�ļ�
	{
		::GetSystemTime(&time); 
		srand(time.wMilliseconds); // ������
		nRandNum =  rand();        // �����
		m_header.HeadSizeBytes = sizeof(FILE_HEADER);// ͷ��Ϣ��С
		m_header.FileType= DEFAULT_FILE_TYPE;     // �ļ�����
		m_header.BusType = DEFAULT_BUSTYPE;       // ��������
		m_header.DeviceNum = DEFAULT_DEVICE_NUM;	// �豸ID��
		m_header.HeadVersion = 0x0600;            // ͷ�汾��: 6.0
		m_header.ADPara = ADPara;                 // Ӳ������		
		m_header.BatCode = nRandNum<<2;           // ���ļ�ʶ����
		m_header.HeadEndFlag = HEAD_END_FLAG;     // �ļ�ͷ������
		
		m_header.nTriggerPos = gl_nTriggerPos;
		switch(gl_InputRange[0].nRangeIndex)    // ��ѹ����
		{		
		case USB2831_INPUT_N10000_P10000mV: // ��10000mV 
			m_header.VoltTopRange = 10000; // ??* ���½���
			m_header.VoltBottomRange = -10000;
			break;
		case USB2831_INPUT_N5000_P5000mV: // ��5000mV
			m_header.VoltTopRange = 5000;
			m_header.VoltBottomRange = -5000;
			break;
		case USB2831_INPUT_N2500_P2500mV: // ��2500mV
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

		// д�ļ�ͷ
		m_File.Write((WORD*)&m_header, 	m_header.HeadSizeBytes);
	}
	
	gl_FileLenghtWords = m_header.HeadSizeBytes; // �ļ����ȳ�ʼ��
	
	CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // ȡ����֡����ָ��
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	
	memset(ADBuffer, 0x00, sizeof(ADBuffer)); // ���û���������λ����
	
	if (!MyStartDeviceAD(m_hDevice)) // �����豸, IDM_DataFileNew����
	{
		//CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // ȡ����֡����ָ��
		//CButton* pStart = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StartDeviceAD));
		//CRect rect;
		//pStart->GetWindowRect(&rect);
		//theApp.MsgWarning("����", "�豸����ʧ��!", rect.CenterPoint(), 10000);
		AfxMessageBox(_T("�豸����ʧ��!"));
		return;
	}

	// �������ƴ����߳�
	m_hProcThread = AfxBeginThread(ProcessDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_hProcThread->m_bAutoDelete = TRUE; // �Զ�ɾ��    
	gl_bDeviceADRun = TRUE; // �����豸������־

	// �������ݲɼ��봰�ڻ���ͬ���¼�����
	gl_hEvent = USB2831_CreateSystemEvent();
	gl_hExitEvent = USB2831_CreateSystemEvent();

	gl_pParaCfgView->EnableControlWindows(FALSE);  // ���豸��ʼ����ʱ����ֹ����Ӳ������
	gl_pADStatusView->EnableControlWindows(FALSE); 

	CADWaveView* pWaveView = (CADWaveView*)CView::FromHandle(m_hWndWave);
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();

	m_hProcThread->ResumeThread(); // �������ƴ����߳�
	gl_bDataProcessing = FALSE;

	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(m_hWndDigit));
	pDigitView->TransitionData();

	// ������ʱ��ȡ�ô�����λ��
	m_hADParaCfgView->SetTimer(1, 1000, NULL);
	device_start_ok = 1;
}

//###################  ���������̺߳��� #########################
UINT ProcessDataThread(PVOID pThreadPara)
{
	CADFrm* pADFrm = theApp.m_ADFrm;
	CADDoc* pADDoc = theApp.m_pADDoc;
	ULONG WroteMB = 0;  
	pADDoc->m_WroteMB = 0;
	int ShowCount = 0;
	gl_nDrawIndex = 0;
	gl_nCurrentIndex = 0; // ��λ��ǰ��ͼ����
	while (gl_bDeviceADRun) // ѭ������AD����
	{ 
		do{
			if (!gl_bDeviceADRun) goto ExitProcDataThread;
		}while (WaitForSingleObject(gl_hEvent, 100) != WAIT_OBJECT_0);
		switch(gl_nProcMode)  // �ж����ݴ���ʽ
		{
		case PROC_MODE_DIGIT: // ���ִ���
			if (gl_bDataProcessing == FALSE)
			{
				gl_nDrawIndex = gl_nCurrentIndex; // ����������������ˢ�£������µĻ����������ţ�ʹ֮�����»�����
				::SendMessage(pADDoc->m_hWndDigit, WM_SHOW_DIGIT, NULL, NULL);
			}
			gl_bProgress = TRUE; // ʹOnDraw�����ܸ��½�����		
			break;
		case PROC_MODE_WAVE:  // ���δ���
			//if (gl_bDataProcessing == FALSE)
			{
				gl_nDrawIndex = gl_nCurrentIndex; // ����������������ˢ�£������µĻ����������ţ�ʹ֮�����»�����
				//::SendMessage(pADDoc->m_hWndDigit, WM_SHOW_DIGIT, NULL, NULL);
				::SendMessage(pADDoc->m_hWndWave, WM_SHOW_WAVE, NULL, NULL);
			}
			gl_bProgress = TRUE; // ʹOnDraw�����ܸ��½�����	
			break;
		case PROC_MODE_SAVE: // ���ݴ��̴���	
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

			//���̵�ʱ��ͬʱ��ʾ����
			gl_nDrawIndex = gl_nCurrentIndex; // ����������������ˢ�£������µĻ����������ţ�ʹ֮�����»�����
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

void CADDoc::Public_ReleaseDevice(void) // ��MDI���ڵ���
{
	if (gl_bDeviceADRun)
	{
		StopDeviceAD();
	}
}

void CADDoc::StopDeviceAD() 
{	


	//DockControlBar(&m_newToolBar); //������Ϊ���ڹ����������۳��߿�ʱ�����������ڲ��ܱ����ء���Ҫ�ؿ���

	CSysApp* pApp = (CSysApp *)AfxGetApp();
	gl_bDeviceADRun = FALSE; // ֹͣ���߳̽����������ݲɼ�	
// 	Sleep(20);

	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // ȡ����֡���ھ��

	if(device_start_ok != 1) return;

	device_start_ok = 0;
	// ֹͣ��ʱ��ȡ�ô�����λ��
	m_hADParaCfgView->KillTimer(1);

	if (!MyStopDeviceAD(m_hDevice)) // �ر�AD�豸
	{
		//CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // ȡ����֡����ָ��
		//CButton* pStop = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StopDeviceAD));
		//CRect rect;
		/*pStop->GetWindowRect(&rect);*/
		//theApp.MsgWarning("����", "�ر�AD�豸ʧ��!", rect.CenterPoint(), 10000);
		AfxMessageBox(_T("�ر�AD�豸ʧ��!"));
		return;
	}
	// ���豸�����ͷź��������Ӳ������
	gl_pParaCfgView->EnableControlWindows(TRUE);
	gl_pADStatusView->EnableControlWindows(TRUE);
	gl_bCreateDevice = FALSE;  
	USB2831_ReleaseSystemEvent(gl_hEvent); // �ͷ���Ϣ
	// ��ֹ�ɼ��߳�
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	


	gl_bCreateDevice = FALSE;     // �ͷ��豸�����豸δ����
	if (gl_nProcMode == PROC_MODE_SAVE)  // ������ݴ���ʽΪ����
	{
		m_File.Seek(0,CFile::begin); // ���ص��ļ���ʼλ��
		m_File.Write((WORD*)&m_header, m_header.HeadSizeBytes);
		OnCloseFile();              // �ͷ��ļ�����
		gl_nProcMode = PROC_MODE_WAVE; // �ó����ֲ��η�ʽ			
		CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(FALSE);  // ʹ���̷�ʽ��ѡ����Ч
	}

	//CADFrm* pADFrm1 = (CADFrm*)(theApp.m_ADFrm); // ȡ����֡����ָ��
	//CButton* pStop1 = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StopDeviceAD));
	//CRect rect;
	//pStop1->GetWindowRect(&rect);
	//theApp.MsgWarning("����", "�ɼ��豸�쳣�Ͽ�!", rect.CenterPoint(), 10000);
	//AfxMessageBox(_T("�ɼ��豸�쳣�Ͽ�!"));
	memset(showData,0,sizeof(showData));
	gl_last_end_id = 0;
}
//�½��ļ� ¼����ť
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
	{  // ������ǵ�һ�δ����ļ�����, ���ͷ����ļ�����
		m_File.Close();
	} 
	
	// �������ļ�����
	int FileNameLength = strNewFileName.GetLength();
	TempFileName = strNewFileName.Left(FileNameLength-4);

	strNewFileName = TempFileName + ".nd";
	BOOL bRet = m_File.Open(strNewFileName,USB2831_modeCreate|USB2831_modeWrite);

    if (bRet == FALSE)		
	{
		AfxMessageBox(_T("�ͷ��ļ�����ʧ��..."));
		return;
	}	
	
	// ����򿪵��ļ�����
	g_strFileFullName = strNewFileName;

	CADFrm* pADFrm = theApp.m_ADFrm; // ȡ����֡���ھ��
	pADFrm->ShowHideDlgSave ();	// ��ʾ������
	pADFrm->ShowHideStatus(FALSE); // ����״̬��
	
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // ʹ���̷�ʽ��ѡ����Ч
	
	ULONGLONG DiskFreeBytes;  // ȡ���ļ����ڴ��̿��ÿռ�

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

	// ȡ���ļ����ڴ��̿��ÿռ�
	m_RemainMB = (ULONG)(DiskFreeBytes/(1024*1024));  // ����ж���M��	
	CString str;
	
	if (m_RemainMB<2) 
	{
		theApp.MsgWarning("����", "Ӳ��ʣ��ռ䲻��2M�����̲������ܽ���...", CPoint(1000, 200), 10000);
		return;
	}

	str.Format(_T("%d Mb"), m_RemainMB);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str); // �õ�ǰӲ�̿��ÿռ�MB
	
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024); // ���ý��̷�Χ
	pADFrm->GetProgressCtrl()->SetPos(0);
	
	/////////////
	pADFrm->GetFileNameStatic()->SetWindowText(strNewFileName); // ��ʾ�����ļ���
	gl_bCreateFile = FALSE;  // ���ɹ��ļ��������ñ�־	
	
	gl_bCloseFile = FALSE; // ���ļ�û�йر�, ���Բ������رյ�ǰ�ļ����˵�
	gl_pADStatusView->SetDlgCheck();
	
	gl_nProcMode = PROC_MODE_SAVE; // �������ļ�ʱ����ζ��Ҫ���д��̴���
	gl_pADStatusView->SetDlgCheck();

	//���ļ�֮��ֱ�ӿ�ʼ�ɼ�
	StartDeviceAD();
}

void CADDoc::OnCloseFile() 
{
	// TODO: Add your command handler code here
	m_File.Close();

	// ��ӵ� ���򿪵��ļ��б���
	this->SetPathName(g_strFileFullName);

	SetTitle(_T("AD���ɲ���"));

	gl_bCloseFile = TRUE;  // �ļ����ر�
	gl_nProcMode = PROC_MODE_DIGIT; // ���ر����ļ�ʱ����ζ��Ҫ�����ٽ��д��̴�������ó����ַ�ʽ
	gl_bCreateFile = TRUE; // �����ļ����رպ���ζ���Ժ����ļ����ǵ�һ�α����� 
	///////
	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // ȡ����֡���ھ��
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // ʹ���̷�ʽ��ѡ����Ч
	
	CString str;
	str.Format(_T("%d"), 0);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str);
	pADFrm->ShowHideDlgSave (FALSE); // ���ش�����
	pADFrm->ShowHideStatus(); // ��ʾ״̬��
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024);  // ���ý��̷�Χ
	pADFrm->GetProgressCtrl()->SetPos(0);	
	// һ�׽���
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

