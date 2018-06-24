//
//  giphoneviewutility.mm
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "GIPhoneViewUtility.h"

#import <AVFoundation/AVFoundation.h>
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

//static public methods
/*static* UIImage* const GIPhoneViewUtility::NewUIImageFromView(UIView* const in_view)
{
    // Create a graphics context with the target size
    // On iOS 4 and later, use UIGraphicsBeginImageContextWithOptions to take the scale into consideration
    // On iOS prior to 4, fall back to use UIGraphicsBeginImageContext
    CGSize imageSize = [in_view bounds].size;
    if (NULL != UIGraphicsBeginImageContextWithOptions)
	{
        UIGraphicsBeginImageContextWithOptions(imageSize, NO, 0);
	}
    else
	{
        UIGraphicsBeginImageContext(imageSize);
	}
	
    CGContextRef context = UIGraphicsGetCurrentContext();
	
    // -renderInContext: renders in the coordinate space of the layer,
    // so we must first apply the layer's geometry to the graphics context
    CGContextSaveGState(context);
    // Center the context around the view's anchor point
    CGContextTranslateCTM(context, [in_view center].x, [in_view center].y);
    // Apply the view's transform about the anchor point
    CGContextConcatCTM(context, [in_view transform]);
    // Offset by the portion of the bounds left of and above the anchor point
    CGContextTranslateCTM(context,
                          -[in_view bounds].size.width * [[in_view layer] anchorPoint].x,
                          -[in_view bounds].size.height * [[in_view layer] anchorPoint].y);
	
    // Render the layer hierarchy to the current context
    [[in_view layer] renderInContext:context];
	
    // Restore the context
    CGContextRestoreGState(context);
	
    // Retrieve the screenshot image
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
	
    UIGraphicsEndImageContext();
	
    return image;
}
*/

static void ReleaseDataCallback(void *info, const void *data, size_t size)
{
	//free((GLubyte *)data);
	return;
}


///*static*/ UIImage* const GIPhoneViewUtility::NewUIImageFromOpenGL()
/*static*/ UIImage* const GIPhoneViewUtility::NewUIImageFromBlitOpenGL(UIImage* const in_underlay)
{
	GLint width = 0;
	GLint height = 0;

    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &width);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &height);
	
    NSInteger myDataLength = width * height * 4;
 
    // allocate array and read pixels into it.
    GLubyte *buffer = (GLubyte *) malloc(myDataLength);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
 
    // make data provider with data.
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, buffer, myDataLength, ReleaseDataCallback);
 
    // prep the ingredients
    int bitsPerComponent = 8;
    int bitsPerPixel = 32;
    int bytesPerRow = 4 * width;
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
 
    // make the cgimage
    CGImageRef imageRef = CGImageCreate(width, height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
	
    // then make the uiimage from that
    UIImage *myImage = [UIImage imageWithCGImage:imageRef scale:1.0F orientation:UIImageOrientationDownMirrored];

	UIImage* const pResult = NewUIImageFromBlit(in_underlay, myImage);

	//CGRelease(imageRef);
	CGImageRelease(imageRef);
	//[myImage release];
	
	//don't free(buffer); we expect ReleaseDataCallback to do this for us
	free(buffer);

	//[in_underlay release];
	
    return pResult;
}

/*static*/ UIImage* const GIPhoneViewUtility::NewUIImageFromBlit(UIImage* const in_underlay, UIImage* const in_overlay)
{
    //CGSize imageSize = [in_underlay size];
    CGSize imageSize = [in_overlay size];

    if (NULL != UIGraphicsBeginImageContextWithOptions)
	{
        UIGraphicsBeginImageContextWithOptions(imageSize, NO, 0);
	}
    else
	{
        UIGraphicsBeginImageContext(imageSize);
	}
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGRect imageRect = CGContextGetClipBoundingBox(context);
	
	//CGContextDrawImage(context, imageRect, [in_underlay CGImage]); 
	//CGContextDrawImage(context, imageRect, [in_overlay CGImage]); 
	[in_underlay drawInRect:imageRect];
	//[in_overlay drawInRect:imageRect blendMode:kCGBlendModeNormal alpha:0.5F];
	[in_overlay drawInRect:imageRect];

    // Retrieve the screenshot image
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
	
    UIGraphicsEndImageContext();
	
    return image;
}

//
//
//
//void ManipulateImagePixelData(CGImageRef inImage)
//{
//    // Create the bitmap context
//    CGContextRef cgctx = CreateARGBBitmapContext(inImage);
//    if (cgctx == NULL) 
//    { 
//        // error creating context
//        return;
//    }
//
//     // Get image width, height. We'll use the entire image.
//    size_t w = CGImageGetWidth(inImage);
//    size_t h = CGImageGetHeight(inImage);
//    CGRect rect = {{0,0},{w,h}}; 
//
//    // Draw the image to the bitmap context. Once we draw, the memory 
//    // allocated for the context for rendering will then contain the 
//    // raw image data in the specified color space.
//    CGContextDrawImage(cgctx, rect, inImage); 
//
//    // Now we can get a pointer to the image data associated with the bitmap
//    // context.
//    void *data = CGBitmapContextGetData (cgctx);
//    if (data != NULL)
//    {
//
//        // **** You have a pointer to the image data ****
//
//        // **** Do stuff with the data here ****
//
//    }
//
//    // When finished, release the context
//    CGContextRelease(cgctx); 
//    // Free image data memory for the context
//    if (data)
//    {
//        free(data);
//    }
//}
