//
//  gwin32windowutility.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gwin32windowutility_H_
#define _gwin32windowutility_H_

#include "gcommon.h"
#include <WINDOWS.H>

class GWin32WindowParam;

class GWin32WindowUtility
{
	///////////////////////////////////////////////////////
	//static public methods
public:
	static LPVOID const UserDataGet(HWND in_hWnd);
	static void UserDataSet(
		HWND in_hWnd, 
		LPARAM in_lParam
		);
	static const bool ClassCreatedTest(
		HINSTANCE in_hInstance,
		const GWin32WindowParam& in_windowParam
		);
	static void ClassCreate(
		HINSTANCE in_hInstance,
		const GWin32WindowParam& in_windowParam,
		WNDPROC in_windowProc
		);
	static const RECT SizeCalculate( 
		const GWin32WindowParam& in_windowParam,
		const DWORD in_windowStyle
		);
	static HWND Create(
		HINSTANCE in_hInstance,
		const GWin32WindowParam& in_windowParam,
		LPVOID in_owner,
		WNDPROC in_windowProc
		);

	//disabled
private:
	~GWin32WindowUtility();
};

#endif //#ifndef _gwin32windowutility_H_
