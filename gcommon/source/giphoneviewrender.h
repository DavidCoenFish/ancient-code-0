//
//  giphoneviewrender.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#ifndef _GIPhoneViewRender_h_
#define _GIPhoneViewRender_h_

#include "gcommon.h"
#include <boost/shared_ptr.hpp>
#include <Foundation/Foundation.h>
#include <UIKit/UIView.h>

class GIPhoneRender;
class GRender;

typedef boost::shared_ptr<GIPhoneRender> TPointerIPhoneRender; //shared, not scopped, as could be weak referenced by GRender (even though for performance we are using a raw pointer)
typedef boost::shared_ptr<GRender> TPointerRender;

/*
hold a GIPhoneRender, derive a UIView, satisfy view delegate interface
*/

@interface GIPhoneViewRender : UIView
{    
@private
    TPointerIPhoneRender mRenderer;
}

+ (Class) layerClass;
- (id) init;
- (id) initParamRender:(TPointerRender&)out_render
 transparentBackground:(const bool)in_transparentBackground
				 frame:(const CGRect&)in_frame
				 depth:(const GBOOL)in_depth
			   stencil:(const GBOOL)in_stencil;


- (void) dealloc;

//implement UIView
- (void)layoutSubviews;

@end

#endif //_GIPhoneViewOpenGL_h_