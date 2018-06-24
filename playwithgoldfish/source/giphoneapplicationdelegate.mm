//
//  GIPhoneApplicationDelegate.mm
//  playwithgoldfish
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "GIPhoneApplicationDelegate.h"
#include "GApplicationObjectTask.h"
#include "GDictionary.h"
#include "GStateFactory.h"
#include "GSharedBase.h"
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<GDictionary> TPointerDictionary;
typedef boost::shared_ptr<GStateFactory> TPointerStateFactory;

extern TPointerDictionary& GetApplicationParamObject();


@implementation GIPhoneApplicationDelegate

- (BOOL) application:(UIApplication*)in_application didFinishLaunchingWithOptions:(NSDictionary *)in_launchOptions
{
	in_application.applicationSupportsShakeToEdit = YES;
	
	TPointerDictionary& paramObject = GetApplicationParamObject();
	TPointerStateFactory stateFactory = boost::dynamic_pointer_cast<GStateFactory, GSharedBase>(paramObject->GetValueSharedBase("taskFactory"));
	paramObject->RemoveKeySharedBase("taskFactory");

	if (stateFactory)
	{
		mApplicationObject.reset(new GApplicationObjectTask(stateFactory));
	}
	
	mTimeLast = 0.0;
	mTimer = [NSTimer scheduledTimerWithTimeInterval:0.016666666F
			target:self
			selector:@selector(timerCallback)
			userInfo:nil
			repeats:YES];

	//[[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSEventTrackingRunLoopMode];
	//[[NSRunLoop currentRunLoop] addTimer:mTimer forMode:NSModalPanelRunLoopMode];

    return YES;
}

- (void) applicationWillResignActive:(UIApplication *)in_application
{
	if (mApplicationObject)
	{
		mApplicationObject->SetActive(false);
	}
	//exit the app if we loss focus
	exit(0);
	
	return;
}

- (void) applicationDidBecomeActive:(UIApplication *)in_application
{
	if (mApplicationObject)
	{
		mApplicationObject->SetActive(true);
	}
	
	return;
}

- (void)applicationWillTerminate:(UIApplication *)in_application
{
	if (mApplicationObject)
	{
		mApplicationObject->SetActive(false);
	}
	
	return;
}

- (void)dealloc
{
	if (mTimer)
	{
		[mTimer invalidate];
		mTimer = nil;
	}

	if (mApplicationObject)
	{
		mApplicationObject.reset();
	}
    
    [super dealloc];
}

- (void)timerCallback
{
	if (mApplicationObject && mTimer)
	{
		float rate = 0.0F;
		const double newDate = [NSDate timeIntervalSinceReferenceDate];
		if (0.0 != mTimeLast)
		{
			rate = newDate - mTimeLast;
		}
		mTimeLast = newDate;
		mApplicationObject->Tick(rate);
	}

	return;
}

@end
