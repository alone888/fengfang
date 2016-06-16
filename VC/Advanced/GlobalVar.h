#include "StdAfx.h"

extern CSysApp theApp;

////////////////////////////////////////////////////////////////////////
extern CADStatusView* gl_pADStatusView;
extern CADParaCfgView* gl_pParaCfgView;
#define MAX_SEGMENT_COUNT 64
extern WORD ADBuffer[MAX_SEGMENT_COUNT][MAX_SEGMENT_SIZE]; // 缓冲队列
extern WORD showData[8][10000];// WYL 10000个点表示屏幕上要显示的点

//包含时间的原始数据
typedef struct _AD_OrgData       /* rcl */
{
	WORD data[8];
	unsigned int time; //us
} AD_ORG_DATA;

extern AD_ORG_DATA gt_AD_OrgData[100]; // WYL 一次读的数据个数
extern int g_nTimeAxisRange;


extern ULONG gl_ReadSizeWords;	// 读入的数据长度
extern int gl_CurrentIndex; // AD数据处理线程当前缓冲区索引号
extern BOOL gl_bDeviceADRun;
extern USB2831_PARA_AD ADPara;
extern UINT ProcessDataThread(PVOID pThreadPara);  // 绘制数据线程
extern BOOL gl_bCreateFile;
extern int  gl_ProcessGraphicMode; // 为1, 波形显示
extern int  gl_ScreenVolume;	// 设置屏幕显示的量程
extern int  gl_voltVolume;	
extern VOLTRANGE  gl_InputRange[USB2831_MAX_AD_CHANNELS];	// 各通道设置的电压量程范围
extern int  gl_TriggerVolt;       // 触发电压
extern int	gl_MiddleLsb[USB2831_MAX_AD_CHANNELS]; // 求平移电压时的中间值 USB2831_MAX_AD_CHANNELS
extern int  gl_nChannelCount;
extern float gl_PerLsbVolt[USB2831_MAX_AD_CHANNELS]; // 单位LSB的电压值
extern float gl_AnalyzeAllCount;
extern UINT gl_OverLimitCount;
extern int gl_ProcessMoveVolt;	// 为1时, 平移电压
extern BOOL gl_bCreateDevice;
extern BOOL gl_bProgress;  // 是否更新进度条

extern int gl_nProcMode;  // 数据处理方式 1：数字显示  2：波形显示  3：数据存盘
// 采样数据处理方式(gl_nProcMode使用的选项)
#define PROC_MODE_DIGIT 1 // 数字显示
#define PROC_MODE_WAVE 2 // 波形显示
#define PROC_MODE_SAVE 3 // 存盘处理

extern int gl_DigitShowMode; // 数字窗口显示模式
// 数字窗口显示模式(gl_DigitShowMode使用的选项)
#define SHOW_MODE_DEC   0 // 十进制显示
#define SHOW_MODE_HEX   1 // 十六进制显示
#define SHOW_MODE_VOLT  2 // 电压值显示

extern HANDLE gl_hEvent;  // 采集线程与绘制线程的同步信号
extern HANDLE gl_hFileObject;
extern BOOL gl_bCloseFile;
extern HANDLE gl_hExitEvent;

extern ULONGLONG gl_FileLenghtWords;
extern int gl_BufferID; // 当前的缓冲索引号
extern int gl_Offset;     // 当前缓冲段内偏移
extern int gl_nCurrentIndex;      // 当前缓冲段
extern int gl_nDrawIndex;         // 绘图段索引

extern BOOL gl_bDataProcessing;
extern CString g_strFileFullName;

extern LONGLONG gl_nTriggerPos;		// 触发点位置


extern CString strMsg;
extern CString strErr;

#define MsgBox AfxMessageBox

//////////////////////////////////////////////////
#define MAX_OFFSET 8192

// extern SHORT AsynModeBuf[MAX_RAM_SIZE*USB2831_MAX_AD_CHANNELS];

//#############################################################
// external declare
extern int gl_WavePointX;
extern BOOL m_bCreateDevice;
extern BOOL gl_bCollected;
extern BOOL gl_bTileWave;
extern BOOL gl_bSyncMode;    // 采集模式（同步或异步）
extern BOOL gl_FirstScreenStop;

extern ULONG gl_TrigCnt;

extern int gl_signal_enable[USB2831_MAX_AD_CHANNELS];
