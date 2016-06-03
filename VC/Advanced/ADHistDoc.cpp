// HistDataDoc.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CADHistDoc
IMPLEMENT_DYNCREATE(CADHistDoc, CDocument)

CADHistDoc::CADHistDoc()
{
	m_Offset = 0;
	m_Rate = 1;	
	m_nFileNum = 0;
	m_ReadDataSize = 256;
	// ǿ�ҽ����ʼ��Ϊ��֪��
	memset(&m_Header, 0x00, sizeof(m_Header));
	m_bFileOpen = FALSE;
	m_nCount = 0;
	m_ScreenOffset = 0;
}

BOOL CADHistDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CADHistDoc::~CADHistDoc()
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
// 	pApp->m_strFilePath[0] = "";
// 	pApp->m_strFilePath[1] = "";
// 	pApp->m_strFilePath[2] = "";
// 	pApp->m_strFilePath[3] = "";
}

BEGIN_MESSAGE_MAP(CADHistDoc, CDocument)
	//{{AFX_MSG_MAP(CADHistDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc diagnostics
#ifdef _DEBUG
void CADHistDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADHistDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc serialization

void CADHistDoc::Serialize(CArchive& ar)
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
// CADHistDoc commands

BOOL CADHistDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//if (!CDocument::OnOpenDocument(NULL))
	//	return FALSE;
	DWORD nFileHeaderSize;
	int ChannelNum = 0;
	CString str;
	if (m_File.Open(lpszPathName, USB2831_modeRead)) // ���ļ�
	{
		m_File.Seek(0, CFile::begin);
		m_File.Read((WORD*)&m_Header, sizeof(m_Header)); // ��ȡ�ļ�ͷ
		nFileHeaderSize = sizeof(m_Header);
		m_ReadDataSize = 8192;
		m_ChannelCount = m_Header.ADPara.LastChannel - m_Header.ADPara.FirstChannel + 1;
		// ������һ�����ݣ���������ʾ	
		m_File.Seek(nFileHeaderSize, CFile::begin);  
		m_File.Read(m_ADBuffer, m_ReadDataSize*2);
		m_FileLength = (ULONG)((m_File.GetLength()-256)/2); // �ļ�����(��)
		m_bFileOpen = TRUE;
	}
	else
	{
		theApp.MsgWarning("����", "Failed to Open File", CPoint(20, 20), 10000);
		return TRUE;
	}
	this->SetPathName(lpszPathName);
	return TRUE;
}

void CADHistDoc::ReadData(void)
{
	m_File.Seek(0,CFile::begin);
	m_File.Read((WORD*)&m_Header, sizeof(m_Header));

	m_File.Seek(sizeof(::_FILE_HEADER)+(m_Offset*2)*m_ChannelCount,CFile::begin);
	try
	{
		m_nCount = m_File.Read((WORD*)&m_ADBuffer, 8192*2);	// sizeof(m_ADBuffer)
	}
	catch(...)
	{
		theApp.MsgWarning("����", "�ļ����ʹ��̳����쳣����", CPoint(20, 20), 10000);
		return;
	}
	m_nCount = m_nCount/m_ChannelCount;
}

void CADHistDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bFileOpen == TRUE)
	{
		m_File.Close();
	}
	
	CDocument::OnCloseDocument();
}
