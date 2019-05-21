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
  //  Outline,  FileCtrl,    ��ʱȡ��������������
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
    // �Զ���ĺ�������
    function OpenDevice(DeviceType,instance,Error:Integer):Boolean;
    function InitializeCan():Boolean;
    function OnButtonSend():Boolean;
    function CloseDevice(DeviceType,instance:Integer):Boolean;

  end;
var
  Mainform: TMainform;

// ����"��¼"����

//2.����CAN��Ϣ֡���������͡�
type
  VCI_CAN_OBJ = record  // ��¼��������VCI_CAN_OBJ
  ID : array[0..3] of Byte;  //CAN��Ϣ�е�ID��������ʽ��������ID=2,����������00 40 00 00
  TimeStamp:Integer;   // ��������������
  TimeFlag: Byte;   // ��������������
  SendType: Byte;   // ��������������
  RemoteFlag: Byte; // ѡ���Ƿ�Զ��֡��1 ��ʾԶ��֡��0 ��ʾ����֡
  ExternFlag: Byte; // ѡ���Ƿ���չ֡��1 ��ʾ��չ֡��0 ��ʾ��׼֡
  DataLen:  Byte;   // ��Ч�����ֽڳ��ȣ�(<=8)
  Data : array[0..7] of Byte;  // CAN ��Ϣ�е����ݻ�������
  Reserved : array[0..2] of BYTE;    // ��������
end;
PVCI_CAN_OBJ =^ VCI_CAN_OBJ;    // ����ָ�����ͣ�PVCI_CAN_OBJ


// 5.�����ʼ��CAN����������
type
  VCI_INIT_CONFIG = record // ��¼��������VCI_INIT_CONFIG
  AccCode: Integer;       // ���������롣�����
  AccMask: Integer;       // ���������롣�����
  Reserved : Integer;     // ��������
  Filter : Byte;          // ���˷�ʽ��0 ��ʾ���˲���1 ��ʾ˫�˲�
  kCanBaud : Byte;        // CAN ������������
  Timing0 : Byte;         // CAN �����ʶ�ʱ��BTR0
  Timing1 : Byte;         // CAN �����ʶ�ʱ��BTR1
  Mode : Byte;            // ����ģʽ��0 ��ʾ����ģʽ��1 ��ʾ�Է�����ģʽ��
  CanRx_IER : Byte;       // ??
end;
PVCI_INIT_CONFIG =^ VCI_INIT_CONFIG;    // ����ָ�����ͣ�PVCI_INIT_CONFIG

// ������ȫ�ֱ����Ķ��壺
var m_hDevice: integer = 0; // ���豸����ֵ��0��fail; 1:Successful; -1:device not open or error
var m_bIsOpen: Boolean = False; // ��ʾ�豸״̬: True: �豸�Ѵ�
var RowNumber: integer = 1; //  StringGrid�Ļ�к�
var m_bIsFirstCallBack: Boolean;
var m_bCallBack: Boolean;
// ȫ�ֱ����������


// �ӿڿ����Ͷ���  
const DEV_CAN232B   = 1;
const DEV_USBCAN    = 2;
const DEV_USBCAN200 = 3;
const DEV_NETCAN    = 4;
const DEV_NETCAN200 = 5;
const DEV_PCICAN    = 6;
const DEV_PCICAN200 = 7;

// ת����������ַ��

//      ��������       ��ַ                 ��  ��

const REFTYPE_MODE   =	0;  // ����ģʽ��0 ��ʾ����ģʽ��1 ��ʾ�Բ�ģʽ
const REFTYPE_FILTER =	1;  //  �˲���ʽ��0 ��ʾ���˲���1 ��ʾ˫�˲�
const REFTYPE_ACR0	 =	2;  //  ���������롣
const REFTYPE_ACR1	 =	3;  //  ���������롣
const REFTYPE_ACR2   =	4;  //  ���������롣
const REFTYPE_ACR3	 =	5;  //  ���������롣
const REFTYPE_AMR0	 =	6;  //  ���������롣
const REFTYPE_AMR1	 =	7;  //  ���������롣
const REFTYPE_AMR2	 =	8;  //  ���������롣
const REFTYPE_AMR3   =	9;  //  ���������롣
const REFTYPE_kCANBAUD =	10;  //  �˲���ֻ���������Բ�����������Ӱ��
const REFTYPE_TIMING0	 =	11;  //  �����ʶ�ʱ��BTR0
const REFTYPE_TIMING1	 =	12;  //  �����ʶ�ʱ��BTR1
const REFTYPE_CANRX_EN =  13;  //  �ӿڿ��ڲ����жϽ����Ƿ�����Ĭ��ֵΪ1������
const REFTYPE_UARTBAUD =	14;  //  ���ڲ�����ѡ�񡣽�����CAN232B
const REFTYPE_ALL		   =	15;  //  ���в���
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

// VCI_CAN.dll�еĺ�������
// �˺����û������豸
function VCI_OpenDevice(DeviceType:Integer;DeviceInd:Integer;Reserved:Integer):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_OpenDevice';
// �˺������ڳ�ʼ��CAN�ӿں���
function VCI_InitCAN(DeviceType:Integer;DeviceInd:Integer;CANInd:Integer;pInitConfig:PVCI_INIT_CONFIG):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_InitCAN';
// �˺�������CAN��Ϣ֡����
function VCI_Transmit(DeviceType:Integer;DeviceInd:Integer;CANInd:Integer;pSend:PVCI_CAN_OBJ):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_Transmit';
// �˺����������ý�����Ϣ
function VCI_Receive(DeviceType:Integer;DeviceInd:Integer;CANInd:Integer;pReceive:PVCI_CAN_OBJ):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_Receive';


// �˺����û��ر��豸
function VCI_CloseDevice(DeviceType:Integer;DeviceInd:Integer):
              Integer;stdcall;external 'VCI_CAN.dll' name 'VCI_CloseDevice';
              
{$R *.dfm}

//*********************************************************************
//  �������� : OpenDevice(DeviceType,instance,Error:Integer)
//  �������� : ���豸
//  �������� :VCI_OpenDevice
//  ȫ�ֱ��� : m_bIsOpen
//  ���: DeviceType�豸����,instance�豸���,Error���ش�����루ռ���ã�
//  ���: result:Boolean
//********************************************************************/
function TMainform.OpenDevice(DeviceType,instance,Error:Integer):Boolean;
begin
  // ���豸
  m_hDevice := VCI_OpenDevice(DEV_PCICAN200,0,0);
  // �ж��豸���Ƿ�ɹ�
  if m_hDevice = 1 then
	begin
    MessageDlg('��PCI_CAN���ɹ���',mtInformation,[mbOK],0);
		m_bIsOpen := True;
    result := True;
	end
  else
  begin
    MessageDlg('��PCI_CAN��ʧ�ܣ�',mtError,[mbOK],0);
		m_bIsOpen := False;
    result := False;
  end;
end;

//*****************************************************
//  ��������:InitializeCan()
//  ��������:��ʼ����·CANͨ��������ʼ��SJA1000
//  ��������:VCI_InitCAN
//  ȫ�ֱ���:m_bIsOpen
//  ���:��
//  ���:result:Boolean
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
    MessageDlg('CAN�豸��ʼ��ʧ�ܣ�',mtError,[mbOK],0);
    m_bIsOpen := False;
  end;
end;

//*********************************************************
//  ��������:OnButtonSend()
//  ��������:��������
//  ��������:Str2Hex()��VCI_Transmit
//  ȫ�ֱ���:��
//  ���:��
//  ���:result:Boolean
//*********************************************************/
function TMainform.OnButtonSend():Boolean;
var  NewVCI_CAN_OBJ :VCI_CAN_OBJ;
var  pSend : PVCI_CAN_OBJ;
var  i,j : integer;
begin
  result := True;
  // �ж��豸�Ƿ����
  if m_bIsOpen = False then
  begin
     MessageDlg('CAN�豸δ�򿪻��ʼ��ʧ�ܣ�',mtError,[mbOK],0);
     result := False;
  end
  else
  begin
    pSend := @NewVCI_CAN_OBJ; // ��ַ��ֵ
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
      MessageDlg('CAN��Ϣ����ʧ��',mtError,[mbOK],0);
      result := False;
    end
    else
    begin
      with StringGrid1 do
      begin
        Cells[0,RowNumber] := IntToStr(RowNumber);
        Cells[1,RowNumber] := FormatDateTime('hh:mm:ss',time);
        Cells[2,RowNumber] := '0';
        Cells[3,RowNumber] := '����';
        Cells[4,RowNumber] := '0001';
        Cells[5,RowNumber] := '����֡';
        Cells[6,RowNumber] := '��׼֡';
        Cells[7,RowNumber] := '8';
        Cells[8,RowNumber] := '00 11 22 33 44 55 66 77';
        Cells[9,RowNumber] := '���ͳɹ�';
      end;
      RowNumber := RowNumber + 1;
    end;
  end;
end;

//*****************************************************
//  ��������:CloseDevice(DeviceType,instance:Integer)
//  ��������:�ر��豸
//  ��������:VCI_CloseDevice
//  ȫ�ֱ���:m_bIsOpen
//  ���:DeviceType,instance
//  ���:result:Boolean
//*****************************************************/
function TMainform.CloseDevice(DeviceType,instance:Integer):Boolean;
var i:integer;
begin
  result := True;
  if m_bIsOpen = False then
  begin
    MessageDlg('�豸�ر�ʧ�ܣ�ԭ���豸δ�򿪣�',mtError,[mbOK],0);
  end
  else
  begin
    i := VCI_CloseDevice(DeviceType,instance);
    // �ж��豸�ر��Ƿ�ɹ�
    if i = 1 then
	  begin
      MessageDlg('PCI_CAN���رճɹ���',mtInformation,[mbOK],0);
		  m_bIsOpen := False;
      result := True;
	  end
    else
    begin
      MessageDlg('PCI_CAN���ر�ʧ�ܣ�',mtError,[mbOK],0);
		  result := False;
    end;
  end;
end;

// �˳�ϵͳʱ��ѯ���Ƿ�ر�
procedure TMainform.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
var ret:integer;
var STR1,STR2:PansiChar;
begin
  STR1 := '   ȷ���˳�CAN����ͨѶ���Գ���    ';
  STR2 := 'CAN����ͨѶ���Գ���';
  ret :=MessageBox(handle,STR1,STR2,MB_YESNO);
  if ret = IDYES then
    Canclose := True
  else
    Canclose := False;
end;

// ״̬���������ڣ�ʱ��
procedure TMainform.Timer1Timer(Sender: TObject);
begin
  with StatusBar1 do
  begin
    //��ʾʱ��
    Panels[0].Text := Concat(DateToStr(Date),'        ',
        FormatDateTime('hh:mm:ss',time));
    //��ʾ���� �� ϵͳ��
    Panels[1].Text := Concat('CAN����ͨѶ���Գ���',
        '          ','Version 1.0', '   ','(All Rights Reserved)');
  end;
end;

// �����彨���¼�
procedure TMainform.FormCreate(Sender: TObject);
begin
//  StringGrid1.EditorMode := False;
  with StringGrid1 do
  begin
    Cells[0,0]:='���';
    Cells[1,0]:='ʱ��';
    Cells[2,0]:='CANͨ��';
    Cells[3,0]:='���䷽��';
    Cells[4,0]:='ID��';
    Cells[5,0]:='֡����';
    Cells[6,0]:='֡��ʽ';
    Cells[7,0]:='���ݳ���';
    Cells[8,0]:='����';
    Cells[9,0]:='��ע';
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

// �˵������豸���������豸
procedure TMainform.N23Click(Sender: TObject);
var i:Boolean;
begin
  // ��PCI_CANA��
  i := OpenDevice(DEV_PCICAN200,0,0); 
  if i = True then
  begin
    //��ʼ��SJA1000
    initializeCan();
  end;
end; 

// �����壺�����͡���ť
procedure TMainform.BitBtn1Click(Sender: TObject);
begin
  OnButtonSend();
end;

// �˵������豸�������ر��豸
procedure TMainform.N24Click(Sender: TObject);
begin
  CloseDevice(DEV_PCICAN200,0);
end;

// �����壺�����հ�ť��
procedure TMainform.BitBtn2Click(Sender: TObject);
var i : integer;
var kk:string;
   var databuf : array[1..300] of VCI_CAN_OBJ;
  var pReceive : PVCI_CAN_OBJ;
  var value : integer;
  begin
    pReceive := @databuf[1]; // ��ַ��ֵ
    value := VCI_Receive(DEV_PCICAN200,0,0,pReceive);
    for i:=0 to (value-1) do

      begin
      with StringGrid1 do
      begin
        Cells[0,RowNumber] := IntToStr(RowNumber);
        Cells[1,RowNumber] := FormatDateTime('hh:mm:ss',time);
        Cells[2,RowNumber] := '0';
        Cells[3,RowNumber] := '����';
        Cells[4,RowNumber] :=inttohex(databuf[i+1].ID[0],2)+' '+inttohex(databuf[i+1].ID[1],2)+' '+inttohex(databuf[i+1].ID[2],2)+' '+inttohex(databuf[i+1].ID[3],2);
         if (databuf[i+1].ExternFlag = 1)  then
          begin  Cells[5,RowNumber] := 'Զ��֡'; end
        else
         begin Cells[5,RowNumber] := '����֡';   end;
 
        if (databuf[i+1].ExternFlag = 1)  then
          begin  Cells[6,RowNumber] := '��չ֡'; end
        else
         begin Cells[6,RowNumber] := '��׼֡';   end;
       // kk:=  inttohex(databuf[i+1].Data[0],2);
        Cells[7,RowNumber] := inttohex(databuf[i+1].DataLen,2);
        Cells[8,RowNumber] :=inttohex(databuf[i+1].Data[0],2)+' '+inttohex(databuf[i+1].Data[1],2)+' '+inttohex(databuf[i+1].Data[2],2)+' '+inttohex(databuf[i+1].Data[3],2)+' '+inttohex(databuf[i+1].Data[4],2)+' '+inttohex(databuf[i+1].Data[5],2)+' '+inttohex(databuf[i+1].Data[6],2)+' '+inttohex(databuf[i+1].Data[7],2);//          kk+' 11 22 33 44 55 66 77';
        Cells[9,RowNumber] := '���ճɹ�';
      end;
      RowNumber := RowNumber + 1;
    end;



  end;






end.






