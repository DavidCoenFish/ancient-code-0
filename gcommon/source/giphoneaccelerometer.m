//
//  GIPhoneAccelerometer.m
//
//  Created by David Coen on 23/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GIPhoneAccelerometer.h"

static const float sDefaultInterval = (1.0F / 60.0F);

@implementation GIPhoneAccelerometer

@synthesize mX;
@synthesize mY;
@synthesize mZ;
@synthesize mValid;

+ (id) newInstance
{
	id instance = [[self alloc] init];
	return instance;
}

+ (id) newInstanceParamInterval:(const float)in_interval
{
	id instance = [[self alloc] initParamInterval:in_interval];
	return instance;
}

- (id) init
{
	return [self initParamInterval:sDefaultInterval];
}

- (id) initParamInterval:(const float)in_interval
{
	self = [super init];
	if (self)
	{
		mAccelerometer = [UIAccelerometer sharedAccelerometer];
		[mAccelerometer setUpdateInterval:in_interval];
		[mAccelerometer setDelegate:self];		
	}
	
	return self;
}

- (void) dealloc
{
	[mAccelerometer setDelegate:nil];		
	mAccelerometer = nil;
    [super dealloc];	
	return;
}

- (void) accelerometer:(UIAccelerometer *)in_accelerometer didAccelerate:(UIAcceleration *)in_acceleration
{
	if (nil == in_acceleration)
	{
		return;
	}

	mValid = true;
	mX = [in_acceleration x];
	mY = [in_acceleration y];
	mZ = [in_acceleration z];
	
	//mShake = [in_acceleration 
	
	return;	
}


@end
