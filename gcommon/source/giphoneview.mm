//
//  giphoneview.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphoneview.h"

#include <UIKit/UIView.h>

///////////////////////////////////////////////////////
//constructor
GIPhoneView::GIPhoneView(
	UIView* const inout_view
	)
	: mView(inout_view)
{
	return;
}

GIPhoneView::~GIPhoneView()
{
	return;
}

const GU32 GIPhoneView::GetWidth()const
{
    return 0;
}

const GU32 GIPhoneView::GetHeight()const
{
    return 0;
}
