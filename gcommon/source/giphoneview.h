//
//  giphoneview.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _giphoneview_H_
#define _giphoneview_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

@class UIView;

/*
unlike GWin32View which is a base class, we are a wrapper to a ID<GIPhoneViewDelegate> view
this is to allod iphone view classes to derrive 
*/
class GIPhoneView : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:

	///////////////////////////////////////////////////////
	//constructor
public:
	GIPhoneView(
        UIView* const inout_view
		);
	~GIPhoneView();

	///////////////////////////////////////////////////////
	//public methods
public:
	UIView* const GetView(){ return mView; }
    
    const GU32 GetWidth()const;
    const GU32 GetHeight()const;

	///////////////////////////////////////////////////////
	//private members
private:
	UIView* mView; //ownership


};

#endif //#ifndef _gapplicationwindowcomponent_H_
