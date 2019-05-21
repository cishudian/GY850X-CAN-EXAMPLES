// PciCanTestDlg.h : header file
//

#if !defined(AFX_PCICANTESTDLG_H__D624CD92_D152_41B6_8347_70FAC46CF189__INCLUDED_)
#define AFX_PCICANTESTDLG_H__D624CD92_D152_41B6_8347_70FAC46CF189__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ControlCAN.h"
#include <afxmt.h>
/////////////////////////////////////////////////////////////////////////////
// CPciCanTestDlg dialog

class CPciCanTestDlg : public CDialog
{
// Construction
public:
	CCriticalSection m_section;
	BOOL m_bIsFirstCallBack;
	UINT m_nSeq;
	HANDLE m_hEvent;
	HANDLE m_hRecieve;
	DWORD Recieve();
	BOOL InitializeCan();
	HANDLE m_hDevice;
	CPciCanTestDlg(CWnd* pParent = NULL);	// standard constructor
	HANDLE OpenDevice(DWORD DeviceType,DWORD instance,DWORD Error);	
	int	HexChar(char c);
	int Str2Hex(CString str);
	void  CanReceiveShow(DWORD CANInd,PVCI_CAN_OBJ pCanObj,DWORD dLength);
// Dialog Data
	//{{AFX_DATA(CPciCanTestDlg)
	enum { IDD = IDD_PCICANTEST_DIALOG };
	CComboBox	m_comboCanIndex;
	CListCtrl	m_listSR;
	CString	m_strID;
	CString	m_strData;
	BOOL	m_bCallBack;
	BOOL	m_bQuery;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPciCanTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPciCanTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonClear();
	afx_msg void OnCheckCallback();
	afx_msg void OnCheckQuery();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCICANTESTDLG_H__D624CD92_D152_41B6_8347_70FAC46CF189__INCLUDED_)
