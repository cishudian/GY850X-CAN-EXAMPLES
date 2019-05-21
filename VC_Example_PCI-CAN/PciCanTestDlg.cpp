// PciCanTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PciCanTest.h"
#include "PciCanTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPciCanTestDlg dialog

CPciCanTestDlg::CPciCanTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPciCanTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPciCanTestDlg)
	m_strID = _T("");
	m_strData = _T("");
	m_bCallBack = FALSE;
	m_bQuery = FALSE;
	m_nSeq=0;
	m_hEvent=NULL;
	m_hRecieve=NULL;
	m_bIsFirstCallBack=FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPciCanTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPciCanTestDlg)
	DDX_Control(pDX, IDC_COMBO_CANINDEX, m_comboCanIndex);
	DDX_Control(pDX, IDC_LIST_SENDANDRECIEVE, m_listSR);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strData);
	DDX_Check(pDX, IDC_CHECK_CALLBACK, m_bCallBack);
	DDX_Check(pDX, IDC_CHECK_QUERY, m_bQuery);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPciCanTestDlg, CDialog)
	//{{AFX_MSG_MAP(CPciCanTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_CHECK_CALLBACK, OnCheckCallback)
	ON_BN_CLICKED(IDC_CHECK_QUERY, OnCheckQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPciCanTestDlg message handlers

BOOL CPciCanTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//设置显示列表
	LONG lStyle = m_listSR.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_listSR.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)lStyle);
	
	m_listSR.InsertColumn(0,"序号");
	m_listSR.SetColumnWidth(0,40);
	m_listSR.InsertColumn(1,"时间");
	m_listSR.SetColumnWidth(1,85);
	m_listSR.InsertColumn(2,"CAN通道");
	m_listSR.SetColumnWidth(2,60);
	m_listSR.InsertColumn(3,"传输方向");
	m_listSR.SetColumnWidth(3,80);
	m_listSR.InsertColumn(4,"ID号");
	m_listSR.SetColumnWidth(4,60);
	m_listSR.InsertColumn(5,"帧类型");
	m_listSR.SetColumnWidth(5,50);
	m_listSR.InsertColumn(6,"帧格式");
	m_listSR.SetColumnWidth(6,60);
	m_listSR.InsertColumn(7,"数据长度");
	m_listSR.SetColumnWidth(7,60);
	m_listSR.InsertColumn(8,"数据");
	m_listSR.SetColumnWidth(8,160);
	m_listSR.InsertColumn(9," ");
	m_listSR.SetColumnWidth(9,300);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//初始化发送数据以及ID
	m_strData="11 22 33 44 55 66 77 88";
	m_strID="00 00 00 01";
	m_comboCanIndex.SetCurSel(0);
	UpdateData(FALSE);
	DWORD	Error;
	//打开设备
	m_hDevice=OpenDevice(DEV_PCICAN,0,Error);
	if (m_hDevice<=0)
	{
		AfxMessageBox("Open Fail!");
		//return FALSE;
	}
//	else
//	{
//		AfxMessageBox("Device found, handle open.\n");
//	}
	//初始化SJA1000
	if(!InitializeCan())
		AfxMessageBox("Initialize SJA1000 Fail!");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPciCanTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPciCanTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPciCanTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/***************************************************************************
函数名称:OpenDevice(DWORD DeviceType,DWORD instance,DWORD Error)
函数描述:打开设备
函数调用:VCI_OpenDevice
全局变量:无
入参:DWORD DeviceType设备类型,DWORD instance设备序号,DWORD Error返回错误代码（赞不用）
输出:设备句柄
***************************************************************************/
HANDLE CPciCanTestDlg::OpenDevice(DWORD DeviceType,DWORD instance,DWORD Error)
{
	//DWORD Error;
	HANDLE hDevice=(HANDLE)VCI_OpenDevice(DeviceType,instance,Error);
	
	if (hDevice == INVALID_HANDLE_VALUE)
		Error = GetLastError();
	
	return hDevice;
	
}

/***************************************************************************
函数名称:InitializeCan()
函数描述:初始化两路CAN通道，即初始化SJA1000
函数调用:VCI_InitCAN
全局变量:无
入参:无
输出:无
***************************************************************************/
BOOL CPciCanTestDlg::InitializeCan()
{
	VCI_INIT_CONFIG *pConfig=new VCI_INIT_CONFIG;
	pConfig->AccCode=0;
	pConfig->AccMask=0xffffffff;
	pConfig->Mode=0x00;
	pConfig->Filter=0x00;
	pConfig->Timing0=0x00;
	pConfig->Timing1=0x14;
    pConfig->CanRx_IER=1;
	//初始化通道1
	if(!VCI_InitCAN(DEV_PCICAN,0,0,pConfig))
		return FALSE;

	return TRUE;
}
/***************************************************************************
函数名称:OnButtonSend()
函数描述:发送数据
函数调用:Str2Hex()，VCI_Transmit
全局变量:无
入参:无
输出:无
***************************************************************************/
void CPciCanTestDlg::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	BYTE SendID[4];
	BYTE buf[10];
	
	UpdateData(TRUE);
	CString str5[32];
	CString strtemp,strtemp1;
	CString str1;
	int len,IDnum=0,newflag=1;
	int datanum=0;
	int flagsend=0;
	len=m_strID.GetLength();	//从对话框获取ID信息
	for(int i=0; i<len;i++)
	{
		strtemp=m_strID.GetAt(i);
		if(strtemp==" ")
			newflag=1;
		else if(newflag==1)
		{
			newflag=0;
			strtemp=m_strID.GetAt(i);
			if(i==(len-1))
			{
				str5[IDnum]="0"+strtemp;
				
			}
			else
			{
				strtemp1=m_strID.GetAt(i+1);
				
				if(strtemp1 == " ")
					str5[IDnum]="0"+strtemp;
				else
					str5[IDnum]=strtemp+strtemp1;
			}
			
			SendID[IDnum]=Str2Hex(str5[IDnum]);
			IDnum++;
			if(IDnum>=4)	
				break;
			
		}
		
	}
	
	newflag=1;
	len=m_strData.GetLength();//从对话框获取数据信息的字节
	for(i=0; i<len;i++)
	{
		
		strtemp=m_strData.GetAt(i);
		if(strtemp==" ")
			newflag=1;
		else if(newflag==1)
		{	
			
			newflag=0;
			strtemp=m_strData.GetAt(i);		
			if(i==(len-1))
			{
				str5[datanum]="0"+strtemp;
				
			}
			else
			{
				strtemp1=m_strData.GetAt(i+1);
				
				if(strtemp1 == " ")
				{
					str5[datanum]="0"+strtemp;
					
				}
				else
					str5[datanum]=strtemp+strtemp1;
				
			}
			buf[datanum]=Str2Hex(str5[datanum]);
			datanum++;
			if(datanum>=8)
				break;
			
		}
		
	}
	DWORD SendIDValue;
	SendIDValue=SendID[0];
	SendIDValue<<=8;
	SendIDValue+=SendID[1];
	SendIDValue<<=8;
	SendIDValue+=SendID[2];
	SendIDValue<<=8;
	SendIDValue+=SendID[3];
	VCI_CAN_OBJ *pSend=new VCI_CAN_OBJ;
	pSend->DataLen=datanum;
	pSend->ExternFlag=0;//是标准帧
	pSend->ID[0]=SendIDValue>>3;
	pSend->ID[1]=SendIDValue<<5;
	//pSend->kCanIndex=1;
	pSend->RemoteFlag=0;
	for(i=0;i<datanum;i++)
	{
		pSend->Data[i]=buf[i];
	}
	//调用动态库函数发送
	if(!VCI_Transmit(DEV_PCICAN,0,0,pSend))
		MessageBox("Send fail");
}
int CPciCanTestDlg::HexChar(char c)
{
	if((c>='0') && (c<='9'))
		return c-0x30;
	else if((c>='A') && (c<='F'))
		return c-'A'+10;
	else if((c>='a') && (c<='f'))
		return c-'a'+10;
	else
		return 0x10;
}
int CPciCanTestDlg::Str2Hex(CString str)
{	
	int len = str.GetLength();
	if(len == 2)
	{
		int a= HexChar(str[0]);
		int b =HexChar(str[1]);
		if(a==16 || b==16 )
		{
			AfxMessageBox("Input format error!");
			return 256;
		}
		else
		{
			return a*16+b;
			
		}
		
	}
	else
	{
		AfxMessageBox("Input len need to be 2!");
		return 256;
	}
}
/***************************************************************************
函数名称:OnButtonClear()
函数描述:清除接收信息列表
函数调用:
全局变量:无
入参:无
输出:无
***************************************************************************/
void CPciCanTestDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_listSR.DeleteAllItems();
	m_nSeq=0;
}
/***************************************************************************
函数名称:RecieveCallBack()
函数描述:中断回调函数,
函数调用:CPciCanTestDlg::CanReceiveShow()
全局变量:
入参:无
输出:dDeviceIndex设备序号，dwCanIndex通道号，pPciCanRecieve接收信息结构体，
lpUser用户数据
***************************************************************************/
void RecieveCallBack(DWORD dDeviceIndex,DWORD dwCanIndex,VCI_CAN_OBJ * pPciCanRecieve,LPVOID lpUser)
{
	CPciCanTestDlg *pThis=(CPciCanTestDlg *)lpUser;
	if(pThis->m_bCallBack)
	{
		//显示

	    pThis->CanReceiveShow(dwCanIndex,pPciCanRecieve,1);
		//Sleep(200);
	}	
}
/***************************************************************************
函数名称:OnCheckCallback()
函数描述:使用中断回调方式接收数据
函数调用:VCI_CANSetCallBack(),VCI_CANStopCallBack(),VCI_CANStartCallBack
全局变量:m_bIsFirstCallBack,m_bCallBack
入参:无
输出:无
***************************************************************************/
void CPciCanTestDlg::OnCheckCallback() 
{
	// TODO: Add your control notification handler code here
	//设置中断回调接收
	if(!m_bIsFirstCallBack)
	{
		if(!VCI_CANSetCallBack(0,RecieveCallBack,this))
		AfxMessageBox("Set Call Back Fail!");
		m_bIsFirstCallBack=TRUE;
	}
	//暂停中断回调接收
	if(m_bCallBack)
	{
		m_bCallBack=FALSE;
		if(!VCI_CANStopCallBack(0))
			AfxMessageBox("Stop Callback Fail!");

	}
	//开始中断回调接收
	else
	{
		m_bCallBack=TRUE;
		if(!VCI_CANStartCallBack(0))
			AfxMessageBox("Start Callback Fail!");
	}
}
/***************************************************************************
函数名称:RecieveThread()
函数描述:查询方式接收数据线程
函数调用:
全局变量:
入参:无
输出:无
***************************************************************************/
DWORD WINAPI RecieveThread(LPVOID lpParameter)
{
	CPciCanTestDlg *pThis=(CPciCanTestDlg *)lpParameter;
	return pThis->Recieve();
    
}
/***************************************************************************
函数名称:OnCheckQuery()
函数描述:使用查询方式接收数据
函数调用:
全局变量:m_bQuery，m_hEvent，m_hRecieve
入参:无
输出:无
***************************************************************************/
void CPciCanTestDlg::OnCheckQuery() 
{
	// TODO: Add your control notification handler code here
	if(m_bQuery)
	{
		m_bQuery=FALSE;
		ResetEvent(m_hEvent);
	}
	else
	{
		m_bQuery=TRUE;
		SetEvent(m_hEvent);			
	}
	if(m_hEvent==NULL)
	m_hEvent=CreateEvent(NULL,TRUE,TRUE,NULL);
	//创建查询接收线程
	if(m_hRecieve==NULL)
	m_hRecieve=CreateThread(NULL,0,RecieveThread,this,NULL,NULL);
	
}
/***************************************************************************
函数名称:Recieve()
函数描述:处理查询方式所接收数据
函数调用:
全局变量:
入参:无
输出:无
***************************************************************************/
DWORD CPciCanTestDlg::Recieve()
{
	CHAR	bufInput[512];		// Input to device
	CHAR	bufOutput[512];	// Output from device
	ULONG	nOutput=0;
	VCI_CAN_OBJ *pRcv=new VCI_CAN_OBJ[200];
	CString strTemp;
	DWORD dCanindex=0;
	DWORD dLength;
	while(1)
	{

		Sleep(200);
		WaitForSingleObject(m_hEvent,INFINITE);//一直等待
		for(dCanindex=0;dCanindex<2;dCanindex++)
		{
			//接收
			dLength=VCI_Receive(DEV_PCICAN,0,0,pRcv);
			if(dLength==-1)
				AfxMessageBox("Recieve Fail!");
			if(dLength>0)
			{
				//显示
				CanReceiveShow(dCanindex,pRcv,dLength);

			}
		}

	}
	return nOutput;
}
/***************************************************************************
函数名称:CanReceiveShow()
函数描述:接收数据显示
函数调用:
全局变量:
入参:CANInd通道号，pCanObj数据信息指针，dLength数据指针长度
输出:无
***************************************************************************/
void CPciCanTestDlg::CanReceiveShow(DWORD CANInd,PVCI_CAN_OBJ pCanObj,DWORD dLength)
{
	//VCI_CAN_OBJ pCanObj[250];
	CString strbuf[200],str1;
	//int num=0;
    int i;		
	CSize size;
	unsigned int JustnowItem;
	BYTE ReceivedID[4];
	size.cx=0;
	size.cy=50; 
	CString str;
	CString strTime;
	SYSTEMTIME   systime;   
	GetLocalTime(&systime);   
	strTime.Format("%02d:%02d:%02d:%03d", systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds);   
				
	for(int num=0;num<dLength;num++)
	{
		
	
		str.Format("%d",m_nSeq);
		JustnowItem=m_listSR.InsertItem(m_nSeq,str);	//插入行，顺序号
		m_nSeq++;	
		
		m_listSR.SetItemText(JustnowItem,1,strTime);		//显示位置定义
		
		str.Format("%d",CANInd);
		m_listSR.SetItemText(JustnowItem,2,str);	//CAN index
		m_listSR.SetItemText(JustnowItem,3,"接收");		//显示位置定义
		str="";
		
		if((pCanObj[num].RemoteFlag)==1)
			m_listSR.SetItemText(JustnowItem,5,"远程帧");	
		else
			m_listSR.SetItemText(JustnowItem,5,"数据帧");	
		if((pCanObj[num].ExternFlag)==1)			//扩展帧
		{
			for(i=0;i<4;i++)
				ReceivedID[i]=pCanObj[num].ID[i];
			
		
				ReceivedID[3]=(ReceivedID[2]<<5)|(ReceivedID[3]>>3);
				ReceivedID[2]=(ReceivedID[1]<<5)|(ReceivedID[2]>>3);
				ReceivedID[1]=(ReceivedID[0]<<5)|(ReceivedID[1]>>3);
				ReceivedID[0]=ReceivedID[0]>>3;
			
			
			for(i=0;i<4;i++)
			{
				str1.Format("%02X",ReceivedID[i]);
				str+=str1;
			}
			m_listSR.SetItemText(JustnowItem,4,str);	//ID信息						
			m_listSR.SetItemText(JustnowItem,6,"扩展帧");		//						
		}
		else									//标准帧
		{
			for(i=0;i<2;i++)
				ReceivedID[i]=pCanObj[num].ID[i];
			
	
				ReceivedID[1]=(ReceivedID[1]>>5)|(ReceivedID[0]<<3);
				ReceivedID[0]=ReceivedID[0]>>5;
			
			
			for(i=0;i<2;i++)
			{
				str1.Format("%02X",ReceivedID[i]);
				str+=str1;
			}
			m_listSR.SetItemText(JustnowItem,4,str);	//ID信息	
			
			m_listSR.SetItemText(JustnowItem,6,"标准帧");		//
			
			
		}
		str.Format("%d",pCanObj[num].DataLen);	//长度信息
		m_listSR.SetItemText(JustnowItem,7,str);	
		
		str="";
		for(i=0;i<(pCanObj[num].DataLen);i++)			//数据信息
		{
			str1.Format("%02X ",pCanObj[num].Data[i]);
			str+=str1;
			
		}
		m_listSR.SetItemText(JustnowItem,8,str);	
		m_listSR.Scroll(size); 		
	}			
}
/***************************************************************************
函数名称:OnCancel()
函数描述:程序退出关闭接收线程
函数调用:
全局变量:m_hRecieve，m_bIsFirstCallBack
入参:
输出:无
***************************************************************************/
void CPciCanTestDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DWORD dw=0;
	//若查询接收则退出
	if(m_hRecieve)
	{
		TerminateThread(m_hRecieve,dw);
		CloseHandle(m_hRecieve);
	}
	//若打开回调则退出
	if(m_bIsFirstCallBack)
	{
		if(!VCI_CANExitCallBack(0))
		{
			AfxMessageBox("Exit Callback Fail!");
		}
	}
	if(m_hRecieve>0)
	{
		if(!VCI_CloseDevice(DEV_PCICAN,0))
			AfxMessageBox("Close fail!");
	}

	CDialog::OnCancel();
}
