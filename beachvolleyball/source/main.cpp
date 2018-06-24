//
//  main.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

/*
note: the file extention convention
 if the code is c++ and intended for use outside the iphone only part of xcode project, use .cpp 
	if the cpp file includes a objective c header in xcode, add build flag """-x objective-c++"""
 if code is for xcode iphone only and uses c++, or c++ and objective-c, use .mm
 if code is for xcode iphone only and uses objective-c, use .m
 
 only make things object c if they need to be, ie to derrive objective c classes

 aim is to get the codebase to be able to work on iphone and win32 for easier development
 */

#include <boost/shared_ptr.hpp>
#include <GDictionary.h>
#include "BeachVolleyBallStateFactory.h"

typedef boost::shared_ptr<GDictionary> TPointerDictionary;
typedef boost::shared_ptr<GSharedBase> TPointerSharedBase;

TPointerDictionary& GetApplicationParamObject()
{
	static TPointerDictionary sDictionary;
	if (!sDictionary)
	{
		sDictionary.reset(new GDictionary);
	}
	return sDictionary;
}

//debug
#include <GMath.h>
#include <GASSERT.h>
//debug end

static void LocalMain(
	TPointerDictionary& inout_dictionary
	)
{
	inout_dictionary->SetValueBool("setStatusBarHidden", true);
#ifdef IPHONE
	inout_dictionary->SetValueBool("cameraBackground", true);
#endif

	TPointerSharedBase pointerStateFactory;
	pointerStateFactory.reset(new BeachVolleyBallStateFactory(inout_dictionary));
	inout_dictionary->SetValueSharedBase(
		"taskFactory",
		pointerStateFactory
		);

	return;
}

#ifdef IPHONE

#include <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	//create param object
	TPointerDictionary& paramObject = GetApplicationParamObject();
	LocalMain(paramObject);
	
	int retVal = UIApplicationMain(argc, argv, 0, @"GIPhoneApplicationDelegate");
    [pool release];
    return retVal;
}

#elif WIN32

#include <WINDOWS.H>
#include <GCommon.h>
#include "GWin32ApplicationDelegate.h"

static HINSTANCE sHInstance = NULL;
HINSTANCE GetHInstance()
{
	return sHInstance;
}

int __stdcall WinMain(
	HINSTANCE in_hInstance,
    HINSTANCE GCOMMON_UNUSED_VAR( in_hPrevInstance ),
    LPSTR in_lpCmdLine,
    int in_nCmdShow
)
{
	sHInstance = in_hInstance;

	//create param object
	TPointerDictionary& paramObject = GetApplicationParamObject();
	LocalMain(paramObject);

	//create application delegate
	GWin32ApplicationDelegate::Run(paramObject);

	return 0;
}

#endif
