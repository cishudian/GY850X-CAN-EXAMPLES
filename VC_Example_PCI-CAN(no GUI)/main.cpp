#include "ControlCAN.h"
#pragma   comment(lib,"VCI_CAN.lib")
int main()
{
	UINT m_nDeviceType=DEV_PCICAN;
	UINT m_nDeviceIndex=0;//设备索引号，如果只有1个USB-CAN，则填0
	DWORD Reserved=0;
	DWORD m_dAllInfo=0;
	DWORD m_dTime=0;
	//第一步：打开设备
	if(VCI_OpenDevice(m_nDeviceType,m_nDeviceIndex,Reserved)!=1)
	{
		printf("open failed");
		return 0;
	}
	VCI_INIT_CONFIG InitInfo[1];
	InitInfo->kCanBaud=15;
	InitInfo->Timing0=0x00;
	InitInfo->Timing1=0x14;
	InitInfo->Filter=0;
	InitInfo->AccCode=0x80000008;
	InitInfo->AccMask=0xFFFFFFFF;
	InitInfo->Mode=0;
	InitInfo->CanRx_IER=1;
    //第二部：初始化通道0，由于此设备为单通道所以只初始化通道0
	if(VCI_InitCAN(m_nDeviceType,m_nDeviceIndex, 0,InitInfo)!=1)	//can-0
	{
		printf("Init-CAN failed!");
		return 0;
	}
	Sleep(100);
    //第三步：启动通道0，由于此设备为单通道所以只启动通道0*/
 

    //第四步：向VCI_CAN_OBJ结构体填写一个发送信息
	VCI_CAN_OBJ sendbuf;
	sendbuf.DataLen=8;
    sendbuf.ExternFlag=0;//0~标准帧，1~扩展帧
	sendbuf.RemoteFlag=0;//0~数据帧，1~远程帧
	sendbuf.ID[0]=0x00;
	sendbuf.ID[1]=0xE0;
	sendbuf.ID[2]=0x00;
	sendbuf.ID[3]=0x00;
	for(int i=0;i<8;i++)
		sendbuf.Data[i]=16+i;



	//第五步：向外发送一个信息
	int flag=VCI_Transmit(m_nDeviceType,m_nDeviceIndex,0,&sendbuf);//CAN message send
	if(flag<1)
	{
		if(flag==-1)
			printf("failed- device not open\n");
		else if(flag==0)
			printf("send error\n");
		return 0;		
	}
	//第六步：开启一个接收循环，在这里接收循环20秒，可以接收这20秒内的数据；
	//第四步和第五步不分先后，在程序中可以手动控制发送，另外开启线程进行接收，
	//在此只是做一个单线程进行演示
	VCI_CAN_OBJ pCanObj[200];
	Sleep(1000);
	while(m_dTime<20)//调用20次接收函数
	{
		int NumValue=VCI_Receive(m_nDeviceType,m_nDeviceIndex,0,pCanObj);
		m_dAllInfo+=NumValue;
	    Sleep(1000);
		printf("Num of Received Message: %d\n",m_dAllInfo);
		m_dTime++;
	}
	printf("Test finish!");
	return 1;
}
