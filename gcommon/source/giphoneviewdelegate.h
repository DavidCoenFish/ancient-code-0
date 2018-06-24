//
//  giphoneviewdelegate.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _giphoneviewdelegate_h_
#define _giphoneviewdelegate_h_

#include "gcommon.h"
#include <Foundation/Foundation.h>

struct CATransform3D;
struct CGRect;
@class UIView;
@class UIImage;

@protocol GIPhoneViewDelegate <NSObject>

- (void) Present;
//- (UIView*) GetView;
//- (UIImage*) CompositView:(UIImage*)in_image;
//- (void) ResetWindowFrame:(const CGRect* const)in_windowRect Frame:(const CGRect* const)in_frame Transform:(const CATransform3D* const)transform;

@end

#endif //_giphoneviewdelegate_h_