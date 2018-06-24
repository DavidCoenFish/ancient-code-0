//
//  giphoneapplicationwindow.mm
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphoneapplicationwindow.h"

#include "giphonecontroller.h"
#include "giphoneview.h"
#include "giphoneviewrender.h"
#include "giphonewindow.h"
#include "gview.h"
#include "ginput.h"

#include <string>
#include <UIKit/UIKit.h>
#include <UIKit/UIWindow.h>
#include <UIKit/UIView.h>
#include <QuartzCore/QuartzCore.h>

typedef boost::shared_ptr<GIPhoneView> TPointerIPhoneView;

/////////////////////////////////////////////////////////////////////////	
//constructor
GIPhoneApplicationWindow::GIPhoneApplicationWindow()
	: mWindow(0)
	, mController(0)
	, mArrayView()
{
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

	CGRect rect = CGRectMake(0.0F, 0.0F, 320.0F, 16.0F);
	mDebugText = [[UILabel alloc] initWithFrame:rect];

	//window
	mWindow = [[GIPhoneWindow alloc] initParamFrame:[[UIScreen mainScreen] bounds]];

	mController = [[GIPhoneController alloc] init];
	[mWindow setRootViewController:mController];
	[mWindow setMultipleTouchEnabled:FALSE];

	[mWindow makeKeyAndVisible];
		
	[mDebugText setOpaque:NO];
	[mDebugText setColor:[UIColor whiteColor]];
	[mDebugText setBackgroundColor:[UIColor clearColor]];
	[mDebugText setUserInteractionEnabled:FALSE];
	[mWindow addSubview:mDebugText];
	//[mDebugText setText:@"hello world"];
	
	return;
}

GIPhoneApplicationWindow::~GIPhoneApplicationWindow()
{
    [mWindow release];
    [mController release];
    
	return;
}

///////////////////////////////////////////////////////
//public methods
GVOID GIPhoneApplicationWindow::CreateViewRender(
	TPointerView& out_view,
	TPointerRender& out_render,
	const GBOOL in_transparentBackground,
	const GBOOL in_depth,
	const GBOOL in_stencil,
    const GR32 in_scale
	)
{
	CGRect windowRect = [mWindow bounds];
    
    CGRect frame = CGRectMake(
        windowRect.origin.x * in_scale,
        windowRect.origin.y * in_scale,
        windowRect.size.width * in_scale,
        windowRect.size.height * in_scale
        );

	GIPhoneViewRender* const view = [[GIPhoneViewRender alloc] initParamRender:out_render
		transparentBackground:in_transparentBackground
		frame:frame 
		depth:in_depth
		stencil:in_stencil
		];
        
    const GR32 scaleInvert = 1.0F / in_scale;
    
    [view layer].transform = CATransform3DScale (CATransform3DMakeTranslation(
        -windowRect.size.width * scaleInvert,
        -windowRect.size.height * scaleInvert,
        0.0F
        ), scaleInvert, scaleInvert, 1.0F);
	[view layer].frame = windowRect;    
	[view setUserInteractionEnabled:FALSE];
    
	TPointerIPhoneView pointer;
	pointer.reset(new GIPhoneView(view));

	out_view.reset(new GView(pointer));
	return;	
}
	
//TPointerView CreateViewCamera()
void GIPhoneApplicationWindow::PushViewFront(TPointerView& inout_view)
{
	if (GNULL == inout_view)
	{
		return;
	}
    
	UIView* const view = inout_view->GetPointerViewImplementation()->GetView();
	[mWindow addSubview:view];
	
	[mWindow bringSubviewToFront:mDebugText];
	
	mArrayView.push_back(inout_view);
	
	return;
}

void GIPhoneApplicationWindow::PushViewInfront(TPointerView& inout_view, TPointerView& inout_where)
{
	if (GNULL == inout_view)
	{
		return;
	}
	
	UIView* const view = inout_view->GetPointerViewImplementation()->GetView();
	UIView* const where = inout_where->GetPointerViewImplementation()->GetView();
	[mWindow insertSubview:view aboveSubview:where];

	[mWindow bringSubviewToFront:mDebugText];

	for (TArrayWeakView::iterator iterator = mArrayView.begin(); iterator != mArrayView.end(); ++iterator)
	{
		if (inout_where == (*iterator).lock())
		{
			mArrayView.insert(iterator, inout_view);
			return;
		}
	}


	return;
}

void GIPhoneApplicationWindow::RemoveView(TPointerView& inout_view)
{
	if (GNULL == inout_view)
	{
		return;
	}
	
	UIView* const view = inout_view->GetPointerViewImplementation()->GetView();
	[view removeFromSuperview];
	
	for (TArrayWeakView::iterator iterator = mArrayView.begin(); iterator != mArrayView.end(); ++iterator)
	{
		if (inout_view == (*iterator).lock())
		{
			mArrayView.erase(iterator);
			return;
		}
	}	
	
	return;
}

GVOID GIPhoneApplicationWindow::PreTick(const GR32 in_timeDelta)
{
	return;
}

const GBOOL GIPhoneApplicationWindow::Tick(const GR32 in_timeDelta)
{
	return true;
}

GVOID GIPhoneApplicationWindow::PostTick(const GR32 in_timeDelta)
{
	[mWindow PostTick:in_timeDelta];
}

void GIPhoneApplicationWindow::PutText(const std::string& in_text)
{
	[mDebugText setText:[NSString stringWithUTF8String:in_text.c_str()]];
}

void GIPhoneApplicationWindow::Show()
{
	[mWindow setNeedsLayout];
	[mWindow layoutIfNeeded];
	[mWindow makeKeyAndVisible];
	
	return;
}

const GInput& GIPhoneApplicationWindow::GetInput()const
{
	return [mWindow GetInput];
}

void GIPhoneApplicationWindow::GetScreenDimention(
	GApplicationWindowType::TOrientation::TEnum& out_orientation,
	int& out_width, 
	int& out_height
	)const
{
	[mWindow GetScreenSizeWidth:out_width Height:out_height];

    const UIDeviceOrientation orientation = [UIDevice currentDevice].orientation;
    
    switch (orientation)
    {
    default:
    case UIDeviceOrientationFaceUp:              // Device oriented flat, face up
		out_orientation = GApplicationWindowType::TOrientation::TFaceUp;
		break;
    case UIDeviceOrientationFaceDown:            // Device oriented flat, face down
		out_orientation = GApplicationWindowType::TOrientation::TFaceDown;
		break;
    case UIDeviceOrientationPortrait:            // Device oriented vertically, home button on the bottom
        out_orientation = GApplicationWindowType::TOrientation::TTwelveOclock;
        break;    
    case UIDeviceOrientationPortraitUpsideDown:  // Device oriented vertically, home button on the top
        out_orientation = GApplicationWindowType::TOrientation::TSixOclock;
        break;    
    case UIDeviceOrientationLandscapeLeft:       // Device oriented horizontally, home button on the right
        out_orientation = GApplicationWindowType::TOrientation::TNineOclock;
        break;    
    case UIDeviceOrientationLandscapeRight:      // Device oriented horizontally, home button on the left
        out_orientation = GApplicationWindowType::TOrientation::TThreeOclock;
        break;    
    }

	return; 
}


//return 0.0 to 1.0, (if undefined, returns 1.0)
//const GR32 GIPhoneApplicationWindow::GetBatteryLevel()const
//{
//    if ([UIDevice currentDevice].batteryMonitoringEnabled)
//    {
//        return [UIDevice currentDevice].batteryLevel;
//    }
//    
//    return 1.0F;
//}
