{======================================================================
  Title : PCI_CAN TEST
  Discription : ...
  Author : Conjewa
  Date : 2009-7-22
  Vision : V1.0
=======================================================================}
unit MainProgram;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, ImgList, ComCtrls, ToolWin, ExtCtrls, DB, ADODB, StdCtrls,
  Grids, Buttons, DBGrids, Comobj, WordXP;
  //  Outline,  FileCtrl,    临时取消引用这两个类
type
    TMainform = class(TForm)
    StatusBar1: TStatusBar;
    MainMenu1: TMainMenu;
    S1: TMenuItem;
    N23: TMenuItem;
    N24: TMenuItem;
    L1: TMenuItem;
    N21: TMenuItem;
    N25: TMenuItem;
    F1: TMenuItem;
    N2: TMenuItem;
    N1: TMenuItem;
    N14: TMenuItem;
    N15: TMenuItem;
    N16: TMenuItem;
    Timer1: TTimer;
    Panel1: TPanel;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Edit1: TEdit;
    Edit2: TEdit;
    ComboBox1: TComboBox;
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    StringGrid1: TStringGrid;
    BitBtn3: TBitBtn;
    procedure Timer1Timer(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure FormCreate(Sender: TObject);
    procedure N23Click(Sender: TObject);
    procedure N24Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
//   procedure CheckBox2Click(Sender: TObject);
//    procedure BitBtn3Click(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
    // 自定义的函数声明
    function OpenDevice(DeviceType,instance,Error:Integer):Boolean;
    function InitializeCan():Boolean;
    function OnButtonSend():Boolean;
    function CloseDevice(DeviceType,instance:Integer):Boolean;

  end;
var
  Mainform: TMainform;

// 定义"记录"类型

//2.定义CAN信息帧的数据类型。
type
  VCI_CAN_OBJ = record  // 记录类型名：VCI_CAN_OBJ
  ID : array[0..3] of Byte;  //CAN消息中的ID，左对齐格式。例：若ID=2,则依次填入00 40 00 00
  TimeStamp:Integer;   // 保留参数，不用
  TimeFlag: Byte;   // 保留参数，不用
  SendType: Byte;   // 保留参数，不用
  RemoteFlag: Byte; // 选择是否远程帧。1 表示远程帧，0 表示数据帧
  ExternFlag: Byte; // 选择是否扩展帧。1 表示扩展帧，0 表示标准帧
  DataLen:  Byte;   // 有效数据字节长度，(<=8)
  Data : array[0..7] of Byte;  // CAN 消息中的数据缓冲区。
  Reserved : array[0..2] of BYTE;    // 保留参数
end;
PVCI_CAN_OBJ =^ VCI_CAN_OBJ;    // 定义指针类型：PVCI_CAN_OBJ


// 5.定义初始化CAN的数据类型
type
  VCI_INIT_CONFIG = record // 记录类型名：VCI_INIT_CONFIG
  AccCode: Integer;       // 过滤屏蔽码。左对齐
  AccMask: Integer;       // 过滤屏蔽码。左对齐
  Reserved : Integer;     // 保留不用
  Filter : Byte;          // 过滤方式。0 表示单滤波，1 表示双滤波
  kCanBaud : Byte;        // CAN 波特率索引号
  Timing0 : Byte;         // CAN 波特率定时器BTR0
  Timing1 : Byte;         // CAN 波特率定时器BTR1
  Mode : Byte;            // 工作模式。0 表示正常模式，1 表示自发自收模式。
  CanRx_IER : Byte;       // ??
end;
PVCI_INIT_CONFIG =^ VCI_INIT_CONFIG;    // 定义指针类型：PVCI_INIT_CONFIG

// 下面是全局变量的定义：
var m_hDevice: integer = 0; // 打开设备返回值：0：fail; 1:Successful; -1:device not open or error
var m_bIsOpen: Boolean = False; // 表示设备状态: True: 设备已打开
var RowNumber: integer = 1; //  StringGrid的活动行号
var m_bIsFirstCallBack: Boolean;
var m_bCallBack: Boolean;
// 全局变量定义结束


// 接口卡类型定义  
const DEV_CAN232B   = 1;
const DEV_USBCAN    = 2;
const DEV_USBCAN200 = 3;
const DEV_NETCAN    = 4;
const DEV_NETCAN200 = 5;
const DEV_PCICAN    = 6;
const DEV_PCICAN200 = 7;

// 转换器参数地址表

//      参数名称       地址                 内  容

const REFTYPE_MODE   =	0;  // 工作模式。0 表示正常模式，1 表示自测模式
const REFTYPE_FILTER =	1;  //  滤波方式。0 表示单滤波，1 表示双滤波
const REFTYPE_ACR0	 =	2;  //  过滤验收码。
const REFTYPE_ACR1	 =	3;  //  过滤验收码。
const REFTYPE_ACR2   =	4;  //  过滤验收码。
const REFTYPE_ACR3	 =	5;  //  过滤验收码。
const REFTYPE_AMR0	 =	6;  //  过滤验收码。
const REFTYPE_AMR1	 =	7;  //  过滤验收码。
const REFTYPE_AMR2	 =	8;  //  过滤验收码。
const REFTYPE_AMR3   =	9;  //  过滤验收码。
const REFTYPE_kCANBAUD =	10;  //  此参数只是索引，对波特率设置无影响
const REFTYPE_TIMING0	 =	11;  //  波特率定时器BTR0
const REFTYPE_TIMING1	 =	12;  //  波特率定时器BTR1
const REFTYPE_CANRX_EN =  13;  //  接口卡内部的中断接收是否开启。默认值为1，开启
const REFTYPE_UARTBAUD =	14;  //  串口波特率选择。仅用于CAN232B
const REFTYPE_ALL		   =	15;  //  所有参数
const REFTYPE_DEVICE_IP0 =	16;  //
const REFTYPE_DEVICE_IP1 =	17;  //
const REFTYPE_DEVICE_IP2 =	18;  //
const REFTYPE_DEVICE_IP3 =	19;  //
const REFTYPE_HOST_IP0 =	20;    //
const REFTYPE_HOST_IP1 =	21;    //
const REFTYPE_HOST_IP2 =	22;    //
const REFTYPE_HOST_IP3 =	23;    //
const REFTYPE_BORRD_INFO =	32;  //


implementation

// VCI_CAN.dll中的函数声明
// 此函数用户连接设备
function VCI_OpenDevice(DeviceType:Integer;DeviceInd:Integer;Reserved:Integer):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_OpenDevice';
// 此函数用于初始化CAN接口函数
function VCI_InitCAN(DeviceType:Integer;DeviceInd:Integer;CANInd:Integer;pInitConfig:PVCI_INIT_CONFIG):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_InitCAN';
// 此函数用于CAN消息帧发送
function VCI_Transmit(DeviceType:Integer;DeviceInd:Integer;CANInd:Integer;pSend:PVCI_CAN_OBJ):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_Transmit';
// 此函数用于设置接收消息
function VCI_Receive(DeviceType:Integer;DeviceInd:Integer;CANInd:Integer;pReceive:PVCI_CAN_OBJ):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_Receive';


// 此函数用户关闭设备
function VCI_CloseDevice(DeviceType:Integer;DeviceInd:Integer):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_CloseDevice';
              
{$R *.dfm}

//*********************************************************************
//  函数名称 : OpenDevice(DeviceType,instance,Error:Integer)
//  函数描述 : 打开设备
//  函数调用 :VCI_OpenDevice
//  全局变量 : m_bIsOpen
//  入参: DeviceType设备类型,instance设备序号,Error返回错误代码（占不用）
//  输出: result:Boolean
//********************************************************************/
function TMainform.OpenDevice(DeviceType,instance,Error:Integer):Boolean;
begin
  // 打开设备
  m_hDevice := VCI_OpenDevice(DEV_PCICAN200,0,0);
  // 判断设备打开是否成功
  if m_hDevice = 1 then
	begin
    MessageDlg('打开PCI_CAN卡成功！',mtInformation,[mbOK],0);
		m_bIsOpen := True;
    result := True;
	end
  else
  begin
    MessageDlg('打开PCI_CAN卡失败！',mtError,[mbOK],0);
		m_bIsOpen := False;
    result := False;
  end;
end;

//*****************************************************
//  函数名称:InitializeCan()
//  函数描述:初始化两路CAN通道，即初始化SJA1000
//  函数调用:VCI_InitCAN
//  全局变量:m_bIsOpen
//  入参:无
//  输出:result:Boolean
//*****************************************************/
function TMainform.InitializeCan():Boolean;
var  NewVCI_INIT_CONFIG : VCI_INIT_CONFIG;
var  pConfig : PVCI_INIT_CONFIG;
var i,j : integer;
begin
  pConfig := @NewVCI_INIT_CONFIG;
  NewVCI_INIT_CONFIG.AccCode := 0;
  NewVCI_INIT_CONFIG.AccMask := $ffffffff;
	NewVCI_INIT_CONFIG.Mode := $0;
	NewVCI_INIT_CONFIG.Filter := $0;
	NewVCI_INIT_CONFIG.Timing0 := $0;
	NewVCI_INIT_CONFIG.Timing1 := $14;
  NewVCI_INIT_CONFIG.CanRx_IER := 1;
  result := True;
  i := VCI_InitCAN(DEV_PCICAN200,0,0,pConfig);
  if i <> 1 then
    result := False;
  j := VCI_InitCAN(DEV_PCICAN200,0,1,pConfig);
  if j <> 1 then
    result := False;
  if result = False then
  begin
    MessageDlg('CAN设备初始化失败！',mtError,[mbOK],0);
    m_bIsOpen := False;
  end;
end;

//*********************************************************
//  函数名称:OnButtonSend()
//  函数描述:发送数据
//  函数调用:Str2Hex()，VCI_Transmit
//  全局变量:无
//  入参:无
//  输出:result:Boolean
//*********************************************************/
function TMainform.OnButtonSend():Boolean;
var  NewVCI_CAN_OBJ :VCI_CAN_OBJ;
var  pSend : PVCI_CAN_OBJ;
var  i,j : integer;
begin
  result := True;
  // 判断设备是否就绪
  if m_bIsOpen = False then
  begin
     MessageDlg('CAN设备未打开或初始化失败！',mtError,[mbOK],0);
     result := False;
  end
  else
  begin
    pSend := @NewVCI_CAN_OBJ; // 地址赋值
    NewVCI_CAN_OBJ.DataLen := 8;
    NewVCI_CAN_OBJ.ExternFlag := 0;
    NewVCI_CAN_OBJ.ID[0] := $00;
    NewVCI_CAN_OBJ.ID[1] := $40;
    NewVCI_CAN_OBJ.ID[2] := $00;
    NewVCI_CAN_OBJ.ID[3] := $00;
    NewVCI_CAN_OBJ.RemoteFlag := 0;
    for i:=0 to 7 do
    begin
      NewVCI_CAN_OBJ.Data[i] := i;
    end;
    j := VCI_Transmit(DEV_PCICAN200,0,0,pSend);
    if j <> 1 then
    begin
      MessageDlg('CAN消息发送失败',mtError,[mbOK],0);
      result := False;
    end
    else
    begin
      with StringGrid1 do
      begin
        Cells[0,RowNumber] := IntToStr(RowNumber);
        Cells[1,RowNumber] := FormatDateTime('hh:mm:ss',time);
        Cells[2,RowNumber] := '0';
        Cells[3,RowNumber] := '发送';
        Cells[4,RowNumber] := '0001';
        Cells[5,RowNumber] := '数据帧';
        Cells[6,RowNumber] := '标准帧';
        Cells[7,RowNumber] := '8';
        Cells[8,RowNumber] := '00 11 22 33 44 55 66 77';
        Cells[9,RowNumber] := '发送成功';
      end;
      RowNumber := RowNumber + 1;
    end;
  end;
end;

//*****************************************************
//  函数名称:CloseDevice(DeviceType,instance:Integer)
//  函数描述:关闭设备
//  函数调用:VCI_CloseDevice
//  全局变量:m_bIsOpen
//  入参:DeviceType,instance
//  输出:result:Boolean
//*****************************************************/
function TMainform.CloseDevice(DeviceType,instance:Integer):Boolean;
var i:integer;
begin
  result := True;
  if m_bIsOpen = False then
  begin
    MessageDlg('设备关闭失败！原因：设备未打开！',mtError,[mbOK],0);
  end
  else
  begin
    i := VCI_CloseDevice(DeviceType,instance);
    // 判断设备关闭是否成功
    if i = 1 then
	  begin
      MessageDlg('PCI_CAN卡关闭成功！',mtInformation,[mbOK],0);
		  m_bIsOpen := False;
      result := True;
	  end
    else
    begin
      MessageDlg('PCI_CAN卡关闭失败！',mtError,[mbOK],0);
		  result := False;
    end;
  end;
end;

// 退出系统时，询问是否关闭
procedure TMainform.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
var ret:integer;
var STR1,STR2:PansiChar;
begin
  STR1 := '   确认退出CAN总线通讯测试程序？    ';
  STR2 := 'CAN总线通讯测试程序';
  ret :=MessageBox(handle,STR1,STR2,MB_YESNO);
  if ret = IDYES then
    Canclose := True
  else
    Canclose := False;
end;

// 状态栏加入日期，时间
procedure TMainform.Timer1Timer(Sender: TObject);
begin
  with StatusBar1 do
  begin
    //显示时间
    Panels[0].Text := Concat(DateToStr(Date),'        ',
        FormatDateTime('hh:mm:ss',time));
    //显示日期 和 系统名
    Panels[1].Text := Concat('CAN总线通讯测试程序',
        '          ','Version 1.0', '   ','(All Rights Reserved)');
  end;
end;

// 主窗体建立事件
procedure TMainform.FormCreate(Sender: TObject);
begin
//  StringGrid1.EditorMode := False;
  with StringGrid1 do
  begin
    Cells[0,0]:='序号';
    Cells[1,0]:='时间';
    Cells[2,0]:='CAN通道';
    Cells[3,0]:='传输方向';
    Cells[4,0]:='ID号';
    Cells[5,0]:='帧类型';
    Cells[6,0]:='帧格式';
    Cells[7,0]:='数据长度';
    Cells[8,0]:='数据';
    Cells[9,0]:='备注';
    ColWidths[0]:=40;
    ColWidths[1]:=80;
    ColWidths[2]:=60;
    ColWidths[3]:=60;
    ColWidths[4]:=70;
    ColWidths[5]:=60;
    ColWidths[6]:=60;
    ColWidths[7]:=80;
    ColWidths[8]:=180;
    ColWidths[9]:=80;
  end;
end;

// 菜单栏：设备操作：打开设备
procedure TMainform.N23Click(Sender: TObject);
var i:Boolean;
begin
  // 打开PCI_CANA卡
  i := OpenDevice(DEV_PCICAN200,0,0); 
  if i = True then
  begin
    //初始化SJA1000
    initializeCan();
  end;
end; 

// 主窗体：“发送”按钮
procedure TMainform.BitBtn1Click(Sender: TObject);
begin
  OnButtonSend();
end;

// 菜单栏：设备操作：关闭设备
procedure TMainform.N24Click(Sender: TObject);
begin
  CloseDevice(DEV_PCICAN200,0);
end;

// 主窗体：“接收按钮”
procedure TMainform.BitBtn2Click(Sender: TObject);
var i : integer;
var kk:string;
   var databuf : array[1..300] of VCI_CAN_OBJ;
  var pReceive : PVCI_CAN_OBJ;
  var value : integer;
  begin
    pReceive := @databuf[1]; // 地址赋值
    value := VCI_Receive(DEV_PCICAN200,0,0,pReceive);
    for i:=0 to (value-1) do

      begin
      with StringGrid1 do
      begin
        Cells[0,RowNumber] := IntToStr(RowNumber);
        Cells[1,RowNumber] := FormatDateTime('hh:mm:ss',time);
        Cells[2,RowNumber] := '0';
        Cells[3,RowNumber] := '接收';
        Cells[4,RowNumber] :=inttohex(databuf[i+1].ID[0],2)+' '+inttohex(databuf[i+1].ID[1],2)+' '+inttohex(databuf[i+1].ID[2],2)+' '+inttohex(databuf[i+1].ID[3],2);
         if (databuf[i+1].ExternFlag = 1)  then
          begin  Cells[5,RowNumber] := '远程帧'; end
        else
         begin Cells[5,RowNumber] := '数据帧';   end;
 
        if (databuf[i+1].ExternFlag = 1)  then
          begin  Cells[6,RowNumber] := '扩展帧'; end
        else
         begin Cells[6,RowNumber] := '标准帧';   end;
       // kk:=  inttohex(databuf[i+1].Data[0],2);
        Cells[7,RowNumber] := inttohex(databuf[i+1].DataLen,2);
        Cells[8,RowNumber] :=inttohex(databuf[i+1].Data[0],2)+' '+inttohex(databuf[i+1].Data[1],2)+' '+inttohex(databuf[i+1].Data[2],2)+' '+inttohex(databuf[i+1].Data[3],2)+' '+inttohex(databuf[i+1].Data[4],2)+' '+inttohex(databuf[i+1].Data[5],2)+' '+inttohex(databuf[i+1].Data[6],2)+' '+inttohex(databuf[i+1].Data[7],2);//          kk+' 11 22 33 44 55 66 77';
        Cells[9,RowNumber] := '接收成功';
      end;
      RowNumber := RowNumber + 1;
    end;



  end;






end.






