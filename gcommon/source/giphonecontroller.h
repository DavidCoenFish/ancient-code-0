//
//  giphonecontroller.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#ifndef _GIPhoneController_h_
#define _GIPhoneController_h_

#include "gcommon.h"

#include <UIKit/UIKit.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

//typedef

/**/
@interface GIPhoneController : UIViewController
{
@private	
	
}

- (id) init;
- (void) dealloc;

- (UIInterfaceOrientation) getOrientation;


@end

#endif //_GIPhoneViewOpenGLES_h_