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
	gl_last_end_id=0;
	m_ReadThread = AfxBeginThread(ReadDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_ReadThread->m_bAutoDelete = true; // 自动删除线程
	gl_bDeviceADRun = TRUE;             // 告之采集线程可以工作了
	m_ReadThread->ResumeThread();       // 启动采集线程

	
	return TRUE;
}
void AD_Filter_Data_All();
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

		gl_ReadSizeWords = 8192;

// 		if (ADPara.Frequency>100000)
// 		{
// 			gl_ReadSizeWords = 8192;
// 		} 
// 		else
// 		{
// 			gl_ReadSizeWords = 2048*2;
// 		}


		// 读64K数据
		bRet = USB2831_ReadDeviceAD(hDevice, ADBufferForFilter, 
			gl_ReadSizeWords, &lRetReadSizeWords);

		// for debug
		/*if(lRetReadSizeWords != gl_ReadSizeWords)
		{
		lRetReadSizeWords -= 1;
		lRetReadSizeWords +=1;
		}*/
		if (!bRet)
 		{
 			
			gl_bDeviceADRun = FALSE;
			AfxMessageBox(_T("OMG！ 好像采集器的USB线被拔了..."));	
			memset(showData,0,sizeof(showData));
			gl_last_end_id = 0;
 			goto ExitReadThread;
 		}

		AD_Filter_Data_All();
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

	memset(showData,0,sizeof(showData));
	gl_last_end_id = 0;

	return TRUE;
}


void AD_Filter_Data(int H_OR_L,int ch,double *filterParam ,signed short * src,signed short * out)
{
	int i = 0;
	//out 滤波结果
	//filterParam  滤波器参数
	//[0][][]低通   [1][][]高通
	static signed short last_Date[2][8][FILTER_DEEP];
	signed short temp[1024+FILTER_DEEP];
	signed short tempOut[1024+FILTER_DEEP];
	memcpy(temp,src,sizeof(temp));

	//  将上次的数据拼到这次新的数据前端
	for (i = 0; i < FILTER_DEEP; i++) 
	{
		temp[i] = last_Date[H_OR_L][ch][i];
	}

	//  过滤数据
	for (int n = 0; n < 1024; n++) 
	{
		tempOut[n] = 0;
		for ( i = 0; i < FILTER_DEEP; i++)
		{
			tempOut[n] +=filterParam[i]*temp[n+FILTER_DEEP-i];	
		}
	}
	//memcpy(out+FILTER_DEEP*2,tempOut,1024*2);

	//[0][][]低通   [1][][]高通
	//保存本次数据的最后FILTER_DEEP个数，用于下次滤波
	for (i = 0; i < FILTER_DEEP; i++) 
	{
		last_Date[H_OR_L][ch][i] = temp[1024+i];
	}

	for (i = 1023; i>=0; i--) 
	{
		tempOut[i+FILTER_DEEP] = tempOut[i];
	}
	memcpy(out,tempOut,sizeof(tempOut));
}
void AD_Filter_Data_All()
{
	int i = 0, ch = 0;
	static signed short tmp_signel_data[8][1024+FILTER_DEEP];//原始数据
	static signed short tmp_signel_data2[8][1024+FILTER_DEEP];//经过低通后的数据
	static signed short tmp_signel_data3[8][1024+FILTER_DEEP];//经过高通后的数据

	for (ch = 0; ch < 8; ch++)
	{
		if (g_filer[ch][1] == 0 && g_filer[ch][0] == 0)
			continue;
		for ( i = 0; i < 1024; i++)
		{
			tmp_signel_data[ch][i+FILTER_DEEP] = ADBufferForFilter[ch+i*8] - 4096;
		}
	}

	memcpy(ADBuffer[gl_nReadIndex],ADBufferForFilter,sizeof(ADBufferForFilter));

	for(ch = 0; ch < 8 ; ch++) //  低通
	{
		if (g_filer[ch][1] != 0 && g_filter_data_l[ch][0] != 0)//判断是否需要低通
		{
			AD_Filter_Data(0,ch,g_filter_data_l[ch] ,tmp_signel_data[ch],tmp_signel_data2[ch]);
		}
		else
		{
			memcpy(tmp_signel_data2[ch],tmp_signel_data[ch],sizeof(tmp_signel_data[ch]));	
		}

		if (g_filer[ch][0] != 0 && g_filter_data_h[ch][0] != 0)//判断是否需要高通
		{
			AD_Filter_Data(1,ch,g_filter_data_h[ch] ,tmp_signel_data2[ch],tmp_signel_data3[ch]);
		}
		else
		{
			memcpy(tmp_signel_data3[ch],tmp_signel_data2[ch],sizeof(tmp_signel_data[ch]));		
		}
	}
	
	for( ch = 0; ch < 8; ch++)
	{
		if (g_filer[ch][1] == 0 && g_filer[ch][0] == 0)
			continue;
		for (i = 0; i < 1024; i++) // 回填过滤后的数据
		{
			ADBuffer[gl_nReadIndex][i*8+ch] = tmp_signel_data3[ch][i+FILTER_DEEP] + 4096;
		}
	}
}


