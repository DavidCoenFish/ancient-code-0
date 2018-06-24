//
//  gwin32view.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gwin32view_H_
#define _gwin32view_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>


//interface class for application tasks
class GWin32View : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:

	///////////////////////////////////////////////////////
	//constructor
public:
	GWin32View();
	virtual ~GWin32View();

	///////////////////////////////////////////////////////
	//public methods
public:
	void Resize(const GU32 in_width, const GU32 in_height);
	void Paint();

	const GU32 GetWidth()const{ return m_width; }
	const GU32 GetHeight()const{ return m_height; }

	///////////////////////////////////////////////////////
	//interface
private:
	virtual void OnResize(const GU32 in_width, const GU32 in_height);
	virtual void OnPaint();

private:
	GU32 m_width;
	GU32 m_height;

};

#endif //#ifndef _gwin32view_H_
