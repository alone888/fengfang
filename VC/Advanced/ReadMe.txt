
========================================================================
			工程完善记录
2011-7-29 V6.01.21 新做此演示程序		TW
2013-6-26 V6.01.22 修改此演示程序		LYB
2013-7-29 V6.01.23 添加物理ID测试		TW
2014-5-14 V6.01.24 修改演示程序存盘问题         JRL
2015-04-21 V6.01.25  修改回放条文件偏移点问题   ZB

========================================================================
			工程中主要类关系
========================================================================
CWinApp
	CSysApp

CMDIFrameWnd
	CMDITabFrameWnd
		CMainFrame

CMDIChildWnd
	CADFrm
	CDIOFrm
	CCNTFrm
	CDAFrm

CDocument
	CADDoc
	CDIODoc
	CCNTDoc
	CDADoc

CScrollView
	CADDigitView
	CADWaveView

CFormView
	CADParaCfgView
	CADStatusView
	CDIOView
	CCNTView
	CDAView

CWnd
	COScopeCtrl