//
//  gwin32windowutility.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#include "gwin32windowutility.h"
#include "gwin32windowparam.h"
#include <WINDOWS.H>

///////////////////////////////////////////////////////
//public static methods
/*static*/ LPVOID const GWin32WindowUtility::UserDataGet(HWND in_hWnd)
{
	return LongToPtr(GetWindowLongPtr(in_hWnd, GWLP_USERDATA));
}

/*static*/ void GWin32WindowUtility::UserDataSet(
	HWND in_hWnd,
    LPARAM in_lParam
	)
{
	LPCREATESTRUCT lpCreateStruct = (LPCREATESTRUCT)in_lParam;
	LPVOID userData = lpCreateStruct->lpCreateParams;
	SetWindowLongPtr(in_hWnd, GWLP_USERDATA, PtrToLong(userData));
	return;
}

/*static*/ const GBOOL GWin32WindowUtility::ClassCreatedTest(
	HINSTANCE in_hInstance,
	const GWin32WindowParam& in_windowParam
	)
{
	WNDCLASSEX classDummy;
	if(0 != GetClassInfoEx( 
		in_hInstance, 
		in_windowParam.ClassNameGet().c_str(),
		&classDummy 
		))
	{
		return GTRUE;
	}

	return GFALSE;
}

/*static*/ void GWin32WindowUtility::ClassCreate(
	HINSTANCE in_hInstance,
	const GWin32WindowParam& in_windowParam,
	WNDPROC in_windowProc
	)
{
	//is class already registered
	if(GTRUE == ClassCreatedTest( 
		in_hInstance,
		in_windowParam
		))
	{
		return;
	}

	WNDCLASSEX windowClass; 
	SecureZeroMemory(&windowClass, sizeof(windowClass));

	HCURSOR hCursor = NULL;
	HICON hIcon = NULL;
	HICON hIconSmall = NULL;

	windowClass.cbSize = sizeof( windowClass );
	windowClass.style = in_windowParam.ClassStyleGet();
	windowClass.lpfnWndProc = in_windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = sizeof( LONG_PTR );
	windowClass.hInstance = in_hInstance;
	windowClass.hIcon = hIcon;
	windowClass.hCursor = hCursor;
	windowClass.hbrBackground = ( HBRUSH )GetStockObject( NULL_BRUSH );
	windowClass.lpszMenuName =  NULL;
	windowClass.lpszClassName = in_windowParam.ClassNameGet().c_str();
	windowClass.hIconSm = hIconSmall;

	// Register the window class. 
	RegisterClassEx(&windowClass); 

	return;
}

/*static*/ const RECT GWin32WindowUtility::SizeCalculate( 
	const GWin32WindowParam& in_windowParam,
	const DWORD in_windowStyle
	)
{
	RECT size;
	BOOL result = 0;

	size.left = in_windowParam.WindowOriginXGet();
	size.top = in_windowParam.WindowOriginYGet();
	size.right = in_windowParam.WindowOriginXGet() + in_windowParam.WindowDimentionXGet();
	size.bottom = in_windowParam.WindowOriginYGet() + in_windowParam.WindowDimentionYGet();

	const bool menu = false; //( NULL != in_windowParam.MenuGet() );
	result = AdjustWindowRectEx( &size, in_windowStyle, menu, 0 );

	return size;
}

/*static*/ HWND GWin32WindowUtility::Create(
	HINSTANCE in_hInstance,
	const GWin32WindowParam& in_windowParam,
	LPVOID in_owner,
	WNDPROC in_windowProc
	)
{
	ClassCreate(
		in_hInstance,
		in_windowParam,
		in_windowProc
		);

	const RECT size = SizeCalculate( 
		in_windowParam,
		in_windowParam.WindowStyleGet()
		);

	HMENU menu = NULL;

	return CreateWindow( 
		in_windowParam.ClassNameGet().c_str(),
		in_windowParam.WindowTitleGet().c_str(),
		in_windowParam.WindowStyleGet(),
		size.left,
		size.top,
		size.right - size.left,
		size.bottom - size.top,
		in_windowParam.ParentGet(),
		menu,
		in_hInstance,
		in_owner
	);
}
