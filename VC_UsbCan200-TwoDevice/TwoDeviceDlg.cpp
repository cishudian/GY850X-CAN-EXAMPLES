// TwoDeviceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TwoDevice.h"
#include "TwoDeviceDlg.h"

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
// CTwoDeviceDlg dialog

CTwoDeviceDlg::CTwoDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTwoDeviceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTwoDeviceDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Seq=0;
}

void CTwoDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTwoDeviceDlg)
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Control(pDX, IDC_COMBO_ID, m_comboID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTwoDeviceDlg, CDialog)
	//{{AFX_MSG_MAP(CTwoDeviceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTwoDeviceDlg message handlers

BOOL CTwoDeviceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	//查找设备
	VCI_BOARD_INFO pData[1];	
	m_nDeiviceNum=VCI_FindGyUsbDevice(pData);	
	CString strtemp,str;
	for(int i=0;i<m_nDeiviceNum;i++)
	{
		
		str="";
		for(int j=0;j<4;j++)
		{
			strtemp.Format("%c",pData->str_GYUsb_Serial[i][j]);
			str+=strtemp;
		}
		m_ProductSn[i]=str;
		m_comboID.AddString(str);
	}
	m_comboID.SetCurSel(0);

	m_listData.InsertColumn(0,"序号",LVCFMT_CENTER,60);
	m_listData.InsertColumn(1,"方向",LVCFMT_CENTER,60);
	m_listData.InsertColumn(2,"设备序号",LVCFMT_CENTER,100);
	m_listData.InsertColumn(3,"数据",LVCFMT_CENTER,180);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTwoDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTwoDeviceDlg::OnPaint() 
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
HCURSOR CTwoDeviceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD WINAPI ReceiveThread(LPVOID lpParameter)
{
	CTwoDeviceDlg *pThis=(CTwoDeviceDlg *)lpParameter;
	return pThis->ReceiveProcess();
}


void CTwoDeviceDlg::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	DWORD nDevice;//=m_comboID.GetCurSel();
	for(nDevice=0;nDevice<m_nDeiviceNum;nDevice++)
	{
		DWORD Reserved=0;
		if(VCI_OpenDevice(DEV_USBCAN200,nDevice,Reserved)!=1)
		{
			MessageBox("open failed");
			return;		
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
	//	for(int i=0;i<nDevice;i++)
		{
			if(VCI_InitCAN(DEV_USBCAN200,nDevice, 0,InitInfo)!=1)	//can-0
			{
				MessageBox("Init-CAN failed!");
				return;
			}
			Sleep(100);
			
			if(VCI_StartCAN(DEV_USBCAN200,nDevice, 0)!=1)	//can-0
			{
				MessageBox("Start-CAN failed!");
				return;
			}
			if(VCI_InitCAN(DEV_USBCAN200,nDevice, 1,InitInfo)!=1)	//can-1
			{
				MessageBox("Init-CAN failed!");
				return;
			}
			Sleep(100);
			
			if(VCI_StartCAN(DEV_USBCAN200,nDevice, 1)!=1)	//can-1
			{
				MessageBox("Start-CAN failed!");
				return;
			}
		}

		


	}

	MessageBox("Open successfule!\n Start CAN OK!");
	CreateThread(0,0,ReceiveThread,this,0,0);
}

DWORD CTwoDeviceDlg::ReceiveProcess()
{
	while(1)
	{
		Sleep(20);
		for(int i=0;i<m_nDeiviceNum;i++)
		{
			int NumValue;		
			VCI_CAN_OBJ pCanObj[200];	
	        NumValue=VCI_Receive(DEV_USBCAN200,i,0,pCanObj);
			for(int j=0;j<NumValue;j++)
			{
				CString strSeq;
				strSeq.Format("%d",m_Seq);
				CString strDeviceNum;
				strDeviceNum.Format("%s(%d)-0CH",m_ProductSn[i],i);
				CString strData,strTemp;
				for(int k=0;k<pCanObj[j].DataLen;k++)
				{
					strTemp.Format("%02X ",pCanObj[j].Data[k]);
					strData+=strTemp;
				}
				m_listData.InsertItem(m_Seq,strSeq);
				m_listData.SetItemText(m_Seq,1,"接收");

				m_listData.SetItemText(m_Seq,2,strDeviceNum);
				m_listData.SetItemText(m_Seq,3,strData);
				m_Seq++;
			}

	        NumValue=VCI_Receive(DEV_USBCAN200,i,1,pCanObj);
			for(j=0;j<NumValue;j++)
			{
				CString strSeq;
				strSeq.Format("%d",m_Seq);
				CString strDeviceNum;
				strDeviceNum.Format("%s(%d)-1CH",m_ProductSn[i],i);
				CString strData,strTemp;
				for(int k=0;k<pCanObj[j].DataLen;k++)
				{
					strTemp.Format("%02X ",pCanObj[j].Data[k]);
					strData+=strTemp;
				}
				m_listData.InsertItem(m_Seq,strSeq);
				m_listData.SetItemText(m_Seq,1,"接收");

				m_listData.SetItemText(m_Seq,2,strDeviceNum);
				m_listData.SetItemText(m_Seq,3,strData);
				m_Seq++;
			}


		}
	}
	return 1;
}

void CTwoDeviceDlg::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	VCI_CAN_OBJ sendbuf;
	sendbuf.DataLen=8;
	sendbuf.ExternFlag=0;
	for(int i=0;i<4;i++)
		sendbuf.ID[i]=0;
	for(i=0;i<8;i++)
		sendbuf.Data[i]=i;
	sendbuf.RemoteFlag=0;
	memset(sendbuf.Reserved,0,3);
	sendbuf.SendType=0;
	sendbuf.TimeFlag=0;
	sendbuf.TimeStamp=0;
	DWORD DeviceNum=m_comboID.GetCurSel();
	if(1!=VCI_Transmit(DEV_USBCAN200,DeviceNum,0,&sendbuf))
	{
		AfxMessageBox("发送失败！");
	}
	if(1!=VCI_Transmit(DEV_USBCAN200,DeviceNum,1,&sendbuf))
	{
		AfxMessageBox("发送失败！");
	}
/*	if(1!=VCI_Transmit(DEV_USBCAN200,0,0,&sendbuf))
	{
		AfxMessageBox("发送失败！");
	}
	if(1!=VCI_Transmit(DEV_USBCAN200,0,1,&sendbuf))
	{
		AfxMessageBox("发送失败！");
	}
	if(1!=VCI_Transmit(DEV_USBCAN200,1,0,&sendbuf))
	{
		AfxMessageBox("发送失败！");
	}
	if(1!=VCI_Transmit(DEV_USBCAN200,1,1,&sendbuf))
	{
		AfxMessageBox("发送失败！");
	}*/
}

void CTwoDeviceDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here

	if(1!=VCI_CloseDevice(DEV_USBCAN,0))
	{
		AfxMessageBox("关闭失败！");
	}
	if(1!=VCI_CloseDevice(DEV_USBCAN,1))
	{
		AfxMessageBox("关闭失败！");
	}
}
