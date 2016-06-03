// QSortData.cpp: implementation of the CQSortData class.
//////////////////////////////////////////////////////////////////////
//*******************************************************************
//
// QSortData���ʹ��˵����
// �ܹ���SHORTָ��ָ���һ�������Ƚ���ָ�����ȵ�����,����м���(���ָ���������)
// ԭ�ͣ�CQSortData::Sort(SHORT* data, int left, int right)
//
//*******************************************************************
#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQSortData::CQSortData()
{
	m_SortDir = 0;	 // ����
	m_DataMiddle.Value = 0; 
	m_DataMiddle.CountNum = 0;
	m_DataTypes = 1;
	m_bNewData = FALSE; 
}

CQSortData::~CQSortData()
{
	
}

int CQSortData::GetMaxData() // ȡ�����
{
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMax;
}

int CQSortData::GetMinData() // ȡ��С��
{
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMin;
}

int CQSortData::GetMidData() // ȡ�м���
{
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMiddle.Value;
}

int CQSortData::GetMidDataCount()
{
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMiddle.CountNum;
}

int CQSortData::GetDataTypes() // ���ֵ���������
{	
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return  m_DataTypes;
}

int CQSortData::GetSortDir() // ȡ��������(�������)
{
	return m_SortDir;
}

void CQSortData::SetSortDir(int nDir) // ����������(ȱʡΪ =0����)
{
	m_SortDir = nDir;
}

//#################################################################
// ��dataָ��ָ��Ļ��������±�Ϊleft��right��Ԫ�ؽ��� ��������
void CQSortData::SortAsc(WORD* data, int left, int right) 
{
	register int Tleft, Tright;  
	SHORT mid, temp;     
	Tleft = left; Tright = right;
	mid = data[(int)(left+right)/2];  
	
	do{
		while (data[Tleft]<mid && Tleft<right)
			Tleft++;

		while (mid<data[Tright] && Tright>left)
			Tright--;

		if (Tleft <= Tright) 
		{ // ����left, right��ֵ
			temp = data[Tleft];
			data[Tleft] = data[Tright];
			data[Tright] = temp;
			Tleft++; Tright--;
		}
	} while (Tleft <= Tright);

	if (left<Tright)  SortAsc(data, left, Tright);   
	if (Tleft<right) SortAsc(data, Tleft, right);
}

//#################################################################
// ��dataָ��ָ��Ļ��������±�Ϊleft��right��Ԫ�ؽ���  ��������
void CQSortData::SortDsc(WORD* data, int left, int right) 
{
	register int Tleft, Tright;  
	SHORT mid, temp;     
	Tleft = left; Tright=right;
	mid = data[(int)(left+right)/2];  
	
	do{
		while ((data[Tleft]>mid) && (Tleft<right))
			Tleft++;

		while ((mid > data[Tright]) && (Tright>left))
			Tright--;

		if (Tleft <= Tright) 
		{ 
			temp = data[Tleft];
			data[Tleft] = data[Tright];
			data[Tright] = temp;
			Tleft++; Tright--;
		}
	} while (Tleft <= Tright);

	if (left < Tright)  SortDsc(data,left, Tright);   
	if (Tleft < right) SortDsc(data, Tleft, right);
}

//############################################################
// 
void CQSortData::Sort(WORD* data, int left, int right)
{
	
	if (!m_SortDir)
	{
		SortAsc(data, left, right); // ��������(ʡȱ)
		m_DataMin = data[left];
		m_DataMax = data[right];
	}
	else
	{
		SortDsc(data, left, right); //��������
		m_DataMin = data[right];
		m_DataMax = data[left];
	}

	m_bNewData = TRUE;
	nBufferSize = right - left;
	m_pBuffer = data;  // ��ָ��data��ֵ���ݸ�Bufferָ��   
}

//#################################################################
// ��Result�����в����м���������Ƶ���������� 
void CQSortData::ProcessData(void)
{
	m_Result = new SORT_COUNT[nBufferSize]; // ��̬������ʴ�С���ڴ�
	m_Result[0].Value = m_pBuffer[0];
	m_Result[0].CountNum = 0;
	m_DataMiddle.CountNum = 0;
	m_DataMiddle.Value = m_Result[0].Value;
	int TypeIdx = 0;
	for (int Index=0; Index<nBufferSize; Index++) 
	{ 
		if (m_pBuffer[Index] == m_Result[TypeIdx].Value)
		{
			m_Result[TypeIdx].CountNum++; // ͳ�Ƶ�TypeIdx�������ֵĴ���, ������Result[]��			
		}
		else
		{ 
			if (m_Result[TypeIdx].CountNum > m_DataMiddle.CountNum) 
			{
				m_DataMiddle.CountNum = m_Result[TypeIdx].CountNum;	  
				m_DataMiddle.Value = m_Result[TypeIdx].Value;	 
			}
			
			TypeIdx++;
			m_Result[TypeIdx].Value = m_pBuffer[Index];
			m_Result[TypeIdx].CountNum = 1;  
			m_DataTypes++;
		}		
	}
	
	if (m_Result[TypeIdx].CountNum > m_DataMiddle.CountNum)
	{
		m_DataMiddle.CountNum = m_Result[TypeIdx].CountNum;	  
		m_DataMiddle.Value = m_Result[TypeIdx].Value;	  
	}

	// ���û���м���
	if (m_DataMiddle.CountNum == 1)
	{
		AfxMessageBox(_T("û���м���"));
		m_DataMiddle.CountNum = 0;
	}

	delete [] m_Result; // �ͷŶ�̬������ڴ�
	m_Result = NULL;
}


