//
//  giphonewindow.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphonewindow.h"

#include "ginput.h"
#include "ginputjoystick.h"
#include "ginputbutton.h"
#include "gvector2float.h"
#include "gvector3float.h"
#include "GStringUtility.h"
#include "GIPhoneAccelerometer.h"
#include <string>

@interface GIPhoneWindow (PrivateMethods)
- (void) addTouch:(UIEvent *)event;
@end

@implementation GIPhoneWindow

- (id) init
{
	[self release];
	return nil;
}

- (id) initParamFrame:(CGRect)in_frame
{
	self = [super initWithFrame:in_frame];
	if (self)
	{
		mInput.reset(new GInput);
        mInput->SetButtonCount(1);
		mAccelerometer = [GIPhoneAccelerometer newInstance];

	}
	
	return self;
}

- (void) dealloc
{
    [super dealloc];	
	[mAccelerometer release];
	return;
}

- (const GInput&) GetInput
{
	return (*mInput);
}

- (void) GetScreenSizeWidth:(int&)out_width Height:(int&)out_height
{
	CGRect windowRect = [self bounds];
	out_width = (int)(windowRect.size.width + 0.5F);
	out_height = (int)(windowRect.size.height + 0.5F);
	return;
}

- (void) PostTick:(const float)in_timeDelta
{	
	if (mInput)
	{
		mInput->Tick(in_timeDelta);

		if ([mAccelerometer getValid])
		{
			mInput->SetJoystickCount(1);
			mInput->GetJoystick(0).GetDirection() = GVector3Float(
				[mAccelerometer getX],
				[mAccelerometer getY],
				[mAccelerometer getZ]
				);
		}
	}

	return;
}

//UIView
- (void) layoutSubviews
{
	//if (mApplicationWindowCallback)
	//{
	//	CGRect localFrame = [self bounds];
	//	mApplicationWindowCallback->Resize((GU32)(localFrame.size.width + 0.5F),
	//					(GU32)(localFrame.size.height + 0.5F));
	//}
	return;
}

//implement UIResonder
- (BOOL) canBecomeFirstResponder
{
	return YES;
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self addTouch:event];
	[super touchesBegan:touches withEvent:event]; 
	return;
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self addTouch:event];
	[super touchesMoved:touches withEvent:event]; 
	return;
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self addTouch:event];
	[super touchesEnded:touches withEvent:event]; 
	return;
}

- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self addTouch:event];
	[super touchesCancelled:touches withEvent:event]; 
	return;
}

- (void) motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
    mInput->GetButton(0).SetState(true);
	return;
}
- (void) motionEnded:(UIEventSubtype)in_eventSubtype withEvent:(UIEvent *)in_event
{
    mInput->GetButton(0).SetState(false);
	return;
}
- (void) motionCancelled:(UIEventSubtype)in_eventSubtype withEvent:(UIEvent *)in_event
{
    mInput->GetButton(0).SetState(false);
	return;
}


//private methods
- (void) addTouch:(UIEvent *)event
{
	if ((!mInput) || (UIEventTypeTouches != [event type]))
	{
		return;
	}	
	
	NSArray* arrayObject = [[event allTouches] allObjects];
	const int count = [arrayObject count];
	const CGRect viewFrame = self.frame;
	const float width = std::max(1.0F, viewFrame.size.width); //320.0F
	const float height = std::max(.0F, viewFrame.size.height); //480.0F
	
	mInput->SetTouchCount(0);
	
	for (int index = 0; index < count; ++index)
	{
		id object = [arrayObject objectAtIndex:index];
		if (![object isKindOfClass:[UITouch class]] )
		{
			continue;
		}
		UITouch* touch = object;
		const CGPoint location = [touch locationInView:[touch view]];
		const float x = location.x / width;
		const float y = location.y / height;
		const CGPoint oldLocation = [touch previousLocationInView:[touch view]];
		const float deltaX = x - (oldLocation.x / width);
		const float deltaY = y - (oldLocation.y / height);
		
		bool begin = false;
		bool end = false;
		bool cancel = false;
		switch (touch.phase)
		{
			case UITouchPhaseBegan:
				begin = true;
				break;
			case UITouchPhaseEnded:
				end = true;
				break;
			case UITouchPhaseCancelled:
				cancel = true;
				break;
			default:
				break;
		};
		
		mInput->AddTouch((int)touch,
						 GVector2Float(x, y),
						 GVector2Float(deltaX, deltaY),
						 begin,
						 end,
						 cancel
						 );


	}		
	
}

@end
