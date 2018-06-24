//
//  GGuiNodeComponentButtonCallback.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentButtonCallback.h"
#include "GGuiType.h"

//constructor
GGuiNodeComponentButtonCallback::GGuiNodeComponentButtonCallback()
{
	return;
}

/*virtual*/ GGuiNodeComponentButtonCallback::~GGuiNodeComponentButtonCallback()
{
	return;
}

//public methods
void GGuiNodeComponentButtonCallback::ClickDown(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId
	)
{
	OnClickDown(in_buttonName, in_screenPos, in_buttionRelativeRatio, in_clickId);
	return;
}

void GGuiNodeComponentButtonCallback::ClickDrag(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId
	)
{
	OnClickDrag(in_buttonName, in_screenPos, in_buttionRelativeRatio, in_clickId);
	return;
}

//in_activate is true if click location has never left the button (ie, click release inside button)
void GGuiNodeComponentButtonCallback::ClickRelease(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId,
	const bool in_activate	
	)
{
	OnClickRelease(in_buttonName, in_screenPos, in_buttionRelativeRatio, in_clickId, in_activate);
	return;
}

//private methods
/*virtual*/ void GGuiNodeComponentButtonCallback::OnClickDown(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId		
	)
{
	return;
}
/*virtual*/ void GGuiNodeComponentButtonCallback::OnClickDrag(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId		
	)
{
	return;
}
/*virtual*/ void GGuiNodeComponentButtonCallback::OnClickRelease(
	const std::string& in_buttonName,
	const GVector2Float& in_screenPos,
	const GVector2Float& in_buttionRelativeRatio,
	const GS32 in_clickId,
	const bool in_activate
	)
{
	return;
}


