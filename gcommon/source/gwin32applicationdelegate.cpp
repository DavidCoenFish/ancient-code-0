//
//  gwin32applicationdelegate.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#include "gwin32applicationdelegate.h"
#include "gapplicationtask.h"

#include <WINDOWS.H>

/*static*/ GWin32ApplicationDelegate* GWin32ApplicationDelegate::sInstance = NULL;

///////////////////////////////////////////////////////
//static methods
/*static*/ void GWin32ApplicationDelegate::Run(const FApplicationTaskFactory in_applicationTaskFactory)
{
	new GWin32ApplicationDelegate(in_applicationTaskFactory);
	return;
}

///////////////////////////////////////////////////////
//constructor	
GWin32ApplicationDelegate::GWin32ApplicationDelegate(const FApplicationTaskFactory in_applicationTaskFactory)
: mApplicationTask()
{
	sInstance = this;
	DidFinishLaunchingWithOptions(in_applicationTaskFactory);
	return;
}

GWin32ApplicationDelegate::~GWin32ApplicationDelegate()
{
	mApplicationTask.reset();
	sInstance = NULL;
	return;
}

///////////////////////////////////////////////////////
//public methods
void GWin32ApplicationDelegate::ApplicationWillResignActive()
{
	if (mApplicationTask)
	{
		mApplicationTask->SetActive(false);
	}
	return;
}

void GWin32ApplicationDelegate::ApplicationDidBecomeActive()
{
	if (mApplicationTask)
	{
		mApplicationTask->SetActive(true);
	}
	return;
}

void GWin32ApplicationDelegate::ApplicationWillTerminate()
{
	delete this;
	return;
}

///////////////////////////////////////////////////////
//private methods
void GWin32ApplicationDelegate::DidFinishLaunchingWithOptions(const FApplicationTaskFactory in_applicationTaskFactory)
{
	if (GNULL != in_applicationTaskFactory)
	{
		mApplicationTask = in_applicationTaskFactory();
	}

	GU32 tickCount = GetTickCount();
	GBOOL localContinue = GTRUE;
	while ((GTRUE == localContinue) && 
			(GNULL != mApplicationTask))
	{
		GR32 timeDelta = 0.0F;
		const GU32 lastTick = tickCount;
		tickCount = GetTickCount();

		const GU32 deltaTick = tickCount - lastTick;
		if((deltaTick < 1000000 ) && (0 != lastTick))
		{
			timeDelta = ((GR32)(deltaTick)) * 0.001F;
		}

		localContinue &= mApplicationTask->Tick(timeDelta);
	}

	return;
}
