// QSortData.cpp: implementation of the CQSortData class.
//////////////////////////////////////////////////////////////////////
//*******************************************************************
//
// QSortData类的使用说明：
// 能够对SHORT指针指向的一组数据先进行指定长度的排序,求出中间数(出现概率最大的数)
// 原型：CQSortData::Sort(SHORT* data, int left, int right)
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
	m_SortDir = 0;	 // 升序
	m_DataMiddle.Value = 0; 
	m_DataMiddle.CountNum = 0;
	m_DataTypes = 1;
	m_bNewData = FALSE; 
}

CQSortData::~CQSortData()
{
	
}

int CQSortData::GetMaxData() // 取最大数
{
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMax;
}

int CQSortData::GetMinData() // 取最小数
{
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return m_DataMin;
}

int CQSortData::GetMidData() // 取中间数
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

int CQSortData::GetDataTypes() // 出现的数据种数
{	
	if (m_bNewData)
	{
		ProcessData();
		m_bNewData = FALSE;
	}
	return  m_DataTypes;
}

int CQSortData::GetSortDir() // 取得排序方向(升序或降序)
{
	return m_SortDir;
}

void CQSortData::SetSortDir(int nDir) // 设置排序方向(缺省为 =0升序)
{
	m_SortDir = nDir;
}

//#################################################################
// 对data指针指向的缓冲区从下标为left到right的元素进行 升序排序
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
		{ // 交换left, right的值
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
// 对data指针指向的缓冲区从下标为left到right的元素进行  降序排序
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
		SortAsc(data, left, right); // 升序排序(省缺)
		m_DataMin = data[left];
		m_DataMax = data[right];
	}
	else
	{
		SortDsc(data, left, right); //降序排序
		m_DataMin = data[right];
		m_DataMax = data[left];
	}

	m_bNewData = TRUE;
	nBufferSize = right - left;
	m_pBuffer = data;  // 把指针data的值传递给Buffer指针   
}

//#################################################################
// 从Result数组中查找中间数（出现频率最大的数） 
void CQSortData::ProcessData(void)
{
	m_Result = new SORT_COUNT[nBufferSize]; // 动态分配合适大小的内存
	m_Result[0].Value = m_pBuffer[0];
	m_Result[0].CountNum = 0;
	m_DataMiddle.CountNum = 0;
	m_DataMiddle.Value = m_Result[0].Value;
	int TypeIdx = 0;
	for (int Index=0; Index<nBufferSize; Index++) 
	{ 
		if (m_pBuffer[Index] == m_Result[TypeIdx].Value)
		{
			m_Result[TypeIdx].CountNum++; // 统计第TypeIdx个数出现的次数, 保存在Result[]中			
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

	// 如果没有中间数
	if (m_DataMiddle.CountNum == 1)
	{
		AfxMessageBox(_T("没有中间数"));
		m_DataMiddle.CountNum = 0;
	}

	delete [] m_Result; // 释放动态分配的内存
	m_Result = NULL;
}


