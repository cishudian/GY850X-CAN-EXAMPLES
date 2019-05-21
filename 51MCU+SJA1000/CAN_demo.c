//�ļ�sja1000.h
//SJA1000�ڲ�RAM��ַ����
               
#define sja1000_base_addr	0x8f00			//A[15,14]=1,0,CS_CAN=0,CS_USB=1

//SJA1000�ڲ��Ĵ�����ַ(��ʼ����λģʽ)
#define SJA_MOD 	*(unsigned char xdata *)(sja1000_base_addr + 0)     	/*ģʽ�Ĵ��� ���Զ������ģʽ MOD.3=AFM  00H*/
#define SJA_CMR 	*(unsigned char xdata *)(sja1000_base_addr + 1)      	//����Ĵ���
#define SJA_SR 		*(unsigned char xdata *)(sja1000_base_addr + 2)      	//״̬�Ĵ��� 
#define SJA_IR 		*(unsigned char xdata *)(sja1000_base_addr + 3)   		//  �жϱ�־         03
#define SJA_IER 	*(unsigned char xdata *)(sja1000_base_addr + 4)      	/*�ж����� 04H  */
#define SJA_BTR0 	*(unsigned char xdata *)(sja1000_base_addr + 6)     	/*���߶�ʱ��0   06H*/
#define SJA_BTR1 	*(unsigned char xdata *)(sja1000_base_addr + 7)      	/*���߶�ʱ��1   07H*/
#define SJA_OCR		*(unsigned char xdata *)(sja1000_base_addr + 8)      	/*�������   08H*/
#define SJA_ALC 	*(unsigned char xdata *)(sja1000_base_addr + 11)  		//  �ٲö�ʧ��׽     11           
#define SJA_ECC 	*(unsigned char xdata *)(sja1000_base_addr + 12)       	/*������벶׽  0CH*/
#define SJA_EWLR 	*(unsigned char xdata *)(sja1000_base_addr + 13)      	//���󱨾��Ĵ���
#define SJA_TXERR 	*(unsigned char xdata *)(sja1000_base_addr + 15)     	/*���ʹ������ 0FH*/ 

//����Ϊ��λģʽ����
#define SJA_ACR0 	*(unsigned char xdata *)(sja1000_base_addr + 16)      	/*���մ��� ��ID ?16*/  
#define SJA_ACR1 	*(unsigned char xdata *)(sja1000_base_addr + 17)      	/*���մ��� ��ID   17*/
#define SJA_ACR2 	*(unsigned char xdata *)(sja1000_base_addr + 18)      	/*���մ��� ��ID   18*/
#define SJA_ACR3 	*(unsigned char xdata *)(sja1000_base_addr + 19)      	/*���մ��� ��ID   19*/
#define SJA_AMR0 	*(unsigned char xdata *)(sja1000_base_addr + 20)     	/*��������  ����ַ20*/ 
#define SJA_AMR1 	*(unsigned char xdata *)(sja1000_base_addr + 21)      	/*��������  ����ַ21*/ 
#define SJA_AMR2 	*(unsigned char xdata *)(sja1000_base_addr + 22)      	/*��������  ����ַ22*/ 
#define SJA_AMR3 	*(unsigned char xdata *)(sja1000_base_addr + 23)      	/*��������  ����ַ23*/

//SJA1000�ڲ��Ĵ�����ַ(����ģʽ)
#define SJA_EFF 	*(unsigned char xdata *)(sja1000_base_addr + 16)      	//RX��TX�ṹ��Ϣ ��չ֡/��׼֡
#define SJA_ID1 	*(unsigned char xdata *)(sja1000_base_addr + 17)      	//ʶ����
#define SJA_ID2 	*(unsigned char xdata *)(sja1000_base_addr + 18)      	//ʶ����
#define SJA_ID3 	*(unsigned char xdata *)(sja1000_base_addr + 19)      	//ʶ���룬��׼֡��
#define SJA_ID4 	*(unsigned char xdata *)(sja1000_base_addr + 20)      	//ʶ���룬��׼֡��
#define SJA_D1	 	*(unsigned char xdata *)(sja1000_base_addr + 21)      	//
#define SJA_D2	 	*(unsigned char xdata *)(sja1000_base_addr + 22)      
#define SJA_D3	 	*(unsigned char xdata *)(sja1000_base_addr + 23)      
#define SJA_D4	 	*(unsigned char xdata *)(sja1000_base_addr + 24)      
#define SJA_D5	 	*(unsigned char xdata *)(sja1000_base_addr + 25)    
#define SJA_D6	 	*(unsigned char xdata *)(sja1000_base_addr + 26)      
#define SJA_D7	 	*(unsigned char xdata *)(sja1000_base_addr + 27)    
#define SJA_D8	 	*(unsigned char xdata *)(sja1000_base_addr + 28)      	//  	//	//  	//	//	//	//
#define SJA_SFF_DATA 	*(unsigned char xdata *)(sja1000_base_addr + 19)     //������������ַ ��չ֡  
#define SJA_EFF_DATA 	*(unsigned char xdata *)(sja1000_base_addr + 21) 		//  ���ջ�������ַ   21(PeilCAN,��չ��)   
#define SJA_RBSA 	*(unsigned char xdata *)(sja1000_base_addr + 30)      	/*���ջ�������ʼ��ַ 1EH*/
#define SJA_CD

void CanInit(void)
{

	Read24c02(UartRxFrame,0,8);

	SJA_MOD = 0x09;			//ģʽ�Ĵ��� ���븴λģʽ ���˲�
	SJA_CDR = 0x0C8;		//ʱ�ӷ�Ƶѡ��PeliCANģʽ�ر�ʱ�����CLKOUT
	SJA_AMR0 = 0xFF;		//��������0, ������
	SJA_AMR1 = 0xFF;
	SJA_AMR2 = 0xFF;
	SJA_AMR3 = 0xFF;
	SJA_ACR0 = 0x09;		//���մ���0 ��չ�� ���� 8�ֽ�
	SJA_ACR1 = 0x88;
	SJA_ACR2 = 0x88;
	SJA_ACR3 = 0x88;

	SJA_IER = 0x01;			//�ж�����

	k=P1 & 0x0F;
	k=15;
	SJA_BTR0=Buad_SJA_BTR0[k]; //BTR0=0x00,BTR1=0x14 ��ʾ1Mbps
	SJA_BTR1=Buad_SJA_BTR1[k]; 

	SJA_OCR = 0xda;		//�������
	SJA_MOD = 0x08;			//ģʽ�Ĵ���  �˳���λģʽ
}

unsigned char CanSend(unsigned char len,unsigned char *buf)
{
	unsigned char temp2,i;
	unsigned int sjabuf;
	SJA_CMR=0x04;
	
	temp2=(SJA_SR)&0x10;
	if(temp2==0x10)			//
		temp2=SJA_SR & 0x10;
	
	temp2= SJA_SR &0x08;	
	temp2= SJA_SR &0x04;

	nn++;
	*(buf+9)=nn;
	*(buf+8)=nn>>8;

	sjabuf=&SJA_EFF;		//ȡSJA_IDO��ַ
   	for(i=0;i<13;i++)  
   	{
        *(unsigned char xdata *)(sjabuf)=*(buf+i);
		
	   	sjabuf++;
   	 }
	SJA_CMR = 0x01;			//Send

	return len;

}


void can_rx_isr() interrupt 2						//sja1000 CAN�����ж�
{
	unsigned char i=0;
	unsigned char *sjabuf;
	DISABLE;
	if((SJA_SR & 0x0c3)!=0)
	  {
		if(( SJA_SR & 0x80)==0)						//
		{
		   if(( SJA_IR  & 0x80)==0x80)				//
				SJA_CMR = 0x0c;
		   else if(( SJA_IR  & 0x01)==0x01)			//
		      {
				sjabuf=&SJA_EFF;					//register 17. data start address
				for(i=0;i<13;i++)
			 	{	
				  CanRxBuf[i]=*(unsigned char xdata *)sjabuf;
				  sjabuf++;  
				}
				SJA_CMR = 0x04 ;
		      }
		 }
	   }
	CAN_LED=!CAN_LED;

	can_rxdone_flag = 1;//for 232

	ENABLE;	
}