//
//  main.cpp
//  Great Little Shooter
//
//  Created by David Coen on 2011 05 29
//  Copyright Pleasure seeking morons 2011. All rights reserved.
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

#if WIN32
	#include <vld.h>

	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "main.h"
#include "applicationtaskgame.h"
#include "applicationtasktest.h"
#include "applicationtasktestparticle.h"
#include <gapplicationtask.h>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<GApplicationTask> TPointerApplicationTask;

//extern on iPhone
boost::shared_ptr<GApplicationTask> MainApplicationTaskFactory()
{
#if 0
	return boost::shared_ptr<GApplicationTask>(new ApplicationTaskTestParticle);
#elif 0
	return boost::shared_ptr<GApplicationTask>(new ApplicationTaskTest);
#else
	return boost::shared_ptr<GApplicationTask>(new ApplicationTaskGame);
#endif
}


#ifdef IPHONE
#include <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	int retVal = UIApplicationMain(argc, argv, 0, @"GIPhoneApplicationDelegate");
    [pool release];
    return retVal;
}

#elif WIN32

#include <gwin32applicationdelegate.h>
#include <WINDOWS.H>

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

	//create application delegate
	{
		GWin32ApplicationDelegate applicationDelegate(MainApplicationTaskFactory);
	}

	return 0;
}

#endif
