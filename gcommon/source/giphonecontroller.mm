//
//  giphonecontroller.mm
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphonecontroller.h"

@implementation GIPhoneController

- (id) init
{
	self = [super init];
	if (self)
	{
	}
	
	return self;
}

- (void) dealloc
{
    [super dealloc];	
	return;
}


- (UIInterfaceOrientation) getOrientation
{
    return self.interfaceOrientation;
}

@end
