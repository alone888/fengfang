#include "stdAfx.h"
#include "math.h"

int gl_nReadIndex = 0;             // 缓冲段索引
CWinThread* m_ReadThread;          // 子线程句柄
ULONGLONG g_TrigPosAD;
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread(PVOID pThreadPara); // ??改变参数名
//########################################################################
// 启动数据采集线程
BOOL MyStartDeviceAD(HANDLE hDevice)
{	
	m_ReadThread = AfxBeginThread(ReadDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_ReadThread->m_bAutoDelete = true; // 自动删除线程
	gl_bDeviceADRun = TRUE;             // 告之采集线程可以工作了
	m_ReadThread->ResumeThread();       // 启动采集线程
	return TRUE;
}

//###############################################################
// 采集线程函数(程序方式读取数据)
UINT ReadDataThread(PVOID hWnd)  
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	gl_nReadIndex = 0; // 复位段索引号
	int nRemainder = AD_FIFO_FULL_LENGTH % (gl_nChannelCount * 256);
	gl_ReadSizeWords = AD_FIFO_FULL_LENGTH - nRemainder; // 读取数据的大小(整个RAM长度64K)
	LONG lRetReadSizeWords = 0;
	HANDLE hDevice = pDoc->m_hDevice; // 设备句柄
	BOOL bRet = FALSE;

	if (!USB2831_InitDeviceAD(hDevice, &ADPara)) // 用硬件参数初始化设备
	{
		AfxMessageBox(_T("不明的初始化错误..."));
		return -1;
	}

	while(gl_bDeviceADRun)  // 循环采集AD数据
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


		// 读64K数据
		bRet = USB2831_ReadDeviceAD(hDevice, ADBuffer[gl_nReadIndex], 
			gl_ReadSizeWords, &lRetReadSizeWords);
// 		if (!bRet)
// 		{
// 			gl_bDeviceADRun = FALSE;
// 			goto ExitReadThread;
// 		}
		gl_nReadIndex++;
		if (gl_nReadIndex > MAX_SEGMENT_COUNT-1) gl_nReadIndex = 0;
		// 发送事件，告诉绘制窗口线程，该批数据已采集完毕
		SetEvent(gl_hEvent); // 控制显示
		if (gl_FirstScreenStop) // 若要求首屏停止，则只第一次刷新	
		{
			gl_bDeviceADRun = FALSE; // 中止AD采集
			USB2831_ReleaseDeviceAD(hDevice); // 释放AD
			goto ExitReadThread;
		}
	} // 线程循环取样
ExitReadThread:
// 	Beep(3000, 1);        // 响铃，采集线程结束
	SetEvent(gl_hExitEvent); // 发出设备成功退出消息，让关闭设备的线程得此消息
	gl_bCollected = TRUE; // 完成了一次采集
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();
	return TRUE;  
} 


BOOL MyStopDeviceAD(HANDLE hDevice)
{
	gl_bDeviceADRun = FALSE;   // 中止AD采集
	WaitForSingleObject(gl_hExitEvent, 100);
	USB2831_ReleaseDeviceAD(hDevice); // 释放AD
	gl_bCreateDevice = FALSE;
	return TRUE;
}

