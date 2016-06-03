// 本程序演示了如何进行AD采样的过程

#include "stdafx.h"
#include "conio.h"
#include "USB2831.h"

char key;
int InputRange;
int SelectInputRange(void);

int main(int argc, char* argv[])
{
	HANDLE hDevice;  // 设备对象句柄
	int DeviceLgcID;   // 物理设备ID号(由板上JP1决定)
	BOOL bReturn; // 函数的返回值
	int nReadSizeWords;  // 每次读取AD数据个数
	LONG nRetWords;      // 实际读取的数据个数
	int nChannelCount = 0;  // 采样通道数
	WORD ADBuffer[32768];  // 接收AD 数据的缓冲区
	WORD ADData;
	float Volt;     // 将AD原始数据转换为电压值
	int nRemainder = 0;

	InputRange = SelectInputRange();
	USB2831_PARA_AD ADPara;        // 初始化AD的参数结构
	ADPara.CheckStsMode		= USB2831_CHKSTSMODE_HALF;		// 查询FIFO的非空标志
	ADPara.ADMode			= USB2831_ADMODE_SEQUENCE;		//	选择连续采集模式
	ADPara.FirstChannel		= 0;   // 首通道0
	ADPara.LastChannel		= 3;   // 末通道3
	ADPara.Frequency		= 25000; // 采样频率设为25KHz
	ADPara.InputRange		= InputRange; // 量程选择
	ADPara.GroupInterval	= 100;	// 组间间隔设为1000微秒
	ADPara.LoopsOfGroup		= 1;	// 组内循环次数设为1次
	ADPara.Gains			= USB2831_GAINS_1MULT;	// 使用1倍增益
	ADPara.TriggerMode		= USB2831_TRIGMODE_SOFT; // 触发模式选择软件内触发
	ADPara.TriggerSource	= USB2831_TRIGSOURCE_ATR; // 触发源选择模拟触发ATR
	ADPara.TriggerType		= USB2831_TRIGTYPE_EDGE; // 触发类型选择边沿触发
	ADPara.TriggerDir		= USB2831_TRIGDIR_NEGATIVE; // 触发方向选择
	ADPara.TrigWindow		= 10; // 触发灵敏度
	ADPara.GroundingMode	= USB2831_GNDMODE_SE; // 单端方式
	ADPara.ClockSource		= USB2831_CLOCKSRC_IN;
	ADPara.bClockOutput		= FALSE;

	nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1; // 采样通道数

	DeviceLgcID = 0;  // 设备ID号, 假设系统中只有一个USB2831设备，即DeviceLgcID=0;
	hDevice = USB2831_CreateDevice(DeviceLgcID); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE) { printf("Create Device Error\n"); return 0; }

	bReturn = USB2831_InitDeviceAD( hDevice, &ADPara ); // 初始化AD
	if (!bReturn)
	{
		printf( "USB2831_InitDeviceAD Error\n" );
		_getch();
		goto ExitReleaseDevice;
	}

	nReadSizeWords = 2048;
	nRemainder = 2048 % nChannelCount;
	nReadSizeWords = 2048 - nRemainder;   // 读取数据的大小(整个RAM长度64K)
	while (!_kbhit()) // kbhit()探测用户是否有按键动作,若有按键则退出
	{
		printf("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...\n");
		if(!USB2831_ReadDeviceAD(hDevice, ADBuffer, nReadSizeWords, &nRetWords)) // 读取AD转换数据
		{
			printf("ReadDeviceAD Error...\n");
			_getch();
			goto ExitReleaseDeviceAD;
		}
		
		int nChannel = ADPara.FirstChannel;
		for (int Index=0; Index<64; Index++) // 总共显示64个点的AD数据
		{
			ADData = ADBuffer[Index]&0x1FFF;
			switch(InputRange) // 根据量程选择，将AD原码按相应公式换算成电压值
			{
			case USB2831_INPUT_N10000_P10000mV: // ±10V
				Volt = (float)((20000.00/8192) * ADData - 10000.00); // 将AD数据转换为电压值
				break;
			case USB2831_INPUT_N5000_P5000mV: // ±5V
				Volt = (float)((10000.00/8192) * ADData - 5000.00); // 将AD数据转换为电压值
				break;
			case USB2831_INPUT_N2500_P2500mV: // ±2.5V
				Volt = (float)((5000.00/8192) * ADData - 2500.00); // 将AD数据转换为电压值
				break;
			case USB2831_INPUT_0_P10000mV: // 0～10V
				Volt = (float)((10000.00/8192) * ADData); // 将AD数据转换为电压值
				break;
			}
			printf( "[AI%d]=%8.2f\t", nChannel, Volt);
			nChannel++; // 通道号递加，准备换算下一个通道的数据
			if (nChannel>ADPara.LastChannel) // 如果换算到末通道，再回到首通道
			{
				printf("\n"); // 将显示光标位置移到下一项
				nChannel = ADPara.FirstChannel;
			}
		} // 多点数据换算显示
	} // 循环采集

ExitReleaseDeviceAD:
	USB2831_ReleaseDeviceAD(hDevice); // 释放AD，停止AD数据转换

ExitReleaseDevice:
	USB2831_ReleaseDevice(hDevice);   // 释放设备对象	
	return 0;
}

//////////////////////////////////////////////////////
// 获取用户选择的输入量程
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. -10V  ～ +10V\n");
	printf("1. -5V   ～ +5V\n");
	printf("2. -2.5V   ～ +2.5V\n");
	printf("3. 0V    ～ +10V\n");
	printf("Please Select Input Range[0-3]:");
	scanf_s("%d", &InputRange);
	if(InputRange<0 || InputRange>3) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
	return InputRange;
}