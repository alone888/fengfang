// 演示了开关量的输入输出功能

#include "stdafx.h"
#include "windows.h"
#include "conio.h"
#include "USB2831.h"

int main(int argc, char* argv[])
{
	BYTE bDISts[16], bDOSts[16];

	HANDLE hDevice;
	int DeviceLgcID = 0; 
	hDevice = USB2831_CreateDevice(DeviceLgcID);
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Create Device Error...\n");
		return 0;
	}

	bDOSts[0] = 0;
	bDOSts[1] = 1;
	bDOSts[2] = 0;
	bDOSts[3] = 1;
	bDOSts[4] = 0;
	bDOSts[5] = 1;
	bDOSts[6] = 0;
	bDOSts[7] = 1;
	bDOSts[8] = 0;
	bDOSts[9] = 1;
	bDOSts[10] = 0;
	bDOSts[11] = 1;
	bDOSts[12] = 0;
	bDOSts[13] = 1;
	bDOSts[14] = 0;
	bDOSts[15] = 1;
Repeat:

	printf("Press any key to set DO\n\n");
	_getch();
	if(!USB2831_SetDeviceDO(hDevice, bDOSts)) // 开关量输出
	{
		printf("SetDeviceDO Error...\n");
		return 0;
	}

	if (bDOSts[0])  printf("DO0 = On\n"); else printf("DO0 = Off\n");
	if (bDOSts[1])  printf("DO1 = On\n"); else printf("DO1 = Off\n");
	if (bDOSts[2])  printf("DO2 = On\n"); else printf("DO2 = Off\n");
	if (bDOSts[3])  printf("DO3 = On\n"); else printf("DO3 = Off\n");
	if (bDOSts[4])  printf("DO4 = On\n"); else printf("DO4 = Off\n");
	if (bDOSts[5])  printf("DO5 = On\n"); else printf("DO5 = Off\n");
	if (bDOSts[6])  printf("DO6 = On\n"); else printf("DO6 = Off\n");
	if (bDOSts[7])  printf("DO7 = On\n"); else printf("DO7 = Off\n");
	if (bDOSts[8])  printf("DO8 = On\n"); else printf("DO8 = Off\n");
	if (bDOSts[9])  printf("DO9 = On\n"); else printf("DO9 = Off\n");
	if (bDOSts[10])  printf("DO10 = On\n"); else printf("DO10 = Off\n");
	if (bDOSts[11])  printf("DO11 = On\n"); else printf("DO11 = Off\n");
	if (bDOSts[12])  printf("DO12 = On\n"); else printf("DO12 = Off\n");
	if (bDOSts[13])  printf("DO13 = On\n"); else printf("DO13 = Off\n");
	if (bDOSts[14])  printf("DO14 = On\n"); else printf("DO14 = Off\n");
	if (bDOSts[15])  printf("DO15 = On\n"); else printf("DO15 = Off\n");

	printf("\n");	
	printf("Press any key to get DI\n\n");
	_getch();
	if(!USB2831_GetDeviceDI(hDevice, bDISts)) // 开关量输入
	{
		printf("USB2831_GetDeviceDI...\n");
		return 0;
	}
	
	if (bDISts[0])  printf("DI0 = On\n"); else printf("DI0 = Off\n");
	if (bDISts[1])  printf("DI1 = On\n"); else printf("DI1 = Off\n");
	if (bDISts[2])  printf("DI2 = On\n"); else printf("DI2 = Off\n");
	if (bDISts[3])  printf("DI3 = On\n"); else printf("DI3 = Off\n");
	if (bDISts[4])  printf("DI4 = On\n"); else printf("DI4 = Off\n");
	if (bDISts[5])  printf("DI5 = On\n"); else printf("DI5 = Off\n");
	if (bDISts[6])  printf("DI6 = On\n"); else printf("DI6 = Off\n");
	if (bDISts[7])  printf("DI7 = On\n"); else printf("DI7 = Off\n");
	if (bDISts[8])  printf("DI8 = On\n"); else printf("DI8 = Off\n");
	if (bDISts[9])  printf("DI9 = On\n"); else printf("DI9 = Off\n");
	if (bDISts[10])  printf("DI10 = On\n"); else printf("DI10 = Off\n");
	if (bDISts[11])  printf("DI11 = On\n"); else printf("DI11 = Off\n");
	if (bDISts[12])  printf("DI12 = On\n"); else printf("DI12 = Off\n");
	if (bDISts[13])  printf("DI13 = On\n"); else printf("DI13 = Off\n");
	if (bDISts[14])  printf("DI14 = On\n"); else printf("DI14 = Off\n");
	if (bDISts[15])  printf("DI15 = On\n"); else printf("DI15 = Off\n");

	printf("\n");
	char Key;
	printf("Press any to continue, ESC to quit\n");
	Key = _getch();
	if(Key != 27) goto Repeat;

	USB2831_ReleaseDevice(hDevice);
	return 0;
}

