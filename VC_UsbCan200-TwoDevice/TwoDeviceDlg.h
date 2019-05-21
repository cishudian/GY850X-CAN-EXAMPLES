// TwoDeviceDlg.h : header file
//

#if !defined(AFX_TWODEVICEDLG_H__9DE113A4_59B7_4CFB_B902_E46BE4B43CC0__INCLUDED_)
#define AFX_TWODEVICEDLG_H__9DE113A4_59B7_4CFB_B902_E46BE4B43CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTwoDeviceDlg dialog
#include "ControlCAN.h"
class CTwoDeviceDlg : public CDialog
{
// Construction
public:
	ULONG m_Seq;
	CString m_ProductSn[5];
	int m_nDeiviceNum;
	DWORD ReceiveProcess();
	CTwoDeviceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTwoDeviceDlg)
	enum { IDD = IDD_TWODEVICE_DIALOG };
	CListCtrl	m_listData;
	CComboBox	m_comboID;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTwoDeviceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTwoDeviceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWODEVICEDLG_H__9DE113A4_59B7_4CFB_B902_E46BE4B43CC0__INCLUDED_)
