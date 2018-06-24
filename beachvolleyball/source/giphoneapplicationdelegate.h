//
//  GIPhoneApplicationDelegate.h
//  playwithgoldfish
//
//  Created by David Coen on 2011 04 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _GIPhoneApplicationDelegate_h_
#define _GIPhoneApplicationDelegate_h_

#include <UIKit/UIKit.h>
#include <boost/scoped_ptr.hpp>

class GApplicationObjectTask;

typedef boost::scoped_ptr<GApplicationObjectTask> TPointerApplicationObjectTask;

/*
 this seems to need to live in the main translation unit to be creatable from main.cpp via name param of UIApplicationMain
 */
@interface GIPhoneApplicationDelegate : NSObject <UIApplicationDelegate> 
{
@private
	NSTimer* mTimer;
	TPointerApplicationObjectTask mApplicationObject;
	double mTimeLast;
}

@end

#endif //_GIPhoneApplicationDelegate_h_