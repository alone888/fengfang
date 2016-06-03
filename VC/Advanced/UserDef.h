#include "USB2831.h"  // ������������ӿ�
#include "ADThread.h"
//#############################################################
// �������ڿ�������λ�������̵ĳ���
#define AD_LSB_COUNT  8192.0       // LSB������
#define AD_LSB_HALF   4096         // ��ȡƫ����ʱ����ԭ��λ��
#define AD_VOLT_RANGE 10000.0		// ���̣�mV��(ע������С��)
#define MASK_MSB (0x1FFF)			// 

#define MAX_CHANNEL_COUNT	16

#define DA_LSB_COUNT  4096.0       
#define DA_LSB_HALF   2048         

// ������������
#define DEVICE_NAME        "USB2831"   // �����豸��
#define MAX_FREQUENCY     250000    // ���Ƶ��Hz
#define MAX_SEGMENT_SIZE   8192
//#############################################################
// Header define
#define PCI_BUS     0x01
#define USB_BUS     0x02
#define ISA_BUS     0x03
#define PC104_BUS   0x04

const int MAX_RAM_SIZE = 1024 * 64; // ÿͨ��RAM���������(�����)

#define PRO_MODE	0x01 // �ǿղɼ�
#define HALF_MODE   0x02 // �����ɼ�
#define INT_MODE	0x04 // �жϲɼ�
#define DMA_MODE	0x03 // DMA�ɼ�

#define AD_FILE_TYPE 0x00  // �洢AD���ݵ��ļ�
#define DA_FILE_TYPE 0x01  // �洢DA���ݵ��ļ�
#define DIO_FILE_TYPE 0x02 // �洢DIO���ݵ��ļ�

// ����FIFO�ĳ���(��)
#define FIFO_IDT7202_LENGTH 1024
#define FIFO_IDT7203_LENGTH 2048
#define FIFO_IDT7204_LENGTH 4096
#define FIFO_IDT7205_LENGTH 8192
#define FIFO_IDT7206_LENGTH 16384
#define FIFO_IDT7207_LENGTH 32768

#define AD_FIFO_FULL_LENGTH FIFO_IDT7205_LENGTH	   // FIFOȫ������
#define AD_FIFO_HALF_LENGTH AD_FIFO_FULL_LENGTH/2  // FIFO��������

#define DEFAULT_FILE_TYPE AD_FILE_TYPE
#define DEFAULT_BUSTYPE  USB_BUS
#define DEFAULT_DEVICE_NUM 0x2831		// Ĭ�ϵ��豸���


#define DEFAULT_VOLTBOTTOM_RANGE -50000
#define DEFAULT_VOLTTOP_RANGE     5000

#define HEAD_END_FLAG  0xAA55AA55

typedef struct _FILE_HEADER
{
	LONG HeadSizeBytes;		// �ļ�ͷ��Ϣ����
	LONG FileType;  
	// ���豸�����ļ����еĳ�Ա
	LONG BusType;				// �豸��������(DEFAULT_BUS_TYPE)
	LONG DeviceNum;				// ���豸�ı��(DEFAULT_DEVICE_NUM)
	LONG HeadVersion;      		// ͷ��Ϣ�汾(D31-D16=Major  D15-D0=Minijor) = 1.0
	LONG VoltBottomRange;       // ��������(mV)
	LONG VoltTopRange;	        // ��������(mV)	
	USB2831_PARA_AD ADPara;     // ����Ӳ������
	LONGLONG nTriggerPos;       // ������λ��
	LONG BatCode;               // ͬ���ļ�ʶ����
	LONG HeadEndFlag;			// �ļ�����λ

} FILE_HEADER, *PFILE_HEADER;

// ��ѹ����
typedef struct _VOLTRANGE
{
	int nRangeIndex;	// ��������
	int nVoltRange;		// ��ѹ����
}VOLTRANGE, *PVOLTRANGE;

#define MsgBox AfxMessageBox

