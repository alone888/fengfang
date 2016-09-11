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
		strcpy((char*)file_path,CT2CA(lpszPathName));
		m_File.Seek(0, CFile::begin);
		m_File.Read((WORD*)&m_Header, sizeof(m_Header)); // ��ȡ�ļ�ͷ
		nFileHeaderSize = sizeof(m_Header);
		m_ReadDataSize = 8192;
		//m_ChannelCount = m_Header.ADPara.LastChannel - m_Header.ADPara.FirstChannel + 1;
		m_ChannelCount = 8;
		m_ChannelCount = 0;
		for (int i = 0; i < 8 ; i++)
		{
			if (m_Header.input[i]==1)
			{
				m_ChannelCount++;
			}
			hist_m_channel_enable[i] = m_Header.input[i];
		}
		hist_m_channel_cnt = m_ChannelCount;
		// ������һ�����ݣ���������ʾ	
		m_File.Seek(nFileHeaderSize, CFile::begin);  

		ULONG tmp1;
		WORD tmp_read[8];
		ULONG seek_pos = 0;
		g_nTimeAxisRange = 1000000;
		tmp1 = g_nTimeAxisRange/10000;//showdata��ÿ�����ʱ�� 
		int tmp3 = m_Header.ADPara.Frequency/1000;// 1��ms�ж��ٸ�ԭʼ����
		for (ULONG i = 0; i < SHOW_DATA_CNT; i++)
		{
			ULONG tmp2 = i*tmp1*tmp3;
			read_point_offset[i]=tmp2/(1000*8);
		}
		// i*��range/10000)/(1000000/frq) 
		// i*showdata��ÿ�����ʱ��/ÿ��ԭʼ���ݵ�ʱ�䣨usΪ��λ��

		for (int i = 0; i < 10000; i++)
		{
			seek_pos = sizeof(::_FILE_HEADER)+read_point_offset[i]*8*2;

			if(seek_pos+16 < m_FileLength)
			{
				m_File.Seek(seek_pos,CFile::begin);
				m_File.Read((WORD*)&tmp_read, 8*2);
			}
			else
				memset(tmp_read,0x00,sizeof(tmp_read));
			for(int j = 0; j < 8 ; j++)
			{
				showData[j][i] = tmp_read[j];
			}
		}
		//m_File.Read(m_ADBuffer, m_ReadDataSize*2);
		m_FileLength = (ULONG)((m_File.GetLength()-256)/2); // �ļ�����(��)
		m_bFileOpen = TRUE;
	}
	else
	{
		theApp.MsgWarning("����", "Failed to Open File", CPoint(20, 20), 10000);
		return TRUE;
	}
	this->SetPathName(lpszPathName);


	//�����������ʾ�������õ�
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	//ShowControlBar(&pMainFrame->m_wndToolBar,!pMainFrame->m_wndToolBar.IsWindowVisible(),FALSE);
	if (pMainFrame->m_wndToolBar.IsWindowVisible()) //�������������ʾ�ţ���ô�����أ��������ʾ��
	{
		pMainFrame->m_wndToolBar.ShowWindow(SW_HIDE);
	}
	else
	{
		pMainFrame->m_wndToolBar.ShowWindow(SW_SHOW);
	}
	pMainFrame->RecalcLayout(); //ע�������д���У�����ֹ��������أ�����������Ȼ���ڵ�������Ҫ���¼���

	return TRUE;
}

void CADHistDoc::ReadData(void)
{
	m_File.Seek(0,CFile::begin);
	m_File.Read((WORD*)&m_Header, sizeof(m_Header));
	//m_File.Seek(sizeof(::_FILE_HEADER)+(m_Offset*2)*8,CFile::begin);
	WORD tmp_read[8];
	ULONG seek_pos = 0;

	if(read_point_offset[9999] == 0)
		return;
	try
	{
		for (int i = 0; i < 10000; i++)
		{
			seek_pos = sizeof(::_FILE_HEADER)+read_point_offset[i]*8*2+(m_Offset*2)*8;

			if(seek_pos+16 < m_FileLength*2)// m_FileLength WORD seek_pos byte
			{
				m_File.Seek(seek_pos,CFile::begin);
				m_File.Read((WORD*)&tmp_read, 8*2);
			}
			else
				memset(tmp_read,0x00,sizeof(tmp_read));
			for(int j = 0; j < 8 ; j++)
			{
				showData[j][i] = tmp_read[j];
			}
		}
	}
	catch(...)
	{
		theApp.MsgWarning("����", "�ļ����ʹ��̳����쳣����", CPoint(20, 20), 10000);
		return;
	}
	m_nCount = 10000;
}

void CADHistDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bFileOpen == TRUE)
	{
		//�������ʾ��ʾ�������õ�
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
		//ShowControlBar(&pMainFrame->m_wndToolBar,!pMainFrame->m_wndToolBar.IsWindowVisible(),FALSE);
		if (pMainFrame->m_wndToolBar.IsWindowVisible()) //�������������ʾ�ţ���ô�����أ��������ʾ��
		{
			pMainFrame->m_wndToolBar.ShowWindow(SW_HIDE);
		}
		else
		{
			pMainFrame->m_wndToolBar.ShowWindow(SW_SHOW);
		}
		pMainFrame->RecalcLayout(); //ע�������д���У�����ֹ��������أ�����������Ȼ���ڵ�������Ҫ���¼���
		
		m_File.Close();
	}
	
	CDocument::OnCloseDocument();
}


ULONG CADHistDoc::ReadDataForExcel(WORD *buf,ULONG size,ULONG offset)
{
	ULONG read_size = 0;
	m_File.Seek(0,CFile::begin);
	m_File.Read((WORD*)&m_Header, sizeof(m_Header));
	//m_File.Seek(sizeof(::_FILE_HEADER)+(m_Offset*2)*8,CFile::begin);
	try
	{
		m_File.Seek(sizeof(::_FILE_HEADER)+offset,CFile::begin);
		read_size = m_File.Read((WORD*)buf, size);
		return read_size;
	}
	catch(...)
	{
		theApp.MsgWarning("����", "�ļ����ʹ��̳����쳣����", CPoint(20, 20), 10000);
		return 0;
	}
}

 void CADHistDoc::Retern_FilePath(unsigned char *path)
{
	int len = 0;
	strcpy((char*)path,(char*)file_path);
	len = strlen((char*)path);
	path[len-4] = 0;
}
