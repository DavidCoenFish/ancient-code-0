//
//  gview.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gview.h"

#ifdef IPHONE
	#include "giphoneview.h"
#elif WIN32
	#include "gwin32view.h"
#endif

///////////////////////////////////////////////////////
//constructor
GView::GView(TPointerViewImplementation& inout_pointerViewImplementation)
: mPointerViewImplementation(inout_pointerViewImplementation)
{
	return;
}

/*virtual*/ GView::~GView()
{
	return;
}
