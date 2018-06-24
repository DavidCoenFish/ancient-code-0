//
//  gwin32windowparam.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gwin32windowparam.h"

///////////////////////////////////////////////////////
//constructor
GWin32WindowParam::GWin32WindowParam(
	const std::string& in_className,
	const std::string& in_windowTitle,
	const GBOOL in_showWindow,
	const GU32 in_classStyle,
	const GU32 in_windowStyle,
	const GU32 in_windowOriginX,
	const GU32 in_windowOriginY,
	const GU32 in_windowDimentionX,
	const GU32 in_windowDimentionY,
	const HWND in_parentHWnd
	)
	: mClassName(in_className)
	, mWindowTitle(in_windowTitle)
	, mShowWindow(in_showWindow)
	, mClassStyle(in_classStyle)
	, mWindowStyle(in_windowStyle)
	, mWindowOriginX(in_windowOriginX)
	, mWindowOriginY(in_windowOriginY)
	, mWindowDimentionX(in_windowDimentionX)
	, mWindowDimentionY(in_windowDimentionY)
	, mParentHWnd(in_parentHWnd)
{
	return;
}

GWin32WindowParam::~GWin32WindowParam()
{
	return;
}

///////////////////////////////////////////////////////
// public static accessors
/*static*/ const GU32 GWin32WindowParam::ClassStyleDefaultGet()
{
	return 0; //( CS_HREDRAW | CS_VREDRAW ); //| CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW );
}

/*static*/ const GU32 GWin32WindowParam::WindowStyleDefaultGet()
{
	return( WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
}
