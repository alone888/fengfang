// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D485DEFF_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_STDAFX_H__D485DEFF_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
/////////////////////////////////
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

// CADHistScope window

/////////////////////////////////
// 用户填加部分
#include "math.h"
#include "usb2831.h"
#include "UserDef.h"
#include "Sys.h"
#include "MainFrm.h"


#include "ADFrm.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADStatusView.h"
#include "ADParaCfgView.h"
#include "ADScopeCtrl.h"

#include "ADHistFrm.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
#include "ADHistWaveView.h"
#include "ADHistScope.h"

#include "DIOFrm.h"
#include "DIODoc.h"
#include "DIOView.h"

#include "DAFrm.h"
#include "DADoc.h"
#include "DAView.h"

#include "QSortData.h"

#include "GlobalVar.h"
#include "BalloonHelp.h"
#define WM_SHOW_WAVE  WM_USER+100
#define WM_SHOW_DIGIT WM_USER+101
 //{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_STDAFX_H__D485DEFF_85AB_11D1_B878_A68EB300DD63__INCLUDED_)




