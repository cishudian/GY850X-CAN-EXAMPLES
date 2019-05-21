// PciCanTest.h : main header file for the PCICANTEST application
//

#if !defined(AFX_PCICANTEST_H__6B5C6B1E_D58E_4789_8AC3_79E032DC836E__INCLUDED_)
#define AFX_PCICANTEST_H__6B5C6B1E_D58E_4789_8AC3_79E032DC836E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPciCanTestApp:
// See PciCanTest.cpp for the implementation of this class
//

class CPciCanTestApp : public CWinApp
{
public:
	CPciCanTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPciCanTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPciCanTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCICANTEST_H__6B5C6B1E_D58E_4789_8AC3_79E032DC836E__INCLUDED_)
