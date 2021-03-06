#include <stdio.h> 
#include <windows.h>
#include <cvirte.h>		
#include <userint.h>
#include <formatio.h>

#include "cu.h"
#include "ControlCAN.h"
#include <utility.h>
//多线程: 主线程显示 和查询接收各一个线程
HANDLE hThread_show; 		     // 显示线程句柄
DWORD threadID_show;
DWORD WINAPI ShowMode(void);

HANDLE hThread_receive; 		     // 查询接收线程句柄
DWORD threadID_receive;
DWORD WINAPI receive(void);



VCI_INIT_CONFIG  InitInfo;  
VCI_CAN_OBJ      sendcan;
VCI_CAN_OBJ      receivecan[300];

unsigned char receiveid[4];
unsigned char receivedata[8];
int receivelen;

unsigned char sendidshow[4];

int len_r;
int len;

int number = 0; 

int sendbasicready = 0;
int sendpeliready = 0;
int receivebasicready = 0;
int receivepeliready = 0;

unsigned int frame_format;
unsigned int frame_type;

unsigned int frame_id[10];
unsigned char frame_id_char[20];
unsigned int frame_data[20];
unsigned char frame_data_char[50];

unsigned char formatbuffer[13];


static int cantools;

void format(unsigned char x[ ], int len);




int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((cantools = LoadPanel (0, "cu.uir", CANTooLS)) < 0)
		return -1;
	DisplayPanel (cantools);
	RunUserInterface ();
	DiscardPanel (cantools);
	return 0;
}


//*********************************打开设备*******************************************
void CVICALLBACK open_can (int menuBar, int menuItem, void *callbackData,
		int panel)
{
     if(VCI_OpenDevice(DEV_USBCAN, 0, 0)!=1)
	 {
	      MessagePopup("提示", "打开失败   ");
	 }
	 else
	 {
		  SetMenuBarAttribute(MENUBAR_Menu1, MENUBAR_Menu1_open, ATTR_DIMMED, 1);
		  SetMenuBarAttribute(MENUBAR_Menu1, MENUBAR_Menu1_close, ATTR_DIMMED, 0);
		  
	      InitInfo.kCanBaud=15;
	      InitInfo.Timing0=0x00;
	      InitInfo.Timing1=0x14;
	      InitInfo.Filter=0;
	      InitInfo.AccCode=0x80000008;
	      InitInfo.AccMask=0xFFFFFFFF;
	      InitInfo.Mode=1;
	      InitInfo.CanRx_IER=1;
		  
		  if(VCI_InitCAN(DEV_USBCAN, 0, 0, &InitInfo)!=1)	
	      {
			   MessagePopup("提示", "初始化CAN失败   ");
	      }
		  else
		  {
			  if(VCI_StartCAN(DEV_USBCAN, 0, 0)!=1)	
	          {
		            MessagePopup("提示", "开始CAN失败  ");
	          }
			  else 
			  {
				    MessagePopup("提示", "打开成功   ");
					//安装新线程，新线程用于界面显示 
                    hThread_show = CreateThread( 
                                   NULL, 0,
                                   (LPTHREAD_START_ROUTINE)ShowMode,
                                   NULL, 0,
                                   (LPDWORD)&threadID_show);
				   //安装新线程，新线程用于查询接收并处理	
					hThread_receive = CreateThread( 
                                    NULL, 0,
                                    (LPTHREAD_START_ROUTINE)receive,
                                    NULL, 0,
                                    (LPDWORD)&threadID_receive);
			  }
		  }
	 }    
}

void CVICALLBACK close_can (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	 if(VCI_CloseDevice(DEV_USBCAN, 0)!=1)
	 {
		   MessagePopup("提示", "关闭失败   ");
	 }
	 else
	 {
	       MessagePopup("提示", "关闭成功   ");
		   SetMenuBarAttribute(MENUBAR_Menu1, MENUBAR_Menu1_open, ATTR_DIMMED, 0);
		   SetMenuBarAttribute(MENUBAR_Menu1, MENUBAR_Menu1_close, ATTR_DIMMED, 1);
	 }
}

void CVICALLBACK set_can (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	QuitUserInterface (0);
}

void CVICALLBACK reset_can (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	QuitUserInterface (0);
}

void CVICALLBACK quit_can (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	QuitUserInterface (0);
}

void CVICALLBACK clear_can (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	QuitUserInterface (0);
}

//**********************************清空显示******************************************
int CVICALLBACK clear (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			 DeleteTextBoxLines(CANTooLS, CANTooLS_receive, 0, -1);
			 number = 0;
			 
			break;
	}
	return 0;
}

//**********************************退出*********************************************

int CVICALLBACK quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			QuitUserInterface (0);
			
			break;
	}
	return 0;
}


//********************************发送*********************************************** 
int CVICALLBACK send (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	unsigned char sendcanid[4];
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(CANTooLS, CANTooLS_frame_format, &frame_format);
			GetCtrlVal(CANTooLS, CANTooLS_frame_type, &frame_type);
			GetCtrlVal(CANTooLS, CANTooLS_frame_data, frame_data_char);
			GetCtrlVal(CANTooLS, CANTooLS_frame_id, frame_id_char);

			
			len_r = strlen(frame_data_char);
			len = (len_r+1)/3;
			
			//**************标准帧************************
			if(frame_format == 0)
			{ 
			    format(frame_id_char, 2);
				sendcanid[0] = (formatbuffer[0]<<5) | (formatbuffer[1]>>3);
			    sendcanid[1] =  formatbuffer[1]<<5;  //ID 识别码
				
			    sendcan.ID[0] = sendcanid[0];
			    sendcan.ID[1] = sendcanid[1];
				
				sendidshow[1] = (sendcanid[1]>>5)|(sendcanid[0]<<3);
				sendidshow[0] =  sendcanid[0]>>5;
			 
			    format(frame_data_char, len);
			    for(i=0; i<len; i++)
			   {
				   sendcan.Data[i] = formatbuffer[i]; //数据
			   }
				sendcan.RemoteFlag = 0;
				sendcan.ExternFlag = 0;
				sendcan.DataLen	= len;

			   if(VCI_Transmit(DEV_USBCAN, 0, 0, &sendcan)!=1)
			   {
				   MessagePopup("提示", "发送失败   ");
			   }
			   else 
			   {
				   sendbasicready = 1;
			   }
			} 
			//**************扩展帧************************ 
			if(frame_format == 1)
			{ 
			    format(frame_id_char, 4);
				sendcanid[0] = (formatbuffer[0]<<3) | (formatbuffer[1]>>5);
				sendcanid[1] = (formatbuffer[1]<<3) | (formatbuffer[2]>>5);
			    sendcanid[2] = (formatbuffer[2]<<3) | (formatbuffer[3]>>5);
			    sendcanid[3] =  formatbuffer[3]<<3;  //ID 识别码
				
				sendcan.ID[0] = sendcanid[0];
				sendcan.ID[1] = sendcanid[1]; 
			    sendcan.ID[2] = sendcanid[2];
			    sendcan.ID[3] = sendcanid[3];
				
				sendidshow[3] = (sendcanid[2]<<5) | (sendcanid[3]>>3);
				sendidshow[2] = (sendcanid[1]<<5) | (sendcanid[2]>>3);
				sendidshow[1] = (sendcanid[0]<<5) | (sendcanid[1]>>3);
				sendidshow[0] =  sendcanid[0]>>3;
				
			 
			    format(frame_data_char, len);
			    for(i=0; i<len; i++)
			   {
				   sendcan.Data[i] = formatbuffer[i]; //数据
			   }
				sendcan.RemoteFlag = 0;
				sendcan.ExternFlag = 1;
				sendcan.DataLen	= len;

			   if(VCI_Transmit(DEV_USBCAN, 0, 0, &sendcan)!=1)
			   {
				   MessagePopup("提示", "发送失败   ");
			   }
			   else
			   {
				   sendpeliready = 1;
			   }
			} 			

			break;
	}
	return 0;
}


//*************************将带空格分立数据处理为unsigned char 可使用数据*****************
void format(unsigned char x[ ], int length)
{
	int i;
	int j;
	for(i=0; i< length*3-1; i++)
	{
		if(x[i]>=48 && x[i]<=57)
			x[i] -= 48;
		if(x[i]>=65 && x[i]<=70)
			x[i] -= 55;
		if(x[i]>=97 && x[i]<=102)
			x[i] -= 87;
	}
	for(i=0, j=0; i<length; i++)
	{
		formatbuffer[i] = 16*x[j] + x[j+1];
		j += 3;
	}
}

//***************************数据显示线程***********************************
DWORD WINAPI ShowMode(void) 
{
 char textbuf[1000];
 int i;
 int numvalue;
 int num;

 for(;;)
 {   
	 //***************发送标准帧显示*********
	 if(sendbasicready == 1)
	 {   
		 Fmt(textbuf,"%s< %d[w10p0]          发送          %x[w2p0]%x[w2p0]              数据帧          标准帧          %d          ", 
			 number++, sendidshow[0], sendidshow[1], len);
	     SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		 
		for(i=0; i<len; i++)
		 {
			  Fmt(textbuf,"%s<%x[w2p0] ", sendcan.Data[i]);
			  SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		 }
		 Fmt(textbuf,"%s<\n");
		 SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf); 
		 
		 sendbasicready = 0;
	 }
	 
	 //***************发送扩展帧显示**********
	 if(sendpeliready == 1)
	 {   																									    
		 Fmt(textbuf,"%s< %d[w10p0]          发送          %x[w2p0]%x[w2p0]%x[w2p0]%x[w2p0]          数据帧          扩展帧          %d          ", 
			 number++, sendidshow[0], sendidshow[1], sendidshow[2], sendidshow[3], len);
	     SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		 
		 for(i=0; i<len; i++)
		 {
			  Fmt(textbuf,"%s<%x[w2p0] ", sendcan.Data[i]);
			  SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		 }
		 Fmt(textbuf,"%s<\n");
		 SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		 																		
		 sendpeliready = 0;
	 }
	  
	//***************接收标准帧显示*********** 
	 if(receivebasicready == 1)
	 {
		  Fmt(textbuf,"%s< %d[w10p0]          接收          %x[w2p0]%x[w2p0]              数据帧          标准帧          %d          ", 
			  number++, receiveid[0], receiveid[1], receivelen);
	      SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
				 
		  for(i=0; i<receivelen; i++)
		  {
			   Fmt(textbuf,"%s<%x[w2p0] ", receivedata[i]);
			   SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		  }
		  
		  Fmt(textbuf,"%s<\n");
		  SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		  
		  receivebasicready = 0;
	 }
	 
    //***************接收扩展帧显示*************  	   
	 if(receivepeliready == 1)
	 {
		  Fmt(textbuf,"%s< %d[w10p0]          接收          %x[w2p0]%x[w2p0]%x[w2p0]%x[w2p0]          数据帧          扩展帧          %d          ", 
			  number++, receiveid[0], receiveid[1], receiveid[2], receiveid[3], receivelen);
	      SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
				 
		  for(i=0; i<receivelen; i++)
		  {																						
			   Fmt(textbuf,"%s<%x[w2p0] ", receivedata[i]);
			   SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		  }
		  
		  Fmt(textbuf,"%s<\n");
		  SetCtrlVal(CANTooLS, CANTooLS_receive, textbuf);
		  
		  receivepeliready = 0;
	 }   
 }
}

//***************************接收查询处理线程***********************************
DWORD WINAPI receive(void)
{
	int numvalue;
    int num;
	int i;
	char textbuf[1000];
	
	for(;;)
	{
	    numvalue = VCI_Receive(DEV_USBCAN,0,0, receivecan);
        if(numvalue > 0)
	    {
		    for(num=0; num<numvalue; num++)
		    {
			    if(receivecan[num].RemoteFlag == 1)
		        {
				 break;
		        }
				
			    if(receivecan[num].ExternFlag == 0)
			    {
					receiveid[0] = receivecan[num].ID[0];
					receiveid[1] = receivecan[num].ID[1];	
				    
				    receiveid[1] = (receiveid[1]>>5)|(receiveid[0]<<3);
					receiveid[0] =  receiveid[0]>>5;
				 
				    receivelen = receivecan[num].DataLen;
				 
				    for(i=0; i<receivelen; i++)
				    {
					    receivedata[i] = receivecan[num].Data[i];
				    }
					receivebasicready = 1;
					
	            }  
	    		else if(receivecan[num].ExternFlag == 1)          
			    {
					receiveid[0] = receivecan[num].ID[0];
					receiveid[1] = receivecan[num].ID[1];
					receiveid[2] = receivecan[num].ID[2];
					receiveid[3] = receivecan[num].ID[3];
					
					receiveid[3] = (receiveid[2]<<5) | (receiveid[3]>>3);
				    receiveid[2] = (receiveid[1]<<5) | (receiveid[2]>>3);
					receiveid[1] = (receiveid[0]<<5) | (receiveid[1]>>3);
					receiveid[0] =  receiveid[0]>>3;
				 
				    receivelen = receivecan[num].DataLen;
				 
				    for(i=0; i<receivelen; i++)
				    {
					    receivedata[i] = receivecan[num].Data[i];
				    }
					receivepeliready = 1;
	            }
				
		    }
		 }
	}
} 
  


