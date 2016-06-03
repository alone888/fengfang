#include "USB2831.h"  // 包含驱动程序接口
#include "ADThread.h"
//#############################################################
// 定义用于控制数据位数及量程的常数
#define AD_LSB_COUNT  8192.0       // LSB码数量
#define AD_LSB_HALF   4096         // 当取偏移码时，其原点位置
#define AD_VOLT_RANGE 10000.0		// 量程（mV）(注意必须带小数)
#define MASK_MSB (0x1FFF)			// 

#define MAX_CHANNEL_COUNT	16

#define DA_LSB_COUNT  4096.0       
#define DA_LSB_HALF   2048         

// 定义其他常量
#define DEVICE_NAME        "USB2831"   // 定义设备名
#define MAX_FREQUENCY     250000    // 最高频率Hz
#define MAX_SEGMENT_SIZE   8192
//#############################################################
// Header define
#define PCI_BUS     0x01
#define USB_BUS     0x02
#define ISA_BUS     0x03
#define PC104_BUS   0x04

const int MAX_RAM_SIZE = 1024 * 64; // 每通道RAM的最大容量(点或字)

#define PRO_MODE	0x01 // 非空采集
#define HALF_MODE   0x02 // 半满采集
#define INT_MODE	0x04 // 中断采集
#define DMA_MODE	0x03 // DMA采集

#define AD_FILE_TYPE 0x00  // 存储AD数据的文件
#define DA_FILE_TYPE 0x01  // 存储DA数据的文件
#define DIO_FILE_TYPE 0x02 // 存储DIO数据的文件

// 各种FIFO的长度(点)
#define FIFO_IDT7202_LENGTH 1024
#define FIFO_IDT7203_LENGTH 2048
#define FIFO_IDT7204_LENGTH 4096
#define FIFO_IDT7205_LENGTH 8192
#define FIFO_IDT7206_LENGTH 16384
#define FIFO_IDT7207_LENGTH 32768

#define AD_FIFO_FULL_LENGTH FIFO_IDT7205_LENGTH	   // FIFO全满长度
#define AD_FIFO_HALF_LENGTH AD_FIFO_FULL_LENGTH/2  // FIFO半满长度

#define DEFAULT_FILE_TYPE AD_FILE_TYPE
#define DEFAULT_BUSTYPE  USB_BUS
#define DEFAULT_DEVICE_NUM 0x2831		// 默认的设备编号


#define DEFAULT_VOLTBOTTOM_RANGE -50000
#define DEFAULT_VOLTTOP_RANGE     5000

#define HEAD_END_FLAG  0xAA55AA55

typedef struct _FILE_HEADER
{
	LONG HeadSizeBytes;		// 文件头信息长度
	LONG FileType;  
	// 该设备数据文件共有的成员
	LONG BusType;				// 设备总线类型(DEFAULT_BUS_TYPE)
	LONG DeviceNum;				// 该设备的编号(DEFAULT_DEVICE_NUM)
	LONG HeadVersion;      		// 头信息版本(D31-D16=Major  D15-D0=Minijor) = 1.0
	LONG VoltBottomRange;       // 量程下限(mV)
	LONG VoltTopRange;	        // 量程上限(mV)	
	USB2831_PARA_AD ADPara;     // 保存硬件参数
	LONGLONG nTriggerPos;       // 触发点位置
	LONG BatCode;               // 同批文件识别码
	LONG HeadEndFlag;			// 文件结束位

} FILE_HEADER, *PFILE_HEADER;

// 电压量程
typedef struct _VOLTRANGE
{
	int nRangeIndex;	// 量程索引
	int nVoltRange;		// 电压量程
}VOLTRANGE, *PVOLTRANGE;

#define MsgBox AfxMessageBox

