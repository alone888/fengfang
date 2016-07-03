#include "StdAfx.h"

////////////////////////////////////////////////////////////////////////
CADStatusView* gl_pADStatusView = NULL;
CADParaCfgView* gl_pParaCfgView = NULL;
WORD ADBuffer[MAX_SEGMENT_COUNT][MAX_SEGMENT_SIZE]; // 缓冲队列8192*64
WORD ADBufferForFilter[MAX_SEGMENT_SIZE]; // 缓冲队列8192
WORD showData[8][10000];// WYL 10000个点表示屏幕上要显示的点
UINT g_filer[8][2] = {0}; // 8个通道的 0--高通 1--低通滤波
double g_filter_data_h[8][FILTER_DEEP] = {0};
double g_filter_data_l[8][FILTER_DEEP] = {0};

AD_ORG_DATA gt_AD_OrgData[3000]; // WYL 一次读的数据个数
unsigned int g_nTimeAxisRange = 1000000;
float g_nVAxisRange = 10000;

ULONG gl_ReadSizeWords;	// 读入的数据长度

BOOL gl_bDeviceADRun = FALSE;
USB2831_PARA_AD ADPara;
USB2831_PARA_AD OldADPara;

UINT ProcessDataThread(PVOID hWnd);  // 绘制数据线程
BOOL gl_bCreateFile = FALSE;
int  gl_ScreenVolume;     // 设置屏幕显示的量程
int  gl_voltVolume = 20000;     

VOLTRANGE  gl_InputRange[USB2831_MAX_AD_CHANNELS]; // 各通道设置的电压量程范围
int  gl_TriggerVolt;       // 触发电压
int gl_MiddleLsb[USB2831_MAX_AD_CHANNELS]; // 求平移电压时的中间值
int  gl_nChannelCount = 8;
float gl_PerLsbVolt[USB2831_MAX_AD_CHANNELS]; // 单位LSB的电压值
float gl_AnalyzeAllCount;
UINT gl_OverLimitCount;
int gl_ProcessMoveVolt; // 为1时, 平移电压
BOOL gl_bCreateDevice = FALSE;
BOOL gl_bProgress = FALSE;  // 是否更新进度条
int gl_nProcessMode;  // 数据处理方式 1：数字显示  2：波形显示  3：数据存盘
HANDLE gl_hEvent;  // 采集线程与绘制线程的同步信号
int gl_DigitShowMode; // 数字窗口显示模式
HANDLE gl_hFileObject;
BOOL gl_bCloseFile;
HANDLE gl_hExitEvent = NULL;
ULONGLONG gl_FileLenghtWords;
int	gl_BufferID = 0; // 当前的缓冲索引号
int gl_Offset=0;					// 当前缓冲段内偏移
int gl_nCurrentIndex = 0;			// 当前缓冲段
int gl_nDrawIndex = 0;				// 绘图段索引

BOOL	gl_bDataProcessing = FALSE;
CString g_strFileFullName;

LONGLONG gl_nTriggerPos = 0;		// 触发点位置

CString strMsg;
CString strErr;

BOOL gl_bSyncMode = TRUE;			// 同步采集标志
BOOL gl_FirstScreenStop = FALSE;
///////////////////////
int gl_WavePointX;					// 记录鼠标X位置

BOOL gl_bTileWave = TRUE;
int  gl_nProcMode = PROC_MODE_DIGIT;

BOOL gl_bCollected = FALSE;			// 是否已经进行过一次采集

ULONG gl_TrigCnt = 0;


int gl_signal_enable[USB2831_MAX_AD_CHANNELS] ={0};


int gl_last_end_id = 0;// 上一帧数据填充到的showdata的位置，用于下一帧的开始

// 数据回放
int m_channel_cnt;//选择的通道总数
int m_channel_enable[8]; // 通道选择
ULONG read_point_offset[10000] = {0}; // 读取文件时每个点的文件位置
