// ��������ʾ����ν���AD�����Ĺ���

#include "stdafx.h"
#include "conio.h"
#include "USB2831.h"

char key;
int InputRange;
int SelectInputRange(void);

int main(int argc, char* argv[])
{
	HANDLE hDevice;  // �豸������
	int DeviceLgcID;   // �����豸ID��(�ɰ���JP1����)
	BOOL bReturn; // �����ķ���ֵ
	int nReadSizeWords;  // ÿ�ζ�ȡAD���ݸ���
	LONG nRetWords;      // ʵ�ʶ�ȡ�����ݸ���
	int nChannelCount = 0;  // ����ͨ����
	WORD ADBuffer[32768];  // ����AD ���ݵĻ�����
	WORD ADData;
	float Volt;     // ��ADԭʼ����ת��Ϊ��ѹֵ
	int nRemainder = 0;

	InputRange = SelectInputRange();
	USB2831_PARA_AD ADPara;        // ��ʼ��AD�Ĳ����ṹ
	ADPara.CheckStsMode		= USB2831_CHKSTSMODE_HALF;		// ��ѯFIFO�ķǿձ�־
	ADPara.ADMode			= USB2831_ADMODE_SEQUENCE;		//	ѡ�������ɼ�ģʽ
	ADPara.FirstChannel		= 0;   // ��ͨ��0
	ADPara.LastChannel		= 3;   // ĩͨ��3
	ADPara.Frequency		= 25000; // ����Ƶ����Ϊ25KHz
	ADPara.InputRange		= InputRange; // ����ѡ��
	ADPara.GroupInterval	= 100;	// �������Ϊ1000΢��
	ADPara.LoopsOfGroup		= 1;	// ����ѭ��������Ϊ1��
	ADPara.Gains			= USB2831_GAINS_1MULT;	// ʹ��1������
	ADPara.TriggerMode		= USB2831_TRIGMODE_SOFT; // ����ģʽѡ������ڴ���
	ADPara.TriggerSource	= USB2831_TRIGSOURCE_ATR; // ����Դѡ��ģ�ⴥ��ATR
	ADPara.TriggerType		= USB2831_TRIGTYPE_EDGE; // ��������ѡ����ش���
	ADPara.TriggerDir		= USB2831_TRIGDIR_NEGATIVE; // ��������ѡ��
	ADPara.TrigWindow		= 10; // ����������
	ADPara.GroundingMode	= USB2831_GNDMODE_SE; // ���˷�ʽ
	ADPara.ClockSource		= USB2831_CLOCKSRC_IN;
	ADPara.bClockOutput		= FALSE;

	nChannelCount = ADPara.LastChannel - ADPara.FirstChannel + 1; // ����ͨ����

	DeviceLgcID = 0;  // �豸ID��, ����ϵͳ��ֻ��һ��USB2831�豸����DeviceLgcID=0;
	hDevice = USB2831_CreateDevice(DeviceLgcID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE) { printf("Create Device Error\n"); return 0; }

	bReturn = USB2831_InitDeviceAD( hDevice, &ADPara ); // ��ʼ��AD
	if (!bReturn)
	{
		printf( "USB2831_InitDeviceAD Error\n" );
		_getch();
		goto ExitReleaseDevice;
	}

	nReadSizeWords = 2048;
	nRemainder = 2048 % nChannelCount;
	nReadSizeWords = 2048 - nRemainder;   // ��ȡ���ݵĴ�С(����RAM����64K)
	while (!_kbhit()) // kbhit()̽���û��Ƿ��а�������,���а������˳�
	{
		printf("��ȴ��������԰�������˳������벻Ҫֱ�ӹرմ���ǿ���˳�...\n");
		if(!USB2831_ReadDeviceAD(hDevice, ADBuffer, nReadSizeWords, &nRetWords)) // ��ȡADת������
		{
			printf("ReadDeviceAD Error...\n");
			_getch();
			goto ExitReleaseDeviceAD;
		}
		
		int nChannel = ADPara.FirstChannel;
		for (int Index=0; Index<64; Index++) // �ܹ���ʾ64�����AD����
		{
			ADData = ADBuffer[Index]&0x1FFF;
			switch(InputRange) // ��������ѡ�񣬽�ADԭ�밴��Ӧ��ʽ����ɵ�ѹֵ
			{
			case USB2831_INPUT_N10000_P10000mV: // ��10V
				Volt = (float)((20000.00/8192) * ADData - 10000.00); // ��AD����ת��Ϊ��ѹֵ
				break;
			case USB2831_INPUT_N5000_P5000mV: // ��5V
				Volt = (float)((10000.00/8192) * ADData - 5000.00); // ��AD����ת��Ϊ��ѹֵ
				break;
			case USB2831_INPUT_N2500_P2500mV: // ��2.5V
				Volt = (float)((5000.00/8192) * ADData - 2500.00); // ��AD����ת��Ϊ��ѹֵ
				break;
			case USB2831_INPUT_0_P10000mV: // 0��10V
				Volt = (float)((10000.00/8192) * ADData); // ��AD����ת��Ϊ��ѹֵ
				break;
			}
			printf( "[AI%d]=%8.2f\t", nChannel, Volt);
			nChannel++; // ͨ���ŵݼӣ�׼��������һ��ͨ��������
			if (nChannel>ADPara.LastChannel) // ������㵽ĩͨ�����ٻص���ͨ��
			{
				printf("\n"); // ����ʾ���λ���Ƶ���һ��
				nChannel = ADPara.FirstChannel;
			}
		} // ������ݻ�����ʾ
	} // ѭ���ɼ�

ExitReleaseDeviceAD:
	USB2831_ReleaseDeviceAD(hDevice); // �ͷ�AD��ֹͣAD����ת��

ExitReleaseDevice:
	USB2831_ReleaseDevice(hDevice);   // �ͷ��豸����	
	return 0;
}

//////////////////////////////////////////////////////
// ��ȡ�û�ѡ�����������
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. -10V  �� +10V\n");
	printf("1. -5V   �� +5V\n");
	printf("2. -2.5V   �� +2.5V\n");
	printf("3. 0V    �� +10V\n");
	printf("Please Select Input Range[0-3]:");
	scanf_s("%d", &InputRange);
	if(InputRange<0 || InputRange>3) goto Repeat; // �ж��û�ѡ��������Ƿ�Ϸ������Ϸ���������ѡ��
	return InputRange;
}