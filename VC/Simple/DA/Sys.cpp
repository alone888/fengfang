#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"
#include "USB2831.h"

#define ESC 27

int SelectOutputRange(void);

int main(int argc, char* argv[])
{
	HANDLE hDevice;
	int DeviceLgcID;

	int OutputRange;
	int nDAChannel;
	BOOL bRetStatus; // 函数的返回值
	char Key;
	SHORT nDAData;
	float Voltage;

	DeviceLgcID = 0;
	hDevice = USB2831_CreateDevice(DeviceLgcID); // 创建设备对象
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		return 0;
	}

	OutputRange = SelectOutputRange();
Repeat:
	printf("Please Input nDAChannel[0~3]:");
	scanf_s("%d", &nDAChannel);
	printf("Please Input Voltage[mV]:");
	scanf_s("%f", &Voltage);
	
	// 将原码转换为电压值
	switch (OutputRange)
	{
	case USB2831_OUTPUT_0_P5000mV: // 0 - +5V
		nDAData = (WORD)(Voltage/(5000.00/4096));
		break;
	case USB2831_OUTPUT_0_P10000mV: // 0 - +10V
		nDAData = (WORD)(Voltage/(10000.00/4096));
		break;
	case USB2831_OUTPUT_0_P10800mV: // 0 - +10.8V
		nDAData = (WORD)(Voltage/(10800.00/4096));
		break;
	case USB2831_OUTPUT_N5000_P5000mV: // -5V - +5V
		nDAData = (WORD)(Voltage/(10000.00/4096) + 2048);
		break;
	case USB2831_OUTPUT_N10000_P10000mV: // -10V - +10V
		nDAData = (WORD)(Voltage/(20000.00/4096) + 2048);
		break;
	case USB2831_OUTPUT_N10800_P10800mV: // -10.8V - +10.8V
		nDAData = (WORD)(Voltage/(21600.00/4096) + 2048);
		break;
	default:
		break;
	}

	printf("nDAData = %d\n", nDAData);
	
	bRetStatus = USB2831_WriteDeviceDA(hDevice, OutputRange, nDAData, nDAChannel);
	if(!bRetStatus)
	{
		printf("WriteDeviceDA error...\n");
		goto ExitRead0;  
	}

	printf("Press ESC to quit, other key to continue...\n");
	Key = _getch();
	if (Key != ESC) goto Repeat;
ExitRead0:
	USB2831_ReleaseDevice(hDevice);   // 释放设备对象	
	return 0;
}

//////////////////////////////////////////////////////
// 获取用户选择的输入量程
int SelectOutputRange(void)
{
	LONG OutputRange;
Repeat:
	printf("\n");
	printf("0.  0V    ～ +5V\n");
	printf("1.  0V    ～ +10V\n");
	printf("2.  0V    ～ +10.8V\n");
	printf("3. -5V   ～ +5V\n");
	printf("4. -10V  ～ +10V\n");
	printf("5. -10.8V  ～ +10.8V\n");
	
	printf("Please Select Input Range[0-5]:");
	scanf_s("%d", &OutputRange);
	if (OutputRange<0 || OutputRange>5) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
	return OutputRange;
}


