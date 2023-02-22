// Capon.h : main header file for the CAPON application
//

#if !defined(AFX_CAPON_H__9FDC9A9C_10F7_4787_89C0_5898601B40FC__INCLUDED_)
#define AFX_CAPON_H__9FDC9A9C_10F7_4787_89C0_5898601B40FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCaponApp:
// See Capon.cpp for the implementation of this class
//

class CCaponApp : public CWinApp
{
public:
	CCaponApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaponApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCaponApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPON_H__9FDC9A9C_10F7_4787_89C0_5898601B40FC__INCLUDED_)
