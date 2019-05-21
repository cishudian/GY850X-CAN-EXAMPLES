Attribute VB_Name = "Module1"

Option Explicit

Public Type PVCI_INIT_CONFIG    'for Init CAN configuration
 AccCode As Long       'Accept code
 AccMask As Long       'Mask code
 Reserved As Long      'reserved
 Filter As Byte        'filter
 kCanBaud As Byte      '
 Timing0 As Byte       'Timig0 for Baud rate setting BTR0
 Timing1 As Byte       'Timig1 for Baud rate setting BTR1
 Mode As Byte          'Work Mode,0 normal,1 self-reception
 CanRx_IER As Byte 		 'Enable/Disable CAN Receive
End Type


Public Type PVCI_CAN_OBJ    'for VCI_Transmit ºÍVCI_Receive function
 ID(0 To 3) As Byte         'Message ID, 4 bytes
 TimeStamp As Long     'Not used, reserved
 TimeFlag As Byte      'Not used, reserved
 SendType As Byte      'Not used, reserved
 RemoteFlag As Byte    '1 Remote frame,0 data frame
 ExternFlag As Byte    '1 extended frame,0 basic frame
 DataLen As Byte       'data length, <=8
 Data(0 To 7) As Byte       'data buf
 Reserved(0 To 2) As Byte   'reserved
End Type




'Return value: 1 successful, 0 fail, -1 device not open
'Connect usb port to device
Public Declare Function VCI_OpenDevice Lib "VCI_CAN.dll" (ByVal DevType As Long, ByVal DevInd As Long, ByVal Reserved As Long) As Long

'Disconnet
Public Declare Function VCI_CloseDevice Lib "VCI_CAN.dll" (ByVal DevType As Long, ByVal DevInd As Long) As Long

'Init CAN parameters
Public Declare Function VCI_InitCAN Lib "VCI_CAN.dll" (ByVal DevType As Long, ByVal DevInd As Long, ByVal CANIndex As Long, ByRef pInitConfig As PVCI_INIT_CONFIG) As Long

'Enable CAN controller, and enable CAN receive intterupt of the device
Public Declare Function VCI_StartCAN Lib "VCI_CAN.dll" (ByVal DevType As Long, ByVal DevInd As Long, ByVal CANIndex As Long) As Long

'Reset CAN controller, and will disable CAN receive interrupt
Public Declare Function VCI_ResetCAN Lib "VCI_CAN.dll" (ByVal DevType As Long, ByVal DevInd As Long, ByVal CANIndex As Long) As Long

'Receive message from the R-buffer of the device.Returned the number of CAN frame messages
Public Declare Function VCI_Receive Lib "VCI_CAN.dll" (ByVal DevType As Long, ByVal DevInd As Long, ByVal CANIndex As Long, ByRef pReceive As PVCI_CAN_OBJ) As Long

'Send CAN messages
Public Declare Function VCI_Transmit Lib "VCI_CAN.dll" (ByVal DevType As Long, ByVal DevInd As Long, ByVal CANIndex As Long, ByRef pSend As PVCI_CAN_OBJ) As Long



