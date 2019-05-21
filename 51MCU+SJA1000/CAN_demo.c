//文件sja1000.h
//SJA1000内部RAM地址定义
               
#define sja1000_base_addr	0x8f00			//A[15,14]=1,0,CS_CAN=0,CS_USB=1

//SJA1000内部寄存器地址(初始化复位模式)
#define SJA_MOD 	*(unsigned char xdata *)(sja1000_base_addr + 0)     	/*模式寄存器 可以定义过滤模式 MOD.3=AFM  00H*/
#define SJA_CMR 	*(unsigned char xdata *)(sja1000_base_addr + 1)      	//命令寄存器
#define SJA_SR 		*(unsigned char xdata *)(sja1000_base_addr + 2)      	//状态寄存器 
#define SJA_IR 		*(unsigned char xdata *)(sja1000_base_addr + 3)   		//  中断标志         03
#define SJA_IER 	*(unsigned char xdata *)(sja1000_base_addr + 4)      	/*中断允许 04H  */
#define SJA_BTR0 	*(unsigned char xdata *)(sja1000_base_addr + 6)     	/*总线定时器0   06H*/
#define SJA_BTR1 	*(unsigned char xdata *)(sja1000_base_addr + 7)      	/*总线定时器1   07H*/
#define SJA_OCR		*(unsigned char xdata *)(sja1000_base_addr + 8)      	/*输出控制   08H*/
#define SJA_ALC 	*(unsigned char xdata *)(sja1000_base_addr + 11)  		//  仲裁丢失捕捉     11           
#define SJA_ECC 	*(unsigned char xdata *)(sja1000_base_addr + 12)       	/*错误代码捕捉  0CH*/
#define SJA_EWLR 	*(unsigned char xdata *)(sja1000_base_addr + 13)      	//错误报警寄存器
#define SJA_TXERR 	*(unsigned char xdata *)(sja1000_base_addr + 15)     	/*发送错误记数 0FH*/ 

//以下为复位模式定义
#define SJA_ACR0 	*(unsigned char xdata *)(sja1000_base_addr + 16)      	/*验收代码 即ID ?16*/  
#define SJA_ACR1 	*(unsigned char xdata *)(sja1000_base_addr + 17)      	/*验收代码 即ID   17*/
#define SJA_ACR2 	*(unsigned char xdata *)(sja1000_base_addr + 18)      	/*验收代码 即ID   18*/
#define SJA_ACR3 	*(unsigned char xdata *)(sja1000_base_addr + 19)      	/*验收代码 即ID   19*/
#define SJA_AMR0 	*(unsigned char xdata *)(sja1000_base_addr + 20)     	/*验收屏蔽  初地址20*/ 
#define SJA_AMR1 	*(unsigned char xdata *)(sja1000_base_addr + 21)      	/*验收屏蔽  初地址21*/ 
#define SJA_AMR2 	*(unsigned char xdata *)(sja1000_base_addr + 22)      	/*验收屏蔽  初地址22*/ 
#define SJA_AMR3 	*(unsigned char xdata *)(sja1000_base_addr + 23)      	/*验收屏蔽  初地址23*/

//SJA1000内部寄存器地址(工作模式)
#define SJA_EFF 	*(unsigned char xdata *)(sja1000_base_addr + 16)      	//RX和TX结构信息 扩展帧/标准帧
#define SJA_ID1 	*(unsigned char xdata *)(sja1000_base_addr + 17)      	//识别码
#define SJA_ID2 	*(unsigned char xdata *)(sja1000_base_addr + 18)      	//识别码
#define SJA_ID3 	*(unsigned char xdata *)(sja1000_base_addr + 19)      	//识别码，标准帧无
#define SJA_ID4 	*(unsigned char xdata *)(sja1000_base_addr + 20)      	//识别码，标准帧无
#define SJA_D1	 	*(unsigned char xdata *)(sja1000_base_addr + 21)      	//
#define SJA_D2	 	*(unsigned char xdata *)(sja1000_base_addr + 22)      
#define SJA_D3	 	*(unsigned char xdata *)(sja1000_base_addr + 23)      
#define SJA_D4	 	*(unsigned char xdata *)(sja1000_base_addr + 24)      
#define SJA_D5	 	*(unsigned char xdata *)(sja1000_base_addr + 25)    
#define SJA_D6	 	*(unsigned char xdata *)(sja1000_base_addr + 26)      
#define SJA_D7	 	*(unsigned char xdata *)(sja1000_base_addr + 27)    
#define SJA_D8	 	*(unsigned char xdata *)(sja1000_base_addr + 28)      	//  	//	//  	//	//	//	//
#define SJA_SFF_DATA 	*(unsigned char xdata *)(sja1000_base_addr + 19)     //发送数据区首址 扩展帧  
#define SJA_EFF_DATA 	*(unsigned char xdata *)(sja1000_base_addr + 21) 		//  接收缓冲区首址   21(PeilCAN,扩展祯)   
#define SJA_RBSA 	*(unsigned char xdata *)(sja1000_base_addr + 30)      	/*接收缓冲器起始地址 1EH*/
#define SJA_CD

void CanInit(void)
{

	Read24c02(UartRxFrame,0,8);

	SJA_MOD = 0x09;			//模式寄存器 进入复位模式 单滤波
	SJA_CDR = 0x0C8;		//时钟分频选择PeliCAN模式关闭时钟输出CLKOUT
	SJA_AMR0 = 0xFF;		//验收屏蔽0, 不屏蔽
	SJA_AMR1 = 0xFF;
	SJA_AMR2 = 0xFF;
	SJA_AMR3 = 0xFF;
	SJA_ACR0 = 0x09;		//验收代码0 扩展祯 数据 8字节
	SJA_ACR1 = 0x88;
	SJA_ACR2 = 0x88;
	SJA_ACR3 = 0x88;

	SJA_IER = 0x01;			//中断允许

	k=P1 & 0x0F;
	k=15;
	SJA_BTR0=Buad_SJA_BTR0[k]; //BTR0=0x00,BTR1=0x14 表示1Mbps
	SJA_BTR1=Buad_SJA_BTR1[k]; 

	SJA_OCR = 0xda;		//输出控制
	SJA_MOD = 0x08;			//模式寄存器  退出复位模式
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

	sjabuf=&SJA_EFF;		//取SJA_IDO地址
   	for(i=0;i<13;i++)  
   	{
        *(unsigned char xdata *)(sjabuf)=*(buf+i);
		
	   	sjabuf++;
   	 }
	SJA_CMR = 0x01;			//Send

	return len;

}


void can_rx_isr() interrupt 2						//sja1000 CAN接收中断
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