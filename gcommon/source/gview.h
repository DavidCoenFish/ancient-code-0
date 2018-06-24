//
//  gview.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gview_H_
#define _gview_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#ifdef IPHONE
	class GIPhoneView;
	typedef GIPhoneView GViewImplementation;
#elif WIN32
	class GWin32View;
	typedef GWin32View GViewImplementation;
#endif


//interface class for application tasks
class GView : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GViewImplementation> TPointerViewImplementation;

	///////////////////////////////////////////////////////
	//constructor
public:
	GView(
		TPointerViewImplementation& inout_pointerViewImplementation //WARN: take ownership
		);
	~GView();

	///////////////////////////////////////////////////////
	//public methods
public:
	TPointerViewImplementation& GetPointerViewImplementation(){ return mPointerViewImplementation; }

	///////////////////////////////////////////////////////
	//interface
private:
	TPointerViewImplementation mPointerViewImplementation;

};

#endif //#ifndef _gview_H_
