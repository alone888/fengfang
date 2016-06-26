#ifndef _USB2831_DEVICE_
#define _USB2831_DEVICE_

#include<windows.h>
//***********************************************************
// ����AD�ɼ��Ĳ����ṹ
typedef struct _USB2831_PARA_AD
{
	LONG CheckStsMode;		// ���״̬ģʽ
	LONG ADMode;            // ADģʽѡ��(�����ɼ�/����ɼ�)
	LONG FirstChannel;      // ��ͨ��,ȡֵ��ΧΪ[0, 15]
	LONG LastChannel;		// ĩͨ��,ȡֵ��ΧΪ[0, 15]
	LONG Frequency;         // �ɼ�Ƶ��,��λΪHz,ȡֵ��ΧΪ[31, 250000]
	LONG InputRange;		// ����ѡ��
	LONG GroupInterval;     // �������ʱ�������(��λ��΢��),ȡֵ��ΧΪ[1, 32767]
	LONG LoopsOfGroup;		// �������ʱ��ÿ��ѭ��������ȡֵ��ΧΪ[1, 255]
	LONG Gains;				// ���������
	LONG TriggerMode;       // ����ģʽѡ��(����������󴥷�)
	LONG TriggerSource;		// ����Դѡ��(ART��DTR)
	LONG TriggerType;		// ��������ѡ��(���ش���/���崥��)
	LONG TriggerDir;		// ��������ѡ��(����/���򴥷�)
	LONG TrigWindow;		// ����������(1-255)����λ:0.5΢��
	LONG GroundingMode;		// �ӵط�ʽ�����˻�˫��ѡ��
	LONG ClockSource;		// ʱ��Դѡ��(��/��ʱ��Դ)
	LONG bClockOutput;      // �Ƿ�������ADת��ʱ�������=TRUE:���������CN1�ϵ�CLKOUT��=FALSE:��ֹ�����CN1�ϵ�CLKOUT
} USB2831_PARA_AD, *PUSB2831_PARA_AD;

// ADӲ������USB2831_PARA_AD�е�CheckStsMode���״̬��ģʽ��ʹ�õ�ѡ��
const long USB2831_CHKSTSMODE_HALF		= 0x00; // ��ѯFIFO������־(�����Ƶ�ʲɼ�ʱʹ��)
const long USB2831_CHKSTSMODE_NPT		= 0x01; // ��ѯFIFO�ǿձ�־(�����ʵʱ�ɼ�ʱʹ��)

// ADӲ������USB2831_PARA_AD�е�ADMode����ģʽ��ʹ�õ�ѡ��
const long USB2831_ADMODE_SEQUENCE		= 0x00; // ��������
const long USB2831_ADMODE_GROUP			= 0x01; // �������

//***********************************************************
// ADӲ������USB2831_PARA_AD�е�InputRange������ʹ�õ�ѡ��
const long USB2831_INPUT_N10000_P10000mV= 0x00; // ��10000mV
const long USB2831_INPUT_N5000_P5000mV	= 0x01; // ��5000mV
const long USB2831_INPUT_N2500_P2500mV	= 0x02; // ��2500mV
const long USB2831_INPUT_0_P10000mV		= 0x03; // 0��10000mV

//***********************************************************
// AD����USB2831_PARA_AD�е�Gainsʹ�õ�Ӳ������ѡ��
const long USB2831_GAINS_1MULT			= 0x00; // 1������(ʹ��AD8251�Ŵ���)
const long USB2831_GAINS_2MULT			= 0x01; // 2������(ʹ��AD8251�Ŵ���)
const long USB2831_GAINS_4MULT			= 0x02; // 4������(ʹ��AD8251�Ŵ���)
const long USB2831_GAINS_8MULT			= 0x03; // 8������(ʹ��AD8251�Ŵ���)

//***********************************************************
// ADӲ������USB2831_PARA_AD�е�TriggerMode��Ա������ʹ��AD����ģʽѡ��
const long USB2831_TRIGMODE_SOFT		= 0x00; // �������(�����ڴ���)
const long USB2831_TRIGMODE_POST		= 0x01; // Ӳ���󴥷�(�����ⴥ��)

//***********************************************************
// ADӲ������USB2831_PARA_AD�е�TriggerSource��Ա������ʹ��AD����Դѡ��
const long USB2831_TRIGSOURCE_ATR		= 0x00; // ATR����
const long USB2831_TRIGSOURCE_DTR		= 0x01; // DTR����

// ADӲ������USB2831_PARA_AD�е�TriggerType����������ʹ�õ�ѡ��
const long USB2831_TRIGTYPE_EDGE		= 0x00; // ���ش���
const long USB2831_TRIGTYPE_PULSE		= 0x01; // ���崥��(��ƽ����)

//***********************************************************
// ADӲ������USB2831_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
const long USB2831_TRIGDIR_NEGATIVE		= 0x00; // ���򴥷�(�͵�ƽ/�½��ش���)
const long USB2831_TRIGDIR_POSITIVE		= 0x01; // ���򴥷�(�ߵ�ƽ/�����ش���)
const long USB2831_TRIGDIR_POSIT_NEGAT	= 0x02; // �����򴥷�(��/�͵�ƽ������/�½��ش���)

//***********************************************************
// AD����(USB2831_PARA_AD)�е�GroundingModeʹ�õ�ģ���źŽӵط�ʽѡ��
const long USB2831_GNDMODE_SE			= 0x00;	// ���˷�ʽ(SE:Single end)
const long USB2831_GNDMODE_DI			= 0x01;	// ˫�˷�ʽ(DI:Differential)

//***********************************************************
// ADӲ������USB2831_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
const long USB2831_CLOCKSRC_IN			= 0x00; // �ڲ�ʱ�Ӷ�ʱ����
const long USB2831_CLOCKSRC_OUT			= 0x01; // �ⲿʱ�Ӷ�ʱ����(ʹ��CN1�ϵ�CLKIN�ź�����)

//***********************************************************
// DA�������USB2831_WriteDeviceDA��ģ���������Χ����OutputRange��ʹ�õ�ѡ��
const long USB2831_OUTPUT_0_P5000mV		= 0x00;		// 0��5000mV
const long USB2831_OUTPUT_0_P10000mV	= 0x01;		// 0��10000mV
const long USB2831_OUTPUT_0_P10800mV	= 0x02;		// 0��10800mV
const long USB2831_OUTPUT_N5000_P5000mV	= 0x03;		// ��5000mV
const long USB2831_OUTPUT_N10000_P10000mV= 0x04;	// ��10000mV
const long USB2831_OUTPUT_N10800_P10800mV= 0x05;	// ��10800mV

//***********************************************************
// CreateFileObject�е�Mode����ʹ�õ��ļ�������ʽ������(��ͨ����ָ��ʵ�ֶ��ַ�ʽ������)
const long	USB2831_modeRead			= 0x0000;   // ֻ���ļ���ʽ
const long  USB2831_modeWrite			= 0x0001;   // ֻд�ļ���ʽ
const long 	USB2831_modeReadWrite		= 0x0002;   // �ȶ���д�ļ���ʽ
const long  USB2831_modeCreate			= 0x1000;   // ����ļ�������Դ������ļ���������ڣ����ؽ����ļ�������0

//***********************************************************
// ���������ӿ�
#ifndef _USB2831_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## ����ͨ�ú��� #################################
    HANDLE DEVAPI FAR PASCAL USB2831_CreateDevice(int DeviceLgcID = 0); // �����豸����(�ú���ʹ��ϵͳ���߼��豸ID��
	int DEVAPI FAR PASCAL USB2831_GetDeviceCount(HANDLE hDevice);      // ȡ��USB2831��ϵͳ�е��豸����
	BOOL DEVAPI FAR PASCAL USB2831_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID); // ȡ�õ�ǰ�豸���߼�ID�ź�����ID��
	BOOL DEVAPI FAR PASCAL USB2831_ListDeviceDlg(void); // �öԻ����б�ϵͳ���е�����USB2831�豸
	BOOL DEVAPI FAR PASCAL USB2831_ResetDevice(HANDLE hDevice);		 // ��λ����USB�豸
    BOOL DEVAPI FAR PASCAL USB2831_ReleaseDevice(HANDLE hDevice);    // �豸���

	//####################### AD���ݶ�ȡ���� #################################
    BOOL DEVAPI FAR PASCAL USB2831_InitDeviceAD(				// ��ʼ���豸,������TRUE��,�豸���̿�ʼ����.
									HANDLE hDevice,				// �豸���,��Ӧ��CreateDevice��������
									PUSB2831_PARA_AD pADPara);  // Ӳ������, �����ڴ˺����о���Ӳ��״̬							

    BOOL DEVAPI FAR PASCAL USB2831_ReadDeviceAD(				// ��ʼ���豸�󣬼����ô˺�����ȡ�豸�ϵ�AD����
									HANDLE hDevice,				// �豸���,��Ӧ��CreateDevice��������
									USHORT ADBuffer[],			// �����ڽ������ݵ��û�������
									LONG nReadSizeWords,		// ��ȡAD���ݵĳ���(��)  
									PLONG nRetSizeWords = NULL);// ʵ�ʷ������ݵĳ���(��)

    BOOL DEVAPI FAR PASCAL USB2831_ReleaseDeviceAD( HANDLE hDevice); // ֹͣAD�ɼ����ͷ�AD������ռ��Դ

   	//################# AD��Ӳ�������������� ########################	
	BOOL DEVAPI FAR PASCAL USB2831_SaveParaAD(HANDLE hDevice, PUSB2831_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL USB2831_LoadParaAD(HANDLE hDevice, PUSB2831_PARA_AD pADPara);
    BOOL DEVAPI FAR PASCAL USB2831_ResetParaAD(HANDLE hDevice, PUSB2831_PARA_AD pADPara); // ��AD���������ָ�������Ĭ��ֵ

	//####################### DA����������� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������һ������������
	// ��ʵ�ָ��١�������DA�������
	BOOL DEVAPI FAR PASCAL USB2831_WriteDeviceDA(			// дDA����
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									LONG OutputRange,		// ������̣����嶨����ο�����ĳ������岿��
									SHORT nDAData,			// �����DAԭʼ����[0, 4095]
									int nDAChannel);		// DA���ͨ��[0-3](д��4�������ĸ�ͨ��������)

	//####################### ����I/O����������� #################################
	BOOL DEVAPI FAR PASCAL USB2831_GetDeviceDI(					// ȡ�ÿ�����״̬     
									HANDLE hDevice,				// �豸���,��Ӧ��CreateDevice��������								        
									BYTE bDISts[16]);			// ��������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)

    BOOL DEVAPI FAR PASCAL USB2831_SetDeviceDO(					// ���������״̬
									HANDLE hDevice,				// �豸���,��Ӧ��CreateDevice��������								        
									BYTE bDOSts[16]);			// �������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)

	//############################################################################
	BOOL DEVAPI FAR PASCAL USB2831_GetDevVersion(				// ��ȡ�豸�̼�������汾
									HANDLE hDevice,				// �豸������,����CreateDevice��������
									PULONG pulFmwVersion,		// �̼��汾
									PULONG pulDriverVersion);	// �����汾

   	//########################## �ļ��������� ####################################
    HANDLE DEVAPI FAR PASCAL USB2831_CreateFileObject(			// �����ļ�����
									HANDLE hDevice,				// �豸���,��Ӧ��CreateDevice��������
									LPCTSTR strFileName,		// ·�����ļ���
									int Mode);					// �ļ�������ʽ

    BOOL DEVAPI FAR PASCAL USB2831_WriteFile(					// �����û��ռ������ݵ������ļ�
									HANDLE hFileObject,			// �ļ����,��Ӧ��CreateFileObject��������
									PVOID pDataBuffer,			// �û����ݿռ��ַ
									LONG nWriteSizeBytes);		// ��������С(�ֽ�)

    BOOL DEVAPI FAR PASCAL USB2831_ReadFile(					// �Ӵ����ļ��ж�ȡ���ݵ��û��ռ�
									HANDLE hFileObject,			// �ļ����,��Ӧ��CreateFileObject��������
									PVOID pDataBuffer,			// �����ļ����ݵ��û��ڴ滺����
									LONG OffsetBytes,			// ���ļ�ǰ�˿�ʼ��ƫ��λ��
									LONG nReadSizeBytes);		// ��ƫ��λ�ÿ�ʼ�����ֽ���

	BOOL DEVAPI FAR PASCAL USB2831_SetFileOffset(				// �����ļ�ƫ��ָ��
									HANDLE hFileObject,			// �ļ����,��Ӧ��CreateFileObject��������
									LONG nOffsetBytes);			// �ļ�ƫ��λ�ã�����Ϊ��λ��  

	ULONGLONG DEVAPI FAR PASCAL USB2831_GetFileLength(HANDLE hFileObject); // ȡ��ָ���ļ����ȣ��ֽڣ�

    BOOL DEVAPI FAR PASCAL USB2831_ReleaseFile(HANDLE hFileObject);

	//############################ �̲߳������� ################################
	DEVAPI HANDLE FAR PASCAL USB2831_CreateSystemEvent(void); 	// �����ں�ϵͳ�¼�����
	BOOL DEVAPI FAR PASCAL USB2831_ReleaseSystemEvent(HANDLE hEvent); // �ͷ��ں��¼�����
	BOOL DEVAPI FAR PASCAL USB2831_CreateVBThread(HANDLE* hThread, LPTHREAD_START_ROUTINE RoutineAddr);
	BOOL DEVAPI FAR PASCAL USB2831_TerminateVBThread(HANDLE hThread);

	//################# �������Ӻ��� ########################
	BOOL DEVAPI FAR PASCAL USB2831_kbhit(void); // ̽���û��Ƿ��л�������(�ڿ���̨Ӧ�ó���Console�����ڷ�VC������)
	char DEVAPI FAR PASCAL USB2831_getch(void); // �ȴ�����ȡ�û�����ֵ(�ڿ���̨Ӧ�ó���Console����Ч)

#ifdef __cplusplus
}
#endif

//#################### �������� #####################

const long USB2831_MAX_AD_CHANNELS = 16;

#ifndef _USB2831_FIFO_LENGTH_
#define _USB2831_FIFO_LENGTH_
// ��������֧�ֵĸ���FIFO�洢���ĳ���(��)
const long FIFO_IDT7202_LENGTH				= 1024;
const long FIFO_IDT7203_LENGTH				= 2048;
const long FIFO_IDT7204_LENGTH				= 4096;
const long FIFO_IDT7205_LENGTH				= 8192;
const long FIFO_IDT7206_LENGTH				= 16384;
const long FIFO_IDT7207_LENGTH				= 32768;
#endif; // _USB2831_FIFO_LENGTH_

// �Զ������������������
#ifndef _USB2831_DRIVER_
#ifndef _WIN64
#pragma comment(lib, "USB2831_32.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with USB2831_32.dll...")
#pragma message("======== Successfully linked with USB2831_32.dll")
#else
#pragma comment(lib, "USB2831_64.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with USB2831_64.dll...")
#pragma message("======== Successfully linked with USB2831_64.dll")
#endif
#endif

#endif; // _USB2831_DEVICE_