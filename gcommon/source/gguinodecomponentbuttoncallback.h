//
//  GGuiNodeComponentButtonCallback.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentButtonCallback_h_
#define _GGuiNodeComponentButtonCallback_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "GCommon.h"

class GVector2Float;

/**/
class GGuiNodeComponentButtonCallback : public boost::noncopyable
{
	//constructor
public:
	GGuiNodeComponentButtonCallback();
	virtual ~GGuiNodeComponentButtonCallback();

	//public methods
public:
	void ClickDown(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId
		);
	void ClickDrag(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId
		);
	//in_activate is true if click location has never left the button (ie, click release inside button)
	void ClickRelease(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId,
		const bool in_activate
		);

	//private interface
private:
	virtual void OnClickDown(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId		
		);
	virtual void OnClickDrag(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId	
		);
	virtual void OnClickRelease(
		const std::string& in_buttonName,
		const GVector2Float& in_screenPos,
		const GVector2Float& in_buttionRelativeRatio,
		const GS32 in_clickId,
		const bool in_activate
		);

};

#endif 