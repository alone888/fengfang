// QSortData.h: interface for the CQSortData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QSORTDATA_H__05802C51_F215_4B0C_A99D_F1D37D869F96__INCLUDED_)
#define AFX_QSORTDATA_H__05802C51_F215_4B0C_A99D_F1D37D869F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_BUFFER 8192 // 最大的排序个数

typedef struct _SORT_COUNT
{ 
	int Value;
	int CountNum;
} SORT_COUNT, *PSORT_COUNT; 

class CQSortData  
{

public:
	CQSortData();

public:
	void SetSortDir(int nDir = 0);
	int GetSortDir();
	void Sort(WORD* data, int left, int right);	
	int GetDataTypes();
	int GetMidDataCount();
	int GetMidData();
	int GetMinData();
	int GetMaxData();
	virtual ~CQSortData();

private:
	BOOL m_bNewData;
	int m_DataMax; // 最大数
	int m_DataMin; // 最小数
	int m_DataTypes; // 数据种数	
	int m_SortDir; // 排序方向(升序或降序)
	int nBufferSize; // 需动态分配的内存大小
	SORT_COUNT m_DataMiddle; // 中间数
	PSORT_COUNT  m_Result; // _SORTCOUNT结构体指针, 指向分析后的数据
	WORD* m_pBuffer;
private:
 	void SortAsc(WORD* data, int left, int right);	// 升序排列
	void SortDsc(WORD* data, int left, int right); // 降序排列
	void ProcessData(void);
};

#endif // !defined(AFX_QSORTDATA_H__05802C51_F215_4B0C_A99D_F1D37D869F96__INCLUDED_)
