
// rubbish_detect.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Crubbish_detectApp:
// See rubbish_detect.cpp for the implementation of this class
//

class Crubbish_detectApp : public CWinApp
{
public:
	Crubbish_detectApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Crubbish_detectApp theApp;