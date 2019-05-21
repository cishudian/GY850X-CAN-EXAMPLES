#include "ControlCAN.h"
#pragma   comment(lib,"VCI_CAN.lib")
int main()
{
	UINT m_nDeviceType=DEV_PCICAN;
	UINT m_nDeviceIndex=0;//�豸�����ţ����ֻ��1��USB-CAN������0
	DWORD Reserved=0;
	DWORD m_dAllInfo=0;
	DWORD m_dTime=0;
	//��һ�������豸
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
    //�ڶ�������ʼ��ͨ��0�����ڴ��豸Ϊ��ͨ������ֻ��ʼ��ͨ��0
	if(VCI_InitCAN(m_nDeviceType,m_nDeviceIndex, 0,InitInfo)!=1)	//can-0
	{
		printf("Init-CAN failed!");
		return 0;
	}
	Sleep(100);
    //������������ͨ��0�����ڴ��豸Ϊ��ͨ������ֻ����ͨ��0*/
 

    //���Ĳ�����VCI_CAN_OBJ�ṹ����дһ��������Ϣ
	VCI_CAN_OBJ sendbuf;
	sendbuf.DataLen=8;
    sendbuf.ExternFlag=0;//0~��׼֡��1~��չ֡
	sendbuf.RemoteFlag=0;//0~����֡��1~Զ��֡
	sendbuf.ID[0]=0x00;
	sendbuf.ID[1]=0xE0;
	sendbuf.ID[2]=0x00;
	sendbuf.ID[3]=0x00;
	for(int i=0;i<8;i++)
		sendbuf.Data[i]=16+i;



	//���岽�����ⷢ��һ����Ϣ
	int flag=VCI_Transmit(m_nDeviceType,m_nDeviceIndex,0,&sendbuf);//CAN message send
	if(flag<1)
	{
		if(flag==-1)
			printf("failed- device not open\n");
		else if(flag==0)
			printf("send error\n");
		return 0;		
	}
	//������������һ������ѭ�������������ѭ��20�룬���Խ�����20���ڵ����ݣ�
	//���Ĳ��͵��岽�����Ⱥ��ڳ����п����ֶ����Ʒ��ͣ����⿪���߳̽��н��գ�
	//�ڴ�ֻ����һ�����߳̽�����ʾ
	VCI_CAN_OBJ pCanObj[200];
	Sleep(1000);
	while(m_dTime<20)//����20�ν��պ���
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
