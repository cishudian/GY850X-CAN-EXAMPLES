//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include <time.h>
#include "Main.h"
#include "Math.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "CCALENDR"
#pragma link "PERFGRAP"
#pragma resource "*.dfm"
TForm1 *Form1;

/*
下面是具体的调用动态库的做法，采用的是动态加载的方法，主要就是在程序初始化的
时候从动态库中取得各个函数的地址并保存起来，然后在需要的时候调用就可以了，最后在
程序退出的时候释放掉打开的动态库句柄就行。
*/   

//首先定义需要用到的数据结构
//1.GY85XX系列CAN接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
		USHORT	hw_Version;
		USHORT	fw_Version;
		USHORT	dr_Version;
		USHORT	in_Version;
		USHORT	irq_Num;
		BYTE	can_Num;
		CHAR	str_Serial_Num[8];
		CHAR	str_hw_Type[16];
		char	str_GYUsb_Serial[4][4];
	
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; //共63个字节

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
	BYTE	ID[4];
	UINT	TimeStamp;
	BYTE	TimeFlag;
	BYTE	SendType;//保留不用 yyd
	BYTE	RemoteFlag;//是否是远程帧
	BYTE	ExternFlag;//是否是扩展帧
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];//Reserved[0] means the kCanIndex for received data
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;
	UCHAR	regMode;    
	UCHAR	regStatus;
	UCHAR	regALCapture;
	UCHAR	regECCapture; 
	UCHAR	regEWLimit;
	UCHAR	regRECounter; 
	UCHAR	regTECounter;
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _ERR_INFO{
		UINT	ErrCode;
		BYTE	Passive_ErrData[3];
		BYTE	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	Reserved;
	UCHAR	Filter;
	UCHAR	kCanBaud;
	UCHAR	Timing0;
	UCHAR	Timing1;
	UCHAR	Mode;
	UCHAR	CanRx_IER;  //added by yyd
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

//接下来定义要导入的函数类型
//根据ControlCAN.h中的函数声明定义函数指针类型
//////////////////////////////////////////////////////////////////////////
typedef DWORD (CALLBACK*  LPVCI_OpenDevice)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_CloseDevice)(DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_InitCan)(DWORD,DWORD,DWORD,PVCI_INIT_CONFIG);

typedef DWORD (CALLBACK*  LPVCI_ReadBoardInfo)(DWORD,DWORD,PVCI_BOARD_INFO);
typedef DWORD (CALLBACK*  LPVCI_ReadErrInfo)(DWORD,DWORD,DWORD,PVCI_ERR_INFO);
typedef DWORD (CALLBACK*  LPVCI_ReadCanStatus)(DWORD,DWORD,DWORD,PVCI_CAN_STATUS);

typedef DWORD (CALLBACK*  LPVCI_GetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);
typedef DWORD (CALLBACK*  LPVCI_SetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);

typedef ULONG (CALLBACK*  LPVCI_GetReceiveNum)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_ClearBuffer)(DWORD,DWORD,DWORD);

typedef DWORD (CALLBACK*  LPVCI_StartCAN)(DWORD,DWORD,DWORD);
typedef DWORD (CALLBACK*  LPVCI_ResetCAN)(DWORD,DWORD,DWORD);

typedef ULONG (CALLBACK*  LPVCI_Transmit)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ);
typedef ULONG (CALLBACK*  LPVCI_Receive)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ);
//////////////////////////////////////////////////////////////////////////

HANDLE m_hDLL;//用来保存打开的动态库句柄

//接口函数指针
LPVCI_OpenDevice VCI_OpenDevice;
LPVCI_CloseDevice VCI_CloseDevice;
LPVCI_InitCan VCI_InitCAN;
LPVCI_ReadBoardInfo VCI_ReadBoardInfo;
LPVCI_ReadErrInfo VCI_ReadErrInfo;
LPVCI_ReadCanStatus VCI_ReadCanStatus;
LPVCI_GetReference VCI_GetReference;
LPVCI_SetReference VCI_SetReference;
LPVCI_GetReceiveNum VCI_GetReceiveNum;
LPVCI_ClearBuffer VCI_ClearBuffer;
LPVCI_StartCAN VCI_StartCAN;
LPVCI_ResetCAN VCI_ResetCAN;
LPVCI_Transmit VCI_Transmit;
LPVCI_Receive VCI_Receive;

int m_connect=0;

int i=0;
int j=0;
int k=0;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        m_hDLL = LoadLibrary("VCI_CAN.dll");//打开动态库
        //取得函数地址
        VCI_OpenDevice=(LPVCI_OpenDevice)GetProcAddress(m_hDLL,"VCI_OpenDevice");
 	VCI_CloseDevice=(LPVCI_CloseDevice)GetProcAddress(m_hDLL,"VCI_CloseDevice");
   	VCI_InitCAN=(LPVCI_InitCan)GetProcAddress(m_hDLL,"VCI_InitCAN");
   	VCI_ReadBoardInfo=(LPVCI_ReadBoardInfo)GetProcAddress(m_hDLL,"VCI_ReadBoardInfo");
    	VCI_ReadErrInfo=(LPVCI_ReadErrInfo)GetProcAddress(m_hDLL,"VCI_ReadErrInfo");
    	VCI_ReadCanStatus=(LPVCI_ReadCanStatus)GetProcAddress(m_hDLL,"VCI_ReadCANStatus");
  	VCI_GetReference=(LPVCI_GetReference)GetProcAddress(m_hDLL,"VCI_GetReference");
    	VCI_SetReference=(LPVCI_SetReference)GetProcAddress(m_hDLL,"VCI_SetReference");
  	VCI_GetReceiveNum=(LPVCI_GetReceiveNum)GetProcAddress(m_hDLL,"VCI_GetReceiveNum");
 	VCI_ClearBuffer=(LPVCI_ClearBuffer)GetProcAddress(m_hDLL,"VCI_ClearBuffer");
 	VCI_StartCAN=(LPVCI_StartCAN)GetProcAddress(m_hDLL,"VCI_StartCAN");
  	VCI_ResetCAN=(LPVCI_ResetCAN)GetProcAddress(m_hDLL,"VCI_ResetCAN");
    	VCI_Transmit=(LPVCI_Transmit)GetProcAddress(m_hDLL,"VCI_Transmit");
    	VCI_Receive=(LPVCI_Receive)GetProcAddress(m_hDLL,"VCI_Receive");
}
//---------------------------------------------------------------------------
void ReceiveThread(void *param)
{
   TListBox *box=(TListBox*)param;
   VCI_CAN_OBJ receivedata[50];
   VCI_ERR_INFO errinfo;
   int len,i;
   AnsiString str,tmpstr;
   while(1)
   {
        if(m_connect==0)
                break;
        Sleep(1);
        len=VCI_Receive(2,0,0,receivedata);
        if(len<=0)
        {
                //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
                //千万不能省略这一步（即使你可能不想知道错误码是什么）
                VCI_ReadErrInfo(2,0,0,&errinfo);
        }
        else
        {
	        for(i=0;i<len;i++)
		{
			str="接收到数据帧:  ";
			if(receivedata[i].TimeFlag==0)
				tmpstr="时间标识:无  ";
			else
				tmpstr="时间标识:0x"+IntToHex((int)receivedata[i].TimeStamp,8)+" ";
			str+=tmpstr;
			tmpstr="帧ID:0x"+IntToHex((int)receivedata[i].ID,8)+" ";
			str+=tmpstr;
			str+="帧格式:";
			if(receivedata[i].RemoteFlag==0)
				tmpstr="数据帧 ";
			else
				tmpstr="远程帧 ";
			str+=tmpstr;
			str+="帧类型:";
			if(receivedata[i].ExternFlag==0)
				tmpstr="标准帧 ";
			else
				tmpstr="扩展帧 ";
			str+=tmpstr;
			box->Items->Add(str);
			if(receivedata[i].RemoteFlag==0)
			{
				str="数据:";
                                if(receivedata[i].DataLen>8)
                                        receivedata[i].DataLen=8;
				for(int j=0;j<receivedata[i].DataLen;j++)
				{
					tmpstr=IntToHex((int)receivedata[i].Data[j],2)+" ";
					str+=tmpstr;
				}
				box->Items->Add(str);
			}
		}
		box->ItemIndex=box->Items->Count-1;
        }
   }

   //_endthread();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
 
 // PaintBox1->Left=PaintBox1->Left-10;
  
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
{
 // PaintBox1->Left=PaintBox1->Left+10;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  // PaintBox1->Canvas->Pen->Color=clRed;
   k=15+rand()%490;
  // Edit7->Text=k;
  // PaintBox1->Canvas->LineTo(j,k);
   j=j+10;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{


    if(VCI_OpenDevice(2,0,0)!=1)
    {
       ShowMessage("fail to open!");
       return;
    }
 
    
    m_connect=1;
    Button5->Caption ="Close Device";
    //ShowMessage("Start Ok!");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
{


    VCI_INIT_CONFIG initconfig;
    initconfig.Mode=1;   //1 自收发 self-reception test
    initconfig.Filter=0;
    initconfig.kCanBaud=15;
    initconfig.Timing0=StrToInt("0x00");
    initconfig.Timing1=StrToInt("0x14");
    initconfig.AccCode=StrToInt("0x00000000");
    initconfig.AccMask=StrToInt("0xFFFFFFFF");
    initconfig.CanRx_IER = 1;

   if(VCI_InitCAN(2,0,0,&initconfig)!=1)
    {
       ShowMessage("fail to Init CAN!");
       return;
    }
    if(VCI_StartCAN(2,0,0)!=1)
    {
       ShowMessage("Fail to start CAN!");
       return;
    }


}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        if(m_connect==1)
        {
                m_connect=0;
                VCI_CloseDevice(2,0);
        }

        FreeLibrary(m_hDLL);//释放动态库句柄
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
        if(m_connect==0)
        {
                ShowMessage("please open device");
                return;
        }
        VCI_CAN_OBJ sendbuf[2];
        sendbuf[0].ExternFlag=0;   //
        sendbuf[0].DataLen=8;
        sendbuf[0].RemoteFlag=0;  //

        sendbuf[0].ID[0]=0;
        sendbuf[0].ID[1]=113;
        sendbuf[0].ID[2]=0;
        sendbuf[0].ID[3]=0;

        sendbuf[0].Data[0]=0x33;
        sendbuf[0].Data[1]=0x35;
        sendbuf[0].Data[2]=0x31;
        sendbuf[0].Data[3]=0x31;
        sendbuf[0].Data[4]=0x31;
        sendbuf[0].Data[5]=0x31;
        sendbuf[0].Data[6]=0x31;
        sendbuf[0].Data[7]=0x31;
        if(VCI_Transmit(2,0,0,sendbuf)!=1)
        {
           ShowMessage("Transmit failure!");
           return;
        }
        String temp;
        for(int i=0;i<=7;i++)
        {
           temp=temp+" "+sendbuf[0].Data[i];
        }
        time_t ltime;
        ListBox1->Items->Add("Transmit    "+ Now().FormatString("yyyy-mm-dd hh:nn:ss    ")+temp);
        //ShowMessage("发送成功!"+temp);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button8Click(TObject *Sender)
{
   if(m_connect==0)
   {
      ShowMessage("please open device");
      return;
   }
   VCI_CAN_OBJ databuf[200];
   int rxlen= VCI_Receive(2,0,0,databuf);
   if(rxlen<=0)
   {
      ListBox1->Items->Add("Receive fail");
      //ShowMessage("接收失败");
      return;
   }
   for(int j=0;j<rxlen;j++)
   {
          String temp;
          for(int i=0;i<=7;i++)
          {
            temp=temp+" "+databuf[j].Data[i];
          }
          ListBox1->Items->Add("Receive    "+ Now().FormatString("yyyy-mm-dd hh:nn:ss    ")+temp);
  }

   //ShowMessage("接收成功!"+temp);

}
//---------------------------------------------------------------------------


