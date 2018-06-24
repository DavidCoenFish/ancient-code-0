//
//  giphoneviewrender.mm
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//



#include "giphoneviewrender.h"

#include "giphonerender.h"
#include "grender.h"
#include "giphoneviewutility.h"

#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#include <QuartzCore/QuartzCore.h>

@implementation GIPhoneViewRender

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id) init
{
	[self release];
	return nil;
}


- (id) initParamRender:(TPointerRender&)out_render
 transparentBackground:(const bool)in_transparentBackground
				 frame:(const CGRect&)in_frame
				 depth:(const GBOOL)in_depth
			   stencil:(const GBOOL)in_stencil
{
	self = [super initWithFrame:in_frame];
	if (self)
	{
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
		if (eaglLayer)
		{
			eaglLayer.opaque = !in_transparentBackground;
			eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
											[NSNumber numberWithBool:FALSE], 
											kEAGLDrawablePropertyRetainedBacking, 
											kEAGLColorFormatRGBA8, 
											kEAGLDrawablePropertyColorFormat, 
											nil
											];
		}
		
		if (in_transparentBackground)
		{
			self.backgroundColor = [UIColor clearColor];
		}
		else 
		{
			self.backgroundColor = [UIColor blackColor];
		}

		mRenderer.reset(new GIPhoneRender(in_depth, in_stencil));
		out_render.reset(new GRender(mRenderer.get()));
	}
	
	return self;
}

- (void) dealloc
{
    [super dealloc];
	
	mRenderer.reset();

	return;
}

- (void)layoutSubviews
{
	//[self.layer setFrame:[self bounds]];
	
	if (mRenderer)
	{
		mRenderer->Resize((CAEAGLLayer*)self.layer);
	}
    //[self drawView:nil];
	return;
}

//implement GIPhoneViewComponentDelegate
- (void) Present
{
	if (mRenderer)
	{
		mRenderer->Present();
	}

	return;
}

//- (UIView*) GetView
//{
//	return self;
//}

//- (UIImage*) CompositView:(UIImage*)in_image
//{
	//UIImage* const pGLImage = GIPhoneViewUtility::NewUIImageFromOpenGL();
	//UIImage* resultImage = GIPhoneViewUtility::NewUIImageFromBlit(in_image, pGLImage);
	
	//[pGLImage release];
//	UIImage* resultImage = GIPhoneViewUtility::NewUIImageFromBlitOpenGL(in_image);
	
//	return resultImage;
//}

//- (void) ResetWindowFrame:(const CGRect* const)in_windowRect Frame:(const CGRect* const)in_frame Transform:(const CATransform3D* const)in_transform
//{
//	[self setFrame:*in_frame];
//	self.layer.transform = *in_transform;
//	[self.layer setFrame:*in_windowRect];
//	
//	[self layoutSubviews];
//	
//	return; 
//}


@end