
// 15T1_MDU118_Assignment_3.h : main header file for the 15T1_MDU118_Assignment_3 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGameFrameworkApp:
// See 15T1_MDU118_Assignment_3.cpp for the implementation of this class
//

class CGameFrameworkApp : public CWinApp
{
public:
	CGameFrameworkApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGameFrameworkApp theApp;
