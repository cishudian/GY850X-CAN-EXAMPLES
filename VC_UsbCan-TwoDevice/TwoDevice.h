// TwoDevice.h : main header file for the TWODEVICE application
//

#if !defined(AFX_TWODEVICE_H__6C2CF96F_4AEF_495D_A6E6_6817E8F64B09__INCLUDED_)
#define AFX_TWODEVICE_H__6C2CF96F_4AEF_495D_A6E6_6817E8F64B09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTwoDeviceApp:
// See TwoDevice.cpp for the implementation of this class
//

class CTwoDeviceApp : public CWinApp
{
public:
	CTwoDeviceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTwoDeviceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTwoDeviceApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWODEVICE_H__6C2CF96F_4AEF_495D_A6E6_6817E8F64B09__INCLUDED_)
