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
	gl_last_end_id=0;
	m_ReadThread = AfxBeginThread(ReadDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_ReadThread->m_bAutoDelete = true; // �Զ�ɾ���߳�
	gl_bDeviceADRun = TRUE;             // ��֮�ɼ��߳̿��Թ�����
	m_ReadThread->ResumeThread();       // �����ɼ��߳�

	
	return TRUE;
}
void AD_Filter_Data_All();
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

		gl_ReadSizeWords = 8192;

// 		if (ADPara.Frequency>100000)
// 		{
// 			gl_ReadSizeWords = 8192;
// 		} 
// 		else
// 		{
// 			gl_ReadSizeWords = 2048*2;
// 		}


		// ��64K����
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
			AfxMessageBox(_T("OMG�� ����ɼ�����USB�߱�����..."));	
			memset(showData,0,sizeof(showData));
			gl_last_end_id = 0;
 			goto ExitReadThread;
 		}

		AD_Filter_Data_All();
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

	memset(showData,0,sizeof(showData));
	gl_last_end_id = 0;

	return TRUE;
}


void AD_Filter_Data(int H_OR_L,int ch,double *filterParam ,signed short * src,signed short * out)
{
	int i = 0;
	//out �˲����
	//filterParam  �˲�������
	//[0][][]��ͨ   [1][][]��ͨ
	static signed short last_Date[2][8][FILTER_DEEP];
	signed short temp[1024+FILTER_DEEP];
	signed short tempOut[1024+FILTER_DEEP];
	memcpy(temp,src,sizeof(temp));

	//  ���ϴε�����ƴ������µ�����ǰ��
	for (i = 0; i < FILTER_DEEP; i++) 
	{
		temp[i] = last_Date[H_OR_L][ch][i];
	}

	//  ��������
	for (int n = 0; n < 1024; n++) 
	{
		tempOut[n] = 0;
		for ( i = 0; i < FILTER_DEEP; i++)
		{
			tempOut[n] +=filterParam[i]*temp[n+FILTER_DEEP-i];	
		}
	}
	//memcpy(out+FILTER_DEEP*2,tempOut,1024*2);

	//[0][][]��ͨ   [1][][]��ͨ
	//���汾�����ݵ����FILTER_DEEP�����������´��˲�
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
	static signed short tmp_signel_data[8][1024+FILTER_DEEP];//ԭʼ����
	static signed short tmp_signel_data2[8][1024+FILTER_DEEP];//������ͨ�������
	static signed short tmp_signel_data3[8][1024+FILTER_DEEP];//������ͨ�������

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

	for(ch = 0; ch < 8 ; ch++) //  ��ͨ
	{
		if (g_filer[ch][1] != 0 && g_filter_data_l[ch][0] != 0)//�ж��Ƿ���Ҫ��ͨ
		{
			AD_Filter_Data(0,ch,g_filter_data_l[ch] ,tmp_signel_data[ch],tmp_signel_data2[ch]);
		}
		else
		{
			memcpy(tmp_signel_data2[ch],tmp_signel_data[ch],sizeof(tmp_signel_data[ch]));	
		}

		if (g_filer[ch][0] != 0 && g_filter_data_h[ch][0] != 0)//�ж��Ƿ���Ҫ��ͨ
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
		for (i = 0; i < 1024; i++) // ������˺������
		{
			ADBuffer[gl_nReadIndex][i*8+ch] = tmp_signel_data3[ch][i+FILTER_DEEP] + 4096;
		}
	}
}


