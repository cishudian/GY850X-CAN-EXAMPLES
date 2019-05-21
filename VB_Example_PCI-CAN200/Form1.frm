VERSION 5.00
Begin VB.Form DeviceNet 
   Caption         =   "VB Example  GY8508 USB-CAN200 Test"
   ClientHeight    =   6945
   ClientLeft      =   2490
   ClientTop       =   1740
   ClientWidth     =   11115
   LinkTopic       =   "Form1"
   ScaleHeight     =   6945
   ScaleWidth      =   11115
   Begin VB.CommandButton Command1 
      Caption         =   "Close"
      Height          =   375
      Left            =   8400
      TabIndex        =   5
      Top             =   360
      Width           =   1215
   End
   Begin VB.CommandButton CAN_RECEIVE 
      Caption         =   "CAN0 Receive"
      Height          =   375
      Left            =   6360
      TabIndex        =   4
      Top             =   360
      Width           =   1455
   End
   Begin VB.CommandButton CAN_SEND 
      Caption         =   "CAN1 Send"
      Height          =   375
      Left            =   4560
      TabIndex        =   3
      Top             =   360
      Width           =   1335
   End
   Begin VB.CommandButton BUTTON_INIT_CAN 
      Caption         =   "InitCan"
      Height          =   375
      Left            =   2880
      TabIndex        =   2
      Top             =   360
      Width           =   1215
   End
   Begin VB.ListBox LIST_INFO 
      Height          =   5460
      Left            =   720
      TabIndex        =   1
      Top             =   1200
      Width           =   9855
   End
   Begin VB.CommandButton BUTTON_OPEN_DEVICE 
      Caption         =   "Open Device"
      Height          =   375
      Left            =   840
      TabIndex        =   0
      Top             =   360
      Width           =   1215
   End
   Begin VB.Menu 开始 
      Caption         =   "开始"
      Index           =   1
      WindowList      =   -1  'True
   End
End
Attribute VB_Name = "DeviceNet"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Sub BUTTON_INIT_CAN_Click()
    Dim str1$, i%
    Dim temp As Long
    str1 = Format(Now, "yyyy-mm-dd  hh:mm:ss") + vbTab
     Dim pInitCan As PVCI_INIT_CONFIG

    pInitCan.Mode = 1  ' 0~正常模式， 1~自收发模式
    pInitCan.Filter = 0
    pInitCan.kCanBaud = 15 '1000kbps
    pInitCan.Timing0 = &H1  '0x00
    pInitCan.Timing1 = &H14 '0x14
    pInitCan.AccCode = 1
    pInitCan.AccMask = -1 '0xFFFFFFFF
    pInitCan.CanRx_IER = 1 'Eanble CAN receive
    
    temp = VCI_InitCAN(7, 0, 0, pInitCan) '初始化0通道
    
 
    temp = VCI_InitCAN(7, 0, 1, pInitCan) '初始化0通道
    
  '  temp = VCI_StartCAN(3, 0, 0)  '初始化0通道
 '
  '  temp = VCI_StartCAN(3, 0, 1) '初始化0通道
      If temp = 1 Then
       str1 = str1 + "successful to Init device"
    Else
       str1 = str1 + "fail to Init device"
    End If
    
     i = LIST_INFO.ListCount
     LIST_INFO.List(i) = str1

    LIST_INFO.Enabled = False
End Sub

Private Sub BUTTON_OPEN_DEVICE_Click()

    Dim str1$, i%
    Dim temp As Long
    str1 = Format(Now, "yyyy-mm-dd  hh:mm:ss") + vbTab
    temp = VCI_OpenDevice(7, 0, 0)
    If temp = 1 Then
       str1 = str1 + "successful to open device"
    Else
       str1 = str1 + "fail to open device"
    End If
    
     i = LIST_INFO.ListCount
     LIST_INFO.List(i) = str1

    LIST_INFO.Enabled = False
End Sub

Private Sub CAN_RECEIVE_Click()
Dim pReceiveData(0 To 200) As PVCI_CAN_OBJ
Dim num%, n%, i%, j%, DL%
Dim str1$, str$
Dim IDByte(0 To 3) As Byte

num = VCI_Receive(7, 0, 0, pReceiveData(0)) 'CAN0通道

 For n = 0 To (num - 1)
        str = Format(Now, "yyyy-mm-dd  hh:mm:ss") + vbTab + "CAN0 Receive" + "    "
        For i = 0 To 3
            IDByte(3 - i) = pReceiveData(n).ID(i)
        Next i
        str = str + "ID: " + Hex(IDByte(3)) + " " + Hex(IDByte(2)) + " " + Hex(IDByte(1)) + " " + Hex(IDByte(0)) + "  "
        DL = pReceiveData(n).DataLen
        str = str + "DLen:" + Hex(DL) + "  DATA: "
        For i = 0 To DL - 1
          str1 = Hex(pReceiveData(n).Data(i)) + " "
           str = str + str1
        Next i
        j = LIST_INFO.ListCount
        LIST_INFO.List(j) = str
    Next n
    
num = VCI_Receive(7, 0, 1, pReceiveData(0)) 'CAN01通道

 For n = 0 To (num - 1)
        str = Format(Now, "yyyy-mm-dd  hh:mm:ss") + vbTab + "CAN1 Receive" + "    "
        For i = 0 To 3
            IDByte(3 - i) = pReceiveData(n).ID(i)
        Next i
        str = str + "ID: " + Hex(IDByte(3)) + " " + Hex(IDByte(2)) + " " + Hex(IDByte(1)) + " " + Hex(IDByte(0)) + "  "
        DL = pReceiveData(n).DataLen
        str = str + "DLen:" + Hex(DL) + "  DATA: "
        For i = 0 To DL - 1
          str1 = Hex(pReceiveData(n).Data(i)) + " "
           str = str + str1
        Next i
        j = LIST_INFO.ListCount
        LIST_INFO.List(j) = str
    Next n


End Sub

Private Sub CAN_SEND_Click()

Dim pSend As PVCI_CAN_OBJ
Dim num%, n%, i%, j%, DL%
Dim str1$, str$
Dim IDByte(0 To 3) As Byte

pSend.ExternFlag = 1
pSend.RemoteFlag = 0
pSend.ID(0) = 0
pSend.ID(1) = 112
pSend.ID(2) = 0
pSend.ID(3) = 0
pSend.DataLen = 2
pSend.Data(0) = 32
pSend.Data(1) = 33
pSend.Data(2) = 34
pSend.Data(3) = 35
pSend.Data(4) = 36
pSend.Data(5) = 37
pSend.Data(6) = 38
pSend.Data(7) = 39
num = VCI_Transmit(7, 0, 1, pSend)  'send CAN1通道
i = LIST_INFO.ListCount
If num <> 0 Then
        str = Format(Now, "yyyy-mm-dd  hh:mm:ss") + vbTab + "CAN1 Send Ok: " + "  "
        For i = 0 To 3
            IDByte(3 - i) = pSend.ID(i)
        Next i
        str = str + "ID: " + Hex(IDByte(3)) + " " + Hex(IDByte(2)) + " " + Hex(IDByte(1)) + " " + Hex(IDByte(0)) + "  "
        DL = pSend.DataLen
        str = str + "DLen:" + Hex(DL) + "  DATA: "
        For i = 0 To DL - 1
          str1 = Hex(pSend.Data(i)) + " "
           str = str + str1
        Next i
        j = LIST_INFO.ListCount
        LIST_INFO.List(j) = str
     
Else
    LIST_INFO.List(i) = str1 + "CAN1 Send Error"
End If

pSend.ID(0) = 0
pSend.ID(1) = 113
pSend.ID(2) = 0
pSend.ID(3) = 0
pSend.DataLen = 8
pSend.Data(0) = 48
pSend.Data(1) = 49
pSend.Data(2) = 50
pSend.Data(3) = 51
pSend.Data(4) = 52
pSend.Data(5) = 53
pSend.Data(6) = 54
pSend.Data(7) = 55
num = VCI_Transmit(7, 0, 1, pSend) 'send CAN1通道
i = LIST_INFO.ListCount
If num <> 0 Then
        str = Format(Now, "yyyy-mm-dd  hh:mm:ss") + vbTab + "CAN1 Send Ok: " + "  "
        For i = 0 To 3
            IDByte(3 - i) = pSend.ID(i)
        Next i
        str = str + "ID: " + Hex(IDByte(3)) + " " + Hex(IDByte(2)) + " " + Hex(IDByte(1)) + " " + Hex(IDByte(0)) + "  "
        DL = pSend.DataLen
        str = str + "DLen:" + Hex(DL) + "  DATA: "
        For i = 0 To DL - 1
          str1 = Hex(pSend.Data(i)) + " "
           str = str + str1
        Next i
        j = LIST_INFO.ListCount
        LIST_INFO.List(j) = str
     
Else
    LIST_INFO.List(i) = str1 + "CAN1 Send Error"
End If


End Sub

Private Sub Command1_Click()
Dim temp As Integer
temp = VCI_CloseDevice(7, 0)
End Sub

Private Sub Form_Unload(Cancel As Integer)
Dim temp As Integer
temp = VCI_CloseDevice(7, 0)
    

End Sub

Private Sub 开始_Click(Index As Integer)
Load configer
Unload Me
End Sub
