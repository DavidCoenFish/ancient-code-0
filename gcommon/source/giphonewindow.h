//
//  giphonewindow.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GIPhoneWindow_h_
#define _GIPhoneWindow_h_

#include "GApplicationWindowType.h"
#include <UIKit/UIKit.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GApplicationWindowCallback;
class GInput;

//typedef
typedef boost::scoped_ptr<GInput> TPointerInput;

/**/
@interface GIPhoneWindow : UIWindow
{
@private	
	TPointerInput mInput;
	id mAccelerometer;
}

- (id) init;
- (id) initParamFrame:(CGRect)in_frame;
- (void) dealloc;

- (const GInput&) GetInput;

- (void) GetScreenSizeWidth:(int&)out_width Height:(int&)out_height;

- (void) PostTick:(const float)in_timeDelta;

//UIView
- (void) layoutSubviews;

//UIResponder
- (BOOL) canBecomeFirstResponder;
- (void) motionBegan:(UIEventSubtype)in_eventSubtype withEvent:(UIEvent*)in_event  __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_3_0);
- (void) motionEnded:(UIEventSubtype)in_eventSubtype withEvent:(UIEvent *)in_event __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_3_0);
- (void) motionCancelled:(UIEventSubtype)in_eventSubtype withEvent:(UIEvent *)in_event __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_3_0);

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;



@end

#endif //_GIPhoneViewOpenGLES_h_