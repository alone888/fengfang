// HistDigitView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strSampleCtrl[7] = 
{
	"��������: 64K��", 
	"��������: 32K��", 
	"��������: 16K��", 
	"��������: 8K��", 
	"��������: 4K��", 
	"��������: 2K��", 
	"��������: 1K��"
};


/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView

IMPLEMENT_DYNCREATE(CADHistDigitView, CListView)

CADHistDigitView::CADHistDigitView()
{
	nChooseMode = 1;
}

CADHistDigitView::~CADHistDigitView()
{
}


BEGIN_MESSAGE_MAP(CADHistDigitView, CListView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CADHistDigitView)
	ON_COMMAND(IDM_ShowHex, OnShowHex)
	ON_UPDATE_COMMAND_UI(IDM_ShowHex, OnUpdateShowHex)
	ON_COMMAND(IDM_ShowVolt, OnShowVolt)
	ON_UPDATE_COMMAND_UI(IDM_ShowVolt, OnUpdateShowVolt)
	ON_COMMAND(IDM_ShowDec, OnShowDec)
	ON_UPDATE_COMMAND_UI(IDM_ShowDec, OnUpdateShowDec)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView drawing

void CADHistDigitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	
}
/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView diagnostics

#ifdef _DEBUG
void CADHistDigitView::AssertValid() const
{
	CListView::AssertValid();
}

void CADHistDigitView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CADHistDoc* CADHistDigitView::GetDocument()// non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADHistDoc)));
	return (CADHistDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView message handlers

void CADHistDigitView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	int ChannelCount = 1;	

	DWORD type;
	type= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES	/* |
		LVS_EX_HEADERDRAGDROP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_SUBITEMIMAGES*/;	
	// �ı��б���ͼ�ķ��������Ϊ��ʾ����
	GetListCtrl().SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(type));	

	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	pDoc->m_hWndDigit = m_hWnd; 

	CString str;
	CListCtrl& List = GetListCtrl();
	
	long VoltTopRange, VoltBottomRange;
	VoltTopRange = pDoc->m_Header.VoltTopRange;       // ��ѹ�����ֵ
	VoltBottomRange = pDoc->m_Header.VoltBottomRange; // ��ѹ����Сֵ
	m_PerLsbVolt = (float)((VoltTopRange - VoltBottomRange) / AD_LSB_COUNT);

	CADHistFrm* pADHistFrm = (CADHistFrm*)GetParentFrame();	
	CListBox* pList = (CListBox*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_FILESINFO));
	switch(pDoc->m_Header.BusType)
	{ //1:PCI, 2:USB, 3:ISA, 4:PC104
		case PCI_BUS:
			str = "BusType:PCI";
			break;
		case USB_BUS:
			str = "BusType:USB";
			break;
		case ISA_BUS:
			str = "BusType:ISA";
			break;
		case PC104_BUS:
			str = "BusType:PC104";
			break;
		default:
			AfxMessageBox(_T("������Ч�����ļ���"));
			return;
			//ASSERT(FALSE);
	} // �忨����	
	pList->AddString(str);
	str.Format(_T("DeviceNum:%x"), pDoc->m_Header.DeviceNum);
	pList->AddString(str);

	
	str.Format(_T("HeadSizeBytes:%d"), pDoc->m_Header.HeadSizeBytes);
	pList->AddString(str);
	
	str.Format(_T("�������ޣ�%d"), pDoc->m_Header.VoltTopRange);
	pList->AddString(str);

	str.Format(_T("�������ޣ�%d"), pDoc->m_Header.VoltBottomRange);
	pList->AddString(str);
	if (pDoc->m_Header.ADPara.ADMode == USB2831_ADMODE_SEQUENCE)
		str = "�ɼ���ʽ�������ɼ�";
	else
		str = "�ɼ���ʽ������ɼ�";
	pList->AddString(str);
	str.Format(L"����Ƶ��:%dHz", pDoc->m_Header.ADPara.Frequency);
	pList->AddString (str); // ����Ƶ��
	switch (pDoc->m_Header.ADPara.TriggerMode)
	{
	case 0:
		str.Format(L"����ģʽ: ����ڴ���");
		break;
	case 1:
		str.Format(L"����ģʽ: Ӳ���ⴥ��(�󴥷�)");
		break;
	}
	pList->AddString (str); // ������ʽ



	switch (pDoc->m_Header.ADPara.TriggerType)
	{
	case 0:
		str.Format(_T("��������: ���ش���"));
		break;
	case 1:
		str.Format(_T("��������: ���崥��(��ƽ)"));
		break;
				default:
					ASSERT(FALSE);
	}
	pList->AddString(str); // ��������

	switch (pDoc->m_Header.ADPara.TriggerDir)
	{
	case 0:
		str.Format(_T("��������: ���򴥷�"));
		break;
	case 1:
		str.Format(_T("��������: ���򴥷�"));
		break;
	case 2:
		str.Format(_T("��������: �����򴥷�"));
		break;
				default:
					ASSERT(FALSE);
	}
	pList->AddString(str); // ��������
	switch (pDoc->m_Header.ADPara.TriggerSource)
	{
	case 0:
		str.Format(L"����Դ:   ATRģ��������");
		break;
	case 1:
		str.Format(L"����Դ:   DTRģ��������");
		break;
	}
	pList->AddString(str); // ����Դ
	

	switch (pDoc->m_Header.ADPara.ClockSource)
	{
	case 0:
		str.Format(_T("ʱ��Դ: �ڲ�ʱ��Դ"));
		break;
	case 1:
		str.Format(_T("ʱ��Դ: �ⲿʱ��Դ"));
		break;
				default:
					ASSERT(FALSE);
	}
	pList->AddString(str); // ʱ��Դ

	str.Format(_T("����λ��:%d"), pDoc->m_Header.nTriggerPos);
	pList->AddString(str); // ����λ��

	ChannelCount = pDoc->m_Header.ADPara.LastChannel - pDoc->m_Header.ADPara.FirstChannel + 1;
	CEdit* pOffsetEdit = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	str.Format(_T("%d"), pDoc->m_Offset);
	pOffsetEdit->SetWindowText(str);  // ��ʾ�ļ�ƫ��

	CEdit* pLength = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_FileLength));	
	str.Format(_T("%d"), pDoc->m_FileLength);
	pLength->SetWindowText(str);  // ��ʾ�ļ����ȣ��֣�
	
		
	CSliderCtrl* pSliderScreenCur = (CSliderCtrl*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_SLIDER_ScreenCur));	



	// ֮���Գ���ͨ�����������ÿͨ������
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));
	pSliderOffset->SetRange(0, ((pDoc->m_FileLength-pDoc->m_ReadDataSize)/ChannelCount)); // ���û�������Χ���֣�
	pSliderOffset->SetPageSize(1000);

	pSliderScreenCur->SetRange(0, (pDoc->m_FileLength/ChannelCount)); // ���û�������Χ���֣�
	pSliderScreenCur->SetPageSize(1000);

	CEdit* pEditPerChannel = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_PerChannel));	
    str.Format(_T("%d"), (pDoc->m_FileLength-pDoc->m_ReadDataSize)/ChannelCount);
	pEditPerChannel->SetWindowText(str);  // ��ʾÿͨ������

	CComboBox* pRateCombo = (CComboBox*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	pRateCombo->SetCurSel(0);
	
	CStatic* pCardTypeStatic = (CStatic*)pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_STATIC_CardType);
	pCardTypeStatic->SetWindowText(pDoc->m_File.GetFilePath()); // �ļ�·����

	List.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 42, -1);  // �����һ��
	int Row = 0, Col = 0;

	List.InsertColumn(1, str, LVCFMT_LEFT, 55, -1);

	for (Row=0; Row<(int)(pDoc->m_ReadDataSize/ChannelCount); Row++)
	{
		str.Format(_T("%d"), Row);
		List.InsertItem(Row, str);  // ����к�, Ӧһ������һ������	
	} 
	str.Format(_T("file:%s"), pDoc->m_File.GetFileName());
	pDoc->SetTitle(str); 
 	SetListView(); // ���б����������
	GetListCtrl().SetHotItem(0);
}

void CADHistDigitView::SetListView(void)
{
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	int nChannelCount = pDoc->m_Header.ADPara.LastChannel - pDoc->m_Header.ADPara.FirstChannel + 1;
	int Channel = 0;
	CListCtrl& List = GetListCtrl();
	int Col = 0, Row = 0;
	int ADLsb = 0, ADData = 0;
	float fADVolt = 0.0f;
	pDoc->ReadData();
	List.ShowWindow(SW_HIDE);
	List.SetRedraw(FALSE);
	int index(0);
 	//int start = pDoc->m_Offset % nChannelCount;
	int start = 0;

	CString strChannel;
	for (Channel = 0; Channel < nChannelCount; Channel++)
	{
		List.DeleteColumn(Channel + 1);
		strChannel.Format(_T("CH%d"), pDoc->m_Header.ADPara.FirstChannel + Channel); // ȡ��ͷ��Ϣ�����ͨ����
		List.InsertColumn(Channel + 1, strChannel, LVCFMT_LEFT, 75, -1);
	}

	///////////////////////////	
	for (Row=0; Row<(int)(pDoc->m_ReadDataSize); Row++)
	{
		swprintf_s(str, _T("%d"), pDoc->m_Offset+Row);
		List.SetItemText(Row, 0, str);  // ����Index��
		for (Col=0; Col<nChannelCount; Col++)  
		{
			switch(nChooseMode)
			{
			case 1: // ʮ������ʾ
				swprintf_s(str, _T("%d"), (pDoc->m_ADBuffer[index] - AD_LSB_HALF)); 
				
				break;
			case 2: // ʮ��������ʾ
				ADLsb = pDoc->m_ADBuffer[index]; // �ӻ������л�����յ�ԭʼ������LSB(δ���θ�2λ)
				swprintf_s(str, _T("%d:%d:%6x"), (ADLsb>>15)&0x0001, (ADLsb>>14)&0x0001, ADLsb);
				break;
			case 3:
				ADLsb = (pDoc->m_ADBuffer[index]); // �ӻ������л�����յ�ԭʼ������LSB
				//ADData = ADLsb - AD_LSB_HALF; // ��ԭʼ�����ݼ�ȥƽ�Ƶ�ѹ��ֵ�Ͱ��ƫ����ֵ
				//fADVolt = m_PerLsbVolt * ADData;					
				fADVolt = m_PerLsbVolt * (ADLsb &MASK_MSB) + pDoc->m_Header.VoltBottomRange;					
				swprintf_s(str, _T("%.2f"), fADVolt);	
				break;
			default:
				ASSERT(FALSE);
			}
			if(index >(INT)((pDoc->m_nCount) * nChannelCount))
			{
				List.SetRedraw(TRUE);
				List.ShowWindow(SW_SHOW);
				return;	
			}
			List.SetItemText(Row,(start % nChannelCount)+1, str);// ��Ӹ�������
			start++;
			index++;
		}
	} 
	List.SetRedraw(TRUE);
	List.ShowWindow(SW_SHOW);
}

BOOL CADHistDigitView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~(LVS_LIST | LVS_ICON | LVS_SMALLICON);
	cs.style |= LVS_REPORT;
	cs.style |= LVS_SINGLESEL;
	cs.style |= LVS_SHOWSELALWAYS;
	return CListView::PreCreateWindow(cs);
}

void CADHistDigitView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CListView::OnUpdate(pSender, lHint, pHint);	
}

void CADHistDigitView::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This block was added by the Pop-up Menu component
	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		
		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_ShowMode));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
		point.x, point.y, 
		pWndPopupOwner);
}

void CADHistDigitView::OnShowHex() 
{
	// TODO: Add your command handler code here
	nChooseMode = 2;
	SetListView(); // ˢ���б�
}

void CADHistDigitView::OnUpdateShowHex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode == 2);
}

void CADHistDigitView::OnShowVolt() 
{
	// TODO: Add your command handler code here
	nChooseMode = 3;
	SetListView(); // ˢ���б�
}

void CADHistDigitView::OnUpdateShowVolt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode == 3);
}

void CADHistDigitView::OnShowDec() 
{
	// TODO: Add your command handler code here
	nChooseMode = 1; // ˢ���б�
	SetListView(); // ˢ���б�
}

void CADHistDigitView::OnUpdateShowDec(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode == 1);
}

void CADHistDigitView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	CRect rect;	
	GetWindowRect(&rect);	
	pDoc->m_nDigitWidth = rect.right+7;

}

void CADHistDigitView::SetHotItem(int Index)
{
	GetListCtrl().SetHotItem(Index);
}

void CADHistDigitView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CADHistDigitView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default	
	CListView::OnVScroll(nSBCode, nPos, pScrollBar);
}

