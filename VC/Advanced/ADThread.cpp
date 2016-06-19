#include "stdAfx.h"
#include "math.h"

int gl_nReadIndex = 0;             // ���������
CWinThread* m_ReadThread;          // ���߳̾��
ULONGLONG g_TrigPosAD;
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread(PVOID pThreadPara); // ??�ı������
//########################################################################
// �������ݲɼ��߳�
BOOL MyStartDeviceAD(HANDLE hDevice)
{	
	m_ReadThread = AfxBeginThread(ReadDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_ReadThread->m_bAutoDelete = true; // �Զ�ɾ���߳�
	gl_bDeviceADRun = TRUE;             // ��֮�ɼ��߳̿��Թ�����
	m_ReadThread->ResumeThread();       // �����ɼ��߳�
	return TRUE;
}

//###############################################################
// �ɼ��̺߳���(����ʽ��ȡ����)
UINT ReadDataThread(PVOID hWnd)  
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	gl_nReadIndex = 0; // ��λ��������
	int nRemainder = AD_FIFO_FULL_LENGTH % (gl_nChannelCount * 256);
	gl_ReadSizeWords = AD_FIFO_FULL_LENGTH - nRemainder; // ��ȡ���ݵĴ�С(����RAM����64K)
	LONG lRetReadSizeWords = 0;
	HANDLE hDevice = pDoc->m_hDevice; // �豸���
	BOOL bRet = FALSE;

	if (!USB2831_InitDeviceAD(hDevice, &ADPara)) // ��Ӳ��������ʼ���豸
	{
		AfxMessageBox(_T("�����ĳ�ʼ������..."));
		return -1;
	}

	while(gl_bDeviceADRun)  // ѭ���ɼ�AD����
	{	

		gl_ReadSizeWords =2048;

		if (ADPara.Frequency>100000)
		{
			gl_ReadSizeWords = 8192;
		} 
		else
		{
			gl_ReadSizeWords = 2048;
		}


		// ��64K����
		bRet = USB2831_ReadDeviceAD(hDevice, ADBuffer[gl_nReadIndex], 
			gl_ReadSizeWords, &lRetReadSizeWords);
// 		if (!bRet)
// 		{
// 			gl_bDeviceADRun = FALSE;
// 			goto ExitReadThread;
// 		}
		gl_nReadIndex++;
		if (gl_nReadIndex > MAX_SEGMENT_COUNT-1) gl_nReadIndex = 0;
		// �����¼������߻��ƴ����̣߳����������Ѳɼ����
		SetEvent(gl_hEvent); // ������ʾ
		if (gl_FirstScreenStop) // ��Ҫ������ֹͣ����ֻ��һ��ˢ��	
		{
			gl_bDeviceADRun = FALSE; // ��ֹAD�ɼ�
			USB2831_ReleaseDeviceAD(hDevice); // �ͷ�AD
			goto ExitReadThread;
		}
	} // �߳�ѭ��ȡ��
ExitReadThread:
// 	Beep(3000, 1);        // ���壬�ɼ��߳̽���
	SetEvent(gl_hExitEvent); // �����豸�ɹ��˳���Ϣ���ùر��豸���̵߳ô���Ϣ
	gl_bCollected = TRUE; // �����һ�βɼ�
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();
	return TRUE;  
} 


BOOL MyStopDeviceAD(HANDLE hDevice)
{
	gl_bDeviceADRun = FALSE;   // ��ֹAD�ɼ�
	WaitForSingleObject(gl_hExitEvent, 100);
	USB2831_ReleaseDeviceAD(hDevice); // �ͷ�AD
	gl_bCreateDevice = FALSE;
	return TRUE;
}

