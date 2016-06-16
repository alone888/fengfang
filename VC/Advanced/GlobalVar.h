#include "StdAfx.h"

extern CSysApp theApp;

////////////////////////////////////////////////////////////////////////
extern CADStatusView* gl_pADStatusView;
extern CADParaCfgView* gl_pParaCfgView;
#define MAX_SEGMENT_COUNT 64
extern WORD ADBuffer[MAX_SEGMENT_COUNT][MAX_SEGMENT_SIZE]; // �������
extern WORD showData[8][10000];// WYL 10000�����ʾ��Ļ��Ҫ��ʾ�ĵ�

//����ʱ���ԭʼ����
typedef struct _AD_OrgData       /* rcl */
{
	WORD data[8];
	unsigned int time; //us
} AD_ORG_DATA;

extern AD_ORG_DATA gt_AD_OrgData[100]; // WYL һ�ζ������ݸ���
extern int g_nTimeAxisRange;


extern ULONG gl_ReadSizeWords;	// ��������ݳ���
extern int gl_CurrentIndex; // AD���ݴ����̵߳�ǰ������������
extern BOOL gl_bDeviceADRun;
extern USB2831_PARA_AD ADPara;
extern UINT ProcessDataThread(PVOID pThreadPara);  // ���������߳�
extern BOOL gl_bCreateFile;
extern int  gl_ProcessGraphicMode; // Ϊ1, ������ʾ
extern int  gl_ScreenVolume;	// ������Ļ��ʾ������
extern int  gl_voltVolume;	
extern VOLTRANGE  gl_InputRange[USB2831_MAX_AD_CHANNELS];	// ��ͨ�����õĵ�ѹ���̷�Χ
extern int  gl_TriggerVolt;       // ������ѹ
extern int	gl_MiddleLsb[USB2831_MAX_AD_CHANNELS]; // ��ƽ�Ƶ�ѹʱ���м�ֵ USB2831_MAX_AD_CHANNELS
extern int  gl_nChannelCount;
extern float gl_PerLsbVolt[USB2831_MAX_AD_CHANNELS]; // ��λLSB�ĵ�ѹֵ
extern float gl_AnalyzeAllCount;
extern UINT gl_OverLimitCount;
extern int gl_ProcessMoveVolt;	// Ϊ1ʱ, ƽ�Ƶ�ѹ
extern BOOL gl_bCreateDevice;
extern BOOL gl_bProgress;  // �Ƿ���½�����

extern int gl_nProcMode;  // ���ݴ���ʽ 1��������ʾ  2��������ʾ  3�����ݴ���
// �������ݴ���ʽ(gl_nProcModeʹ�õ�ѡ��)
#define PROC_MODE_DIGIT 1 // ������ʾ
#define PROC_MODE_WAVE 2 // ������ʾ
#define PROC_MODE_SAVE 3 // ���̴���

extern int gl_DigitShowMode; // ���ִ�����ʾģʽ
// ���ִ�����ʾģʽ(gl_DigitShowModeʹ�õ�ѡ��)
#define SHOW_MODE_DEC   0 // ʮ������ʾ
#define SHOW_MODE_HEX   1 // ʮ��������ʾ
#define SHOW_MODE_VOLT  2 // ��ѹֵ��ʾ

extern HANDLE gl_hEvent;  // �ɼ��߳�������̵߳�ͬ���ź�
extern HANDLE gl_hFileObject;
extern BOOL gl_bCloseFile;
extern HANDLE gl_hExitEvent;

extern ULONGLONG gl_FileLenghtWords;
extern int gl_BufferID; // ��ǰ�Ļ���������
extern int gl_Offset;     // ��ǰ�������ƫ��
extern int gl_nCurrentIndex;      // ��ǰ�����
extern int gl_nDrawIndex;         // ��ͼ������

extern BOOL gl_bDataProcessing;
extern CString g_strFileFullName;

extern LONGLONG gl_nTriggerPos;		// ������λ��


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
extern BOOL gl_bSyncMode;    // �ɼ�ģʽ��ͬ�����첽��
extern BOOL gl_FirstScreenStop;

extern ULONG gl_TrigCnt;

extern int gl_signal_enable[USB2831_MAX_AD_CHANNELS];
