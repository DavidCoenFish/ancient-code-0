//
//  GIPhoneAccelerometer.h
//
//  Created by David Coen on 23/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GIPhoneAccelerometer_h_
#define _GIPhoneAccelerometer_h_

#include <Foundation/Foundation.h>
#include <UIKit/UIAccelerometer.h>

/*
 act as wrapper and delegate for UIAccelerometer 
 */
@interface GIPhoneAccelerometer : NSObject <UIAccelerometerDelegate>
{
@private
	UIAccelerometer* mAccelerometer;
	bool mValid;
	float mX;	
	float mY;	
	float mZ;	
}

@property (readonly, getter=getValid) bool mValid;
@property (readonly, getter=getX) float mX;
@property (readonly, getter=getY) float mY;
@property (readonly, getter=getZ) float mZ;

+ (id) newInstance;
+ (id) newInstanceParamInterval:(const float)in_interval;

- (id) init;
- (id) initParamInterval:(const float)in_interval;
- (void) dealloc;

//implement UIAccelerometerDelegate
- (void) accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration;

@end

#endif // _GIPhoneAccelerometer_h_
