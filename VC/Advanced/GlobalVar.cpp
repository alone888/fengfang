#include "StdAfx.h"

////////////////////////////////////////////////////////////////////////
CADStatusView* gl_pADStatusView = NULL;
CADParaCfgView* gl_pParaCfgView = NULL;
WORD ADBuffer[MAX_SEGMENT_COUNT][MAX_SEGMENT_SIZE]; // �������8192*64
WORD ADBufferForFilter[MAX_SEGMENT_SIZE]; // �������8192
WORD showData[8][10000];// WYL 10000�����ʾ��Ļ��Ҫ��ʾ�ĵ�
UINT g_filer[8][2] = {0}; // 8��ͨ���� 0--��ͨ 1--��ͨ�˲�
double g_filter_data_h[8][FILTER_DEEP] = {0};
double g_filter_data_l[8][FILTER_DEEP] = {0};

AD_ORG_DATA gt_AD_OrgData[3000]; // WYL һ�ζ������ݸ���
unsigned int g_nTimeAxisRange = 1000000;
float g_nVAxisRange = 10000;

ULONG gl_ReadSizeWords;	// ��������ݳ���

BOOL gl_bDeviceADRun = FALSE;
USB2831_PARA_AD ADPara;
USB2831_PARA_AD OldADPara;

UINT ProcessDataThread(PVOID hWnd);  // ���������߳�
BOOL gl_bCreateFile = FALSE;
int  gl_ScreenVolume;     // ������Ļ��ʾ������
int  gl_voltVolume = 20000;     

VOLTRANGE  gl_InputRange[USB2831_MAX_AD_CHANNELS]; // ��ͨ�����õĵ�ѹ���̷�Χ
int  gl_TriggerVolt;       // ������ѹ
int gl_MiddleLsb[USB2831_MAX_AD_CHANNELS]; // ��ƽ�Ƶ�ѹʱ���м�ֵ
int  gl_nChannelCount = 8;
float gl_PerLsbVolt[USB2831_MAX_AD_CHANNELS]; // ��λLSB�ĵ�ѹֵ
float gl_AnalyzeAllCount;
UINT gl_OverLimitCount;
int gl_ProcessMoveVolt; // Ϊ1ʱ, ƽ�Ƶ�ѹ
BOOL gl_bCreateDevice = FALSE;
BOOL gl_bProgress = FALSE;  // �Ƿ���½�����
int gl_nProcessMode;  // ���ݴ���ʽ 1��������ʾ  2��������ʾ  3�����ݴ���
HANDLE gl_hEvent;  // �ɼ��߳�������̵߳�ͬ���ź�
int gl_DigitShowMode; // ���ִ�����ʾģʽ
HANDLE gl_hFileObject;
BOOL gl_bCloseFile;
HANDLE gl_hExitEvent = NULL;
ULONGLONG gl_FileLenghtWords;
int	gl_BufferID = 0; // ��ǰ�Ļ���������
int gl_Offset=0;					// ��ǰ�������ƫ��
int gl_nCurrentIndex = 0;			// ��ǰ�����
int gl_nDrawIndex = 0;				// ��ͼ������

BOOL	gl_bDataProcessing = FALSE;
CString g_strFileFullName;

LONGLONG gl_nTriggerPos = 0;		// ������λ��

CString strMsg;
CString strErr;

BOOL gl_bSyncMode = TRUE;			// ͬ���ɼ���־
BOOL gl_FirstScreenStop = FALSE;
///////////////////////
int gl_WavePointX;					// ��¼���Xλ��

BOOL gl_bTileWave = TRUE;
int  gl_nProcMode = PROC_MODE_DIGIT;

BOOL gl_bCollected = FALSE;			// �Ƿ��Ѿ����й�һ�βɼ�

ULONG gl_TrigCnt = 0;


int gl_signal_enable[USB2831_MAX_AD_CHANNELS] ={0};


int gl_last_end_id = 0;// ��һ֡������䵽��showdata��λ�ã�������һ֡�Ŀ�ʼ

// ���ݻط�
int m_channel_cnt;//ѡ���ͨ������
int m_channel_enable[8]; // ͨ��ѡ��
ULONG read_point_offset[10000] = {0}; // ��ȡ�ļ�ʱÿ������ļ�λ��
