//
//  gwin32windowparam.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gwin32windowparam_H_
#define _gwin32windowparam_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <string>
#include <WINDOWS.H>

class GWin32WindowParam : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//constructor
public:
	GWin32WindowParam(
		const std::string& in_className = "SWindowDefault",
		const std::string& in_windowTitle = "default title",
		const GBOOL in_showWindow = true,
		const GU32 in_classStyle = ClassStyleDefaultGet(),
		const GU32 in_windowStyle = WindowStyleDefaultGet(),
		const GU32 in_windowOriginX = 64,
		const GU32 in_windowOriginY = 64,
		const GU32 in_windowDimentionX = 320, //640, //320,
		const GU32 in_windowDimentionY = 480, //960, //480,
		const HWND in_parentHWnd = 0
		);
	~GWin32WindowParam();

	///////////////////////////////////////////////////////
	// public static accessors
public:
	static const GU32 ClassStyleDefaultGet();
	static const GU32 WindowStyleDefaultGet();

	///////////////////////////////////////////////////////
	// public accessors
public:
	const std::string& ClassNameGet()const{ return mClassName; }
	const std::string& WindowTitleGet()const{ return mWindowTitle; }
	const GBOOL ShowWindowGet()const{ return mShowWindow; }
	const GU32 WindowOriginXGet()const{ return mWindowOriginX; }
	const GU32 WindowOriginYGet()const{ return mWindowOriginY; }
	const GU32 WindowDimentionXGet()const{ return mWindowDimentionX; }
	const GU32 WindowDimentionYGet()const{ return mWindowDimentionY; }
	const GU32 ClassStyleGet()const{ return mClassStyle; }
	const GU32 WindowStyleGet()const{ return mWindowStyle; }
	const HWND ParentGet()const{ return mParentHWnd; }

	///////////////////////////////////////////////////////
	// private members
private:
	const std::string mClassName;
	const std::string mWindowTitle;
	const GBOOL mShowWindow;
	const GU32 mClassStyle;
	const GU32 mWindowStyle;
	const GU32 mWindowOriginX;
	const GU32 mWindowOriginY;
	const GU32 mWindowDimentionX;
	const GU32 mWindowDimentionY;
	const HWND mParentHWnd;

};


#endif //#ifndef _gwin32windowparam_H_
