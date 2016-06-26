#ifndef _USB2831_DEVICE_
#define _USB2831_DEVICE_

#include<windows.h>
//***********************************************************
// 用于AD采集的参数结构
typedef struct _USB2831_PARA_AD
{
	LONG CheckStsMode;		// 检查状态模式
	LONG ADMode;            // AD模式选择(连续采集/分组采集)
	LONG FirstChannel;      // 首通道,取值范围为[0, 15]
	LONG LastChannel;		// 末通道,取值范围为[0, 15]
	LONG Frequency;         // 采集频率,单位为Hz,取值范围为[31, 250000]
	LONG InputRange;		// 量程选择
	LONG GroupInterval;     // 分组采样时的组间间隔(单位：微秒),取值范围为[1, 32767]
	LONG LoopsOfGroup;		// 分组采样时，每组循环次数，取值范围为[1, 255]
	LONG Gains;				// 增益控制字
	LONG TriggerMode;       // 触发模式选择(软件触发、后触发)
	LONG TriggerSource;		// 触发源选择(ART、DTR)
	LONG TriggerType;		// 触发类型选择(边沿触发/脉冲触发)
	LONG TriggerDir;		// 触发方向选择(正向/负向触发)
	LONG TrigWindow;		// 触发灵敏度(1-255)，单位:0.5微秒
	LONG GroundingMode;		// 接地方式（单端或双端选择）
	LONG ClockSource;		// 时钟源选择(内/外时钟源)
	LONG bClockOutput;      // 是否允许本地AD转换时钟输出，=TRUE:允许输出到CN1上的CLKOUT，=FALSE:禁止输出到CN1上的CLKOUT
} USB2831_PARA_AD, *PUSB2831_PARA_AD;

// AD硬件参数USB2831_PARA_AD中的CheckStsMode检查状态的模式所使用的选项
const long USB2831_CHKSTSMODE_HALF		= 0x00; // 查询FIFO半满标志(建议高频率采集时使用)
const long USB2831_CHKSTSMODE_NPT		= 0x01; // 查询FIFO非空标志(建议高实时采集时使用)

// AD硬件参数USB2831_PARA_AD中的ADMode工作模式所使用的选项
const long USB2831_ADMODE_SEQUENCE		= 0x00; // 连续采样
const long USB2831_ADMODE_GROUP			= 0x01; // 分组采样

//***********************************************************
// AD硬件参数USB2831_PARA_AD中的InputRange量程所使用的选项
const long USB2831_INPUT_N10000_P10000mV= 0x00; // ±10000mV
const long USB2831_INPUT_N5000_P5000mV	= 0x01; // ±5000mV
const long USB2831_INPUT_N2500_P2500mV	= 0x02; // ±2500mV
const long USB2831_INPUT_0_P10000mV		= 0x03; // 0～10000mV

//***********************************************************
// AD参数USB2831_PARA_AD中的Gains使用的硬件增益选项
const long USB2831_GAINS_1MULT			= 0x00; // 1倍增益(使用AD8251放大器)
const long USB2831_GAINS_2MULT			= 0x01; // 2倍增益(使用AD8251放大器)
const long USB2831_GAINS_4MULT			= 0x02; // 4倍增益(使用AD8251放大器)
const long USB2831_GAINS_8MULT			= 0x03; // 8倍增益(使用AD8251放大器)

//***********************************************************
// AD硬件参数USB2831_PARA_AD中的TriggerMode成员变量所使用AD触发模式选项
const long USB2831_TRIGMODE_SOFT		= 0x00; // 软件触发(属于内触发)
const long USB2831_TRIGMODE_POST		= 0x01; // 硬件后触发(属于外触发)

//***********************************************************
// AD硬件参数USB2831_PARA_AD中的TriggerSource成员变量所使用AD触发源选项
const long USB2831_TRIGSOURCE_ATR		= 0x00; // ATR触发
const long USB2831_TRIGSOURCE_DTR		= 0x01; // DTR触发

// AD硬件参数USB2831_PARA_AD中的TriggerType触发类型所使用的选项
const long USB2831_TRIGTYPE_EDGE		= 0x00; // 边沿触发
const long USB2831_TRIGTYPE_PULSE		= 0x01; // 脉冲触发(电平类型)

//***********************************************************
// AD硬件参数USB2831_PARA_AD中的TriggerDir触发方向所使用的选项
const long USB2831_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低电平/下降沿触发)
const long USB2831_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高电平/上升沿触发)
const long USB2831_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低电平或上升/下降沿触发)

//***********************************************************
// AD参数(USB2831_PARA_AD)中的GroundingMode使用的模拟信号接地方式选项
const long USB2831_GNDMODE_SE			= 0x00;	// 单端方式(SE:Single end)
const long USB2831_GNDMODE_DI			= 0x01;	// 双端方式(DI:Differential)

//***********************************************************
// AD硬件参数USB2831_PARA_AD中的ClockSource时钟源所使用的选项
const long USB2831_CLOCKSRC_IN			= 0x00; // 内部时钟定时触发
const long USB2831_CLOCKSRC_OUT			= 0x01; // 外部时钟定时触发(使用CN1上的CLKIN信号输入)

//***********************************************************
// DA输出函数USB2831_WriteDeviceDA的模拟量输出范围参数OutputRange所使用的选项
const long USB2831_OUTPUT_0_P5000mV		= 0x00;		// 0～5000mV
const long USB2831_OUTPUT_0_P10000mV	= 0x01;		// 0～10000mV
const long USB2831_OUTPUT_0_P10800mV	= 0x02;		// 0～10800mV
const long USB2831_OUTPUT_N5000_P5000mV	= 0x03;		// ±5000mV
const long USB2831_OUTPUT_N10000_P10000mV= 0x04;	// ±10000mV
const long USB2831_OUTPUT_N10800_P10800mV= 0x05;	// ±10800mV

//***********************************************************
// CreateFileObject中的Mode参数使用的文件操作方式控制字(可通过或指令实现多种方式并操作)
const long	USB2831_modeRead			= 0x0000;   // 只读文件方式
const long  USB2831_modeWrite			= 0x0001;   // 只写文件方式
const long 	USB2831_modeReadWrite		= 0x0002;   // 既读又写文件方式
const long  USB2831_modeCreate			= 0x1000;   // 如果文件不存可以创建该文件，如果存在，则重建此文件，并清0

//***********************************************************
// 驱动函数接口
#ifndef _USB2831_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 常规通用函数 #################################
    HANDLE DEVAPI FAR PASCAL USB2831_CreateDevice(int DeviceLgcID = 0); // 创建设备对象(该函数使用系统内逻辑设备ID）
	int DEVAPI FAR PASCAL USB2831_GetDeviceCount(HANDLE hDevice);      // 取得USB2831在系统中的设备数量
	BOOL DEVAPI FAR PASCAL USB2831_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID); // 取得当前设备的逻辑ID号和物理ID号
	BOOL DEVAPI FAR PASCAL USB2831_ListDeviceDlg(void); // 用对话框列表系统当中的所有USB2831设备
	BOOL DEVAPI FAR PASCAL USB2831_ResetDevice(HANDLE hDevice);		 // 复位整个USB设备
    BOOL DEVAPI FAR PASCAL USB2831_ReleaseDevice(HANDLE hDevice);    // 设备句柄

	//####################### AD数据读取函数 #################################
    BOOL DEVAPI FAR PASCAL USB2831_InitDeviceAD(				// 初始化设备,当返回TRUE后,设备即刻开始传输.
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建
									PUSB2831_PARA_AD pADPara);  // 硬件参数, 它仅在此函数中决定硬件状态							

    BOOL DEVAPI FAR PASCAL USB2831_ReadDeviceAD(				// 初始化设备后，即可用此函数读取设备上的AD数据
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建
									USHORT ADBuffer[],			// 将用于接受数据的用户缓冲区
									LONG nReadSizeWords,		// 读取AD数据的长度(字)  
									PLONG nRetSizeWords = NULL);// 实际返回数据的长度(字)

    BOOL DEVAPI FAR PASCAL USB2831_ReleaseDeviceAD( HANDLE hDevice); // 停止AD采集，释放AD对象所占资源

   	//################# AD的硬件参数操作函数 ########################	
	BOOL DEVAPI FAR PASCAL USB2831_SaveParaAD(HANDLE hDevice, PUSB2831_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL USB2831_LoadParaAD(HANDLE hDevice, PUSB2831_PARA_AD pADPara);
    BOOL DEVAPI FAR PASCAL USB2831_ResetParaAD(HANDLE hDevice, PUSB2831_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	//####################### DA数据输出函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面一个函数便能轻
	// 松实现高速、连续的DA数据输出
	BOOL DEVAPI FAR PASCAL USB2831_WriteDeviceDA(			// 写DA数据
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									LONG OutputRange,		// 输出量程，具体定义请参考上面的常量定义部分
									SHORT nDAData,			// 输出的DA原始数据[0, 4095]
									int nDAChannel);		// DA输出通道[0-3](写入4，代表四个通道都启动)

	//####################### 数字I/O输入输出函数 #################################
	BOOL DEVAPI FAR PASCAL USB2831_GetDeviceDI(					// 取得开关量状态     
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
									BYTE bDISts[16]);			// 开关输入状态(注意: 必须定义为8个字节元素的数组)

    BOOL DEVAPI FAR PASCAL USB2831_SetDeviceDO(					// 输出开关量状态
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
									BYTE bDOSts[16]);			// 开关输出状态(注意: 必须定义为8个字节元素的数组)

	//############################################################################
	BOOL DEVAPI FAR PASCAL USB2831_GetDevVersion(				// 获取设备固件及程序版本
									HANDLE hDevice,				// 设备对象句柄,它由CreateDevice函数创建
									PULONG pulFmwVersion,		// 固件版本
									PULONG pulDriverVersion);	// 驱动版本

   	//########################## 文件操作函数 ####################################
    HANDLE DEVAPI FAR PASCAL USB2831_CreateFileObject(			// 创建文件对象
									HANDLE hDevice,				// 设备句柄,它应由CreateDevice函数创建
									LPCTSTR strFileName,		// 路径及文件名
									int Mode);					// 文件操作方式

    BOOL DEVAPI FAR PASCAL USB2831_WriteFile(					// 保存用户空间中数据到磁盘文件
									HANDLE hFileObject,			// 文件句柄,它应由CreateFileObject函数创建
									PVOID pDataBuffer,			// 用户数据空间地址
									LONG nWriteSizeBytes);		// 缓冲区大小(字节)

    BOOL DEVAPI FAR PASCAL USB2831_ReadFile(					// 从磁盘文件中读取数据到用户空间
									HANDLE hFileObject,			// 文件句柄,它应由CreateFileObject函数创建
									PVOID pDataBuffer,			// 接受文件数据的用户内存缓冲区
									LONG OffsetBytes,			// 从文件前端开始的偏移位置
									LONG nReadSizeBytes);		// 从偏移位置开始读的字节数

	BOOL DEVAPI FAR PASCAL USB2831_SetFileOffset(				// 设置文件偏移指针
									HANDLE hFileObject,			// 文件句柄,它应由CreateFileObject函数创建
									LONG nOffsetBytes);			// 文件偏移位置（以字为单位）  

	ULONGLONG DEVAPI FAR PASCAL USB2831_GetFileLength(HANDLE hFileObject); // 取得指定文件长度（字节）

    BOOL DEVAPI FAR PASCAL USB2831_ReleaseFile(HANDLE hFileObject);

	//############################ 线程操作函数 ################################
	DEVAPI HANDLE FAR PASCAL USB2831_CreateSystemEvent(void); 	// 创建内核系统事件对象
	BOOL DEVAPI FAR PASCAL USB2831_ReleaseSystemEvent(HANDLE hEvent); // 释放内核事件对象
	BOOL DEVAPI FAR PASCAL USB2831_CreateVBThread(HANDLE* hThread, LPTHREAD_START_ROUTINE RoutineAddr);
	BOOL DEVAPI FAR PASCAL USB2831_TerminateVBThread(HANDLE hThread);

	//################# 其他附加函数 ########################
	BOOL DEVAPI FAR PASCAL USB2831_kbhit(void); // 探测用户是否有击键动作(在控制台应用程序Console中且在非VC语言中)
	char DEVAPI FAR PASCAL USB2831_getch(void); // 等待并获取用户击键值(在控制台应用程序Console中有效)

#ifdef __cplusplus
}
#endif

//#################### 辅助常量 #####################

const long USB2831_MAX_AD_CHANNELS = 16;

#ifndef _USB2831_FIFO_LENGTH_
#define _USB2831_FIFO_LENGTH_
// 本卡可以支持的各种FIFO存储器的长度(点)
const long FIFO_IDT7202_LENGTH				= 1024;
const long FIFO_IDT7203_LENGTH				= 2048;
const long FIFO_IDT7204_LENGTH				= 4096;
const long FIFO_IDT7205_LENGTH				= 8192;
const long FIFO_IDT7206_LENGTH				= 16384;
const long FIFO_IDT7207_LENGTH				= 32768;
#endif; // _USB2831_FIFO_LENGTH_

// 自动包含驱动函数导入库
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